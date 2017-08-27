/********************   One Artisan's Regret And Pride   ***********************
*
*	@File    	： syn6288.c
*	@Brief   	： syn6288语音合成模块的驱动
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

#include "includes.h"

/****************************************************************************
*	@Function name	： LV4CCD
*	@Description    ： 初始化LV4CCD并进行自检
*	@Author  		： Blade@oarap.org
*	@Date			： 2016-05
*	@param    		： 无
*	@return			： 无
*****************************************************************************/
u8 LV4CCD_Init()
{
  u8 i=0,n=0;	//i为重试次数
  //u8 Get_Release[]={0x02,0x85,0x03,0x84};//获取版本号，自带亦或校验
  u8 Get_Status[] ={0x02,0x83,0x03,0x82};//获取工作状态字，自带亦或校验
	while(!((UART_DATA.BAR_REC.FLAG) && (UART_DATA.BAR_REC.DATA[UART_DATA.BAR_REC.COUNTER-3] == 0x24)))
	{
		while(n<=3)
			USART_Send(USART3,Get_Status[n++]);//发送命令
		Delay_ms(200);
		n=0;i++;
		if(i>=10)
		{
		  UART_DATA.BAR_REC.FLAG = false;
		  UART_DATA.BAR_REC.COUNTER = 0;
		  return BIT2;//返回错误代码
		}
	}
	UART_DATA.BAR_REC.FLAG = false;
	UART_DATA.BAR_REC.COUNTER = 0;
	return 0;
}
	

