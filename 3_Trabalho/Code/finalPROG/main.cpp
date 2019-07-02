/* =======================================================================
 * SISTEMA DE AQUISIÇÃO DE DADOS
 * =======================================================================
 * Universidade de Brasília
 * campus Gama
 *
 * Versão: rev 3.6
 * Autor: Arthur Evangelista
 * Matrícula: 14/0016686
 *
 * Código open-source
 * =======================================================================
 * Falta:
 * - Alterar o makefile
 * - Testar com todos os componentes
 * - Alterar fileHandler
 * - Implementar FFT
 * - Implementar GNU Plot
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
 * =======================================================================
 * Para rápida referência, as structs estão enumeradas da seguinte forma:
 *
 * meiaAsaDireita = imu_struct[0]
 * meiaAsaEsquerda = imu_struct[1]
 * Aviao = imu_struct[2]
 * =======================================================================
 */

// =======================================================================
// INICIALICAÇÃO DAS BIBLIOTECAS EM COMUM
// =======================================================================
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

// =======================================================================
// define das constantes
// =======================================================================

// Pinos utilizados
#define CONTROL_BUTTON_PIN 20

#ifndef BUZZER_PIN
  #define BUZZER_PIN 7
#endif

// =======================================================================
// typedef das structs que armazenarão os dados
// =======================================================================
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
    float atitude;
    float alpha;
    // Dados do GPS
    float latitude; // graus
    float longitude; // graus
    float altitude; // metros
    float velSubida; // metros/segundo
    float velTerrest; // metros/segundo
    float timestamp; // segundos
  }aeronave;

// =======================================================================
// Variáveis Globais
// =======================================================================
  // volatile é utilizado para evitar otimizações excessivas do compilador
  // struct para TODOS os dados da aeronave
  volatile aeronave uav;
  // struct para uso do sinal de interrupção
  struct sigaction act;
  // Variável global para uso do GPSD
  gps_data_t* dataGPS;
  // Chave mutex para armazenamento dos dados
  static pthread_mutex_t mutexLock;
  // Declaração das threads a serem usadas
  pthread_t threadGPS;
  pthread_t processamentoDireita;
  pthread_t processamentoEsquerda;
  pthread_t threadFileHandler;

// =======================================================================
// Sub-rotina para tratamento do sinal de interrupção
// =======================================================================
void trataSinal(int signum, siginfo_t* info, void* ptr){
  switch (signum) {
    case SIGINT:
      system("clear");
      fprintf(stderr, "Recebido o sinal de interrupção [%d].\n", signum);
      printf("\n%s\n", "Teste realizado com sucesso!");
      sleep(3);

      // Encerramento do GPS
      killGPS(dataGPS);
      free(dataGPS);
      pthread_cancel(threadGPS);

      // Encerramento do fileHandler
      pthread_cancel(fileHandler);

      // Encerramento dos processamentos
      pthread_cancel(processamentoDireita);
      pthread_cancel(processamentoEsquerda);

      // Destruindo chave mutex
      pthread_mutex_destroy(&mutexLock);
      // Toque do buzzer
      buzzerTone('B',200);
      buzzerTone('B',200);
      buzzerTone('A',100);
      buzzerTone('B',200);
      buzzerTone('E',200);
      buzzerTone('D',200);
      buzzerTone('D',50);
      buzzerTone('X',100);
      exit(EXIT_SUCCESS);
      break;
    case SIGUSR1:
      // Salva os dados do dataGPS->fix.etc no uav para fusão
      pthread_mutex_lock(&mutexLock);
      uav.latitude = dataGPS->fix.latitude;
      uav.longitude = dataGPS->fix.longitude;
      uav.altitude = dataGPS->fix.altitude;
      uav.velTerrest = dataGPS->fix.speed;
      uav.velSubida = dataGPS->fix.climb;
      uav.timestamp = dataGPS->fix.time;
      pthread_mutex_unlock(&mutexLock);
      break;
    default:
    break;
  } // FIM DO SWITCH-CASE
} // FIM DA SUBROTINA trataSinal

// =======================================================================
// thread para processamento dos dados na struct
// =======================================================================
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

  uav.anguloDeFlexao.meiaAsaDireita = copysign((pitch0 - std::abs(pitch2)), pitch0);
  uav.anguloDeTorcao.meiaAsaDireita = copysign((roll0 - std::abs(roll2)), roll0);

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

  uav.anguloDeFlexao.meiaAsaEsquerda = copysign((pitch1 - std::abs(pitch2)), pitch1);
  uav.anguloDeTorcao.meiaAsaEsquerda = copysign((roll1 - std::abs(roll2)), roll1);

  return NULL;
} // FIM DA THREAD procDadosEsq

// =======================================================================
// thread para armazenamento dos dados
// =======================================================================
void* fileHandler(void* dados){
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
	char buffer[50];
	snprintf(buffer, sizeof(char)*50, "home/pi/Embarcados/3_Trabalho/Code/Resultados/dados_%s_%s.txt", __DATE__, __TIME__);

	FILE *fp = fopen(buffer, "a");

	if (fp == NULL){
		// Se não for possível abrir o arquivo, EXIT_FAILURE
		fprintf(stderr, "Não foi possível realizar a abertura do arquivo %s na linha # %d\n", __FILE__,__LINE__);
		exit(EXIT_FAILURE);
	}else{
		// Caso tenha sido possível realizar a abertura do arquivo, locka a chave
		// MUTEX e armazena os dados no arquivo
		pthread_mutex_lock(&mutexLock);

    // Apresentação dos dados no terminal
    system("clear");
    fprintf(stderr, "Latitude: %f deg\n", dataGPS->fix.latitude);
    fprintf(stderr, "Longitude: %f deg\n", dataGPS->fix.longitude);
    fprintf(stderr, "Altitude: %f m\n", dataGPS->fix.altitude);
    fprintf(stderr, "Velocidade Horizontal: %f m/s\n", dataGPS->fix.speed);
    fprintf(stderr, "Velocidade de Subida: %f m/s\n", dataGPS->fix.climb);
    fprintf(stderr, "Marca-passo: %f segundos\n\n", dataGPS->fix.time);
    fprintf(stderr, "Torcao meia asa Direita: %f\n", uav.anguloDeTorcao.meiaAsaDireita);
    fprintf(stderr, "Torcao meia asa Esquerda: %f\n\n", uav.anguloDeTorcao.meiaAsaEsquerda);
    fprintf(stderr, "Flexao meia asa Direita: %f\n", uav.anguloDeFlexao.meiaAsaDireita);
    fprintf(stderr, "Flexao meia asa Esquerda: %f\n", uav.anguloDeFlexao.meiaAsaEsquerda);

		fputs("%f\t\t%f\t\t%f\t\t%f\n" uav.anguloDeFlexao.meiaAsaDireita, uav.anguloDeFlexao.meiaAsaEsquerda, uav.anguloDeTorcao.meiaAsaDireita, uav.anguloDeTorcao.meiaAsaEsquerda);
		fputs("\t\t%f\t\t%f\t\t%f\n", imu_struct[2].roll, imu_struct[2].pitch, imu_struct[2].yaw);
		/* fputs("\t\t%f\t\t%f\t\t%f\t\t%f", velocidade, posicaoX, posicaoY, posicaoZ); */
		fputs("\n");

		pthread_mutex_unlock(&mutexLock);
	} // FIM DA CONDICIONAL IF-ELSE
	fclose(fp);
	return NULL;
} // FIM DA THREAD fileHandler

// =======================================================================
// Função de inicialização do GPS
// =======================================================================
void* threadGPS(void* param){
  /* Nesta thread, quando o GPS receber um 3D FIX, os dados irão ser
   * utilizados para a Fusão dos dados com a unidade IMU no centro da
   * aeronave. Irá ocorrer uma interrupção que será tratada pelo SIGUSR1
   */

  dataGPS = (gps_data_t*)malloc(sizeof(gps_data));
  initGPS(dataGPS);

  while (1) {
    usleep(750000); // A cada 3/4 de segundo, verifica GPS
    pthread_mutex_lock(&mutexLock);
    leituraGPS(dataGPS);
    /* Se foi recebido um 3D FIX, envia SIGUSR1 para a thread principal.
     * Como todas as threads possuem o mesmo pid (retornado por getpid)
     * Podemos utilizar a chamada getpid() que qualquer thread que receber
     * o sinal poderá tratá-lo.
     *
     * Caso isso não funcione, utilizar gettid()
     * na mainThread e passar isto como (void*)param para esta thread!
     */
    if ((dataGPS->status == STATUS_FIX) && (dataGPS->fix.mode == MODE_3D)){
      pthread_mutex_unlock(&mutexLock);
      sigqueue(getpid(),SIGUSR1,NULL);
    }else{
      pthread_mutex_unlock(&mutexLock);
    } // FIM DO IF-ELSE 3D FIX
  } // FIM DO LOOP-INFINITO DA THREAD DO GPS

} //	FIM DA THREAD threadGPS

// =======================================================================
// Função principal main
// =======================================================================
int main (){
  // Ponteiro para classe imu
	RTIMU *imu[2];

	// Variável para contar o IMU a ser lido/processado para manter a ordem
	int contadorIMU = 0;

  // imu_struct [0] e imu_struct++ [1] são para MPU-6050
  // imu_struct ++ ++ [2] para MPU-9250
  imuDataAngulo* imu_struct;
  imu_struct = (imuDataAngulo*)malloc(sizeof(imuDataAngulo)*3);

  // set das flags para o sinal de interrupção SIGINT = ctrl + c
  act.sa_sigaction = trataSinal;
  act.sa_flags = SA_SIGINFO; // info sobre o sinal

  // Direcionamento para o devido tratamento dos sinais
  sigaction(SIGINT | SIGUSR1, &act, NULL);

	// Setup da lib wiringPi para uso do GPIO
  wiringPiSetup();

	// Setup do buzzer
	buzzerInit();

  // Botão de controle do loop infinito
	pinMode(CONTROL_BUTTON_PIN, INPUT);

// =======================================================================
// INICIALIZAÇÃO DOS SENSORES IMU
// =======================================================================
  for (contadorIMU = 0; contadorIMU < 3; contadorIMU++) {
		// Aqui cabe uma otimização fazendo initIMU(contadorIMU, imu)
		// e alterando a implementação do IMU
  	imu[contadorIMU] = initIMU(contadorIMU);
  }

// =======================================================================
// INICIALIZAÇÃO DO GPS
// =======================================================================
  if(pthread_create(&threadGPS, NULL, &threadGPS, NULL) != 0){
    fprintf(stderr, "Erro na inicialização da thread do GPS na linha # %d\n", __LINE__);
    exit(EXIT_FAILURE);
  }

	// Loop infinito
	while(/*digitalRead(CONTROL_BUTTON_PIN)*/1){
		// =======================================================================
		// Laço de repetição para leitura dos sensores
		// =======================================================================
		for (contadorIMU = 0; contadorIMU < 3; contadorIMU++) {
			leituraIMU(imu[contadorIMU], imu_struct);
      imu_struct ++;
		}
    imu_struct -= 2;

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

		// =======================================================================
		// thread para processamento dos dados na struct
		// =======================================================================
		if( pthread_create(&threadFileHandler, NULL, &fileHandler, NULL) != 0){
			fprintf(stderr, "Erro na inicialização da thread fileHandler na linha # %d\n", __LINE__);
			exit(EXIT_FAILURE);
		}
		pthread_join(threadFileHandler, NULL);

	} // FIM DO LOOP INFINITO

	// =======================================================================
	// Pós-processamento dos dados (FFT) e plot dos gráficos
	// =======================================================================

  // Ao final de tudo, a própria main executa o SIGINT
	sigqueue(getpid(),SIGINT,NULL);
} // FIM DA FUNÇÃO MAIN
