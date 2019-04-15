#include "implementacaoIMU.h"

// =======================================================================
// Função de inicialização dos IMU
// =======================================================================
void initIMU(int i){
  printf("\n==============================\n");
  printf("Inicializando os IMUs\n");
  printf("==============================\n\n");
  // INICIALICAÇÃO
  if((fd = wiringPiI2CSetup(MPU9250_ADDR)) < 0){
    printf("MPU-9250 não detectado!\n");
    return fd;
  }else{
    printf("MPU-9250 identificado na porta: %d\n", wiringPiI2CReadReg8(fd,WHO_AM_I));
    // Do contrário, envia um sinal pro 9250 acessar os dois 6050
    // e verificar se eles existem
    wiringPiI2CWriteReg8(fd,INT_PIN_CFG,0x);
  } /* FIM DO IF-ELSE */

  // SELF-TESTS

} /* FIM DO initIMU */

// =======================================================================
// Função de leitura dos sensores
// =======================================================================
void leituraIMU(int i, IMU imu_struct){
  int ax, ay, az, gx, gy, gz;
  // DADOS BRUTOS VINDOS DE UM SENSOR
  ax = wiringPiI2CReadReg8(fd,ACCEL_XOUT_H)<<8|wiringPiI2CReadReg8(fd,ACCEL_XOUT_L);
  ay = wiringPiI2CReadReg8(fd,ACCEL_YOUT_H)<<8|wiringPiI2CReadReg8(fd,ACCEL_YOUT_L);
  az = wiringPiI2CReadReg8(fd,ACCEL_ZOUT_H)<<8|wiringPiI2CReadReg8(fd,ACCEL_ZOUT_L);
  gx = wiringPiI2CReadReg8(fd,GYRO_XOUT_H)<<8|wiringPiI2CReadReg8(fd,GYRO_XOUT_L);
  gy = wiringPiI2CReadReg8(fd,GYRO_YOUT_H)<<8|wiringPiI2CReadReg8(fd,GYRO_YOUT_L);
  gz = wiringPiI2CReadReg8(fd,GYRO_ZOUT_H)<<8|wiringPiI2CReadReg8(fd,GYRO_ZOUT_L);

} /* FIM DO leituraIMU */

// =======================================================================
// Função de inserção dos dados na struct de cada sensor
// =======================================================================
void insereStruct(int i, IMU imu_struct[]){
} /* FIM DO insereStruct */
