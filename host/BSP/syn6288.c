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


char SYN_StopCom[]		={0xFD,0X00,0X02,0X02,0XFD};//ֹͣ�ϳ�
char SYN_SuspendCom[]	={0XFD,0X00,0X02,0X03,0XFC};//��ͣ�ϳ�
char SYN_RecoverCom[]	={0XFD,0X00,0X02,0X04,0XFB};//�ָ��ϳ�
char SYN_ChackCom[]		={0XFD,0X00,0X02,0X21,0XDE};//״̬��ѯ
char SYN_PowerDownCom[]	={0XFD,0X00,0X02,0X88,0X77};//����POWER DOWN ״̬����
char SYN_Baud9600[]		={0XFD,0X00,0X03,0X31,0XCF};//������9600
char SYN_Baud19200[]	={0xFD,0x00,0x03,0x31,0x01,0xCE};

/****************************************************************************
*	@Function name	�� YS-SYN6288 �ı��ϳɺ���
*	@Description    �� ���ͺϳ��ı���SYN6288оƬ���кϳɲ���
*	@Author  		�� Blade@oarap.org
*	@Date			�� 2016-05
*	@param    Music �� 0�ޱ������֡�1-15����ر�������
*	@param  *HZdata �� �ı�ָ�����
*	@return			�� ��
*****************************************************************************/
void SYN_FrameInfo(uint8_t Music,char *HZdata)
{ 
		 unsigned  char  headOfFrame[5];
         unsigned  char  length;  
		 unsigned  char  ecc  = 0;  			//����У���ֽ�
	     unsigned  int   i=0; 
		 
		 length =strlen(HZdata); 			//��Ҫ�����ı��ĳ���          
		 headOfFrame[0] = 0xFD ; 			//����֡ͷFD
		 headOfFrame[1] = 0x00 ; 			//�������������ȵĸ��ֽ�
		 headOfFrame[2] = length + 3; 		//�������������ȵĵ��ֽ�
		 headOfFrame[3] = 0x01 ; 			//���������֣��ϳɲ�������
		 headOfFrame[4] = 0x01 | (Music<<4);//������������������ʽΪGBK

		 //while(BUSY);						//��æ�ȴ������BUSY����2016-05-30
		 for(i = 0; i<5; i++)   				//���η��͹���õ�5��֡ͷ�ֽ�
	     {  
	         ecc=ecc^(headOfFrame[i]); 		//�Է��͵��ֽڽ������У��	 
			 USART_Send(UART4,headOfFrame[i]);
		 }

	   	 for(i = 0; i<length; i++)   		//���η��ʹ��ϳɵ��ı�����
	     {  
	         ecc=ecc^(HZdata[i]); 				//�Է��͵��ֽڽ������У��	
			 USART_Send(UART4,HZdata[i]);
		 }

   		USART_Send(UART4,ecc);
//Ӧ����ش����ּ��ģ��൱����������·���������ش��Ļ��ƣ�������Ӧ�ò������û�и��ܵ�
		
}
/****************************************************************************
*	@Function name	�� syn6288����
*	@Description    �� �������SYN6288
*	@Author  		�� Blade@oarap.org
*	@Date			�� 2016-05
*	@param    		�� ��
*	@return			�� ��
*****************************************************************************/
void SYN_Set(char *CMD)
{
  u8 i;
  for(i=0;i<5;i++)
	USART_Send(UART4,CMD[i]);
}
/****************************************************************************
*	@Function name	�� syn6288��BUSY���ų�ʼ��
*	@Description    �� 
*	@Author  		�� Blade@oarap.org
*	@Date			�� 2016-06
*	@param    		�� ��
*	@return			�� ��
*****************************************************************************/
void SYN_GPIO_Init()
{	
  	//����һ��GPIO_InitTypeDef ���͵Ľṹ��
	GPIO_InitTypeDef  GPIO_InitStructure;
	//����GPIOC������ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	//ѡ��Ҫ�õ�GPIO����		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	//��������ģʽΪ��������ģʽ				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 
	//���ÿ⺯������ʼ��GPIO
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
	
}
/****************************************************************************
*	@Function name	�� syn6288��ʼ��
*	@Description    �� ��ʼ�������ʣ����ͻ�ӭ��
*	@Author  		�� Blade@oarap.org
*	@Date			�� 2016-05
*	@param    		�� ��
*	@return			�� ��
*****************************************************************************/
u8 SYN_Init()
{//d-���ȫ�ֱ��� v-ǰ�����ȼ����16 m-�������ȼ����16 t-���ٵȼ����6 y-1�Ķ��� o-�ı��ʶ���ʽ
  u8 i;	
  char init_param[]= "[d][v10][m2][t6][n2][y1][o0]";//ǿ�ư�����ֵ�ķ�ʽ�ϳ�,1����yao

	SYN_GPIO_Init();
  	SYN_Set(SYN_Baud9600);
	Delay_ms(100);	
	SYN_FrameInfo(0,init_param);
	Delay_ms(100);	
	while(!((UART_DATA.SYN_REC.DATA[0]==0X41) || (UART_DATA.SYN_REC.DATA[0]==0X4E) || (UART_DATA.SYN_REC.DATA[0]==0X4F)))
	{
		SYN_FrameInfo(1,"��");//SYN_FrameInfo(1,"��ӭʹ�������Ͳ�ϵͳ");
		Delay_ms(200);
		i++;
		if(i>=10)
		{
		  UART_DATA.SYN_REC.FLAG = false;
		  return BIT1;//���ش������
		}
	}
	UART_DATA.SYN_REC.FLAG = false;
	SYN_Set(SYN_StopCom);
	Delay_ms(300);
	SYN_FrameInfo(1,"��ӭʹ�������Ͳ�ϵͳ");
	//while(BUSY);
	return 0;
}
/****************************************************************************
*	@Function name	�� ������ʾ�ϳ�
*	@Description    �� ��ʾ�û������Ƿ���ȷ�����ź�
*	@Author  		�� Blade@oarap.org
*	@Date			�� 2016-05
*	@param  box_num	�� �͹����ڵ����ݿ�λ��
*	@return			�� ��
*****************************************************************************/
void SYN_Send(u8 Box_Num)//��ڲ���Ϊ����λ��
{
	char Voice_Buf[] = "nn�������Ѵ�";
	Voice_Buf[0] = SmartBox_DataBase[Box_Num].Box_Ascii[0];//ת����ASCII�����ʽ
	Voice_Buf[1] = SmartBox_DataBase[Box_Num].Box_Ascii[1];
	if(Box_Num)
		SYN_FrameInfo(0,Voice_Buf);
	else
		SYN_FrameInfo(0,"�������");
}
