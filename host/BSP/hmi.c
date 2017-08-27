/********************   One Artisan's Regret And Pride   ***********************
*
*	@File    	： hmi.c
*	@Brief   	： 人机交互界面的驱动文件
*	@Author  	： Blade@oarap.org
*	@Hardware	： STM32F103RCT6
*	@Date		： 2016-05
*	@Description： 负责驱动人机交互界面
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
*	@Function name	： 人机界面初始化
*	@Description    ： 显示启动文字
*	@Author  		： Blade@oarap.org
*	@Date			： 2016-05
*	@param    		： 无
*	@return			： 无
*****************************************************************************/
u8 Hmi_Init(u8 num)
{
  u8 touch[]={"----------轻触LOGO后进入系统--"};
  u8 load []={"------     Loading     ------"};
  u8 clear[]={0xFF,0xFF};//清屏指令
  u8 self_check[]={0xA5,0X5A,0x01};
  u8 i=0;
  switch(num)
  {
  	case 0:
		Hmi_Send_reg(VERSION_PTR,REG_W,clear,0,2);//同步波特率
		Hmi_Page_Switch(WELCOME);//显示第0页面
		Hmi_Send(WELCOME_CACHE,VAR_W,load,0,30);//显示loading
		break;
	case 1:
		Hmi_Send(WELCOME_CACHE,VAR_W,clear,0,2);//欢迎语的缓冲区清空
		Hmi_Send(WELCOME_CACHE,VAR_W,self_check,0,2);//写入自检命令在【欢迎语缓存区】
		while(!((UART_DATA.HMI_REC.FLAG==true) && (UART_DATA.HMI_REC.DATA[6]==0X01) && (UART_DATA.HMI_REC.DATA[7]==0XA5) && (UART_DATA.HMI_REC.DATA[8]==0X5A)))
		{
			Hmi_Send(WELCOME_CACHE,VAR_R,self_check,2,1);//读取数据
			Delay_ms(200);
			i++;
			if(i>=10)
			{
			  UART_DATA.HMI_REC.FLAG=false;//清空接收标志位
			  return BIT3;//提前结束，并返回错误代码
			}
		}
		Hmi_Send(WELCOME_CACHE,VAR_W,clear,0,2);//欢迎语的缓冲区清空	  
		Hmi_Send(WELCOME_CACHE,VAR_W,touch,0,30);
		//UART_DATA.BAR_REC.FLAG=false;//清空接收标志位
		break;
  }
  UART_DATA.HMI_REC.FLAG=false;//清空接收标志位

  return 0;
}
/****************************************************************************
*	@Function name	： 数据发送，对用户存储位置进行操作
*	@Description    ： 将数据封装成指定的帧格式发送给HMI
*	@Author  		： Blade@oarap.org
*	@Date			： 2016-05
*	@param send_add ： 变量地址 
*	@param send_cmd ： 命令｛写寄存器，读寄存器，写变量，读变量，画图缓存
*	@param send_data： 如果为写入命令，send_data为数据，如果为读取命令，send_data为欲读取的长度
*	@param start_ptr： 起始位置
*	@param send_len ： 发送的长度 
*	@return			： 无
*****************************************************************************/
void Hmi_Send(u16 send_add,u8 send_cmd,u8 *send_data,u8 start_ptr,u8 send_len)
{
  u8 i=0;
  u8 frame_head[6]={0x5A,0XA5};//定义数据帧
  frame_head[STR_LEN] = send_len + 3;//strlen(send_data)+3; //放入数据长度到数据帧
  frame_head[HMI_CMD] = send_cmd;  
  frame_head[ADD_H]   = send_add>>8;//小端模式的操作
  frame_head[ADD_L]   = send_add;
  while(i!=6)
  	USART_Send(USART1,frame_head[i++]);
  for(i=start_ptr;i< start_ptr + send_len;i++)
  	USART_Send(USART1,send_data[i]);
}
/****************************************************************************
*	@Function name	： 数据发送，对系统寄存器进行读取操作
*	@Description    ： 将数据封装成指定的帧格式发送给HMI
*	@Author  		： Blade@oarap.org
*	@Date			： 2016-05
*	@param send_add ： 变量地址 
*	@param send_cmd ： 命令｛写寄存器，读寄存器，写变量，读变量，画图缓存
*	@param send_data： 如果为写入命令，send_data为数据，如果为读取命令，send_data为欲读取的长度
*	@param start_ptr： 起始位置
*	@param send_len ： 发送的长度 
*	@return			： 无
*****************************************************************************/
void Hmi_Send_reg(u16 send_add,u8 send_cmd,u8 *send_data,u8 start_ptr,u8 send_len)
{
  u8 i=0;
  u8 frame_head[5]={0x5A,0XA5};//定义数据帧
  frame_head[STR_LEN] = send_len + 2;//strlen(send_data)+3; //放入数据长度到数据帧
  frame_head[HMI_CMD] = send_cmd;  
  frame_head[ADD]     = send_add;
  while(i!=5)
  	USART_Send(USART1,frame_head[i++]);
  for(i=start_ptr;i< start_ptr + send_len;i++)
  	USART_Send(USART1,send_data[i]);
}
/****************************************************************************
*	@Function name	： 人机界面页面切换
*	@Description    ： 
*	@Author  		： Blade@oarap.org
*	@Date			： 2016-05
*	@param    (u8)id： 指定的页面号
*	@return			： 无
*****************************************************************************/
void Hmi_Page_Switch(u8 id)
{
		u8 pic_id[]={0x00,0x00};
		
		pic_id[1] =id;
    Hmi_Send_reg(PIC_ID_PTR,REG_W,pic_id,0,2);//显示第0页面
}
/*
void Hmi_Read_reg(u16 send_add,u8 send_cmd,u8 read_len,u8 start_ptr,u8 send_len)
{
  u8 i=0;
  u8 frame_head[5]={0x5A,0XA5};//定义数据帧
  frame_head[STR_LEN] = send_len + 2;//strlen(send_data)+3; //放入数据长度到数据帧
  frame_head[HMI_CMD] = send_cmd;  
  frame_head[ADD]     = send_add;
  while(i!=5)
  	USART_Send(USART1,frame_head[i++]);
  for(i=start_ptr;i< start_ptr + send_len;i++)
  	USART_Send(USART1,send_data[i]);
}*/
/****************************************************************************
*	@Function name	： 人机界面的RTC读取
*	@Description    ： 
*	@Author  		： Blade@oarap.org
*	@Date			： 2016-05
*	@param    (u8)id： 指定的页面号
*	@return			： 无
*****************************************************************************/
u8 Hmi_RTC_Read()//0x20为日期 0x24为时间 的起始地址
{//格式为：年 月 日 周 时 分 秒 
	/*u8 Date_Len[]={0x03};//日期的数据长度为3	从0x20开始
	u8 Time_Len[]={0x03};//时间的数据长度为3 从0x24开始*/
	u8 RTC_Len[]={0x07};//从0x20开始
	u8 i=10;

    Hmi_Send_reg(RTC_PTR,REG_R,RTC_Len,0,1);
	while((!(UART_DATA.HMI_REC.FLAG)) && (i--))
	{
		Delay_ms(10);//一共等待了100ms
		if(i==1)
		{
		  UART_DATA.HMI_REC.FLAG=false;
		  UART_DATA.HMI_REC.COUNTER=0;
		  return BIT5;
		}
	}
	UART_DATA.HMI_REC.FLAG=false;
	UART_DATA.HMI_REC.COUNTER=0;
	return 0;
}
