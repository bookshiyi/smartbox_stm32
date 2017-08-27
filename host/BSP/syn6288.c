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


char SYN_StopCom[]		={0xFD,0X00,0X02,0X02,0XFD};//停止合成
char SYN_SuspendCom[]	={0XFD,0X00,0X02,0X03,0XFC};//暂停合成
char SYN_RecoverCom[]	={0XFD,0X00,0X02,0X04,0XFB};//恢复合成
char SYN_ChackCom[]		={0XFD,0X00,0X02,0X21,0XDE};//状态查询
char SYN_PowerDownCom[]	={0XFD,0X00,0X02,0X88,0X77};//进入POWER DOWN 状态命令
char SYN_Baud9600[]		={0XFD,0X00,0X03,0X31,0XCF};//波特率9600
char SYN_Baud19200[]	={0xFD,0x00,0x03,0x31,0x01,0xCE};

/****************************************************************************
*	@Function name	： YS-SYN6288 文本合成函数
*	@Description    ： 发送合成文本到SYN6288芯片进行合成播放
*	@Author  		： Blade@oarap.org
*	@Date			： 2016-05
*	@param    Music ： 0无背景音乐。1-15：相关背景音乐
*	@param  *HZdata ： 文本指针变量
*	@return			： 无
*****************************************************************************/
void SYN_FrameInfo(uint8_t Music,char *HZdata)
{ 
		 unsigned  char  headOfFrame[5];
         unsigned  char  length;  
		 unsigned  char  ecc  = 0;  			//定义校验字节
	     unsigned  int   i=0; 
		 
		 length =strlen(HZdata); 			//需要发送文本的长度          
		 headOfFrame[0] = 0xFD ; 			//构造帧头FD
		 headOfFrame[1] = 0x00 ; 			//构造数据区长度的高字节
		 headOfFrame[2] = length + 3; 		//构造数据区长度的低字节
		 headOfFrame[3] = 0x01 ; 			//构造命令字：合成播放命令
		 headOfFrame[4] = 0x01 | (Music<<4);//构造命令参数：编码格式为GBK

		 //while(BUSY);						//判忙等待，检测BUSY引脚2016-05-30
		 for(i = 0; i<5; i++)   				//依次发送构造好的5个帧头字节
	     {  
	         ecc=ecc^(headOfFrame[i]); 		//对发送的字节进行异或校验	 
			 USART_Send(UART4,headOfFrame[i]);
		 }

	   	 for(i = 0; i<length; i++)   		//依次发送待合成的文本数据
	     {  
	         ecc=ecc^(HZdata[i]); 				//对发送的字节进行异或校验	
			 USART_Send(UART4,HZdata[i]);
		 }

   		USART_Send(UART4,ecc);
//应加入回传握手检测的，相当于在数据链路层加入出错重传的机制，但是在应用层调用是没有感受的
		
}
/****************************************************************************
*	@Function name	： syn6288设置
*	@Description    ： 发送命令到SYN6288
*	@Author  		： Blade@oarap.org
*	@Date			： 2016-05
*	@param    		： 无
*	@return			： 无
*****************************************************************************/
void SYN_Set(char *CMD)
{
  u8 i;
  for(i=0;i<5;i++)
	USART_Send(UART4,CMD[i]);
}
/****************************************************************************
*	@Function name	： syn6288的BUSY引脚初始化
*	@Description    ： 
*	@Author  		： Blade@oarap.org
*	@Date			： 2016-06
*	@param    		： 无
*	@return			： 无
*****************************************************************************/
void SYN_GPIO_Init()
{	
  	//定义一个GPIO_InitTypeDef 类型的结构体
	GPIO_InitTypeDef  GPIO_InitStructure;
	//开启GPIOC的外设时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	//选择要用的GPIO引脚		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	//设置引脚模式为上拉输入模式				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 
	//调用库函数，初始化GPIO
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
	
}
/****************************************************************************
*	@Function name	： syn6288初始化
*	@Description    ： 初始化波特率，发送欢迎语
*	@Author  		： Blade@oarap.org
*	@Date			： 2016-05
*	@param    		： 无
*	@return			： 无
*****************************************************************************/
u8 SYN_Init()
{//d-清除全局变量 v-前景音等级最大16 m-背景音等级最大16 t-语速等级最大6 y-1的读法 o-文本朗读方式
  u8 i;	
  char init_param[]= "[d][v10][m2][t6][n2][y1][o0]";//强制按照数值的方式合成,1读作yao

	SYN_GPIO_Init();
  	SYN_Set(SYN_Baud9600);
	Delay_ms(100);	
	SYN_FrameInfo(0,init_param);
	Delay_ms(100);	
	while(!((UART_DATA.SYN_REC.DATA[0]==0X41) || (UART_DATA.SYN_REC.DATA[0]==0X4E) || (UART_DATA.SYN_REC.DATA[0]==0X4F)))
	{
		SYN_FrameInfo(1,"欢");//SYN_FrameInfo(1,"欢迎使用智能送餐系统");
		Delay_ms(200);
		i++;
		if(i>=10)
		{
		  UART_DATA.SYN_REC.FLAG = false;
		  return BIT1;//返回错误代码
		}
	}
	UART_DATA.SYN_REC.FLAG = false;
	SYN_Set(SYN_StopCom);
	Delay_ms(300);
	SYN_FrameInfo(1,"欢迎使用智能送餐系统");
	//while(BUSY);
	return 0;
}
/****************************************************************************
*	@Function name	： 语音提示合成
*	@Description    ： 提示用户密码是否正确和箱门号
*	@Author  		： Blade@oarap.org
*	@Date			： 2016-05
*	@param  box_num	： 餐柜所在的数据库位置
*	@return			： 无
*****************************************************************************/
void SYN_Send(u8 Box_Num)//入口参数为数据位置
{
	char Voice_Buf[] = "nn号箱门已打开";
	Voice_Buf[0] = SmartBox_DataBase[Box_Num].Box_Ascii[0];//转换成ASCII码的形式
	Voice_Buf[1] = SmartBox_DataBase[Box_Num].Box_Ascii[1];
	if(Box_Num)
		SYN_FrameInfo(0,Voice_Buf);
	else
		SYN_FrameInfo(0,"密码错误");
}
