#ifndef __HMI_H
#define __HMI_H

/*页面号定义*/
#define WELCOME 0 
#define INPUT_FINISH 9 
#define ADMIN_PAGE 11
/*数据帧的帧头场*/
#define STR_LEN 2		//数据长度的数据在数据帧中的位置
#define HMI_CMD 3		//控制命令在数据帧中的位置
#define ADD_H 	4		//高地址在数据帧中的位置
#define ADD_L 	5		//低地址数据在数据帧中的位置

#define ADD 	4		//地址数据在数据帧中的位置

/*数据帧中的命令场*/
#define REG_W 0X80		//寄存器写入命令
#define REG_R 0X81		//寄存器读取命令
#define VAR_W 0X82		//变量写入命令
#define VAR_R 0X83		//变量读取命令

/*数据帧中的地址场*/
//1.系统寄存器区
#define VERSION_PTR 0X00
#define BZ_TIME_PTR 0X02
#define PIC_ID_PTR 	0X03
#define RTC_PTR 	0X20				//RTC读寄存器在HMI中的位置
#define TOUCH_CORRECT_PTR	0xEA	//触摸校准寄存器

/*数据帧中的地址场*/
//2.用户自定义数据区
#define WELCOME_CACHE 0X0000	//欢迎语的缓存位置
#define ORDER_PTR     0X0020	//单号地址
#define PWD_PTR       0X0030	//密码地址
#define ADMIN_PTR     0X0040	//管理员密码地址
#define USABLE_PTR	  0X0050	//可用餐柜数量
#define ICON_PTR	  0X0060	//图标地址
#define BOXNUM_PTR	  0X0070	//当前餐柜号
#define SENDSTA_PTR	  0X00A0	//发送状态



#define CONTACT_INF_SET_PTR		0X0080		//联系方式修改
#define GROUP_NUM_SET_PTR		0X0090		//柜组号修改
#define ADMIN_PWD_SET_PTR		0X00B0		//管理员密码修改
#define	OPEN_BOX_SET_PTR		0X00C0		//开任意柜门
#define LATEST_TIME_SET_PTR		0X00D0		//最晚取餐时间修改
#define ADMIN_KEY_RETURN		0X00E0		//管理员洁面的按键返回位置

//管理员界面的按键返回值定义
#define ADMIN_TOUCH_CORRECT		0X01		//触摸校准
#define ADMIN_RESTART_SYS		0X02		//重启系统
#define ADMIN_HISTORY			0X03		//历史记录
#define ADMIN_RESERVE			0X04		//保留


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
void Hmi_Page_Switch(u8 id);//页面切换
void Hmi_Send(u16 send_add,u8 send_cmd,u8 *send_data,u8 start_ptr,u8 send_len);
void Hmi_Send_reg(u16 send_add,u8 send_cmd,u8 *send_data,u8 start_ptr,u8 send_len);
u8 Hmi_RTC_Read(void);//读取HMI内部的RTC
#endif
