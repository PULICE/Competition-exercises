#include "global.h"
#include "steering_engine.h"

void TIM_Config(uint16_t Channel2Pulse)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef        TIM_OCInitStructure;
	
	/* TIM2 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);		
	
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 20000;  //1KHz
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	//TIM2预分频设置:1MHZ,APB1分频系数2，输入到TIM3时钟为36MHzx2 = 72MHz  
	TIM_PrescalerConfig(TIM4,71, TIM_PSCReloadMode_Immediate);		

	/* Channel 2 and 3 Configuration in PWM mode */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_Pulse = Channel2Pulse;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;	
	
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);//PB9  TIME4  CH4
	//使能TIM2定时计数器
	TIM_Cmd(TIM4, ENABLE);
	//使能TIM2 PWM输出模式
	TIM_CtrlPWMOutputs(TIM4, ENABLE);
}

/**
  * @说明     PWM输出模式 PA1(TIM2-CH2)、PA2(TIM2-CH3)引脚配置
  * @参数     None 
  * @返回值   None
  */
void PWM_IO_Config(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//		GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2,ENABLE);
//    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

}
