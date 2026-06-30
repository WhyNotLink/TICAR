#ifndef __BSP_BUZZER_H__
#define __BSP_BUZZER_H__

#include "board.h"

/* Buzzer: PA7 */
#define BEEP_PORT       GPIOA
#define BEEP_PA7_PIN    DL_GPIO_PIN_7

/* ========== API ========== */
void Buzzer_Init(void);
void Buzzer_On(void);
void Buzzer_Off(void);
void Buzzer_Toggle(void);
void Buzzer_Beep(uint16_t ms);
void Buzzer_BeepTimes(uint8_t times, uint16_t on_ms, uint16_t off_ms);

#endif /* __BSP_BUZZER_H__ */
