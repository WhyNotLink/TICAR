/*
 * bsp_servo_180.c - 180° 舵机驱动 (MSPM0G3507)
 *
 * 2 通道, 50Hz PWM 占空比控制角度:
 *   PTZ1: TIMA1 CC0 (PB2)  → 500kHz, period=10000
 *   PTZ2: TIMG6 CC1 (PB3)  → 4MHz,   period=80000
 *
 * 脉冲宽度: 0°=500µs, 90°=1500µs, 180°=2500µs
 * 周期固定 20ms (50Hz)
 */

#include "bsp_servo_180.h"

/* ========== 内部辅助 ========== */

/* 角度转脉冲宽度 (µs): 0→500, 180→2500 */
static uint32_t AngleToPulseUs(uint8_t angle)
{
    if (angle > 180) angle = 180;
    return 500 + ((uint32_t)angle * 2000 / 180);
}

/* 脉冲宽度转 CCR 计数值 */
static uint32_t PulseUsToCCR(uint32_t period, uint32_t clk_hz, uint32_t pulse_us)
{
    /* duty_ccr = period * pulse_us / 20000 */
    return (uint32_t)((uint64_t)period * (uint64_t)pulse_us / 20000UL);
}

/* ========== API 实现 ========== */

void Servo_Init(uint8_t ch)
{
    uint32_t ccr;

    if (ch > SERVO_PTZ2) return;

    if (ch == SERVO_PTZ1) {
        /* TIMA1: 500kHz = 32MHz/8/(63+1), period=10000 → 50Hz, 初始0° */
        DL_TimerA_ClockConfig clk = {
            .clockSel    = DL_TIMER_CLOCK_BUSCLK,
            .divideRatio = DL_TIMER_CLOCK_DIVIDE_8,
            .prescale    = 63U
        };
        DL_TimerA_setClockConfig(SERVO1_PWM_INST, &clk);
        DL_Timer_setLoadValue(SERVO1_PWM_INST, SERVO1_PERIOD - 1);
        ccr = PulseUsToCCR(SERVO1_PERIOD, SERVO1_CLK_FREQ, AngleToPulseUs(0));
        DL_Timer_setCaptureCompareValue(SERVO1_PWM_INST, (uint16_t)ccr, DL_TIMER_CC_0_INDEX);
        DL_Timer_startCounter(SERVO1_PWM_INST);
    } else {
        /* TIMG6: 4MHz = 32MHz/8/(0+1), period=80000 → 50Hz, 初始0° */
        DL_TimerG_ClockConfig clk = {
            .clockSel    = DL_TIMER_CLOCK_BUSCLK,
            .divideRatio = DL_TIMER_CLOCK_DIVIDE_8,
            .prescale    = 0U
        };
        DL_TimerG_setClockConfig(SERVO2_PWM_INST, &clk);
        DL_Timer_setLoadValue(SERVO2_PWM_INST, SERVO2_PERIOD - 1);
        ccr = PulseUsToCCR(SERVO2_PERIOD, SERVO2_CLK_FREQ, AngleToPulseUs(0));
        DL_Timer_setCaptureCompareValue(SERVO2_PWM_INST, (uint16_t)ccr, DL_TIMER_CC_1_INDEX);
        DL_Timer_startCounter(SERVO2_PWM_INST);
    }
}

void Servo_SetAngle(uint8_t ch, uint8_t angle)
{
    uint32_t period, clk_hz;
    uint32_t pulse_us, ccr;

    if (ch > SERVO_PTZ2 || angle > 180) return;

    if (ch == SERVO_PTZ1) {
        period  = SERVO1_PERIOD;
        clk_hz  = SERVO1_CLK_FREQ;
    } else {
        period  = SERVO2_PERIOD;
        clk_hz  = SERVO2_CLK_FREQ;
    }

    pulse_us = AngleToPulseUs(angle);
    ccr = PulseUsToCCR(period, clk_hz, pulse_us);

    if (ch == SERVO_PTZ1) {
        DL_Timer_setCaptureCompareValue(SERVO1_PWM_INST, (uint16_t)ccr, DL_TIMER_CC_0_INDEX);
    } else {
        DL_Timer_setCaptureCompareValue(SERVO2_PWM_INST, (uint16_t)ccr, DL_TIMER_CC_1_INDEX);
    }
}
