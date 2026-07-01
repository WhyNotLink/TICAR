#ifndef __BSP_SERVO_360_H__
#define __BSP_SERVO_360_H__

#include "board.h"

/* ========== 通道编号 ========== */
#define SERVO360_PTZ1  0
#define SERVO360_PTZ2  1

/*
 * 360° 连续旋转舵机, 50Hz PWM, 占空比控制转速:
 *   1.5ms = 停止
 *   0.5~1.5ms = 正转 (CW), 越短越快
 *   1.5~2.5ms = 反转 (CCW), 越长越快
 *
 * PTZ1 (TIMA1, 16位): 32MHz÷8÷64=500kHz → period=10000 → 50Hz
 * PTZ2 (TIMG6, 32位): 32MHz÷8÷1=4MHz    → period=80000 → 50Hz
 * _Init(ch) 自动配置 prescaler + period, 无需改 SysConfig。
 */

/* ========== 舵机配置参数 ========== */

#define SERVO3601_PWM_INST      TIMA1
#define SERVO3602_PWM_INST      TIMG6

#define SERVO3601_CLK_FREQ      500000
#define SERVO3602_CLK_FREQ      4000000

#define SERVO3601_PERIOD        10000
#define SERVO3602_PERIOD        80000

/* ========== API ========== */
void Servo360_Init(uint8_t ch);
void Servo360_SetSpeed(uint8_t ch, int8_t speed);  /* -100~100, 0=Stop, +CW/-CCW */

#endif /* __BSP_SERVO_360_H__ */
