#include "timer.h"


uint16_t Timercont=0;
volatile bool  TCU_Flag ;//这是一个布尔类型变量，在通用头文件“stdbool.h”中定义并实现

void Timer_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef         NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 4999;//10ms中断一次
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	/* Prescaler configuration */
  TIM_PrescalerConfig(TIM3, 71, TIM_PSCReloadMode_Immediate);
	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	  /* Enable the TIM3 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  NVIC_Init(&NVIC_InitStructure);
  /* TIM IT enable */
  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
  /* TIM3 enable counter */
  TIM_Cmd(TIM3, ENABLE);	  

}
void TIM3_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
  {
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
//		TIM_Cmd(TIM3, DISABLE);
		if (TCU_Flag==1)
		{
			Timercont++;
      if(Timercont>2)
			{
			  	  RXcont=0;
					
  	        USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
            TCU_Flag = 0;		
				    Timercont = 0;
    	      USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
//				  printf("关闭串口中断");
			}
    }
  }
}
