/**********************************************************
* @ File name -> lcd12864.h
* @ Version   -> V1.1.2
* @ Date      -> 11-30-2013
* @ Brief     -> LCD12864驱动函数头文件
*                适用于ST7920驱动芯片

 V1.1
* @ Revise    -> A、修正操作命令宏定义。
* @           -> B、增加串口、并口转换宏定义，增加对高速度MCU控制通讯频率延时函数
* @           -> C、显示字符串函数增加显示长度选择，即显示多少个ASCII可显示字符，汉字x2即可
* @           -> D、去掉显示数组函数，合并在字符串显示里

 V1.1.1
* @ Revise    -> 增加字符串显示函数，可以从要显示的数组中任意位置显示一定长度字符串 

 V1.1.2
* @ Revise    -> A、增加对低速晶振系统的MCU通讯频率是否使用的宏定义
* @           -> B、增加在检测液晶忙超时退出，预防死循环
**********************************************************/

#ifndef _LCD12864_H_ 
#define _LCD12864_H_


/**********************************************************
                        参数宏定义                       
**********************************************************/

#define LCD_Busy				0x80	//LCD忙标志

#define Graph_BassADD			0x00	//显示GRAM基地址

//CGRAM显示地址单元
#define ANT_Graph				0x00
//#define Temperature_Graph		0x02
#define Bluetooth_Graph			0x02
#define CD_Graph				0x04
#define Hand_Graph				0x06

//////////////////////////////////////////////////////
//LCD12864指令集
//////////////////////////////////////////////////////
#define One_LineAddress			0x80	//第一行基地址
#define Two_LineAddress			0x90	//第二行基地址
#define Three_LineAddress		0x88	//第三行基地址
#define Four_LineAddress		0x98	//第四行基地址

//基本指令宏定义
#define MPU_Busy				0x80	//LCD忙，同时也读出了地址计数器（AC）数值
#define Clear_Display			0x01	//清屏指令
#define Zero_Address			0x02	//显示地址归零，AC = 0

//扩充指令宏定义
#define LCD_Standby				0x01	//待机指令，先设置RE = 1（功能设定寄存器），在送此命令即进入待机模式
                                        //退出待机，写入任意指令可以退出，再将RE = 0即可

/***********基本指令***********/

//进入设定点（entry mode set）
// b7  b6  b5  b4  b3  b2  b1  b0
// 0   0   0   0   0   1   I/D  S 
#define Entry_ModeSet			0x04	//基数

#define Vernier_ShiftRight		(Entry_ModeSet | (1<<1))	//游标右移，AC+1
#define Vernier_ShiftLeft		(Entry_ModeSet | (0<<1))	//游标右移，AC-1

#define Frames_ShiftRight		(Vernier_ShiftLeft | 0x01)	//画面整体右移
#define Frames_ShiftLeft		(Vernier_ShiftRight | 0x01)	//画面整体左移

//显示开关设置（display status）
// b7  b6  b5  b4  b3  b2  b1  b0
// 0   0   0   0   1   D   C   B 
#define Display_Status			0x08	//基数

#define Open_Display			(Display_Status | (1<<2))	//整体显示开
#define Close_Display			(Display_Status | (0<<2))	//整体显示关

#define Open_Vernier			(Display_Status | (1<<1))	//游标显示开
#define Close_Vernier			(Display_Status | (0<<1))	//游标显示关

#define Vernier_White_Open		(Display_Status | 0x01)	//游标位置显示反白开
#define Vernier_White_Close		(Display_Status | 0x00)	//游标位置显示反白关，正常显示

//游标或显示移位控制（cursor and display shift control）
// b7  b6  b5  b4  b3    b2   b1  b0
// 0   0   0   1   S/C  R/L1  X   X 
#define Cursor_Display_Control	0x10	//基数

#define Cursor_ShiftLeft		(Cursor_Display_Control | (0<<2))	//游标向左移动，AC = AC - 1
#define Cursor_ShiftRight		(Cursor_Display_Control | (1<<2))	//游标向右移动，AC = AC + 1
#define Display_ShiftLeft		(Cursor_Display_Control | (2<<2))	//显示向左移动，游标跟着移动，AC = AC
#define Display_ShiftRight		(Cursor_Display_Control | (3<<2))	//显示向右移动，游标跟着移动，AC = AC  

//功能设定（function set）
// b7  b6  b5  b4  b3  b2    b1  b0
// 0   0   1   DL  X   0/RE  X 	 X
#define Function_SetBasic		0x20	//基数

#define MPU_8bit				(Function_SetBasic | (1<<4))	//8bit MPU接口
#define MPU_4bit				(Function_SetBasic | (0<<4))	//4bit MPU接口

#define Instruct_Basic4			(MPU_4bit | (0<<2))				//4bit MPU接口，基本指令集
#define Instruct_Basic8			(MPU_8bit | (0<<2))				//8bit MPU接口，基本指令集

#define Instruct_Expansion4		(MPU_4bit | (1<<2))				//4bit MPU接口，扩充指令集
#define Instruct_Expansion8		(MPU_8bit | (1<<2))				//8bit MPU接口，扩充指令集

//设定CGRAM地址
// b7  b6  b5  b4  b3  b2  b1  b0
// 0   1   A5  A4  A3  A2  A1  A0
#define CGRAM_Set				0x40	//基数
//设定CGRAM地址到地址计数器（AC），AC范围为0x00 ~ 0x3f，需确认扩充指令中SR = 0（卷动位置或RAM地址选择）

//设定DDRAM地址
// b7  b6  b5  b4  b3  b2  b1  b0
// 1   0   A5  A4  A3  A2  A1  A0
#define DDRAM_Set				0x80	//基数
//设定CGRAM地址到地址计数器（AC）

//写显示数据到RAM（RS = 1 RW = 0）
// b7  b6  b5  b4  b3  b2  b1  b0
// D7  D6  D5  D4  D3  D2  D1  D0
//写入数据后会使AC改变，每个RAM（CGRAM、DDRAM、IRAM）地址都可以连续写入2字节的显示数据，
//当写入第二个字节时，地址计数器（AC）的值自动加1

//读取显示数据到RAM（RS = 1 RW = 1）
// b7  b6  b5  b4  b3  b2  b1  b0
// D7  D6  D5  D4  D3  D2  D1  D0
//读取数据后会使AC改变，设定RAM（CGRAM、DDRAM、IRAM）地址后，先要Dummy read一次后才能读取到正确的显示数据，
//第二次读取不需要Dummy read，除非重新设定了RAM地址

/***********扩充指令***********/

//卷动位置或RAM地址选择
// b7  b6  b5  b4  b3  b2  b1  b0
// 0   0   0   0   0   0   1   SR
#define Write_Roll_Address_Allow		0x03	//允许输入垂直卷动地址
#define Write_IRAM_Address_Allow		0x02	//允许输入IRAM地址（扩充指令），及允许设定CGRAM地址（基本指令）

//反白显示
// b7  b6  b5  b4  b3  b2  b1  b0
// 0   0   0   0   0   1   0   R0
#define Display_White_One				0x04	//第一行反白显示
#define Display_White_Two				0x05	//第二行反白显示

//睡眠模式
// b7  b6  b5  b4  b3  b2  b1  b0
// 0   0   0   0   1   SL  0   0
#define LCD_Sleep_Open					(0x08 | (1<<2))	//进入睡眠模式
#define LCD_Sleep_Close					(0x08 | (0<<2))	//退出睡眠模式

//扩充功能设定
// b7  b6  b5  b4  b3  b2  b1  b0
// 0   0   1   DL  X   RE  G   X
#define MPU_4bit_Basic_Draw_Open		(Instruct_Basic4 | (1<<1))	//4bit MPU接口，基本指令，绘图开
#define MPU_4bit_Basic_Draw_Close		(Instruct_Basic4 | (0<<1))	//4bit MPU接口，基本指令，绘图关

#define MPU_8bit_Basic_Draw_Open		(Instruct_Basic8 | (1<<1))	//8bit MPU接口，基本指令，绘图开
#define MPU_8bit_Basic_Draw_Close		(Instruct_Basic8 | (0<<1))	//8bit MPU接口，基本指令，绘图关

#define MPU_8bit_Expansion_Draw_Open	(Instruct_Expansion8 | (1<<1))	//8bit MPU接口，扩充指令，绘图开
#define MPU_8bit_Expansion_Draw_Close	(Instruct_Expansion8 | (0<<1))	//8bit MPU接口，扩充指令，绘图关

#define MPU_4bit_Expansion_Draw_Open	(Instruct_Expansion4 | (1<<1))	//4bit MPU接口，扩充指令，绘图开
#define MPU_4bit_Expansion_Draw_Close	(Instruct_Expansion4 | (0<<1))	//4bit MPU接口，扩充指令，绘图关

//设定IRAM地址或卷动地址
// b7  b6  b5  b4  b3  b2  b1  b0
// 0   1   A5  A4  A3  A2  A1  A0
#define IRAM_Set						0x40	//基数
//SR = 1，A5 ~ A0为垂直卷动地址，SR = 0，A3 ~ A0为IRAM地址

//设定绘图RAM地址
// b7  b6  b5  b4  b3  b2  b1  b0
// 1   0   0   0   A3  A2  A1  A0 --->水平地址范围：A3 ~ A0
// 1   A6  A5  A4  A3  A2  A1  A0 --->垂直地址范围：A6 ~ A0
#define Draw_RAMSet						0x80	//基数
//设定GDRAM地址到地址计数器（AC），先设置垂直位置再设置水平位置（连续写入2字节数据来完成垂直与水平坐标的设置）。


   








//PB5 - CS 
//PB7 - SDA 
//PB9 - SCL 
#define LCD12864_CS_H    GPIOB->ODR |=  BIT5;
#define LCD12864_CS_L    GPIOB->ODR &= ~BIT5;
#define LCD12864_SDA_H   GPIOB->ODR |=  BIT8;
#define LCD12864_SDA_L   GPIOB->ODR &= ~BIT8;
#define LCD12864_SCL_H   GPIOB->ODR |=  BIT9;
#define LCD12864_SCL_L   GPIOB->ODR &= ~BIT9;

#define LCD12864_Delay 20	//延时时间长度
#define CGRAM_Value   64	//写入CGRAM数量 

/**********************************************************
                      外部功能函数                      
**********************************************************/

void LCD12864_GPIO_Config(void);//GPIO初始化

void LCD12864_GPIO_TEST(void);//引脚控制测试

void LCD12864_Init(void);	//LCD YB12864ZB初始化

void LCD12864_Clear(void);	//LCD YB12864ZB清屏

void LCD12864_Write_Byte(u8 dat,u8 cmd);	//LCD写入一个字节命令或者数据，判断忙标志

void LCD12864_Display_String(u8 x,u8 y,u8 *str,u8 len);	//LCD显示字符串

void LCD12864_Display_String_Assign(u8 x,u8 y,u8 *str,u8 starsum,u8 len);	//LCD显示字符串 指定位置长度

void LCD12864_Display_CGRAM(u8 x,u8 y,u8 add_h,u8 add_l);	//显示一个CGRAM内容

void LCD12864_Display_GDRAM(u8 x,u8 y,u8 width,u8 height,u8 *bmp,u8 dis_mode);	//LCD写入GDRAM绘图



#endif


/*********************************************************/

/*

 1、指令表1（RE = 0，基本指令集）
  ___________________________________________________________________________
 |               |  控制信号  |              控 制 代 码              | 执行 |
 |               |____________|_______________________________________| 	 |
 |   指令名称    | RS | R/W   | D7 | D6 | D5 | D4 | D3 | D2 | D1 | D0 | 时间 |     HEX
 |_______________|____|_______|____|____|____|____|____|____|____|____|______|
 |   清除显示    | 0  |   0   | 0  | 0  | 0  | 0  | 0  | 0  | 0  | 1  | 1.6ms|     0x01
 |_______________|____|_______|____|____|____|____|____|____|____|____|______|
 |   地址归0     | 0  |   0   | 0  | 0  | 0  | 0  | 0  | 0  | 1  | X  | 72us |     0x02 //不改变DDRAM内容
 |_______________|____|_______|____|____|____|____|____|____|____|____|______|
 |  进入设定点   | 0  |   0   | 0  | 0  | 0  | 0  | 0  | 1  | I/D| S  | 72us |     0x06 游标右移    0x07 画面整体左移
 |_______________|____|_______|____|____|____|____|____|____|____|____|______|     0x04 游标左移    0x05 画面整体右移
 | 显示开关设置  | 0  |   0   | 0  | 0  | 0  | 0  | 1  | D  | C  | S  | 72us |   D = 1; 整体显示开  D = 0; 整体显示关 // 不改变DDRAM内容     
 |_______________|____|_______|____|____|____|____|____|____|____|____|______|   C = 1; 游标显示开  C = 0; 游标显示关  S = 1; 游标位置显示反白 S = 0; 正常显示     
 |   移位控制    | 0  |   0   | 0  | 0  | 0  | 1  | S/C| R/L| X  | X  | 72us |     0x10 游标向左移动 AC = AC - 1   0x14 游标向右移动 AC = AC + 1
 |_______________|____|_______|____|____|____|____|____|____|____|____|______|     0x18 显示向左移动 AC = AC       0x1c 显示向右移动 AC = AC
 |   功能设定    | 0  |   0   | 0  | 0  | 1  | DL | X  |0/RE| X  | X  | 72us |     0x30 8bit接口，基本指令   0x34 8bit接口，扩充指令
 |_______________|____|_______|____|____|____|____|____|____|____|____|______|     0x20 4bit接口，基本指令   0x24 4bit接口，扩充指令 
 | 设定CGRAM地址 | 0  |   0   | 0  | 1  | A5 | A4 | A3 | A2 | A1 | A0 | 72us |
 |_______________|____|_______|____|____|____|____|____|____|____|____|______| 设定CGRAM地址到地址计数器（AC），AC范围0x00 ~ 0x3f，需确认扩充指令中SR = 0
 | 设定DDRAM地址 | 0  |   0   | 1  | 0  | A5 | A4 | A3 | A2 | A1 | A0 | 72us |
 |_______________|____|_______|____|____|____|____|____|____|____|____|______| 设定DDRAM地址到地址计数器（AC），第一行AC范围 0x80 ~ 0x8f    第二行AC范围 0x90 ~ 0x9f 
 | 读忙标志和地址| 0  |   1   | BF | A6 | A5 | A4 | A3 | A2 | A1 | A0 | 72us |
 |_______________|____|_______|____|____|____|____|____|____|____|____|______|     BF = 0; //可以接受新数据
 |  写显示数据   | 1  |   0   |                显示数据               | 72us |
 |_______________|____|_______|_______________________________________|______| 每个RAM（CGRAM、DDRAM、IRAM）地址都可以连续写入2个字节，第二个字节时AC自动增加1
 |  读显示数据   | 1  |   1   |                显示数据               | 72us |
 |_______________|____|_______|_______________________________________|______| 设定RAM（CGRAM、DDRAM、IRAM）地址后，先要Dummy read（假读）一次后才能读到正确的显示
                                                                               数据，第二次读取不需要再Dummy read（假读）一次，除非重新设置了RAM地址


 
 2、指令表2（RE = 1，扩充指令集）
  ___________________________________________________________________________
 |               |  控制信号  |              控 制 代 码              | 执行 |
 |               |____________|_______________________________________| 	 |
 |   指令名称    | RS | R/W   | D7 | D6 | D5 | D4 | D3 | D2 | D1 | D0 | 时间 |     HEX
 |_______________|____|_______|____|____|____|____|____|____|____|____|______|
 |   待命模式    | 0  |   0   | 0  | 0  | 0  | 0  | 0  | 0  | 0  | 1  | 72us |     0x01   该指令不能改变RAM内容
 |_______________|____|_______|____|____|____|____|____|____|____|____|______|
 |   卷动地址或  | 0  |   0   | 0  | 0  | 0  | 0  | 0  | 0  | 1  | SR | 72us |
 |  RAM地址选择  |    |       |    |    |    |    |    |    |    |    |      | 初始值0x02， 0x03 允许输入垂直卷动地址
 |_______________|____|_______|____|____|____|____|____|____|____|____|______|              0x02 允许输入IRAM地址（扩充指令），及允许设定CGRAM地址（基本指令）
 |   反白显示    | 0  |   0   | 0  | 0  | 0  | 0  | 0  | 1  | R1 | R0 | 72us |
 |_______________|____|_______|____|____|____|____|____|____|____|____|______| 初始值0x04   0x05反白显示
 |   睡眠模式    | 0  |   0   | 0  | 0  | 0  | 0  | 1  | SL | X  | X  | 72us |
 |_______________|____|_______|____|____|____|____|____|____|____|____|______|     0x0c 脱离睡眠模式    0x08 进入睡眠模式
 |  扩充功能设定 | 0  |   0   | 0  | 0  | 1  | DL | X  |1/RE| G  | 0  | 72us |
 |_______________|____|_______|____|____|____|____|____|____|____|____|______|
 |设定IRAM地址或 | 0  |   0   | 0  | 1  | A5 | A4 | A3 | A2 | A1 | A0 | 72us |
 |   卷动地址    |    |       |    |    |    |    |    |    |    |    |      |     SR = 1;	//A5 ~ A0为垂直卷动地址
 |_______________|____|_______|____|____|____|____|____|____|____|____|______|     SR = 0;	//A3 ~ A0为IRAM地址
 |               |    |       |    | 0  | 0  | 0  | A3 | A2 | A1 | A0 |      |
 |               |    |       |    |____|____|____|____|____|____|____|      | 设定GDRAM地址到计数器地址（AC），先设置垂直地址再设置水平地址，
 |设定绘图RAM地址| 0  |   0   | 1  | A6 | A5 | A4 | A3 | A2 | A1 | A0 | 72us | （连续写入2个字节数据来完成垂直和水平地址设置）
 |_______________|____|_______|____|____|____|____|____|____|____|____|______| 垂直地址范围：AC6 ~ AC0
                                                                               水平地址范围：AC3 ~ AC0
  
    扩充功能设定：

	DL = 1;	//8bit MPU接口      DL = 0;	//4bit MPU接口

	RE = 1;	//扩充指令          RE = 0;	//基本指令

	G = 1;	//绘图显示开        G = 0;	// 绘图显示关

*/
