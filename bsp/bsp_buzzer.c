/*
 * bsp_buzzer.c - 蜂鸣器控制 (MSPM0G3507)
 *
 * Buzzer: PA7
 */

#include "bsp_buzzer.h"

void Buzzer_Init(void)
{
    DL_GPIO_clearPins(BEEP_GPIO_PORT, BEEP_GPIO_BEEP_PIN_PIN);
}

void Buzzer_On(void)
{
    DL_GPIO_setPins(BEEP_GPIO_PORT, BEEP_GPIO_BEEP_PIN_PIN);
}

void Buzzer_Off(void)
{
    DL_GPIO_clearPins(BEEP_GPIO_PORT, BEEP_GPIO_BEEP_PIN_PIN);
}

void Buzzer_Toggle(void)
{
    DL_GPIO_togglePins(BEEP_GPIO_PORT, BEEP_GPIO_BEEP_PIN_PIN);
}

void Buzzer_Beep(uint16_t ms)
{
    Buzzer_On();
    delay_ms(ms);
    Buzzer_Off();
}

void Buzzer_BeepTimes(uint8_t times, uint16_t on_ms, uint16_t off_ms)
{
    uint8_t i;
    for (i = 0; i < times; i++) {
        Buzzer_On();
        delay_ms(on_ms);
        Buzzer_Off();
        if (i < (times - 1)) {
            delay_ms(off_ms);
        }
    }
}
