/********************   One Artisan's Regret And Pride   ***********************
*
*	@File    	�� syn6288.c
*	@Brief   	�� syn6288�����ϳ�ģ�������
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

/****************************************************************************
*	@Function name	�� LV4CCD
*	@Description    �� ��ʼ��LV4CCD�������Լ�
*	@Author  		�� Blade@oarap.org
*	@Date			�� 2016-05
*	@param    		�� ��
*	@return			�� ��
*****************************************************************************/
u8 LV4CCD_Init()
{
  u8 i=0,n=0;	//iΪ���Դ���
  //u8 Get_Release[]={0x02,0x85,0x03,0x84};//��ȡ�汾�ţ��Դ����У��
  u8 Get_Status[] ={0x02,0x83,0x03,0x82};//��ȡ����״̬�֣��Դ����У��
	while(!((UART_DATA.BAR_REC.FLAG) && (UART_DATA.BAR_REC.DATA[UART_DATA.BAR_REC.COUNTER-3] == 0x24)))
	{
		while(n<=3)
			USART_Send(USART3,Get_Status[n++]);//��������
		Delay_ms(200);
		n=0;i++;
		if(i>=10)
		{
		  UART_DATA.BAR_REC.FLAG = false;
		  UART_DATA.BAR_REC.COUNTER = 0;
		  return BIT2;//���ش������
		}
	}
	UART_DATA.BAR_REC.FLAG = false;
	UART_DATA.BAR_REC.COUNTER = 0;
	return 0;
}
	

