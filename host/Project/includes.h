/********************   One Artisan's Regret And Pride   ***********************
*
*	@File    	： includes.h
*	@Brief   	： 智能柜头文件集合，其他的函数只需包含这一个头文件即可
*	@Author  	： Blade@oarap.org
*	@Hardware	： STM32F103RCT6
*	@Date		： 2016-05
*	@Description： 
*	@History	： 
*
*	Rev1.0 
*		Date：2016-05-16
*		Author：Blade
*		Modification：
*
*--------------------Copyright (C) oarap.org 2016------------------------------
*----------------------All rights reserved-------------------------------------
*
********************          一名工匠的遗憾与骄傲          ********************/

//官方库
#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "stm32f10x_it.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h" 
//驱动库
#include "smartbox.h"
#include "ccd.h"
#include "hmi.h"
#include "led.h"
#include "rtc.h"
#include "lock.h"
#include "usart.h"
//#include "i2c_sw.h"
#include "24C02.h"
#include "SYSTICK.h"
#include "syn6288.h"
#include "sim900a.h"
#include "lcd12864.h"

#ifndef __INCLUDES_H
#define __INCLUDES_H

#define BIT0	0x0001
#define BIT1	0x0002
#define BIT2	0x0004
#define BIT3	0x0008
#define BIT4	0x0010
#define BIT5	0x0020
#define BIT6	0x0040
#define BIT7	0x0080
#define BIT8	0x0100
#define BIT9	0x0200
#define BIT10	0x0400
#define BIT11	0x0800
#define BIT12	0x1000
#define BIT13	0x2000
#define BIT14	0x4000
#define BIT15	0x8000
#define BIT16	0x1000

//枚举布尔型变量
typedef enum 
{
	false,
	true
}bool;


#endif
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/





