/* =======================================================================
 * Header da implementação dos IMUs
 * =======================================================================
 * O presente arquivo de header tem o propósito de modularizar o código com
 * as funções que implementam o IMU.
 *
 * Observe que este pacote de implementações e o arquivo principal com a
 * função main não possuem o extern "C" uma vez que estão usando fortemente
 * os recursos do C++.
 *
 * initIMU recebe um inteiro referente ao número do IMU sendo inicializado
 * e retorna um ponteiro para classe RTIMU. Este ponteiro será utilizado nas
 * outras funções de leitura e encerramento do IMU.
 *
 * =======================================================================
 */

#ifndef _IMPLEMENTACAOIMU_H_
#define _IMPLEMENTACAOIMU_H_

#ifndef _BUZZER_H_
  #include "buzzer.h"
#endif

#ifndef _RTIMULIB_H
  #include "RTIMULib.h"
#endif

typedef struct imuDataAngulo{
	double roll;
	double pitch;
	double yaw;
}imuDataAngulo;

RTIMU* initIMU(int i);

void leituraIMU(RTIMU* imu, int i, imuDataAngulo imu_struct);

#endif // _IMPLEMENTACAOIMU_H_
