#ifndef __BSP_LED_H__
#define __BSP_LED_H__

#include "board.h"

/* ========== LED 编号 ========== */
#define LED1  0
#define LED2  1
#define LED3  2
#define LED4  3

/* LED1: PB22 */
#define LED1_PORT       GPIOB
#define LED1_PB22_PIN   DL_GPIO_PIN_22

/* LED2: PA30 */
#define LED2_PORT       GPIOA
#define LED2_PA30_PIN   DL_GPIO_PIN_30

/* LED3: PB0 */
#define LED3_PORT       GPIOB
#define LED3_PB0_PIN    DL_GPIO_PIN_0

/* LED4: PB1 */
#define LED4_PORT       GPIOB
#define LED4_PB1_PIN    DL_GPIO_PIN_1

/* ========== API ========== */
void LED_Init(void);
void LED_On(uint8_t led);
void LED_Off(uint8_t led);
void LED_Toggle(uint8_t led);
void LED_AllOn(void);
void LED_AllOff(void);

#endif /* __BSP_LED_H__ */
