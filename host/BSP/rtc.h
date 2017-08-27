#ifndef __RTC_H
#define __RTC_H			 
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include <time.h>
#include <stdio.h>
#include <string.h>
typedef struct 
	{
    int Sec;  
    int Min;  
    int Hour; 
    int Mday; 
    int Mon;   
    int Year;   
}Clock_Type;
	
/* Private function prototypes -----------------------------------------------*/
void RTC_Init(void);
void Time_Display(void);
void Time_Regulate(void);
void RTC_IRQHandler(void);
void Time_Init(void);
void DateTimeDis_Init(void);
/* Private variables ---------------------------------------------------------*/
extern FunctionalState TimeDisplay;
struct tm Time_GetCalendarTime(void);
#endif



