#ifndef _COMPARE_H_
#define _COMPARE_H_


#include "stm32f10x.h"
#include "lcd.h"
#include "led.h"
#include "key.h"
#include "adc.h"
#include "usart.h"
#include "timer.h"
#include <stdio.h>


void Delay_Ms(u32 nTime);

void KEY_B1 (void);
void LED_CPY(uint16_t GPIO_Pin);
//static int SCpy (uint8_t* cpy1,uint8_t* cpy2);
void Compare_CMD (void);

/*测试字符串相加实现*/
uint16_t Compare_Number (uint8_t* test);

#endif



