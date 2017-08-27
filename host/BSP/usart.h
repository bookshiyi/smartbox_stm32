


#ifndef __USART_H
#define __USART_H
/*
#include "stm32f10x.h"
#include <stdio.h>*/

#include "includes.h"

#define HMI_RAM_PTR		5 //HMI用户内存区中的数据标识符指针

typedef struct //二级结构体
{
  bool  FLAG;
  u8    DATA[30];
  u8    COUNTER;
}Element_Data_TypeDef;//元素定义

typedef struct //一级结构体
{
  Element_Data_TypeDef BAR_REC;                   
  Element_Data_TypeDef SIM_REC;
  Element_Data_TypeDef SYN_REC;
  Element_Data_TypeDef HMI_REC;
  
  /*DATA_TYPE_DEF BAR_SEND;                   
  DATA_TYPE_DEF SIM_SEND;
  DATA_TYPE_DEF SYN_SEND;
  DATA_TYPE_DEF SCR_SEND;*/
}UART_Data_TypeDef;

//在对应的.c中定义后只要在其.h中声明为外部变量就可以在其他地方调用
extern UART_Data_TypeDef UART_DATA;


void USART1_Config(void);
void USART1_IRQHandler(void);
void USART2_Config(void);
void USART2_IRQHandler(void);
void USART3_Config(void);
void USART3_IRQHandler(void);
void UART4_Config(void);
void UART4_IRQHandler(void);
void UART5_Config(void);
void UART5_IRQHandler(void);
void NVIC_Configuration(void);
int fputc(int ch, FILE *f);
void USART1_Putc(u8 c);
void USART1_Puts(char * str);



void USART_Send(USART_TypeDef* USARTx,u16 data);//USART发送函数


#endif
