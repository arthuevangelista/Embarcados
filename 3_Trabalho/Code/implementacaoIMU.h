#ifndef _IMPLEMENTACAOIMU_H_

#define _IMPLEMENTACAOIMU_H_

// constantes úteis declaradas na biblioteca RTIMULib2
#define	RTMATH_PI             3.1415926535
#define	RTMATH_DEGREE_TO_RAD  (RTMATH_PI / 180.0)
#define	RTMATH_RAD_TO_DEGREE  (180.0 / RTMATH_PI)

void initIMU(char* dirPath, int i);

void leituraIMU(int i, int &sampleCount, int &sampleRate, uint64_t &rateTimer, uint64_t &displayTimer, uint64_t &now);

void insereStruct(int i, RTVector3& vec);

#endif // _IMPLEMENTACAOIMU_H_
