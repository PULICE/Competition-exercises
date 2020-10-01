/****************************************************************************
*
* 文件名: usart_printf.c
* 内容简述: 本模块实现printf和scanf函数重定向到串口1
*	实现重定向，只需要添加2个函数
		int fputc(int ch, FILE *f);
		int fgetc(FILE *f);
*
*
*/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include <stdio.h>

#pragma import(__use_no_semihosting)
_sys_exit(int x)
{
x = x;
}
struct __FILE
{
int handle;
};
 
FILE __stdout;
int fputc(int ch, FILE *f)
{
USART_SendData(USART1, (uint8_t) ch);

	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
	{}

	return ch;
}

/*******************************************************************************
	函数名：PrintfLogo
	输  入: 例程名称和例程最后更新日期
	输  出:
	功能说明：
*/
void PrintfLogo(char *strName, char *strDate)
{
	printf("*************************************************************\n\r");
	printf("* Example Name : %s\r\n", strName);
	printf("* Update Date  : %s\r\n", strDate);
	printf("* StdPeriph_Lib Version : V3.1.2\n\r");
	printf("* \n\r");
	printf("*************************************************************\n\r");
}

/*******************************************************************************
	函数名：USART_Configuration
	输  入:
	输  出:
	功能说明：
	初始化串口硬件设备，未启用中断。
	配置步骤：
	(1)打开GPIO和USART的时钟
	(2)设置USART两个管脚GPIO模式
	(3)配置USART数据格式、波特率等参数
	(4)最后使能USART功能
*/
void USART_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* 第1步：打开GPIO和USART部件的时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	/* 第2步：将USART Tx的GPIO配置为推挽复用模式 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* 第3步：将USART Rx的GPIO配置为浮空输入模式
		由于CPU复位后，GPIO缺省都是浮空输入模式，因此下面这个步骤不是必须的
		但是，我还是建议加上便于阅读，并且防止其它地方修改了这个口线的设置参数
	*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/*  第3步已经做了，因此这步可以不做
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	*/
	GPIO_Init(GPIOA, &GPIO_InitStructure);


	/* 第4步：配置USART参数
	    - BaudRate = 115200 baud
	    - Word Length = 8 Bits
	    - One Stop Bit
	    - No parity
	    - Hardware flow control disabled (RTS and CTS signals)
	    - Receive and transmit enabled
	*/
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);

	/* 第5步：使能 USART， 配置完毕 */
	USART_Cmd(USART1, ENABLE);
	printf(" \r\n");
	/* CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去
		如下语句解决第1个字节无法正确发送出去的问题 */
	//USART_ClearFlag(USART1, USART_FLAG_TC);     /* 清发送完成标志，Transmission Complete flag */
}

void USART2_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStrue;
	USART_InitTypeDef USART_InitStrue;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	// 外设使能时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	USART_DeInit(USART2);  //复位串口2 -> 可以没有
	
	// 初始化 串口对应IO口  TX-PA2  RX-PA3
	GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStrue.GPIO_Pin=GPIO_Pin_2;
	GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStrue);
	
	GPIO_InitStrue.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStrue.GPIO_Pin=GPIO_Pin_3;
  GPIO_Init(GPIOA,&GPIO_InitStrue);
	
	// 初始化 串口模式状态
	USART_InitStrue.USART_BaudRate=115200; // 波特率
	USART_InitStrue.USART_HardwareFlowControl=USART_HardwareFlowControl_None; // 硬件流控制
	USART_InitStrue.USART_Mode=USART_Mode_Tx|USART_Mode_Rx; // 发送 接收 模式都使用
	USART_InitStrue.USART_Parity=USART_Parity_No; // 没有奇偶校验
	USART_InitStrue.USART_StopBits=USART_StopBits_1; // 一位停止位
	USART_InitStrue.USART_WordLength=USART_WordLength_8b; // 每次发送数据宽度为8位
	USART_Init(USART2,&USART_InitStrue);
	
	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_0);
	
	USART_ITConfig(USART2,USART_IT_IDLE,ENABLE);
	NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	USART_Cmd(USART2,ENABLE);//使能串口
	/*USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);//开启接收中断
	
	// 初始化 中断优先级
	NVIC_InitStrue.NVIC_IRQChannel=USART2_IRQn;
	NVIC_InitStrue.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStrue.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStrue.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStrue);*/

}


/*******************************************************************************
	函数名：fputc
	输  入:
	输  出:
	功能说明：
	重定义putc函数，这样可以使用printf函数从串口1打印输出
*/


/*******************************************************************************
	函数名：fputc
	输  入:
	输  出:
	功能说明：
	重定义getc函数，这样可以使用scanff函数从串口1输入数据
*/


void Usart2_sendBuf(unsigned char* buf,int len)
{
	int i=0;
	for(;i<len;i++)
	{
		while((USART2->SR&0X40)==0);
		USART2->DR = buf[i];  
	}
}

