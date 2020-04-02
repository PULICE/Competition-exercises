#ifndef _USART_H_
#define _USRAT_H_



extern  unsigned char RxBuffer[20];
extern unsigned short RXFLAG ; 
void Clear_Rxbuff(void);
void UART_Sendstring(unsigned char *str);
void Usart_Init(unsigned int BaudRate );
#endif

