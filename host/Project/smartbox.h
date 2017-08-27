#ifndef __SMARTBOX_H
#define __SMARTBOX_H
/********************   One Artisan's Regret And Pride   ***********************
*
*	@File    	�� smartbox.h
*	@Brief   	�� ���ܹ�ͷ�ļ�
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

#include "includes.h"

//���徲̬����
#define BUF 		0 					//��0�����ݿ�����Ϊ������
#define BOX_NUM 	20 					//����������Ŀ
#define ORDER_LEN 	15					//���嶩���ŵĳ���
#define PWD_LEN 	6 					//���������֤��ĳ���
#define HIGH 		0					//�߰���Ϊ�����0��
#define LOW	 		1					//��λ��Ϊ�����1��

//����ϵͳ���������ṹ������
typedef struct 
{
	struct tm Cur_RTC;					//ϵͳ����ʱ���ʵʱʱ�ӣ�����STM32Ƭ�ϵ�RTC
	u8 	Admin_Pwd[6];					//����Ա����
	u8  Group_Num[3];				//���
	u8  Contact_Inf[11];				//��ϵ��ʽ
	u8	Latest_Time[4];					//����ȡ��
}SmartBox_Arg_TypeDef;

//����ϵͳ���ݿ�ṹ������
typedef struct 
{
  u8  	 Box_Ascii[2];		//���źŵ�ASCIIֵ
  bool   usable;	//�Ƿ����
  u8	 pwd[PWD_LEN];	//����
}SmartBox_DataBase_TypeDef;

//������ʷ��¼���ݿ�
typedef struct 
{
  u8	 Day;		//��
  u8	 Hour;		//ʱ
  u8	 Min;		//��
  u8	 Sec;		//��
  u8	 Order[ORDER_LEN]; //������
  u8  	 Box;		//���ź�
  u8	 Pwd[PWD_LEN];	//����
}Hist_DataBase_TypeDef;

extern SmartBox_DataBase_TypeDef 	SmartBox_DataBase[];
extern struct tm Cur_RTC;//ϵͳ����ʱ���ʵʱʱ�ӣ�����STM32Ƭ�ϵ�RTC
extern SmartBox_Arg_TypeDef		SmartBox_Arg;//��������ṹ��
/*************************��������********************************************/
void Struct_Init(void);//��ʼ�����ݿ�
bool Pwd_Create(u8 box_num);//�����֤�����ɺ���
bool send_pwd2_pc(void);//�����漴��֤�뵽��λ��
u8 Box_Find(void);
u8 Pwd_Match(u8 *Str);
bool Admin_Match(u8 *Str);
u8 *Num2Ascii(u8 num);

void Self_Check(void);//�Լ����
void Sys_Init(void);
void Main_Process(void);//������
void LCD12864_Disp_Frame(void);
void Refresh_Usable_Num(void);
//void Self_Check_Code_Disp(u8 Code);

bool Order_Check(u8 *Str);//��������ȷ�Լ��
bool Group_Num_Check(u8 *Str);//�������ȷ�Լ��
bool Pwd_Check(void);//������ȷ�Լ��
void Bar_Handler(void);
void Hmi_Handler(void);
void ReadData_WriteFlash(u8 *str,u8 length,u8 flash_ptr);

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
#endif




