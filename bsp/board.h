#ifndef __BOARD_H__
#define __BOARD_H__

#include "ti_msp_dl_config.h"

#ifndef u8
#define u8 uint8_t
#endif

#ifndef u16
#define u16 uint16_t
#endif

#ifndef u32
#define u32 uint32_t
#endif

#ifndef u64
#define u64 uint64_t
#endif

/* ========== OLED 软件I2C引脚映射 (I2C1: PA15=SCL, PA16=SDA) ========== */
/* OLED_PORT 已由 ti_msp_dl_config.h 定义为 GPIOA */
/* 将 soft_oled 使用的宏映射到 SysConfig 生成的名称 */
#ifndef OLED_SCL_PIN
#define OLED_SCL_PIN   OLED_OLED_SCL_PIN
#endif
#ifndef OLED_SDA_PIN
#define OLED_SDA_PIN   OLED_OLED_SDA_PIN
#endif

/* 延时函数 */
void delay_us(int __us);
void delay_ms(int __ms);

void delay_1us(int __us);
void delay_1ms(int __ms);

#endif
