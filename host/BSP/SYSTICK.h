#ifndef __SYSTICK_H
#define __SYSTICK_H


#include "stm32f10x.h"

void SYSTICK_Init(void);
void Delay_us(u16 nus);
void Delay_ms(u16 nms);
#endif
