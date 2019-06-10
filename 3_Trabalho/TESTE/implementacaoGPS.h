/* =======================================================================
 * Header da implementação do GPS
 * =======================================================================
 * O presente arquivo de header tem o propósito de modularizar o código com
 * as funções que implementam o GPS.
 *
 * 1_ A função initGPS inicializa o daemon do GPS e retorna uma struct que
 * será usada para guardar os dados do GPS. Por se tratar de uma variável
 * local, é necessário que na main haja uma declaração explícita de uma
 * struct igual. Outras funções que necessitem destes dados ou devem
 * receber uma cópia dos dados ou receber a struct completa.
 *
 * De preferência, declare um ponteiro para gps_data_t na main
 * faça a passagem de dados por referência!
 *
 * 2_ A função leituraGPS realiza a leitura dos dados adquiridos pelo GPS
 * (armazenados em um buffer pelo daemon) e é responsável pelo tempo de
 * espera entre leituras. É importante relembrar que o módulo GPS está
 * conectado na porta UART da raspberry e, portanto, possui comportamento
 * assíncrono. Isso possibilita a realização de leituras do GPS a qualquer
 * momento. Entretanto, segundo a documentação da biblioteca gpsd:
 *
 * "The timing of your read loop is important. When it has satellite lock,
 * the daemon will be writing into its end of the socket once per whatever
 * the normal reporting-cycle time of your device is - for a GPS normally
 * one peer second. You must poll the socket more often that that."
 *
 * =======================================================================
 */

#ifndef _IMPLEMENTACAOGPS_H_
#define _IMPLEMENTACAOGPS_H_

#ifdef __cplusplus
  extern "C"{
#endif

#include <gps.h>
#include <math.h>

void initGPS(gps_data_t** dataGPS);

void leituraGPS(gps_data_t** dataGPS);

void killGPS(gps_data_t** dataGPS);

#ifdef __cplusplus
  }
#endif

#endif // _IMPLEMENTACAOGPS_H_
