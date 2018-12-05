
// INICIALICAÇÃO DAS BIBLIOTECAS EM COMUM
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <string.h>

typedef struct MPU9250{
	// Dados do accel
	double accel_X;
	double accel_Y;
	double accel_Z;

	// Dados do gyro
	double gyro_X;
	double gyro_Y;
	double gyro_Z;

	// Dados do compasso
	double compass_X;
	double compass_Y;
	double compass_Z;
}MPU9250;

typedef struct MPU6050{
	// Dados do accel
	double accel_X;
	double accel_Y;
	double accel_Z;

	// Dados do gyro
	double gyro_X;
	double gyro_Y;
	double gyro_Z;
}MPU6050;

// Declaração das structs dos IMUs (GLOBAL)

  // declarados como 0 e 1 para manter o valor de i no nome do imu
  MPU6050 *imu0;
  MPU6050 *imu1;
  // declarado como 2 para não confundir com os imus mpu 6050
  MPU9250 *imu2;

void initIMU(){

		RTIMUSettings *settings = new RTIMUSettings("RTIMULib");

		RTIMU *imu = RTIMU::createIMU(settings);

		if((imu == NULL)||(imu->IMUType() == RTIMU_TYPE_NULL)){
			printf("IMU não identificado");
			exit(1);
		}

		imu->IMUInit();

		imu->setSlerpPower(0.02);
		imu->serGyroEnable(true);
		imu->setAccelEnable(true);
		imu->setCompassEnable(true);
		/*	FIM initIMU*/
}

void initGPS(){
	/*	INICIALIZAÇÃO DO GPS	*/

	/*	FIM INIT GPS	*/
}

int main (){
  wiringPiSetup();

  // Definindo os Endereços dos IMU para INICIALICAÇÃO
  pinMode(12, OUTPUT);
  pinMode(17, OUTPUT);
  pinMode(27, OUTPUT);

  /*	INICIALIZAÇÃO DOS SENSORES IMU	*/

  digitalWrite(12, HIGH);
  digitalWrite(17, LOW);
  digitalWrite(27, LOW);
  initIMU();



  digitalWrite(12, LOW);
  digitalWrite(17, HIGH);
  digitalWrite(27, LOW);
  initIMU();

  digitalWrite(12, LOW);
  digitalWrite(17, LOW);
  digitalWrite(27, HIGH);
  initIMU();
  /*	INICIALIZAÇÃO DO GPS	*/
  initGPS();

	return 0;
}
