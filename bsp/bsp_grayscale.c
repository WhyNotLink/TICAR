/*
 * bsp_grayscale.c - 8路灰度传感器
 *
 * OUT1: PA24, OUT2: PA25, OUT3: PB24, OUT4: PA22
 * OUT5: PA14, OUT6: PA17, OUT7: PB17, OUT8: PB18
 */

#include "bsp_grayscale.h"

/* ====================== 通道映射表 ====================== */
typedef struct {
    GPIO_Regs *port;
    uint32_t   pin;
} gray_ch_t;

static const gray_ch_t g_gray[8] = {
    { GRAY_OUT1_PORT, GRAY_OUT1_PIN },  /* ch0 */
    { GRAY_OUT2_PORT, GRAY_OUT2_PIN },  /* ch1 */
    { GRAY_OUT3_PORT, GRAY_OUT3_PIN },  /* ch2 */
    { GRAY_OUT4_PORT, GRAY_OUT4_PIN },  /* ch3 */
    { GRAY_OUT5_PORT, GRAY_OUT5_PIN },  /* ch4 */
    { GRAY_OUT6_PORT, GRAY_OUT6_PIN },  /* ch5 */
    { GRAY_OUT7_PORT, GRAY_OUT7_PIN },  /* ch6 */
    { GRAY_OUT8_PORT, GRAY_OUT8_PIN },  /* ch7 */
};

/* ====================== API 实现 ====================== */

void Grayscale_Init(void)
{
    /* GPIO 初始化已由 SYSCFG_DL_GPIO_init() 完成 */
}

uint8_t Grayscale_Read(uint8_t ch)
{
    if (ch > 7) return 0;

    if (DL_GPIO_readPins(g_gray[ch].port, g_gray[ch].pin)) {
        return 1;
    }
    return 0;
}

uint8_t Grayscale_ReadAll(void)
{
    uint8_t i, result = 0;

    for (i = 0; i < 8; i++) {
        if (DL_GPIO_readPins(g_gray[i].port, g_gray[i].pin)) {
            result |= (1 << i);
        }
    }

    return result;
}
