#ifndef _TIMER_H_
#define _TIMER_H_

#include "stm32f10x_tim.h"
#include "stdbool.h"
#include "usart.h"
#include "stdio.h"
#include "led.h"

//#define PrescalerValue       71
extern volatile bool TCU_Flag ;

void Timer_Init(void);
#endif
