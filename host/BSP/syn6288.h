#ifndef __SYN6288_H
#define __SYN6288_H

#include "stm32f10x.h"

#define BUSY  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7)


void SYN_FrameInfo(uint8_t Music,char *HZdata);
void SYN_Set(char *CMD);
void SYN_GPIO_Init(void);//GPIO初始化
u8 SYN_Init(void);
void SYN_Send(u8 Box_Num);//入口参数为数据位置


#endif
