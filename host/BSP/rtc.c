/*******************************************************************************
* 本文件实现基于RTC的日期功能，提供年月日的读写。（基于ANSI-C的time.h）
* 
* RTC中保存的时间格式，是UNIX时间戳格式的。即一个32bit的time_t变量（实为u32）
*
* ANSI-C的标准库中，提供了两种表示时间的数据  型：
* time_t:   	UNIX时间戳（从1970-1-1起到某时间经过的秒数）
* typedef unsigned int time_t;
* 
* struct tm:	Calendar格式（年月日形式）
*   tm结构如下：
*   struct tm {
*   int tm_sec;   // 秒 seconds after the minute, 0 to 60  (0 - 60 allows for the occasional leap second)
*   int tm_min;   // 分 minutes after the hour, 0 to 59
*		int tm_hour;  // 时 hours since midnight, 0 to 23
*		int tm_mday;  // 日 day of the month, 1 to 31
*		int tm_mon;   // 月 months since January, 0 to 11
*		int tm_year;  // 年 years since 1900
*		int tm_wday;  // 星期 days since Sunday, 0 to 6
*		int tm_yday;  // 从元旦起的天数 days since January 1, 0 to 365
* 	int tm_isdst; // 夏令时？？Daylight Savings Time flag
* 		...
* 	}
* 	其中wday，yday可以自动产生，软件直接读取
* 	mon的取值为0-11
*	***注意***：
*	tm_year:在time.h库中定义为1900年起的年份，即2008年应表示为2008-1900=108
* 	这种表示方法对用户来说不是十分友好，与现实有较大差异。
* 	所以在本文件中，屏蔽了这种差异。
* 	即外部调用本文件的函数时，tm结构体类型的日期，tm_year即为2008
* 	注意：若要调用系统库time.c中的函数，需要自行将tm_year-=1900
* 
* 成员函数说明：
* struct tm Time_ConvUnixToCalendar(time_t t);
* 	输入一个Unix时间戳（time_t），返回Calendar格式日期
* time_t Time_ConvCalendarToUnix(struct tm t);
* 	输入一个Calendar格式日期，返回Unix时间戳（time_t）
* time_t Time_GetUnixTime(void);
* 	从RTC取当前时间的Unix时间戳值
* struct tm Time_GetCalendarTime(void);
* 	从RTC取当前时间的日历时间
* void Time_SetUnixTime(time_t);
* 	输入UNIX时间戳格式时间，设置为当前RTC时间
* void Time_SetCalendarTime(struct tm t);
* 	输入Calendar格式时间，设置为当前RTC时间
* 
* 外部调用实例：
* 定义一个Calendar格式的日期变量：
* struct tm now;
* now.tm_year = 2008;
* now.tm_mon = 11;		//12月
* now.tm_mday = 20;
* now.tm_hour = 20;
* now.tm_min = 12;
* now.tm_sec = 30;
* 
* 获取当前日期时间：
* tm_now = Time_GetCalendarTime();
* 然后可以直接读tm_now.tm_wday获取星期数
* 
* 设置时间：
* Step1. tm_now.xxx = xxxxxxxxx;
* Step2. Time_SetCalendarTime(tm_now);
* 
* 计算两个时间的差
* struct tm t1,t2;
* t1_t = Time_ConvCalendarToUnix(t1);
* t2_t = Time_ConvCalendarToUnix(t2);
* dt = t1_t - t2_t;
* dt就是两个时间差的秒数
* dt_tm = mktime(dt);	//注意dt的年份匹配，ansi库中函数为相对年份，注意超限
* 另可以参考相关资料，调用ansi-c库的格式化输出等功能，ctime，strftime等
* 
*******************************************************************************/

//=============================================================================
//文件名称:RTC.c
//功能概要:初始化RTC 
//版权所有:源地工作室www.vcc-gnd.com
//版本更新:2013-02-20 V1.0
//调试方式:J-LINK-OB	
//=============================================================================

#include "includes.h"
void Time_Set(u32 t);

//=============================================================================
//函数名称:Time_ConvUnixToCalendar
//功能概要:转换UNIX时间戳为日历时间
//参数说明:- t: 当前时间的UNIX时间戳
//函数返回:struct tm
//=============================================================================
struct tm Time_ConvUnixToCalendar(time_t t)
{
	struct tm *t_tm;
	t_tm = localtime(&t);
	t_tm->tm_year += 1900;	/* localtime转换结果的tm_year是相对值，需要转成绝对值 */
	return *t_tm;
}

//=============================================================================
//函数名称:Time_ConvCalendarToUnix
//功能概要:写入RTC时钟当前时间
//参数说明:- t: struct tm
//函数返回:time_t
//=============================================================================
time_t Time_ConvCalendarToUnix(struct tm t)
{
	t.tm_year -= 1900;  /* 外部tm结构体存储的年份为2008格式	*/
						/* 而time.h中定义的年份格式为1900年开始的年份 */
						/* 所以，在日期转换时要考虑到这个因素。*/
	return mktime(&t);
}


//=============================================================================
//函数名称:Time_GetUnixTime
//功能概要:从RTC取当前时间的Unix时间戳值
//参数说明:无
//函数返回:time_t
//=============================================================================
time_t Time_GetUnixTime(void)
{
	return (time_t)RTC_GetCounter();
}

//=============================================================================
//函数名称:Time_GetCalendarTime
//功能概要:从RTC取当前时间的日历时间（struct tm）
//参数说明:无
//函数返回:struct tm
//=============================================================================
struct tm Time_GetCalendarTime(void)
{
	time_t t_t;//时间戳格式的时间
	struct tm t_tm;//定义为结构体的时间格式

	t_t = (time_t)RTC_GetCounter();
	t_tm = Time_ConvUnixToCalendar(t_t);
	return t_tm;
}


//=============================================================================
//函数名称:Time_SetUnixTime
//功能概要:将给定的Unix时间戳写入RTC
//参数说明:- t: time_t 
//函数返回:无
//=============================================================================
void Time_SetUnixTime(time_t t)
{
	RTC_WaitForLastTask();
	RTC_SetCounter((u32)t);
	RTC_WaitForLastTask();
	return;
}


//=============================================================================
//函数名称:Time_SetCalendarTime
//功能概要: 将给定的Calendar格式时间转换成UNIX时间戳写入RTC
//参数说明:- t: struct tm
//函数返回:无
//=============================================================================
void Time_SetCalendarTime(struct tm t)
{
	Time_SetUnixTime(Time_ConvCalendarToUnix(t));
	return;
}

//=============================================================================
//函数名称:NVIC_Configuration
//功能概要: Configures the nested vectored interrupt controller.
//参数说明:无
//函数返回:无
//=============================================================================
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  /* Enable the RTC Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


//=============================================================================
//函数名称:RTC_Configuration
//功能概要:Configures the RTC.
//参数说明:无
//函数返回:无
//=============================================================================
static void RTC_Configuration(void)
{
  //启用PWR和BKP的时钟（from APB1）
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
  //后备域解锁
  PWR_BackupAccessCmd(ENABLE);
  //备份寄存器模块复位
  BKP_DeInit();
  //外部32.768K时钟使能
  RCC_LSEConfig(RCC_LSE_ON);
  //等待稳定
  while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {}
  //RTC时钟源配置成LSE（外部32.768K）
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
  //RTC开启
  RCC_RTCCLKCmd(ENABLE);
  //开启后需要等待APB1时钟与RTC时钟同步，才能读写寄存器
  RTC_WaitForSynchro();
  //读写寄存器前，要确定上一个操作已经结束
  RTC_WaitForLastTask();
  //使能秒中断
  /*RTC_ITConfig(RTC_IT_SEC, ENABLE);*/
  //等待寄存器写入完成
  RTC_WaitForLastTask();
  //设置RTC分频器，使RTC时钟为1Hz
  //RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1)
  RTC_SetPrescaler(32767); 
  //等待写入完成
  RTC_WaitForLastTask();
}

//=============================================================================
//函数名称:RTC_Init
//功能概要:RTC Initialization
//参数说明:无
//函数返回:无
//=============================================================================
void RTC_Init(void)
{
	struct tm time;
	 
	memset(&time, 0 , sizeof(time) );	/* 清空结构体 */
  //if (BKP_ReadBackupRegister(BKP_DR1)!= 0xA5A5)//------------------如果还没有设置时间--------------
  {
    /* Backup data register value is not correct or not yet programmed (when
    the first time the program is executed) */
    //printf("RTC not yet configured....\r\n");
    /* RTC Configuration */
     RTC_Configuration();					//------------配置RTC的基本设置--------------------
	 //Time_Regulate();						//-----------时间录入---------------------


    if(!Hmi_RTC_Read())//如果HMI的RTC读取正常
	{//-------------注意HMIRTC读回来的数据为16进制的表示方法----------请注意-----------
		time.tm_year = (UART_DATA.HMI_REC.DATA[6]/16)*10  + UART_DATA.HMI_REC.DATA[6]%16 +2000;//年
		time.tm_mon  = (UART_DATA.HMI_REC.DATA[7]/16)*10  + UART_DATA.HMI_REC.DATA[7]%16;//月
		time.tm_mday = (UART_DATA.HMI_REC.DATA[8]/16)*10  + UART_DATA.HMI_REC.DATA[8]%16;//日
		time.tm_hour = (UART_DATA.HMI_REC.DATA[10]/16)*10 + UART_DATA.HMI_REC.DATA[10]%16;//时
		time.tm_min  = (UART_DATA.HMI_REC.DATA[11]/16)*10 + UART_DATA.HMI_REC.DATA[11]%16;//分
		time.tm_sec  = (UART_DATA.HMI_REC.DATA[12]/16)*10 + UART_DATA.HMI_REC.DATA[12]%16;//秒
		Time_SetCalendarTime(time);
	}
  /* Return the value to store in RTC counter register */
 
  
	 /* Adjust time by values entred by the user on the hyperterminal */
    //printf("RTC configured....\r\n");
    //BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
  }
  //else
  {
    /* Check if the Power On Reset flag is set */
 //   if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
    {//提示用户中断类型为电源复位
      //printf("Power On Reset occurred....\r\n");
    }
    /* Check if the Pin Reset flag is set */
//    else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
    {//复位类型为外部中断
      //printf("External Reset occurred....\r\n");
    }

    //printf("No need to configure RTC....\r\n");
    /* Wait for RTC registers synchronization */
//    RTC_WaitForSynchro();

    /* Enable the RTC Second */
//    RTC_ITConfig(RTC_IT_SEC, ENABLE);
    /* Wait until last write operation on RTC registers has finished */
//    RTC_WaitForLastTask();
  }

   /* NVIC configuration */
   NVIC_Configuration();

#ifdef RTCClockOutput_Enable
  /* Enable PWR and BKP clocks */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

  /* Allow access to BKP Domain */
  PWR_BackupAccessCmd(ENABLE);

  /* Disable the Tamper Pin */
  BKP_TamperPinCmd(DISABLE); /* To output RTCCLK/64 on Tamper pin, the tamper
                                 functionality must be disabled */

  /* Enable RTC Clock Output on Tamper Pin */
  BKP_RTCOutputConfig(BKP_RTCOutputSource_CalibClock);
#endif

   /* Clear reset flags */
  RCC_ClearFlag();
  return;
}

//=============================================================================
//函数名称:Time_Init
//功能概要:Printf Time
//参数说明:无
//函数返回:无
//=============================================================================
void Time_Init(void)
{
    struct tm time;
    memset(&time, 0 , sizeof(time) );	/* 清空结构体 */
    PWR_BackupAccessCmd(ENABLE);	
    RTC_WaitForLastTask(); 
    time.tm_year=2070;
    time.tm_mon=00;
    time.tm_mday=00;
    time.tm_hour=0;
    time.tm_min=0;
    time.tm_sec=0;
    Time_SetCalendarTime(time); 
    RTC_WaitForLastTask();
    PWR_BackupAccessCmd(DISABLE);	
    printf("=======================Time Settings==========================\r\n");
	 
}
/*
*********************************************************************************************************
*	函 数 名: DateTimeDis_Init
*	功能说明: 初始化显示屏显示
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*//*
void DateTimeDis_Init(void)
{
	 OLED_DispStr(10, 25, "    年  月  日", &tFont12);
	 OLED_DispStr(10, 40, "  时  分  秒", &tFont12);
}
//=============================================================================
//函数名称:Time_Display
//功能概要:Printf Time
//参数说明:无
//函数返回:无
//=============================================================================
u8 displayflag;
void Time_Display(void)
{
	 struct tm time;
	 time = Time_GetCalendarTime();
	 printf("Time: %d-%d-%d   %02d:%02d:%02d \r\n", time.tm_year, \
									 time.tm_mon+1, time.tm_mday,\
									 time.tm_hour, time.tm_min, time.tm_sec);
	OLED_DisDigital(10, 25,time.tm_year, &tFont12);//显示年
	if((time.tm_mon+1)==1 && time.tm_mday==1 && time.tm_hour == 0 && time.tm_min == 0 && time.tm_sec == 0)
	{
		 OLED_DispStr(55, 25,"  ", &tFont12);
	}
	OLED_DisDigital(55, 25,(time.tm_mon+1), &tFont12);//显示月
	
	if(time.tm_mday==1 && time.tm_hour == 0 && time.tm_min == 0 && time.tm_sec == 0)
	{
		 OLED_DispStr(78, 25,"  ", &tFont12);
	}
	OLED_DisDigital(78, 25,time.tm_mday, &tFont12);//显示日
	
	if(time.tm_hour == 0 && time.tm_min == 0 && time.tm_sec == 0)
	{
		 OLED_DispStr(10, 40,"  ", &tFont12);
	}
	OLED_DisDigital(10, 40,time.tm_hour, &tFont12);//显示时
	
	if(time.tm_min == 0 && time.tm_sec == 0)
	{
		 OLED_DispStr(37, 40,"  ", &tFont12);
	}
	OLED_DisDigital(37, 40,time.tm_min, &tFont12);//显示分
	
	if(time.tm_sec == 0)
	{
		 OLED_DispStr(63, 40,"  ", &tFont12);
	}
	OLED_DisDigital(63, 40,time.tm_sec, &tFont12);//显示秒
}
*/
//=============================================================================
//函数名称:RTC_IRQHandler
//功能概要:This function handles RTC global interrupt request.
//参数说明:无
//函数返回:无
//=============================================================================
void RTC_IRQHandler(void)
{
  //static uint8_t Display;
  if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
  {
    /* Clear the RTC Second interrupt */
    RTC_ClearITPendingBit(RTC_IT_SEC);
	  //Display = ~Display;   
    //Time_Display();
  }
}
