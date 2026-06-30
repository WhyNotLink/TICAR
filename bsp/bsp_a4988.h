#ifndef __BSP_A4988_H__
#define __BSP_A4988_H__

#include "board.h"

/* ========== 通道编号 ========== */
#define PTZ1  0
#define PTZ2  1

/* ========== 步进模式 ========== */
#define A4988_FULL_STEP       0
#define A4988_HALF_STEP       1
#define A4988_QUARTER_STEP    2
#define A4988_EIGHTH_STEP     3
#define A4988_SIXTEENTH_STEP  4

/* ========== 旋转方向 ========== */
#define A4988_DIR_CW   0
#define A4988_DIR_CCW  1

/* ========== PTZ1: TIMA1-C0 PWM (PB2) ========== */
#define PTZ1_PWM_INST       TIMA1
#define PTZ1_PWM_CLK_FREQ   4000000

/* PTZ1 方向: PA12 */
#define PTZ_DIR1_PORT       GPIOA
#define PTZ_DIR1_PA12_PIN   DL_GPIO_PIN_12

/* PTZ2 方向: PB23 */
#define PTZ_DIR2_PORT       GPIOB
#define PTZ_DIR2_PB23_PIN   DL_GPIO_PIN_23

/* ========== API ========== */
void A4988_Init(void);
void A4988_SetDirection(uint8_t ch, uint8_t dir);
void A4988_SetSpeed(uint8_t ch, uint16_t speed);
void A4988_Step(uint8_t ch, uint32_t steps);
void A4988_Enable(uint8_t ch);
void A4988_Disable(uint8_t ch);

#endif /* __BSP_A4988_H__ */
