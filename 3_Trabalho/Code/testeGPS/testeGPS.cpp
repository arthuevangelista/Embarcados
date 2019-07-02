#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#ifndef _IMPLEMENTACAOGPS_H_
  #include "implementacaoGPS.h"
#endif

#ifndef	__WIRING_PI_H__
  #include <wiringPi.h>
#endif

#ifndef _BUZZER_H_
  #include "buzzer.h"
#endif

#ifndef BUZZER_PIN
  #define BUZZER_PIN 7
#endif

struct sigaction act;

typedef struct dadosFusao{
  // Todos dados da struct tem retorno de erro tbm
  float latitude; // graus
  float longitude; // graus
  float altitude; // metros
  float velSubida; // metros/segundo
  float velTerrest; // metros/segundo
  float timestamp; // segundos
}dadosFusao;

// Variável global para uso do GPSD
gps_data_t* dataGPS;

// Sub-rotina para tratamento do sinal de kill
void trataSinal(int signum, siginfo_t* info, void* ptr){
  system("clear");
  fprintf(stderr, "Recebido o sinal %d.\n", signum);
  fprintf(stderr, "\nPressione qualquer tecla para continuar... ");
  getchar();
  printf("\n%s\n", "Teste realizado com sucesso!");
  sleep(3);
  killGPS(dataGPS);
  free(dataGPS);
  buzzerTone('B',100);
  buzzerTone('X',100);
  buzzerTone('B',200);
  buzzerTone('X',100);
  buzzerTone('A',200);
  buzzerTone('X',100);
  buzzerTone('B',100);
  buzzerTone('X',100);
  buzzerTone('E',100);
  buzzerTone('X',100);
  buzzerTone('D',300);
  buzzerTone('X',100);
  exit(EXIT_SUCCESS);
}

// main
int main(){
  int i = 0;

  dadosFusao df;

  // set das flags para o sinal de interrupção SIGINT = ctrl + c
  act.sa_sigaction = trataSinal;
  act.sa_flags = SA_SIGINFO; // info sobre o sinal

  // Direcionamento para o devido tratamento dos sinais
  sigaction(SIGINT, &act, NULL);

  wiringPiSetup();
  buzzerInit();

  // Aloca na main, desloca no sigaction
  dataGPS = (gps_data_t*)malloc(sizeof(gps_data_t));
  initGPS(dataGPS);

  while(1){
    leituraGPS(dataGPS);

    df.latitude = dataGPS->fix.latitude;
    df.longitude = dataGPS->fix.longitude;
    df.altitude = dataGPS->fix.altitude;
    df.velTerrest = dataGPS->fix.speed;
    df.velSubida = dataGPS->fix.climb;
    df.timestamp = dataGPS->fix.time;

    system("clear");

    fprintf(stderr, "Latitude: %f deg\n", df.latitude);
    fprintf(stderr, "Longitude: %f deg\n", df.longitude);
    fprintf(stderr, "Altitude: %f m\n", df.altitude);
    fprintf(stderr, "Velocidade Horizontal: %f m/s\n", df.velTerrest);
    fprintf(stderr, "Velocidade de Subida: %f m/s\n", df.velSubida);
    fprintf(stderr, "Marca-passo: %f segundos\n", df.timestamp);
  } // FIM DO WHILE
} // FIM DA MAIN
