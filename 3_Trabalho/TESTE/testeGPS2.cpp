// g++ -c -o teste testeGPS.cpp -lm -lgps

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "implementacaoGPS.h"

typedef struct dadosFusao{
  // Todos dados da struct tem retorno de erro tbm
  float latitude; // graus
  float longitude; // graus
  float altitude; // metros
  float velSubida; // metros/segundo
  float velTerrest; // metros/segundo
  float timestamp; // segundos
}dadosFusao;

int main(){
  int i = 0;

dadosFusao df;

  gps_data* dataGPS;
  dataGPS = (gps_data*)malloc(sizeof(gps_data));
  initGPS(&dataGPS);

  while(i <= 10){
        leituraGPS(&dataGPS);

        df.latitude = dataGPS->fix.latitude;
        df.longitude = dataGPS->fix.longitude;
        df.altitude = dataGPS->fix.altitude;
        df.velTerrest = dataGPS->fix.speed;
        df.velSubida = dataGPS->fix.climb;
        df.timestamp = dataGPS->fix.time;

        fprintf(stderr, "\n");

        fprintf(stderr, "%f\n", df.latitude);
        fprintf(stderr, "%f\n", df.longitude);
        fprintf(stderr, "%f\n", df.altitude);
        fprintf(stderr, "%f\n", df.velTerrest);
        fprintf(stderr, "%f\n", df.velSubida);
        fprintf(stderr, "%f\n", df.timestamp);

        i++;
  }
      fprintf(stderr, "%s\n", "Teste realizado com sucesso!");
      sleep(3);
}

    killGPS(&dataGPS);

    return EXIT_SUCCESS;
  }
