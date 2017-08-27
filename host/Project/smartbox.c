/********************   One Artisan's Regret And Pride   ***********************
*
*	@File    	： smartbox.c
*	@Brief   	： 智能柜核心代码
*	@Author  	： Blade@oarap.org
*	@Hardware	： STM32F103RCT6
*	@Date		： 2016-05
*	@Description： 负责联系底层驱动和算法的实现以及系统调度 需要注意UART指针清零的位置不同
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

SmartBox_Arg_TypeDef		SmartBox_Arg;//定义参数结构体
SmartBox_DataBase_TypeDef 	SmartBox_DataBase[BOX_NUM+1];//共20个箱门（多生成一个第0数组用来记录状态和对齐数组，保留）
Hist_DataBase_TypeDef       Hist_DataBase[100];//记录100条历史记录
Icon_Data_TypeDef 			Icon_Data={{0x00,0x00},{0x00,0x01},{0x00,0x02},{0x00,0x03},{0x00,0x04},{0x00,0x01},{0x00,0x05},{0x00,0x06}};

//u32 Systick_Counter;
u8 Ascii_buf[2];//ASCII字符显示缓冲
/****************************************************************************
*	@Function name	： 数据库初始化
*	@Description    ： 
*	@Author  		： Blade@oarap.org
*	@Date			： 2016-05
*	@param    		： 无
*	@return			： 无
*****************************************************************************/
void Struct_Init()
{
  u8 i;
	for(i=0;i<=BOX_NUM;i++)
	{
		SmartBox_DataBase[i].usable = true;
		SmartBox_DataBase[i].Box_Ascii[0]=(i/10)+48;//ascii码转换功能
		SmartBox_DataBase[i].Box_Ascii[1]=(i%10)+48;
	}

	//上电后从FLASH中读取参数
	I2C_ReadByte(ReadBuffer,0x50,0, ADDR_24LC02);//直接连续读出所有参数
	//转存数据，只在开机的时候从FLASH中读取一次到RAM中的结构体，
	//之后的操作之后向Flash写和使用RAM中的结构体数据【匹配和送显之类的操作】
	for(i=0;i<6;i++)
		SmartBox_Arg.Admin_Pwd[i]	=ReadBuffer[ADMIN_PWD_FLASH_PTR+i];
	for(i=0;i<11;i++)
		SmartBox_Arg.Contact_Inf[i]	=ReadBuffer[CONTACT_INF_FLASH_PTR+i];
	for(i=0;i<3;i++)
		SmartBox_Arg.Group_Num[i]	=ReadBuffer[GROUP_NUM_FLASH_PTR+i];
	for(i=0;i<4;i++)
		SmartBox_Arg.Latest_Time[i]	=ReadBuffer[LATEST_TIME_FLASH_PTR+i];
    for(i=0;i<255;i++)
	  WriteBuffer[i] = ReadBuffer[i];
	Hmi_Send(CONTACT_INF_SET_PTR,VAR_W,SmartBox_Arg.Contact_Inf,0,11);//开机时写入【存在FLASH中】的联系方式
  	Hmi_Send(GROUP_NUM_SET_PTR,VAR_W,SmartBox_Arg.Group_Num,0,3);//写入【存在FLASH中】的柜组号
	Hmi_Send(LATEST_TIME_SET_PTR,VAR_W,SmartBox_Arg.Latest_Time,0,4);//写入【存在FLASH中】的柜组号
		
}
/****************************************************************************
*	@Function name	： 送餐界面框架显示
*	@Description    ： 显示基础的送餐界面框架并更新可用餐柜数量
*	@Author  		： Blade@oarap.org
*	@Date			： 2016-05
*	@param    		： 无
*	@return			： 无
*****************************************************************************/
void LCD12864_Disp_Frame()
{
  LCD12864_Clear();//清屏
  LCD12864_Display_String(1,0,"---------------",15);
  LCD12864_Display_String(2,0,"状态：请扫描订单",16);
  LCD12864_Display_String(3,0,"请使用：__号餐柜",16);
  LCD12864_Display_String(4,0,"可用：  个餐柜",14);
}

/****************************************************************************
*	@Function name	： 系统自检程序
*	@Description    ： 检查各个模块是否好用并返回其自检码
*	@Author  		： Blade@oarap.org
*	@Date			： 2016-05
*	@param  		： 无
*	@return			： 无
*****************************************************************************/
void Self_Check()
{  
	//0-没有错误
	//BIT0-SIM900A错误
	//BIT1-SYN6288错误
	//BIT2-LV4CCD错误
	//BIT3-HMI错误
	//BIT4-24C08错误
  	//BIT5-HMI的RTC读取错误

	u8 Self_Check_Code = 0;						//自检码
	u8 Err_Bits[]={"00000000--轻触LOGO后进入系统--"};
	u8 BUZZ_100MS[]={0x0a};
	u8 BUZZ_500MS[]={0x32};//单位为10ms，控制蜂鸣器响500ms【报警用】
	Self_Check_Code |= Sim900a_Init();			//初始化自检短信模组
	Self_Check_Code |= SYN_Init();				//初始化自检语音合成
	Self_Check_Code |= LV4CCD_Init();			//初始化自检CCD
	Self_Check_Code |= Hmi_Init(1);				//初始化自检人机界面
	Self_Check_Code |= I2C_EEPROM_Init();		//初始化自检24C08 EEPROM	
	Self_Check_Code |= Hmi_RTC_Read();			//初始化自检HMI RTC
		
	if(Self_Check_Code)
	{//从最高位开始检查错误代码
		if(Self_Check_Code&BIT7)Err_Bits[0]=49;
		if(Self_Check_Code&BIT6)Err_Bits[1]=49;
		if(Self_Check_Code&BIT5)Err_Bits[2]=49;
		if(Self_Check_Code&BIT4)Err_Bits[3]=49;
		if(Self_Check_Code&BIT3)Err_Bits[4]=49;
		if(Self_Check_Code&BIT2)Err_Bits[5]=49;
		if(Self_Check_Code&BIT1)Err_Bits[6]=49;
		if(Self_Check_Code&BIT0)Err_Bits[7]=49;
		Hmi_Send(WELCOME_CACHE,VAR_W,Err_Bits,0,30);//显示错误代码到HMI
		LCD12864_Display_String(4,0,Err_Bits,8);//送现至12864
		
		Hmi_Send_reg(BZ_TIME_PTR,REG_W,BUZZ_100MS,0,1);
		Delay_ms(100);
		Hmi_Send_reg(BZ_TIME_PTR,REG_W,BUZZ_100MS,0,1);
		Delay_ms(100);
		Hmi_Send_reg(BZ_TIME_PTR,REG_W,BUZZ_100MS,0,1);
		Delay_ms(100);
		Hmi_Send_reg(BZ_TIME_PTR,REG_W,BUZZ_100MS,0,1);
		Delay_ms(100);
		Hmi_Send_reg(BZ_TIME_PTR,REG_W,BUZZ_100MS,0,1);
		Delay_ms(1500);
	}
	else//如果全部驱动正常
	{
		LED_GPIO_Config();						//运行指示灯（闪烁）
		Hmi_Send_reg(BZ_TIME_PTR,REG_W,BUZZ_500MS,0,1);//一切正常
	}
}
/****************************************************************************
*	@Function name	： 系统初始化
*	@Description    ： 
*	@Author  		： Blade@oarap.org
*	@Date			： 2016-05
*	@param  		： 无
*	@return			： 无
*****************************************************************************/
void Sys_Init()
{

	Decoder_GPIO_Config();			//译码器引脚配置
	I2C_Configuration();			//I2C引脚配置
	LCD12864_GPIO_Config();			//LCD12864引脚初始化
	SYSTICK_Init();					//系统运行指示灯开始闪烁
    USART1_Config();				//人机界面通信接口初始化
    USART2_Config();				//串口参数配置初始化
	USART3_Config();				//条码扫描仪串口
	UART4_Config();					//STN6288
	UART5_Config();					//SIM900A
    NVIC_Configuration();			//中断向量配置初始化
	
	Delay_ms(3000);					//等待外部硬件初始化完成
	Hmi_Init(0);					//开机界面显示在HMI上
	RTC_Init();						//片上RTC向HMI校准
	LCD12864_Init();				//显示器初始化
	
	Self_Check();					//系统自检
	
	Struct_Init();					//初始化数据库
	LCD12864_Disp_Frame();			//显示框架
	Refresh_Usable_Num();
}

/****************************************************************************
*	@Function name	： 数字转换成ASCII码
*	@Description    ： 将U8类型的数字转换成ASCII值
*	@Author  		： Blade@oarap.org
*	@Date			： 2016-05
*	@param    num	： 输入一个U8型待转换的数字
*	@return	Ascii_buf： 返回为两个字符数组的头指针
*****************************************************************************/
u8 *Num2Ascii(u8 num)
{
	Ascii_buf[0] = num/10 + 48;
  	Ascii_buf[1] = num%10 + 48;
	return Ascii_buf;
}
/****************************************************************************
*	@Function name	： 可用餐柜数更新
*	@Description    ： 检查可用餐柜数后更新显示，并且负责更新缓冲区的可用状态（代表整个系统的可用状态）
*						更新显示，更新系统状态
*	@Author  		： Blade@oarap.org
*	@Date			： 2016-05
*	@param    		： 无
*	@return			： 无
*****************************************************************************/
void Refresh_Usable_Num()
{ 
  u8 k=BOX_NUM+1;//实际箱门个数和数据库的对应相差1
  u8 Usable_Num[4]={0};//usable_num[3]为可用的数目，usable_num[2]为HMI发送的高字节，usable_num[0]十位ASCII，usable_num[1]个位ASCII
  
  while(--k)//遍历智能柜数据库，数据库中从BOX_NUM到1
  { 
	if(SmartBox_DataBase[k].usable)
  		Usable_Num[3]++;//第三个位置为缓存
  }
  if(Usable_Num[3])//如果有可用的餐柜
  	SmartBox_DataBase[BUF].usable = true;
  else
  {
  	SmartBox_DataBase[BUF].usable = false;//如果没有可用餐柜，那么将缓冲区中的usable设置为false，表示全部不可以用
  	Usable_Num[3]=0;
  }
  LCD12864_Display_String(4,3,Num2Ascii(Usable_Num[3]),2);//x,y,变量，显示的长度，将可用餐柜数量转换成ASCII后放入到显示缓冲区
  
  Hmi_Send(USABLE_PTR,VAR_W,Usable_Num,2,2);
}

/****************************************************************************
*	@Function name	： 查找可用的餐柜
*	@Description    ： 找到可用餐柜返回在数据库中的位置，从上一次使用的位置+1开始查找可用的餐柜
*	@Author  		： Blade@oarap.org
*	@Date			： 2016-05
*	@param    		： 无
*	@return	u8) last_box：当前的餐柜数据库位置，为0说明餐柜不可用
*	负责资源的申请，pwd_match中释放资源					
*****************************************************************************/
u8 Box_Find()//应加入订单号查询功能
{
  static u8 Last_Box = 0;//静态的局部变量
  u8 i;
  
  if(!SmartBox_DataBase[0].usable)//如果系统不可用便不进行查找
  {
	Last_Box = 0;
	return 0;
  }
  for(i=1;i<=BOX_NUM;i++)//最多查找“BOX_NUM”次
  {
	if((Last_Box+i)>=BOX_NUM)//如果到了最大的餐柜数
	  Last_Box = 0;//则从1开始查找，所以认为1的上一个餐柜数为0，
	if(SmartBox_DataBase[Last_Box+i].usable)
	{
	  SmartBox_DataBase[Last_Box+i].usable = false;  
	  Last_Box += i;//更新上一次使用的箱门的数值
	  break;
	}
  }
  return Last_Box;//返回当前的数值
}
/****************************************************************************
*	@Function name	： 创建随机密码
*	@Description    ： 将创建一个六位数的随机验证码并送到指定的数据库位置
*	@Author  		： Blade@oarap.org
*	@Date			： 2016-05
*	@param  box_num	： 输入餐柜在数据库中的序列号即位置
*	@return	true	： 创建成功
*	@history		： 加入防止密码相近或者重复的检测功能
*****************************************************************************/
bool Pwd_Create(u8 box_num)//创建随机密码给指定箱子
{ 
  u8 i=0,j=0;
  u8 Match_Flag = BIT5;
  int seed;
  while(Match_Flag >= BIT5)//如果生成的密码不可用，说明有重复或相近
  {
	  for(j=0;j<6;j++)//根据随机种子生成六位的随机数
	  {
			srand(Systick_Counter+j);
			seed=(int)(10.0*rand()/(RAND_MAX+1.0));
			SmartBox_DataBase[box_num].pwd[j] = seed;
	  }
	  for(i=1;i<=BOX_NUM;i++)//搜索所有箱门，检测密码冲突
	  {	
			Match_Flag = BIT0;
			for(j=0;j<6;j++)
			{
				if((SmartBox_DataBase[i].pwd[j] == SmartBox_DataBase[box_num].pwd[j]) && (i!=box_num))//密码防重复检测，//被匹配的数据库不能是当前生成的库
					Match_Flag <<= 1;	
			}		
			if(Match_Flag >= BIT5)//完全相同或者及其相近则重新生成
				break;	//结束本次查找（循环），由上面重新生成				
	  }
  }
  return true;
}
/****************************************************************************
*	@Function name	： 随机密码匹配
*	@Description    ： 读取在UART_DATA.HMI_REC.DATA帧中密码字段的数据和数据库中
*						所有的密码段进行匹配并验证餐柜可用性，若成功则释放资源
*	@Author  		： Blade@oarap.org
*	@Date			： 2016-05
*	@param  hmi_rec_data 从hmi发来的密码帧数据
*	@return	（u8）i ： 为所在的数据库位置
*	负责资源的释放，box_find中申请资源
*****************************************************************************/
u8 Pwd_Match(u8 *Str)
{//还需要检查密码长度和箱门的是否可用
  u8 i=0,j=0; 	
  u8 Match_Flag = BIT0;//每次匹配一个字节就向左移动一位，如果最终匹配6位说明密码匹配成功
  for(i=1;i<=BOX_NUM;i++)//搜索所有箱门
  {
	  Match_Flag = BIT0;
	  for(j=0;j<PWD_LEN;j++)//2016-05-31修改
	  {
			if(SmartBox_DataBase[i].pwd[j] == Str[j+7]-48)//UART_DATA.HMI_REC.DATA[j+7]-48)//从第七位为密码的ASCII
			  	Match_Flag <<= 1;						
	  }
	  if((Match_Flag == BIT6) && (!SmartBox_DataBase[i].usable))//说明六次匹配成功并且箱门不可用（说明正在使用中）
	  {
			SmartBox_DataBase[i].usable = true;//改变箱门状态，释放资源
			return i;//返回值为数据库位置
	  }
  }
  return 0;
}
/****************************************************************************
*	@Function name	： 管理员密码检查
*	@Description    ： 读取在UART_DATA.HMI_REC.DATA帧中密码字段的数据
*	@Author  		： Blade@oarap.org
*	@Date			： 2016-05
*	@param  hmi_rec_data 从hmi发来的密码帧数据
*	@return	BIT1 ： 成功
*****************************************************************************/
bool Admin_Match(u8 *Str)
{
  u8 i=0; 	
  u8 Match_Flag = BIT0;//每次匹配一个字节就向左移动一位，如果最终匹配6位说明密码匹配成功
  
  for(i=0;i<PWD_LEN;i++)
  {
		if(SmartBox_Arg.Admin_Pwd[i] == Str[i+7])//UART_DATA.HMI_REC.DATA[j+7]-48)//从第七位为密码的ASCII
			Match_Flag <<= 1;						
  }
  if(Match_Flag == BIT6)//说明六次匹配成功并且箱门不可用（说明正在使用中）
  {
		return true;//返回true，标志管理员密码匹配成功
  }
  return false;
}
/**********************************发送随机验证码到PC**********************************/
bool send_pwd2_pc()
{
  u8 i;
	for(i=0;i<6;i++)
		USART_Send(USART2,SmartBox_DataBase[BUF].pwd[i]+48);
	USART_Send(USART2,'\r');
	USART_Send(USART2,'\n');
	return true;
}

/****************************************************************************
*	@Function name	： 主进程
*	@Description    ： 用来处理各种中断和系统调度
*	@Author  		： Blade@oarap.org
*	@Date			： 2016-05
*	@param  		： 无
*	@return			： 无
*****************************************************************************/
void Main_Process()
{
 
	while(UART_DATA.BAR_REC.FLAG || UART_DATA.SIM_REC.FLAG || UART_DATA.HMI_REC.FLAG)//反复检查标志位的变化
	{
  		SmartBox_Arg.Cur_RTC = Time_GetCalendarTime();//如果没有数据操作系统时间是不会更新的
		if(UART_DATA.BAR_REC.FLAG)//lv4 ccd传回来的数据为ascii码的形式
		{
			UART_DATA.BAR_REC.FLAG=false;//清空接收标志位
  			Bar_Handler();
		}	
		if(UART_DATA.HMI_REC.FLAG)//HMI发来数据
		{
			UART_DATA.HMI_REC.COUNTER=0;
			UART_DATA.HMI_REC.FLAG=false;
			Hmi_Handler();
		}
		if(UART_DATA.SIM_REC.FLAG)
		{
			UART_DATA.SIM_REC.FLAG=false;
		}
	}
}
/****************************************************************************
*	@Function name	： 订单号检查
*	@Description    ： 
*	@Author  		： Blade@oarap.org
*	@Date			： 2016-05
*	@param  		： 无
*	@return			： 无
*****************************************************************************/
bool Order_Check(u8 *Str)
{	//从起始位置x 到 x+ORDER_LEN检查订单号
  	//还应检查最后三位为柜组号码
  	//检查订单是否全为数字（Num_Check（））
  	//检查订单号的格式是否正确
	//数据库检查订单号是否备用过
  //strncmp 可以调用
  u8 i;
  u8 Correct_Counter = ORDER_LEN;//正确的字节个数,应该和订单长度相等
  
  if((Str[0]-48)+(Str[1]-48)==0)//如果订单是以00开头说明是DEMO单号
	return true;
  for(i=0;i<ORDER_LEN;i++)
  {
	if((Str[i] > 47) && (Str[i] < 58))//标明该字节表示的数据
	  Correct_Counter--;
  }
  if(Correct_Counter)
	return false;//如果Correct_Counter非0则说明有数据不是数字，直接结束
  
  /*-------------------------------------------调试用---------------------------------------------------*/
  if((Str[0]-48==5)&&(Str[1]-48==2))
  	return true;
  /*-------------------------------------------调试用---------------------------------------------------*/
  
  if(SmartBox_Arg.Cur_RTC.tm_mday!=((Str[0]-48)*10+(Str[1]-48)))//日期检查
	return false;//日期不正确，直接结束
  
  return true;
}
/****************************************************************************
*	@Function name	： 柜组号检查
*	@Description    ： 用在手动录入订单时候检查柜组号是否正确写入
*	@Author  		： Blade@oarap.org
*	@Date			： 2016-05
*	@param  	hmi_rec_data： 入口参数数据指针，Start_Num起始位置
*	@return			： true false表示是否成功
*****************************************************************************/
bool Group_Num_Check(u8 *Str)
{  
  u8 i;
  u8 Match_Flag = BIT0;//匹配的字节个数
  for(i=0;i<3;i++)//检查订单输入的后三位为柜组号
  {//从第22开始是柜组号所在位置
  	if(SmartBox_Arg.Group_Num[i]==Str[i+15])
	{
		Match_Flag <<= 1;
	}
  }
  if(Match_Flag != BIT3)//如果没有有3个相等的位，说明柜组号录入有问题
  {
  	return false;
  }
  return true;
}
/****************************************************************************
*	@Function name	： 读数据写入到flash
*	@Description    ： 用来处理人机界面修改常用flash参数
*	@Author  		： Blade@oarap.org
*	@Date			： 2016-05
*	@param  *str	： 系统常用参数结构体
*	@param  length	： 从UARTDATA中要转存的长度
*	@param  flash_ptr： 要更改的参数在WriteBuffer中的位置
*	@return			： 无
*****************************************************************************/
void ReadData_WriteFlash(u8 *str,u8 length,u8 flash_ptr)
{
  u8 i=0;
	for(i=0;i<length;i++)
  {//转存到RAM
	str[i]=UART_DATA.HMI_REC.DATA[i+7];
	WriteBuffer[flash_ptr+i]=UART_DATA.HMI_REC.DATA[i+7];
  }
  for(i=0; i<0x50; i++)//并写入flash
		while( !I2C_WriteByte(WriteBuffer[i], i,  ADDR_24LC02) );
  //Hmi_Send(ICON_PTR,VAR_W,Icon_Data.MODI_FINISH,0,2);//显示修改成功的标志
}
/****************************************************************************
*	@Function name	： BAR_Hadler
*	@Description    ： 条形码数据处理程序
*	@Author  		： Blade@oarap.org
*	@Date			： 2016-05
*	@param  		： 无
*	@return			： 无
*****************************************************************************/
void Bar_Handler()
{
  	u8 Cur_Box;//当前箱门号
	if(Order_Check(UART_DATA.BAR_REC.DATA))
	{
		Cur_Box = Box_Find();//查找箱门
		if(Cur_Box)//如果餐柜号非0
		{
			LCD12864_Display_String(1,0,UART_DATA.BAR_REC.DATA,15);//显示订单号在LCD12864上面
			LCD12864_Display_String(2,3,"发送中....",10);//显示当前的发送状态
			Pwd_Create(Cur_Box);//创建随机验证码给指定的箱门
			Lock_Open(Cur_Box);//开指定箱门的锁
			LCD12864_Display_String(3,4,SmartBox_DataBase[Cur_Box].Box_Ascii,2);//显示当前餐柜号码到LCD12864上面
			if(Sim900a_Send_Message(UART_DATA.BAR_REC.DATA,Cur_Box))//发送短信（订单号，餐柜号起始位置，密码起始位置，所在的餐柜）	
			{
				LCD12864_Display_String(2,3,"发送已完成",10);//显示当前的工作状态
			}
			else
			{//如果返回false则认为发送失败
				LCD12864_Display_String(2,3,"--无法发送",10);//显示当前的工作状态
				//---------设计为如果又一次发送失败，则所有都不可发送--------------
			}
			Delay_ms(300);
			LCD12864_Display_String(2,3,"请扫描条码",10);//扫描可用
		}
		else
		{
			LCD12864_Display_String(3,4,"__",2);//不显示餐柜号
			LCD12864_Display_String(2,3,"无可用餐柜",10);
		}
	}
	else//订单号不正确
	{
		LCD12864_Display_String(1,0,"----单号有误---",15);
		Delay_ms(800);
	}
	Refresh_Usable_Num();//更新显示可用的餐柜数量
	LCD12864_Display_String(1,0,"---------------",15);//关闭订单显示
}

/****************************************************************************
*	@Function name	： Hmi_Hadler
*	@Description    ： 人机界面数据处理程序
*	@Author  		： Blade@oarap.org
*	@Date			： 2016-05
*	@param  		： 无
*	@return			： 无
*****************************************************************************/
void Hmi_Handler()
{
  	u8 Touch_Correct[]={0x5a};
    u8 Cur_Box;//当前箱门号
	u8 Cur_Order[18];//当前订单号
	u8 i=0;
	u8 Clear_Disp[2]={0xff,0xff};//清空显示区的字
	u8 Sending[]={"短信发送中...."};
	u8 Send_Success[]={"短信已发送成功"};
	u8 Send_Fail[]={"短信已发送失败"};
	//为了显示效果的完美，录入完成后HMI自动调到带有等待icon的页面，当单片机处理完后会立即将页面调整到
	//录入完成页面（和刚刚的布局完全相同）并显示等待图标，让后在刷新结果图标；
	
	Hmi_Send(BOXNUM_PTR,VAR_W,Clear_Disp,0,2);//关闭箱门号显示
	Hmi_Send(SENDSTA_PTR,VAR_W,Clear_Disp,0,2);//关闭【短信发送状态的显示】
	Hmi_Send(ICON_PTR,VAR_W,Icon_Data.WAIT,0,2);//显示等待ICON【在录入完成页面】
	Hmi_Page_Switch(INPUT_FINISH);//跳转到【空白的录入完成页面】
	

	switch(UART_DATA.HMI_REC.DATA[HMI_RAM_PTR])//根据不同的返回地址确定不同的操作
	{
		case PWD_PTR://如果发来的数据是随机密码区域
		{
			Cur_Box = Pwd_Match(UART_DATA.HMI_REC.DATA);
			if(Cur_Box)
			{	
				Delay_ms(200);//显示等待图标用
				Lock_Open(Cur_Box);		  
				Hmi_Send(ICON_PTR,VAR_W,Icon_Data.PWD_RIGHT,0,2);//显示密码成功的ICON
				Hmi_Send(BOXNUM_PTR,VAR_W,Num2Ascii(Cur_Box),0,2);//显示箱门号				
				SYN_Send(Cur_Box);
			}
			else		  
			{	
				Delay_ms(500);	//显示等待图标用	
				SYN_Send(0);
				Hmi_Send(BOXNUM_PTR,VAR_W,Clear_Disp,0,2);//关闭箱门号显示
				Hmi_Send(ICON_PTR,VAR_W,Icon_Data.PWD_ERROR,0,2);//显示密码失败的ICON
			}
			//Refresh_Usable_Num();
		}break;

		case ORDER_PTR://如果发来的数据是订单号区域
		{
			for(i=0;i<18;i++)//获取订单号
			{
				Cur_Order[i] = UART_DATA.HMI_REC.DATA[i+7];
			}
			if(Order_Check(Cur_Order) && Group_Num_Check(Cur_Order))//----------------订单号正确性检查-----------------------
			{
				Cur_Box = Box_Find();
				if(Cur_Box)//如果餐柜号非0
				{
					Delay_ms(200);//显示等待图标用
					Pwd_Create(Cur_Box);//创建随机验证码给指定的箱门
					Lock_Open(Cur_Box);		  
					Hmi_Send(ICON_PTR,VAR_W,Icon_Data.ORDER_RIGHT,0,4);//显示密码成功的ICON【xx号箱门已打开】和开箱门公用同一个LOGO
					Hmi_Send(BOXNUM_PTR,VAR_W,Num2Ascii(Cur_Box),0,2);//显示箱门号				
					SYN_Send(Cur_Box);
					//Refresh_Usable_Num();//更新显示可用的餐柜数量
					Hmi_Send(SENDSTA_PTR,VAR_W,Sending,0,14);//向【发送状态区】写入
					if(Sim900a_Send_Message(Cur_Order,Cur_Box))//发送短信（订单号，餐柜号起始位置，密码起始位置，所在的餐柜）		
					{//发送成功
						Hmi_Send(SENDSTA_PTR,VAR_W,Send_Success,0,14);//向【发送状态区】写入
					}
					else
					{//短信发送失败
						Hmi_Send(SENDSTA_PTR,VAR_W,Send_Fail,0,14);//向【发送状态区】写入
					}
				}
				else	//没有可用餐柜或单号错误-------需要修改--------------------------------------------
				{
					Delay_ms(500);	//显示等待图标
					//Refresh_Usable_Num();//更新显示可用的餐柜数量
					LCD12864_Display_String(3,4,"__",2);//不显示餐柜号
					LCD12864_Display_String(2,0,"--无可用餐柜-- ",15);
				}
			}
			else//单号检查失败的情况下
			{
			  Delay_ms(500);
				Hmi_Send(ICON_PTR,VAR_W,Icon_Data.ORDER_ERROR,0,4);//显示订单错误的ICON
				SYN_FrameInfo(0,"单号有误");
			}
			
		}break;
		
		case ADMIN_PTR://如果发来的数据管理员密码录入
		{
		  	Delay_ms(300);//显示等待图标
			if(Admin_Match(UART_DATA.HMI_REC.DATA))
			{
			  SYN_FrameInfo(0,"登陆成功");
			  Hmi_Send(ICON_PTR,VAR_W,Icon_Data.ADMIN_RIGHT,0,2);//显示密码正确的ICON
			  Delay_ms(500);//等待看清图标
			  Hmi_Page_Switch(ADMIN_PAGE);//跳转到【管理员界面】
			}
			else
			{
			  Hmi_Send(ICON_PTR,VAR_W,Icon_Data.ADMIN_ERROR,0,2);//显示密码错误的ICON
			  SYN_FrameInfo(0,"密码错误");
			}
		}break;
		
		
		
		case CONTACT_INF_SET_PTR://联系方式设置
		{
		  Delay_ms(300);
		  ReadData_WriteFlash(SmartBox_Arg.Contact_Inf,11,CONTACT_INF_FLASH_PTR);
		  Hmi_Send(ICON_PTR,VAR_W,Icon_Data.MODI_FINISH,0,2);//显示修改成功的标志
		}break;
		
		case GROUP_NUM_SET_PTR://柜组号设置
		{
		  Delay_ms(300);
		  ReadData_WriteFlash(SmartBox_Arg.Group_Num,3,GROUP_NUM_FLASH_PTR);
		  Hmi_Send(ICON_PTR,VAR_W,Icon_Data.MODI_FINISH,0,2);//显示修改成功的标志
		}break;
		
		case ADMIN_PWD_SET_PTR://管理员密码设置
		{
		  Delay_ms(300);
		  ReadData_WriteFlash(SmartBox_Arg.Admin_Pwd,6,ADMIN_PWD_FLASH_PTR);
		  Hmi_Send(ICON_PTR,VAR_W,Icon_Data.MODI_FINISH,0,2);//显示修改成功的标志
		  
		}break;
		
		case OPEN_BOX_SET_PTR://打开餐柜，也是ASCII录入
		{
		  Delay_ms(300);
		  if(((UART_DATA.HMI_REC.DATA[7]==0x30)&&(UART_DATA.HMI_REC.DATA[8]==0x30)) || ((UART_DATA.HMI_REC.DATA[7]==0X30)&&(UART_DATA.HMI_REC.DATA[8]==0xFF)))
		  {//打开所有餐柜,如果输入的是0或00
			for(i=1;i<=BOX_NUM;i++)
		  		Lock_Open(i);
		  }
		  else
		  {//打开指定餐柜
			if(UART_DATA.HMI_REC.DATA[8]==0XFF)
			  	Lock_Open(UART_DATA.HMI_REC.DATA[7]-48);
			else//如果输入两位
		  		Lock_Open(((UART_DATA.HMI_REC.DATA[7]-48)*10)+((UART_DATA.HMI_REC.DATA[8]-48)));
		  }
		  Hmi_Page_Switch(ADMIN_PAGE);//跳转到【管理员界面】
		  
		}break;
		
		case LATEST_TIME_SET_PTR://最晚取餐时间
		{
		  Delay_ms(300);
		  ReadData_WriteFlash(SmartBox_Arg.Latest_Time,4,LATEST_TIME_FLASH_PTR);
		  Hmi_Send(ICON_PTR,VAR_W,Icon_Data.MODI_FINISH,0,2);//显示修改成功的标志
		}break;

		
		
		case ADMIN_KEY_RETURN://管理员界面的按键返回
	  	{
		 	Delay_ms(300);
	  		switch(UART_DATA.HMI_REC.DATA[8])
			{
			  case ADMIN_TOUCH_CORRECT:
			  {
			  		Hmi_Send_reg(TOUCH_CORRECT_PTR,REG_W,Touch_Correct,0,1);//触发触摸校准操作
			  }break;
			  
			  case ADMIN_RESTART_SYS:
			  {//重启系统
        			NVIC_SystemReset();
			  }break;
			  
			  case ADMIN_HISTORY:	
			  {
			  }break;
			  
			  case ADMIN_RESERVE:
			  {
			  }break;
			}
			Hmi_Page_Switch(ADMIN_PAGE);//跳转到【管理员界面】
	  	}break;
	}
	Refresh_Usable_Num();
}

