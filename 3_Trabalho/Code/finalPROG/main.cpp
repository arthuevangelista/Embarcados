/* =====================================================================
 * SISTEMA DE AQUISIÇÃO DE DADOS
 * =====================================================================
 * Universidade de Brasília
 * campus Gama
 *
 * Versão: rev 3.9
 * Autor: Arthur Evangelista
 * Matrícula: 14/0016686
 *
 * Código open-source
 * =====================================================================
 * Falta:
 * - Implementar FFT:
 *    - Precisa do timestamp (GPS) e do usecs since last epoch (IMU)?;
 * - Alterar fileHandler:
 *       - Tamanho do arquivo é o tamanho do vetor => EOF = fim do vetor
 *       - while(feof(file_pointer)){ length++ } malloc(sizeof()*length)
 *    - Inicializar a tabela na main para a thread apenas add os dados
 *    - Primeira linha deve conter as definições de cada coluna
 * - Implementar GNU Plot
 *    - Plottar FFT
 *    - Plottar PSD (power spectral density = fft/freq)
 *    - Plottar Espectograma (FFT pelo tempo)
 *    - Plottar trajetória 2D (precisa de kalman)
 *    - Plottar trajetória 3D (kalman)
 * - EXTRA: Usar gtk+ pra fazer a GUI
 * Este código implementa um sistema de aquisição de dados. Ele utiliza
 * dois sensores MPU-6050, um em cada meia asa, um sensor MPU-9250, no
 * centro da aeronave, um GPS também no centro da aeronave, um botão
 * para inicialização do sistema e um buzzer para sinalizar a condição
 * do sistema.
 *
 *  (i) O buzzer apitará algumas vezes para sinalizar que o
 * sistema e seus módulos foram inicializados com sucesso.
 *
 * (ii) Tocará uma pequena música para sinalizar que o
 * sistema foi desligado. Neste momento, os dados serão salvos antes
 * do código sinalizar a terminação da presente sessão da raspberry.
 *
 * Para compilar este código, siga o esquemático, entre na pasta do
 * projeto e digite os seguintes comandos:
 *
 * make -j4
 * sudo make install
 * sudo finalPROG
 *
 * =====================================================================
 * Para rápida referência, as structs são enumeradas da seguinte forma:
 *
 * meiaAsaDireita = imu_struct[0]
 * meiaAsaEsquerda = imu_struct[1]
 * Aviao = imu_struct[2]
 * =====================================================================
 */

// =====================================================================
// INICIALICAÇÃO DAS BIBLIOTECAS EM COMUM
// =====================================================================
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>

#ifndef _IMPLEMENTACAOGPS_H_
  #include "implementacaoGPS.h"
#endif

#ifndef	__WIRING_PI_H__
  #include <wiringPi.h>
  #include <wiringPiI2C.h>
#endif

#ifndef _RTIMULIB_H
  #include "RTIMULib.h"
#endif

#ifndef _IMPLEMENTACAOIMU_H_
  #include "implementacaoIMU.h"
#endif

#ifndef _BUZZER_H_
  #include "buzzer.h"
#endif

// =====================================================================
// define das constantes
// =====================================================================

// Pinos utilizados
#define CONTROL_BUTTON_PIN 11
#define ADDR_PIN 18

#ifndef BUZZER_PIN
  #define BUZZER_PIN 7
#endif

// =====================================================================
// typedef das structs que armazenarão os dados
// =====================================================================
  typedef struct bend{
    double meiaAsaDireita;
    double meiaAsaEsquerda;
  }bend;

  typedef struct torsion{
    double meiaAsaDireita;
    double meiaAsaEsquerda;
  }torsion;

  typedef struct aeronave{
    // Dados aeroelásticos
    bend anguloDeFlexao;
    torsion anguloDeTorcao;
    // Dados desempenho
    float alpha;
    float atitude;
    float roll;
    float pitch;
    float yaw;
    // Dados do GPS
    float latitude; // graus
    float longitude; // graus
    float altitude; // metros
    float velSubida; // metros/segundo
    float velTerrest; // metros/segundo
    float timestamp; // segundos
    int fixmode; // auto-explicativo
  }aeronave;

// =====================================================================
// Variáveis Globais
// =====================================================================
  // volatile é utilizado para evitar otimizações do compilador
  // struct para TODOS os dados da aeronave
  volatile aeronave uav;
  // struct para uso do sinal de interrupção
  struct sigaction act;
  union sigval value;
  // Variável global para uso do GPSD
  gps_data_t* dataGPS;
  // Chaves mutex para armazenamento dos dados
  static pthread_mutex_t mutexGPS;
  static pthread_mutex_t mutexUAV;
  // Declaração das threads a serem usadas
  pthread_t pthreadGPS;
  pthread_t processamentoDireita;
  pthread_t processamentoEsquerda;

// =====================================================================
// Sub-rotina para tratamento do sinal de interrupção
// =====================================================================
void trataSinal(int signum, siginfo_t* info, void* ptr){
    system("clear");
    fprintf(stderr, "Recebido o sinal de interrupção [%d].\n", signum);
    printf("\n%s\n", "Teste realizado com sucesso!");
    // Encerramento dos processamentos
    pthread_cancel(processamentoDireita);
    pthread_cancel(processamentoEsquerda);
    // Encerramento do GPS
    pthread_cancel(pthreadGPS);
    killGPS(dataGPS);
    free(dataGPS);
    // Destruindo chave mutex
    pthread_mutex_destroy(&mutexGPS);
    pthread_mutex_destroy(&mutexUAV);
    // Toque do buzzer
    buzzerTone('B',100);
    buzzerTone('A',100);
    buzzerTone('G',100);
    buzzerTone('F',100);
    buzzerTone('E',100);
    buzzerTone('D',100);
    buzzerTone('C',200);
    buzzerTone('X',100);
    sleep(1);
    exit(EXIT_SUCCESS);
} // FIM DA SUBROTINA trataSinal

// =====================================================================
// thread para processamento dos dados na struct
// =====================================================================
void* procDadosDir(void* unused){
  imuDataAngulo* imu_struct;
  imu_struct = (imuDataAngulo*)malloc(sizeof(imuDataAngulo)*3);
  imu_struct = (imuDataAngulo *) unused;

  double pitch0 = imu_struct->pitch;
  double roll0 = imu_struct->roll;

  imu_struct++;
  imu_struct++;

  double pitch2 = imu_struct->pitch;
  double roll2 = imu_struct->roll;

  pthread_mutex_lock(&mutexUAV);
  uav.anguloDeFlexao.meiaAsaDireita = copysign((pitch0 - std::abs(pitch2)), pitch0);
  uav.anguloDeTorcao.meiaAsaDireita = copysign((roll0 - std::abs(roll2)), roll0);
  pthread_mutex_unlock(&mutexUAV);

  imu_struct--;
  imu_struct--;

  return NULL;
} // FIM DA THREAD procDadosEsqDir

void* procDadosEsq(void* unused){
  imuDataAngulo* imu_struct;
  imu_struct = (imuDataAngulo*)malloc(sizeof(imuDataAngulo)*3);
  imu_struct = (imuDataAngulo *) unused;

  imu_struct++;

  double pitch1 = imu_struct->pitch;
  double roll1 = imu_struct->roll;

  imu_struct++;
  double pitch2 = imu_struct->pitch;
  double roll2 = imu_struct->roll;

  pthread_mutex_lock(&mutexUAV);
  uav.anguloDeFlexao.meiaAsaEsquerda = copysign((pitch1 - std::abs(pitch2)), pitch1);
  uav.anguloDeTorcao.meiaAsaEsquerda = copysign((roll1 - std::abs(roll2)), roll1);
  pthread_mutex_unlock(&mutexUAV);

  imu_struct--;
  imu_struct--;

  return NULL;
} // FIM DA THREAD procDadosEsq

// =====================================================================
// thread para armazenamento dos dados
// =====================================================================
int fileHandler(int i){
	/* Sub-rotina dedicada a apenas armazenar o valor enviado para thread
	* em um arquivo. A chave MUTEX será utilizada para que:
	*
	* i) Os próximos dados podem estar prontos antes que a thread tenha
	* salvo os dados da iteração anterior; e
	* ii) A próxima thread pode ser chamada antes que a thread atual tenha
	* salvo os dados.
	*
	* Em outras palavras, utilizaremos a chave MUTEX para evitar as condições
	* de corrida crítica citadas acima.
        */

	// Pra salvar o arquivo com um nome customizado toda vez que houver aquisição
	char buffer[100]; // Nome do arquivo
	char scr[128]; // Para printar timestamp do GPS
	snprintf(buffer, sizeof(char)*100, "/home/pi/Embarcados/3_Trabalho/Code/Resultados/dados_%s.txt", __TIME__);
	FILE *fp = fopen(buffer, "a+");

	if (fp == NULL){
		// Se não for possível abrir o arquivo, EXIT_FAILURE
		fprintf(stderr, "Não foi possível realizar a abertura do arquivo [dados_%s.txt] na linha # %d.\n", __TIME__,__LINE__);
		exit(EXIT_FAILURE);
	}else{
		// Caso tenha sido possível realizar a abertura do arquivo, locka a chave
		// MUTEX e armazena os dados no arquivo
		pthread_mutex_lock(&mutexUAV);

    // Apresentação dos dados no terminal
    system("clear");
    fprintf(stderr, "Latitude: %f deg\n", uav.latitude);
    fprintf(stderr, "Longitude: %f deg\n", uav.longitude);
    fprintf(stderr, "Altitude: %f m\n", uav.altitude);
    fprintf(stderr, "Velocidade Horizontal: %f m/s\n", uav.velTerrest);
    fprintf(stderr, "Velocidade de Subida: %f m/s\n", uav.velSubida);
    unix_to_iso8601(uav.timestamp, scr, sizeof(scr));
    fprintf(stderr, "Tempo: %s\n\n", scr);
    fprintf(stderr, "FIX MODE: %d\n\n", uav.fixmode);
    fprintf(stderr, "Roll: %f deg\n", uav.roll);
    fprintf(stderr, "Pitch: %f deg\n", uav.pitch);
    fprintf(stderr, "Yaw: %f deg\n\n", uav.yaw);
    fprintf(stderr, "Torcao meia asa Direita: %f\n", uav.anguloDeTorcao.meiaAsaDireita);
    fprintf(stderr, "Torcao meia asa Esquerda: %f\n\n", uav.anguloDeTorcao.meiaAsaEsquerda);
    fprintf(stderr, "Flexao meia asa Direita: %f\n", uav.anguloDeFlexao.meiaAsaDireita);
    fprintf(stderr, "Flexao meia asa Esquerda: %f\n", uav.anguloDeFlexao.meiaAsaEsquerda);

		fprintf(fp, "%f\t\t%f\t\t%f\t\t%f", uav.anguloDeFlexao.meiaAsaDireita, uav.anguloDeFlexao.meiaAsaEsquerda, uav.anguloDeTorcao.meiaAsaDireita, uav.anguloDeTorcao.meiaAsaEsquerda);
		fprintf(fp, "\t\t%f\t\t%f\t\t%f", uav.roll, uav.pitch, uav.yaw);
		/* fprintf(fp, "\t\t%f\t\t%f\t\t%f\t\t%f", velocidade, posicaoX, posicaoY, posicaoZ); */
		fprintf(fp, "\n"); i++;

		pthread_mutex_unlock(&mutexUAV);
	} // FIM DA CONDICIONAL IF-ELSE
	fclose(fp);
	return i;
} // FIM DA FUNÇÃO fileHandler

// =====================================================================
// Função de inicialização do GPS
// =====================================================================
void* threadGPS(void* param){
  /* Nesta thread, quando o GPS receber um 3D FIX, os dados irão ser
   * utilizados para a Fusão dos dados com a unidade IMU no centro da
   * aeronave. Irá ocorrer uma interrupção que será tratada pelo SIGUSR1
   */

  dataGPS = (gps_data_t*)malloc(sizeof(gps_data_t));
  initGPS(dataGPS);

  while (1) {
    usleep(750000); // A cada 3/4 de segundo (1.33 Hz), verifica GPS
    pthread_mutex_lock(&mutexGPS);
    leituraGPS(dataGPS);
    if ((dataGPS->status == STATUS_FIX) && (dataGPS->fix.mode == MODE_3D)){
        // Salva os dados do dataGPS->fix.etc no uav para fusão
	pthread_mutex_lock(&mutexUAV);
	uav.latitude = dataGPS->fix.latitude;
	uav.longitude = dataGPS->fix.longitude;
	uav.altitude = dataGPS->fix.altitude;
	uav.velTerrest = dataGPS->fix.speed;
	uav.velSubida = dataGPS->fix.climb;
	uav.timestamp = dataGPS->fix.time;
	uav.fixmode = dataGPS->fix.mode;
	pthread_mutex_unlock(&mutexUAV);
	pthread_mutex_unlock(&mutexGPS);
    }else{
      pthread_mutex_unlock(&mutexGPS);
    } // FIM DO IF-ELSE 3D FIX
  } // FIM DO LOOP-INFINITO DA THREAD DO GPS

} //	FIM DA THREAD threadGPS

// =====================================================================
// Função principal main
// =====================================================================
int main (){
  // Ponteiro para classe imu
	RTIMU *imu[3];

	// Variável para contar o IMU a ser lido/processado para manter a ordem
	int contadorIMU = 0; int tamanhoFFT;

  // imu_struct [0] e imu_struct++ [1] são para MPU-6050
  // imu_struct ++ ++ [2] para MPU-9250
  imuDataAngulo* imu_struct;
  imu_struct = (imuDataAngulo*)malloc(sizeof(imuDataAngulo)*3);

  // set das flags para o sinal de interrupção SIGINT = ctrl + c
  act.sa_sigaction = trataSinal;
  act.sa_flags = SA_SIGINFO; // info sobre o sinal

  // Direcionamento para o devido tratamento dos sinais
  sigaction(SIGINT, &act, NULL);

	// Setup da lib wiringPi para uso do GPIO
  wiringPiSetup();

	// Setup do buzzer
	buzzerInit();

  // Botão de controle do loop infinito
	pinMode(CONTROL_BUTTON_PIN, INPUT);
  pinMode(ADDR_PIN, OUTPUT);
  digitalWrite(ADDR_PIN, HIGH);

// =====================================================================
// INICIALIZAÇÃO DOS SENSORES IMU
// =====================================================================
  while (contadorIMU < 3) {
		// Aqui cabe uma otimização fazendo initIMU(contadorIMU, imu)
		// e alterando a implementação do IMU
  	imu[contadorIMU] = initIMU(contadorIMU);
	contadorIMU++;
  }
  
  // Zera o contador
  contadorIMU = 0;

// =====================================================================
// INICIALIZAÇÃO DO GPS
// =====================================================================
  if(pthread_create(&pthreadGPS, NULL, &threadGPS, NULL) != 0){
    fprintf(stderr, "Erro na inicialização da thread do GPS na linha # %d\n", __LINE__);
    exit(EXIT_FAILURE);
  }
// =====================================================================
// INICILIZAÇÃO DO ARQUIVO
// =====================================================================
  char buffer[100]; // Nome do arquivo
  snprintf(buffer, sizeof(char)*100, "/home/pi/Embarcados/3_Trabalho/Code/Resultados/dados_%s.txt", __TIME__);
  FILE *fp = fopen(buffer, "w");
  if (fp == NULL){
    fprintf(stderr, "Não foi possível realizar a abertura do arquivo [dados_%s.txt] na linha # %d.\n", __TIME__,__LINE__);
    exit(EXIT_FAILURE);
  }else{
    fprintf(fp,"Flexao Direita\t\tFlexao Esquerda\t\t");
    fprintf(fp,"Torcao Direita\t\tTorcao Esquerda\t\t");
    fprintf(fp,"Roll\t\t\tPitch\t\t\tYaw\n");
  }

	// Loop infinito
	while(digitalRead(CONTROL_BUTTON_PIN)/*1*/){
		// =======================================================================
		// Laço de repetição para leitura dos sensores
		// =======================================================================
		contadorIMU = 0;
		while (contadorIMU < 3) {
			leituraIMU(imu[contadorIMU], imu_struct);
			if(contadorIMU == 2){
			  pthread_mutex_lock(&mutexUAV);
			  uav.roll = imu_struct->roll;
			  uav.pitch = imu_struct->pitch;
			  uav.yaw = imu_struct->yaw;
			  pthread_mutex_unlock(&mutexUAV);
			}
			contadorIMU++;
			imu_struct ++;
		}
		imu_struct--;
		imu_struct--;
		imu_struct--;

		// =======================================================================
		// thread para processamento dos dados na struct
		// =======================================================================
		if( pthread_create (&processamentoDireita, NULL, &procDadosDir, (void*) imu_struct) != 0){
			fprintf(stderr, "Erro na inicialização da thread procDadosDir na linha # %d\n", __LINE__);
			exit(EXIT_FAILURE);
		}
		if( pthread_create (&processamentoEsquerda, NULL, &procDadosEsq, (void*) imu_struct) != 0){
			fprintf(stderr, "Erro na inicialização da thread procDadosEsq na linha # %d\n", __LINE__);
			exit(EXIT_FAILURE);
		}
		pthread_join(processamentoDireita, NULL);
		pthread_join(processamentoEsquerda, NULL);
		// pthread_create (&threadKalmanFusion, NULL, &kalmanFusion, NULL);
		// pthread_join(threadKalmanFusion, NULL);

		// Chama função para guardar e printar os dados
    tamanhoFFT = fileHandler(tamanhoFFT);
	} // FIM DO LOOP INFINITO

	// =======================================================================
	// Pós-processamento dos dados (FFT) e plot dos gráficos
	// =======================================================================

	// Ao final de tudo, a própria main executa o SIGINT
	sigqueue(getpid(),SIGINT,value);
	return 0;
} // FIM DA FUNÇÃO MAIN
