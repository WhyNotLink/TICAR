/*
 * bsp_servo_360.c - 360° 连续旋转舵机驱动 (MSPM0G3507)
 *
 * 2 通道, 50Hz PWM 占空比控制转速:
 *   PTZ1: TIMA1 CC0 (PB2)  → 500kHz, period=10000
 *   PTZ2: TIMG6 CC1 (PB3)  → 4MHz,   period=80000
 *
 * 占空比 → 转速:
 *   1.5ms (7.5%) = 停止
 *   0.5~1.5ms    = 正转 (越快越短)
 *   1.5~2.5ms    = 反转 (越快越长)
 */

#include "bsp_servo_360.h"

/* ========== 内部辅助 ========== */

/* 转速(-100~100) → 脉冲宽度(µs): -100→500, 0→1500, +100→2500 */
static uint32_t SpeedToPulseUs(int8_t speed)
{
    if (speed > 100)  speed = 100;
    if (speed < -100) speed = -100;
    return 1500 + ((int32_t)speed * 1000 / 100);
}

/* 脉冲宽度 → CCR 计数值 */
static uint32_t PulseUsToCCR(uint32_t period, uint32_t clk_hz, uint32_t pulse_us)
{
    return (uint32_t)((uint64_t)period * (uint64_t)pulse_us / 20000UL);
}

/* ========== API 实现 ========== */

void Servo360_Init(uint8_t ch)
{
    uint32_t ccr;

    if (ch > SERVO360_PTZ2) return;

    if (ch == SERVO360_PTZ1) {
        /* TIMA1: 500kHz = 32MHz/8/(63+1), period=10000 → 50Hz, 初始停止 */
        DL_TimerA_ClockConfig clk = {
            .clockSel    = DL_TIMER_CLOCK_BUSCLK,
            .divideRatio = DL_TIMER_CLOCK_DIVIDE_8,
            .prescale    = 63U
        };
        DL_TimerA_setClockConfig(SERVO3601_PWM_INST, &clk);
        DL_Timer_setLoadValue(SERVO3601_PWM_INST, SERVO3601_PERIOD - 1);
        ccr = PulseUsToCCR(SERVO3601_PERIOD, SERVO3601_CLK_FREQ, 1500);
        DL_Timer_setCaptureCompareValue(SERVO3601_PWM_INST, (uint16_t)ccr, DL_TIMER_CC_0_INDEX);
        DL_Timer_startCounter(SERVO3601_PWM_INST);
    } else {
        /* TIMG6: 4MHz = 32MHz/8/(0+1), period=80000 → 50Hz, 初始停止 */
        DL_TimerG_ClockConfig clk = {
            .clockSel    = DL_TIMER_CLOCK_BUSCLK,
            .divideRatio = DL_TIMER_CLOCK_DIVIDE_8,
            .prescale    = 0U
        };
        DL_TimerG_setClockConfig(SERVO3602_PWM_INST, &clk);
        DL_Timer_setLoadValue(SERVO3602_PWM_INST, SERVO3602_PERIOD - 1);
        ccr = PulseUsToCCR(SERVO3602_PERIOD, SERVO3602_CLK_FREQ, 1500);
        DL_Timer_setCaptureCompareValue(SERVO3602_PWM_INST, (uint16_t)ccr, DL_TIMER_CC_1_INDEX);
        DL_Timer_startCounter(SERVO3602_PWM_INST);
    }
}

void Servo360_SetSpeed(uint8_t ch, int8_t speed)
{
    uint32_t period, clk_hz;
    uint32_t pulse_us, ccr;

    if (ch > SERVO360_PTZ2) return;

    if (ch == SERVO360_PTZ1) {
        period  = SERVO3601_PERIOD;
        clk_hz  = SERVO3601_CLK_FREQ;
    } else {
        period  = SERVO3602_PERIOD;
        clk_hz  = SERVO3602_CLK_FREQ;
    }

    pulse_us = SpeedToPulseUs(speed);
    ccr = PulseUsToCCR(period, clk_hz, pulse_us);

    if (ch == SERVO360_PTZ1) {
        DL_Timer_setCaptureCompareValue(SERVO3601_PWM_INST, (uint16_t)ccr, DL_TIMER_CC_0_INDEX);
    } else {
        DL_Timer_setCaptureCompareValue(SERVO3602_PWM_INST, (uint16_t)ccr, DL_TIMER_CC_1_INDEX);
    }
}
