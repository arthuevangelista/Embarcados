/* =======================================================================
 * Pacote de implementação das funções do GPS
 * =======================================================================
 * Devem ser adicionadas as flags a seguir no momento da compilação:
 * gps -lm -lgps
 *
 * Exemplo:
 * g++ -o gps filename.cpp -lm -lgps
 * =======================================================================
 */

#include "implementacaoGPS.h"

// =======================================================================
// Função de inicialização do GPS
// =======================================================================
gps_data* initGPS(){
  int rc;
  gps_data* dataGPS;

  printf("Inicializando GPS ...\n");
  if((rc = gps_open(source.server, source.port, dataGPS)) == -1){
    fprintf(stderr, "Erro na inicialização do GPS na linha # %d\nRazão do erro: %s\n", __LINE__, gps_errstr(rc));
    exit(EXIT_FAILURE);
  }
  gps_stream(dataGPS, WATCH_ENABLE | WATCH_JSON, NULL);

  return dataGPS;
} // FIM DA FUNÇÃO initGPS

// =======================================================================
// Função de leitura dos dados do GPS
// =======================================================================
void leituraGPS(gps_data* dataGPS){
  /* Espera 2 segundos para receber os dados do GPS */
  if (gps_waiting (dataGPS, 2000000)) {
      /* leitura dos dados */
      if ((rc = gps_read(dataGPS)) == -1) {
          fprintf(stderr, "Erro ao realizar a leitura do GPS na linha # %d. Código do erro: %s\n",__LINE__, gps_errstr(rc));
      } else {
          /* Apresenteação dos dados recebidos pelo módulo GPS */
          if ((dataGPS->status == STATUS_FIX) &&
              (dataGPS->fix.mode == MODE_2D || dataGPS->fix.mode == MODE_3D) &&
              !isnan(dataGPS->fix.latitude) &&
              !isnan(dataGPS->fix.longitude)) {
                  // Print para fins de testes. Esta linha deve ser comentada!
                  fprintf(stderr, "latitude: %f, longitude: %f, speed: %f, timestamp: %lf\n", dataGPS->fix.latitude, dataGPS->fix.longitude, dataGPS->fix.speed, dataGPS->fix.time);
          } else {
              printf("Sem dados do GPS disponíveis!\n");
          }
      }
  } // FIM DO IF DE ESPERA ENTRE LEITURAS
} // FIM DA FUNÇÃO leituraGPS

// =======================================================================
// Função de encerramento do daemon do GPS
// =======================================================================
void killGPS(gps_data* dataGPS){
  gps_stream(dataGPS, WATCH_DISABLE, NULL);
  gps_close (dataGPS);
} // FIM DA FUNÇÃO killGPS
