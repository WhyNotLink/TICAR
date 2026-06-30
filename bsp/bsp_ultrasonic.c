/*
 * bsp_ultrasonic.c - HC-SR04 超声波传感器驱动 (TIMG12 硬件定时器测距)
 *
 * TRIG: PB25 (output)
 * ECHO: PB20 (TIMG12-C0 捕获, GPIOB.20)
 *
 * TIMG12 32位计数器: BUSCLK/8=4MHz, 每 tick=0.25µs
 * 32位溢出时间 = 2^32 / 4MHz ≈ 1073秒 → 远够
 * 不依赖捕获中断, 通过 GPIO 轮询 + 读硬件计数器实现脉宽测量.
 */

#include "bsp_ultrasonic.h"

/* ====================== 静态变量 ====================== */
static volatile uint8_t  g_capture_ready;   /* 捕获完成标志 */

void Ultrasonic_Init(void)
{
    /* 确保 TRIG 低电平 */
    DL_GPIO_clearPins(ULTRASONIC_PORT, ULTRASONIC_TRIG_PIN);

    /*
     * TIMG12 由 SYSCFG_DL_ULTRASONIC_TIMER_init() 配置为 Capture 模式,
     * 4MHz, startTimer=STOP. 这里启动为连续计数器用于手动测脉宽.
     * 32位计数器: load=0xFFFFFFFF, 向下计数.
     */
    DL_Timer_setLoadValue(ULTRASONIC_TIMER_INST, 0xFFFFFFFF);
    DL_Timer_startCounter(ULTRASONIC_TIMER_INST);

    g_capture_ready = 0;
}

float Ultrasonic_GetDistance(void)
{
    uint32_t rise_tick, fall_tick, timeout;
    float    pulse_us;

    /*
     * 重置计数器 (32位, 向下计数)
     */
    DL_Timer_stopCounter(ULTRASONIC_TIMER_INST);
    DL_Timer_setLoadValue(ULTRASONIC_TIMER_INST, 0xFFFFFFFF);
    DL_Timer_startCounter(ULTRASONIC_TIMER_INST);

    /* ---- 发出 10µs 触发脉冲 ---- */
    DL_GPIO_setPins(ULTRASONIC_PORT, ULTRASONIC_TRIG_PIN);
    delay_us(10);
    DL_GPIO_clearPins(ULTRASONIC_PORT, ULTRASONIC_TRIG_PIN);

    /* ---- 等待 ECHO 上升沿 (超时 ~2ms) ---- */
    timeout = 2000;
    while (!DL_GPIO_readPins(GPIOB, DL_GPIO_PIN_20) && timeout > 0) {
        delay_us(1);
        timeout--;
    }
    if (timeout == 0) return -1.0f;

    /* 读取上升沿时刻 (向下计数: rise_tick > fall_tick) */
    rise_tick = DL_Timer_getTimerCount(ULTRASONIC_TIMER_INST);

    /* ---- 等待 ECHO 下降沿 (超时 ~38ms = ~6.5m) ---- */
    timeout = 38000;
    while (DL_GPIO_readPins(GPIOB, DL_GPIO_PIN_20) && timeout > 0) {
        delay_us(1);
        timeout--;
    }
    if (timeout == 0) return -1.0f;

    /* 读取下降沿时刻 */
    fall_tick = DL_Timer_getTimerCount(ULTRASONIC_TIMER_INST);

    /*
     * 计算距离:
     *   TIMG12 @ 4MHz → 1 tick = 0.25µs
     *   向下计数: pulse_ticks = rise - fall
     *   pulse_us = pulse_ticks * 0.25
     *   distance(cm) = pulse_us * 0.034 / 2 = pulse_us * 0.017
     */
    pulse_us = (float)(rise_tick - fall_tick) * 0.25f;
    return pulse_us * 0.017f;
}
