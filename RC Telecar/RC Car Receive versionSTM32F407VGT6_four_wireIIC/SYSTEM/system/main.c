/*********************************************************************************************	 
*
*  �� �� ��   : main.c
*  ��������   : OLED IIC (ģ��IIC)�ӿ���ʾ����
*              ˵��: 
*              ----------------------------------------------------------------
*              GND   ��Դ��
*              VCC   ��5V��3.3v��Դ
*              SCL   ��PA7��SCL��
*              SDA   ��PA5��SDA��            
*              ----------------------------------------------------------------
*����f4 ��Ƶ̫����AHBԭʼʱ��OLED��������ס������OLED���ͺ���OLED_WR_Byte��ʱ��AHB���˽�Ƶ����.
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
	  delay_init(RCC_SYSCLK_Div8);	    	 //��ʱ������ʼ��	
	  uart_init(9600);
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ� 	LED_Init();			     //LED�˿ڳ�ʼ��
	 	OLED_Init();			//��ʼ��OLED  
		OLED_Clear(); 
		GPIO_433_INIT();
	  OLED_ShowString(0,0,(uint8_t*)"Please hold the locomotive while the equipment starts",150);
		
	  TIM2_PWM_Init(20000-1,84-1);	//84M/84=1Mhz�ļ���Ƶ��,��װ��ֵ20000������PWMƵ��Ϊ 1M/20000=50hz.    
		TIM3_PWM_Init(20000-1,84-1);

		OLED_Clear(); 
		OLED_ShowString(0,1,(uint8_t*)"   Ready to go",200);
		OLED_ShowString(15,4,(uint8_t*)"growupfanfan     @gmail.com",16);
	while(1) /*duoji 2100  1500  900   ʵ�ʲ��Է�����ת�����1000 - 1490С 1500ͣ  ��ת�����Ǵ�2000-1570С  ��ֵ��20000*/
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
		/*�������*/
		switch(vale[1])
		{	
//			case 0: TIM_SetCompare1(TIM2,20000-1500);
//			break;
			case's':TIM_SetCompare1(TIM2,20000-1500);
			break;
			case'a':TIM_SetCompare1(TIM2,20000-1400);//��С
			break;
			case'b':TIM_SetCompare1(TIM2,20000-900);
			break;
			case'c':TIM_SetCompare1(TIM2,20000-1600);//��С
			break;
			case'd':TIM_SetCompare1(TIM2,20000-2100);
			break;
		}
		/*���Ŵ�С*/
		switch(vale[2])
		{	
			case'e':	TIM_SetCompare1(TIM3,18500);
			break;
			case'f':TIM_SetCompare1(TIM3,20000-1420);
//				printf("ǰ��2\r\n");
			break;
			case'g':TIM_SetCompare1(TIM3,20000-800);
//			printf("ǰ��3\r\n");
			break;
			case'h':TIM_SetCompare1(TIM3,20000-1630);
//			printf("����\r\n");
			break;
		}
//		TIM_SetCompare1(TIM2,20000-900);
	} 	

}


