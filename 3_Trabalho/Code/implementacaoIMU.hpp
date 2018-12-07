#ifndef _IMPLEMENTACAOIMU_HPP_

#define _IMPLEMENTACAOIMU_HPP_

// constantes úteis declaradas na biblioteca RTIMULib2
#define	RTMATH_PI             3.1415926535
#define	RTMATH_DEGREE_TO_RAD  (RTMATH_PI / 180.0)
#define	RTMATH_RAD_TO_DEGREE  (180.0 / RTMATH_PI)

void initIMU(const char* dirPath, int i, RTIMUSettings* settings[2], RTIMU* imu[2]);

void leituraIMU(int i, int &sampleCount, int &sampleRate, uint64_t &rateTimer, uint64_t &displayTimer, uint64_t &now, RTIMU* imu[2], MPU6050 imu_struct[]){

void insereStruct(int i, RTVector3& vec, MPU6050 imu_struct[]);

#endif // _IMPLEMENTACAOIMU_HPP_
