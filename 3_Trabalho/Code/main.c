/* =======================================================================
 * SISTEMA DE AQUISIÇÃO DE DADOS
 * =======================================================================
 * Universidade de Brasília
 * campus Gama
 *
 * Versão: rev 2.0
 * Autor: Arthur Evangelista
 * Matrícula: 14/0016686
 *
 * Código open-source
 * =======================================================================
 * Este código implementa um sistema de aquisição de dados. Ele utiliza
 * dois sensores MPU-6050, um em cada meia asa, um sensor MPU-9250, no
 * centro da aeronave, um GPS também no centro da aeronave, um botão
 * para inicialização do sistema e um buzzer para sinalizar a condição
 * do sistema.
 *
 *  (i) O buzzer apitará um toque uma única vez para sinalizar que o
 * sistema foi inicilizado com sucesso.
 *
 * (ii) Apitará diversas vezes por alguns segundos afim de sinalizar
 * que ocorreu algum erro no sistema que deve ser analisado no terminal.
 *
 * (iii) Apitará uma única vez, por meio segundo, para sinalizar que o
 * sistema foi desligado. Neste momento, os dados serão salvos antes
 * do código sinalizar a terminação da presente sessão da raspberry.
 *
 * Para compilar este código, siga o esquemático e insira a linha abaixo
 * no terminal da raspberry pi:
 *
 * g++ main.cpp -o main -lwiringPi -lpthread
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
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <sys/types.h>

#include "RTIMULib.h"

#include "buzzer.h"
#include "implementacaoIMU.h"
#include "implementacaoGPS.h"

// =======================================================================
// define das constantes
// =======================================================================

// Tempo de start-up dos dois sensores
#define INIT_TIME_MPU_6050		100 // ms

// Pinos utilizados
#define MPU_6050_0_PIN 27
#define MPU_6050_1_PIN 22
#define MPU_9250_PIN 17
#define CONTROL_BUTTON_PIN 20
// =======================================================================
// typedef das structs que armazenarão os dados
// =======================================================================
typedef struct IMU{
	// Dados da Fusão dos Dados
	double roll;
	double pitch;
	double yaw;
}IMU;

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

// =======================================================================
// Declaração das structs dos IMUs (GLOBAL)
// =======================================================================

// imu_struct[0] e imu_struct[1] são para MPU-6050
// imu_struct[2] para MPU-9250
volatile IMU imu_struct[2];
// volatile é utilizado para evitar otimizações excessivas do compilador
volatile bend anguloDeFlexao;
volatile torsion anguloDeTorcao;

// =======================================================================
// Declaração dos vetores dos objetos a serem instanciados
// =======================================================================

volatile RTIMUSettings *settings[2];
volatile RTIMU *unit_imu[2];

// =======================================================================
// thread para processamento dos dados na struct
// =======================================================================
void* procDadosDir(void* unused){
  // Calculo do ângulo de Flexão
  anguloDeFlexao.meiaAsaDireita = copysign((imu_struct[0].pitch - std::abs(imu_struct[2].pitch)), imu_struct[0].pitch);
  // Calculo do ângulo de Torção
  anguloDeTorcao.meiaAsaDireita = copysign((imu_struct[0].roll - std::abs(imu_struct[2].roll)), imu_struct[0].roll);

  return NULL;
} // FIM DA THREAD procDadosDir

void* procDadosEsq(void* unused){
  // Calculo do ângulo de Flexão
  anguloDeFlexao.meiaAsaEsquerda = copysign((imu_struct[1].pitch - std::abs(imu_struct[2].pitch)), imu_struct[1].pitch);
  // Calculo do ângulo de Torção
  anguloDeTorcao.meiaAsaEsquerda = copysign((imu_struct[1].roll - std::abs(imu_struct[2].roll)), imu_struct[1].roll);

  return NULL;
} // FIM DA THREAD procDadosEsq

// =======================================================================
// thread para armazenamento dos dados
// =======================================================================
static pthread_mutex_t mutexLock;

void* fileHandler(void* dados){
	// Sub-rotina dedicada a apenas armazenar o valor enviado para thread
	// em um arquivo. A chave MUTEX será utilizada para que:
	//
	// i) Os próximos dados podem estar prontos antes que a thread tenha
	// salvo os dados da iteração anterior; e
	// ii) A próxima thread pode ser chamada antes que a thread atual tenha
	// salvo os dados.
	//
	// Em outras palavras, utilizaremos a chave MUTEX para evitar as condições
	// de corrida crítica citadas acima.

	// Pra salvar o arquivo com um nome customizado toda vez que houver aquisição
	char buffer[50];
	snprintf(buffer, sizeof(char)*50, "/temp/resultados_%s_%s.txt", __DATE__, __TIME__);

	FILE *fp = fopen(buffer, "a");

	if (fp == NULL){
		// Se não for possível abrir o arquivo, EXIT_FAILURE
		fprintf(stderr, "Não foi possível realizar a abertura do arquivo %s na linha # %d\n", __FILE__,__LINE__);
		exit(EXIT_FAILURE);
	}else{
		// Caso tenha sido possível realizar a abertura do arquivo, locka a chave
		// MUTEX e armazena os dados no arquivo
		pthread_mutex_lock(&mutexLock);

		fputs("%f\t\t%f\t\t%f\t\t%f" anguloDeFlexao.meiaAsaDireita, anguloDeFlexao.meiaAsaEsquerda, anguloDeTorcao.meiaAsaDireita, anguloDeTorcao.meiaAsaEsquerda);
		fputs("\t\t%f\t\t%f\t\t%f", imu_struct[2].roll, imu_struct[2].pitch, imu_struct[2].yaw);
		// fputs("\t\t%f\t\t%f\t\t%f\t\t%f", velocidade, posicaoX, posicaoY, posicaoZ);
		fputs("\n");

		pthread_mutex_unlock(&mutexLock);
	} // FIM DA CONDICIONAL IF-ELSE
	fclose(fp);
	return NULL;
} // FIM DA THREAD fileHandler

// =======================================================================
// Função de inicialização do GPS
// =======================================================================
 //void initGPS(){

//} //	FIM INIT GPS

// =======================================================================
// Função principal main
// =======================================================================
int main (){
	// Declaração das variáveis do temporizador de amostragem
	int sampleCount;
	int sampleRate;
	uint64_t rateTimer;
	uint64_t displayTimer;
	uint64_t now;

	// Variável para contar o IMU a ser lido/processado para manter a ordem
	int contadorIMU;
	int processadorIMU;

	// Declaração das threads a serem usadas para processamento de dados
	pthread_t processamentoDireita;
	pthread_t processamentoEsquerda;
	// pthread_t threadKalmanFusion;
	pthread_t threadFileHandler;

	// Setup da lib wiringPi para uso do GPIO
  wiringPiSetup();

	// Setup do buzzer
	buzzerInit();

  // Definindo os Endereços dos IMU para inicialização
  pinMode(MPU_9250_PIN, OUTPUT); // MPU9250 (I2C pra redução de gastos)
  pinMode(MPU_6050_0_PIN, OUTPUT); // MPU6050 0
  pinMode(MPU_6050_1_PIN, OUTPUT); // MPU6050 1
	pinMode(CONTROL_BUTTON_PIN, INPUT); // Botão de controle do loop infinito

// =======================================================================
// INICIALIZAÇÃO DOS SENSORES IMU
// =======================================================================

	// MPU9250 desligado | MPU6050 no 0x68 e 0x69 ligados
  digitalWrite(MPU_9250_PIN, LOW);
  digitalWrite(MPU_6050_0_PIN, LOW);
  digitalWrite(MPU_6050_1_PIN, HIGH);
	usleep(INIT_TIME_MPU_6050);
  initIMU("../../../Embarcados/3_Trabalho/Code/MPU6050_0", 0, settings, unit_imu);
	buzzerTone('C', 100);
	buzzerTone('X', 100);
	initIMU("../../../Embarcados/3_Trabalho/Code/MPU6050_1", 1, settings, unit_imu);
	buzzerTone('D', 100);
	buzzerTone('X', 100);

	// MPU9250 no 0x69 ligado | MPU6050 no 0x68 ligado e 0x69 desligado
	digitalWrite(MPU_9250_PIN, HIGH);
  digitalWrite(MPU_6050_0_PIN, LOW);
  digitalWrite(MPU_6050_1_PIN, LOW);
	usleep(INIT_TIME_MPU_9250);
  initIMU("../../../Embarcados/3_Trabalho/Code/MPU9250_2", 2, settings, unit_imu);
	buzzerTone('E', 100);
	buzzerTone('X', 100);

	// set up do temporizador de amostragem
	rateTimer = displayTimer = RTMath::currentUSecsSinceEpoch();

// =======================================================================
// INICIALIZAÇÃO DO GPS
// =======================================================================
//  initGPS();

	// Loop infinito
	while(digitalRead(CONTROL_BUTTON_PIN)){
		contadorIMU = 0;
		processadorIMU = 0;
		digitalWrite(MPU_9250_PIN, LOW);
		digitalWrite(MPU_6050_0_PIN, LOW);
		digitalWrite(MPU_6050_1_PIN, HIGH);
		usleep(INIT_TIME_MPU_6050);

		// =======================================================================
		// Laço de repetição para leitura dos sensores
		// =======================================================================
		while (contadorIMU > 3) {
			switch (contadorIMU) {
				case 0:
				case 1:
					leituraIMU(contadorIMU, sampleCount, sampleRate, rateTimer, displayTimer, now, unit_imu, imu_struct);
					break;
				case 2:
					digitalWrite(MPU_9250_PIN, HIGH);
					digitalWrite(MPU_6050_0_PIN, LOW);
					digitalWrite(MPU_6050_1_PIN, LOW);
					usleep(INIT_TIME_MPU_9250);
					leituraIMU(contadorIMU, sampleCount, sampleRate, rateTimer, displayTimer, now);
					break;
				default:
					break;
			} // FIM SWITCH CASE
			contadorIMU++;
		} // FIM WHILE CONTADOR

		// =======================================================================
		// thread para processamento dos dados na struct
		// =======================================================================
		if( pthread_create (&processamentoDireita, NULL, &procDadosDir, NULL) != 0){
			fprintf(stderr, "Erro na inicialização da thread procDadosDir na linha # %d\n", __LINE__);
			exit(EXIT_FAILURE);
		}
		if( pthread_create (&processamentoEsquerda, NULL, &procDadosEsq, NULL) != 0){
			fprintf(stderr, "Erro na inicialização da thread procDadosEsq na linha # %d\n", __LINE__);
			exit(EXIT_FAILURE);
		}
		// pthread_create (&threadKalmanFusion, NULL, &kalmanFusion, NULL);
		pthread_join(processamentoDireita, NULL);
		pthread_join(processamentoEsquerda, NULL);
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
	pthread_mutex_destroy(&mutexLock);

	// =======================================================================
	// Pós-processamento dos dados (FFT) e plot dos gráficos
	// =======================================================================

	return 0;
} // FIM DA FUNÇÃO MAIN
