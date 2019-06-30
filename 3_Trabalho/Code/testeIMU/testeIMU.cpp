#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>

#include <string.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

#ifndef _IMPLEMENTACAOIMU_H_
  #include "implementacaoIMU.h"
#endif

#ifndef _BUZZER_H_
  #include "buzzer.h"
#endif

#ifndef _RTIMULIB_H
  #include "RTIMULib.h"
#endif

#define CONTROL_BUTTON_PIN 20

#ifndef BUZZER_PIN
  #define BUZZER_PIN 26
#endif

typedef struct imuDataAngulo{
	// Dados da Fusão dos Dados
	double roll;
	double pitch;
	double yaw;
}imuDataAngulo;

typedef struct bend{
	double meiaAsaDireita;
	double meiaAsaEsquerda;
}bend;

typedef struct torsion{
	double meiaAsaDireita;
	double meiaAsaEsquerda;
}torsion;

// imu_struct[0] e imu_struct[1] são para MPU-6050
// imu_struct[2] para MPU-9250
volatile imuDataAngulo imu_struct[2];

volatile bend anguloDeFlexao;
volatile torsion anguloDeTorcao;

// =======================================================================
// thread para processamento dos dados na struct
// =======================================================================
void* procDadosDir(void* unused){
  // Calculo do ângulo de Flexão
  anguloDeFlexao.meiaAsaDireita = copysign((imu_struct[0].pitch - std::abs(imu_struct[2].pitch)), imu_struct[0].pitch);
  // Calculo do ângulo de Torção
  anguloDeTorcao.meiaAsaDireita = copysign((imu_struct[0].roll - std::abs(imu_struct[2].roll)), imu_struct[0].roll);

  return NULL;
} // FIM DA THREAD procDadosEsqDir

void* procDadosEsq(void* unused){
  // Calculo do ângulo de Flexão
  anguloDeFlexao.meiaAsaEsquerda = copysign((imu_struct[1].pitch - std::abs(imu_struct[2].pitch)), imu_struct[1].pitch);
  // Calculo do ângulo de Torção
  anguloDeTorcao.meiaAsaEsquerda = copysign((imu_struct[1].roll - std::abs(imu_struct[2].roll)), imu_struct[1].roll);

  return NULL;
} // FIM DA THREAD procDadosEsq

int main(){
  RTIMU *imu[2];

  int contadorIMU = 0;

  pthread_t processamentoDireita;
	pthread_t processamentoEsquerda;

  wiringPiSetup();
  buzzerInit();

  // Botão de controle do loop infinito
  pinMode(CONTROL_BUTTON_PIN, INPUT);

  for (contadorIMU = 0; contadorIMU < 3; contadorIMU++) {
    imu[contadorIMU] = initIMU(contadorIMU);
  }

  while (digitalRead(CONTROL_BUTTON_PIN)){
    for (contadorIMU = 0; contadorIMU < 3; contadorIMU++) {
      leituraIMU(imu[contadorIMU], contadorIMU, imu_struct[contadorIMU]);
    }

    if( pthread_create (&processamentoDireita, NULL, &procDadosDir, NULL) != 0){
      fprintf(stderr, "Erro na inicialização da thread procDadosDir na linha # %d\n", __LINE__);
      exit(EXIT_FAILURE);
    }

    if( pthread_create (&processamentoEsquerda, NULL, &procDadosEsq, NULL) != 0){
      fprintf(stderr, "Erro na inicialização da thread procDadosEsq na linha # %d\n", __LINE__);
      exit(EXIT_FAILURE);
    }

    pthread_join(processamentoDireita, NULL);
    pthread_join(processamentoEsquerda, NULL);

    fprintf(stderr, "Torção meia asa Direita: %f\n", anguloDeTorcao.meiaAsaDireita);
    fprintf(stderr, "Torção meia asa Esquerda: %f\n\n", anguloDeTorcao.meiaAsaEsquerda);

    fprintf(stderr, "Flexão meia asa Direita: %f\n", anguloDeFlexao.meiaAsaDireita);
    fprintf(stderr, "Flexão meia asa Esquerda: %f\n", anguloDeFlexao.meiaAsaEsquerda);
  }
  return 0;
}
