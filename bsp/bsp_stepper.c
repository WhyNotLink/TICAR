/*
 * bsp_stepper.c - A4988 步进电机驱动 (MSPM0G3507)
 *
 * 2 通道, 脉冲频率控制速度:
 *   PTZ1: TIMA1 CC0 PWM (PB2) + PA12 方向
 *   PTZ2: TIMG6 CC1 PWM (PB3) + PB23 方向
 *
 * PTZ1: BUSCLK/8/100 = 40kHz, period=1000 → 40Hz 基频
 * PTZ2: BUSCLK/8/100 = 40kHz, period=1000 → 40Hz 基频
 */

#include "bsp_stepper.h"

/* ========== A4988 通道配置 ========== */
typedef struct {
    uint8_t  enabled;
} stepper_ch_t;

static stepper_ch_t g_ptz[2];

/* ========== API 实现 ========== */

void A4988_Init(uint8_t ch)
{
    if (ch > PTZ2) return;

    if (ch == PTZ1) {
        /* TIMA1: 40kHz = 32MHz/8/(99+1), period=1000, STOPPED */
        DL_TimerA_ClockConfig clk = {
            .clockSel    = DL_TIMER_CLOCK_BUSCLK,
            .divideRatio = DL_TIMER_CLOCK_DIVIDE_8,
            .prescale    = 99U
        };
        DL_TimerA_setClockConfig(PTZ1_PWM_INST, &clk);
        DL_Timer_setLoadValue(PTZ1_PWM_INST, 999);
        DL_GPIO_clearPins(PTZ_DIR_PTZ1_DIR_PORT, PTZ_DIR_PTZ1_DIR_PIN);
    } else {
        /* TIMG6: 40kHz = 32MHz/8/(99+1), period=1000, STOPPED */
        DL_TimerG_ClockConfig clk = {
            .clockSel    = DL_TIMER_CLOCK_BUSCLK,
            .divideRatio = DL_TIMER_CLOCK_DIVIDE_8,
            .prescale    = 99U
        };
        DL_TimerG_setClockConfig(PTZ2_PWM_INST, &clk);
        DL_Timer_setLoadValue(PTZ2_PWM_INST, 999);
        DL_GPIO_clearPins(PTZ_DIR_PTZ2_DIR_PORT, PTZ_DIR_PTZ2_DIR_PIN);
    }

    g_ptz[ch].enabled = 0;
}

void A4988_SetDirection(uint8_t ch, uint8_t dir)
{
    if (ch > PTZ2) return;

    if (ch == PTZ1) {
        if (dir == A4988_DIR_CW) {
            DL_GPIO_clearPins(PTZ_DIR_PTZ1_DIR_PORT, PTZ_DIR_PTZ1_DIR_PIN);
        } else {
            DL_GPIO_setPins(PTZ_DIR_PTZ1_DIR_PORT, PTZ_DIR_PTZ1_DIR_PIN);
        }
    } else {
        if (dir == A4988_DIR_CW) {
            DL_GPIO_clearPins(PTZ_DIR_PTZ2_DIR_PORT, PTZ_DIR_PTZ2_DIR_PIN);
        } else {
            DL_GPIO_setPins(PTZ_DIR_PTZ2_DIR_PORT, PTZ_DIR_PTZ2_DIR_PIN);
        }
    }
}

void A4988_SetSpeed(uint8_t ch, uint16_t speed)
{
    uint32_t period;
    uint32_t clk_freq;

    if (ch > PTZ2 || speed == 0) return;

    /* 步进速率 = PWM时钟频率 / period */
    if (ch == PTZ1) {
        clk_freq = PTZ1_PWM_INST_CLK_FREQ;
    } else {
        clk_freq = PTZ2_PWM_INST_CLK_FREQ;
    }

    period = clk_freq / speed;
    if (period < 2)   period = 2;
    if (period > 65535) period = 65535;

    if (ch == PTZ1) {
        DL_Timer_setLoadValue(PTZ1_PWM_INST, (uint16_t)(period - 1));
        DL_Timer_setCaptureCompareValue(PTZ1_PWM_INST,
            (uint16_t)(period / 2), DL_TIMER_CC_0_INDEX);
    } else {
        /* PTZ2 使用 TIMG6: 修改 load 值改变周期 */
        DL_Timer_setLoadValue(PTZ2_PWM_INST, (uint16_t)(period - 1));
        DL_Timer_setCaptureCompareValue(PTZ2_PWM_INST,
            (uint16_t)(period / 2), DL_TIMER_CC_1_INDEX);
    }
}

void A4988_Step(uint8_t ch, uint32_t steps)
{
    uint32_t i;

    if (ch > PTZ2 || steps == 0) return;

    if (ch == PTZ1) {
        uint32_t period = DL_Timer_getLoadValue(PTZ1_PWM_INST) + 1;
        uint32_t wait_us;

        if (period < 2) period = 2;
        wait_us = (period * 1000000UL) / PTZ1_PWM_INST_CLK_FREQ;

        for (i = 0; i < steps; i++) {
            DL_Timer_startCounter(PTZ1_PWM_INST);
            delay_us((uint32_t)wait_us);
            DL_Timer_stopCounter(PTZ1_PWM_INST);
            /* 重载 LOAD 确保下次启动从正确初值开始 */
            DL_Timer_setLoadValue(PTZ1_PWM_INST, (uint16_t)(period - 1));
        }
    } else {
        /* PTZ2: 使用 TIMG6 PWM (硬件脉冲) */
        uint32_t period = DL_Timer_getLoadValue(PTZ2_PWM_INST) + 1;
        uint32_t wait_us;

        if (period < 2) period = 2;
        wait_us = (period * 1000000UL) / PTZ2_PWM_INST_CLK_FREQ;

        for (i = 0; i < steps; i++) {
            DL_Timer_startCounter(PTZ2_PWM_INST);
            delay_us((uint32_t)wait_us);
            DL_Timer_stopCounter(PTZ2_PWM_INST);
            /* 重载 LOAD 确保下次启动从正确初值开始 */
            DL_Timer_setLoadValue(PTZ2_PWM_INST, (uint16_t)(period - 1));
        }
    }
}

void A4988_Enable(uint8_t ch)
{
    if (ch > PTZ2) return;

    g_ptz[ch].enabled = 1;

    if (ch == PTZ1) {
        DL_Timer_startCounter(PTZ1_PWM_INST);
    } else {
        DL_Timer_startCounter(PTZ2_PWM_INST);
    }
}

void A4988_Disable(uint8_t ch)
{
    if (ch > PTZ2) return;

    g_ptz[ch].enabled = 0;

    if (ch == PTZ1) {
        DL_Timer_stopCounter(PTZ1_PWM_INST);
    } else {
        DL_Timer_stopCounter(PTZ2_PWM_INST);
    }
}
