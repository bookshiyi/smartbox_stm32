#ifndef __SMARTBOX_H
#define __SMARTBOX_H
/********************   One Artisan's Regret And Pride   ***********************
*
*	@File    	： smartbox.h
*	@Brief   	： 智能柜头文件
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

//定义静态参数
#define BUF 		0 					//第0组数据库数组为缓冲区
#define BOX_NUM 	20 					//定义箱子数目
#define ORDER_LEN 	15					//定义订单号的长度
#define PWD_LEN 	6 					//定义随机验证码的长度
#define HIGH 		0					//高半字为数组第0个
#define LOW	 		1					//低位字为数组第1个

//定义系统变量参数结构体类型
typedef struct 
{
	struct tm Cur_RTC;					//系统工作时候的实时时钟，来自STM32片上的RTC
	u8 	Admin_Pwd[6];					//管理员密码
	u8  Group_Num[3];				//组号
	u8  Contact_Inf[11];				//联系方式
	u8	Latest_Time[4];					//最晚取餐
}SmartBox_Arg_TypeDef;

//定义系统数据库结构体类型
typedef struct 
{
  u8  	 Box_Ascii[2];		//箱门号的ASCII值
  bool   usable;	//是否可用
  u8	 pwd[PWD_LEN];	//密码
}SmartBox_DataBase_TypeDef;

//定义历史记录数据库
typedef struct 
{
  u8	 Day;		//日
  u8	 Hour;		//时
  u8	 Min;		//分
  u8	 Sec;		//秒
  u8	 Order[ORDER_LEN]; //订单号
  u8  	 Box;		//箱门号
  u8	 Pwd[PWD_LEN];	//密码
}Hist_DataBase_TypeDef;

extern SmartBox_DataBase_TypeDef 	SmartBox_DataBase[];
extern struct tm Cur_RTC;//系统工作时候的实时时钟，来自STM32片上的RTC
extern SmartBox_Arg_TypeDef		SmartBox_Arg;//定义参数结构体
/*************************函数声明********************************************/
void Struct_Init(void);//初始化数据库
bool Pwd_Create(u8 box_num);//随机验证码生成函数
bool send_pwd2_pc(void);//发送随即验证码到上位机
u8 Box_Find(void);
u8 Pwd_Match(u8 *Str);
bool Admin_Match(u8 *Str);
u8 *Num2Ascii(u8 num);

void Self_Check(void);//自检程序
void Sys_Init(void);
void Main_Process(void);//主进程
void LCD12864_Disp_Frame(void);
void Refresh_Usable_Num(void);
//void Self_Check_Code_Disp(u8 Code);

bool Order_Check(u8 *Str);//订单号正确性检查
bool Group_Num_Check(u8 *Str);//柜组号正确性检查
bool Pwd_Check(void);//密码正确性检查
void Bar_Handler(void);
void Hmi_Handler(void);
void ReadData_WriteFlash(u8 *str,u8 length,u8 flash_ptr);

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
#endif




