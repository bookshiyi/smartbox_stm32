/********************   One Artisan's Regret And Pride   ***********************
*
*	@File    	�� sim900a.h
*	@Brief   	�� SIM900A�������ļ�
*	@Author  	�� Blade@oarap.org
*	@Hardware	�� STM32F103RCT6
*	@Date		�� 2016-05
*	@Description�� 
*	@History	�� 
*
*	Rev1.0 
*		Date��2016-05-16
*		Author��Blade
*		Modification��
*
*--------------------Copyright (C) oarap.org 2016------------------------------
*----------------------All rights reserved-------------------------------------
*
********************          һ���������ź��뽾��          ********************/
/*sim900a������֡���͸�ʽ*/
//���ͣ�AT XX  \r\n
//���գ�<����ģ�������> \r\n  XXX \r\n 
//\r\n��ʾ<CR><LF>���س����У���ASCIIֵΪ0X0D\0X0A
#ifndef __SIM900A_H__
#define __SIM900A_H__	 

#include "includes.h"
//extern char message[];



#define PWD_START_POS 			166			//�ڶ������������ʼλ��
#define BOXNUM_START_POS 		122			//�ڶ����в͹�ŵ���ʼλ��
#define GROUP_NUM_START_POS		90			//��88��100
#define LATEST_TIME_START_POS	206			//��204��ʼ

u8 Sim900a_Init(void);

bool Sim900a_Str_Cmp(u8 *str1,u8 *str2,u8 str2_ptr);

void Sim900a_Send(char *send_data);

void Sim900a_Send_Len(char *send_data,u8 start_ptr,u8 send_len);

bool Sim900a_Send_Message(u8 *Order_Num,u8 Box_Num);













/*
#define swap16(x) (x&0XFF)<<8|(x&0XFF00)>>8		//�ߵ��ֽڽ����궨��

void sim900a_test(void);//sim900a�����Գ��� 
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
u8 sim900a_call_test(void);			//���Ų���
void sim900a_sms_read_test(void);	//�����Ų���
void sim900a_sms_send_test(void);	//�����Ų��� 
void sim900a_sms_ui(u16 x,u16 y);	//���Ų���UI���溯��
u8 sim900a_sms_test(void);			//���Ų���
void sim900a_mtest_ui(u16 x,u16 y);	//SIM900A������UI
u8 sim900a_gsminfo_show(u16 x,u16 y);//��ʾGSMģ����Ϣ*/
#endif





