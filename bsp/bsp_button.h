#ifndef __BSP_BUTTON_H__
#define __BSP_BUTTON_H__

#include "board.h"

void    Button_Init(void);
uint8_t Button_Read(uint8_t ch);
uint8_t Button_GetPress(uint8_t ch);
uint8_t Button_GetLongPress(uint8_t ch, uint16_t ms);
void    Button_Task(void);

#endif /* __BSP_BUTTON_H__ */
