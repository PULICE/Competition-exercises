/*********************************************************************************************	 
*
*  文 件 名   : main.c
*  功能描述   : OLED IIC (模拟IIC)接口演示例程
*              说明: 
*              ----------------------------------------------------------------
*              GND   电源地
*              VCC   接5V或3.3v电源
*              SCL   接PA7（SCL）
*              SDA   接PA5（SDA）            
*              ----------------------------------------------------------------
*由于f4 主频太高用AHB原始时钟OLED根本扛不住所以在OLED发送函数OLED_WR_Byte暂时对AHB做了降频处理.
***********************************************************************************************/
#include "delay.h"
#include "sys.h"
#include "oled.h"
#include "usart.h"
#include "bmp.h"                                                   
#include "spi.h"
#include "pwm.h"

u8 vale[3];
uint8_t  UART_BUFFER[64]={"TEST"},RF24L01RxBuffer[64]={0};
 int main(void)
{
	  delay_init(RCC_SYSCLK_Div8);	    	 //延时函数初始化	
	  uart_init(9600);
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级 	LED_Init();			     //LED端口初始化
	 	OLED_Init();			//初始化OLED  
		OLED_Clear(); 
		GPIO_433_INIT();
	  OLED_ShowString(0,0,(uint8_t*)"Please hold the locomotive while the equipment starts",150);
		
	  TIM2_PWM_Init(20000-1,84-1);	//84M/84=1Mhz的计数频率,重装载值20000，所以PWM频率为 1M/20000=50hz.    
		TIM3_PWM_Init(20000-1,84-1);

		OLED_Clear(); 
		OLED_ShowString(0,1,(uint8_t*)"   Ready to go",200);
		OLED_ShowString(15,4,(uint8_t*)"growupfanfan     @gmail.com",16);
	while(1) /*duoji 2100  1500  900   实际测试发现正转区间大1000 - 1490小 1500停  反转区间是大2000-1570小  分值是20000*/
	{	
//			if(GPIO_ReadInputDataBit(GPIOA,  GPIO_Pin_4)==0)
//			{
//				USART_ITConfig(USART1,USART_IT_RXNE,DISABLE);
//					delay_us(4990);
//	  		USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
// //      usart_Buffclear();
//			}
				vale[1]=USART_RX_BUF[1];	
				vale[2]=USART_RX_BUF[2];
				usart_Buffclear();
		/*舵机方向*/
		switch(vale[1])
		{	
//			case 0: TIM_SetCompare1(TIM2,20000-1500);
//			break;
			case's':TIM_SetCompare1(TIM2,20000-1500);
			break;
			case'a':TIM_SetCompare1(TIM2,20000-1400);//左小
			break;
			case'b':TIM_SetCompare1(TIM2,20000-900);
			break;
			case'c':TIM_SetCompare1(TIM2,20000-1600);//右小
			break;
			case'd':TIM_SetCompare1(TIM2,20000-2100);
			break;
		}
		/*油门大小*/
		switch(vale[2])
		{	
			case'e':	TIM_SetCompare1(TIM3,18500);
			break;
			case'f':TIM_SetCompare1(TIM3,20000-1420);
//				printf("前进2\r\n");
			break;
			case'g':TIM_SetCompare1(TIM3,20000-800);
//			printf("前进3\r\n");
			break;
			case'h':TIM_SetCompare1(TIM3,20000-1630);
//			printf("倒车\r\n");
			break;
		}
//		TIM_SetCompare1(TIM2,20000-900);
	} 	

}


