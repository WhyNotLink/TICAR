#ifndef __BSP_SERVO_180_H__
#define __BSP_SERVO_180_H__

#include "board.h"

/* ========== 通道编号 ========== */
#define SERVO_PTZ1  0
#define SERVO_PTZ2  1

/*
 * 180° 舵机, 50Hz PWM (周期 20ms), 占空比控制角度:
 *   0°  → 0.5ms  90° → 1.5ms  180° → 2.5ms
 *
 * PTZ1 (TIMA1, 16位): 32MHz÷8÷64=500kHz → period=10000 → 50Hz
 * PTZ2 (TIMG6, 32位): 32MHz÷8÷1=4MHz    → period=80000 → 50Hz
 * _Init(ch) 自动配置 prescaler + period, 无需改 SysConfig。
 */

/* ========== 舵机配置参数 ========== */

/* PTZ1 / PTZ2: PWM 实例 */
#define SERVO1_PWM_INST         TIMA1
#define SERVO2_PWM_INST         TIMG6

/* PWM 时钟频率 */
#define SERVO1_CLK_FREQ         500000
#define SERVO2_CLK_FREQ         4000000

/* PWM 周期 (50Hz) */
#define SERVO1_PERIOD           10000
#define SERVO2_PERIOD           80000

/* ========== API ========== */
void Servo_Init(uint8_t ch);
void Servo_SetAngle(uint8_t ch, uint8_t angle);  /* angle: 0~180 */

#endif /* __BSP_SERVO_180_H__ */
