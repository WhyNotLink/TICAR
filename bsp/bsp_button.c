/*
 * bsp_button.c - 2路按键驱动 (带消抖)
 *
 * KEY1: PA0, KEY2: PA1
 * 每 10ms 调用 Button_Task() 进行消抖扫描
 */

#include "bsp_button.h"

/* ====================== 按键通道配置 ====================== */
#define BUTTON_NUM 2

typedef struct {
    GPIO_Regs *port;
    uint32_t   pin;
} btn_hw_t;

static const btn_hw_t g_btn_hw[BUTTON_NUM] = {
    { KEY_PORT, KEY_KEY1_PIN },  /* ch0 = KEY1 */
    { KEY_PORT, KEY_KEY2_PIN },  /* ch1 = KEY2 */
};

/* ====================== 消抖状态 ====================== */
#define DEBOUNCE_CNT  3

typedef struct {
    uint8_t  state;
    uint8_t  stable_cnt;
    uint8_t  last_raw;
    uint8_t  press_flag;
    uint32_t press_start_ms;
    uint32_t hold_duration_ms;
} btn_state_t;

static btn_state_t g_btn[BUTTON_NUM];
static uint32_t    g_btn_tick_ms;

/* ====================== API 实现 ====================== */

void Button_Init(void)
{
    uint8_t i;

    g_btn_tick_ms = 0;

    for (i = 0; i < BUTTON_NUM; i++) {
        uint8_t raw = DL_GPIO_readPins(g_btn_hw[i].port, g_btn_hw[i].pin) ? 1 : 0;
        g_btn[i].state           = raw;
        g_btn[i].stable_cnt      = DEBOUNCE_CNT;
        g_btn[i].last_raw        = raw;
        g_btn[i].press_flag      = 0;
        g_btn[i].press_start_ms  = 0;
        g_btn[i].hold_duration_ms = 0;
    }
}

uint8_t Button_Read(uint8_t ch)
{
    if (ch >= BUTTON_NUM) return 1;
    return g_btn[ch].state;
}

uint8_t Button_GetPress(uint8_t ch)
{
    uint8_t flag;

    if (ch >= BUTTON_NUM) return 0;

    flag = g_btn[ch].press_flag;
    g_btn[ch].press_flag = 0;
    return flag;
}

uint8_t Button_GetLongPress(uint8_t ch, uint16_t ms)
{
    if (ch >= BUTTON_NUM) return 0;

    if (g_btn[ch].state == 0 && g_btn[ch].hold_duration_ms >= ms) {
        return 1;
    }
    return 0;
}

void Button_Task(void)
{
    uint8_t i;

    g_btn_tick_ms += 10;

    for (i = 0; i < BUTTON_NUM; i++) {
        uint8_t raw = DL_GPIO_readPins(g_btn_hw[i].port, g_btn_hw[i].pin) ? 1 : 0;

        if (raw == g_btn[i].last_raw) {
            if (g_btn[i].stable_cnt < DEBOUNCE_CNT) {
                g_btn[i].stable_cnt++;
                if (g_btn[i].stable_cnt >= DEBOUNCE_CNT) {
                    if (raw != g_btn[i].state) {
                        g_btn[i].state = raw;

                        if (raw == 0) {
                            g_btn[i].press_flag     = 1;
                            g_btn[i].press_start_ms  = g_btn_tick_ms;
                            g_btn[i].hold_duration_ms = 0;
                        } else {
                            g_btn[i].hold_duration_ms = 0;
                        }
                    }
                }
            }
        } else {
            g_btn[i].stable_cnt = 0;
        }

        g_btn[i].last_raw = raw;

        if (g_btn[i].state == 0) {
            g_btn[i].hold_duration_ms = g_btn_tick_ms - g_btn[i].press_start_ms;
        }
    }
}
