#ifndef __BSP_MPU6050_H__
#define __BSP_MPU6050_H__

#include "board.h"

#define MPU6050_ADDR         0x68
#define MPU6050_REG_PWR_MGMT1 0x6B
#define MPU6050_REG_ACCEL_CONFIG 0x1C
#define MPU6050_REG_GYRO_CONFIG  0x1B
#define MPU6050_REG_ACCEL_XOUT_H 0x3B
#define MPU6050_REG_GYRO_XOUT_H  0x43

void    MPU6050_Init(void);
void    MPU6050_Update(void);

int16_t MPU6050_ReadAccelX(void);
int16_t MPU6050_ReadAccelY(void);
int16_t MPU6050_ReadAccelZ(void);
int16_t MPU6050_ReadGyroX(void);
int16_t MPU6050_ReadGyroY(void);
int16_t MPU6050_ReadGyroZ(void);

float   MPU6050_GetAccelX_G(void);
float   MPU6050_GetAccelY_G(void);
float   MPU6050_GetAccelZ_G(void);
float   MPU6050_GetGyroX_DPS(void);
float   MPU6050_GetGyroY_DPS(void);
float   MPU6050_GetGyroZ_DPS(void);

#endif /* __BSP_MPU6050_H__ */
