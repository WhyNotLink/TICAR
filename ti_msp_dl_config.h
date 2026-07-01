/*
 * Copyright (c) 2023, Texas Instruments Incorporated - http://www.ti.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ============ ti_msp_dl_config.h =============
 *  Configured MSPM0 DriverLib module declarations
 *
 *  DO NOT EDIT - This file is generated for the MSPM0G350X
 *  by the SysConfig tool.
 */
#ifndef ti_msp_dl_config_h
#define ti_msp_dl_config_h

#define CONFIG_MSPM0G350X
#define CONFIG_MSPM0G3507

#if defined(__ti_version__) || defined(__TI_COMPILER_VERSION__)
#define SYSCONFIG_WEAK __attribute__((weak))
#elif defined(__IAR_SYSTEMS_ICC__)
#define SYSCONFIG_WEAK __weak
#elif defined(__GNUC__)
#define SYSCONFIG_WEAK __attribute__((weak))
#endif

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  ======== SYSCFG_DL_init ========
 *  Perform all required MSP DL initialization
 *
 *  This function should be called once at a point before any use of
 *  MSP DL.
 */


/* clang-format off */

#define POWER_STARTUP_DELAY                                                (16)


#define CPUCLK_FREQ                                                     32000000



/* Defines for MOTOR_PWM */
#define MOTOR_PWM_INST                                                     TIMA0
#define MOTOR_PWM_INST_IRQHandler                               TIMA0_IRQHandler
#define MOTOR_PWM_INST_INT_IRQN                                 (TIMA0_INT_IRQn)
#define MOTOR_PWM_INST_CLK_FREQ                                          1000000
/* GPIO defines for channel 0 */
#define GPIO_MOTOR_PWM_C0_PORT                                             GPIOA
#define GPIO_MOTOR_PWM_C0_PIN                                      DL_GPIO_PIN_8
#define GPIO_MOTOR_PWM_C0_IOMUX                                  (IOMUX_PINCM19)
#define GPIO_MOTOR_PWM_C0_IOMUX_FUNC                 IOMUX_PINCM19_PF_TIMA0_CCP0
#define GPIO_MOTOR_PWM_C0_IDX                                DL_TIMER_CC_0_INDEX
/* GPIO defines for channel 1 */
#define GPIO_MOTOR_PWM_C1_PORT                                             GPIOA
#define GPIO_MOTOR_PWM_C1_PIN                                      DL_GPIO_PIN_9
#define GPIO_MOTOR_PWM_C1_IOMUX                                  (IOMUX_PINCM20)
#define GPIO_MOTOR_PWM_C1_IOMUX_FUNC                 IOMUX_PINCM20_PF_TIMA0_CCP1
#define GPIO_MOTOR_PWM_C1_IDX                                DL_TIMER_CC_1_INDEX
/* GPIO defines for channel 2 */
#define GPIO_MOTOR_PWM_C2_PORT                                             GPIOB
#define GPIO_MOTOR_PWM_C2_PIN                                     DL_GPIO_PIN_12
#define GPIO_MOTOR_PWM_C2_IOMUX                                  (IOMUX_PINCM29)
#define GPIO_MOTOR_PWM_C2_IOMUX_FUNC                 IOMUX_PINCM29_PF_TIMA0_CCP2
#define GPIO_MOTOR_PWM_C2_IDX                                DL_TIMER_CC_2_INDEX
/* GPIO defines for channel 3 */
#define GPIO_MOTOR_PWM_C3_PORT                                             GPIOB
#define GPIO_MOTOR_PWM_C3_PIN                                     DL_GPIO_PIN_13
#define GPIO_MOTOR_PWM_C3_IOMUX                                  (IOMUX_PINCM30)
#define GPIO_MOTOR_PWM_C3_IOMUX_FUNC                 IOMUX_PINCM30_PF_TIMA0_CCP3
#define GPIO_MOTOR_PWM_C3_IDX                                DL_TIMER_CC_3_INDEX

/* Defines for PTZ1_PWM */
#define PTZ1_PWM_INST                                                      TIMA1
#define PTZ1_PWM_INST_IRQHandler                                TIMA1_IRQHandler
#define PTZ1_PWM_INST_INT_IRQN                                  (TIMA1_INT_IRQn)
#define PTZ1_PWM_INST_CLK_FREQ                                             40000
/* GPIO defines for channel 0 */
#define GPIO_PTZ1_PWM_C0_PORT                                              GPIOB
#define GPIO_PTZ1_PWM_C0_PIN                                       DL_GPIO_PIN_2
#define GPIO_PTZ1_PWM_C0_IOMUX                                   (IOMUX_PINCM15)
#define GPIO_PTZ1_PWM_C0_IOMUX_FUNC                  IOMUX_PINCM15_PF_TIMA1_CCP0
#define GPIO_PTZ1_PWM_C0_IDX                                 DL_TIMER_CC_0_INDEX

/* Defines for PTZ2_PWM */
#define PTZ2_PWM_INST                                                      TIMG6
#define PTZ2_PWM_INST_IRQHandler                                TIMG6_IRQHandler
#define PTZ2_PWM_INST_INT_IRQN                                  (TIMG6_INT_IRQn)
#define PTZ2_PWM_INST_CLK_FREQ                                             40000
/* GPIO defines for channel 1 */
#define GPIO_PTZ2_PWM_C1_PORT                                              GPIOB
#define GPIO_PTZ2_PWM_C1_PIN                                       DL_GPIO_PIN_3
#define GPIO_PTZ2_PWM_C1_IOMUX                                   (IOMUX_PINCM16)
#define GPIO_PTZ2_PWM_C1_IOMUX_FUNC                  IOMUX_PINCM16_PF_TIMG6_CCP1
#define GPIO_PTZ2_PWM_C1_IDX                                 DL_TIMER_CC_1_INDEX



/* Defines for ULTRASONIC_TIMER */
#define ULTRASONIC_TIMER_INST                                           (TIMG12)
#define ULTRASONIC_TIMER_INST_IRQHandler                       TIMG12_IRQHandler
#define ULTRASONIC_TIMER_INST_INT_IRQN                         (TIMG12_INT_IRQn)
#define ULTRASONIC_TIMER_INST_LOAD_VALUE                               (159999U)
/* GPIO defines for channel 0 */
#define GPIO_ULTRASONIC_TIMER_C0_PORT                                      GPIOB
#define GPIO_ULTRASONIC_TIMER_C0_PIN                              DL_GPIO_PIN_20
#define GPIO_ULTRASONIC_TIMER_C0_IOMUX                           (IOMUX_PINCM48)
#define GPIO_ULTRASONIC_TIMER_C0_IOMUX_FUNC            IOMUX_PINCM48_PF_TIMG12_CCP0





/* Defines for PID_TIMER */
#define PID_TIMER_INST                                                   (TIMG7)
#define PID_TIMER_INST_IRQHandler                               TIMG7_IRQHandler
#define PID_TIMER_INST_INT_IRQN                                 (TIMG7_INT_IRQn)
#define PID_TIMER_INST_LOAD_VALUE                                        (9999U)




/* Defines for I2C_0 */
#define I2C_0_INST                                                          I2C0
#define I2C_0_INST_IRQHandler                                    I2C0_IRQHandler
#define I2C_0_INST_INT_IRQN                                        I2C0_INT_IRQn
#define GPIO_I2C_0_SDA_PORT                                                GPIOA
#define GPIO_I2C_0_SDA_PIN                                        DL_GPIO_PIN_28
#define GPIO_I2C_0_IOMUX_SDA                                      (IOMUX_PINCM3)
#define GPIO_I2C_0_IOMUX_SDA_FUNC                       IOMUX_PINCM3_PF_I2C0_SDA
#define GPIO_I2C_0_SCL_PORT                                                GPIOA
#define GPIO_I2C_0_SCL_PIN                                        DL_GPIO_PIN_31
#define GPIO_I2C_0_IOMUX_SCL                                      (IOMUX_PINCM6)
#define GPIO_I2C_0_IOMUX_SCL_FUNC                       IOMUX_PINCM6_PF_I2C0_SCL


/* Defines for UART0 */
#define UART0_INST                                                         UART0
#define UART0_INST_FREQUENCY                                            32000000
#define UART0_INST_IRQHandler                                   UART0_IRQHandler
#define UART0_INST_INT_IRQN                                       UART0_INT_IRQn
#define GPIO_UART0_RX_PORT                                                 GPIOA
#define GPIO_UART0_TX_PORT                                                 GPIOA
#define GPIO_UART0_RX_PIN                                         DL_GPIO_PIN_11
#define GPIO_UART0_TX_PIN                                         DL_GPIO_PIN_10
#define GPIO_UART0_IOMUX_RX                                      (IOMUX_PINCM22)
#define GPIO_UART0_IOMUX_TX                                      (IOMUX_PINCM21)
#define GPIO_UART0_IOMUX_RX_FUNC                       IOMUX_PINCM22_PF_UART0_RX
#define GPIO_UART0_IOMUX_TX_FUNC                       IOMUX_PINCM21_PF_UART0_TX
#define UART0_BAUD_RATE                                                   (9600)
#define UART0_IBRD_32_MHZ_9600_BAUD                                        (208)
#define UART0_FBRD_32_MHZ_9600_BAUD                                         (21)





/* Defines for DMA_CH0 */
#define DMA_CH0_CHAN_ID                                                      (1)
#define UART0_INST_DMA_TRIGGER_0                             (DMA_UART0_RX_TRIG)

/* Defines for DMA_CH1 */
#define DMA_CH1_CHAN_ID                                                      (0)
#define UART0_INST_DMA_TRIGGER_1                             (DMA_UART0_TX_TRIG)



/* Port definition for Pin Group ULTRASONIC */
#define ULTRASONIC_PORT                                                  (GPIOB)

/* Defines for TRIG: GPIOB.25 with pinCMx 56 on package pin 27 */
#define ULTRASONIC_TRIG_PIN                                     (DL_GPIO_PIN_25)
#define ULTRASONIC_TRIG_IOMUX                                    (IOMUX_PINCM56)
/* Port definition for Pin Group BEEP_GPIO */
#define BEEP_GPIO_PORT                                                   (GPIOA)

/* Defines for BEEP_PIN: GPIOA.7 with pinCMx 14 on package pin 49 */
#define BEEP_GPIO_BEEP_PIN_PIN                                   (DL_GPIO_PIN_7)
#define BEEP_GPIO_BEEP_PIN_IOMUX                                 (IOMUX_PINCM14)
/* Port definition for Pin Group MOTORA_DIR */
#define MOTORA_DIR_PORT                                                  (GPIOB)

/* Defines for MOTA_DIR1: GPIOB.15 with pinCMx 32 on package pin 3 */
#define MOTORA_DIR_MOTA_DIR1_PIN                                (DL_GPIO_PIN_15)
#define MOTORA_DIR_MOTA_DIR1_IOMUX                               (IOMUX_PINCM32)
/* Defines for MOTA_DIR2: GPIOB.16 with pinCMx 33 on package pin 4 */
#define MOTORA_DIR_MOTA_DIR2_PIN                                (DL_GPIO_PIN_16)
#define MOTORA_DIR_MOTA_DIR2_IOMUX                               (IOMUX_PINCM33)
/* Port definition for Pin Group MOTORB_DIR */
#define MOTORB_DIR_PORT                                                  (GPIOB)

/* Defines for MOTB_DIR1: GPIOB.5 with pinCMx 18 on package pin 53 */
#define MOTORB_DIR_MOTB_DIR1_PIN                                 (DL_GPIO_PIN_5)
#define MOTORB_DIR_MOTB_DIR1_IOMUX                               (IOMUX_PINCM18)
/* Defines for MOTB_DIR2: GPIOB.4 with pinCMx 17 on package pin 52 */
#define MOTORB_DIR_MOTB_DIR2_PIN                                 (DL_GPIO_PIN_4)
#define MOTORB_DIR_MOTB_DIR2_IOMUX                               (IOMUX_PINCM17)
/* Port definition for Pin Group MOTORC_DIR */
#define MOTORC_DIR_PORT                                                  (GPIOB)

/* Defines for MOTC_DIR1: GPIOB.9 with pinCMx 26 on package pin 61 */
#define MOTORC_DIR_MOTC_DIR1_PIN                                 (DL_GPIO_PIN_9)
#define MOTORC_DIR_MOTC_DIR1_IOMUX                               (IOMUX_PINCM26)
/* Defines for MOTC_DIR2: GPIOB.7 with pinCMx 24 on package pin 59 */
#define MOTORC_DIR_MOTC_DIR2_PIN                                 (DL_GPIO_PIN_7)
#define MOTORC_DIR_MOTC_DIR2_IOMUX                               (IOMUX_PINCM24)
/* Defines for MOTD_DIR1: GPIOB.26 with pinCMx 57 on package pin 28 */
#define MOTORD_DIR_MOTD_DIR1_PORT                                        (GPIOB)
#define MOTORD_DIR_MOTD_DIR1_PIN                                (DL_GPIO_PIN_26)
#define MOTORD_DIR_MOTD_DIR1_IOMUX                               (IOMUX_PINCM57)
/* Defines for MOTD_DIR2: GPIOA.13 with pinCMx 35 on package pin 6 */
#define MOTORD_DIR_MOTD_DIR2_PORT                                        (GPIOA)
#define MOTORD_DIR_MOTD_DIR2_PIN                                (DL_GPIO_PIN_13)
#define MOTORD_DIR_MOTD_DIR2_IOMUX                               (IOMUX_PINCM35)
/* Port definition for Pin Group ENCA */
#define ENCA_PORT                                                        (GPIOB)

/* Defines for ENCA_A: GPIOB.10 with pinCMx 27 on package pin 62 */
// groups represented: ["ENCB","ENCD","ENCA"]
// pins affected: ["ENCB_b","ENCD_A","ENCD_B","ENCA_A","ENCA_B"]
#define GPIO_MULTIPLE_GPIOB_INT_IRQN                            (GPIOB_INT_IRQn)
#define GPIO_MULTIPLE_GPIOB_INT_IIDX            (DL_INTERRUPT_GROUP1_IIDX_GPIOB)
#define ENCA_ENCA_A_IIDX                                    (DL_GPIO_IIDX_DIO10)
#define ENCA_ENCA_A_PIN                                         (DL_GPIO_PIN_10)
#define ENCA_ENCA_A_IOMUX                                        (IOMUX_PINCM27)
/* Defines for ENCA_B: GPIOB.11 with pinCMx 28 on package pin 63 */
#define ENCA_ENCA_B_IIDX                                    (DL_GPIO_IIDX_DIO11)
#define ENCA_ENCA_B_PIN                                         (DL_GPIO_PIN_11)
#define ENCA_ENCA_B_IOMUX                                        (IOMUX_PINCM28)
/* Defines for ENCB_A: GPIOA.29 with pinCMx 4 on package pin 36 */
#define ENCB_ENCB_A_PORT                                                 (GPIOA)
// groups represented: ["ENCC","KEY","ENCB"]
// pins affected: ["ENCC_A","ENCC_B","KEY1","KEY2","ENCB_A"]
#define GPIO_MULTIPLE_GPIOA_INT_IRQN                            (GPIOA_INT_IRQn)
#define GPIO_MULTIPLE_GPIOA_INT_IIDX            (DL_INTERRUPT_GROUP1_IIDX_GPIOA)
#define ENCB_ENCB_A_IIDX                                    (DL_GPIO_IIDX_DIO29)
#define ENCB_ENCB_A_PIN                                         (DL_GPIO_PIN_29)
#define ENCB_ENCB_A_IOMUX                                         (IOMUX_PINCM4)
/* Defines for ENCB_b: GPIOB.27 with pinCMx 58 on package pin 29 */
#define ENCB_ENCB_b_PORT                                                 (GPIOB)
#define ENCB_ENCB_b_IIDX                                    (DL_GPIO_IIDX_DIO27)
#define ENCB_ENCB_b_PIN                                         (DL_GPIO_PIN_27)
#define ENCB_ENCB_b_IOMUX                                        (IOMUX_PINCM58)
/* Port definition for Pin Group ENCC */
#define ENCC_PORT                                                        (GPIOA)

/* Defines for ENCC_A: GPIOA.26 with pinCMx 59 on package pin 30 */
#define ENCC_ENCC_A_IIDX                                    (DL_GPIO_IIDX_DIO26)
#define ENCC_ENCC_A_PIN                                         (DL_GPIO_PIN_26)
#define ENCC_ENCC_A_IOMUX                                        (IOMUX_PINCM59)
/* Defines for ENCC_B: GPIOA.27 with pinCMx 60 on package pin 31 */
#define ENCC_ENCC_B_IIDX                                    (DL_GPIO_IIDX_DIO27)
#define ENCC_ENCC_B_PIN                                         (DL_GPIO_PIN_27)
#define ENCC_ENCC_B_IOMUX                                        (IOMUX_PINCM60)
/* Port definition for Pin Group ENCD */
#define ENCD_PORT                                                        (GPIOB)

/* Defines for ENCD_A: GPIOB.21 with pinCMx 49 on package pin 20 */
#define ENCD_ENCD_A_IIDX                                    (DL_GPIO_IIDX_DIO21)
#define ENCD_ENCD_A_PIN                                         (DL_GPIO_PIN_21)
#define ENCD_ENCD_A_IOMUX                                        (IOMUX_PINCM49)
/* Defines for ENCD_B: GPIOB.19 with pinCMx 45 on package pin 16 */
#define ENCD_ENCD_B_IIDX                                    (DL_GPIO_IIDX_DIO19)
#define ENCD_ENCD_B_PIN                                         (DL_GPIO_PIN_19)
#define ENCD_ENCD_B_IOMUX                                        (IOMUX_PINCM45)
/* Defines for PTZ1_DIR: GPIOA.12 with pinCMx 34 on package pin 5 */
#define PTZ_DIR_PTZ1_DIR_PORT                                            (GPIOA)
#define PTZ_DIR_PTZ1_DIR_PIN                                    (DL_GPIO_PIN_12)
#define PTZ_DIR_PTZ1_DIR_IOMUX                                   (IOMUX_PINCM34)
/* Defines for PTZ2_DIR: GPIOB.23 with pinCMx 51 on package pin 22 */
#define PTZ_DIR_PTZ2_DIR_PORT                                            (GPIOB)
#define PTZ_DIR_PTZ2_DIR_PIN                                    (DL_GPIO_PIN_23)
#define PTZ_DIR_PTZ2_DIR_IOMUX                                   (IOMUX_PINCM51)
/* Defines for OUT1: GPIOA.24 with pinCMx 54 on package pin 25 */
#define GRAY_OUT1_PORT                                                   (GPIOA)
#define GRAY_OUT1_PIN                                           (DL_GPIO_PIN_24)
#define GRAY_OUT1_IOMUX                                          (IOMUX_PINCM54)
/* Defines for OUT2: GPIOA.25 with pinCMx 55 on package pin 26 */
#define GRAY_OUT2_PORT                                                   (GPIOA)
#define GRAY_OUT2_PIN                                           (DL_GPIO_PIN_25)
#define GRAY_OUT2_IOMUX                                          (IOMUX_PINCM55)
/* Defines for OUT3: GPIOB.24 with pinCMx 52 on package pin 23 */
#define GRAY_OUT3_PORT                                                   (GPIOB)
#define GRAY_OUT3_PIN                                           (DL_GPIO_PIN_24)
#define GRAY_OUT3_IOMUX                                          (IOMUX_PINCM52)
/* Defines for OUT4: GPIOA.22 with pinCMx 47 on package pin 18 */
#define GRAY_OUT4_PORT                                                   (GPIOA)
#define GRAY_OUT4_PIN                                           (DL_GPIO_PIN_22)
#define GRAY_OUT4_IOMUX                                          (IOMUX_PINCM47)
/* Defines for OUT5: GPIOA.14 with pinCMx 36 on package pin 7 */
#define GRAY_OUT5_PORT                                                   (GPIOA)
#define GRAY_OUT5_PIN                                           (DL_GPIO_PIN_14)
#define GRAY_OUT5_IOMUX                                          (IOMUX_PINCM36)
/* Defines for OUT6: GPIOA.17 with pinCMx 39 on package pin 10 */
#define GRAY_OUT6_PORT                                                   (GPIOA)
#define GRAY_OUT6_PIN                                           (DL_GPIO_PIN_17)
#define GRAY_OUT6_IOMUX                                          (IOMUX_PINCM39)
/* Defines for OUT7: GPIOB.17 with pinCMx 43 on package pin 14 */
#define GRAY_OUT7_PORT                                                   (GPIOB)
#define GRAY_OUT7_PIN                                           (DL_GPIO_PIN_17)
#define GRAY_OUT7_IOMUX                                          (IOMUX_PINCM43)
/* Defines for OUT8: GPIOB.18 with pinCMx 44 on package pin 15 */
#define GRAY_OUT8_PORT                                                   (GPIOB)
#define GRAY_OUT8_PIN                                           (DL_GPIO_PIN_18)
#define GRAY_OUT8_IOMUX                                          (IOMUX_PINCM44)
/* Port definition for Pin Group KEY */
#define KEY_PORT                                                         (GPIOA)

/* Defines for KEY1: GPIOA.0 with pinCMx 1 on package pin 33 */
#define KEY_KEY1_IIDX                                        (DL_GPIO_IIDX_DIO0)
#define KEY_KEY1_PIN                                             (DL_GPIO_PIN_0)
#define KEY_KEY1_IOMUX                                            (IOMUX_PINCM1)
/* Defines for KEY2: GPIOA.1 with pinCMx 2 on package pin 34 */
#define KEY_KEY2_IIDX                                        (DL_GPIO_IIDX_DIO1)
#define KEY_KEY2_PIN                                             (DL_GPIO_PIN_1)
#define KEY_KEY2_IOMUX                                            (IOMUX_PINCM2)
/* Defines for LED1_PIN: GPIOB.22 with pinCMx 50 on package pin 21 */
#define LED12_LED1_PIN_PORT                                              (GPIOB)
#define LED12_LED1_PIN_PIN                                      (DL_GPIO_PIN_22)
#define LED12_LED1_PIN_IOMUX                                     (IOMUX_PINCM50)
/* Defines for LED2_PIN: GPIOA.30 with pinCMx 5 on package pin 37 */
#define LED12_LED2_PIN_PORT                                              (GPIOA)
#define LED12_LED2_PIN_PIN                                      (DL_GPIO_PIN_30)
#define LED12_LED2_PIN_IOMUX                                      (IOMUX_PINCM5)
/* Port definition for Pin Group LED34 */
#define LED34_PORT                                                       (GPIOB)

/* Defines for LED3_PIN: GPIOB.0 with pinCMx 12 on package pin 47 */
#define LED34_LED3_PIN_PIN                                       (DL_GPIO_PIN_0)
#define LED34_LED3_PIN_IOMUX                                     (IOMUX_PINCM12)
/* Defines for LED4_PIN: GPIOB.1 with pinCMx 13 on package pin 48 */
#define LED34_LED4_PIN_PIN                                       (DL_GPIO_PIN_1)
#define LED34_LED4_PIN_IOMUX                                     (IOMUX_PINCM13)
/* Port definition for Pin Group OLED */
#define OLED_PORT                                                        (GPIOA)

/* Defines for OLED_SDA: GPIOA.15 with pinCMx 37 on package pin 8 */
#define OLED_OLED_SDA_PIN                                       (DL_GPIO_PIN_15)
#define OLED_OLED_SDA_IOMUX                                      (IOMUX_PINCM37)
/* Defines for OLED_SCL: GPIOA.16 with pinCMx 38 on package pin 9 */
#define OLED_OLED_SCL_PIN                                       (DL_GPIO_PIN_16)
#define OLED_OLED_SCL_IOMUX                                      (IOMUX_PINCM38)

/* clang-format on */

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);
void SYSCFG_DL_MOTOR_PWM_init(void);
void SYSCFG_DL_PTZ1_PWM_init(void);
void SYSCFG_DL_PTZ2_PWM_init(void);
void SYSCFG_DL_ULTRASONIC_TIMER_init(void);
void SYSCFG_DL_PID_TIMER_init(void);
void SYSCFG_DL_I2C_0_init(void);
void SYSCFG_DL_UART0_init(void);
void SYSCFG_DL_DMA_init(void);


bool SYSCFG_DL_saveConfiguration(void);
bool SYSCFG_DL_restoreConfiguration(void);

#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */
