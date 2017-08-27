/********************   One Artisan's Regret And Pride   ***********************
*
*	@File    	�� smartbox.c
*	@Brief   	�� ���ܹ���Ĵ���
*	@Author  	�� Blade@oarap.org
*	@Hardware	�� STM32F103RCT6
*	@Date		�� 2016-05
*	@Description�� ������ϵ�ײ��������㷨��ʵ���Լ�ϵͳ���� ��Ҫע��UARTָ�������λ�ò�ͬ
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

SmartBox_Arg_TypeDef		SmartBox_Arg;//��������ṹ��
SmartBox_DataBase_TypeDef 	SmartBox_DataBase[BOX_NUM+1];//��20�����ţ�������һ����0����������¼״̬�Ͷ������飬������
Hist_DataBase_TypeDef       Hist_DataBase[100];//��¼100����ʷ��¼
Icon_Data_TypeDef 			Icon_Data={{0x00,0x00},{0x00,0x01},{0x00,0x02},{0x00,0x03},{0x00,0x04},{0x00,0x01},{0x00,0x05},{0x00,0x06}};

//u32 Systick_Counter;
u8 Ascii_buf[2];//ASCII�ַ���ʾ����
/****************************************************************************
*	@Function name	�� ���ݿ��ʼ��
*	@Description    �� 
*	@Author  		�� Blade@oarap.org
*	@Date			�� 2016-05
*	@param    		�� ��
*	@return			�� ��
*****************************************************************************/
void Struct_Init()
{
  u8 i;
	for(i=0;i<=BOX_NUM;i++)
	{
		SmartBox_DataBase[i].usable = true;
		SmartBox_DataBase[i].Box_Ascii[0]=(i/10)+48;//ascii��ת������
		SmartBox_DataBase[i].Box_Ascii[1]=(i%10)+48;
	}

	//�ϵ���FLASH�ж�ȡ����
	I2C_ReadByte(ReadBuffer,0x50,0, ADDR_24LC02);//ֱ�������������в���
	//ת�����ݣ�ֻ�ڿ�����ʱ���FLASH�ж�ȡһ�ε�RAM�еĽṹ�壬
	//֮��Ĳ���֮����Flashд��ʹ��RAM�еĽṹ�����ݡ�ƥ�������֮��Ĳ�����
	for(i=0;i<6;i++)
		SmartBox_Arg.Admin_Pwd[i]	=ReadBuffer[ADMIN_PWD_FLASH_PTR+i];
	for(i=0;i<11;i++)
		SmartBox_Arg.Contact_Inf[i]	=ReadBuffer[CONTACT_INF_FLASH_PTR+i];
	for(i=0;i<3;i++)
		SmartBox_Arg.Group_Num[i]	=ReadBuffer[GROUP_NUM_FLASH_PTR+i];
	for(i=0;i<4;i++)
		SmartBox_Arg.Latest_Time[i]	=ReadBuffer[LATEST_TIME_FLASH_PTR+i];
    for(i=0;i<255;i++)
	  WriteBuffer[i] = ReadBuffer[i];
	Hmi_Send(CONTACT_INF_SET_PTR,VAR_W,SmartBox_Arg.Contact_Inf,0,11);//����ʱд�롾����FLASH�С�����ϵ��ʽ
  	Hmi_Send(GROUP_NUM_SET_PTR,VAR_W,SmartBox_Arg.Group_Num,0,3);//д�롾����FLASH�С��Ĺ����
	Hmi_Send(LATEST_TIME_SET_PTR,VAR_W,SmartBox_Arg.Latest_Time,0,4);//д�롾����FLASH�С��Ĺ����
		
}
/****************************************************************************
*	@Function name	�� �Ͳͽ�������ʾ
*	@Description    �� ��ʾ�������Ͳͽ����ܲ����¿��ò͹�����
*	@Author  		�� Blade@oarap.org
*	@Date			�� 2016-05
*	@param    		�� ��
*	@return			�� ��
*****************************************************************************/
void LCD12864_Disp_Frame()
{
  LCD12864_Clear();//����
  LCD12864_Display_String(1,0,"---------------",15);
  LCD12864_Display_String(2,0,"״̬����ɨ�趩��",16);
  LCD12864_Display_String(3,0,"��ʹ�ã�__�Ų͹�",16);
  LCD12864_Display_String(4,0,"���ã�  ���͹�",14);
}

/****************************************************************************
*	@Function name	�� ϵͳ�Լ����
*	@Description    �� ������ģ���Ƿ���ò��������Լ���
*	@Author  		�� Blade@oarap.org
*	@Date			�� 2016-05
*	@param  		�� ��
*	@return			�� ��
*****************************************************************************/
void Self_Check()
{  
	//0-û�д���
	//BIT0-SIM900A����
	//BIT1-SYN6288����
	//BIT2-LV4CCD����
	//BIT3-HMI����
	//BIT4-24C08����
  	//BIT5-HMI��RTC��ȡ����

	u8 Self_Check_Code = 0;						//�Լ���
	u8 Err_Bits[]={"00000000--�ᴥLOGO�����ϵͳ--"};
	u8 BUZZ_100MS[]={0x0a};
	u8 BUZZ_500MS[]={0x32};//��λΪ10ms�����Ʒ�������500ms�������á�
	Self_Check_Code |= Sim900a_Init();			//��ʼ���Լ����ģ��
	Self_Check_Code |= SYN_Init();				//��ʼ���Լ������ϳ�
	Self_Check_Code |= LV4CCD_Init();			//��ʼ���Լ�CCD
	Self_Check_Code |= Hmi_Init(1);				//��ʼ���Լ��˻�����
	Self_Check_Code |= I2C_EEPROM_Init();		//��ʼ���Լ�24C08 EEPROM	
	Self_Check_Code |= Hmi_RTC_Read();			//��ʼ���Լ�HMI RTC
		
	if(Self_Check_Code)
	{//�����λ��ʼ���������
		if(Self_Check_Code&BIT7)Err_Bits[0]=49;
		if(Self_Check_Code&BIT6)Err_Bits[1]=49;
		if(Self_Check_Code&BIT5)Err_Bits[2]=49;
		if(Self_Check_Code&BIT4)Err_Bits[3]=49;
		if(Self_Check_Code&BIT3)Err_Bits[4]=49;
		if(Self_Check_Code&BIT2)Err_Bits[5]=49;
		if(Self_Check_Code&BIT1)Err_Bits[6]=49;
		if(Self_Check_Code&BIT0)Err_Bits[7]=49;
		Hmi_Send(WELCOME_CACHE,VAR_W,Err_Bits,0,30);//��ʾ������뵽HMI
		LCD12864_Display_String(4,0,Err_Bits,8);//������12864
		
		Hmi_Send_reg(BZ_TIME_PTR,REG_W,BUZZ_100MS,0,1);
		Delay_ms(100);
		Hmi_Send_reg(BZ_TIME_PTR,REG_W,BUZZ_100MS,0,1);
		Delay_ms(100);
		Hmi_Send_reg(BZ_TIME_PTR,REG_W,BUZZ_100MS,0,1);
		Delay_ms(100);
		Hmi_Send_reg(BZ_TIME_PTR,REG_W,BUZZ_100MS,0,1);
		Delay_ms(100);
		Hmi_Send_reg(BZ_TIME_PTR,REG_W,BUZZ_100MS,0,1);
		Delay_ms(1500);
	}
	else//���ȫ����������
	{
		LED_GPIO_Config();						//����ָʾ�ƣ���˸��
		Hmi_Send_reg(BZ_TIME_PTR,REG_W,BUZZ_500MS,0,1);//һ������
	}
}
/****************************************************************************
*	@Function name	�� ϵͳ��ʼ��
*	@Description    �� 
*	@Author  		�� Blade@oarap.org
*	@Date			�� 2016-05
*	@param  		�� ��
*	@return			�� ��
*****************************************************************************/
void Sys_Init()
{

	Decoder_GPIO_Config();			//��������������
	I2C_Configuration();			//I2C��������
	LCD12864_GPIO_Config();			//LCD12864���ų�ʼ��
	SYSTICK_Init();					//ϵͳ����ָʾ�ƿ�ʼ��˸
    USART1_Config();				//�˻�����ͨ�Žӿڳ�ʼ��
    USART2_Config();				//���ڲ������ó�ʼ��
	USART3_Config();				//����ɨ���Ǵ���
	UART4_Config();					//STN6288
	UART5_Config();					//SIM900A
    NVIC_Configuration();			//�ж��������ó�ʼ��
	
	Delay_ms(3000);					//�ȴ��ⲿӲ����ʼ�����
	Hmi_Init(0);					//����������ʾ��HMI��
	RTC_Init();						//Ƭ��RTC��HMIУ׼
	LCD12864_Init();				//��ʾ����ʼ��
	
	Self_Check();					//ϵͳ�Լ�
	
	Struct_Init();					//��ʼ�����ݿ�
	LCD12864_Disp_Frame();			//��ʾ���
	Refresh_Usable_Num();
}

/****************************************************************************
*	@Function name	�� ����ת����ASCII��
*	@Description    �� ��U8���͵�����ת����ASCIIֵ
*	@Author  		�� Blade@oarap.org
*	@Date			�� 2016-05
*	@param    num	�� ����һ��U8�ʹ�ת��������
*	@return	Ascii_buf�� ����Ϊ�����ַ������ͷָ��
*****************************************************************************/
u8 *Num2Ascii(u8 num)
{
	Ascii_buf[0] = num/10 + 48;
  	Ascii_buf[1] = num%10 + 48;
	return Ascii_buf;
}
/****************************************************************************
*	@Function name	�� ���ò͹�������
*	@Description    �� �����ò͹����������ʾ�����Ҹ�����»������Ŀ���״̬����������ϵͳ�Ŀ���״̬��
*						������ʾ������ϵͳ״̬
*	@Author  		�� Blade@oarap.org
*	@Date			�� 2016-05
*	@param    		�� ��
*	@return			�� ��
*****************************************************************************/
void Refresh_Usable_Num()
{ 
  u8 k=BOX_NUM+1;//ʵ�����Ÿ��������ݿ�Ķ�Ӧ���1
  u8 Usable_Num[4]={0};//usable_num[3]Ϊ���õ���Ŀ��usable_num[2]ΪHMI���͵ĸ��ֽڣ�usable_num[0]ʮλASCII��usable_num[1]��λASCII
  
  while(--k)//�������ܹ����ݿ⣬���ݿ��д�BOX_NUM��1
  { 
	if(SmartBox_DataBase[k].usable)
  		Usable_Num[3]++;//������λ��Ϊ����
  }
  if(Usable_Num[3])//����п��õĲ͹�
  	SmartBox_DataBase[BUF].usable = true;
  else
  {
  	SmartBox_DataBase[BUF].usable = false;//���û�п��ò͹���ô���������е�usable����Ϊfalse����ʾȫ����������
  	Usable_Num[3]=0;
  }
  LCD12864_Display_String(4,3,Num2Ascii(Usable_Num[3]),2);//x,y,��������ʾ�ĳ��ȣ������ò͹�����ת����ASCII����뵽��ʾ������
  
  Hmi_Send(USABLE_PTR,VAR_W,Usable_Num,2,2);
}

/****************************************************************************
*	@Function name	�� ���ҿ��õĲ͹�
*	@Description    �� �ҵ����ò͹񷵻������ݿ��е�λ�ã�����һ��ʹ�õ�λ��+1��ʼ���ҿ��õĲ͹�
*	@Author  		�� Blade@oarap.org
*	@Date			�� 2016-05
*	@param    		�� ��
*	@return	u8) last_box����ǰ�Ĳ͹����ݿ�λ�ã�Ϊ0˵���͹񲻿���
*	������Դ�����룬pwd_match���ͷ���Դ					
*****************************************************************************/
u8 Box_Find()//Ӧ���붩���Ų�ѯ����
{
  static u8 Last_Box = 0;//��̬�ľֲ�����
  u8 i;
  
  if(!SmartBox_DataBase[0].usable)//���ϵͳ�����ñ㲻���в���
  {
	Last_Box = 0;
	return 0;
  }
  for(i=1;i<=BOX_NUM;i++)//�����ҡ�BOX_NUM����
  {
	if((Last_Box+i)>=BOX_NUM)//����������Ĳ͹���
	  Last_Box = 0;//���1��ʼ���ң�������Ϊ1����һ���͹���Ϊ0��
	if(SmartBox_DataBase[Last_Box+i].usable)
	{
	  SmartBox_DataBase[Last_Box+i].usable = false;  
	  Last_Box += i;//������һ��ʹ�õ����ŵ���ֵ
	  break;
	}
  }
  return Last_Box;//���ص�ǰ����ֵ
}
/****************************************************************************
*	@Function name	�� �����������
*	@Description    �� ������һ����λ���������֤�벢�͵�ָ�������ݿ�λ��
*	@Author  		�� Blade@oarap.org
*	@Date			�� 2016-05
*	@param  box_num	�� ����͹������ݿ��е����кż�λ��
*	@return	true	�� �����ɹ�
*	@history		�� �����ֹ������������ظ��ļ�⹦��
*****************************************************************************/
bool Pwd_Create(u8 box_num)//������������ָ������
{ 
  u8 i=0,j=0;
  u8 Match_Flag = BIT5;
  int seed;
  while(Match_Flag >= BIT5)//������ɵ����벻���ã�˵�����ظ������
  {
	  for(j=0;j<6;j++)//�����������������λ�������
	  {
			srand(Systick_Counter+j);
			seed=(int)(10.0*rand()/(RAND_MAX+1.0));
			SmartBox_DataBase[box_num].pwd[j] = seed;
	  }
	  for(i=1;i<=BOX_NUM;i++)//�����������ţ���������ͻ
	  {	
			Match_Flag = BIT0;
			for(j=0;j<6;j++)
			{
				if((SmartBox_DataBase[i].pwd[j] == SmartBox_DataBase[box_num].pwd[j]) && (i!=box_num))//������ظ���⣬//��ƥ������ݿⲻ���ǵ�ǰ���ɵĿ�
					Match_Flag <<= 1;	
			}		
			if(Match_Flag >= BIT5)//��ȫ��ͬ���߼����������������
				break;	//�������β��ң�ѭ��������������������				
	  }
  }
  return true;
}
/****************************************************************************
*	@Function name	�� �������ƥ��
*	@Description    �� ��ȡ��UART_DATA.HMI_REC.DATA֡�������ֶε����ݺ����ݿ���
*						���е�����ν���ƥ�䲢��֤�͹�����ԣ����ɹ����ͷ���Դ
*	@Author  		�� Blade@oarap.org
*	@Date			�� 2016-05
*	@param  hmi_rec_data ��hmi����������֡����
*	@return	��u8��i �� Ϊ���ڵ����ݿ�λ��
*	������Դ���ͷţ�box_find��������Դ
*****************************************************************************/
u8 Pwd_Match(u8 *Str)
{//����Ҫ������볤�Ⱥ����ŵ��Ƿ����
  u8 i=0,j=0; 	
  u8 Match_Flag = BIT0;//ÿ��ƥ��һ���ֽھ������ƶ�һλ���������ƥ��6λ˵������ƥ��ɹ�
  for(i=1;i<=BOX_NUM;i++)//������������
  {
	  Match_Flag = BIT0;
	  for(j=0;j<PWD_LEN;j++)//2016-05-31�޸�
	  {
			if(SmartBox_DataBase[i].pwd[j] == Str[j+7]-48)//UART_DATA.HMI_REC.DATA[j+7]-48)//�ӵ���λΪ�����ASCII
			  	Match_Flag <<= 1;						
	  }
	  if((Match_Flag == BIT6) && (!SmartBox_DataBase[i].usable))//˵������ƥ��ɹ��������Ų����ã�˵������ʹ���У�
	  {
			SmartBox_DataBase[i].usable = true;//�ı�����״̬���ͷ���Դ
			return i;//����ֵΪ���ݿ�λ��
	  }
  }
  return 0;
}
/****************************************************************************
*	@Function name	�� ����Ա������
*	@Description    �� ��ȡ��UART_DATA.HMI_REC.DATA֡�������ֶε�����
*	@Author  		�� Blade@oarap.org
*	@Date			�� 2016-05
*	@param  hmi_rec_data ��hmi����������֡����
*	@return	BIT1 �� �ɹ�
*****************************************************************************/
bool Admin_Match(u8 *Str)
{
  u8 i=0; 	
  u8 Match_Flag = BIT0;//ÿ��ƥ��һ���ֽھ������ƶ�һλ���������ƥ��6λ˵������ƥ��ɹ�
  
  for(i=0;i<PWD_LEN;i++)
  {
		if(SmartBox_Arg.Admin_Pwd[i] == Str[i+7])//UART_DATA.HMI_REC.DATA[j+7]-48)//�ӵ���λΪ�����ASCII
			Match_Flag <<= 1;						
  }
  if(Match_Flag == BIT6)//˵������ƥ��ɹ��������Ų����ã�˵������ʹ���У�
  {
		return true;//����true����־����Ա����ƥ��ɹ�
  }
  return false;
}
/**********************************���������֤�뵽PC**********************************/
bool send_pwd2_pc()
{
  u8 i;
	for(i=0;i<6;i++)
		USART_Send(USART2,SmartBox_DataBase[BUF].pwd[i]+48);
	USART_Send(USART2,'\r');
	USART_Send(USART2,'\n');
	return true;
}

/****************************************************************************
*	@Function name	�� ������
*	@Description    �� ������������жϺ�ϵͳ����
*	@Author  		�� Blade@oarap.org
*	@Date			�� 2016-05
*	@param  		�� ��
*	@return			�� ��
*****************************************************************************/
void Main_Process()
{
 
	while(UART_DATA.BAR_REC.FLAG || UART_DATA.SIM_REC.FLAG || UART_DATA.HMI_REC.FLAG)//��������־λ�ı仯
	{
  		SmartBox_Arg.Cur_RTC = Time_GetCalendarTime();//���û�����ݲ���ϵͳʱ���ǲ�����µ�
		if(UART_DATA.BAR_REC.FLAG)//lv4 ccd������������Ϊascii�����ʽ
		{
			UART_DATA.BAR_REC.FLAG=false;//��ս��ձ�־λ
  			Bar_Handler();
		}	
		if(UART_DATA.HMI_REC.FLAG)//HMI��������
		{
			UART_DATA.HMI_REC.COUNTER=0;
			UART_DATA.HMI_REC.FLAG=false;
			Hmi_Handler();
		}
		if(UART_DATA.SIM_REC.FLAG)
		{
			UART_DATA.SIM_REC.FLAG=false;
		}
	}
}
/****************************************************************************
*	@Function name	�� �����ż��
*	@Description    �� 
*	@Author  		�� Blade@oarap.org
*	@Date			�� 2016-05
*	@param  		�� ��
*	@return			�� ��
*****************************************************************************/
bool Order_Check(u8 *Str)
{	//����ʼλ��x �� x+ORDER_LEN��鶩����
  	//��Ӧ��������λΪ�������
  	//��鶩���Ƿ�ȫΪ���֣�Num_Check������
  	//��鶩���ŵĸ�ʽ�Ƿ���ȷ
	//���ݿ��鶩�����Ƿ��ù�
  //strncmp ���Ե���
  u8 i;
  u8 Correct_Counter = ORDER_LEN;//��ȷ���ֽڸ���,Ӧ�úͶ����������
  
  if((Str[0]-48)+(Str[1]-48)==0)//�����������00��ͷ˵����DEMO����
	return true;
  for(i=0;i<ORDER_LEN;i++)
  {
	if((Str[i] > 47) && (Str[i] < 58))//�������ֽڱ�ʾ������
	  Correct_Counter--;
  }
  if(Correct_Counter)
	return false;//���Correct_Counter��0��˵�������ݲ������֣�ֱ�ӽ���
  
  /*-------------------------------------------������---------------------------------------------------*/
  if((Str[0]-48==5)&&(Str[1]-48==2))
  	return true;
  /*-------------------------------------------������---------------------------------------------------*/
  
  if(SmartBox_Arg.Cur_RTC.tm_mday!=((Str[0]-48)*10+(Str[1]-48)))//���ڼ��
	return false;//���ڲ���ȷ��ֱ�ӽ���
  
  return true;
}
/****************************************************************************
*	@Function name	�� ����ż��
*	@Description    �� �����ֶ�¼�붩��ʱ���������Ƿ���ȷд��
*	@Author  		�� Blade@oarap.org
*	@Date			�� 2016-05
*	@param  	hmi_rec_data�� ��ڲ�������ָ�룬Start_Num��ʼλ��
*	@return			�� true false��ʾ�Ƿ�ɹ�
*****************************************************************************/
bool Group_Num_Check(u8 *Str)
{  
  u8 i;
  u8 Match_Flag = BIT0;//ƥ����ֽڸ���
  for(i=0;i<3;i++)//��鶩������ĺ���λΪ�����
  {//�ӵ�22��ʼ�ǹ��������λ��
  	if(SmartBox_Arg.Group_Num[i]==Str[i+15])
	{
		Match_Flag <<= 1;
	}
  }
  if(Match_Flag != BIT3)//���û����3����ȵ�λ��˵�������¼��������
  {
  	return false;
  }
  return true;
}
/****************************************************************************
*	@Function name	�� ������д�뵽flash
*	@Description    �� ���������˻������޸ĳ���flash����
*	@Author  		�� Blade@oarap.org
*	@Date			�� 2016-05
*	@param  *str	�� ϵͳ���ò����ṹ��
*	@param  length	�� ��UARTDATA��Ҫת��ĳ���
*	@param  flash_ptr�� Ҫ���ĵĲ�����WriteBuffer�е�λ��
*	@return			�� ��
*****************************************************************************/
void ReadData_WriteFlash(u8 *str,u8 length,u8 flash_ptr)
{
  u8 i=0;
	for(i=0;i<length;i++)
  {//ת�浽RAM
	str[i]=UART_DATA.HMI_REC.DATA[i+7];
	WriteBuffer[flash_ptr+i]=UART_DATA.HMI_REC.DATA[i+7];
  }
  for(i=0; i<0x50; i++)//��д��flash
		while( !I2C_WriteByte(WriteBuffer[i], i,  ADDR_24LC02) );
  //Hmi_Send(ICON_PTR,VAR_W,Icon_Data.MODI_FINISH,0,2);//��ʾ�޸ĳɹ��ı�־
}
/****************************************************************************
*	@Function name	�� BAR_Hadler
*	@Description    �� ���������ݴ������
*	@Author  		�� Blade@oarap.org
*	@Date			�� 2016-05
*	@param  		�� ��
*	@return			�� ��
*****************************************************************************/
void Bar_Handler()
{
  	u8 Cur_Box;//��ǰ���ź�
	if(Order_Check(UART_DATA.BAR_REC.DATA))
	{
		Cur_Box = Box_Find();//��������
		if(Cur_Box)//����͹�ŷ�0
		{
			LCD12864_Display_String(1,0,UART_DATA.BAR_REC.DATA,15);//��ʾ��������LCD12864����
			LCD12864_Display_String(2,3,"������....",10);//��ʾ��ǰ�ķ���״̬
			Pwd_Create(Cur_Box);//���������֤���ָ��������
			Lock_Open(Cur_Box);//��ָ�����ŵ���
			LCD12864_Display_String(3,4,SmartBox_DataBase[Cur_Box].Box_Ascii,2);//��ʾ��ǰ�͹���뵽LCD12864����
			if(Sim900a_Send_Message(UART_DATA.BAR_REC.DATA,Cur_Box))//���Ͷ��ţ������ţ��͹����ʼλ�ã�������ʼλ�ã����ڵĲ͹�	
			{
				LCD12864_Display_String(2,3,"���������",10);//��ʾ��ǰ�Ĺ���״̬
			}
			else
			{//�������false����Ϊ����ʧ��
				LCD12864_Display_String(2,3,"--�޷�����",10);//��ʾ��ǰ�Ĺ���״̬
				//---------���Ϊ�����һ�η���ʧ�ܣ������ж����ɷ���--------------
			}
			Delay_ms(300);
			LCD12864_Display_String(2,3,"��ɨ������",10);//ɨ�����
		}
		else
		{
			LCD12864_Display_String(3,4,"__",2);//����ʾ�͹��
			LCD12864_Display_String(2,3,"�޿��ò͹�",10);
		}
	}
	else//�����Ų���ȷ
	{
		LCD12864_Display_String(1,0,"----��������---",15);
		Delay_ms(800);
	}
	Refresh_Usable_Num();//������ʾ���õĲ͹�����
	LCD12864_Display_String(1,0,"---------------",15);//�رն�����ʾ
}

/****************************************************************************
*	@Function name	�� Hmi_Hadler
*	@Description    �� �˻��������ݴ������
*	@Author  		�� Blade@oarap.org
*	@Date			�� 2016-05
*	@param  		�� ��
*	@return			�� ��
*****************************************************************************/
void Hmi_Handler()
{
  	u8 Touch_Correct[]={0x5a};
    u8 Cur_Box;//��ǰ���ź�
	u8 Cur_Order[18];//��ǰ������
	u8 i=0;
	u8 Clear_Disp[2]={0xff,0xff};//�����ʾ������
	u8 Sending[]={"���ŷ�����...."};
	u8 Send_Success[]={"�����ѷ��ͳɹ�"};
	u8 Send_Fail[]={"�����ѷ���ʧ��"};
	//Ϊ����ʾЧ����������¼����ɺ�HMI�Զ��������еȴ�icon��ҳ�棬����Ƭ����������������ҳ�������
	//¼�����ҳ�棨�͸ոյĲ�����ȫ��ͬ������ʾ�ȴ�ͼ�꣬�ú���ˢ�½��ͼ�ꣻ
	
	Hmi_Send(BOXNUM_PTR,VAR_W,Clear_Disp,0,2);//�ر����ź���ʾ
	Hmi_Send(SENDSTA_PTR,VAR_W,Clear_Disp,0,2);//�رա����ŷ���״̬����ʾ��
	Hmi_Send(ICON_PTR,VAR_W,Icon_Data.WAIT,0,2);//��ʾ�ȴ�ICON����¼�����ҳ�桿
	Hmi_Page_Switch(INPUT_FINISH);//��ת�����հ׵�¼�����ҳ�桿
	

	switch(UART_DATA.HMI_REC.DATA[HMI_RAM_PTR])//���ݲ�ͬ�ķ��ص�ַȷ����ͬ�Ĳ���
	{
		case PWD_PTR://��������������������������
		{
			Cur_Box = Pwd_Match(UART_DATA.HMI_REC.DATA);
			if(Cur_Box)
			{	
				Delay_ms(200);//��ʾ�ȴ�ͼ����
				Lock_Open(Cur_Box);		  
				Hmi_Send(ICON_PTR,VAR_W,Icon_Data.PWD_RIGHT,0,2);//��ʾ����ɹ���ICON
				Hmi_Send(BOXNUM_PTR,VAR_W,Num2Ascii(Cur_Box),0,2);//��ʾ���ź�				
				SYN_Send(Cur_Box);
			}
			else		  
			{	
				Delay_ms(500);	//��ʾ�ȴ�ͼ����	
				SYN_Send(0);
				Hmi_Send(BOXNUM_PTR,VAR_W,Clear_Disp,0,2);//�ر����ź���ʾ
				Hmi_Send(ICON_PTR,VAR_W,Icon_Data.PWD_ERROR,0,2);//��ʾ����ʧ�ܵ�ICON
			}
			//Refresh_Usable_Num();
		}break;

		case ORDER_PTR://��������������Ƕ���������
		{
			for(i=0;i<18;i++)//��ȡ������
			{
				Cur_Order[i] = UART_DATA.HMI_REC.DATA[i+7];
			}
			if(Order_Check(Cur_Order) && Group_Num_Check(Cur_Order))//----------------��������ȷ�Լ��-----------------------
			{
				Cur_Box = Box_Find();
				if(Cur_Box)//����͹�ŷ�0
				{
					Delay_ms(200);//��ʾ�ȴ�ͼ����
					Pwd_Create(Cur_Box);//���������֤���ָ��������
					Lock_Open(Cur_Box);		  
					Hmi_Send(ICON_PTR,VAR_W,Icon_Data.ORDER_RIGHT,0,4);//��ʾ����ɹ���ICON��xx�������Ѵ򿪡��Ϳ����Ź���ͬһ��LOGO
					Hmi_Send(BOXNUM_PTR,VAR_W,Num2Ascii(Cur_Box),0,2);//��ʾ���ź�				
					SYN_Send(Cur_Box);
					//Refresh_Usable_Num();//������ʾ���õĲ͹�����
					Hmi_Send(SENDSTA_PTR,VAR_W,Sending,0,14);//�򡾷���״̬����д��
					if(Sim900a_Send_Message(Cur_Order,Cur_Box))//���Ͷ��ţ������ţ��͹����ʼλ�ã�������ʼλ�ã����ڵĲ͹�		
					{//���ͳɹ�
						Hmi_Send(SENDSTA_PTR,VAR_W,Send_Success,0,14);//�򡾷���״̬����д��
					}
					else
					{//���ŷ���ʧ��
						Hmi_Send(SENDSTA_PTR,VAR_W,Send_Fail,0,14);//�򡾷���״̬����д��
					}
				}
				else	//û�п��ò͹�򵥺Ŵ���-------��Ҫ�޸�--------------------------------------------
				{
					Delay_ms(500);	//��ʾ�ȴ�ͼ��
					//Refresh_Usable_Num();//������ʾ���õĲ͹�����
					LCD12864_Display_String(3,4,"__",2);//����ʾ�͹��
					LCD12864_Display_String(2,0,"--�޿��ò͹�-- ",15);
				}
			}
			else//���ż��ʧ�ܵ������
			{
			  Delay_ms(500);
				Hmi_Send(ICON_PTR,VAR_W,Icon_Data.ORDER_ERROR,0,4);//��ʾ���������ICON
				SYN_FrameInfo(0,"��������");
			}
			
		}break;
		
		case ADMIN_PTR://������������ݹ���Ա����¼��
		{
		  	Delay_ms(300);//��ʾ�ȴ�ͼ��
			if(Admin_Match(UART_DATA.HMI_REC.DATA))
			{
			  SYN_FrameInfo(0,"��½�ɹ�");
			  Hmi_Send(ICON_PTR,VAR_W,Icon_Data.ADMIN_RIGHT,0,2);//��ʾ������ȷ��ICON
			  Delay_ms(500);//�ȴ�����ͼ��
			  Hmi_Page_Switch(ADMIN_PAGE);//��ת��������Ա���桿
			}
			else
			{
			  Hmi_Send(ICON_PTR,VAR_W,Icon_Data.ADMIN_ERROR,0,2);//��ʾ��������ICON
			  SYN_FrameInfo(0,"�������");
			}
		}break;
		
		
		
		case CONTACT_INF_SET_PTR://��ϵ��ʽ����
		{
		  Delay_ms(300);
		  ReadData_WriteFlash(SmartBox_Arg.Contact_Inf,11,CONTACT_INF_FLASH_PTR);
		  Hmi_Send(ICON_PTR,VAR_W,Icon_Data.MODI_FINISH,0,2);//��ʾ�޸ĳɹ��ı�־
		}break;
		
		case GROUP_NUM_SET_PTR://���������
		{
		  Delay_ms(300);
		  ReadData_WriteFlash(SmartBox_Arg.Group_Num,3,GROUP_NUM_FLASH_PTR);
		  Hmi_Send(ICON_PTR,VAR_W,Icon_Data.MODI_FINISH,0,2);//��ʾ�޸ĳɹ��ı�־
		}break;
		
		case ADMIN_PWD_SET_PTR://����Ա��������
		{
		  Delay_ms(300);
		  ReadData_WriteFlash(SmartBox_Arg.Admin_Pwd,6,ADMIN_PWD_FLASH_PTR);
		  Hmi_Send(ICON_PTR,VAR_W,Icon_Data.MODI_FINISH,0,2);//��ʾ�޸ĳɹ��ı�־
		  
		}break;
		
		case OPEN_BOX_SET_PTR://�򿪲͹�Ҳ��ASCII¼��
		{
		  Delay_ms(300);
		  if(((UART_DATA.HMI_REC.DATA[7]==0x30)&&(UART_DATA.HMI_REC.DATA[8]==0x30)) || ((UART_DATA.HMI_REC.DATA[7]==0X30)&&(UART_DATA.HMI_REC.DATA[8]==0xFF)))
		  {//�����в͹�,����������0��00
			for(i=1;i<=BOX_NUM;i++)
		  		Lock_Open(i);
		  }
		  else
		  {//��ָ���͹�
			if(UART_DATA.HMI_REC.DATA[8]==0XFF)
			  	Lock_Open(UART_DATA.HMI_REC.DATA[7]-48);
			else//���������λ
		  		Lock_Open(((UART_DATA.HMI_REC.DATA[7]-48)*10)+((UART_DATA.HMI_REC.DATA[8]-48)));
		  }
		  Hmi_Page_Switch(ADMIN_PAGE);//��ת��������Ա���桿
		  
		}break;
		
		case LATEST_TIME_SET_PTR://����ȡ��ʱ��
		{
		  Delay_ms(300);
		  ReadData_WriteFlash(SmartBox_Arg.Latest_Time,4,LATEST_TIME_FLASH_PTR);
		  Hmi_Send(ICON_PTR,VAR_W,Icon_Data.MODI_FINISH,0,2);//��ʾ�޸ĳɹ��ı�־
		}break;

		
		
		case ADMIN_KEY_RETURN://����Ա����İ�������
	  	{
		 	Delay_ms(300);
	  		switch(UART_DATA.HMI_REC.DATA[8])
			{
			  case ADMIN_TOUCH_CORRECT:
			  {
			  		Hmi_Send_reg(TOUCH_CORRECT_PTR,REG_W,Touch_Correct,0,1);//��������У׼����
			  }break;
			  
			  case ADMIN_RESTART_SYS:
			  {//����ϵͳ
        			NVIC_SystemReset();
			  }break;
			  
			  case ADMIN_HISTORY:	
			  {
			  }break;
			  
			  case ADMIN_RESERVE:
			  {
			  }break;
			}
			Hmi_Page_Switch(ADMIN_PAGE);//��ת��������Ա���桿
	  	}break;
	}
	Refresh_Usable_Num();
}

