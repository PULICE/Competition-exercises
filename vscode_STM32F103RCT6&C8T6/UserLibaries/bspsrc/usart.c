#include "usart.h"

uint8_t RXData[20];
static int RXcont=0;
static int Vale=0;
void uart_init(uint32_t Rate)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef  NVIC_InitStruct;
	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	//uart_configuration
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	//uart NVIC configuration
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStruct.NVIC_IRQChannel=USART2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	USART_InitStruct.USART_BaudRate=Rate;
	USART_InitStruct.USART_WordLength=USART_WordLength_8b;
	USART_InitStruct.USART_StopBits=USART_StopBits_1;
	USART_InitStruct.USART_Parity=USART_Parity_No;
	USART_InitStruct.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
  USART_Init(USART2,&USART_InitStruct);
	USART_Cmd(USART2,ENABLE);
	//uart IT_RX ENABLE
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	
}

//USART发送一个字符串，且无论什么条件一定会清零缓存数组
void UART_Sendstring(uint8_t *str)
{
	u8 tmpe=0;
	do
	{
	USART_SendData(USART2,(uint8_t)str[tmpe]);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
	tmpe++;
	}while(str[tmpe] !=RESET);
	Clear_Data();
}
//缓存数组清理函数/且清理完之后会打开uart的中断标志USART_IT_RXNE
void Clear_Data(void)
{
	uint8_t i=0;
	for(i=0;i<20;i++)//清理该数组的20个元素
	{
		RXData[i]=RESET;
	}
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
}
//该函数除了用来返回接收数据的大小，还用来使能USART2的中断
int Compare_String(void)
{
	int cont=RESET;
	cont=Vale;
	Vale=RESET;
  USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	return cont;
}
/*
*description:被定义在本文件中的全局变量RXData数组为方便使用也在uart.h
*            中使用extern关键字声明。
*/
//UART的中断接收函数，接收到的所有内容都会存放到定义在本文件的RXData数组中
void USART2_IRQHandler(void)
{
	uint8_t tmp=0;
	if(USART_GetITStatus(USART2,USART_IT_RXNE)==SET)
	{
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
		tmp=USART_ReceiveData(USART2);
		RXData[RXcont]=tmp;
		RXcont++;
		if(tmp=='\n')
		{
			RXData[RXcont-1]=RESET;
			RXData[RXcont-2]=RESET;
			Vale=RXcont-2;
			RXcont=RESET;
			USART_ITConfig(USART2,USART_IT_RXNE,DISABLE);
//  			UART_Sendstring(RXData);
		}
  }

		
}
