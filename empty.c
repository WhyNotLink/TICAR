/*
 * 天猛星 MSPM0G3507 小车底座 主程序
 *
 * 使用的外设驱动:
 *   TB6612 + MG513直流电机 (bsp_motor)
 *   A4988 步进电机 (bsp_a4988)
 *   MPU6050 陀螺仪 (bsp_mpu6050)
 *   OLED 0.96寸IIC屏 (soft_oled)
 *   HC-SR04 超声波 (bsp_ultrasonic)
 *   灰度传感器 8ch (bsp_grayscale)
 *   按键 2个 (bsp_button)
 *   LED 4个 (bsp_led)
 *   蜂鸣器 (bsp_buzzer)
 *
 * 注意: 7合1拨码开关是硬件直连各模块EN脚，无需MCU参与。
 *       PTZ1_EN / PTZ2_EN / MAB_EN / MCD_EN / MS1/MS2/MS3
 *       直接通过高低电平控制模块使能和步进细分。
 */

#include "ti_msp_dl_config.h"
#include "bsp/board.h"
#include "bsp/soft_oled.h"
#include "bsp/bsp_motor.h"
#include "bsp/bsp_a4988.h"
#include "bsp/bsp_mpu6050.h"
#include "bsp/bsp_ultrasonic.h"
#include "bsp/bsp_grayscale.h"
#include "bsp/bsp_button.h"
#include "bsp/bsp_led.h"
#include "bsp/bsp_buzzer.h"

int main(void)
{
    /* ---- 系统初始化 ---- */
    SYSCFG_DL_init();

    /* ---- 外设初始化 ---- */
    OLED_Init();                    // OLED屏幕
    OLED_Clear();
    OLED_ShowString(0, 0, (uint8_t *)"Car Init...", 16, 1);

    Motor_Init();                   // 直流电机(默认启用A和C，AB/CD的硬件EN由开关控制)
    A4988_Init();                   // 步进电机(硬件EN和微步细分由开关直连)
    MPU6050_Init();                 // 陀螺仪
    Ultrasonic_Init();              // 超声波
    LED_Init();                     // LED指示灯
    Buzzer_Init();                  // 蜂鸣器
    Button_Init();                  // 按键
    /* Grayscale 由 SYSCFG_DL_GPIO_init 自动初始化 */

    /* ---- 启动提示 ---- */
    Buzzer_Beep(100);
    OLED_Clear();
    OLED_ShowString(0, 0, (uint8_t *)"Car Ready!", 16, 1);

    while (1)
    {
        /* ====== 通用定时任务(各自内部计时) ====== */
        Button_Task();              // 按键消抖(每10ms调用)

        /* ====== 直流电机控制(需要PID闭环的话在定时器中断里调用) ====== */
        /*
         * Motor_PIDTask() 在 TIMG7_IRQHandler 中由 bsp_motor.c 自动调用
         * 如需直接控制:
         *   Motor_SetSpeed(MOTOR_A, 500);   // A电机 50%正向
         *   Motor_SetSpeed(MOTOR_C, -300);  // C电机 30%反向
         *
         * AB/CD 的硬件 EN 由拨码开关 MAB_EN / MCD_EN 直连 TB6612
         * 软件侧只需设置方向和PWM即可，无需额外使能操作。
         */

        /* ====== 步进电机控制(硬件EN和MS细分由开关直连A4988) ====== */
        /*
         * A4988_Enable(PTZ1);               // 软件侧开启脉冲输出
         * A4988_SetDirection(PTZ1, A4988_DIR_CW);
         * A4988_SetSpeed(PTZ1, 1000);
         */

        /* ====== 按键测试 ====== */
        if (Button_GetPress(0)) {   // KEY1 按下
            LED_Toggle(LED1);
            Buzzer_Beep(50);
        }
        if (Button_GetPress(1)) {   // KEY2 按下
            LED_Toggle(LED2);
        }

        /* ====== 传感器读取 ====== */
        MPU6050_Update();                   // 更新IMU数据
        float dist = Ultrasonic_GetDistance();  // 超声波测距

        /* ====== OLED 显示(每200ms刷新) ====== */
        static uint32_t tick_counter = 0;
        tick_counter++;
        if (tick_counter % 20000 == 0) {   // 约200ms
            OLED_ShowString(0, 2, (uint8_t *)"Dist:", 8, 1);
            OLED_ShowNum(40, 2, (uint32_t)dist, 3, 8, 1);

            OLED_ShowString(0, 4, (uint8_t *)"ENC A:", 8, 1);
            OLED_ShowNum(40, 4, Motor_GetEncoder(MOTOR_A), 7, 8, 1);

            OLED_Refresh();
        }
    }
}

/*
 * 中断说明:
 *
 *   TIMG7_IRQHandler   → bsp_motor.c 已实现 (PID 20ms定时)
  *   GROUP1_IRQHandler  → bsp_motor.c 已实现 (编码器中断)
  *   超声波使用 TIMG12 硬件计数器轮询，不占用中断。
 */
