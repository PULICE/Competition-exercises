/*************************************************************/
/*以下是2020.3.26第一次模拟的源码
*
*在main.c文件中只有应用层的实现，具体的函数实体分布在不同的底层文件中
*所有底层文件统一放在Driver目录下，并按功能写入到相应的.c文件中
*
*其中compare.c中主要实现串口接收区数据与工程内置命令之间的比较
*K1按键长短按实现函数也在compare.c文件中实现
*而串口的中断处理函数为移植方便以写入到usart.c中，同时在该文件实现了printf重定位
*
*notes:如果要测试该工程的串口命令功能，无需回车，最小命令响应时间16ms；
****************************************************************/




#include <stdio.h>
#include "compare.h"

extern uint8_t  string[20];
extern uint32_t  LEDvalue;
extern float  ucTmpeValue ,TmpeValue;

//Main Body
int main(void)
{
  LED_LOCK_Init();
	STM3210B_LCD_Init();
	LCD_Clear(Blue2);
	
	LCD_SetTextColor(White);
 	Usart_Init(9600);
  Adc_GPIOInit();
	SysTick_Config(SystemCoreClock/1000);
	Key_Init();
	Timer_Init();
  GPIOC->ODR = 0XFFFF;
	LED_LOCK;
	LCD_SetBackColor(White);
	LCD_DisplayStringLine(Line0,(uint8_t *)"                        ");
	LCD_DisplayStringLine(Line1,(uint8_t *)"                        ");
	LCD_DisplayStringLine(Line9,(uint8_t *)"                        ");

	LCD_SetBackColor(Blue2);
	LCD_DisplayStringLine(Line4,(uint8_t *)" B1:R");
	LCD_DisplayStringLine(Line5,(uint8_t *)" B2:R");
	LCD_DisplayStringLine(Line6,(uint8_t *)" B3:R");
	LCD_DisplayStringLine(Line7,(uint8_t *)" B4:R");
	LCD_DisplayStringLine(Line8,(uint8_t *)" LED:00");
	
	while(1)
	{
		printf("test");
		/*下面测试数组能否通过相加后得出一个确定的值*************/
		if(RXFLAG==1)
		{
		  Compare_CMD();			
		printf("Value= %d\r\n",Compare_Number(RxBuffer));
		printf ((char *)RxBuffer);
//		printf ("\r\n");
		//初步猜测，gpioC端口高4位被LCD严重复用所以值随时都在改变，得想办法只在特殊时间读，之后就不读

			LEDvalue=((GPIO_ReadInputData(GPIOC)>>8) & 0XFF);
			sprintf((char*)string," LED:%02X",(~LEDvalue)&0xFF);
			LCD_DisplayStringLine(Line8,string);

			Clear_Rxbuff();
			RXFLAG=0;
		}
		KEY_B1();
		ucTmpeValue=ADC1_Conv();
		TmpeValue = ucTmpeValue*3.3/4096;
		sprintf((char*)string," ADC=%.2fV ",TmpeValue);
		LCD_DisplayStringLine(Line3,string);
		Delay_Ms(100);

 	}
}
