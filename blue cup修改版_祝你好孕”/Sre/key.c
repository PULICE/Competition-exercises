#include "key.h"

void Key_Init(void)
{
// 	EXTI_InitTypeDef   EXTI_InitStructure;
//   NVIC_InitTypeDef   NVIC_InitStructure;
	GPIO_InitTypeDef   GPIO_InitStructure;
	 /* Enable GPIOA clock **/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
  
  /* Configure PA.0 - 8 pin as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
   /* Configure PB.1 - 2 pin as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}
// //   /* Enable AFIO clock */
// //   RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

//   /* Connect EXTI0 Line to PA.00 pin */
//   GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
//   GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);
// 	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource2);
//   GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource8);
//   /* Configure EXTI0 line */
//   EXTI_InitStructure.EXTI_Line = EXTI_Line0|EXTI_Line8|EXTI_Line1|EXTI_Line2;
//   EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//   EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;  
//   EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//   EXTI_Init(&EXTI_InitStructure);
//   
// 	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
// 	
// 	  /* Enable and set EXTI0 Interrupt to the lowest priority */
//   NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
//   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
//   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
//   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//   NVIC_Init(&NVIC_InitStructure);

//   /* Enable and set EXTI0 Interrupt to the lowest priority */
//   NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
//   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
//   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
//   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//   NVIC_Init(&NVIC_InitStructure);

//   /* Enable and set EXTI0 Interrupt to the lowest priority */
//   NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
//   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
//   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
//   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//   NVIC_Init(&NVIC_InitStructure);

//   /* Enable and set EXTI0 Interrupt to the lowest priority */
//   NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
//   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
//   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
//   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//   NVIC_Init(&NVIC_InitStructure);

// }
// //中断服务函数
// void EXTI0_IRQHandler (void)
// {
//   if(EXTI_GetITStatus(EXTI_Line0) != RESET)
//   {
// //     /* Toggle LED2 */
// //      STM_EVAL_LEDToggle(LED2);
//       GPIOC->ODR = 0X40FF;
// 	    LED_LOCK;
//     /* Clear the  EXTI line 0 pending bit */
//     EXTI_ClearITPendingBit(EXTI_Line0);
//   }
// }

// void EXTI9_5_IRQHandler (void)
// {
//   if(EXTI_GetITStatus(EXTI_Line8) != RESET)
//   {
// //     /* Toggle LED2 */
// //      STM_EVAL_LEDToggle(LED2);
//       GPIOC->ODR = 0XaaFF;
// 	    LED_LOCK;
// 			printf("1\r\n");
//     /* Clear the  EXTI line 8 pending bit */
//     EXTI_ClearITPendingBit(EXTI_Line8);
//   }
// }
// void EXTI1_IRQHandler (void)
// {
//   if(EXTI_GetITStatus(EXTI_Line1) != RESET)
//   {
// //     /* Toggle LED2 */
// //      STM_EVAL_LEDToggle(LED2);
//       GPIOC->ODR = 0XffFF;
// 	    LED_LOCK;
//     /* Clear the  EXTI line 1 pending bit */
//     EXTI_ClearITPendingBit(EXTI_Line1);
//   }
// }
// void EXTI2_IRQHandler (void)
// {
//   if(EXTI_GetITStatus(EXTI_Line2) != RESET)
//   {

//       GPIOC->ODR = 0X70FF;
// 	    LED_LOCK;
//     /* Clear the  EXTI line 2 pending bit */
//     EXTI_ClearITPendingBit(EXTI_Line2);
//   }
// }


