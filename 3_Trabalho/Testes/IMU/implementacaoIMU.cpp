/* =======================================================================
 * Pacote de implementação das funções dos IMUs
 * =======================================================================
 * O makefile deve ser construído com cuidado. De preferência seguir o
 * exemplo de makefile do RTIMULibDriver no caminho:
 * RTIMULib/Linux/RTIMULibDrive/Makefile
 * =======================================================================
 */

#include "implementacaoIMU.h"

// =======================================================================
// Função de inicialização dos IMU
// =======================================================================
RTIMU* initIMU(int i){
		// Inicializa os setting das i unidades IMU
    RTIMUSettings* settings;
    // Declaração redundante do imu. Pode ser retirado para otimização
    RTIMU* imu;

    /* Os arquivos .ini possuem as configurações de inicialização das unidades.
     * Como a configuração dos dois MPU6050 é igual, eles possuem o mesmo .ini
     * com a única diferença sendo o endereço I2C no qual eles estão conectados.
     * O arquivo .ini para o MPU9250 é distinto pois ele será utilizado com o
     * protocolo SPI.
     */
      switch (i) {
        case 0:
          // Referencia a pasta e o nome do arquivo que possui o .ini
          settings = new RTIMUSettings("initFiles", "MPU6050_0");

          // Cria o objeto i do IMU
    		  imu = RTIMU::createIMU(settings);

          if((imu == NULL)||(imu->IMUType() == RTIMU_TYPE_NULL)){
            printf("Unidade IMU [%d] não identificado na porta I2C 0x68.\n", i);
            exit(1);
          }

          imu->IMUInit();
          imu->setSlerpPower(0.02);
          imu->setGyroEnable(true);
          imu->setAccelEnable(true);
          imu->setCompassEnable(false);
          buzzerTone('C', 100);
          buzzerTone('X', 100);
          break;
        case 1:
          settings = new RTIMUSettings("initFiles", "MPU6050_1");

          imu = RTIMU::createIMU(settings);

          if((imu == NULL)||(imu->IMUType() == RTIMU_TYPE_NULL)){
            printf("Unidade IMU [%d] não identificado na porta I2C 0x69.\n", i);
            exit(1);
          }

          imu->IMUInit();
          imu->setSlerpPower(0.02);
          imu->setGyroEnable(true);
          imu->setAccelEnable(true);
          imu->setCompassEnable(false);
          buzzerTone('D', 100);
          buzzerTone('X', 100);
          break;
        case 2:
          settings = new RTIMUSettings("initFiles", "MPU9250_2");

          imu = RTIMU::createIMU(settings);

          if((imu == NULL)||(imu->IMUType() == RTIMU_TYPE_NULL)){
            printf("Unidade IMU [%d] não identificado na porta SPI.\n", i);
            exit(1);
          }

          imu->IMUInit();
          imu->setSlerpPower(0.02);
          imu->setGyroEnable(true);
          imu->setAccelEnable(true);
          imu->setCompassEnable(true);
          buzzerTone('A', 100);
          buzzerTone('X', 100);
          break;
        default:
          break;
      } /* FIM DO SWITCH CASE DO BUZZER */
    return imu;
} /*	FIM initIMU*/

// =======================================================================
// Função de leitura dos sensores
// =======================================================================
void leituraIMU(int i, RTIMU* imu[2], imuDataAngulo imu_struct[2]){
  // Se for necessário retornar também a taxa de amostragem, referir a:
  // RTIMULib/Linux/RTIMULibDrive/RTIMULibDrive.cpp

  // Realiza o polling na taxa recomendada
  usleep(imu[i]->IMUGetPollInterval() * 1000);

	while(imu[i]->IMURead()){
		RTIMU_DATA imuData = imu[i]->getIMUData();
	  insereStruct(i, imuData.fusionPose, imu_struct);
	} // FIM DO WHILE
} // FIM DA FUNÇÃO leituraIMU

// =======================================================================
// Função de inserção dos dados na struct de cada sensor
// =======================================================================
void insereStruct(int i, RTVector3 &vec, imuDataAngulo imu_struct[]){
			imu_struct[i].roll = (vec.x() * RTMATH_RAD_TO_DEGREE);
			imu_struct[i].pitch = (vec.y() * RTMATH_RAD_TO_DEGREE);
			imu_struct[i].yaw = (vec.z() * RTMATH_RAD_TO_DEGREE);
} // FIM DA FUNÇÃO insereStruct
