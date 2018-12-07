#include "implementacaoIMU.h"

// =======================================================================
// Função de inicialização dos IMU
// =======================================================================
void initIMU(const char* dirPath, int i, RTIMUSettings* settings[2], RTIMU* imu[2]){
		// Inicializa apenas o setting da unidade i do vetor
		settings[i] = new RTIMUSettings(dirPath, "RTIMULib");

		// Cria apenas o objeto i do IMU
		imu[i] = RTIMU::createIMU(settings[i]);

		if((imu[i] == NULL)||(imu[i]->IMUType() == RTIMU_TYPE_NULL)){
			printf("IMU não identificado");
			exit(1);
		}

		imu[i]->IMUInit();

		imu[i]->setSlerpPower(0.02);
		imu[i]->serGyroEnable(true);
		imu[i]->setAccelEnable(true);
		imu[i]->setCompassEnable(true);

} /*	FIM initIMU*/

// =======================================================================
// Função de leitura dos sensores
// =======================================================================
void leituraIMU(int i, int &sampleCount, int &sampleRate, uint64_t &rateTimer, uint64_t &displayTimer, uint64_t &now, RTIMU* imu[2], MPU6050 imu_struct[]){
	// Zera os valores de sampleRate e sampleCount
	sampleRate = 0;
	sampleCount = 0;

	usleep(imu[i]->IMUGetPollInterval() * 1000);

	while(imu[i]->IMURead()){
		RTIMU_DATA imuData = imu[i]->getIMUData();
		sampleCount++;

		now = RTMath::currentUSecsSinceEpoch();

		// Apresenta os dados 10 vezes por segundo (100.000 ns = 0.1 s)
		if((now - displayTimer) > 100000){
			// A função RTMath::displayDegrees(const char* label, RTVector3& vec)
			// retorna os valores lidos dentro de uma string pronta para ser printada
			printf("Sample rate: %d: %s\r\n", sampleRate, RTMath::displayDegrees("", imuData.fusionPose));
			// Talvez seja interessante adquirir diretamente o RTVector3& vec
			// e inserir os valores lidos dentro do struct
			insereStruct(i, imuData.fusionPose, imu_struct);
			fflush(stdout);
			displayTimer = now;
		} // FIM DO PRIMEIRO IF

		// Apresenta a taxa de amostragem todo segundo (1.000.000 ns = 1 s)
		if(() > 1000000){
			sampleRate = sampleCount;
			sampleCount = 0;
			rateTimer = now;
		} // FIM DO SEGUNDO IF
	} // FIM DO WHILE
} // FIM DA FUNÇÃO leituraIMU

// =======================================================================
// Função de inserção dos dados na struct de cada sensor
// =======================================================================
void insereStruct(int i, RTVector3& vec, MPU6050 imu_struct[]){
			imu_struct[i].roll = (vec.x() * RTMATH_RAD_TO_DEGREE);
			imu_struct[i].pitch = (vec.y() * RTMATH_RAD_TO_DEGREE);
			imu_struct[i].yaw = (vec.z() * RTMATH_RAD_TO_DEGREE);
} // FIM DA FUNÇÃO insereStruct
