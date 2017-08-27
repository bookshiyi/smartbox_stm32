/**********************************************************
* @ File name -> lcd12864.h
* @ Version   -> V1.1.2
* @ Date      -> 11-30-2013
* @ Brief     -> LCD12864��������ͷ�ļ�
*                ������ST7920����оƬ

 V1.1
* @ Revise    -> A��������������궨�塣
* @           -> B�����Ӵ��ڡ�����ת���궨�壬���ӶԸ��ٶ�MCU����ͨѶƵ����ʱ����
* @           -> C����ʾ�ַ�������������ʾ����ѡ�񣬼���ʾ���ٸ�ASCII����ʾ�ַ�������x2����
* @           -> D��ȥ����ʾ���麯�����ϲ����ַ�����ʾ��

 V1.1.1
* @ Revise    -> �����ַ�����ʾ���������Դ�Ҫ��ʾ������������λ����ʾһ�������ַ��� 

 V1.1.2
* @ Revise    -> A�����ӶԵ��پ���ϵͳ��MCUͨѶƵ���Ƿ�ʹ�õĺ궨��
* @           -> B�������ڼ��Һ��æ��ʱ�˳���Ԥ����ѭ��
**********************************************************/

#ifndef _LCD12864_H_ 
#define _LCD12864_H_


/**********************************************************
                        �����궨��                       
**********************************************************/

#define LCD_Busy				0x80	//LCDæ��־

#define Graph_BassADD			0x00	//��ʾGRAM����ַ

//CGRAM��ʾ��ַ��Ԫ
#define ANT_Graph				0x00
//#define Temperature_Graph		0x02
#define Bluetooth_Graph			0x02
#define CD_Graph				0x04
#define Hand_Graph				0x06

//////////////////////////////////////////////////////
//LCD12864ָ�
//////////////////////////////////////////////////////
#define One_LineAddress			0x80	//��һ�л���ַ
#define Two_LineAddress			0x90	//�ڶ��л���ַ
#define Three_LineAddress		0x88	//�����л���ַ
#define Four_LineAddress		0x98	//�����л���ַ

//����ָ��궨��
#define MPU_Busy				0x80	//LCDæ��ͬʱҲ�����˵�ַ��������AC����ֵ
#define Clear_Display			0x01	//����ָ��
#define Zero_Address			0x02	//��ʾ��ַ���㣬AC = 0

//����ָ��궨��
#define LCD_Standby				0x01	//����ָ�������RE = 1�������趨�Ĵ����������ʹ�����������ģʽ
                                        //�˳�������д������ָ������˳����ٽ�RE = 0����

/***********����ָ��***********/

//�����趨�㣨entry mode set��
// b7  b6  b5  b4  b3  b2  b1  b0
// 0   0   0   0   0   1   I/D  S 
#define Entry_ModeSet			0x04	//����

#define Vernier_ShiftRight		(Entry_ModeSet | (1<<1))	//�α����ƣ�AC+1
#define Vernier_ShiftLeft		(Entry_ModeSet | (0<<1))	//�α����ƣ�AC-1

#define Frames_ShiftRight		(Vernier_ShiftLeft | 0x01)	//������������
#define Frames_ShiftLeft		(Vernier_ShiftRight | 0x01)	//������������

//��ʾ�������ã�display status��
// b7  b6  b5  b4  b3  b2  b1  b0
// 0   0   0   0   1   D   C   B 
#define Display_Status			0x08	//����

#define Open_Display			(Display_Status | (1<<2))	//������ʾ��
#define Close_Display			(Display_Status | (0<<2))	//������ʾ��

#define Open_Vernier			(Display_Status | (1<<1))	//�α���ʾ��
#define Close_Vernier			(Display_Status | (0<<1))	//�α���ʾ��

#define Vernier_White_Open		(Display_Status | 0x01)	//�α�λ����ʾ���׿�
#define Vernier_White_Close		(Display_Status | 0x00)	//�α�λ����ʾ���׹أ�������ʾ

//�α����ʾ��λ���ƣ�cursor and display shift control��
// b7  b6  b5  b4  b3    b2   b1  b0
// 0   0   0   1   S/C  R/L1  X   X 
#define Cursor_Display_Control	0x10	//����

#define Cursor_ShiftLeft		(Cursor_Display_Control | (0<<2))	//�α������ƶ���AC = AC - 1
#define Cursor_ShiftRight		(Cursor_Display_Control | (1<<2))	//�α������ƶ���AC = AC + 1
#define Display_ShiftLeft		(Cursor_Display_Control | (2<<2))	//��ʾ�����ƶ����α�����ƶ���AC = AC
#define Display_ShiftRight		(Cursor_Display_Control | (3<<2))	//��ʾ�����ƶ����α�����ƶ���AC = AC  

//�����趨��function set��
// b7  b6  b5  b4  b3  b2    b1  b0
// 0   0   1   DL  X   0/RE  X 	 X
#define Function_SetBasic		0x20	//����

#define MPU_8bit				(Function_SetBasic | (1<<4))	//8bit MPU�ӿ�
#define MPU_4bit				(Function_SetBasic | (0<<4))	//4bit MPU�ӿ�

#define Instruct_Basic4			(MPU_4bit | (0<<2))				//4bit MPU�ӿڣ�����ָ�
#define Instruct_Basic8			(MPU_8bit | (0<<2))				//8bit MPU�ӿڣ�����ָ�

#define Instruct_Expansion4		(MPU_4bit | (1<<2))				//4bit MPU�ӿڣ�����ָ�
#define Instruct_Expansion8		(MPU_8bit | (1<<2))				//8bit MPU�ӿڣ�����ָ�

//�趨CGRAM��ַ
// b7  b6  b5  b4  b3  b2  b1  b0
// 0   1   A5  A4  A3  A2  A1  A0
#define CGRAM_Set				0x40	//����
//�趨CGRAM��ַ����ַ��������AC����AC��ΧΪ0x00 ~ 0x3f����ȷ������ָ����SR = 0����λ�û�RAM��ַѡ��

//�趨DDRAM��ַ
// b7  b6  b5  b4  b3  b2  b1  b0
// 1   0   A5  A4  A3  A2  A1  A0
#define DDRAM_Set				0x80	//����
//�趨CGRAM��ַ����ַ��������AC��

//д��ʾ���ݵ�RAM��RS = 1 RW = 0��
// b7  b6  b5  b4  b3  b2  b1  b0
// D7  D6  D5  D4  D3  D2  D1  D0
//д�����ݺ��ʹAC�ı䣬ÿ��RAM��CGRAM��DDRAM��IRAM����ַ����������д��2�ֽڵ���ʾ���ݣ�
//��д��ڶ����ֽ�ʱ����ַ��������AC����ֵ�Զ���1

//��ȡ��ʾ���ݵ�RAM��RS = 1 RW = 1��
// b7  b6  b5  b4  b3  b2  b1  b0
// D7  D6  D5  D4  D3  D2  D1  D0
//��ȡ���ݺ��ʹAC�ı䣬�趨RAM��CGRAM��DDRAM��IRAM����ַ����ҪDummy readһ�κ���ܶ�ȡ����ȷ����ʾ���ݣ�
//�ڶ��ζ�ȡ����ҪDummy read�����������趨��RAM��ַ

/***********����ָ��***********/

//��λ�û�RAM��ַѡ��
// b7  b6  b5  b4  b3  b2  b1  b0
// 0   0   0   0   0   0   1   SR
#define Write_Roll_Address_Allow		0x03	//�������봹ֱ����ַ
#define Write_IRAM_Address_Allow		0x02	//��������IRAM��ַ������ָ����������趨CGRAM��ַ������ָ�

//������ʾ
// b7  b6  b5  b4  b3  b2  b1  b0
// 0   0   0   0   0   1   0   R0
#define Display_White_One				0x04	//��һ�з�����ʾ
#define Display_White_Two				0x05	//�ڶ��з�����ʾ

//˯��ģʽ
// b7  b6  b5  b4  b3  b2  b1  b0
// 0   0   0   0   1   SL  0   0
#define LCD_Sleep_Open					(0x08 | (1<<2))	//����˯��ģʽ
#define LCD_Sleep_Close					(0x08 | (0<<2))	//�˳�˯��ģʽ

//���书���趨
// b7  b6  b5  b4  b3  b2  b1  b0
// 0   0   1   DL  X   RE  G   X
#define MPU_4bit_Basic_Draw_Open		(Instruct_Basic4 | (1<<1))	//4bit MPU�ӿڣ�����ָ���ͼ��
#define MPU_4bit_Basic_Draw_Close		(Instruct_Basic4 | (0<<1))	//4bit MPU�ӿڣ�����ָ���ͼ��

#define MPU_8bit_Basic_Draw_Open		(Instruct_Basic8 | (1<<1))	//8bit MPU�ӿڣ�����ָ���ͼ��
#define MPU_8bit_Basic_Draw_Close		(Instruct_Basic8 | (0<<1))	//8bit MPU�ӿڣ�����ָ���ͼ��

#define MPU_8bit_Expansion_Draw_Open	(Instruct_Expansion8 | (1<<1))	//8bit MPU�ӿڣ�����ָ���ͼ��
#define MPU_8bit_Expansion_Draw_Close	(Instruct_Expansion8 | (0<<1))	//8bit MPU�ӿڣ�����ָ���ͼ��

#define MPU_4bit_Expansion_Draw_Open	(Instruct_Expansion4 | (1<<1))	//4bit MPU�ӿڣ�����ָ���ͼ��
#define MPU_4bit_Expansion_Draw_Close	(Instruct_Expansion4 | (0<<1))	//4bit MPU�ӿڣ�����ָ���ͼ��

//�趨IRAM��ַ�����ַ
// b7  b6  b5  b4  b3  b2  b1  b0
// 0   1   A5  A4  A3  A2  A1  A0
#define IRAM_Set						0x40	//����
//SR = 1��A5 ~ A0Ϊ��ֱ����ַ��SR = 0��A3 ~ A0ΪIRAM��ַ

//�趨��ͼRAM��ַ
// b7  b6  b5  b4  b3  b2  b1  b0
// 1   0   0   0   A3  A2  A1  A0 --->ˮƽ��ַ��Χ��A3 ~ A0
// 1   A6  A5  A4  A3  A2  A1  A0 --->��ֱ��ַ��Χ��A6 ~ A0
#define Draw_RAMSet						0x80	//����
//�趨GDRAM��ַ����ַ��������AC���������ô�ֱλ��������ˮƽλ�ã�����д��2�ֽ���������ɴ�ֱ��ˮƽ��������ã���


   








//PB5 - CS 
//PB7 - SDA 
//PB9 - SCL 
#define LCD12864_CS_H    GPIOB->ODR |=  BIT5;
#define LCD12864_CS_L    GPIOB->ODR &= ~BIT5;
#define LCD12864_SDA_H   GPIOB->ODR |=  BIT8;
#define LCD12864_SDA_L   GPIOB->ODR &= ~BIT8;
#define LCD12864_SCL_H   GPIOB->ODR |=  BIT9;
#define LCD12864_SCL_L   GPIOB->ODR &= ~BIT9;

#define LCD12864_Delay 20	//��ʱʱ�䳤��
#define CGRAM_Value   64	//д��CGRAM���� 

/**********************************************************
                      �ⲿ���ܺ���                      
**********************************************************/

void LCD12864_GPIO_Config(void);//GPIO��ʼ��

void LCD12864_GPIO_TEST(void);//���ſ��Ʋ���

void LCD12864_Init(void);	//LCD YB12864ZB��ʼ��

void LCD12864_Clear(void);	//LCD YB12864ZB����

void LCD12864_Write_Byte(u8 dat,u8 cmd);	//LCDд��һ���ֽ�����������ݣ��ж�æ��־

void LCD12864_Display_String(u8 x,u8 y,u8 *str,u8 len);	//LCD��ʾ�ַ���

void LCD12864_Display_String_Assign(u8 x,u8 y,u8 *str,u8 starsum,u8 len);	//LCD��ʾ�ַ��� ָ��λ�ó���

void LCD12864_Display_CGRAM(u8 x,u8 y,u8 add_h,u8 add_l);	//��ʾһ��CGRAM����

void LCD12864_Display_GDRAM(u8 x,u8 y,u8 width,u8 height,u8 *bmp,u8 dis_mode);	//LCDд��GDRAM��ͼ



#endif


/*********************************************************/

/*

 1��ָ���1��RE = 0������ָ���
  ___________________________________________________________________________
 |               |  �����ź�  |              �� �� �� ��              | ִ�� |
 |               |____________|_______________________________________| 	 |
 |   ָ������    | RS | R/W   | D7 | D6 | D5 | D4 | D3 | D2 | D1 | D0 | ʱ�� |     HEX
 |_______________|____|_______|____|____|____|____|____|____|____|____|______|
 |   �����ʾ    | 0  |   0   | 0  | 0  | 0  | 0  | 0  | 0  | 0  | 1  | 1.6ms|     0x01
 |_______________|____|_______|____|____|____|____|____|____|____|____|______|
 |   ��ַ��0     | 0  |   0   | 0  | 0  | 0  | 0  | 0  | 0  | 1  | X  | 72us |     0x02 //���ı�DDRAM����
 |_______________|____|_______|____|____|____|____|____|____|____|____|______|
 |  �����趨��   | 0  |   0   | 0  | 0  | 0  | 0  | 0  | 1  | I/D| S  | 72us |     0x06 �α�����    0x07 ������������
 |_______________|____|_______|____|____|____|____|____|____|____|____|______|     0x04 �α�����    0x05 ������������
 | ��ʾ��������  | 0  |   0   | 0  | 0  | 0  | 0  | 1  | D  | C  | S  | 72us |   D = 1; ������ʾ��  D = 0; ������ʾ�� // ���ı�DDRAM����     
 |_______________|____|_______|____|____|____|____|____|____|____|____|______|   C = 1; �α���ʾ��  C = 0; �α���ʾ��  S = 1; �α�λ����ʾ���� S = 0; ������ʾ     
 |   ��λ����    | 0  |   0   | 0  | 0  | 0  | 1  | S/C| R/L| X  | X  | 72us |     0x10 �α������ƶ� AC = AC - 1   0x14 �α������ƶ� AC = AC + 1
 |_______________|____|_______|____|____|____|____|____|____|____|____|______|     0x18 ��ʾ�����ƶ� AC = AC       0x1c ��ʾ�����ƶ� AC = AC
 |   �����趨    | 0  |   0   | 0  | 0  | 1  | DL | X  |0/RE| X  | X  | 72us |     0x30 8bit�ӿڣ�����ָ��   0x34 8bit�ӿڣ�����ָ��
 |_______________|____|_______|____|____|____|____|____|____|____|____|______|     0x20 4bit�ӿڣ�����ָ��   0x24 4bit�ӿڣ�����ָ�� 
 | �趨CGRAM��ַ | 0  |   0   | 0  | 1  | A5 | A4 | A3 | A2 | A1 | A0 | 72us |
 |_______________|____|_______|____|____|____|____|____|____|____|____|______| �趨CGRAM��ַ����ַ��������AC����AC��Χ0x00 ~ 0x3f����ȷ������ָ����SR = 0
 | �趨DDRAM��ַ | 0  |   0   | 1  | 0  | A5 | A4 | A3 | A2 | A1 | A0 | 72us |
 |_______________|____|_______|____|____|____|____|____|____|____|____|______| �趨DDRAM��ַ����ַ��������AC������һ��AC��Χ 0x80 ~ 0x8f    �ڶ���AC��Χ 0x90 ~ 0x9f 
 | ��æ��־�͵�ַ| 0  |   1   | BF | A6 | A5 | A4 | A3 | A2 | A1 | A0 | 72us |
 |_______________|____|_______|____|____|____|____|____|____|____|____|______|     BF = 0; //���Խ���������
 |  д��ʾ����   | 1  |   0   |                ��ʾ����               | 72us |
 |_______________|____|_______|_______________________________________|______| ÿ��RAM��CGRAM��DDRAM��IRAM����ַ����������д��2���ֽڣ��ڶ����ֽ�ʱAC�Զ�����1
 |  ����ʾ����   | 1  |   1   |                ��ʾ����               | 72us |
 |_______________|____|_______|_______________________________________|______| �趨RAM��CGRAM��DDRAM��IRAM����ַ����ҪDummy read���ٶ���һ�κ���ܶ�����ȷ����ʾ
                                                                               ���ݣ��ڶ��ζ�ȡ����Ҫ��Dummy read���ٶ���һ�Σ���������������RAM��ַ


 
 2��ָ���2��RE = 1������ָ���
  ___________________________________________________________________________
 |               |  �����ź�  |              �� �� �� ��              | ִ�� |
 |               |____________|_______________________________________| 	 |
 |   ָ������    | RS | R/W   | D7 | D6 | D5 | D4 | D3 | D2 | D1 | D0 | ʱ�� |     HEX
 |_______________|____|_______|____|____|____|____|____|____|____|____|______|
 |   ����ģʽ    | 0  |   0   | 0  | 0  | 0  | 0  | 0  | 0  | 0  | 1  | 72us |     0x01   ��ָ��ܸı�RAM����
 |_______________|____|_______|____|____|____|____|____|____|____|____|______|
 |   ����ַ��  | 0  |   0   | 0  | 0  | 0  | 0  | 0  | 0  | 1  | SR | 72us |
 |  RAM��ַѡ��  |    |       |    |    |    |    |    |    |    |    |      | ��ʼֵ0x02�� 0x03 �������봹ֱ����ַ
 |_______________|____|_______|____|____|____|____|____|____|____|____|______|              0x02 ��������IRAM��ַ������ָ����������趨CGRAM��ַ������ָ�
 |   ������ʾ    | 0  |   0   | 0  | 0  | 0  | 0  | 0  | 1  | R1 | R0 | 72us |
 |_______________|____|_______|____|____|____|____|____|____|____|____|______| ��ʼֵ0x04   0x05������ʾ
 |   ˯��ģʽ    | 0  |   0   | 0  | 0  | 0  | 0  | 1  | SL | X  | X  | 72us |
 |_______________|____|_______|____|____|____|____|____|____|____|____|______|     0x0c ����˯��ģʽ    0x08 ����˯��ģʽ
 |  ���书���趨 | 0  |   0   | 0  | 0  | 1  | DL | X  |1/RE| G  | 0  | 72us |
 |_______________|____|_______|____|____|____|____|____|____|____|____|______|
 |�趨IRAM��ַ�� | 0  |   0   | 0  | 1  | A5 | A4 | A3 | A2 | A1 | A0 | 72us |
 |   ����ַ    |    |       |    |    |    |    |    |    |    |    |      |     SR = 1;	//A5 ~ A0Ϊ��ֱ����ַ
 |_______________|____|_______|____|____|____|____|____|____|____|____|______|     SR = 0;	//A3 ~ A0ΪIRAM��ַ
 |               |    |       |    | 0  | 0  | 0  | A3 | A2 | A1 | A0 |      |
 |               |    |       |    |____|____|____|____|____|____|____|      | �趨GDRAM��ַ����������ַ��AC���������ô�ֱ��ַ������ˮƽ��ַ��
 |�趨��ͼRAM��ַ| 0  |   0   | 1  | A6 | A5 | A4 | A3 | A2 | A1 | A0 | 72us | ������д��2���ֽ���������ɴ�ֱ��ˮƽ��ַ���ã�
 |_______________|____|_______|____|____|____|____|____|____|____|____|______| ��ֱ��ַ��Χ��AC6 ~ AC0
                                                                               ˮƽ��ַ��Χ��AC3 ~ AC0
  
    ���书���趨��

	DL = 1;	//8bit MPU�ӿ�      DL = 0;	//4bit MPU�ӿ�

	RE = 1;	//����ָ��          RE = 0;	//����ָ��

	G = 1;	//��ͼ��ʾ��        G = 0;	// ��ͼ��ʾ��

*/
