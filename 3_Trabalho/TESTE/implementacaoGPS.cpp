/* =======================================================================
 * Pacote de implementação das funções do GPS
 * =======================================================================
 * Devem ser adicionadas as flags a seguir no momento da compilação:
 * -lm -lgps
 *
 * Exemplo:
 * g++ -o output filename.cpp -lm -lgps
 * =======================================================================
 */

#include "implementacaoGPS.h"

// =======================================================================
// Função de inicialização do GPS
// =======================================================================
void initGPS(gps_data** dataGPS){
  int rc;

  fprintf(stderr, "Inicializando GPS ...\n");
  if((rc = gps_open(source.server, source.port, **dataGPS)) == -1){
    fprintf(stderr, "Erro na inicialização do GPS na linha # %d\nRazão do erro: %s\n", __LINE__, gps_errstr(rc));
    exit(EXIT_FAILURE);
  }
  gps_stream(**dataGPS, WATCH_ENABLE | WATCH_JSON, NULL);
} // FIM DA FUNÇÃO initGPS

// =======================================================================
// Função de leitura dos dados do GPS
// =======================================================================
void leituraGPS(gps_data** dataGPS){
  /* Espera 2 segundos para receber os dados do GPS */
  if (gps_waiting (**dataGPS, 2000000)) {
      if ((rc = gps_read(**dataGPS)) == -1) {
          fprintf(stderr, "Erro ao realizar a leitura do GPS na linha # %d. Código do erro: %s\n",__LINE__, gps_errstr(rc));
      } else {
          if (((*dataGPS)->status == STATUS_FIX) &&
              ((*dataGPS)->fix.mode == MODE_2D || (*dataGPS)->fix.mode == MODE_3D) &&
              !isnan((*dataGPS)->fix.latitude) &&
              !isnan((*dataGPS)->fix.longitude)) {
                  // Print para fins de testes. Esta linha deve ser comentada!
                  //fprintf(stderr, "\nLatitude: %f\nLongitude: %f\nVelocidade: %f\nMarca de Tempo: %lf\n", (*dataGPS)->fix.latitude, (*dataGPS)->fix.longitude, (*dataGPS)->fix.speed, (*dataGPS)->fix.time);
          } else {
              fprintf(stderr, "Sem dados do GPS disponíveis!\n");
          }
      }
  } // FIM DO IF DE ESPERA ENTRE LEITURAS
} // FIM DA FUNÇÃO leituraGPS

// =======================================================================
// Função de encerramento do daemon do GPS
// =======================================================================
void killGPS(gps_data** dataGPS){
  gps_stream(**dataGPS, WATCH_DISABLE, NULL);
  gps_close (**dataGPS);
} // FIM DA FUNÇÃO killGPS
