// g++ main.cpp -o main -lwiringPi -lpthread

// meiaAsaDireita = imu_struct[0]
// meiaAsaEsquerda = imu_struct[1]

// =======================================================================
// INICIALICAÇÃO DAS BIBLIOTECAS EM COMUM
// =======================================================================
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <wiringPi.h>
#include <sys/types.h>
#include <cmath>

#include "buzzer.h"
#include "RTIMULib.h"
#include "implementacaoIMU.h"
#include "threadProcessamento.h"

// =======================================================================
// define das constantes
// =======================================================================

// Tempo de start-up dos dois sensores
#define INIT_TIME_MPU_6050		100 // ms
#define INIT_TIME_MPU_6050		100 // ms
// Constantes úteis
#define	RTMATH_PI             3.1415926535
#define	RTMATH_DEGREE_TO_RAD  (RTMATH_PI / 180.0)
#define	RTMATH_RAD_TO_DEGREE  (180.0 / RTMATH_PI)
// Pinos utilizados
#define MPU_6050_0_PIN 27
#define MPU_6050_1_PIN 22
#define MPU_9250_PIN 17
#define CONTROL_BUTTON_PIN 20
// =======================================================================
// typedef das structs que armazenarão os dados
// =======================================================================
typedef struct MPU6050{
	// Dados da Fusão dos Dados
	double roll;
	double pitch;
	double yaw;
}MPU6050;

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
volatile MPU6050 imu_struct[2];
// volatile é utilizado para evitar otimizações excessivas do compilador
volatile bend anguloDeFlexao;
volatile torsion anguloDeTorcao;

// =======================================================================
// Declaração dos vetores dos objetos a serem instanciados
// =======================================================================

RTIMUSettings *settings[2];
RTIMU *imu[2];

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
	pthread_t processamentoDireita; int rDireita; int iDir = 0;
	pthread_t processamentoEsquerda; int rEsquerda; int iEsq = 1;

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
  initIMU("../../../Embarcados/3_Trabalho/Code/MPU6050_0", 0);
	buzzerTone('C', 100);
	buzzerTone('X', 100);
	initIMU("../../../Embarcados/3_Trabalho/Code/MPU6050_1", 1);
	buzzerTone('D', 100);
	buzzerTone('X', 100);

	// MPU9250 no 0x69 ligado | MPU6050 no 0x68 ligado e 0x69 desligado
	digitalWrite(MPU_9250_PIN, HIGH);
  digitalWrite(MPU_6050_0_PIN, LOW);
  digitalWrite(MPU_6050_1_PIN, LOW);
	usleep(INIT_TIME_MPU_9250);
  initIMU("../../../Embarcados/3_Trabalho/Code/MPU9250_2", 2);
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
					leituraIMU(contadorIMU, sampleCount, sampleRate, rateTimer, displayTimer, now);
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
		rDireita = pthread_create (&processamentoDireita, NULL, &procDados, &iDir);
		rEsquerda = pthread_create (&processamentoEsquerda, NULL, &procDados, &iEsq);
		rDireita = pthread_join(processamentoDireita, NULL);
		rEsquerda = pthread_join(processamentoEsquerda, NULL);

	} // FIM DO LOOP INFINITO

	return 0;
} // FIM DA FUNÇÃO MAIN
