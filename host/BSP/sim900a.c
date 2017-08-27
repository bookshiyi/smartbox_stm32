/********************   One Artisan's Regret And Pride   ***********************
*
*	@File    	： sim900a.c
*	@Brief   	： SIM900A的驱动文件
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

char Text_Message[]  = {"301083DC5305914D900130115BA25B98FF0C5C0F4E8C628A59165356653E57288FBD5B815DE57A0B59275B660030003000317EC4667A80FD67DC76840058005853F7991067DC4E2DFF0C53D699105BC67801005800580058005800580058FF0C4EC55F5365E500320031003A00300030524D670965483002"};
char Receiver[] = {"AT+CMGS=\"00310035003500360036003600310032003200300036\"\r\n"};

/****************************************************************************
*	@Function name	： SIM900A初始化
*	@Description    ： 同步波特率，//设置参数，查询状态
*	@Author  		： Blade@oarap.org
*	@Date			： 2016-05
*	@param    		： 无
*	@return			： 无
*****************************************************************************/
u8 Sim900a_Init()
{
    //char buf[] = "AT\r\n";
	u8 i=0;//记录初始化次数
	
	UART_DATA.SIM_REC.FLAG = false;
	UART_DATA.SIM_REC.COUNTER = 0;//复位接收指针/计数器
	Sim900a_Send("AT+CFUN=1,1\r\n");
	Delay_ms(50);
	Sim900a_Send("AT+CFUN=1,1\r\n");//发送重启指令
	 while(!((UART_DATA.SIM_REC.FLAG) && (UART_DATA.SIM_REC.DATA[UART_DATA.SIM_REC.COUNTER-4]=='d') && (UART_DATA.SIM_REC.DATA[UART_DATA.SIM_REC.COUNTER-3]=='y')))
	 {//Call Ready
	   if(UART_DATA.SIM_REC.FLAG)
	   {
		 if(!((UART_DATA.SIM_REC.DATA[UART_DATA.SIM_REC.COUNTER-4]=='d') && (UART_DATA.SIM_REC.DATA[UART_DATA.SIM_REC.COUNTER-3]=='y')))
		 {
		   UART_DATA.SIM_REC.FLAG = false;
			UART_DATA.SIM_REC.COUNTER = 0;//复位接收指针/计数器
		 }
	   }
		Delay_ms(200);//while(!(UART_DATA.SIM_REC.FLAG);
		i++;
		if(i>=100)
		{
		  	UART_DATA.SIM_REC.FLAG = false;
			UART_DATA.SIM_REC.COUNTER = 0;//复位接收指针/计数器
			return BIT0;
		}
	 }
	i=0;
	UART_DATA.SIM_REC.FLAG = false;
	UART_DATA.SIM_REC.COUNTER = 0;//复位接收指针/计数器
	
	while(!((UART_DATA.SIM_REC.FLAG) && (UART_DATA.SIM_REC.DATA[UART_DATA.SIM_REC.COUNTER-4]=='O') && (UART_DATA.SIM_REC.DATA[UART_DATA.SIM_REC.COUNTER-3]=='K')))
	{//在此等待，如果初始化不成功反复发送
		Sim900a_Send("AT\r\n");
		Delay_ms(200);//while(!(UART_DATA.SIM_REC.FLAG);
		i++;
		if(i>=10)
		{
		  	UART_DATA.SIM_REC.FLAG = false;
			UART_DATA.SIM_REC.COUNTER = 0;//复位接收指针/计数器
			return BIT0;
		}
	}
	i=0;
	UART_DATA.SIM_REC.FLAG = false;
	UART_DATA.SIM_REC.COUNTER = 0;//复位接收指针/计数器
	
	
	while(!((UART_DATA.SIM_REC.FLAG) && (UART_DATA.SIM_REC.DATA[UART_DATA.SIM_REC.COUNTER-4]=='O') && (UART_DATA.SIM_REC.DATA[UART_DATA.SIM_REC.COUNTER-3]=='K')))
	{//删除所有短信
		Sim900a_Send("AT+CMGD=1,4\r\n");
		Delay_ms(200);//while(!(UART_DATA.SIM_REC.FLAG);
		i++;
		if(i>=10)
		{
		  	UART_DATA.SIM_REC.FLAG = false;
			UART_DATA.SIM_REC.COUNTER = 0;//复位接收指针/计数器
			return BIT0;
		}
	}
	i=0;
	UART_DATA.SIM_REC.FLAG = false;
	UART_DATA.SIM_REC.COUNTER = 0;//复位接收指针/计数器

	while(!((UART_DATA.SIM_REC.FLAG) && (UART_DATA.SIM_REC.DATA[UART_DATA.SIM_REC.COUNTER-4]=='O') && (UART_DATA.SIM_REC.DATA[UART_DATA.SIM_REC.COUNTER-3]=='K')))
	{//在此等待，如果初始化不成功反复发送
		Sim900a_Send("AT+CSCS=\"UCS2\"\r\n");
		Delay_ms(200);//while(!(UART_DATA.SIM_REC.FLAG);
		i++;
		if(i>=10)
		{
		  	UART_DATA.SIM_REC.FLAG = false;
			UART_DATA.SIM_REC.COUNTER = 0;//复位接收指针/计数器
			return BIT0;
		}
	}
	i=0;
	UART_DATA.SIM_REC.FLAG = false;
	UART_DATA.SIM_REC.COUNTER = 0;//复位接收指针/计数器
	
	while(!((UART_DATA.SIM_REC.FLAG) && (UART_DATA.SIM_REC.DATA[UART_DATA.SIM_REC.COUNTER-4]=='O') && (UART_DATA.SIM_REC.DATA[UART_DATA.SIM_REC.COUNTER-3]=='K')))
	{//在此等待，如果初始化不成功反复发送
		Sim900a_Send("AT+CSMP=17,167,2,25\r\n");
		Delay_ms(200);//while(!(UART_DATA.SIM_REC.FLAG);
		i++;
		if(i>=10)
		{
		  	UART_DATA.SIM_REC.FLAG = false;
			UART_DATA.SIM_REC.COUNTER = 0;//复位接收指针/计数器
			return BIT0;
		}
	}
	i=0;
	UART_DATA.SIM_REC.FLAG = false;
	UART_DATA.SIM_REC.COUNTER = 0;//复位接收指针/计数器
		
	while(!((UART_DATA.SIM_REC.FLAG) && (UART_DATA.SIM_REC.DATA[UART_DATA.SIM_REC.COUNTER-4]=='O') && (UART_DATA.SIM_REC.DATA[UART_DATA.SIM_REC.COUNTER-3]=='K')))
	{//在此等待，如果初始化不成功反复发送
		Sim900a_Send("AT+CMGF=1\r\n");
		Delay_ms(200);//while(!(UART_DATA.SIM_REC.FLAG);
		i++;
		if(i>=10)
		{
		  	UART_DATA.SIM_REC.FLAG = false;
			UART_DATA.SIM_REC.COUNTER = 0;//复位接收指针/计数器
			return BIT0;
		}
	}
	i=0;
	UART_DATA.SIM_REC.FLAG = false;
	UART_DATA.SIM_REC.COUNTER = 0;//复位接收指针/计数器

	return 0;
	
}
/****************************************************************************
*	@Function name	： SIM900A字符串匹配
*	@Description    ： 
*	@Author  		： Blade@oarap.org
*	@Date			： 2016-05
*	@param    *str1	： 字符串1的地址
*	@param    *str1	： 字符串2的地址
*	@param  str2_ptr： 字符串2的起始地址
*	@return			： 无
*****************************************************************************/
bool Sim900a_Str_Cmp(u8 *str1,u8 *str2,u8 str2_ptr)
{
    u8 i=0;
    u8 cmp_len;
    u8 cmp_val = BIT0;//返回值
    cmp_len = strlen(str1);//获取字符串1的长度
	for(i=0;i < cmp_len;i++)
	{	
	  if(str1[i] == str2[str2_ptr+i])
		cmp_val <<= 1;
	}
	if (cmp_val>>cmp_len == BIT1)
	  return true;
  return false;
}
/****************************************************************************
*	@Function name	： Sim900a字符串发送
*	@Description    ： 
*	@Author  		： Blade@oarap.org
*	@Date			： 2016-05
*	@param char *send_data： 字符串/数组的头指针
*	@return			： 无
*****************************************************************************/
void Sim900a_Send(char *send_data)
{ 
  u8 i=0;
  while(send_data[i]!='\0')
  	USART_Send(UART5,send_data[i++]);
}
/****************************************************************************
*	@Function name	： Sim900a字符串发送  定长
*	@Description    ： 发送任意长度的字符串/数组
*	@Author  		： Blade@oarap.org
*	@Date			： 2016-05
*	@param *send_data： 字符串/数组的头指针
*	@param start_ptr： 起始位置
*	@param send_len ： 发送的长度 
*	@return			： 无
*****************************************************************************/
void Sim900a_Send_Len(char *send_data,u8 start_ptr,u8 send_len)
{ 
  u8 i;
  for(i=start_ptr;i< start_ptr + send_len;i++)
  	USART_Send(UART5,send_data[i]);
}
/****************************************************************************
*	@Function name	： Sim900a发送短信
*	@Description    ： 发送外卖短信到指定的电话号码
*	@Author  		： Blade@oarap.org
*	@Date			： 2016-05
*	@param *order_num： 订单号，内含手机号码
*	@param box_start ：  在短信中箱门号的起始位置
*	@param pwd_start ： 在短信中密码的起始位置
*	@param box_num  ： 输入当前的箱门号（用来查找密码和箱门号ASCII）
*	@return			： 无
*****************************************************************************/
bool Sim900a_Send_Message(u8 *Order_Num,u8 Box_Num)
{
	u8 i=0,j=0;
	char End_Symbol = 0x1a;
	char *End_Symbol_Ptr = &End_Symbol;
	
	for(i=0;i<=5;i++)//单字节转双字节
	{//密码
		Text_Message[PWD_START_POS   + (4*i)] = (SmartBox_DataBase[Box_Num].pwd[i] + 48)/16 + 48;
		Text_Message[PWD_START_POS+1 + (4*i)] = (SmartBox_DataBase[Box_Num].pwd[i] + 48)%16 + 48;
	}
	for(i=0;i<=1;i++)
	{//箱门号
		Text_Message[BOXNUM_START_POS   + (4*i)]=SmartBox_DataBase[Box_Num].Box_Ascii[i]/16 + 48;
		Text_Message[BOXNUM_START_POS+1 + (4*i)]=SmartBox_DataBase[Box_Num].Box_Ascii[i]%16 + 48;
	}
	for(i=0;i<=2;i++)
	{//柜组号
		Text_Message[GROUP_NUM_START_POS   + (4*i)]=SmartBox_Arg.Group_Num[i]/16 + 48;
		Text_Message[GROUP_NUM_START_POS+1 + (4*i)]=SmartBox_Arg.Group_Num[i]%16 + 48;
	}
	for(i=0;i<=1;i++)
	{//小时
		Text_Message[LATEST_TIME_START_POS   + (4*i)]=SmartBox_Arg.Latest_Time[i]/16 + 48;
		Text_Message[LATEST_TIME_START_POS+1 + (4*i)]=SmartBox_Arg.Latest_Time[i]%16 + 48;
	}
	for(i=3;i<=4;i++)
	{//分钟
		Text_Message[LATEST_TIME_START_POS   + (4*i)]=SmartBox_Arg.Latest_Time[i-1]/16 + 48;
		Text_Message[LATEST_TIME_START_POS+1 + (4*i)]=SmartBox_Arg.Latest_Time[i-1]%16 + 48;
	}

	
	for(i=0;i<=11;i++)
	{
	  Receiver[12+(4*i)] = Order_Num[2+i];//从第12位开始是电话号码部分
	}
	Sim900a_Send(Receiver);//发送接收短信的手机号码
	Delay_ms(500);
	
	Sim900a_Send(Text_Message);
	Sim900a_Send(End_Symbol_Ptr);//发送短信停止符号
	
	while(!((UART_DATA.SIM_REC.FLAG) && (UART_DATA.SIM_REC.DATA[UART_DATA.SIM_REC.COUNTER-4]=='O') && (UART_DATA.SIM_REC.DATA[UART_DATA.SIM_REC.COUNTER-3]=='K')))
	{//在此等待从传输完数据到发送完短信要5s左右，要考虑是否加入这个模块
		Delay_ms(1000);//while(!(UART_DATA.SIM_REC.FLAG);
		j++;
		if(j>=10)
		{
		  	UART_DATA.SIM_REC.FLAG = false;
			UART_DATA.SIM_REC.COUNTER = 0;//复位接收指针/计数器
			return false;
		}
	}
	UART_DATA.SIM_REC.FLAG=false;
	UART_DATA.SIM_REC.COUNTER=0;
	
	return true;
}




