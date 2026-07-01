/*
 * bsp_uart.c - 串口驱动实现 (MSPM0G3507, 不定长接收, DMA)
 *
 * 引脚: PA10=TX, PA11=RX
 * SysConfig 已配: UART0, 9600/8N1, DMA_CH0(RX), DMA_CH1(TX)
 * 驱动职责: 配置 DMA RX 缓冲区; 处理 RX_TIMEOUT/DMA_DONE 中断
 */

#include "bsp_uart.h"

/* ========== DMA RX 缓冲区 ========== */
/* DMA 自动将收到的字节写入此数组, 主循环从 buf_start 读 */
static uint8_t  dma_buf[UART_RX_BUF_SIZE];
static uint16_t dma_rd_idx = 0;   /* 主循环读位置 */

/* ========== 帧状态 ========== */
static volatile uint8_t  frame_ready = 0;
static volatile uint16_t frame_len   = 0;
static volatile uint16_t frame_start = 0;  /* 帧在 dma_buf 中的起始位置 */

/* ========== DMA RX 内部状态 ========== */
static uint16_t dma_wr_pos = 0;    /* DMA 下次写入的 buf 位置(追踪) */

static void _dma_rx_restart(void)
{
    /*
     * 停止并重新配置 DMA RX 通道, 使之从 buf[dma_wr_pos] 继续填充.
     *
     * DMA_CH0: channel 1, 触发源 = UART0 RX, 单次传输模式
     *   transferSize = 剩余空间, 填满后触发 DMA_DONE_RX 中断.
     */
    uint16_t remaining = UART_RX_BUF_SIZE;

    DL_DMA_disableChannel(DMA, DMA_CH0_CHAN_ID);

    DL_DMA_setSrcAddr(DMA, DMA_CH0_CHAN_ID,
        (uint32_t)&UART0_INST->RXDATA);
    DL_DMA_setDestAddr(DMA, DMA_CH0_CHAN_ID,
        (uint32_t)(dma_buf + dma_wr_pos));
    DL_DMA_setTransferSize(DMA, DMA_CH0_CHAN_ID, remaining);

    DL_DMA_enableChannel(DMA, DMA_CH0_CHAN_ID);
}

/* ========== 初始化 ========== */
void UART_Init(uint32_t baudrate)
{
    (void)baudrate;  /* SysConfig 已设 9600 */

    /* ---- 清状态 ---- */
    dma_rd_idx   = 0;
    dma_wr_pos   = 0;
    frame_ready  = 0;
    frame_len    = 0;
    frame_start  = 0;

    /* ---- 配置 DMA RX (CH0, channel 1) ---- */
    /*
     * SysConfig 已调 DL_DMA_initChannel 设置触发和模式,
     * 此处补设源/目标地址、传输大小.
     */
    DL_DMA_setSrcAddr(DMA, DMA_CH0_CHAN_ID,
        (uint32_t)&UART0_INST->RXDATA);
    DL_DMA_setDestAddr(DMA, DMA_CH0_CHAN_ID,
        (uint32_t)dma_buf);
    DL_DMA_setTransferSize(DMA, DMA_CH0_CHAN_ID, UART_RX_BUF_SIZE);

    DL_DMA_enableChannel(DMA, DMA_CH0_CHAN_ID);

    /* ---- DMA TX (CH1, channel 0) 仅用于大块发送, 暂不启用 ---- */
}

/* ========== 发送 ========== */

void UART_SendByte(uint8_t data)
{
    while (DL_UART_isTXFIFOFull(UART0_INST));
    DL_UART_transmitData(UART0_INST, data);
}

void UART_SendBytes(const uint8_t *data, uint16_t len)
{
    while (len--) {
        UART_SendByte(*data++);
    }
}

void UART_SendString(const char *str)
{
    while (*str) {
        UART_SendByte((uint8_t)*str++);
    }
}

/* ========== 逐字节接收 ========== */

uint16_t UART_Available(void)
{
    /* 计算 DMA 已写入但尚未读取的字节数 */
    uint16_t wr = dma_wr_pos;
    uint16_t rd = dma_rd_idx;

    if (wr >= rd) {
        return wr - rd;
    } else {
        return UART_RX_BUF_SIZE - rd + wr;
    }
}

uint8_t UART_ReadByte(void)
{
    if (UART_Available() == 0) return 0;

    uint8_t data = dma_buf[dma_rd_idx];
    dma_rd_idx = (dma_rd_idx + 1) % UART_RX_BUF_SIZE;
    return data;
}

uint16_t UART_ReadBytes(uint8_t *buf, uint16_t maxLen)
{
    uint16_t count = 0;
    while (count < maxLen && UART_Available() > 0) {
        buf[count++] = UART_ReadByte();
    }
    return count;
}

/* ========== 帧接收 ========== */

uint8_t UART_IsFrameReady(void)
{
    return frame_ready;
}

uint16_t UART_GetFrameLen(void)
{
    return frame_len;
}

uint16_t UART_ReadFrame(uint8_t *buf, uint16_t maxLen)
{
    uint16_t len;
    uint16_t i;

    if (!frame_ready) return 0;

    len = frame_len;
    if (len > maxLen) len = maxLen;

    for (i = 0; i < len; i++) {
        buf[i] = dma_buf[(frame_start + i) % UART_RX_BUF_SIZE];
    }

    frame_ready = 0;
    frame_len   = 0;

    return len;
}

void UART_Flush(void)
{
    dma_rd_idx  = dma_wr_pos;
    frame_ready = 0;
    frame_len   = 0;
}

/* ========== 中断处理 ========== */

void UART_IRQHandler(void)
{
    /*
     * SysConfig 使能的中断:
     *   1. DL_UART_MAIN_INTERRUPT_DMA_DONE_RX  — DMA 搬完所有字节
     *   2. DL_UART_MAIN_INTERRUPT_DMA_DONE_TX  — (暂留)
     *   3. DL_UART_MAIN_INTERRUPT_RX_TIMEOUT_ERROR — RX 空闲超时
     */
    uint32_t mis = UART0_INST->CPU_INT.MIS;  /* Masked Interrupt Status */

    /* ---- RX 超时: 帧结束 ---- */
    if (mis & DL_UART_MAIN_INTERRUPT_RX_TIMEOUT_ERROR) {
        uint32_t remaining;
        uint16_t transferred;

        /* 读 DMA 剩余传输次数, 计算已传输字节数 */
        remaining   = DL_DMA_getTransferSize(DMA, DMA_CH0_CHAN_ID);
        transferred = UART_RX_BUF_SIZE - (uint16_t)remaining;

        if (transferred > 0) {
            /* 帧范围: dma_wr_pos ~ dma_wr_pos+transferred */
            frame_start = dma_wr_pos;
            frame_len   = transferred;
            frame_ready = 1;

            /* 更新 DMA 写位置 */
            dma_wr_pos = (dma_wr_pos + transferred) % UART_RX_BUF_SIZE;
        }

        /*
         * 清除超时中断标志.
         * 注意: 读取 RXDATA 也可清除 RTIM 标志,
         *       但这里显式清除更安全.
         */
        DL_UART_Main_clearInterruptStatus(UART0_INST,
            DL_UART_MAIN_INTERRUPT_RX_TIMEOUT_ERROR);

        /* 重启 DMA RX (从新位置继续填充) */
        _dma_rx_restart();
    }

    /* ---- DMA_RX_DONE: 缓冲区满 (256字节连收) ---- */
    if (mis & DL_UART_MAIN_INTERRUPT_DMA_DONE_RX) {
        uint16_t transferred = UART_RX_BUF_SIZE;

        frame_start = dma_wr_pos;
        frame_len   = transferred;
        frame_ready = 1;

        dma_wr_pos = (dma_wr_pos + transferred) % UART_RX_BUF_SIZE;

        /* 清除 DMA_DONE 中断 */
        DL_UART_Main_clearInterruptStatus(UART0_INST,
            DL_UART_MAIN_INTERRUPT_DMA_DONE_RX);

        /* 重启 DMA RX */
        _dma_rx_restart();
    }
}
