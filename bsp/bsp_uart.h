/*
 * bsp_uart.h - 串口驱动 (MSPM0G3507, 不定长接收, DMA)
 *
 * 引脚: PA10=TX, PA11=RX
 * SysConfig: UART0, 9600/8N1, DMA_CH0(RX)/DMA_CH1(TX), RX_TIMEOUT中断
 *
 * 不定长接收原理:
 *   - DMA 自动将 UART RX 数据搬运到环形缓冲区
 *   - RX 超时 (RTIM) 检测帧结束: 空闲超过 RTIM 个位周期触发中断
 *   - 中断中读取 DMA 剩余计数, 计算帧长度
 */

#ifndef __BSP_UART_H
#define __BSP_UART_H

#include "board.h"

/* ========== 环形缓冲区大小 ========== */
#define UART_RX_BUF_SIZE    256

/* ========== API ========== */

void UART_Init(uint32_t baudrate);

/* ---- 发送 (阻塞, 不使用DMA) ---- */
void UART_SendByte(uint8_t data);
void UART_SendBytes(const uint8_t *data, uint16_t len);
void UART_SendString(const char *str);

/* ---- 逐字节接收 ---- */
uint16_t UART_Available(void);
uint8_t  UART_ReadByte(void);
uint16_t UART_ReadBytes(uint8_t *buf, uint16_t maxLen);

/* ---- 帧接收 (RX超时自动分包) ---- */
uint8_t  UART_IsFrameReady(void);
uint16_t UART_GetFrameLen(void);
uint16_t UART_ReadFrame(uint8_t *buf, uint16_t maxLen);

void UART_Flush(void);

/* ---- 中断处理 (在 UART0_IRQHandler 中调用) ---- */
void UART_IRQHandler(void);

#endif /* __BSP_UART_H */
