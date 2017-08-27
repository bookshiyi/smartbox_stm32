#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"


/*������stm32f103c8t6���İ�
				stm32f103rct6���İ�
				stm32f103vet6���İ�
				stm32f103vct6���İ�
				stm32f103zet6���İ�
				stm32f103rbt6���İ�
				stm32f103rct6mini���İ� 
				stm32f103rbt6mini���İ� */
#define LED1_GPIO_RCC           RCC_APB2Periph_GPIOC
#define LED1_GPIO_PORT          GPIOC
#define LED1_GPIO_PIN      			GPIO_Pin_13
#define LED1_ONOFF(x)     			GPIO_WriteBit(LED1_GPIO_PORT,LED1_GPIO_PIN,x);

/*������stm32f103zet6mini���İ�  */
#define LED2_GPIO_RCC           RCC_APB2Periph_GPIOG
#define LED2_GPIO_PORT          GPIOG
#define LED2_GPIO_PIN      			GPIO_Pin_15
#define LED2_ONOFF(x)     			GPIO_WriteBit(LED2_GPIO_PORT,LED2_GPIO_PIN,x);

/*������stm32f103vct6mini���İ� 
				stm32f103vet6mini���İ� */

#define LED3_GPIO_RCC           RCC_APB2Periph_GPIOB
#define LED3_GPIO_PORT          GPIOB
#define LED3_GPIO_PIN      			GPIO_Pin_9
#define LED3_ONOFF(x)      			GPIO_WriteBit(LED3_GPIO_PORT,LED3_GPIO_PIN,x);

void LED_GPIO_Config(void);	
void LEDXToggle(uint8_t ledx);
#endif
