/********************   One Artisan's Regret And Pride   ***********************
*
*	@File    	： usart.c
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

#include "includes.h"

UART_Data_TypeDef UART_DATA={{{false},{0},{0}},{{false},{0},{0}},{{false},{0},{0}},{{false},{0},{0}}};//初始化 UART_DATA结构体


//=============================================================================
//函数名称:USART1_Config
//功能概要:USART1/GPIO 配置,工作模式配置。9600-8-N-1
//参数说明:无
//函数返回:无
//=============================================================================
void USART1_Config(void)
{

  GPIO_InitTypeDef  GPIO_InitStructure;	
  USART_InitTypeDef USART_InitStructure;  //定义串口初始化结构体
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
 
  /*USART1_TX ->PA9*/			
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	       //选中串口默认输出管脚         
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //定义输出最大速率 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//定义管脚2的模式  
  GPIO_Init(GPIOA, &GPIO_InitStructure);           //调用函数，把结构体参数输入进行初始化		   
  /*USART1_RX ->PA10*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	        
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  /*串口通讯参数设置*/
  USART_InitStructure.USART_BaudRate = 57600; //波特率
  USART_InitStructure.USART_WordLength = USART_WordLength_8b; //数据位8位
  USART_InitStructure.USART_StopBits = USART_StopBits_1;	//停止位1位
  USART_InitStructure.USART_Parity = USART_Parity_No;		//校验位 无
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无流控制
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//使能接收和发送引脚
   //将以上赋完值的结构体带入库函数USART_Init进行初始化
  USART_Init(USART1, &USART_InitStructure); 
  USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//使能串口中断接收
  USART_Cmd(USART1, ENABLE);//开启USART1，注意与上面RCC_APB2PeriphClockCmd()设置的区别
}
//=============================================================================
//函数名称:USART2_Config
//功能概要:USART2/GPIO 配置,工作模式配置。9600-8-N-1
//参数说明:无
//函数返回:无
//=============================================================================
void USART2_Config(void)
{	
  GPIO_InitTypeDef GPIO_InitStructure;	
  USART_InitTypeDef USART_InitStructure;  //定义串口初始化结构体
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
 
  /*USART2_TX ->PA2*/			
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	       //选中串口默认输出管脚         
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //定义输出最大速率 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//定义管脚2的模式  
  GPIO_Init(GPIOA, &GPIO_InitStructure);           //调用函数，把结构体参数输入进行初始化		   
  /*USART2_RX ->PA3*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	        
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  /*串口通讯参数设置*/
  USART_InitStructure.USART_BaudRate = 9600; //波特率
  USART_InitStructure.USART_WordLength = USART_WordLength_8b; //数据位8位
  USART_InitStructure.USART_StopBits = USART_StopBits_1;	//停止位1位
  USART_InitStructure.USART_Parity = USART_Parity_No;		//校验位 无
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无流控制
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//使能接收和发送引脚
   //将以上赋完值的结构体带入库函数USART_Init进行初始化
  USART_Init(USART2, &USART_InitStructure); 
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);//使能串口中断接收
  USART_Cmd(USART2, ENABLE);//开启USART1，注意与上面RCC_APB2PeriphClockCmd()设置的区别
}
//=============================================================================
//函数名称:USART3_Config
//功能概要:USART3/GPIO 配置,工作模式配置。9600-8-N-1
//参数说明:无
//函数返回:无
//=============================================================================
void USART3_Config(void)//LV4红外CCD
{	
  GPIO_InitTypeDef GPIO_InitStructure;	
  USART_InitTypeDef USART_InitStructure;  //定义串口初始化结构体
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
 
  /*USART3_TX ->PB10*/			
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	       //选中串口默认输出管脚         
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //定义输出最大速率 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//定义管脚9的模式  
  GPIO_Init(GPIOB, &GPIO_InitStructure);           //调用函数，把结构体参数输入进行初始化		   
  /*USART3_RX ->PB11*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	        
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  /*串口通讯参数设置*/
  USART_InitStructure.USART_BaudRate = 9600; //波特率
  USART_InitStructure.USART_WordLength = USART_WordLength_8b; //数据位8位
  USART_InitStructure.USART_StopBits = USART_StopBits_1;	//停止位1位
  USART_InitStructure.USART_Parity = USART_Parity_No;		//校验位 无
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无流控制
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//使能接收和发送引脚
   //将以上赋完值的结构体带入库函数USART_Init进行初始化
  USART_Init(USART3, &USART_InitStructure); 
  USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);//使能串口中断接收
  USART_Cmd(USART3, ENABLE);//开启USART3，注意与上面RCC_APB2PeriphClockCmd()设置的区别
}
//=============================================================================
//函数名称:UART4_Config
//功能概要:UART4/GPIO 配置,工作模式配置。9600-8-N-1
//参数说明:无
//函数返回:无
//=============================================================================
void UART4_Config(void)//SYM6288
{	
  GPIO_InitTypeDef GPIO_InitStructure;	
  USART_InitTypeDef USART_InitStructure;  //定义串口初始化结构体
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
  
  /*UART4_TX ->PC10*/			
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	   //选中串口默认输出管脚         
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //定义输出最大速率 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//定义管脚9的模式  
  GPIO_Init(GPIOC, &GPIO_InitStructure);           //调用函数，把结构体参数输入进行初始化		   
  /*UART4_RX ->Pc11*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	        
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  /*串口通讯参数设置*/
  USART_InitStructure.USART_BaudRate = 9600; //波特率
  USART_InitStructure.USART_WordLength = USART_WordLength_8b; //数据位8位
  USART_InitStructure.USART_StopBits = USART_StopBits_1;	//停止位1位
  USART_InitStructure.USART_Parity = USART_Parity_No;		//校验位 无
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无流控制
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//使能接收和发送引脚
   //将以上赋完值的结构体带入库函数UART_Init进行初始化
  USART_Init(UART4, &USART_InitStructure); 
  USART_ITConfig(UART4,USART_IT_RXNE,ENABLE);//使能串口中断接收
  USART_Cmd(UART4, ENABLE);//开启UART5，注意与上面RCC_APB2PeriphClockCmd()设置的区别
}
//=============================================================================
//函数名称:UART5_Config
//功能概要:UART5/GPIO 配置,工作模式配置。9600-8-N-1
//参数说明:无
//函数返回:无
//=============================================================================
void UART5_Config(void)//gsm900a
{	
  GPIO_InitTypeDef GPIO_InitStructure;	
  USART_InitTypeDef USART_InitStructure;  //定义串口初始化结构体
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);
  
  /*UART5_TX ->PC12*/			
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;	   //选中串口默认输出管脚         
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //定义输出最大速率 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//定义管脚9的模式  
  GPIO_Init(GPIOC, &GPIO_InitStructure);           //调用函数，把结构体参数输入进行初始化		   
  /*UART5_RX ->PD2*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	        
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  /*串口通讯参数设置*/
  USART_InitStructure.USART_BaudRate = 9600; //波特率
  USART_InitStructure.USART_WordLength = USART_WordLength_8b; //数据位8位
  USART_InitStructure.USART_StopBits = USART_StopBits_1;	//停止位1位
  USART_InitStructure.USART_Parity = USART_Parity_No;		//校验位 无
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无流控制
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//使能接收和发送引脚
   //将以上赋完值的结构体带入库函数UART_Init进行初始化
  USART_Init(UART5, &USART_InitStructure); 
  USART_ITConfig(UART5,USART_IT_RXNE,ENABLE);//使能串口中断接收
  USART_Cmd(UART5, ENABLE);//开启UART5，注意与上面RCC_APB2PeriphClockCmd()设置的区别
}
//=============================================================================
//函数名称:NVIC_Configuration
//功能概要:中断向量配置
//参数说明:无
//函数返回:无
//=============================================================================
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure; 
  /* Configure the NVIC Preemption Priority Bits */  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  
  /* Enable the USARTy Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;	 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;	 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;	 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;	 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
//=============================================================================
//函数名称:USART_Send
//功能概要:向指定的USART发送数据并且还有等待功能，发送完毕后自动清空标志位
//参数说明:无
//函数返回:无
//=============================================================================
void USART_Send(USART_TypeDef* USARTx,uint16_t data)
{
	USART_SendData(USARTx,data);
	while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)!=SET);//等待发送完成中断标志位	
	 USART_ClearFlag(USARTx,USART_FLAG_TC);
}
//=============================================================================
//函数名称:USART1_IRQHandler
//功能概要:
//参数说明:无
//函数返回:无
//=============================================================================
void USART1_IRQHandler(void)//HMI接口
{
  //访问DWIN屏幕的数据存储器时数据帧结构：两个字节帧头+一个字节数据长度（包括后面所有的数据）+两字节地址+数据
  
  
  //ASCII录入返回的是数据的ASCII值 
  //应答数据的结构两个字节帧头+一个字节 该帧数据长度（包括后面所有的数据,字节长度）+两字节地址+读了几个/*字*/的数据+数据
  //如果返回的变量是奇数个字节，会在变量的最后两个FF后填充一个0x00.言外之意，数据帧的字节长度为奇数个
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
  { 		
	UART_DATA.HMI_REC.DATA[UART_DATA.HMI_REC.COUNTER++] = USART_ReceiveData(USART1);//
	if(UART_DATA.HMI_REC.COUNTER > 2)//当接收到第三字节数据时 此数据对应据帧的长度（言外之意不能在接收两个字节的时候就判断数据的长度，因为第三个字节还没被刷新，还是上一次的数据，无法做出判断）
	{
		if(UART_DATA.HMI_REC.COUNTER>=UART_DATA.HMI_REC.DATA[2]+3 || (UART_DATA.HMI_REC.COUNTER>=30))  
		{	//如果超出或等于帧长就结束本次接收
		  if((UART_DATA.HMI_REC.DATA[0]==0x5A)&&(UART_DATA.HMI_REC.DATA[1]==0xA5))//数据帧完整
			  UART_DATA.HMI_REC.FLAG=true;//数据帧完整，提醒主程序
		  UART_DATA.HMI_REC.COUNTER=0;//准备下次接收
		}
	}
  } 
  USART_ClearITPendingBit(USART1, USART_IT_RXNE);
}
//=============================================================================
//函数名称:USART2_IRQHandler
//功能概要:
//参数说明:无
//函数返回:无
//=============================================================================
void USART2_IRQHandler(void)//未使用
{
  u8 c;
  if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
  { 	
    c=USART2->DR;
    printf("%c",c);    //将接受到的数据直接返回打印
  } 
  
  USART_ClearITPendingBit(USART2, USART_IT_RXNE);
}

//=============================================================================
//函数名称:USART3_IRQHandler
//功能概要:
//参数说明:无
//函数返回:无
//=============================================================================
void USART3_IRQHandler(void)//红外ccd_LV4//指针+2为校验和的位置（亦或校验）
{
  if(USART_GetITStatus(USART3,USART_IT_RXNE) != RESET)
  { 	
	UART_DATA.BAR_REC.DATA[UART_DATA.BAR_REC.COUNTER++]= USART_ReceiveData(USART3);
	if((UART_DATA.BAR_REC.DATA[UART_DATA.BAR_REC.COUNTER-1]=='\n') && (UART_DATA.BAR_REC.COUNTER==ORDER_LEN+1))//检测停止位（cr \r 0x0d （lf \n 0x0a） \r回到行首 \n下一行	
	{
	  UART_DATA.BAR_REC.FLAG=true;//接受完一帧数据，置高标志位
	  UART_DATA.BAR_REC.COUNTER=0;//清空字符串计数器
	}
	if(UART_DATA.BAR_REC.DATA[UART_DATA.BAR_REC.COUNTER-2]==0x03)//说明接收到停止符号
	{
	  UART_DATA.BAR_REC.FLAG=true;//主要用于开机自检
	  //UART_DATA.BAR_REC.COUNTER=0;//清空字符串计数器
	}
	if(UART_DATA.BAR_REC.COUNTER>=30)//2016-05-31,防止指针溢出
		UART_DATA.BAR_REC.COUNTER=0;
  } 	
  USART_ClearITPendingBit(USART3, USART_IT_RXNE);
}
//=============================================================================
//函数名称:UART4_IRQHandler
//功能概要:
//参数说明:无
//函数返回:无
//=============================================================================
void UART4_IRQHandler(void)//SYN6288
{
  if(USART_GetITStatus(UART4,USART_IT_RXNE) != RESET)
  { //每次回传的数据都仅是一个字节大小
	UART_DATA.SYN_REC.DATA[UART_DATA.SYN_REC.COUNTER]= USART_ReceiveData(UART4);
	if((UART_DATA.SYN_REC.DATA[UART_DATA.SYN_REC.COUNTER] & 0X40) == 0X40)
	{//说明数据的格式是正确的
		UART_DATA.SYN_REC.FLAG = true;
	}
	if(UART_DATA.SIM_REC.COUNTER>=30)//2016-05-31,防止指针溢出
		UART_DATA.SIM_REC.COUNTER=0;
  } 
  //
  USART_ClearITPendingBit(UART4, USART_IT_RXNE);
}
//=============================================================================
//函数名称:UART5_IRQHandler
//功能概要:
//参数说明:无
//函数返回:无
//=============================================================================
void UART5_IRQHandler(void)//SIM900A
{
  //static u8 FLAG_VALUE_COUNTER = BIT1;//静态局部变量，每次调用都继续使用上一次的数值，特征值计数器，用来记录\r\n的个数
  if(USART_GetITStatus(UART5,USART_IT_RXNE) != RESET)
  { 	
	UART_DATA.SIM_REC.DATA[UART_DATA.SIM_REC.COUNTER++] = USART_ReceiveData(UART5);//到COUNTER的位置如果能够查询到三个\r\n说明数据接收完毕
	if((UART_DATA.SIM_REC.DATA[UART_DATA.SIM_REC.COUNTER-2]=='\r')&&(UART_DATA.SIM_REC.DATA[UART_DATA.SIM_REC.COUNTER-1]=='\n'))
	{
		UART_DATA.SIM_REC.FLAG = true;//接受到一个换行字符
		//UART_DATA.SIM_REC.COUNTER = 0;
	}
	if(UART_DATA.SIM_REC.COUNTER>=30)//2016-05-31,防止指针溢出
		UART_DATA.SIM_REC.COUNTER=0;
	
	  /*if(UART_DATA.SIM_REC.COUNTER>=2)//当接收到完第二字节数据后
	{
		if((UART_DATA.SIM_REC.DATA[0]=='A')&&(UART_DATA.SIM_REC.DATA[1]=='T'))//说明帧头对齐，完成一帧数据的接受
		{
			if((UART_DATA.SIM_REC.DATA[UART_DATA.SIM_REC.COUNTER-2]=='\r')&&(UART_DATA.SIM_REC.DATA[UART_DATA.SIM_REC.COUNTER-1]=='\n'))
			{
				FLAG_VALUE_COUNTER <<= 1;
				if(FLAG_VALUE_COUNTER == BIT4)//说明出现了三对\r\n,完整的数据帧
				{
					UART_DATA.SIM_REC.FLAG = true;//接受完一帧数据，置高标志位 
					FLAG_VALUE_COUNTER = BIT1;
					UART_DATA.SIM_REC.COUNTER = 0;
				}
			}
		}
		else//帧头出错
		  UART_DATA.SIM_REC.COUNTER = 0;//清空计数指针，重新接受
	}*/

	/*if((UART_DATA.SIM_REC.DATA[UART_DATA.SIM_REC.COUNTER-1]=='\n')&&(UART_DATA.SIM_REC.COUNTER >= 2))//不能和第一个\n冲突
	  //检测停止位（cr \r 0x0d （lf \n 0x0a） \r回到行首 \n下一行	
	{
	  if((UART_DATA.SIM_REC.DATA[0] == '\r') && (UART_DATA.SIM_REC.DATA[1] == '\n'))
	  	UART_DATA.SIM_REC.FLAG=true;//接受完一帧数据，置高标志位 
	  UART_DATA.SIM_REC.COUNTER = 0;
	}*/
  } 
  USART_ClearITPendingBit(UART5, USART_IT_RXNE);	
}






















