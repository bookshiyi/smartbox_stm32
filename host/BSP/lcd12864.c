/********************   One Artisan's Regret And Pride   ***********************
*
*	@File    	： lcd12864.c
*	@Brief   	： LCD12864的底层驱动文件
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

u8 CGROM_Code[] = {
					//天线图形	
					0xff,0xff,0x80,0x01,0xbf,0xfd,0xdf,0xfb,0xec,0x37,0xf6,0x6f,0xfb,0xdf,0xfd,0xbf,
					0xfe,0x7f,0xfe,0x7f,0xfe,0x7f,0xfe,0x7f,0xfe,0x7f,0xfe,0x7f,0xfe,0x7f,0xff,0xff,
					//摄氏图形
//						0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x70,0x14,0xd8,0x1d,0x88,0x01,0x80,0x01,0x80,
//						0x01,0x80,0x01,0x80,0x01,0x88,0x00,0xc8,0x00,0x70,0x00,0x00,0x00,0x00,0x00,0x00,   
					//蓝牙图形
					0x07,0xe0,0x0e,0x70,0x1e,0xb8,0x3e,0xdc,0x36,0xec,0x3a,0xdc,0x3c,0xbc,0x3e,0x7c,
					0x3e,0x7c,0x3c,0xbc,0x3a,0xdc,0x36,0xec,0x3e,0xdc,0x1e,0xb8,0x0e,0x70,0x07,0xe0,
					//CD图形
					0xff,0xff,0xff,0xff,0xff,0xff,0xe3,0x07,0xc9,0xb3,0x9d,0xb9,0x9f,0xb9,0x9f,0xb9,
					0x9f,0xb9,0x9f,0xb9,0x9d,0xb9,0xc9,0xb3,0xe3,0x07,0xff,0xff,0xff,0xff,0xff,0xff,
					//右指向手型
					0x00,0x00,0x1e,0x00,0x12,0x00,0x73,0xfc,0x52,0x02,0x52,0x1c,0x53,0xe0,0x52,0x10,
					0x53,0xe0,0x52,0x10,0x53,0xe0,0x52,0x10,0x73,0xe0,0x12,0x00,0x1e,0x00,0x00,0x00			
					};
/****************************************************************************
*	@Function name	： GPIO测试
*	@Description    ： 测试GPIO位操作是否成功，需要用逻辑分析仪或示波器看引脚电平变化
*	@Author  		： Blade@oarap.org
*	@Date			： 2016-05
*	@param    		： 无
*	@return			： 无
*****************************************************************************/
void LCD12864_GPIO_TEST()
{
	while(1)
	{
		LCD12864_CS_H;
		Delay_ms(30);
		LCD12864_SDA_H;
		Delay_ms(30);
		LCD12864_SCL_H;
		Delay_ms(30);
		LCD12864_CS_L;
		Delay_ms(30);
		LCD12864_SDA_L;
		Delay_ms(30);
		LCD12864_SCL_L;
		Delay_ms(30);
	}
}
/****************************************************************************
*	@Function name	： LCD12864GPIO初始化
*	@Description    ： 初始化串行状态下LCD12864的GPIO
*	@Author  		： Blade@oarap.org
*	@Date			： 2016-05
*	@param    		： 无
*	@return			： 无
*****************************************************************************/
void LCD12864_GPIO_Config(void) //GPIOB 5 8 9
{	
    //定义一个GPIO_InitTypeDef 类型的结构体
    GPIO_InitTypeDef  GPIO_InitStructure;	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能GPIO的外设时钟

    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_5 | GPIO_Pin_8 | GPIO_Pin_9;//选择要用的GPIO引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //设置引脚模式为推免输出模式						 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//设置引脚速度为50MHZ         
    GPIO_Init(GPIOB, &GPIO_InitStructure);//调用库函数，初始化GPIO
}
/****************************************************************************
*	@Function name	： LCD启动字节
*	@Description    ： 首先传送一个启动字节，送入连续5个“1”用来启
*               		动一个周期，此时传输计数被重置，并且串行传输
*               		被同步，紧接着的两个位指定传输方向（R/W，确
*               		定是读还是写）和传输性质（RS，确定是命令寄存
*               		器还是数据寄存器），最后的第八位是一个“0”
*	@Author  		： Blade@oarap.org
*	@Date			： 2016-05
*	@param  dt		： 传输的字节，高5位在函数内部设定为1只是确定RW和RS为，最后那位也是内部确定
*	@return			： 无
*****************************************************************************/
void LCD12864_Start(u8 dt)
{
	u8 dat,j;

	dat = dt | 0xf8;	//高5位设定为“1”，第八位设定为“0”
	LCD12864_SCL_L;
	Delay_us(LCD12864_Delay);
	LCD12864_CS_H;	//选中显示屏，高电平有效
	for(j = 0;j < 8;j++)
	{
		if(dat & 0x80)	
		{
		  LCD12864_SDA_H;	//放数据到数据线
		}
		else	
		  LCD12864_SDA_L;
			
		Delay_us(LCD12864_Delay);
		LCD12864_SCL_H;	//发送数据，上升沿有效
		Delay_us(LCD12864_Delay);
		LCD12864_SCL_L;
		dat <<= 1;	//左移一位，先发的是高位
	}
	Delay_us(LCD12864_Delay);
	LCD12864_SCL_L;
	Delay_us(LCD12864_Delay);	//等待硬件反应（等待数据发送完全）	
}
/****************************************************************************
*	@Function name	： 人机界面初始化
*	@Description    ： LCD12864写入一个字节命令或者数据
*	@Author  		： Blade@oarap.org
*	@Date			： 2016-05
*	@param  dat		： 要写入的命令或者数据
*	@param  cmd		： 命令或者数据判断为。0为命令，1为数据
*	@return			： 无
*****************************************************************************/
void LCD12864_Write_Byte(u8 dat,u8 cmd)
{
	u8 tem;
	u8 i,j;

	tem = dat & 0xf0;	//先发送高4位
	LCD12864_Start((cmd << 1) | 0xf0);	//置RW为“0”、RS为“ord”并启动串行传输为数据格式
	for(j = 0;j < 2;j++)	//一个字节数据或者命令分两次发送
	{
		LCD12864_SCL_L;	//允许数据线电平变化
		Delay_us(LCD12864_Delay);
		for(i = 0;i < 8;i++)
		{
			if(tem & 0x80)	
			{
			  LCD12864_SDA_H;	//放上数据，屏蔽低4位
			}
			else	LCD12864_SDA_L;	

			Delay_us(LCD12864_Delay);
			LCD12864_SCL_H;	//发送数据，上升沿有效
			Delay_us(LCD12864_Delay);
			LCD12864_SCL_L;
			tem <<= 1;	//左移一位，先发的是高位
		}
		tem = (dat << 4) & 0xf0;	//发完高4位，再次发送低4位
	}
	Delay_us(LCD12864_Delay);
	LCD12864_SDA_L;
	Delay_us(LCD12864_Delay);
	LCD12864_SCL_L;
	Delay_us(LCD12864_Delay);
	LCD12864_CS_L;
	Delay_us(LCD12864_Delay);//等待硬件反应（等待数据发送完全）
}

/****************************************************************************
*	@Function name	： 设置LCD显示位置
*	@Description    ： 向LCD12864写入接下来要显示的开始位置
*	@Author  		： Blade@oarap.org
*	@Date			： 2016-05
*	@param  	x	： 行，1-4
*	@param  	y	： 列  0-7
*	@return			： 无
*****************************************************************************/
void LCD12864_Set_xy(u8 x,u8 y)
{
	switch(x)
	{
		case 1:
					LCD12864_Write_Byte((One_LineAddress + y),0);	//写入操作地址
					break;
		case 2:
					LCD12864_Write_Byte((Two_LineAddress + y),0);	//写入操作地址
					break;
		case 3:
					LCD12864_Write_Byte((Three_LineAddress + y),0);	//写入操作地址
					break;
		case 4:
					LCD12864_Write_Byte((Four_LineAddress + y),0);	//写入操作地址
					break;
		default:
					LCD12864_Write_Byte((One_LineAddress + y),0);	//写入操作地址
					break;
	}
}
/****************************************************************************
*	@Function name	： 写入自定义字符到LCD的CGRAM
*	@Description    ： 
*	@Author  		： Blade@oarap.org
*	@Date			： 2016-05
*	@param *data_code： 写入的数组
*	@return			： 无
*****************************************************************************/
void LCD12864_Write_CGRAM(u8 *data_code)
{
	u8 i;
	LCD12864_Write_Byte(0x34,0);	//打开字符扩展指令
	LCD12864_Write_Byte(0x02,0);	//SR=0，允许输入
	LCD12864_Write_Byte(0x30,0);	//恢复基本指令
	LCD12864_Write_Byte(0x40,0);	//CGRAM地址
	for(i = 0;i < CGRAM_Value;i++)
	{
		LCD12864_Write_Byte(data_code[i*2],1);
		LCD12864_Write_Byte(data_code[i*2+1],1);
	}
}
/****************************************************************************
*	@Function name	： 显示一个CGRAM内容
*	@Description    ： 
*	@Author  		： Blade@oarap.org
*	@Date			： 2016-05
*	@param	x		：行
*	@param	y		：列
*	@param	*str	：为要显示的字符串
*	@param	add_h	：CGRAM高位地址
*	@param	add_l	：CGRAM低位地址
*	@return			： 无
*****************************************************************************/
void Display_CGRAM(u8 x,u8 y,u8 add_h,u8 add_l)
{
	LCD12864_Set_xy(x,y);	//设置显示地址
	LCD12864_Write_Byte(add_h,1);
	LCD12864_Write_Byte(add_l,1);
}
/****************************************************************************
*	@Function name	： LCD12864显示字符串
*	@Description    ： 从数组的开头开始显示len长度字符串
*	@Author  		： Blade@oarap.org
*	@Date			： 2016-05
*	@param	x		：行
*	@param	y		：列
*	@param	*str	：要显示的字符串
*	@param	len		：显示的长度
*	@return			： 无
*****************************************************************************/
void LCD12864_Display_String(u8 x,u8 y,u8 *str,u8 len)
{
	LCD12864_Set_xy(x,y);	//设置显示地址
	while(len--)
	{
		LCD12864_Write_Byte(*str,1);
		str++;
	}
}
/****************************************************************************
*	@Function name	： LCD12864显示字符串
*	@Description    ： 从数组的指定位置开始显示len长度字符串
*	@Author  		： Blade@oarap.org
*	@Date			： 2016-05
*	@param	x		：行
*	@param	y		：列
*	@param	startnum：开始显示的位置
*	@param	*str	：要显示的字符串
*	@param	len		：显示的长度
*	@return			： 无
*****************************************************************************/
void LCD12864_Display_String_Assign(u8 x,u8 y,u8 *str,u8 starsum,u8 len)
{
	u8 i;

	LCD12864_Set_xy(x,y);	//设置显示地址

	for(i = starsum;i < starsum + len;i++)
	{
		LCD12864_Write_Byte(str[i],1);
	}
}
/****************************************************************************
*	@Function name	： LCD写入GDRAM绘图
*	@Description    ： 
*	@Author  		： Blade@oarap.org
*	@Date			： 2016-05
*	@param	x		：行
*	@param	y		：列
*	@param	width	：显示图片的宽度
*	@param	hieght	：显示图片的高度
*	@param	*bmp	：要显示的字符串（位图）
*	@param	dis_mode：显示模式，是半屏显示还是全屏显示
*                         0：半屏显示。1：全屏显示
*	@return			： 无
*****************************************************************************/
void LCD12864_Display_GDRAM(u8 x,u8 y,u8 width,u8 height,u8 *bmp,u8 dis_mode)
{
	u8 i,j,k;
	u8 base_x,base_y;	//起始坐标

	/******全屏绘图显示******/
	if(dis_mode)
	{
		switch(x)
		{
			case 1:
					base_y = One_LineAddress + y;
					break;
			case 2:
					base_y = Two_LineAddress + y;
					break;
			case 3:
					base_y = Three_LineAddress + y;
					break;
			case 4:
					base_y = Four_LineAddress + y;
					break;
			default:	break;
		}
		 
	 	LCD12864_Write_Byte(MPU_8bit_Expansion_Draw_Close,0);	//扩充指令
		LCD12864_Write_Byte(MPU_8bit_Expansion_Draw_Open,0);	//打开绘图功能
		
		for(j = 0;j < height;j++)	//32
		{
			LCD12864_Write_Byte(base_y + j,0);	//写入行号，即第几行开始
			LCD12864_Write_Byte(One_LineAddress,0);	//横坐标的第几个字节开始写
			
			for(i = 0;i < width;i++)	//上半屏
			{
				LCD12864_Write_Byte(bmp[width * j + i],1);	//开始写入数据
			}
			for(k = 0;k < width;k++)	//下半屏
			{
			 	LCD12864_Write_Byte(bmp[width * (j + height) + k],1);	//开始写入数据
			}
		} 		
	}
	/******分开上半屏或者下半屏写******/
	else
	{
		switch(x)	//判断显示的横坐标，确定起始行
		{
			case 1:	//上半屏
					base_x = One_LineAddress;	//起始地址为0x80
					break;
	
			case 2:	//下半屏
					base_x = One_LineAddress;	//起始地址为0x80
					break;
	
			case 3:	//下半屏
					base_x = Three_LineAddress;	//起始地址为0x88
					break;
			default:
					break;
		}
		
		base_y = base_x + y;	//x轴方向的偏移，基地址 + 偏移地址
		
		LCD12864_Write_Byte(MPU_8bit_Expansion_Draw_Close,0);	//扩充指令
		LCD12864_Write_Byte(MPU_8bit_Expansion_Draw_Open,0);	//打开绘图功能
		
		if(x == 1 || x == 3)	//直接在同一个半屏的，直接写完32位数据
		{
			for(i = 0;i < 32;i++)	//写地址有讲究，先写垂直再写水平，详见“设定绘图RAM地址”说明
			{ 
				LCD12864_Write_Byte(One_LineAddress + i,0);	//写入垂直地址，不管上半屏还是下半屏
				LCD12864_Write_Byte(base_y,0);	//写入水平地址
				
				for(j = 0;j < 2;j++)	//两组8bit数据，组成16bit
				{
					LCD12864_Write_Byte(*bmp++,1);	//写入数据
				}
			}
		}
		
		if(x == 2)	//数据显示不在同一半屏上的，先写上半屏，在写下半屏
		{
			for(i = 0;i < 16;i++)	//上半屏16行点阵数据
			{
				LCD12864_Write_Byte(Two_LineAddress + i,0);	//写入垂直地址
				LCD12864_Write_Byte(base_y,0);	//写入水平地址
				
				for(j = 0;j < 2;j++)	//两组8bit数据，组成16bit
				{
					LCD12864_Write_Byte(*bmp++,1);	//写入数据
				}
			}
			for(i = 0;i < 16;i++)	//下半屏16行点阵数据
			{
				LCD12864_Write_Byte(Three_LineAddress + i,0);	//写入垂直地址
				LCD12864_Write_Byte(base_y + 8,0);	//写入水平地址
				
				for(j = 0;j < 2;j++)	//两组8bit数据，组成16bit
				{
					LCD12864_Write_Byte(*bmp++,1);	//写入数据
				}
			}
		}	  

	}
	LCD12864_Write_Byte(MPU_8bit_Expansion_Draw_Open,0);	//扩充指令，打开绘图功能
//	LCD12864_Write_Byte(Instruct_Basic8,0);
}	
/****************************************************************************
*	@Function name	： LCD12864清屏
*	@Description    ： 
*	@Author  		： Blade@oarap.org
*	@Date			： 2016-05
*	@param			： 无
*	@return			： 无
*****************************************************************************/
void LCD12864_Clear(void)
{
	LCD12864_Write_Byte(Clear_Display,0);	//清屏
}

/****************************************************************************
*	@Function name	： LCD12864初始化
*	@Description    ： 显示的预处理，显示基本的开机界面
*	@Author  		： Blade@oarap.org
*	@Date			： 2016-05
*	@param			： 无
*	@return			： 无
*****************************************************************************/
void LCD12864_Init(void)
{
	u8 welcome []="----欢迎使用----";
	u8 smartbox[]="--智能送餐系统--";	
	u8 loading[]="loading";
	  
	LCD12864_Write_Byte(Instruct_Basic8,0);	//基本指令，8bit模式,0x30
	LCD12864_Write_Byte(Open_Display,0);	//整体显示开，光标关，显示反白关,0x0c
	LCD12864_Write_Byte(Clear_Display,0);	//清屏,0x01
	LCD12864_Write_Byte(Vernier_ShiftRight,0);	//设定起点，光标右移,0x06

	//LCD12864_Write_CGRAM(CGROM_Code);	//写入自定义字符
	
  LCD12864_Display_String(1,0,welcome,16);
  LCD12864_Display_String(3,0,smartbox,16);
  LCD12864_Display_String(4,0,loading,7);
}
/*********************************************************/

