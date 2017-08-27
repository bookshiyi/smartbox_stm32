/********************   One Artisan's Regret And Pride   ***********************
*
*	@File    	�� lcd12864.c
*	@Brief   	�� LCD12864�ĵײ������ļ�
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

u8 CGROM_Code[] = {
					//����ͼ��	
					0xff,0xff,0x80,0x01,0xbf,0xfd,0xdf,0xfb,0xec,0x37,0xf6,0x6f,0xfb,0xdf,0xfd,0xbf,
					0xfe,0x7f,0xfe,0x7f,0xfe,0x7f,0xfe,0x7f,0xfe,0x7f,0xfe,0x7f,0xfe,0x7f,0xff,0xff,
					//����ͼ��
//						0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x70,0x14,0xd8,0x1d,0x88,0x01,0x80,0x01,0x80,
//						0x01,0x80,0x01,0x80,0x01,0x88,0x00,0xc8,0x00,0x70,0x00,0x00,0x00,0x00,0x00,0x00,   
					//����ͼ��
					0x07,0xe0,0x0e,0x70,0x1e,0xb8,0x3e,0xdc,0x36,0xec,0x3a,0xdc,0x3c,0xbc,0x3e,0x7c,
					0x3e,0x7c,0x3c,0xbc,0x3a,0xdc,0x36,0xec,0x3e,0xdc,0x1e,0xb8,0x0e,0x70,0x07,0xe0,
					//CDͼ��
					0xff,0xff,0xff,0xff,0xff,0xff,0xe3,0x07,0xc9,0xb3,0x9d,0xb9,0x9f,0xb9,0x9f,0xb9,
					0x9f,0xb9,0x9f,0xb9,0x9d,0xb9,0xc9,0xb3,0xe3,0x07,0xff,0xff,0xff,0xff,0xff,0xff,
					//��ָ������
					0x00,0x00,0x1e,0x00,0x12,0x00,0x73,0xfc,0x52,0x02,0x52,0x1c,0x53,0xe0,0x52,0x10,
					0x53,0xe0,0x52,0x10,0x53,0xe0,0x52,0x10,0x73,0xe0,0x12,0x00,0x1e,0x00,0x00,0x00			
					};
/****************************************************************************
*	@Function name	�� GPIO����
*	@Description    �� ����GPIOλ�����Ƿ�ɹ�����Ҫ���߼������ǻ�ʾ���������ŵ�ƽ�仯
*	@Author  		�� Blade@oarap.org
*	@Date			�� 2016-05
*	@param    		�� ��
*	@return			�� ��
*****************************************************************************/
void LCD12864_GPIO_TEST()
{
	while(1)
	{
		LCD12864_CS_H;
		Delay_ms(30);
		LCD12864_SDA_H;
		Delay_ms(30);
		LCD12864_SCL_H;
		Delay_ms(30);
		LCD12864_CS_L;
		Delay_ms(30);
		LCD12864_SDA_L;
		Delay_ms(30);
		LCD12864_SCL_L;
		Delay_ms(30);
	}
}
/****************************************************************************
*	@Function name	�� LCD12864GPIO��ʼ��
*	@Description    �� ��ʼ������״̬��LCD12864��GPIO
*	@Author  		�� Blade@oarap.org
*	@Date			�� 2016-05
*	@param    		�� ��
*	@return			�� ��
*****************************************************************************/
void LCD12864_GPIO_Config(void) //GPIOB 5 8 9
{	
    //����һ��GPIO_InitTypeDef ���͵Ľṹ��
    GPIO_InitTypeDef  GPIO_InitStructure;	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//ʹ��GPIO������ʱ��

    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_5 | GPIO_Pin_8 | GPIO_Pin_9;//ѡ��Ҫ�õ�GPIO����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //��������ģʽΪ�������ģʽ						 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//���������ٶ�Ϊ50MHZ         
    GPIO_Init(GPIOB, &GPIO_InitStructure);//���ÿ⺯������ʼ��GPIO
}
/****************************************************************************
*	@Function name	�� LCD�����ֽ�
*	@Description    �� ���ȴ���һ�������ֽڣ���������5����1��������
*               		��һ�����ڣ���ʱ������������ã����Ҵ��д���
*               		��ͬ���������ŵ�����λָ�����䷽��R/W��ȷ
*               		���Ƕ�����д���ʹ������ʣ�RS��ȷ��������Ĵ�
*               		���������ݼĴ����������ĵڰ�λ��һ����0��
*	@Author  		�� Blade@oarap.org
*	@Date			�� 2016-05
*	@param  dt		�� ������ֽڣ���5λ�ں����ڲ��趨Ϊ1ֻ��ȷ��RW��RSΪ�������λҲ���ڲ�ȷ��
*	@return			�� ��
*****************************************************************************/
void LCD12864_Start(u8 dt)
{
	u8 dat,j;

	dat = dt | 0xf8;	//��5λ�趨Ϊ��1�����ڰ�λ�趨Ϊ��0��
	LCD12864_SCL_L;
	Delay_us(LCD12864_Delay);
	LCD12864_CS_H;	//ѡ����ʾ�����ߵ�ƽ��Ч
	for(j = 0;j < 8;j++)
	{
		if(dat & 0x80)	
		{
		  LCD12864_SDA_H;	//�����ݵ�������
		}
		else	
		  LCD12864_SDA_L;
			
		Delay_us(LCD12864_Delay);
		LCD12864_SCL_H;	//�������ݣ���������Ч
		Delay_us(LCD12864_Delay);
		LCD12864_SCL_L;
		dat <<= 1;	//����һλ���ȷ����Ǹ�λ
	}
	Delay_us(LCD12864_Delay);
	LCD12864_SCL_L;
	Delay_us(LCD12864_Delay);	//�ȴ�Ӳ����Ӧ���ȴ����ݷ�����ȫ��	
}
/****************************************************************************
*	@Function name	�� �˻������ʼ��
*	@Description    �� LCD12864д��һ���ֽ������������
*	@Author  		�� Blade@oarap.org
*	@Date			�� 2016-05
*	@param  dat		�� Ҫд��������������
*	@param  cmd		�� ������������ж�Ϊ��0Ϊ���1Ϊ����
*	@return			�� ��
*****************************************************************************/
void LCD12864_Write_Byte(u8 dat,u8 cmd)
{
	u8 tem;
	u8 i,j;

	tem = dat & 0xf0;	//�ȷ��͸�4λ
	LCD12864_Start((cmd << 1) | 0xf0);	//��RWΪ��0����RSΪ��ord�����������д���Ϊ���ݸ�ʽ
	for(j = 0;j < 2;j++)	//һ���ֽ����ݻ�����������η���
	{
		LCD12864_SCL_L;	//���������ߵ�ƽ�仯
		Delay_us(LCD12864_Delay);
		for(i = 0;i < 8;i++)
		{
			if(tem & 0x80)	
			{
			  LCD12864_SDA_H;	//�������ݣ����ε�4λ
			}
			else	LCD12864_SDA_L;	

			Delay_us(LCD12864_Delay);
			LCD12864_SCL_H;	//�������ݣ���������Ч
			Delay_us(LCD12864_Delay);
			LCD12864_SCL_L;
			tem <<= 1;	//����һλ���ȷ����Ǹ�λ
		}
		tem = (dat << 4) & 0xf0;	//�����4λ���ٴη��͵�4λ
	}
	Delay_us(LCD12864_Delay);
	LCD12864_SDA_L;
	Delay_us(LCD12864_Delay);
	LCD12864_SCL_L;
	Delay_us(LCD12864_Delay);
	LCD12864_CS_L;
	Delay_us(LCD12864_Delay);//�ȴ�Ӳ����Ӧ���ȴ����ݷ�����ȫ��
}

/****************************************************************************
*	@Function name	�� ����LCD��ʾλ��
*	@Description    �� ��LCD12864д�������Ҫ��ʾ�Ŀ�ʼλ��
*	@Author  		�� Blade@oarap.org
*	@Date			�� 2016-05
*	@param  	x	�� �У�1-4
*	@param  	y	�� ��  0-7
*	@return			�� ��
*****************************************************************************/
void LCD12864_Set_xy(u8 x,u8 y)
{
	switch(x)
	{
		case 1:
					LCD12864_Write_Byte((One_LineAddress + y),0);	//д�������ַ
					break;
		case 2:
					LCD12864_Write_Byte((Two_LineAddress + y),0);	//д�������ַ
					break;
		case 3:
					LCD12864_Write_Byte((Three_LineAddress + y),0);	//д�������ַ
					break;
		case 4:
					LCD12864_Write_Byte((Four_LineAddress + y),0);	//д�������ַ
					break;
		default:
					LCD12864_Write_Byte((One_LineAddress + y),0);	//д�������ַ
					break;
	}
}
/****************************************************************************
*	@Function name	�� д���Զ����ַ���LCD��CGRAM
*	@Description    �� 
*	@Author  		�� Blade@oarap.org
*	@Date			�� 2016-05
*	@param *data_code�� д�������
*	@return			�� ��
*****************************************************************************/
void LCD12864_Write_CGRAM(u8 *data_code)
{
	u8 i;
	LCD12864_Write_Byte(0x34,0);	//���ַ���չָ��
	LCD12864_Write_Byte(0x02,0);	//SR=0����������
	LCD12864_Write_Byte(0x30,0);	//�ָ�����ָ��
	LCD12864_Write_Byte(0x40,0);	//CGRAM��ַ
	for(i = 0;i < CGRAM_Value;i++)
	{
		LCD12864_Write_Byte(data_code[i*2],1);
		LCD12864_Write_Byte(data_code[i*2+1],1);
	}
}
/****************************************************************************
*	@Function name	�� ��ʾһ��CGRAM����
*	@Description    �� 
*	@Author  		�� Blade@oarap.org
*	@Date			�� 2016-05
*	@param	x		����
*	@param	y		����
*	@param	*str	��ΪҪ��ʾ���ַ���
*	@param	add_h	��CGRAM��λ��ַ
*	@param	add_l	��CGRAM��λ��ַ
*	@return			�� ��
*****************************************************************************/
void Display_CGRAM(u8 x,u8 y,u8 add_h,u8 add_l)
{
	LCD12864_Set_xy(x,y);	//������ʾ��ַ
	LCD12864_Write_Byte(add_h,1);
	LCD12864_Write_Byte(add_l,1);
}
/****************************************************************************
*	@Function name	�� LCD12864��ʾ�ַ���
*	@Description    �� ������Ŀ�ͷ��ʼ��ʾlen�����ַ���
*	@Author  		�� Blade@oarap.org
*	@Date			�� 2016-05
*	@param	x		����
*	@param	y		����
*	@param	*str	��Ҫ��ʾ���ַ���
*	@param	len		����ʾ�ĳ���
*	@return			�� ��
*****************************************************************************/
void LCD12864_Display_String(u8 x,u8 y,u8 *str,u8 len)
{
	LCD12864_Set_xy(x,y);	//������ʾ��ַ
	while(len--)
	{
		LCD12864_Write_Byte(*str,1);
		str++;
	}
}
/****************************************************************************
*	@Function name	�� LCD12864��ʾ�ַ���
*	@Description    �� �������ָ��λ�ÿ�ʼ��ʾlen�����ַ���
*	@Author  		�� Blade@oarap.org
*	@Date			�� 2016-05
*	@param	x		����
*	@param	y		����
*	@param	startnum����ʼ��ʾ��λ��
*	@param	*str	��Ҫ��ʾ���ַ���
*	@param	len		����ʾ�ĳ���
*	@return			�� ��
*****************************************************************************/
void LCD12864_Display_String_Assign(u8 x,u8 y,u8 *str,u8 starsum,u8 len)
{
	u8 i;

	LCD12864_Set_xy(x,y);	//������ʾ��ַ

	for(i = starsum;i < starsum + len;i++)
	{
		LCD12864_Write_Byte(str[i],1);
	}
}
/****************************************************************************
*	@Function name	�� LCDд��GDRAM��ͼ
*	@Description    �� 
*	@Author  		�� Blade@oarap.org
*	@Date			�� 2016-05
*	@param	x		����
*	@param	y		����
*	@param	width	����ʾͼƬ�Ŀ��
*	@param	hieght	����ʾͼƬ�ĸ߶�
*	@param	*bmp	��Ҫ��ʾ���ַ�����λͼ��
*	@param	dis_mode����ʾģʽ���ǰ�����ʾ����ȫ����ʾ
*                         0��������ʾ��1��ȫ����ʾ
*	@return			�� ��
*****************************************************************************/
void LCD12864_Display_GDRAM(u8 x,u8 y,u8 width,u8 height,u8 *bmp,u8 dis_mode)
{
	u8 i,j,k;
	u8 base_x,base_y;	//��ʼ����

	/******ȫ����ͼ��ʾ******/
	if(dis_mode)
	{
		switch(x)
		{
			case 1:
					base_y = One_LineAddress + y;
					break;
			case 2:
					base_y = Two_LineAddress + y;
					break;
			case 3:
					base_y = Three_LineAddress + y;
					break;
			case 4:
					base_y = Four_LineAddress + y;
					break;
			default:	break;
		}
		 
	 	LCD12864_Write_Byte(MPU_8bit_Expansion_Draw_Close,0);	//����ָ��
		LCD12864_Write_Byte(MPU_8bit_Expansion_Draw_Open,0);	//�򿪻�ͼ����
		
		for(j = 0;j < height;j++)	//32
		{
			LCD12864_Write_Byte(base_y + j,0);	//д���кţ����ڼ��п�ʼ
			LCD12864_Write_Byte(One_LineAddress,0);	//������ĵڼ����ֽڿ�ʼд
			
			for(i = 0;i < width;i++)	//�ϰ���
			{
				LCD12864_Write_Byte(bmp[width * j + i],1);	//��ʼд������
			}
			for(k = 0;k < width;k++)	//�°���
			{
			 	LCD12864_Write_Byte(bmp[width * (j + height) + k],1);	//��ʼд������
			}
		} 		
	}
	/******�ֿ��ϰ��������°���д******/
	else
	{
		switch(x)	//�ж���ʾ�ĺ����꣬ȷ����ʼ��
		{
			case 1:	//�ϰ���
					base_x = One_LineAddress;	//��ʼ��ַΪ0x80
					break;
	
			case 2:	//�°���
					base_x = One_LineAddress;	//��ʼ��ַΪ0x80
					break;
	
			case 3:	//�°���
					base_x = Three_LineAddress;	//��ʼ��ַΪ0x88
					break;
			default:
					break;
		}
		
		base_y = base_x + y;	//x�᷽���ƫ�ƣ�����ַ + ƫ�Ƶ�ַ
		
		LCD12864_Write_Byte(MPU_8bit_Expansion_Draw_Close,0);	//����ָ��
		LCD12864_Write_Byte(MPU_8bit_Expansion_Draw_Open,0);	//�򿪻�ͼ����
		
		if(x == 1 || x == 3)	//ֱ����ͬһ�������ģ�ֱ��д��32λ����
		{
			for(i = 0;i < 32;i++)	//д��ַ�н�������д��ֱ��дˮƽ��������趨��ͼRAM��ַ��˵��
			{ 
				LCD12864_Write_Byte(One_LineAddress + i,0);	//д�봹ֱ��ַ�������ϰ��������°���
				LCD12864_Write_Byte(base_y,0);	//д��ˮƽ��ַ
				
				for(j = 0;j < 2;j++)	//����8bit���ݣ����16bit
				{
					LCD12864_Write_Byte(*bmp++,1);	//д������
				}
			}
		}
		
		if(x == 2)	//������ʾ����ͬһ�����ϵģ���д�ϰ�������д�°���
		{
			for(i = 0;i < 16;i++)	//�ϰ���16�е�������
			{
				LCD12864_Write_Byte(Two_LineAddress + i,0);	//д�봹ֱ��ַ
				LCD12864_Write_Byte(base_y,0);	//д��ˮƽ��ַ
				
				for(j = 0;j < 2;j++)	//����8bit���ݣ����16bit
				{
					LCD12864_Write_Byte(*bmp++,1);	//д������
				}
			}
			for(i = 0;i < 16;i++)	//�°���16�е�������
			{
				LCD12864_Write_Byte(Three_LineAddress + i,0);	//д�봹ֱ��ַ
				LCD12864_Write_Byte(base_y + 8,0);	//д��ˮƽ��ַ
				
				for(j = 0;j < 2;j++)	//����8bit���ݣ����16bit
				{
					LCD12864_Write_Byte(*bmp++,1);	//д������
				}
			}
		}	  

	}
	LCD12864_Write_Byte(MPU_8bit_Expansion_Draw_Open,0);	//����ָ��򿪻�ͼ����
//	LCD12864_Write_Byte(Instruct_Basic8,0);
}	
/****************************************************************************
*	@Function name	�� LCD12864����
*	@Description    �� 
*	@Author  		�� Blade@oarap.org
*	@Date			�� 2016-05
*	@param			�� ��
*	@return			�� ��
*****************************************************************************/
void LCD12864_Clear(void)
{
	LCD12864_Write_Byte(Clear_Display,0);	//����
}

/****************************************************************************
*	@Function name	�� LCD12864��ʼ��
*	@Description    �� ��ʾ��Ԥ������ʾ�����Ŀ�������
*	@Author  		�� Blade@oarap.org
*	@Date			�� 2016-05
*	@param			�� ��
*	@return			�� ��
*****************************************************************************/
void LCD12864_Init(void)
{
	u8 welcome []="----��ӭʹ��----";
	u8 smartbox[]="--�����Ͳ�ϵͳ--";	
	u8 loading[]="loading";
	  
	LCD12864_Write_Byte(Instruct_Basic8,0);	//����ָ�8bitģʽ,0x30
	LCD12864_Write_Byte(Open_Display,0);	//������ʾ�������أ���ʾ���׹�,0x0c
	LCD12864_Write_Byte(Clear_Display,0);	//����,0x01
	LCD12864_Write_Byte(Vernier_ShiftRight,0);	//�趨��㣬�������,0x06

	//LCD12864_Write_CGRAM(CGROM_Code);	//д���Զ����ַ�
	
  LCD12864_Display_String(1,0,welcome,16);
  LCD12864_Display_String(3,0,smartbox,16);
  LCD12864_Display_String(4,0,loading,7);
}
/*********************************************************/

