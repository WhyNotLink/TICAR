/*
 * bsp_mpu6050.c - MPU6050 6-axis sensor driver (I2C0)
 *
 * I2C0: PA28=SDA, PA31=SCL, 7-bit address 0x68
 */

#include "bsp_mpu6050.h"

/* ====================== 静态缓存 ====================== */
static int16_t g_accel_x_raw, g_accel_y_raw, g_accel_z_raw;
static int16_t g_gyro_x_raw,  g_gyro_y_raw,  g_gyro_z_raw;

/* ====================== 内部辅助 ====================== */

static void mpu6050_write_reg(uint8_t reg, uint8_t val)
{
    uint8_t buf[2];
    buf[0] = reg;
    buf[1] = val;

    DL_I2C_fillControllerTXFIFO(I2C_0_INST, buf, 2);
    DL_I2C_startControllerTransfer(I2C_0_INST,
        MPU6050_ADDR, DL_I2C_CONTROLLER_DIRECTION_TX, 2);

    while (DL_I2C_getControllerStatus(I2C_0_INST)
           != DL_I2C_CONTROLLER_STATUS_IDLE);
}

static void mpu6050_read_regs(uint8_t reg, uint8_t *buf, uint8_t len)
{
    /* Step 1: write register address */
    DL_I2C_fillControllerTXFIFO(I2C_0_INST, &reg, 1);
    DL_I2C_startControllerTransfer(I2C_0_INST,
        MPU6050_ADDR, DL_I2C_CONTROLLER_DIRECTION_TX, 1);

    while (DL_I2C_getControllerStatus(I2C_0_INST)
           != DL_I2C_CONTROLLER_STATUS_IDLE);

    /* Step 2: read data */
    DL_I2C_startControllerTransfer(I2C_0_INST,
        MPU6050_ADDR, DL_I2C_CONTROLLER_DIRECTION_RX, len);

    while (DL_I2C_getControllerStatus(I2C_0_INST)
           != DL_I2C_CONTROLLER_STATUS_IDLE);

    /* 逐字节读取 RX FIFO */
    for (uint8_t i = 0; i < len; i++) {
        buf[i] = DL_I2C_receiveControllerData(I2C_0_INST);
    }
}

/* ====================== API 实现 ====================== */

void MPU6050_Init(void)
{
    /* Wake up device (clear sleep bit in PWR_MGMT1) */
    mpu6050_write_reg(MPU6050_REG_PWR_MGMT1, 0x00);
    delay_ms(10);

    /* Set accelerometer range: ±2g (AFS_SEL = 0) */
    mpu6050_write_reg(MPU6050_REG_ACCEL_CONFIG, 0x00);

    /* Set gyroscope range: ±250 dps (FS_SEL = 0) */
    mpu6050_write_reg(MPU6050_REG_GYRO_CONFIG, 0x00);
}

void MPU6050_Update(void)
{
    uint8_t buf[14];

    /* Read 14 bytes starting from ACCEL_XOUT_H (0x3B) */
    mpu6050_read_regs(MPU6050_REG_ACCEL_XOUT_H, buf, 14);

    g_accel_x_raw = (int16_t)((buf[0] << 8) | buf[1]);
    g_accel_y_raw = (int16_t)((buf[2] << 8) | buf[3]);
    g_accel_z_raw = (int16_t)((buf[4] << 8) | buf[5]);
    /* buf[6..7] = temperature (skip) */
    g_gyro_x_raw  = (int16_t)((buf[8]  << 8) | buf[9]);
    g_gyro_y_raw  = (int16_t)((buf[10] << 8) | buf[11]);
    g_gyro_z_raw  = (int16_t)((buf[12] << 8) | buf[13]);
}

/* ====================== 原始值读取 ====================== */

int16_t MPU6050_ReadAccelX(void) { return g_accel_x_raw; }
int16_t MPU6050_ReadAccelY(void) { return g_accel_y_raw; }
int16_t MPU6050_ReadAccelZ(void) { return g_accel_z_raw; }
int16_t MPU6050_ReadGyroX(void)  { return g_gyro_x_raw;  }
int16_t MPU6050_ReadGyroY(void)  { return g_gyro_y_raw;  }
int16_t MPU6050_ReadGyroZ(void)  { return g_gyro_z_raw;  }

/* ====================== 工程单位转换 ====================== */

/* ±2g 量程: 16384 LSB/g */
#define ACCEL_SENSITIVITY  16384.0f

float MPU6050_GetAccelX_G(void) { return (float)g_accel_x_raw / ACCEL_SENSITIVITY; }
float MPU6050_GetAccelY_G(void) { return (float)g_accel_y_raw / ACCEL_SENSITIVITY; }
float MPU6050_GetAccelZ_G(void) { return (float)g_accel_z_raw / ACCEL_SENSITIVITY; }

/* ±250dps 量程: 131 LSB/(dps) */
#define GYRO_SENSITIVITY   131.0f

float MPU6050_GetGyroX_DPS(void) { return (float)g_gyro_x_raw / GYRO_SENSITIVITY; }
float MPU6050_GetGyroY_DPS(void) { return (float)g_gyro_y_raw / GYRO_SENSITIVITY; }
float MPU6050_GetGyroZ_DPS(void) { return (float)g_gyro_z_raw / GYRO_SENSITIVITY; }
