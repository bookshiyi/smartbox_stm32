/********************   One Artisan's Regret And Pride   ***********************
*
*	@File    	�� lock.c
*	@Brief   	�� ������74ls164������
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
*	@Function name	�� ����������Ƭ74ls164����������������
*	@Description    �� ����GPIO��ر��������ֹ�ű���
*	@Author  		�� Blade@oarap.org
*	@Date			�� 2016-05
*	@param    		�� ��
*	@return			�� ��
*****************************************************************************/
void Decoder_GPIO_Config(void) //GPIOC.0-3
{	
    //����һ��GPIO_InitTypeDef ���͵Ľṹ��
    GPIO_InitTypeDef  GPIO_InitStructure;	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//ʹ��GPIO������ʱ��

    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 ;//ѡ��Ҫ�õ�GPIO����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //��������ģʽΪ�������ģʽ						 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//���������ٶ�Ϊ50MHZ         
    GPIO_Init(GPIOC, &GPIO_InitStructure);//���ÿ⺯������ʼ��GPIO
    //GPIO_SetBits(GPIOC,GPIO_Pin_5 | GPIO_Pin_4);//�ϵ���ø�����ʹ�ܶ˿ڣ��ر����������
	
	Decoder_Output(0x1f);//��32��Ŀ��Ϊ��ʼ��ʱ����ȷ�����ŵ�ƽ״̬����ֹ�̵��������
}
/****************************************************************************
*	@Function name	�� ���������
*	@Description    �� ������Ƭ74ls164������λ�����ķ�ʽ
*	@Author  		�� Blade@oarap.org
*	@Date			�� 2016-05
*	@param  PortVal	�� �͸�����������ֵ
*	@return			�� ��
*****************************************************************************/
void Decoder_Output(uint16_t PortVal)//PA0,PA1,PA2,PA3;Ϊ�Ͷ�  PA4 PA5ΪƬѡ��
{
/*
  PortVal |= 0xfff0;//Ԥ����������ת��Ϊ�Ͱ��ֽ�λ������ģʽ
  GPIOx->ODR &= PortVal;
  */
  if(!(PortVal&BIT4))//�����5λΪ�͡�˵��С��16��ʹ�ܵ�һƬ���õͣ����ض̵ڶ�Ƭ���øߣ�
  {
  	PortVal &= 0xFF0F;
	PortVal |= BIT5;
  }
  GPIOC->ODR = (GPIOC->ODR & 0xFFC0) | PortVal;//������λ�����ݷ�������Ĵ���
}
/****************************************************************************
*	@Function name	�� ���������
*	@Description    �� ��һ�����е����
*	@Author  		�� Blade@oarap.org
*	@Date			�� 2016-05
*	@param    		�� ��
*	@return			�� ��
*****************************************************************************/
void Lock_Test()
{
    u16 i;
	for(i=0;i<24;i++)
	{
		Decoder_Output(i);/*��&����������ֽ���FF*/
		Delay_ms(LOCK_DELAY);
	}
}
/****************************************************************************
*	@Function name	�� ����
*	@Description    �� ��ָ���͹�����ţ�����LOCK_DELAY��MS�ĸߵ�ƽ��
*	@Author  		�� Blade@oarap.org
*	@Date			�� 2016-05
*	@param  ��u16��i�� ��ָ����������1��ʼ��
*	@return			�� ��
*****************************************************************************/
void Lock_Open(u16 i)
{
	Decoder_Output(i-1);
	Delay_ms(LOCK_DELAY);
	Decoder_Output(0x1f);
}
