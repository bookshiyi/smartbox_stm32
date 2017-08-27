/****************************************Copyright (c)**************************************************                         
**
**                                 http://www.powermcu.com
**
**--------------File Info-------------------------------------------------------------------------------
** File name:			24C02.h
** Descriptions:		24C02 操作函数库 
**
**------------------------------------------------------------------------------------------------------
** Created by:			AVRman
** Created date:		2010-10-29
** Version:				1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/
#ifndef __24C02_H
#define __24C02_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Private define ------------------------------------------------------------*/
#define SCL_H         GPIOB->BSRR = GPIO_Pin_6	 /* GPIO_SetBits(GPIOB , GPIO_Pin_6)   */
#define SCL_L         GPIOB->BRR  = GPIO_Pin_6   /* GPIO_ResetBits(GPIOB , GPIO_Pin_6) */
   
#define SDA_H         GPIOB->BSRR = GPIO_Pin_7	 /* GPIO_SetBits(GPIOB , GPIO_Pin_7)   */
#define SDA_L         GPIOB->BRR  = GPIO_Pin_7	 /* GPIO_ResetBits(GPIOB , GPIO_Pin_7) */

#define SCL_read      GPIOB->IDR  & GPIO_Pin_6   /* GPIO_ReadInputDataBit(GPIOB , GPIO_Pin_6) */
#define SDA_read      GPIOB->IDR  & GPIO_Pin_7	 /* GPIO_ReadInputDataBit(GPIOB , GPIO_Pin_7) */

#define I2C_PageSize  8  /* 24C02每页8字节 */

#define ADDR_24LC02		0xA0

//SmartBox中 将一些常用的参数存储在flash中，其存储位置如下
#define ADMIN_PWD_FLASH_PTR 	0X10
#define CONTACT_INF_FLASH_PTR 	0X20
#define GROUP_NUM_FLASH_PTR 	0X30
#define LATEST_TIME_FLASH_PTR 	0X40

//声明为全局变量，随处可以调用
extern uint8_t WriteBuffer[255],ReadBuffer[255];
/* Private function prototypes -----------------------------------------------*/
void I2C_Configuration(void);
FunctionalState I2C_WriteByte(uint8_t SendByte, uint16_t WriteAddress, uint8_t DeviceAddress);
FunctionalState I2C_ReadByte(uint8_t* pBuffer,   uint16_t length,   uint16_t ReadAddress,  uint8_t DeviceAddress);
void I2C_Test(void);
u8 I2C_EEPROM_Init(void);
#endif 
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
