/********************   One Artisan's Regret And Pride   ***********************
*
*	@File    	�� usart.c
*	@Brief   	�� 
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

UART_Data_TypeDef UART_DATA={{{false},{0},{0}},{{false},{0},{0}},{{false},{0},{0}},{{false},{0},{0}}};//��ʼ�� UART_DATA�ṹ��


//=============================================================================
//��������:USART1_Config
//���ܸ�Ҫ:USART1/GPIO ����,����ģʽ���á�9600-8-N-1
//����˵��:��
//��������:��
//=============================================================================
void USART1_Config(void)
{

  GPIO_InitTypeDef  GPIO_InitStructure;	
  USART_InitTypeDef USART_InitStructure;  //���崮�ڳ�ʼ���ṹ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
 
  /*USART1_TX ->PA9*/			
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	       //ѡ�д���Ĭ������ܽ�         
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�������������� 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����ܽ�2��ģʽ  
  GPIO_Init(GPIOA, &GPIO_InitStructure);           //���ú������ѽṹ�����������г�ʼ��		   
  /*USART1_RX ->PA10*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	        
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  /*����ͨѶ��������*/
  USART_InitStructure.USART_BaudRate = 57600; //������
  USART_InitStructure.USART_WordLength = USART_WordLength_8b; //����λ8λ
  USART_InitStructure.USART_StopBits = USART_StopBits_1;	//ֹͣλ1λ
  USART_InitStructure.USART_Parity = USART_Parity_No;		//У��λ ��
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��������
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//ʹ�ܽ��պͷ�������
   //�����ϸ���ֵ�Ľṹ�����⺯��USART_Init���г�ʼ��
  USART_Init(USART1, &USART_InitStructure); 
  USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//ʹ�ܴ����жϽ���
  USART_Cmd(USART1, ENABLE);//����USART1��ע��������RCC_APB2PeriphClockCmd()���õ�����
}
//=============================================================================
//��������:USART2_Config
//���ܸ�Ҫ:USART2/GPIO ����,����ģʽ���á�9600-8-N-1
//����˵��:��
//��������:��
//=============================================================================
void USART2_Config(void)
{	
  GPIO_InitTypeDef GPIO_InitStructure;	
  USART_InitTypeDef USART_InitStructure;  //���崮�ڳ�ʼ���ṹ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
 
  /*USART2_TX ->PA2*/			
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	       //ѡ�д���Ĭ������ܽ�         
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�������������� 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����ܽ�2��ģʽ  
  GPIO_Init(GPIOA, &GPIO_InitStructure);           //���ú������ѽṹ�����������г�ʼ��		   
  /*USART2_RX ->PA3*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	        
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  /*����ͨѶ��������*/
  USART_InitStructure.USART_BaudRate = 9600; //������
  USART_InitStructure.USART_WordLength = USART_WordLength_8b; //����λ8λ
  USART_InitStructure.USART_StopBits = USART_StopBits_1;	//ֹͣλ1λ
  USART_InitStructure.USART_Parity = USART_Parity_No;		//У��λ ��
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��������
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//ʹ�ܽ��պͷ�������
   //�����ϸ���ֵ�Ľṹ�����⺯��USART_Init���г�ʼ��
  USART_Init(USART2, &USART_InitStructure); 
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);//ʹ�ܴ����жϽ���
  USART_Cmd(USART2, ENABLE);//����USART1��ע��������RCC_APB2PeriphClockCmd()���õ�����
}
//=============================================================================
//��������:USART3_Config
//���ܸ�Ҫ:USART3/GPIO ����,����ģʽ���á�9600-8-N-1
//����˵��:��
//��������:��
//=============================================================================
void USART3_Config(void)//LV4����CCD
{	
  GPIO_InitTypeDef GPIO_InitStructure;	
  USART_InitTypeDef USART_InitStructure;  //���崮�ڳ�ʼ���ṹ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
 
  /*USART3_TX ->PB10*/			
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	       //ѡ�д���Ĭ������ܽ�         
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�������������� 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����ܽ�9��ģʽ  
  GPIO_Init(GPIOB, &GPIO_InitStructure);           //���ú������ѽṹ�����������г�ʼ��		   
  /*USART3_RX ->PB11*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	        
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  /*����ͨѶ��������*/
  USART_InitStructure.USART_BaudRate = 9600; //������
  USART_InitStructure.USART_WordLength = USART_WordLength_8b; //����λ8λ
  USART_InitStructure.USART_StopBits = USART_StopBits_1;	//ֹͣλ1λ
  USART_InitStructure.USART_Parity = USART_Parity_No;		//У��λ ��
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��������
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//ʹ�ܽ��պͷ�������
   //�����ϸ���ֵ�Ľṹ�����⺯��USART_Init���г�ʼ��
  USART_Init(USART3, &USART_InitStructure); 
  USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);//ʹ�ܴ����жϽ���
  USART_Cmd(USART3, ENABLE);//����USART3��ע��������RCC_APB2PeriphClockCmd()���õ�����
}
//=============================================================================
//��������:UART4_Config
//���ܸ�Ҫ:UART4/GPIO ����,����ģʽ���á�9600-8-N-1
//����˵��:��
//��������:��
//=============================================================================
void UART4_Config(void)//SYM6288
{	
  GPIO_InitTypeDef GPIO_InitStructure;	
  USART_InitTypeDef USART_InitStructure;  //���崮�ڳ�ʼ���ṹ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
  
  /*UART4_TX ->PC10*/			
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	   //ѡ�д���Ĭ������ܽ�         
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�������������� 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����ܽ�9��ģʽ  
  GPIO_Init(GPIOC, &GPIO_InitStructure);           //���ú������ѽṹ�����������г�ʼ��		   
  /*UART4_RX ->Pc11*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	        
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  /*����ͨѶ��������*/
  USART_InitStructure.USART_BaudRate = 9600; //������
  USART_InitStructure.USART_WordLength = USART_WordLength_8b; //����λ8λ
  USART_InitStructure.USART_StopBits = USART_StopBits_1;	//ֹͣλ1λ
  USART_InitStructure.USART_Parity = USART_Parity_No;		//У��λ ��
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��������
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//ʹ�ܽ��պͷ�������
   //�����ϸ���ֵ�Ľṹ�����⺯��UART_Init���г�ʼ��
  USART_Init(UART4, &USART_InitStructure); 
  USART_ITConfig(UART4,USART_IT_RXNE,ENABLE);//ʹ�ܴ����жϽ���
  USART_Cmd(UART4, ENABLE);//����UART5��ע��������RCC_APB2PeriphClockCmd()���õ�����
}
//=============================================================================
//��������:UART5_Config
//���ܸ�Ҫ:UART5/GPIO ����,����ģʽ���á�9600-8-N-1
//����˵��:��
//��������:��
//=============================================================================
void UART5_Config(void)//gsm900a
{	
  GPIO_InitTypeDef GPIO_InitStructure;	
  USART_InitTypeDef USART_InitStructure;  //���崮�ڳ�ʼ���ṹ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);
  
  /*UART5_TX ->PC12*/			
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;	   //ѡ�д���Ĭ������ܽ�         
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�������������� 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����ܽ�9��ģʽ  
  GPIO_Init(GPIOC, &GPIO_InitStructure);           //���ú������ѽṹ�����������г�ʼ��		   
  /*UART5_RX ->PD2*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	        
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  /*����ͨѶ��������*/
  USART_InitStructure.USART_BaudRate = 9600; //������
  USART_InitStructure.USART_WordLength = USART_WordLength_8b; //����λ8λ
  USART_InitStructure.USART_StopBits = USART_StopBits_1;	//ֹͣλ1λ
  USART_InitStructure.USART_Parity = USART_Parity_No;		//У��λ ��
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��������
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//ʹ�ܽ��պͷ�������
   //�����ϸ���ֵ�Ľṹ�����⺯��UART_Init���г�ʼ��
  USART_Init(UART5, &USART_InitStructure); 
  USART_ITConfig(UART5,USART_IT_RXNE,ENABLE);//ʹ�ܴ����жϽ���
  USART_Cmd(UART5, ENABLE);//����UART5��ע��������RCC_APB2PeriphClockCmd()���õ�����
}
//=============================================================================
//��������:NVIC_Configuration
//���ܸ�Ҫ:�ж���������
//����˵��:��
//��������:��
//=============================================================================
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure; 
  /* Configure the NVIC Preemption Priority Bits */  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  
  /* Enable the USARTy Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;	 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;	 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;	 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;	 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
//=============================================================================
//��������:USART_Send
//���ܸ�Ҫ:��ָ����USART�������ݲ��һ��еȴ����ܣ�������Ϻ��Զ���ձ�־λ
//����˵��:��
//��������:��
//=============================================================================
void USART_Send(USART_TypeDef* USARTx,uint16_t data)
{
	USART_SendData(USARTx,data);
	while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)!=SET);//�ȴ���������жϱ�־λ	
	 USART_ClearFlag(USARTx,USART_FLAG_TC);
}
//=============================================================================
//��������:USART1_IRQHandler
//���ܸ�Ҫ:
//����˵��:��
//��������:��
//=============================================================================
void USART1_IRQHandler(void)//HMI�ӿ�
{
  //����DWIN��Ļ�����ݴ洢��ʱ����֡�ṹ�������ֽ�֡ͷ+һ���ֽ����ݳ��ȣ������������е����ݣ�+���ֽڵ�ַ+����
  
  
  //ASCII¼�뷵�ص������ݵ�ASCIIֵ 
  //Ӧ�����ݵĽṹ�����ֽ�֡ͷ+һ���ֽ� ��֡���ݳ��ȣ������������е�����,�ֽڳ��ȣ�+���ֽڵ�ַ+���˼���/*��*/������+����
  //������صı������������ֽڣ����ڱ������������FF�����һ��0x00.����֮�⣬����֡���ֽڳ���Ϊ������
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
  { 		
	UART_DATA.HMI_REC.DATA[UART_DATA.HMI_REC.COUNTER++] = USART_ReceiveData(USART1);//
	if(UART_DATA.HMI_REC.COUNTER > 2)//�����յ������ֽ�����ʱ �����ݶ�Ӧ��֡�ĳ��ȣ�����֮�ⲻ���ڽ��������ֽڵ�ʱ����ж����ݵĳ��ȣ���Ϊ�������ֽڻ�û��ˢ�£�������һ�ε����ݣ��޷������жϣ�
	{
		if(UART_DATA.HMI_REC.COUNTER>=UART_DATA.HMI_REC.DATA[2]+3 || (UART_DATA.HMI_REC.COUNTER>=30))  
		{	//������������֡���ͽ������ν���
		  if((UART_DATA.HMI_REC.DATA[0]==0x5A)&&(UART_DATA.HMI_REC.DATA[1]==0xA5))//����֡����
			  UART_DATA.HMI_REC.FLAG=true;//����֡����������������
		  UART_DATA.HMI_REC.COUNTER=0;//׼���´ν���
		}
	}
  } 
  USART_ClearITPendingBit(USART1, USART_IT_RXNE);
}
//=============================================================================
//��������:USART2_IRQHandler
//���ܸ�Ҫ:
//����˵��:��
//��������:��
//=============================================================================
void USART2_IRQHandler(void)//δʹ��
{
  u8 c;
  if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
  { 	
    c=USART2->DR;
    printf("%c",c);    //�����ܵ�������ֱ�ӷ��ش�ӡ
  } 
  
  USART_ClearITPendingBit(USART2, USART_IT_RXNE);
}

//=============================================================================
//��������:USART3_IRQHandler
//���ܸ�Ҫ:
//����˵��:��
//��������:��
//=============================================================================
void USART3_IRQHandler(void)//����ccd_LV4//ָ��+2ΪУ��͵�λ�ã����У�飩
{
  if(USART_GetITStatus(USART3,USART_IT_RXNE) != RESET)
  { 	
	UART_DATA.BAR_REC.DATA[UART_DATA.BAR_REC.COUNTER++]= USART_ReceiveData(USART3);
	if((UART_DATA.BAR_REC.DATA[UART_DATA.BAR_REC.COUNTER-1]=='\n') && (UART_DATA.BAR_REC.COUNTER==ORDER_LEN+1))//���ֹͣλ��cr \r 0x0d ��lf \n 0x0a�� \r�ص����� \n��һ��	
	{
	  UART_DATA.BAR_REC.FLAG=true;//������һ֡���ݣ��ø߱�־λ
	  UART_DATA.BAR_REC.COUNTER=0;//����ַ���������
	}
	if(UART_DATA.BAR_REC.DATA[UART_DATA.BAR_REC.COUNTER-2]==0x03)//˵�����յ�ֹͣ����
	{
	  UART_DATA.BAR_REC.FLAG=true;//��Ҫ���ڿ����Լ�
	  //UART_DATA.BAR_REC.COUNTER=0;//����ַ���������
	}
	if(UART_DATA.BAR_REC.COUNTER>=30)//2016-05-31,��ָֹ�����
		UART_DATA.BAR_REC.COUNTER=0;
  } 	
  USART_ClearITPendingBit(USART3, USART_IT_RXNE);
}
//=============================================================================
//��������:UART4_IRQHandler
//���ܸ�Ҫ:
//����˵��:��
//��������:��
//=============================================================================
void UART4_IRQHandler(void)//SYN6288
{
  if(USART_GetITStatus(UART4,USART_IT_RXNE) != RESET)
  { //ÿ�λش������ݶ�����һ���ֽڴ�С
	UART_DATA.SYN_REC.DATA[UART_DATA.SYN_REC.COUNTER]= USART_ReceiveData(UART4);
	if((UART_DATA.SYN_REC.DATA[UART_DATA.SYN_REC.COUNTER] & 0X40) == 0X40)
	{//˵�����ݵĸ�ʽ����ȷ��
		UART_DATA.SYN_REC.FLAG = true;
	}
	if(UART_DATA.SIM_REC.COUNTER>=30)//2016-05-31,��ָֹ�����
		UART_DATA.SIM_REC.COUNTER=0;
  } 
  //
  USART_ClearITPendingBit(UART4, USART_IT_RXNE);
}
//=============================================================================
//��������:UART5_IRQHandler
//���ܸ�Ҫ:
//����˵��:��
//��������:��
//=============================================================================
void UART5_IRQHandler(void)//SIM900A
{
  //static u8 FLAG_VALUE_COUNTER = BIT1;//��̬�ֲ�������ÿ�ε��ö�����ʹ����һ�ε���ֵ������ֵ��������������¼\r\n�ĸ���
  if(USART_GetITStatus(UART5,USART_IT_RXNE) != RESET)
  { 	
	UART_DATA.SIM_REC.DATA[UART_DATA.SIM_REC.COUNTER++] = USART_ReceiveData(UART5);//��COUNTER��λ������ܹ���ѯ������\r\n˵�����ݽ������
	if((UART_DATA.SIM_REC.DATA[UART_DATA.SIM_REC.COUNTER-2]=='\r')&&(UART_DATA.SIM_REC.DATA[UART_DATA.SIM_REC.COUNTER-1]=='\n'))
	{
		UART_DATA.SIM_REC.FLAG = true;//���ܵ�һ�������ַ�
		//UART_DATA.SIM_REC.COUNTER = 0;
	}
	if(UART_DATA.SIM_REC.COUNTER>=30)//2016-05-31,��ָֹ�����
		UART_DATA.SIM_REC.COUNTER=0;
	
	  /*if(UART_DATA.SIM_REC.COUNTER>=2)//�����յ���ڶ��ֽ����ݺ�
	{
		if((UART_DATA.SIM_REC.DATA[0]=='A')&&(UART_DATA.SIM_REC.DATA[1]=='T'))//˵��֡ͷ���룬���һ֡���ݵĽ���
		{
			if((UART_DATA.SIM_REC.DATA[UART_DATA.SIM_REC.COUNTER-2]=='\r')&&(UART_DATA.SIM_REC.DATA[UART_DATA.SIM_REC.COUNTER-1]=='\n'))
			{
				FLAG_VALUE_COUNTER <<= 1;
				if(FLAG_VALUE_COUNTER == BIT4)//˵������������\r\n,����������֡
				{
					UART_DATA.SIM_REC.FLAG = true;//������һ֡���ݣ��ø߱�־λ 
					FLAG_VALUE_COUNTER = BIT1;
					UART_DATA.SIM_REC.COUNTER = 0;
				}
			}
		}
		else//֡ͷ����
		  UART_DATA.SIM_REC.COUNTER = 0;//��ռ���ָ�룬���½���
	}*/

	/*if((UART_DATA.SIM_REC.DATA[UART_DATA.SIM_REC.COUNTER-1]=='\n')&&(UART_DATA.SIM_REC.COUNTER >= 2))//���ܺ͵�һ��\n��ͻ
	  //���ֹͣλ��cr \r 0x0d ��lf \n 0x0a�� \r�ص����� \n��һ��	
	{
	  if((UART_DATA.SIM_REC.DATA[0] == '\r') && (UART_DATA.SIM_REC.DATA[1] == '\n'))
	  	UART_DATA.SIM_REC.FLAG=true;//������һ֡���ݣ��ø߱�־λ 
	  UART_DATA.SIM_REC.COUNTER = 0;
	}*/
  } 
  USART_ClearITPendingBit(UART5, USART_IT_RXNE);	
}






















