#include <stdio.h>
#include "stm32f10x.h"                  // Device header
#include "global.h"

#include "Bsp_Usart.h"

extern void USART2_Configuration(void);

void boardInit()
{
	RCC_ClocksTypeDef get_rcc_clock;
	USART_Configuration();
	USART2_Configuration();
	
	/*系统时钟初始化*/
	RCC_GetClocksFreq(&get_rcc_clock);
	printf("system -> Dynamic frequency take effect\r\n");
	printf("system -> SYSCLK_Frequency -> %d\r\n",get_rcc_clock.SYSCLK_Frequency);
	printf("system -> HCLK_Frequency -> %d\r\n",get_rcc_clock.HCLK_Frequency);
	printf("system -> PCLK1_Frequency -> %d\r\n",get_rcc_clock.PCLK1_Frequency);
	printf("system -> PCLK2_Frequency -> %d\r\n",get_rcc_clock.PCLK2_Frequency);
	printf("system -> ADCCLK_Frequency -> %d\r\n",get_rcc_clock.ADCCLK_Frequency);
	
}

void deviceInit()
{
	
}


void system_Init()
{
	boardInit();//板级资源初始化
	deviceInit();//设备初始化
}
