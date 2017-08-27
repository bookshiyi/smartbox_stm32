#ifndef __HMI_H
#define __HMI_H

/*ҳ��Ŷ���*/
#define WELCOME 0 
#define INPUT_FINISH 9 
#define ADMIN_PAGE 11
/*����֡��֡ͷ��*/
#define STR_LEN 2		//���ݳ��ȵ�����������֡�е�λ��
#define HMI_CMD 3		//��������������֡�е�λ��
#define ADD_H 	4		//�ߵ�ַ������֡�е�λ��
#define ADD_L 	5		//�͵�ַ����������֡�е�λ��

#define ADD 	4		//��ַ����������֡�е�λ��

/*����֡�е����*/
#define REG_W 0X80		//�Ĵ���д������
#define REG_R 0X81		//�Ĵ�����ȡ����
#define VAR_W 0X82		//����д������
#define VAR_R 0X83		//������ȡ����

/*����֡�еĵ�ַ��*/
//1.ϵͳ�Ĵ�����
#define VERSION_PTR 0X00
#define BZ_TIME_PTR 0X02
#define PIC_ID_PTR 	0X03
#define RTC_PTR 	0X20				//RTC���Ĵ�����HMI�е�λ��
#define TOUCH_CORRECT_PTR	0xEA	//����У׼�Ĵ���

/*����֡�еĵ�ַ��*/
//2.�û��Զ���������
#define WELCOME_CACHE 0X0000	//��ӭ��Ļ���λ��
#define ORDER_PTR     0X0020	//���ŵ�ַ
#define PWD_PTR       0X0030	//�����ַ
#define ADMIN_PTR     0X0040	//����Ա�����ַ
#define USABLE_PTR	  0X0050	//���ò͹�����
#define ICON_PTR	  0X0060	//ͼ���ַ
#define BOXNUM_PTR	  0X0070	//��ǰ�͹��
#define SENDSTA_PTR	  0X00A0	//����״̬



#define CONTACT_INF_SET_PTR		0X0080		//��ϵ��ʽ�޸�
#define GROUP_NUM_SET_PTR		0X0090		//������޸�
#define ADMIN_PWD_SET_PTR		0X00B0		//����Ա�����޸�
#define	OPEN_BOX_SET_PTR		0X00C0		//���������
#define LATEST_TIME_SET_PTR		0X00D0		//����ȡ��ʱ���޸�
#define ADMIN_KEY_RETURN		0X00E0		//����Ա����İ�������λ��

//����Ա����İ�������ֵ����
#define ADMIN_TOUCH_CORRECT		0X01		//����У׼
#define ADMIN_RESTART_SYS		0X02		//����ϵͳ
#define ADMIN_HISTORY			0X03		//��ʷ��¼
#define ADMIN_RESERVE			0X04		//����


typedef struct
{
	u8 WAIT[2];
	u8 PWD_ERROR[2];
	u8 PWD_RIGHT[2];
	u8 ORDER_ERROR[2];
	u8 ORDER_RIGHT[2];
	u8 ADMIN_ERROR[2];
	u8 ADMIN_RIGHT[2];
	u8 MODI_FINISH[2];
}Icon_Data_TypeDef;
//ICON_TYPE_DEF ICON_VAL;


u8 Hmi_Init(u8 num);
void Hmi_Page_Switch(u8 id);//ҳ���л�
void Hmi_Send(u16 send_add,u8 send_cmd,u8 *send_data,u8 start_ptr,u8 send_len);
void Hmi_Send_reg(u16 send_add,u8 send_cmd,u8 *send_data,u8 start_ptr,u8 send_len);
u8 Hmi_RTC_Read(void);//��ȡHMI�ڲ���RTC
#endif
