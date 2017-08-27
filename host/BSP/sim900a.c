/********************   One Artisan's Regret And Pride   ***********************
*
*	@File    	�� sim900a.c
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

#include "includes.h"

char Text_Message[]  = {"301083DC5305914D900130115BA25B98FF0C5C0F4E8C628A59165356653E57288FBD5B815DE57A0B59275B660030003000317EC4667A80FD67DC76840058005853F7991067DC4E2DFF0C53D699105BC67801005800580058005800580058FF0C4EC55F5365E500320031003A00300030524D670965483002"};
char Receiver[] = {"AT+CMGS=\"00310035003500360036003600310032003200300036\"\r\n"};

/****************************************************************************
*	@Function name	�� SIM900A��ʼ��
*	@Description    �� ͬ�������ʣ�//���ò�������ѯ״̬
*	@Author  		�� Blade@oarap.org
*	@Date			�� 2016-05
*	@param    		�� ��
*	@return			�� ��
*****************************************************************************/
u8 Sim900a_Init()
{
    //char buf[] = "AT\r\n";
	u8 i=0;//��¼��ʼ������
	
	UART_DATA.SIM_REC.FLAG = false;
	UART_DATA.SIM_REC.COUNTER = 0;//��λ����ָ��/������
	Sim900a_Send("AT+CFUN=1,1\r\n");
	Delay_ms(50);
	Sim900a_Send("AT+CFUN=1,1\r\n");//��������ָ��
	 while(!((UART_DATA.SIM_REC.FLAG) && (UART_DATA.SIM_REC.DATA[UART_DATA.SIM_REC.COUNTER-4]=='d') && (UART_DATA.SIM_REC.DATA[UART_DATA.SIM_REC.COUNTER-3]=='y')))
	 {//Call Ready
	   if(UART_DATA.SIM_REC.FLAG)
	   {
		 if(!((UART_DATA.SIM_REC.DATA[UART_DATA.SIM_REC.COUNTER-4]=='d') && (UART_DATA.SIM_REC.DATA[UART_DATA.SIM_REC.COUNTER-3]=='y')))
		 {
		   UART_DATA.SIM_REC.FLAG = false;
			UART_DATA.SIM_REC.COUNTER = 0;//��λ����ָ��/������
		 }
	   }
		Delay_ms(200);//while(!(UART_DATA.SIM_REC.FLAG);
		i++;
		if(i>=100)
		{
		  	UART_DATA.SIM_REC.FLAG = false;
			UART_DATA.SIM_REC.COUNTER = 0;//��λ����ָ��/������
			return BIT0;
		}
	 }
	i=0;
	UART_DATA.SIM_REC.FLAG = false;
	UART_DATA.SIM_REC.COUNTER = 0;//��λ����ָ��/������
	
	while(!((UART_DATA.SIM_REC.FLAG) && (UART_DATA.SIM_REC.DATA[UART_DATA.SIM_REC.COUNTER-4]=='O') && (UART_DATA.SIM_REC.DATA[UART_DATA.SIM_REC.COUNTER-3]=='K')))
	{//�ڴ˵ȴ��������ʼ�����ɹ���������
		Sim900a_Send("AT\r\n");
		Delay_ms(200);//while(!(UART_DATA.SIM_REC.FLAG);
		i++;
		if(i>=10)
		{
		  	UART_DATA.SIM_REC.FLAG = false;
			UART_DATA.SIM_REC.COUNTER = 0;//��λ����ָ��/������
			return BIT0;
		}
	}
	i=0;
	UART_DATA.SIM_REC.FLAG = false;
	UART_DATA.SIM_REC.COUNTER = 0;//��λ����ָ��/������
	
	
	while(!((UART_DATA.SIM_REC.FLAG) && (UART_DATA.SIM_REC.DATA[UART_DATA.SIM_REC.COUNTER-4]=='O') && (UART_DATA.SIM_REC.DATA[UART_DATA.SIM_REC.COUNTER-3]=='K')))
	{//ɾ�����ж���
		Sim900a_Send("AT+CMGD=1,4\r\n");
		Delay_ms(200);//while(!(UART_DATA.SIM_REC.FLAG);
		i++;
		if(i>=10)
		{
		  	UART_DATA.SIM_REC.FLAG = false;
			UART_DATA.SIM_REC.COUNTER = 0;//��λ����ָ��/������
			return BIT0;
		}
	}
	i=0;
	UART_DATA.SIM_REC.FLAG = false;
	UART_DATA.SIM_REC.COUNTER = 0;//��λ����ָ��/������

	while(!((UART_DATA.SIM_REC.FLAG) && (UART_DATA.SIM_REC.DATA[UART_DATA.SIM_REC.COUNTER-4]=='O') && (UART_DATA.SIM_REC.DATA[UART_DATA.SIM_REC.COUNTER-3]=='K')))
	{//�ڴ˵ȴ��������ʼ�����ɹ���������
		Sim900a_Send("AT+CSCS=\"UCS2\"\r\n");
		Delay_ms(200);//while(!(UART_DATA.SIM_REC.FLAG);
		i++;
		if(i>=10)
		{
		  	UART_DATA.SIM_REC.FLAG = false;
			UART_DATA.SIM_REC.COUNTER = 0;//��λ����ָ��/������
			return BIT0;
		}
	}
	i=0;
	UART_DATA.SIM_REC.FLAG = false;
	UART_DATA.SIM_REC.COUNTER = 0;//��λ����ָ��/������
	
	while(!((UART_DATA.SIM_REC.FLAG) && (UART_DATA.SIM_REC.DATA[UART_DATA.SIM_REC.COUNTER-4]=='O') && (UART_DATA.SIM_REC.DATA[UART_DATA.SIM_REC.COUNTER-3]=='K')))
	{//�ڴ˵ȴ��������ʼ�����ɹ���������
		Sim900a_Send("AT+CSMP=17,167,2,25\r\n");
		Delay_ms(200);//while(!(UART_DATA.SIM_REC.FLAG);
		i++;
		if(i>=10)
		{
		  	UART_DATA.SIM_REC.FLAG = false;
			UART_DATA.SIM_REC.COUNTER = 0;//��λ����ָ��/������
			return BIT0;
		}
	}
	i=0;
	UART_DATA.SIM_REC.FLAG = false;
	UART_DATA.SIM_REC.COUNTER = 0;//��λ����ָ��/������
		
	while(!((UART_DATA.SIM_REC.FLAG) && (UART_DATA.SIM_REC.DATA[UART_DATA.SIM_REC.COUNTER-4]=='O') && (UART_DATA.SIM_REC.DATA[UART_DATA.SIM_REC.COUNTER-3]=='K')))
	{//�ڴ˵ȴ��������ʼ�����ɹ���������
		Sim900a_Send("AT+CMGF=1\r\n");
		Delay_ms(200);//while(!(UART_DATA.SIM_REC.FLAG);
		i++;
		if(i>=10)
		{
		  	UART_DATA.SIM_REC.FLAG = false;
			UART_DATA.SIM_REC.COUNTER = 0;//��λ����ָ��/������
			return BIT0;
		}
	}
	i=0;
	UART_DATA.SIM_REC.FLAG = false;
	UART_DATA.SIM_REC.COUNTER = 0;//��λ����ָ��/������

	return 0;
	
}
/****************************************************************************
*	@Function name	�� SIM900A�ַ���ƥ��
*	@Description    �� 
*	@Author  		�� Blade@oarap.org
*	@Date			�� 2016-05
*	@param    *str1	�� �ַ���1�ĵ�ַ
*	@param    *str1	�� �ַ���2�ĵ�ַ
*	@param  str2_ptr�� �ַ���2����ʼ��ַ
*	@return			�� ��
*****************************************************************************/
bool Sim900a_Str_Cmp(u8 *str1,u8 *str2,u8 str2_ptr)
{
    u8 i=0;
    u8 cmp_len;
    u8 cmp_val = BIT0;//����ֵ
    cmp_len = strlen(str1);//��ȡ�ַ���1�ĳ���
	for(i=0;i < cmp_len;i++)
	{	
	  if(str1[i] == str2[str2_ptr+i])
		cmp_val <<= 1;
	}
	if (cmp_val>>cmp_len == BIT1)
	  return true;
  return false;
}
/****************************************************************************
*	@Function name	�� Sim900a�ַ�������
*	@Description    �� 
*	@Author  		�� Blade@oarap.org
*	@Date			�� 2016-05
*	@param char *send_data�� �ַ���/�����ͷָ��
*	@return			�� ��
*****************************************************************************/
void Sim900a_Send(char *send_data)
{ 
  u8 i=0;
  while(send_data[i]!='\0')
  	USART_Send(UART5,send_data[i++]);
}
/****************************************************************************
*	@Function name	�� Sim900a�ַ�������  ����
*	@Description    �� �������ⳤ�ȵ��ַ���/����
*	@Author  		�� Blade@oarap.org
*	@Date			�� 2016-05
*	@param *send_data�� �ַ���/�����ͷָ��
*	@param start_ptr�� ��ʼλ��
*	@param send_len �� ���͵ĳ��� 
*	@return			�� ��
*****************************************************************************/
void Sim900a_Send_Len(char *send_data,u8 start_ptr,u8 send_len)
{ 
  u8 i;
  for(i=start_ptr;i< start_ptr + send_len;i++)
  	USART_Send(UART5,send_data[i]);
}
/****************************************************************************
*	@Function name	�� Sim900a���Ͷ���
*	@Description    �� �����������ŵ�ָ���ĵ绰����
*	@Author  		�� Blade@oarap.org
*	@Date			�� 2016-05
*	@param *order_num�� �����ţ��ں��ֻ�����
*	@param box_start ��  �ڶ��������źŵ���ʼλ��
*	@param pwd_start �� �ڶ������������ʼλ��
*	@param box_num  �� ���뵱ǰ�����źţ�����������������ź�ASCII��
*	@return			�� ��
*****************************************************************************/
bool Sim900a_Send_Message(u8 *Order_Num,u8 Box_Num)
{
	u8 i=0,j=0;
	char End_Symbol = 0x1a;
	char *End_Symbol_Ptr = &End_Symbol;
	
	for(i=0;i<=5;i++)//���ֽ�ת˫�ֽ�
	{//����
		Text_Message[PWD_START_POS   + (4*i)] = (SmartBox_DataBase[Box_Num].pwd[i] + 48)/16 + 48;
		Text_Message[PWD_START_POS+1 + (4*i)] = (SmartBox_DataBase[Box_Num].pwd[i] + 48)%16 + 48;
	}
	for(i=0;i<=1;i++)
	{//���ź�
		Text_Message[BOXNUM_START_POS   + (4*i)]=SmartBox_DataBase[Box_Num].Box_Ascii[i]/16 + 48;
		Text_Message[BOXNUM_START_POS+1 + (4*i)]=SmartBox_DataBase[Box_Num].Box_Ascii[i]%16 + 48;
	}
	for(i=0;i<=2;i++)
	{//�����
		Text_Message[GROUP_NUM_START_POS   + (4*i)]=SmartBox_Arg.Group_Num[i]/16 + 48;
		Text_Message[GROUP_NUM_START_POS+1 + (4*i)]=SmartBox_Arg.Group_Num[i]%16 + 48;
	}
	for(i=0;i<=1;i++)
	{//Сʱ
		Text_Message[LATEST_TIME_START_POS   + (4*i)]=SmartBox_Arg.Latest_Time[i]/16 + 48;
		Text_Message[LATEST_TIME_START_POS+1 + (4*i)]=SmartBox_Arg.Latest_Time[i]%16 + 48;
	}
	for(i=3;i<=4;i++)
	{//����
		Text_Message[LATEST_TIME_START_POS   + (4*i)]=SmartBox_Arg.Latest_Time[i-1]/16 + 48;
		Text_Message[LATEST_TIME_START_POS+1 + (4*i)]=SmartBox_Arg.Latest_Time[i-1]%16 + 48;
	}

	
	for(i=0;i<=11;i++)
	{
	  Receiver[12+(4*i)] = Order_Num[2+i];//�ӵ�12λ��ʼ�ǵ绰���벿��
	}
	Sim900a_Send(Receiver);//���ͽ��ն��ŵ��ֻ�����
	Delay_ms(500);
	
	Sim900a_Send(Text_Message);
	Sim900a_Send(End_Symbol_Ptr);//���Ͷ���ֹͣ����
	
	while(!((UART_DATA.SIM_REC.FLAG) && (UART_DATA.SIM_REC.DATA[UART_DATA.SIM_REC.COUNTER-4]=='O') && (UART_DATA.SIM_REC.DATA[UART_DATA.SIM_REC.COUNTER-3]=='K')))
	{//�ڴ˵ȴ��Ӵ��������ݵ����������Ҫ5s���ң�Ҫ�����Ƿ�������ģ��
		Delay_ms(1000);//while(!(UART_DATA.SIM_REC.FLAG);
		j++;
		if(j>=10)
		{
		  	UART_DATA.SIM_REC.FLAG = false;
			UART_DATA.SIM_REC.COUNTER = 0;//��λ����ָ��/������
			return false;
		}
	}
	UART_DATA.SIM_REC.FLAG=false;
	UART_DATA.SIM_REC.COUNTER=0;
	
	return true;
}




