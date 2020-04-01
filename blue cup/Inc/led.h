#ifndef _LED_H_
#define _LED_H_

#include "stm32f10x.h"
void LED_LOCK_Init(void);

#define LED_LOCK 	   {    \
											 GPIOD->ODR |=(1<<2);\
											 GPIOD->ODR &=~(1<<2);\
										 }
										 
#endif

