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
void initGPS(gps_data_t* dataGPS){
  int rc;

  printf("Inicializando GPS ...\n");

  // Sys calls para configuração do gpsd como GPSD_SHARED_MEMORY
  system("sudo systemctl stop serial-getty@ttyS0.service");
  system("sudo systemctl disable serial-getty@ttyS0.service");
  system("sudo systemctl stop gpsd.socket");
  system("sudo systemctl disable gpsd.socket");
  system("sudo killall gpsd");
  system("sudo gpsd -n /dev/ttyS0 -F /var/run/gpsd.sock");

  sleep(2); // Aguarda por 2 segundos
  buzzerTone('D',300);
  buzzerTone('F',600);
  buzzerTone('X',100);

  if((rc = gps_open(GPSD_SHARED_MEMORY, NULL, dataGPS)) == -1){
    printf("Erro na inicialização do GPS na linha # %d\nRazão do erro: %s\n", __LINE__, gps_errstr(rc));
    exit(EXIT_FAILURE);
  }
} // FIM DA FUNÇÃO initGPS

// =======================================================================
// Função de leitura dos dados do GPS
// =======================================================================
void leituraGPS(gps_data_t* dataGPS){
  int rc = 0;
      if ((rc = gps_read(dataGPS)) == -1) {
          printf("Erro ao realizar a leitura do GPS na linha # %d. Código do erro: %s\n",__LINE__, gps_errstr(rc));
      }else{
          if ((dataGPS->status == STATUS_FIX) &&
              (dataGPS->fix.mode == MODE_2D || dataGPS->fix.mode == MODE_3D) &&
              !isnan(dataGPS->fix.latitude) &&
              !isnan(dataGPS->fix.longitude)) {
          }else{
              printf("Sem dados do GPS disponíveis!\n");
          }
      } // FIM DO IF DE ESPERA ENTRE LEITURAS
} // FIM DA FUNÇÃO leituraGPS

// =======================================================================
// Função de encerramento do daemon do GPS
// =======================================================================
void killGPS(gps_data_t* dataGPS){
  gps_stream(dataGPS, WATCH_DISABLE, NULL);
  gps_close (dataGPS);
} // FIM DA FUNÇÃO killGPS
