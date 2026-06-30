#ifndef __BSP_GRAYSCALE_H__
#define __BSP_GRAYSCALE_H__

#include "board.h"

void    Grayscale_Init(void);
uint8_t Grayscale_Read(uint8_t ch);
uint8_t Grayscale_ReadAll(void);

#endif /* __BSP_GRAYSCALE_H__ */
