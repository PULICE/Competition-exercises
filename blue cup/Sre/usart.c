#include "usart.h"
#include "stm32f10x.h"
#include "misc.h"
#include <stdio.h>


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/


unsigned  char  RxBuffer[20];
unsigned  short  RXFLAG = 0; 
static volatile unsigned char RXcont=0;
/* Private variables ---------------------------------------------------------*/

void Usart_Init(unsigned int BaudRate )
{
  USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_AFIO, ENABLE);
	
	/* Configure USARTy Rx as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* Configure USARTy Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

	
  USART_InitStructure.USART_BaudRate = BaudRate;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
   
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  /* Enable the USARTx Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  USART_Init(USART2, &USART_InitStructure);
  USART_Cmd(USART2,ENABLE);

	/* Enable the EVAL_COM1 Receive interrupt: this interrupt is generated when the 
  EVAL_COM1 receive data register is not empty */
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
  USART_ITConfig(USART2, USART_FLAG_TXE, ENABLE);
}
#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART2, (uint8_t) ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);

  return ch;
}

void Clear_Rxbuff(void)
{
		unsigned char i=0;
		for (i=0;i<20;i++)
		 {
			 RxBuffer[i]=0;
		 }
//	 	printf ("串口已经被清理目前值：%s\r\n",RxBuffer);
	  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
//	  printf ("打开接收中断\r\n");
}
//USART发送一个字符串，且无论什么条件一定会清零缓存数组
void UART_Sendstring(unsigned char *str)
{
	u8 tmpe=0;
	do
	{
	USART_SendData(USART2,(uint8_t)str[tmpe]);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
	tmpe++;
	}while(str[tmpe] !=RESET);	
}

void USART2_IRQHandler(void)
{
	uint8_t temp = 0;

  if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
  {
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
    temp= USART_ReceiveData(USART2);
		RxBuffer[RXcont]=temp;
		RXcont++;
		RXFLAG =1;
				if(temp == '\n')
				{
				 RXcont=0;
				 temp = 0;
//				 printf ("禁止接收中断\r\n");
				 USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
					
  			// UART_Sendstring(RxBuffer);
				}	
  }
  
}
