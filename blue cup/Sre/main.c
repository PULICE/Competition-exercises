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
	LCD_Clear(Black);
	LCD_SetBackColor(Black);
	LCD_SetTextColor(White);
 	Usart_Init(9600);
  Adc_GPIOInit();
	SysTick_Config(SystemCoreClock/1000);
	Key_Init();
  GPIOC->ODR = 0XFFFF;
	LED_LOCK;
	LCD_DisplayStringLine(Line4,(uint8_t *)" B1:R");
	LCD_DisplayStringLine(Line5,(uint8_t *)" B2:R");
	LCD_DisplayStringLine(Line6,(uint8_t *)" B3:R");
	LCD_DisplayStringLine(Line7,(uint8_t *)" B4:R");
	LCD_DisplayStringLine(Line8,(uint8_t *)" LED:FF");
	while(1)
	{
		if(RXFLAG==1)
		{
    Compare_CMD();
		//初步猜测，gpioC端口高4位被LCD严重复用所以值随时都在改变，得想办法只在特殊时间读，之后就不读
		sprintf((char*)string," LED:%X",((GPIO_ReadInputData(GPIOC)>>8) & 0XFF));
		LEDvalue=((GPIO_ReadInputData(GPIOC)>>8) & 0XFF);
	  LCD_DisplayStringLine(Line8,string);
		Clear_Rxbuff();
		RXFLAG=0;
		}
		 KEY_B1();
		ucTmpeValue=ADC1_Conv();
		TmpeValue = ucTmpeValue*3.3/4096;
		sprintf((char*)string," ADC=%.2fV ",TmpeValue);
		LCD_DisplayStringLine(Line3,string);
 	}
}
	
