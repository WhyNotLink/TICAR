/*
 * bsp_led.c - LED 控制 (MSPM0G3507)
 *
 * 4路 LED: LED1=PB22, LED2=PA30, LED3=PB0, LED4=PB1
 */

#include "bsp_led.h"

/* ========== LED 引脚映射表 ========== */
typedef struct {
    GPIO_Regs *port;
    uint32_t   pin;
} led_map_t;

static const led_map_t g_led_map[] = {
    {LED12_LED1_PIN_PORT, LED12_LED1_PIN_PIN},   /* LED1: PB22 */
    {LED12_LED2_PIN_PORT, LED12_LED2_PIN_PIN},   /* LED2: PA30 */
    {LED34_PORT,          LED34_LED3_PIN_PIN},   /* LED3: PB0  */
    {LED34_PORT,          LED34_LED4_PIN_PIN},   /* LED4: PB1  */
};

#define LED_COUNT  (sizeof(g_led_map) / sizeof(g_led_map[0]))

/* ========== API 实现 ========== */

void LED_Init(void)
{
    uint8_t i;
    for (i = 0; i < LED_COUNT; i++) {
        DL_GPIO_clearPins(g_led_map[i].port, g_led_map[i].pin);
    }
}

void LED_On(uint8_t led)
{
    if (led >= LED_COUNT) return;
    DL_GPIO_setPins(g_led_map[led].port, g_led_map[led].pin);
}

void LED_Off(uint8_t led)
{
    if (led >= LED_COUNT) return;
    DL_GPIO_clearPins(g_led_map[led].port, g_led_map[led].pin);
}

void LED_Toggle(uint8_t led)
{
    if (led >= LED_COUNT) return;
    DL_GPIO_togglePins(g_led_map[led].port, g_led_map[led].pin);
}

void LED_AllOn(void)
{
    uint8_t i;
    for (i = 0; i < LED_COUNT; i++) {
        DL_GPIO_setPins(g_led_map[i].port, g_led_map[i].pin);
    }
}

void LED_AllOff(void)
{
    uint8_t i;
    for (i = 0; i < LED_COUNT; i++) {
        DL_GPIO_clearPins(g_led_map[i].port, g_led_map[i].pin);
    }
}
