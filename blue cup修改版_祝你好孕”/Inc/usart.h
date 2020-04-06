#ifndef _URAT_H_
#define _URAT_H_

#include "misc.h"
#include <stdio.h>
#include "timer.h"

extern  unsigned char RxBuffer[20];
extern unsigned short RXFLAG ; 
extern volatile uint32_t RXcont;

void Clear_Rxbuff(void);
void UART_Sendstring(unsigned char *str);
void Usart_Init(unsigned int BaudRate );

#endif

