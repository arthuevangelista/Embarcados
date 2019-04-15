#ifndef _IMPLEMENTACAOIMU_H_

#define _IMPLEMENTACAOIMU_H_

// constantes úteis declaradas na biblioteca RTIMULib2
#define	RTMATH_PI             3.1415926535
#define	RTMATH_DEGREE_TO_RAD  (RTMATH_PI / 180.0)
#define	RTMATH_RAD_TO_DEGREE  (180.0 / RTMATH_PI)

#define MPU9250_ADDR 0x68
#define WHO_AM_I 0x75

#define CONFIG 0x1A

#define GYRO_CONFIG 0x18
#define ACCEL_CONFIG 0x1C

#define FIFO_EN 0x23

#define I2C_MST_CTRL 0x24

#define I2C_SLV0_ADDR 0x25 // endereço do primeiro mpu 6050
#define  I2_SLV0_REG 0x26
#define I2C_SLV0_CTRL 0x27

#define I2C_SLV1_ADDR 0x28 // endereço do segundo mpu 6050
#define  I2_SLV1_REG 0x29
#define I2C_SLV1_CTRL 0x2A

#define I2C_MST_STATUS 0x36

#define INT_PIN_CFG 0x37 // INT pin/bypass enable config

// registradores para leitura do acelerômetro
#define ACCEL_XOUT_H 0x3B
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
#define ACCEL_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40

// registradores para leitura do giroscópio
#define GYRO_XOUT_H 0x43
#define GYRO_XOUT_L 0x44
#define GYRO_YOUT_H 0x45
#define GYRO_YOUT_L 0x46
#define GYRO_ZOUT_H 0x47
#define GYRO_ZOUT_L 0x48

// registradores para leituras de temperatura
#define TEMP_OUT_H 0x41
#define TEMP_OUT_L 0x42


void initIMU(int i);

void leituraIMU(int i, IMU imu_struct);

void insereStruct(int i, IMU imu_struct[]);

#endif // _IMPLEMENTACAOIMU_H_
