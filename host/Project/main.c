/********************   One Artisan's Regret And Pride   ***********************
*
*	@File    	： main.c
*	@Brief   	： 智能柜主函数所在
*	@Author  	： Blade@oarap.org
*	@Hardware	： STM32F103RCT6
*	@Date		： 2016-05
*	@Description： 
*	@History	： 
*
*	Rev1.0 
*		Date：2016-05-16
*		Author：Blade
*		Modification：更新可用餐柜函数，更新开箱门算法，规范注释系统
*	Rev1.1
*		Date：2016-06-05
*		Author：Blade
*		Modification：函数、变量的每个单词首字母大写 宏定义全部大写 
*--------------------Copyright (C) oarap.org 2016------------------------------
*----------------------All rights reserved-------------------------------------
*
********************          一名工匠的遗憾与骄傲          ********************/

#include "includes.h"

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/****************************************************************************
*	@Function name	： 主函数
*	@Description    ： 
*	@Author  		： Blade@oarap.org
*	@Date			： 2016-05
*	@param    		： 无
*	@return			： 无
*****************************************************************************/
int main(void)
{	
	Sys_Init();
	while (1)
	{	
		//如果同时有两个中断请求，程序员可以自定义处理的顺序，因为不同的请求的处理速度有所不同
	  Main_Process();
	  Delay_ms(10);
	}
}

/**
  * @brief  重定向printf函数 Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART2, (uint8_t) ch);
  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
  {}
  return ch;
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* Infinite loop */
  while (1)
  {
  }
}
#endif



/*********************************************************************************************************
      END FILE
*********************************************************************************************************/





