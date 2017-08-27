/********************   One Artisan's Regret And Pride   ***********************
*
*	@File    	： sim900a.h
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
/*sim900a的数据帧发送格式*/
//发送：AT XX  \r\n
//接收：<发给模块的命令> \r\n  XXX \r\n 
//\r\n表示<CR><LF>即回车新行，其ASCII值为0X0D\0X0A
#ifndef __SIM900A_H__
#define __SIM900A_H__	 

#include "includes.h"
//extern char message[];



#define PWD_START_POS 			166			//在短信中密码的起始位置
#define BOXNUM_START_POS 		122			//在短信中餐柜号的起始位置
#define GROUP_NUM_START_POS		90			//从88到100
#define LATEST_TIME_START_POS	206			//从204开始

u8 Sim900a_Init(void);

bool Sim900a_Str_Cmp(u8 *str1,u8 *str2,u8 str2_ptr);

void Sim900a_Send(char *send_data);

void Sim900a_Send_Len(char *send_data,u8 start_ptr,u8 send_len);

bool Sim900a_Send_Message(u8 *Order_Num,u8 Box_Num);













/*
#define swap16(x) (x&0XFF)<<8|(x&0XFF00)>>8		//高低字节交换宏定义

void sim900a_test(void);//sim900a主测试程序 
void sim_send_sms(u8*phonenumber,u8*msg);
void sim_at_response(u8 mode);	
u8* sim900a_check_cmd(u8 *str);
u8 sim900a_send_cmd(u8 *cmd,u8 *ack,u16 waittime);
u8 sim900a_chr2hex(u8 chr);
u8 sim900a_hex2chr(u8 hex);
void sim900a_unigbk_exchange(u8 *src,u8 *dst,u8 mode);
void sim900a_load_keyboard(u16 x,u16 y,u8 **kbtbl);
void sim900a_key_staset(u16 x,u16 y,u8 keyx,u8 sta);
u8 sim900a_get_keynum(u16 x,u16 y);
u8 sim900a_call_test(void);			//拨号测试
void sim900a_sms_read_test(void);	//读短信测试
void sim900a_sms_send_test(void);	//发短信测试 
void sim900a_sms_ui(u16 x,u16 y);	//短信测试UI界面函数
u8 sim900a_sms_test(void);			//短信测试
void sim900a_mtest_ui(u16 x,u16 y);	//SIM900A主测试UI
u8 sim900a_gsminfo_show(u16 x,u16 y);//显示GSM模块信息*/
#endif





