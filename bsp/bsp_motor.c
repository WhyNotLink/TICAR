/*
 * bsp_motor.c - TB6612 + MG513 电机驱动 (MSPM0G3507)
 *
 * 4路直流电机控制: 2x TB6612 驱动, MG513 编码器反馈
 * PWM:  TIMA0, BUSCLK/8/4=1MHz, period=1000 → 1kHz (HIGH=导通电机)
 * PID:  TIMG7, BUSCLK/8/4=1MHz, load=9999 → 10ms
 */

#include "bsp_motor.h"

/* ====================== 编码器分辨率 ====================== */
/* MG513: 13 PPR × 4 (正交) × 30 (减速比) = 1560 计数/输出轴转 */
#define ENCODER_RESOLUTION  1560

/* ====================== PID 限幅 ====================== */
#define PID_OUTPUT_MAX   1000
#define PID_OUTPUT_MIN  -1000
#define PID_INTEGRAL_MAX  3000

/* ====================== 电机通道配置结构 ====================== */
typedef struct {
    /* PWM 通道: DL_TIMER_CC_x_INDEX */
    uint32_t         pwm_cc_idx;

    /* 方向引脚 1 */
    GPIO_Regs       *dir1_port;
    uint32_t         dir1_pin;

    /* 方向引脚 2 */
    GPIO_Regs       *dir2_port;
    uint32_t         dir2_pin;

    /* 编码器 A 相 */
    GPIO_Regs       *enc_a_port;
    uint32_t         enc_a_pin;

    /* 编码器 B 相 */
    GPIO_Regs       *enc_b_port;
    uint32_t         enc_b_pin;

    /* ---- 运行状态 ---- */
    volatile int32_t encoder_count;      /* 编码器累计计数值 */
    int32_t          prev_encoder_count; /* 上一周期编码器值  */
    int16_t          target_speed;       /* 目标转速 -1000~1000 */
    int16_t          current_duty;       /* 当前占空比 0~4000  */
    int32_t          measured_rpm;       /* 实测转速 (RPM)       */
    uint8_t          direction;          /* 当前方向             */

    /* ---- PID 参数 ---- */
    float            kp, ki, kd;
    float            integral;
    int32_t          last_error;
    uint8_t          pid_enabled;        /* PID 是否启用          */
} motor_ch_t;

/* ====================== 全局变量 ====================== */
static motor_ch_t g_motor[MOTOR_NUM];
static uint8_t    g_motor_enabled = (1 << MOTOR_A) | (1 << MOTOR_C); /* 默认使能 A 和 C */
/* PWM 周期 = 1000 (与 SysConfig MOTOR_PWM.period 一致) */
#define PWM_PERIOD  1000

/* ====================== 内部辅助函数 ====================== */
static void motor_set_dir_pins(motor_ch_t *m, uint8_t dir)
{
    if (dir == MOTOR_FORWARD) {
        DL_GPIO_setPins(m->dir1_port, m->dir1_pin);
        DL_GPIO_clearPins(m->dir2_port, m->dir2_pin);
    } else if (dir == MOTOR_BACKWARD) {
        DL_GPIO_clearPins(m->dir1_port, m->dir1_pin);
        DL_GPIO_setPins(m->dir2_port, m->dir2_pin);
    } else { /* MOTOR_STOP */
        DL_GPIO_clearPins(m->dir1_port, m->dir1_pin);
        DL_GPIO_clearPins(m->dir2_port, m->dir2_pin);
    }
}

static void motor_set_pwm_duty(uint8_t ch, uint16_t duty)
{
    DL_Timer_setCaptureCompareValue(MOTOR_PWM_INST,
        duty, g_motor[ch].pwm_cc_idx);
}

static void motor_handle_encoder(motor_ch_t *m,
    GPIO_Regs *trig_port, uint32_t trig_pin)
{
    if (trig_port == m->enc_a_port && trig_pin == m->enc_a_pin) {
        /* A 相触发 → 读 B 相电平 */
        if (DL_GPIO_readPins(m->enc_b_port, m->enc_b_pin)) {
            m->encoder_count++;  /* B=HIGH → 正转 */
        } else {
            m->encoder_count--;  /* B=LOW  → 反转 */
        }
    } else if (trig_port == m->enc_b_port && trig_pin == m->enc_b_pin) {
        /* B 相触发 → 读 A 相电平 */
        if (DL_GPIO_readPins(m->enc_a_port, m->enc_a_pin)) {
            m->encoder_count--;  /* A=HIGH → 反转 */
        } else {
            m->encoder_count++;  /* A=LOW  → 正转 */
        }
    }
}

/* ====================== API 实现 ====================== */

void Motor_Init(void)
{
    uint8_t i;

    /* ---- 初始化电机通道静态配置表 ---- */
    /* Motor A: CCP2(PB12), DIR1=PB15, DIR2=PB16, EncA=PB10, EncB=PB11 */
    g_motor[MOTOR_A].pwm_cc_idx  = DL_TIMER_CC_2_INDEX;
    g_motor[MOTOR_A].dir1_port   = MOTORA_DIR_PORT;
    g_motor[MOTOR_A].dir1_pin    = MOTORA_DIR_MOTA_DIR1_PIN;
    g_motor[MOTOR_A].dir2_port   = MOTORA_DIR_PORT;
    g_motor[MOTOR_A].dir2_pin    = MOTORA_DIR_MOTA_DIR2_PIN;
    g_motor[MOTOR_A].enc_a_port  = ENCA_PORT;
    g_motor[MOTOR_A].enc_a_pin   = ENCA_ENCA_A_PIN;
    g_motor[MOTOR_A].enc_b_port  = ENCA_PORT;
    g_motor[MOTOR_A].enc_b_pin   = ENCA_ENCA_B_PIN;

    /* Motor B: CCP3(PB13), DIR1=PB5, DIR2=PB4, EncA=PA29, EncB=PB27 */
    g_motor[MOTOR_B].pwm_cc_idx  = DL_TIMER_CC_3_INDEX;
    g_motor[MOTOR_B].dir1_port   = MOTORB_DIR_PORT;
    g_motor[MOTOR_B].dir1_pin    = MOTORB_DIR_MOTB_DIR1_PIN;
    g_motor[MOTOR_B].dir2_port   = MOTORB_DIR_PORT;
    g_motor[MOTOR_B].dir2_pin    = MOTORB_DIR_MOTB_DIR2_PIN;
    g_motor[MOTOR_B].enc_a_port  = ENCB_ENCB_A_PORT;
    g_motor[MOTOR_B].enc_a_pin   = ENCB_ENCB_A_PIN;
    g_motor[MOTOR_B].enc_b_port  = ENCB_ENCB_b_PORT;
    g_motor[MOTOR_B].enc_b_pin   = ENCB_ENCB_b_PIN;

    /* Motor C: CCP0(PA8), DIR1=PB9, DIR2=PB7, EncA=PA26, EncB=PA27 */
    g_motor[MOTOR_C].pwm_cc_idx  = DL_TIMER_CC_0_INDEX;
    g_motor[MOTOR_C].dir1_port   = MOTORC_DIR_PORT;
    g_motor[MOTOR_C].dir1_pin    = MOTORC_DIR_MOTC_DIR1_PIN;
    g_motor[MOTOR_C].dir2_port   = MOTORC_DIR_PORT;
    g_motor[MOTOR_C].dir2_pin    = MOTORC_DIR_MOTC_DIR2_PIN;
    g_motor[MOTOR_C].enc_a_port  = ENCC_PORT;
    g_motor[MOTOR_C].enc_a_pin   = ENCC_ENCC_A_PIN;
    g_motor[MOTOR_C].enc_b_port  = ENCC_PORT;
    g_motor[MOTOR_C].enc_b_pin   = ENCC_ENCC_B_PIN;

    /* Motor D: CCP1(PA9), DIR1=PB26, DIR2=PA13, EncA=PB21, EncB=PB19 */
    g_motor[MOTOR_D].pwm_cc_idx  = DL_TIMER_CC_1_INDEX;
    g_motor[MOTOR_D].dir1_port   = MOTORD_DIR_MOTD_DIR1_PORT;
    g_motor[MOTOR_D].dir1_pin    = MOTORD_DIR_MOTD_DIR1_PIN;
    g_motor[MOTOR_D].dir2_port   = MOTORD_DIR_MOTD_DIR2_PORT;
    g_motor[MOTOR_D].dir2_pin    = MOTORD_DIR_MOTD_DIR2_PIN;
    g_motor[MOTOR_D].enc_a_port  = ENCD_PORT;
    g_motor[MOTOR_D].enc_a_pin   = ENCD_ENCD_A_PIN;
    g_motor[MOTOR_D].enc_b_port  = ENCD_PORT;
    g_motor[MOTOR_D].enc_b_pin   = ENCD_ENCD_B_PIN;

    /* ---- 初始化所有通道状态 ---- */
    for (i = 0; i < MOTOR_NUM; i++) {
        g_motor[i].encoder_count      = 0;
        g_motor[i].prev_encoder_count = 0;
        g_motor[i].target_speed       = 0;
        g_motor[i].current_duty       = 0;
        g_motor[i].measured_rpm       = 0;
        g_motor[i].direction          = MOTOR_STOP;
        g_motor[i].kp                 = 0.0f;
        g_motor[i].ki                 = 0.0f;
        g_motor[i].kd                 = 0.0f;
        g_motor[i].integral           = 0.0f;
        g_motor[i].last_error         = 0;
        g_motor[i].pid_enabled        = 0;
    }

    /* ---- 初始化方向引脚为低电平 (停止状态) ---- */
    for (i = 0; i < MOTOR_NUM; i++) {
        motor_set_dir_pins(&g_motor[i], MOTOR_STOP);
    }

    /* ---- 初始化 PWM 占空比为 0 ---- */
    for (i = 0; i < MOTOR_NUM; i++) {
        motor_set_pwm_duty(i, PWM_PERIOD); /* CC=period → 输出全LOW → 电机停 (TB6612 HIGH=导通) */
    }

    /* PWM 定时器已在 SYSCFG_DL_MOTOR_PWM_init() 中启动 */

    /* ---- 启动 PID 定时器 (TIMG7, 20ms) ---- */
    DL_Timer_startCounter(PID_TIMER_INST);

    /* ---- 使能 NVIC 中断 ---- */
    NVIC_EnableIRQ(PID_TIMER_INST_INT_IRQN);
    NVIC_EnableIRQ(GPIO_MULTIPLE_GPIOB_INT_IRQN);
    NVIC_EnableIRQ(GPIO_MULTIPLE_GPIOA_INT_IRQN);
}

void Motor_SetSpeed(uint8_t ch, int16_t speed)
{
    motor_ch_t *m;

    if (ch >= MOTOR_NUM) return;
    m = &g_motor[ch];

    /* 限幅 */
    if (speed > 1000)  speed = 1000;
    if (speed < -1000) speed = -1000;

    if (m->pid_enabled) {
        /* PID 模式: 设置目标转速 */
        m->target_speed = speed;
    } else {
        /* 直接 PWM 模式 */
        uint16_t duty;
        uint8_t  dir;

        if (speed > 0) {
            dir  = MOTOR_FORWARD;
            duty = (uint16_t)(PWM_PERIOD - speed);
        } else if (speed < 0) {
            dir  = MOTOR_BACKWARD;
            duty = (uint16_t)(PWM_PERIOD + speed);
        } else {
            dir  = MOTOR_STOP;
            duty = PWM_PERIOD;
        }

        m->direction    = dir;
        m->current_duty = (int16_t)duty;
        motor_set_dir_pins(m, dir);
        motor_set_pwm_duty(ch, duty);
    }
}

void Motor_SetDirection(uint8_t ch, uint8_t dir)
{
    if (ch >= MOTOR_NUM) return;
    g_motor[ch].direction = dir;
    motor_set_dir_pins(&g_motor[ch], dir);
}

void Motor_Enable(uint8_t ch)
{
    if (ch >= MOTOR_NUM) return;
    g_motor_enabled |= (1 << ch);
}

void Motor_Disable(uint8_t ch)
{
    if (ch >= MOTOR_NUM) return;
    g_motor_enabled &= ~(1 << ch);
    motor_set_dir_pins(&g_motor[ch], MOTOR_STOP);
    motor_set_pwm_duty(ch, PWM_PERIOD);
    g_motor[ch].current_duty = 0;
}

void Motor_EnableChannels(uint8_t mask)
{
    uint8_t i;
    g_motor_enabled = mask & 0x0F;
    for (i = 0; i < MOTOR_NUM; i++) {
        if (!(g_motor_enabled & (1 << i))) {
            motor_set_dir_pins(&g_motor[i], MOTOR_STOP);
            motor_set_pwm_duty(i, PWM_PERIOD);
            g_motor[i].current_duty = 0;
        }
    }
}

int32_t Motor_GetEncoder(uint8_t ch)
{
    if (ch >= MOTOR_NUM) return 0;
    return g_motor[ch].encoder_count;
}

void Motor_ClearEncoder(uint8_t ch)
{
    if (ch >= MOTOR_NUM) return;
    g_motor[ch].encoder_count      = 0;
    g_motor[ch].prev_encoder_count = 0;
}

int32_t Motor_GetSpeed(uint8_t ch)
{
    if (ch >= MOTOR_NUM) return 0;
    return g_motor[ch].measured_rpm;
}

void Motor_SetPID(uint8_t ch, float kp, float ki, float kd)
{
    if (ch >= MOTOR_NUM) return;
    g_motor[ch].kp          = kp;
    g_motor[ch].ki          = ki;
    g_motor[ch].kd          = kd;
    g_motor[ch].integral    = 0.0f;
    g_motor[ch].last_error  = 0;
    g_motor[ch].pid_enabled = (kp != 0.0f || ki != 0.0f || kd != 0.0f) ? 1 : 0;
}

/* ====================== PID 计算任务 (每 20ms 调用) ====================== */
void Motor_PIDTask(void)
{
    uint8_t i;

    for (i = 0; i < MOTOR_NUM; i++) {
        motor_ch_t *m = &g_motor[i];

        if (!(g_motor_enabled & (1 << i))) continue;
        if (!m->pid_enabled)                 continue;

        /* ---- 读取编码器增量 ---- */
        int32_t enc_now  = m->encoder_count;
        int32_t enc_delta = enc_now - m->prev_encoder_count;
        m->prev_encoder_count = enc_now;

        /* ---- 计算当前 RPM ---- */
        /* enc_delta 是 20ms 内的计数增量 */
        /* RPM = (delta * 60s/min) / (分辨率 * 0.02s) */
        /* RPM = delta * 3000 / ENCODER_RESOLUTION */
        m->measured_rpm = (int32_t)((float)enc_delta * 3000.0f / (float)ENCODER_RESOLUTION);

        /* ---- 增量式 PID ---- */
        int32_t error    = (int32_t)m->target_speed - m->measured_rpm;
        float   p_term   = m->kp * (float)error;

        m->integral += (float)error;
        if (m->integral > PID_INTEGRAL_MAX)  m->integral = PID_INTEGRAL_MAX;
        if (m->integral < -PID_INTEGRAL_MAX) m->integral = -PID_INTEGRAL_MAX;
        float i_term = m->ki * m->integral;

        float d_term = m->kd * (float)(error - m->last_error);
        m->last_error = error;

        float output = p_term + i_term + d_term;

        if (output > PID_OUTPUT_MAX)  output = PID_OUTPUT_MAX;
        if (output < PID_OUTPUT_MIN)  output = PID_OUTPUT_MIN;

        int16_t pwm_val;
        uint8_t dir;

        if (output > 0) {
            dir     = MOTOR_FORWARD;
            pwm_val = (int16_t)((float)PWM_PERIOD - output);
        } else if (output < 0) {
            dir     = MOTOR_BACKWARD;
            pwm_val = (int16_t)((float)PWM_PERIOD + output);
        } else {
            dir     = MOTOR_STOP;
            pwm_val = PWM_PERIOD;
        }

        if (pwm_val < 0)       pwm_val = 0;
        if (pwm_val > PWM_PERIOD) pwm_val = PWM_PERIOD;

        m->direction    = dir;
        m->current_duty = pwm_val;
        motor_set_dir_pins(m, dir);
        motor_set_pwm_duty(i, (uint16_t)pwm_val);
    }
}

/* ====================== 中断服务函数 ====================== */

/*
 * GROUP1_IRQHandler - 处理 GPIOA / GPIOB 中断 (编码器 A/B 相)
 *
 * MSPM0G3507 上 GPIOA 和 GPIOB 均属于 DL_INTERRUPT_GROUP_1.
 * 通过 DL_Interrupt_getPendingGroup() 获取端口级 IIDX,
 * 再用 DL_GPIO_getEnabledInterruptStatus() 获取具体引脚.
 */
void GROUP1_IRQHandler(void)
{
    uint32_t iidx = DL_Interrupt_getPendingGroup(DL_INTERRUPT_GROUP_1);

    switch (iidx) {
        case GPIO_MULTIPLE_GPIOB_INT_IIDX: {
            uint32_t status = DL_GPIO_getEnabledInterruptStatus(GPIOB, 0xFFFF);
            uint8_t  ch;

            if (status == 0) break;

            for (ch = 0; ch < MOTOR_NUM; ch++) {
                motor_ch_t *m = &g_motor[ch];

                if (m->enc_a_port == GPIOB && (status & m->enc_a_pin)) {
                    motor_handle_encoder(m, GPIOB, m->enc_a_pin);
                }
                if (m->enc_b_port == GPIOB && (status & m->enc_b_pin)) {
                    motor_handle_encoder(m, GPIOB, m->enc_b_pin);
                }
            }

            DL_GPIO_clearInterruptStatus(GPIOB, status);
            break;
        }

        case GPIO_MULTIPLE_GPIOA_INT_IIDX: {
            uint32_t status = DL_GPIO_getEnabledInterruptStatus(GPIOA, 0xFFFF);
            uint8_t  ch;

            if (status == 0) break;

            for (ch = 0; ch < MOTOR_NUM; ch++) {
                motor_ch_t *m = &g_motor[ch];

                if (m->enc_a_port == GPIOA && (status & m->enc_a_pin)) {
                    motor_handle_encoder(m, GPIOA, m->enc_a_pin);
                }
                if (m->enc_b_port == GPIOA && (status & m->enc_b_pin)) {
                    motor_handle_encoder(m, GPIOA, m->enc_b_pin);
                }
            }

            DL_GPIO_clearInterruptStatus(GPIOA, status);
            break;
        }

        default:
            break;
    }
}

/*
 * TIMG7_IRQHandler - PID 定时器中断 (20ms)
 */
void TIMG7_IRQHandler(void)
{
    switch (DL_Timer_getPendingInterrupt(PID_TIMER_INST)) {
        case DL_TIMER_INTERRUPT_ZERO_EVENT:
            DL_Timer_clearInterruptStatus(PID_TIMER_INST,
                DL_TIMER_INTERRUPT_ZERO_EVENT);
            Motor_PIDTask();
            break;
        default:
            break;
    }
}
