/********************   One Artisan's Regret And Pride   ***********************
*
*	@File    	： lock.c
*	@Brief   	： 译码器74ls164的驱动
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
*	@Function name	： 译码器（两片74ls164级联）的引脚配置
*	@Description    ： 配置GPIO后关闭输出，防止门被打开
*	@Author  		： Blade@oarap.org
*	@Date			： 2016-05
*	@param    		： 无
*	@return			： 无
*****************************************************************************/
void Decoder_GPIO_Config(void) //GPIOC.0-3
{	
    //定义一个GPIO_InitTypeDef 类型的结构体
    GPIO_InitTypeDef  GPIO_InitStructure;	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//使能GPIO的外设时钟

    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 ;//选择要用的GPIO引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //设置引脚模式为推免输出模式						 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//设置引脚速度为50MHZ         
    GPIO_Init(GPIOC, &GPIO_InitStructure);//调用库函数，初始化GPIO
    //GPIO_SetBits(GPIOC,GPIO_Pin_5 | GPIO_Pin_4);//上电后置高两个使能端口，关闭译码器输出
	
	Decoder_Output(0x1f);//打开32，目的为初始化时尽早确立引脚电平状态，防止继电器被误打开
}
/****************************************************************************
*	@Function name	： 译码器输出
*	@Description    ： 驱动两片74ls164级联，位操作的方式
*	@Author  		： Blade@oarap.org
*	@Date			： 2016-05
*	@param  PortVal	： 送给译码器的数值
*	@return			： 无
*****************************************************************************/
void Decoder_Output(uint16_t PortVal)//PA0,PA1,PA2,PA3;为低端  PA4 PA5为片选端
{
/*
  PortVal |= 0xfff0;//预处理：将数据转换为低半字节位操作的模式
  GPIOx->ODR &= PortVal;
  */
  if(!(PortVal&BIT4))//如果第5位为低。说明小于16，使能第一片（置低），关短第二片（置高）
  {
  	PortVal &= 0xFF0F;
	PortVal |= BIT5;
  }
  GPIOC->ODR = (GPIOC->ODR & 0xFFC0) | PortVal;//将低六位的数据放入输出寄存器
}
/****************************************************************************
*	@Function name	： 电磁锁测试
*	@Description    ： 逐一打开所有电磁锁
*	@Author  		： Blade@oarap.org
*	@Date			： 2016-05
*	@param    		： 无
*	@return			： 无
*****************************************************************************/
void Lock_Test()
{
    u16 i;
	for(i=0;i<24;i++)
	{
		Decoder_Output(i);/*用&运算符，高字节用FF*/
		Delay_ms(LOCK_DELAY);
	}
}
/****************************************************************************
*	@Function name	： 开锁
*	@Description    ： 打开指定餐柜的箱门（持续LOCK_DELAY个MS的高电平）
*	@Author  		： Blade@oarap.org
*	@Date			： 2016-05
*	@param  （u16）i： 打开指定的锁（从1开始）
*	@return			： 无
*****************************************************************************/
void Lock_Open(u16 i)
{
	Decoder_Output(i-1);
	Delay_ms(LOCK_DELAY);
	Decoder_Output(0x1f);
}
