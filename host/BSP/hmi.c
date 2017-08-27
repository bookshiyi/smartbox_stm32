/********************   One Artisan's Regret And Pride   ***********************
*
*	@File    	�� hmi.c
*	@Brief   	�� �˻���������������ļ�
*	@Author  	�� Blade@oarap.org
*	@Hardware	�� STM32F103RCT6
*	@Date		�� 2016-05
*	@Description�� ���������˻���������
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

/****************************************************************************
*	@Function name	�� �˻������ʼ��
*	@Description    �� ��ʾ��������
*	@Author  		�� Blade@oarap.org
*	@Date			�� 2016-05
*	@param    		�� ��
*	@return			�� ��
*****************************************************************************/
u8 Hmi_Init(u8 num)
{
  u8 touch[]={"----------�ᴥLOGO�����ϵͳ--"};
  u8 load []={"------     Loading     ------"};
  u8 clear[]={0xFF,0xFF};//����ָ��
  u8 self_check[]={0xA5,0X5A,0x01};
  u8 i=0;
  switch(num)
  {
  	case 0:
		Hmi_Send_reg(VERSION_PTR,REG_W,clear,0,2);//ͬ��������
		Hmi_Page_Switch(WELCOME);//��ʾ��0ҳ��
		Hmi_Send(WELCOME_CACHE,VAR_W,load,0,30);//��ʾloading
		break;
	case 1:
		Hmi_Send(WELCOME_CACHE,VAR_W,clear,0,2);//��ӭ��Ļ��������
		Hmi_Send(WELCOME_CACHE,VAR_W,self_check,0,2);//д���Լ������ڡ���ӭ�ﻺ������
		while(!((UART_DATA.HMI_REC.FLAG==true) && (UART_DATA.HMI_REC.DATA[6]==0X01) && (UART_DATA.HMI_REC.DATA[7]==0XA5) && (UART_DATA.HMI_REC.DATA[8]==0X5A)))
		{
			Hmi_Send(WELCOME_CACHE,VAR_R,self_check,2,1);//��ȡ����
			Delay_ms(200);
			i++;
			if(i>=10)
			{
			  UART_DATA.HMI_REC.FLAG=false;//��ս��ձ�־λ
			  return BIT3;//��ǰ�����������ش������
			}
		}
		Hmi_Send(WELCOME_CACHE,VAR_W,clear,0,2);//��ӭ��Ļ��������	  
		Hmi_Send(WELCOME_CACHE,VAR_W,touch,0,30);
		//UART_DATA.BAR_REC.FLAG=false;//��ս��ձ�־λ
		break;
  }
  UART_DATA.HMI_REC.FLAG=false;//��ս��ձ�־λ

  return 0;
}
/****************************************************************************
*	@Function name	�� ���ݷ��ͣ����û��洢λ�ý��в���
*	@Description    �� �����ݷ�װ��ָ����֡��ʽ���͸�HMI
*	@Author  		�� Blade@oarap.org
*	@Date			�� 2016-05
*	@param send_add �� ������ַ 
*	@param send_cmd �� �����д�Ĵ��������Ĵ�����д����������������ͼ����
*	@param send_data�� ���Ϊд�����send_dataΪ���ݣ����Ϊ��ȡ���send_dataΪ����ȡ�ĳ���
*	@param start_ptr�� ��ʼλ��
*	@param send_len �� ���͵ĳ��� 
*	@return			�� ��
*****************************************************************************/
void Hmi_Send(u16 send_add,u8 send_cmd,u8 *send_data,u8 start_ptr,u8 send_len)
{
  u8 i=0;
  u8 frame_head[6]={0x5A,0XA5};//��������֡
  frame_head[STR_LEN] = send_len + 3;//strlen(send_data)+3; //�������ݳ��ȵ�����֡
  frame_head[HMI_CMD] = send_cmd;  
  frame_head[ADD_H]   = send_add>>8;//С��ģʽ�Ĳ���
  frame_head[ADD_L]   = send_add;
  while(i!=6)
  	USART_Send(USART1,frame_head[i++]);
  for(i=start_ptr;i< start_ptr + send_len;i++)
  	USART_Send(USART1,send_data[i]);
}
/****************************************************************************
*	@Function name	�� ���ݷ��ͣ���ϵͳ�Ĵ������ж�ȡ����
*	@Description    �� �����ݷ�װ��ָ����֡��ʽ���͸�HMI
*	@Author  		�� Blade@oarap.org
*	@Date			�� 2016-05
*	@param send_add �� ������ַ 
*	@param send_cmd �� �����д�Ĵ��������Ĵ�����д����������������ͼ����
*	@param send_data�� ���Ϊд�����send_dataΪ���ݣ����Ϊ��ȡ���send_dataΪ����ȡ�ĳ���
*	@param start_ptr�� ��ʼλ��
*	@param send_len �� ���͵ĳ��� 
*	@return			�� ��
*****************************************************************************/
void Hmi_Send_reg(u16 send_add,u8 send_cmd,u8 *send_data,u8 start_ptr,u8 send_len)
{
  u8 i=0;
  u8 frame_head[5]={0x5A,0XA5};//��������֡
  frame_head[STR_LEN] = send_len + 2;//strlen(send_data)+3; //�������ݳ��ȵ�����֡
  frame_head[HMI_CMD] = send_cmd;  
  frame_head[ADD]     = send_add;
  while(i!=5)
  	USART_Send(USART1,frame_head[i++]);
  for(i=start_ptr;i< start_ptr + send_len;i++)
  	USART_Send(USART1,send_data[i]);
}
/****************************************************************************
*	@Function name	�� �˻�����ҳ���л�
*	@Description    �� 
*	@Author  		�� Blade@oarap.org
*	@Date			�� 2016-05
*	@param    (u8)id�� ָ����ҳ���
*	@return			�� ��
*****************************************************************************/
void Hmi_Page_Switch(u8 id)
{
		u8 pic_id[]={0x00,0x00};
		
		pic_id[1] =id;
    Hmi_Send_reg(PIC_ID_PTR,REG_W,pic_id,0,2);//��ʾ��0ҳ��
}
/*
void Hmi_Read_reg(u16 send_add,u8 send_cmd,u8 read_len,u8 start_ptr,u8 send_len)
{
  u8 i=0;
  u8 frame_head[5]={0x5A,0XA5};//��������֡
  frame_head[STR_LEN] = send_len + 2;//strlen(send_data)+3; //�������ݳ��ȵ�����֡
  frame_head[HMI_CMD] = send_cmd;  
  frame_head[ADD]     = send_add;
  while(i!=5)
  	USART_Send(USART1,frame_head[i++]);
  for(i=start_ptr;i< start_ptr + send_len;i++)
  	USART_Send(USART1,send_data[i]);
}*/
/****************************************************************************
*	@Function name	�� �˻������RTC��ȡ
*	@Description    �� 
*	@Author  		�� Blade@oarap.org
*	@Date			�� 2016-05
*	@param    (u8)id�� ָ����ҳ���
*	@return			�� ��
*****************************************************************************/
u8 Hmi_RTC_Read()//0x20Ϊ���� 0x24Ϊʱ�� ����ʼ��ַ
{//��ʽΪ���� �� �� �� ʱ �� �� 
	/*u8 Date_Len[]={0x03};//���ڵ����ݳ���Ϊ3	��0x20��ʼ
	u8 Time_Len[]={0x03};//ʱ������ݳ���Ϊ3 ��0x24��ʼ*/
	u8 RTC_Len[]={0x07};//��0x20��ʼ
	u8 i=10;

    Hmi_Send_reg(RTC_PTR,REG_R,RTC_Len,0,1);
	while((!(UART_DATA.HMI_REC.FLAG)) && (i--))
	{
		Delay_ms(10);//һ���ȴ���100ms
		if(i==1)
		{
		  UART_DATA.HMI_REC.FLAG=false;
		  UART_DATA.HMI_REC.COUNTER=0;
		  return BIT5;
		}
	}
	UART_DATA.HMI_REC.FLAG=false;
	UART_DATA.HMI_REC.COUNTER=0;
	return 0;
}
