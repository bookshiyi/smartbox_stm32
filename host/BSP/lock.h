/********************   One Artisan's Regret And Pride   ***********************
*
*	@File    	： lock.h
*	@Brief   	： 
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

#ifndef _DECODER_H
#define _DECODER_H

#define LOCK_DELAY 300	//电磁锁通电的持续时间，单位MS

void Decoder_GPIO_Config(void);
void Decoder_Output(uint16_t PortVal);
void Lock_Test(void);
void Lock_Open(u16 i);//开锁函数

#endif
