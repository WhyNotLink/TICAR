/*
 * Copyright (c) 2023, Texas Instruments Incorporated
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
 *  ============ ti_msp_dl_config.c =============
 *  Configured MSPM0 DriverLib module definitions
 *
 *  DO NOT EDIT - This file is generated for the MSPM0G350X
 *  by the SysConfig tool.
 */

#include "ti_msp_dl_config.h"

DL_TimerA_backupConfig gMOTOR_PWMBackup;
DL_TimerA_backupConfig gPTZ1_PWMBackup;
DL_TimerG_backupConfig gPTZ2_PWMBackup;
DL_TimerG_backupConfig gPID_TIMERBackup;

/*
 *  ======== SYSCFG_DL_init ========
 *  Perform any initialization needed before using any board APIs
 */
SYSCONFIG_WEAK void SYSCFG_DL_init(void)
{
    SYSCFG_DL_initPower();
    SYSCFG_DL_GPIO_init();
    /* Module-Specific Initializations*/
    SYSCFG_DL_SYSCTL_init();
    SYSCFG_DL_MOTOR_PWM_init();
    SYSCFG_DL_PTZ1_PWM_init();
    SYSCFG_DL_PTZ2_PWM_init();
    SYSCFG_DL_ULTRASONIC_TIMER_init();
    SYSCFG_DL_PID_TIMER_init();
    SYSCFG_DL_I2C_0_init();
    SYSCFG_DL_UART0_init();
    SYSCFG_DL_DMA_init();
    /* Ensure backup structures have no valid state */
	gMOTOR_PWMBackup.backupRdy 	= false;
	gPTZ1_PWMBackup.backupRdy 	= false;
	gPTZ2_PWMBackup.backupRdy 	= false;

	gPID_TIMERBackup.backupRdy 	= false;


}
/*
 * User should take care to save and restore register configuration in application.
 * See Retention Configuration section for more details.
 */
SYSCONFIG_WEAK bool SYSCFG_DL_saveConfiguration(void)
{
    bool retStatus = true;

	retStatus &= DL_TimerA_saveConfiguration(MOTOR_PWM_INST, &gMOTOR_PWMBackup);
	retStatus &= DL_TimerA_saveConfiguration(PTZ1_PWM_INST, &gPTZ1_PWMBackup);
	retStatus &= DL_TimerG_saveConfiguration(PTZ2_PWM_INST, &gPTZ2_PWMBackup);
	retStatus &= DL_TimerG_saveConfiguration(PID_TIMER_INST, &gPID_TIMERBackup);

    return retStatus;
}


SYSCONFIG_WEAK bool SYSCFG_DL_restoreConfiguration(void)
{
    bool retStatus = true;

	retStatus &= DL_TimerA_restoreConfiguration(MOTOR_PWM_INST, &gMOTOR_PWMBackup, false);
	retStatus &= DL_TimerA_restoreConfiguration(PTZ1_PWM_INST, &gPTZ1_PWMBackup, false);
	retStatus &= DL_TimerG_restoreConfiguration(PTZ2_PWM_INST, &gPTZ2_PWMBackup, false);
	retStatus &= DL_TimerG_restoreConfiguration(PID_TIMER_INST, &gPID_TIMERBackup, false);

    return retStatus;
}

SYSCONFIG_WEAK void SYSCFG_DL_initPower(void)
{
    DL_GPIO_reset(GPIOA);
    DL_GPIO_reset(GPIOB);
    DL_TimerA_reset(MOTOR_PWM_INST);
    DL_TimerA_reset(PTZ1_PWM_INST);
    DL_TimerG_reset(PTZ2_PWM_INST);
    DL_TimerG_reset(ULTRASONIC_TIMER_INST);
    DL_TimerG_reset(PID_TIMER_INST);
    DL_I2C_reset(I2C_0_INST);
    DL_UART_Main_reset(UART0_INST);


    DL_GPIO_enablePower(GPIOA);
    DL_GPIO_enablePower(GPIOB);
    DL_TimerA_enablePower(MOTOR_PWM_INST);
    DL_TimerA_enablePower(PTZ1_PWM_INST);
    DL_TimerG_enablePower(PTZ2_PWM_INST);
    DL_TimerG_enablePower(ULTRASONIC_TIMER_INST);
    DL_TimerG_enablePower(PID_TIMER_INST);
    DL_I2C_enablePower(I2C_0_INST);
    DL_UART_Main_enablePower(UART0_INST);

    delay_cycles(POWER_STARTUP_DELAY);
}

SYSCONFIG_WEAK void SYSCFG_DL_GPIO_init(void)
{

    DL_GPIO_initPeripheralOutputFunction(GPIO_MOTOR_PWM_C0_IOMUX,GPIO_MOTOR_PWM_C0_IOMUX_FUNC);
    DL_GPIO_enableOutput(GPIO_MOTOR_PWM_C0_PORT, GPIO_MOTOR_PWM_C0_PIN);
    DL_GPIO_initPeripheralOutputFunction(GPIO_MOTOR_PWM_C1_IOMUX,GPIO_MOTOR_PWM_C1_IOMUX_FUNC);
    DL_GPIO_enableOutput(GPIO_MOTOR_PWM_C1_PORT, GPIO_MOTOR_PWM_C1_PIN);
    DL_GPIO_initPeripheralOutputFunction(GPIO_MOTOR_PWM_C2_IOMUX,GPIO_MOTOR_PWM_C2_IOMUX_FUNC);
    DL_GPIO_enableOutput(GPIO_MOTOR_PWM_C2_PORT, GPIO_MOTOR_PWM_C2_PIN);
    DL_GPIO_initPeripheralOutputFunction(GPIO_MOTOR_PWM_C3_IOMUX,GPIO_MOTOR_PWM_C3_IOMUX_FUNC);
    DL_GPIO_enableOutput(GPIO_MOTOR_PWM_C3_PORT, GPIO_MOTOR_PWM_C3_PIN);
    DL_GPIO_initPeripheralOutputFunction(GPIO_PTZ1_PWM_C0_IOMUX,GPIO_PTZ1_PWM_C0_IOMUX_FUNC);
    DL_GPIO_enableOutput(GPIO_PTZ1_PWM_C0_PORT, GPIO_PTZ1_PWM_C0_PIN);
    DL_GPIO_initPeripheralOutputFunction(GPIO_PTZ2_PWM_C1_IOMUX,GPIO_PTZ2_PWM_C1_IOMUX_FUNC);
    DL_GPIO_enableOutput(GPIO_PTZ2_PWM_C1_PORT, GPIO_PTZ2_PWM_C1_PIN);

    DL_GPIO_initPeripheralInputFunction(GPIO_ULTRASONIC_TIMER_C0_IOMUX,GPIO_ULTRASONIC_TIMER_C0_IOMUX_FUNC);

    DL_GPIO_initPeripheralInputFunctionFeatures(GPIO_I2C_0_IOMUX_SDA,
        GPIO_I2C_0_IOMUX_SDA_FUNC, DL_GPIO_INVERSION_DISABLE,
        DL_GPIO_RESISTOR_NONE, DL_GPIO_HYSTERESIS_DISABLE,
        DL_GPIO_WAKEUP_DISABLE);
    DL_GPIO_initPeripheralInputFunctionFeatures(GPIO_I2C_0_IOMUX_SCL,
        GPIO_I2C_0_IOMUX_SCL_FUNC, DL_GPIO_INVERSION_DISABLE,
        DL_GPIO_RESISTOR_NONE, DL_GPIO_HYSTERESIS_DISABLE,
        DL_GPIO_WAKEUP_DISABLE);
    DL_GPIO_enableHiZ(GPIO_I2C_0_IOMUX_SDA);
    DL_GPIO_enableHiZ(GPIO_I2C_0_IOMUX_SCL);

    DL_GPIO_initPeripheralOutputFunction(
        GPIO_UART0_IOMUX_TX, GPIO_UART0_IOMUX_TX_FUNC);
    DL_GPIO_initPeripheralInputFunction(
        GPIO_UART0_IOMUX_RX, GPIO_UART0_IOMUX_RX_FUNC);

    DL_GPIO_initDigitalOutput(ULTRASONIC_TRIG_IOMUX);

    DL_GPIO_initDigitalOutput(BEEP_GPIO_BEEP_PIN_IOMUX);

    DL_GPIO_initDigitalOutput(MOTORA_DIR_MOTA_DIR1_IOMUX);

    DL_GPIO_initDigitalOutput(MOTORA_DIR_MOTA_DIR2_IOMUX);

    DL_GPIO_initDigitalOutput(MOTORB_DIR_MOTB_DIR1_IOMUX);

    DL_GPIO_initDigitalOutput(MOTORB_DIR_MOTB_DIR2_IOMUX);

    DL_GPIO_initDigitalOutput(MOTORC_DIR_MOTC_DIR1_IOMUX);

    DL_GPIO_initDigitalOutput(MOTORC_DIR_MOTC_DIR2_IOMUX);

    DL_GPIO_initDigitalOutput(MOTORD_DIR_MOTD_DIR1_IOMUX);

    DL_GPIO_initDigitalOutput(MOTORD_DIR_MOTD_DIR2_IOMUX);

    DL_GPIO_initDigitalInputFeatures(ENCA_ENCA_A_IOMUX,
		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_NONE,
		 DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_initDigitalInputFeatures(ENCA_ENCA_B_IOMUX,
		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_NONE,
		 DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_initDigitalInputFeatures(ENCB_ENCB_A_IOMUX,
		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_NONE,
		 DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_initDigitalInputFeatures(ENCB_ENCB_b_IOMUX,
		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_NONE,
		 DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_initDigitalInputFeatures(ENCC_ENCC_A_IOMUX,
		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_NONE,
		 DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_initDigitalInputFeatures(ENCC_ENCC_B_IOMUX,
		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_NONE,
		 DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_initDigitalInputFeatures(ENCD_ENCD_A_IOMUX,
		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_NONE,
		 DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_initDigitalInputFeatures(ENCD_ENCD_B_IOMUX,
		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_NONE,
		 DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_initDigitalOutput(PTZ_DIR_PTZ1_DIR_IOMUX);

    DL_GPIO_initDigitalOutput(PTZ_DIR_PTZ2_DIR_IOMUX);

    DL_GPIO_initDigitalInputFeatures(GRAY_OUT1_IOMUX,
		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_NONE,
		 DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_initDigitalInputFeatures(GRAY_OUT2_IOMUX,
		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_NONE,
		 DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_initDigitalInputFeatures(GRAY_OUT3_IOMUX,
		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_NONE,
		 DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_initDigitalInputFeatures(GRAY_OUT4_IOMUX,
		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_NONE,
		 DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_initDigitalInputFeatures(GRAY_OUT5_IOMUX,
		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_NONE,
		 DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_initDigitalInputFeatures(GRAY_OUT6_IOMUX,
		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_NONE,
		 DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_initDigitalInputFeatures(GRAY_OUT7_IOMUX,
		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_NONE,
		 DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_initDigitalInputFeatures(GRAY_OUT8_IOMUX,
		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_NONE,
		 DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_initDigitalInput(KEY_KEY1_IOMUX);

    DL_GPIO_initDigitalInput(KEY_KEY2_IOMUX);

    DL_GPIO_initDigitalOutput(LED12_LED1_PIN_IOMUX);

    DL_GPIO_initDigitalOutput(LED12_LED2_PIN_IOMUX);

    DL_GPIO_initDigitalOutput(LED34_LED3_PIN_IOMUX);

    DL_GPIO_initDigitalOutput(LED34_LED4_PIN_IOMUX);

    DL_GPIO_initDigitalOutput(OLED_OLED_SDA_IOMUX);

    DL_GPIO_initDigitalOutput(OLED_OLED_SCL_IOMUX);

    DL_GPIO_clearPins(GPIOA, BEEP_GPIO_BEEP_PIN_PIN |
		MOTORD_DIR_MOTD_DIR2_PIN |
		PTZ_DIR_PTZ1_DIR_PIN |
		OLED_OLED_SDA_PIN |
		OLED_OLED_SCL_PIN);
    DL_GPIO_setPins(GPIOA, LED12_LED2_PIN_PIN);
    DL_GPIO_enableOutput(GPIOA, BEEP_GPIO_BEEP_PIN_PIN |
		MOTORD_DIR_MOTD_DIR2_PIN |
		PTZ_DIR_PTZ1_DIR_PIN |
		LED12_LED2_PIN_PIN |
		OLED_OLED_SDA_PIN |
		OLED_OLED_SCL_PIN);
    DL_GPIO_setLowerPinsPolarity(GPIOA, DL_GPIO_PIN_0_EDGE_FALL |
		DL_GPIO_PIN_1_EDGE_FALL);
    DL_GPIO_setUpperPinsPolarity(GPIOA, DL_GPIO_PIN_29_EDGE_FALL |
		DL_GPIO_PIN_26_EDGE_FALL |
		DL_GPIO_PIN_27_EDGE_FALL);
    DL_GPIO_clearInterruptStatus(GPIOA, ENCB_ENCB_A_PIN |
		ENCC_ENCC_A_PIN |
		ENCC_ENCC_B_PIN |
		KEY_KEY1_PIN |
		KEY_KEY2_PIN);
    DL_GPIO_enableInterrupt(GPIOA, ENCB_ENCB_A_PIN |
		ENCC_ENCC_A_PIN |
		ENCC_ENCC_B_PIN |
		KEY_KEY1_PIN |
		KEY_KEY2_PIN);
    DL_GPIO_clearPins(GPIOB, ULTRASONIC_TRIG_PIN |
		MOTORA_DIR_MOTA_DIR1_PIN |
		MOTORA_DIR_MOTA_DIR2_PIN |
		MOTORB_DIR_MOTB_DIR1_PIN |
		MOTORB_DIR_MOTB_DIR2_PIN |
		MOTORC_DIR_MOTC_DIR1_PIN |
		MOTORC_DIR_MOTC_DIR2_PIN |
		MOTORD_DIR_MOTD_DIR1_PIN |
		PTZ_DIR_PTZ2_DIR_PIN);
    DL_GPIO_setPins(GPIOB, LED12_LED1_PIN_PIN |
		LED34_LED3_PIN_PIN |
		LED34_LED4_PIN_PIN);
    DL_GPIO_enableOutput(GPIOB, ULTRASONIC_TRIG_PIN |
		MOTORA_DIR_MOTA_DIR1_PIN |
		MOTORA_DIR_MOTA_DIR2_PIN |
		MOTORB_DIR_MOTB_DIR1_PIN |
		MOTORB_DIR_MOTB_DIR2_PIN |
		MOTORC_DIR_MOTC_DIR1_PIN |
		MOTORC_DIR_MOTC_DIR2_PIN |
		MOTORD_DIR_MOTD_DIR1_PIN |
		PTZ_DIR_PTZ2_DIR_PIN |
		LED12_LED1_PIN_PIN |
		LED34_LED3_PIN_PIN |
		LED34_LED4_PIN_PIN);
    DL_GPIO_setLowerPinsPolarity(GPIOB, DL_GPIO_PIN_10_EDGE_FALL |
		DL_GPIO_PIN_11_EDGE_FALL);
    DL_GPIO_setUpperPinsPolarity(GPIOB, DL_GPIO_PIN_27_EDGE_FALL |
		DL_GPIO_PIN_21_EDGE_FALL |
		DL_GPIO_PIN_19_EDGE_FALL);
    DL_GPIO_clearInterruptStatus(GPIOB, ENCA_ENCA_A_PIN |
		ENCA_ENCA_B_PIN |
		ENCB_ENCB_b_PIN |
		ENCD_ENCD_A_PIN |
		ENCD_ENCD_B_PIN);
    DL_GPIO_enableInterrupt(GPIOB, ENCA_ENCA_A_PIN |
		ENCA_ENCA_B_PIN |
		ENCB_ENCB_b_PIN |
		ENCD_ENCD_A_PIN |
		ENCD_ENCD_B_PIN);

}


SYSCONFIG_WEAK void SYSCFG_DL_SYSCTL_init(void)
{

	//Low Power Mode is configured to be SLEEP0
    DL_SYSCTL_setBORThreshold(DL_SYSCTL_BOR_THRESHOLD_LEVEL_0);

    DL_SYSCTL_setSYSOSCFreq(DL_SYSCTL_SYSOSC_FREQ_BASE);
    /* Set default configuration */
    DL_SYSCTL_disableHFXT();
    DL_SYSCTL_disableSYSPLL();
    DL_SYSCTL_setULPCLKDivider(DL_SYSCTL_ULPCLK_DIV_1);
    DL_SYSCTL_setMCLKDivider(DL_SYSCTL_MCLK_DIVIDER_DISABLE);

}


/*
 * Timer clock configuration to be sourced by  / 8 (4000000 Hz)
 * timerClkFreq = (timerClkSrc / (timerClkDivRatio * (timerClkPrescale + 1)))
 *   1000000 Hz = 4000000 Hz / (8 * (3 + 1))
 */
static const DL_TimerA_ClockConfig gMOTOR_PWMClockConfig = {
    .clockSel = DL_TIMER_CLOCK_BUSCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_8,
    .prescale = 3U
};

static const DL_TimerA_PWMConfig gMOTOR_PWMConfig = {
    .pwmMode = DL_TIMER_PWM_MODE_EDGE_ALIGN,
    .period = 1000,
    .isTimerWithFourCC = true,
    .startTimer = DL_TIMER_START,
};

SYSCONFIG_WEAK void SYSCFG_DL_MOTOR_PWM_init(void) {

    DL_TimerA_setClockConfig(
        MOTOR_PWM_INST, (DL_TimerA_ClockConfig *) &gMOTOR_PWMClockConfig);

    DL_TimerA_initPWMMode(
        MOTOR_PWM_INST, (DL_TimerA_PWMConfig *) &gMOTOR_PWMConfig);

    DL_TimerA_setCaptureCompareOutCtl(MOTOR_PWM_INST, DL_TIMER_CC_OCTL_INIT_VAL_LOW,
		DL_TIMER_CC_OCTL_INV_OUT_DISABLED, DL_TIMER_CC_OCTL_SRC_FUNCVAL,
		DL_TIMERA_CAPTURE_COMPARE_0_INDEX);

    DL_TimerA_setCaptCompUpdateMethod(MOTOR_PWM_INST, DL_TIMER_CC_UPDATE_METHOD_IMMEDIATE, DL_TIMERA_CAPTURE_COMPARE_0_INDEX);
    DL_TimerA_setCaptureCompareValue(MOTOR_PWM_INST, 1000, DL_TIMER_CC_0_INDEX);

    DL_TimerA_setCaptureCompareOutCtl(MOTOR_PWM_INST, DL_TIMER_CC_OCTL_INIT_VAL_LOW,
		DL_TIMER_CC_OCTL_INV_OUT_DISABLED, DL_TIMER_CC_OCTL_SRC_FUNCVAL,
		DL_TIMERA_CAPTURE_COMPARE_1_INDEX);

    DL_TimerA_setCaptCompUpdateMethod(MOTOR_PWM_INST, DL_TIMER_CC_UPDATE_METHOD_IMMEDIATE, DL_TIMERA_CAPTURE_COMPARE_1_INDEX);
    DL_TimerA_setCaptureCompareValue(MOTOR_PWM_INST, 1000, DL_TIMER_CC_1_INDEX);

    DL_TimerA_setCaptureCompareOutCtl(MOTOR_PWM_INST, DL_TIMER_CC_OCTL_INIT_VAL_LOW,
		DL_TIMER_CC_OCTL_INV_OUT_DISABLED, DL_TIMER_CC_OCTL_SRC_FUNCVAL,
		DL_TIMERA_CAPTURE_COMPARE_2_INDEX);

    DL_TimerA_setCaptCompUpdateMethod(MOTOR_PWM_INST, DL_TIMER_CC_UPDATE_METHOD_IMMEDIATE, DL_TIMERA_CAPTURE_COMPARE_2_INDEX);
    DL_TimerA_setCaptureCompareValue(MOTOR_PWM_INST, 1000, DL_TIMER_CC_2_INDEX);

    DL_TimerA_setCaptureCompareOutCtl(MOTOR_PWM_INST, DL_TIMER_CC_OCTL_INIT_VAL_LOW,
		DL_TIMER_CC_OCTL_INV_OUT_DISABLED, DL_TIMER_CC_OCTL_SRC_FUNCVAL,
		DL_TIMERA_CAPTURE_COMPARE_3_INDEX);

    DL_TimerA_setCaptCompUpdateMethod(MOTOR_PWM_INST, DL_TIMER_CC_UPDATE_METHOD_IMMEDIATE, DL_TIMERA_CAPTURE_COMPARE_3_INDEX);
    DL_TimerA_setCaptureCompareValue(MOTOR_PWM_INST, 1000, DL_TIMER_CC_3_INDEX);

    DL_TimerA_enableClock(MOTOR_PWM_INST);


    
    DL_TimerA_setCCPDirection(MOTOR_PWM_INST , DL_TIMER_CC0_OUTPUT | DL_TIMER_CC1_OUTPUT | DL_TIMER_CC2_OUTPUT | DL_TIMER_CC3_OUTPUT );


}
/*
 * Timer clock configuration to be sourced by  / 8 (4000000 Hz)
 * timerClkFreq = (timerClkSrc / (timerClkDivRatio * (timerClkPrescale + 1)))
 *   40000 Hz = 4000000 Hz / (8 * (99 + 1))
 */
static const DL_TimerA_ClockConfig gPTZ1_PWMClockConfig = {
    .clockSel = DL_TIMER_CLOCK_BUSCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_8,
    .prescale = 99U
};

static const DL_TimerA_PWMConfig gPTZ1_PWMConfig = {
    .pwmMode = DL_TIMER_PWM_MODE_EDGE_ALIGN,
    .period = 1000,
    .isTimerWithFourCC = true,
    .startTimer = DL_TIMER_STOP,
};

SYSCONFIG_WEAK void SYSCFG_DL_PTZ1_PWM_init(void) {

    DL_TimerA_setClockConfig(
        PTZ1_PWM_INST, (DL_TimerA_ClockConfig *) &gPTZ1_PWMClockConfig);

    DL_TimerA_initPWMMode(
        PTZ1_PWM_INST, (DL_TimerA_PWMConfig *) &gPTZ1_PWMConfig);

    DL_TimerA_setCaptureCompareOutCtl(PTZ1_PWM_INST, DL_TIMER_CC_OCTL_INIT_VAL_LOW,
		DL_TIMER_CC_OCTL_INV_OUT_DISABLED, DL_TIMER_CC_OCTL_SRC_FUNCVAL,
		DL_TIMERA_CAPTURE_COMPARE_0_INDEX);

    DL_TimerA_setCaptCompUpdateMethod(PTZ1_PWM_INST, DL_TIMER_CC_UPDATE_METHOD_IMMEDIATE, DL_TIMERA_CAPTURE_COMPARE_0_INDEX);
    DL_TimerA_setCaptureCompareValue(PTZ1_PWM_INST, 500, DL_TIMER_CC_0_INDEX);

    DL_TimerA_enableClock(PTZ1_PWM_INST);


    
    DL_TimerA_setCCPDirection(PTZ1_PWM_INST , DL_TIMER_CC0_OUTPUT );


}
/*
 * Timer clock configuration to be sourced by  / 8 (4000000 Hz)
 * timerClkFreq = (timerClkSrc / (timerClkDivRatio * (timerClkPrescale + 1)))
 *   40000 Hz = 4000000 Hz / (8 * (99 + 1))
 */
static const DL_TimerG_ClockConfig gPTZ2_PWMClockConfig = {
    .clockSel = DL_TIMER_CLOCK_BUSCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_8,
    .prescale = 99U
};

static const DL_TimerG_PWMConfig gPTZ2_PWMConfig = {
    .pwmMode = DL_TIMER_PWM_MODE_EDGE_ALIGN,
    .period = 1000,
    .isTimerWithFourCC = false,
    .startTimer = DL_TIMER_STOP,
};

SYSCONFIG_WEAK void SYSCFG_DL_PTZ2_PWM_init(void) {

    DL_TimerG_setClockConfig(
        PTZ2_PWM_INST, (DL_TimerG_ClockConfig *) &gPTZ2_PWMClockConfig);

    DL_TimerG_initPWMMode(
        PTZ2_PWM_INST, (DL_TimerG_PWMConfig *) &gPTZ2_PWMConfig);

    DL_TimerG_setCaptureCompareOutCtl(PTZ2_PWM_INST, DL_TIMER_CC_OCTL_INIT_VAL_LOW,
		DL_TIMER_CC_OCTL_INV_OUT_DISABLED, DL_TIMER_CC_OCTL_SRC_FUNCVAL,
		DL_TIMERG_CAPTURE_COMPARE_1_INDEX);

    DL_TimerG_setCaptCompUpdateMethod(PTZ2_PWM_INST, DL_TIMER_CC_UPDATE_METHOD_IMMEDIATE, DL_TIMERG_CAPTURE_COMPARE_1_INDEX);
    DL_TimerG_setCaptureCompareValue(PTZ2_PWM_INST, 500, DL_TIMER_CC_1_INDEX);

    DL_TimerG_enableClock(PTZ2_PWM_INST);


    
    DL_TimerG_setCCPDirection(PTZ2_PWM_INST , DL_TIMER_CC1_OUTPUT );


}



/*
 * Timer clock configuration to be sourced by BUSCLK /  (4000000 Hz)
 * timerClkFreq = (timerClkSrc / (timerClkDivRatio * (timerClkPrescale + 1)))
 *   4000000 Hz = 4000000 Hz / (8 * (0 + 1))
 */
static const DL_TimerG_ClockConfig gULTRASONIC_TIMERClockConfig = {
    .clockSel    = DL_TIMER_CLOCK_BUSCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_8,
    .prescale = 0U
};

/*
 * Timer load value (where the counter starts from) is calculated as (timerPeriod * timerClockFreq) - 1
 * ULTRASONIC_TIMER_INST_LOAD_VALUE = (40ms * 4000000 Hz) - 1
 */
static const DL_TimerG_CaptureConfig gULTRASONIC_TIMERCaptureConfig = {
    .captureMode    = DL_TIMER_CAPTURE_MODE_EDGE_TIME,
    .period         = ULTRASONIC_TIMER_INST_LOAD_VALUE,
    .startTimer     = DL_TIMER_STOP,
    .edgeCaptMode   = DL_TIMER_CAPTURE_EDGE_DETECTION_MODE_RISING,
    .inputChan      = DL_TIMER_INPUT_CHAN_0,
    .inputInvMode   = DL_TIMER_CC_INPUT_INV_NOINVERT,
};

SYSCONFIG_WEAK void SYSCFG_DL_ULTRASONIC_TIMER_init(void) {

    DL_TimerG_setClockConfig(ULTRASONIC_TIMER_INST,
        (DL_TimerG_ClockConfig *) &gULTRASONIC_TIMERClockConfig);

    DL_TimerG_initCaptureMode(ULTRASONIC_TIMER_INST,
        (DL_TimerG_CaptureConfig *) &gULTRASONIC_TIMERCaptureConfig);
    DL_TimerG_enableClock(ULTRASONIC_TIMER_INST);

}


/*
 * Timer clock configuration to be sourced by BUSCLK /  (4000000 Hz)
 * timerClkFreq = (timerClkSrc / (timerClkDivRatio * (timerClkPrescale + 1)))
 *   1000000 Hz = 4000000 Hz / (8 * (3 + 1))
 */
static const DL_TimerG_ClockConfig gPID_TIMERClockConfig = {
    .clockSel    = DL_TIMER_CLOCK_BUSCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_8,
    .prescale    = 3U,
};

/*
 * Timer load value (where the counter starts from) is calculated as (timerPeriod * timerClockFreq) - 1
 * PID_TIMER_INST_LOAD_VALUE = (10ms * 1000000 Hz) - 1
 */
static const DL_TimerG_TimerConfig gPID_TIMERTimerConfig = {
    .period     = PID_TIMER_INST_LOAD_VALUE,
    .timerMode  = DL_TIMER_TIMER_MODE_ONE_SHOT,
    .startTimer = DL_TIMER_STOP,
};

SYSCONFIG_WEAK void SYSCFG_DL_PID_TIMER_init(void) {

    DL_TimerG_setClockConfig(PID_TIMER_INST,
        (DL_TimerG_ClockConfig *) &gPID_TIMERClockConfig);

    DL_TimerG_initTimerMode(PID_TIMER_INST,
        (DL_TimerG_TimerConfig *) &gPID_TIMERTimerConfig);
    DL_TimerG_enableInterrupt(PID_TIMER_INST , DL_TIMERG_INTERRUPT_ZERO_EVENT);
    DL_TimerG_enableClock(PID_TIMER_INST);





}


static const DL_I2C_ClockConfig gI2C_0ClockConfig = {
    .clockSel = DL_I2C_CLOCK_BUSCLK,
    .divideRatio = DL_I2C_CLOCK_DIVIDE_1,
};

SYSCONFIG_WEAK void SYSCFG_DL_I2C_0_init(void) {

    DL_I2C_setClockConfig(I2C_0_INST,
        (DL_I2C_ClockConfig *) &gI2C_0ClockConfig);
    DL_I2C_setAnalogGlitchFilterPulseWidth(I2C_0_INST,
        DL_I2C_ANALOG_GLITCH_FILTER_WIDTH_50NS);
    DL_I2C_enableAnalogGlitchFilter(I2C_0_INST);




}


static const DL_UART_Main_ClockConfig gUART0ClockConfig = {
    .clockSel    = DL_UART_MAIN_CLOCK_BUSCLK,
    .divideRatio = DL_UART_MAIN_CLOCK_DIVIDE_RATIO_1
};

static const DL_UART_Main_Config gUART0Config = {
    .mode        = DL_UART_MAIN_MODE_NORMAL,
    .direction   = DL_UART_MAIN_DIRECTION_TX_RX,
    .flowControl = DL_UART_MAIN_FLOW_CONTROL_NONE,
    .parity      = DL_UART_MAIN_PARITY_NONE,
    .wordLength  = DL_UART_MAIN_WORD_LENGTH_8_BITS,
    .stopBits    = DL_UART_MAIN_STOP_BITS_ONE
};

SYSCONFIG_WEAK void SYSCFG_DL_UART0_init(void)
{
    DL_UART_Main_setClockConfig(UART0_INST, (DL_UART_Main_ClockConfig *) &gUART0ClockConfig);

    DL_UART_Main_init(UART0_INST, (DL_UART_Main_Config *) &gUART0Config);
    /*
     * Configure baud rate by setting oversampling and baud rate divisors.
     *  Target baud rate: 9600
     *  Actual baud rate: 9600.24
     */
    DL_UART_Main_setOversampling(UART0_INST, DL_UART_OVERSAMPLING_RATE_16X);
    DL_UART_Main_setBaudRateDivisor(UART0_INST, UART0_IBRD_32_MHZ_9600_BAUD, UART0_FBRD_32_MHZ_9600_BAUD);


    /* Configure Interrupts */
    DL_UART_Main_enableInterrupt(UART0_INST,
                                 DL_UART_MAIN_INTERRUPT_DMA_DONE_RX |
                                 DL_UART_MAIN_INTERRUPT_DMA_DONE_TX |
                                 DL_UART_MAIN_INTERRUPT_RX_TIMEOUT_ERROR);

    /* Configure DMA Receive Event */
    DL_UART_Main_enableDMAReceiveEvent(UART0_INST, DL_UART_DMA_INTERRUPT_RX);
    /* Configure DMA Transmit Event */
    DL_UART_Main_enableDMATransmitEvent(UART0_INST);

    DL_UART_Main_enable(UART0_INST);
}

static const DL_DMA_Config gDMA_CH0Config = {
    .transferMode   = DL_DMA_SINGLE_TRANSFER_MODE,
    .extendedMode   = DL_DMA_NORMAL_MODE,
    .destIncrement  = DL_DMA_ADDR_UNCHANGED,
    .srcIncrement   = DL_DMA_ADDR_UNCHANGED,
    .destWidth      = DL_DMA_WIDTH_WORD,
    .srcWidth       = DL_DMA_WIDTH_WORD,
    .trigger        = UART0_INST_DMA_TRIGGER_0,
    .triggerType    = DL_DMA_TRIGGER_TYPE_EXTERNAL,
};

SYSCONFIG_WEAK void SYSCFG_DL_DMA_CH0_init(void)
{
    DL_DMA_initChannel(DMA, DMA_CH0_CHAN_ID , (DL_DMA_Config *) &gDMA_CH0Config);
}
static const DL_DMA_Config gDMA_CH1Config = {
    .transferMode   = DL_DMA_SINGLE_TRANSFER_MODE,
    .extendedMode   = DL_DMA_NORMAL_MODE,
    .destIncrement  = DL_DMA_ADDR_UNCHANGED,
    .srcIncrement   = DL_DMA_ADDR_UNCHANGED,
    .destWidth      = DL_DMA_WIDTH_WORD,
    .srcWidth       = DL_DMA_WIDTH_WORD,
    .trigger        = UART0_INST_DMA_TRIGGER_1,
    .triggerType    = DL_DMA_TRIGGER_TYPE_EXTERNAL,
};

SYSCONFIG_WEAK void SYSCFG_DL_DMA_CH1_init(void)
{
    DL_DMA_initChannel(DMA, DMA_CH1_CHAN_ID , (DL_DMA_Config *) &gDMA_CH1Config);
}
SYSCONFIG_WEAK void SYSCFG_DL_DMA_init(void){
    SYSCFG_DL_DMA_CH0_init();
    SYSCFG_DL_DMA_CH1_init();
}


