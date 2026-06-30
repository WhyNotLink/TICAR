#ifndef __BSP_MOTOR_H__
#define __BSP_MOTOR_H__

#include "board.h"

/* ========== 电机编号 ========== */
#define MOTOR_A  0
#define MOTOR_B  1
#define MOTOR_C  2
#define MOTOR_D  3
#define MOTOR_NUM 4

/* ========== 电机方向 ========== */
#define MOTOR_FORWARD   1
#define MOTOR_BACKWARD  0
#define MOTOR_STOP      2

/* ========== API ========== */
void Motor_Init(void);
void Motor_SetSpeed(uint8_t ch, int16_t speed);
void Motor_SetDirection(uint8_t ch, uint8_t dir);
void Motor_Enable(uint8_t ch);
void Motor_Disable(uint8_t ch);
void Motor_EnableChannels(uint8_t mask);
int32_t Motor_GetEncoder(uint8_t ch);
void Motor_ClearEncoder(uint8_t ch);
int32_t Motor_GetSpeed(uint8_t ch);
void Motor_SetPID(uint8_t ch, float kp, float ki, float kd);
void Motor_PIDTask(void);

#endif /* __BSP_MOTOR_H__ */
