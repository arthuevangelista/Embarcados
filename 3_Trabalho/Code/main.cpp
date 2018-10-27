#include <stdio.h>
#include <stdlib.h>

void initMPU9250(){

		RTIMUSettings *settings = new RTIMUSettings("RTIMULib");

		RTIMU *imu = RTIMU::createIMU(settings);

		if((imu == NULL)||(imu->IMUType() == RTIMU_TYPE_NULL)){
			printf("MPU-9250 não identificado");
			exit(1);
		}

		imu->IMUInit();

		imu->setSlerpPower(0.02);
		imu->serGyroEnable(true);
		imu->setAccelEnable(true);
		imu->setCompassEnable(true);
		/*	FIM initMPU9250*/
}

void initGPS(){
	/*	INICIALIZAÇÃO DO GPS	*/
	
	/*	FIM INIT GPS	*/
}

int main (){
/*	INICIALIZAÇÃO DOS SENSORES IMU	*/
initMPU9250();
/*	INICIALIZAÇÃO DO GPS	*/
initGPS();

	return 0;
}
