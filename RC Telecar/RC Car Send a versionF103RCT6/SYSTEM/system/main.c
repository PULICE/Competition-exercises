/********************************************************************************	 
*
*  �� �� ��   : main.c
*  ��������   :RCң��������
*              ˵��: 
*              ----------------------------------------------------------------
*              GND   ��Դ��
*          
*             -----------------------------------------------------------------
********************************************************************************/
#include "delay.h"
#include "sys.h"
#include "adc.h"
#include "oled.h"
#include "bmp.h"
#include "usart.h"
char string1[3];
#define Direction    ADC1_Conv(1)/10
#define Accelerator  ADC1_Conv(6)/10
 int main(void)
 {
	 int i=0,Avale=0,vale=0;
		delay_init();	    	     //��ʱ������ʼ��	  
		NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ� 	LED_Init();			     //LED�˿ڳ�ʼ��
		OLED_Init();			       //��ʼ��OLED 
	  OLED_Clear();
	  Adc_GPIOInit();
		LED_Init();
	 	uart_init(9600);
		OLED_ShowString(15,0,(uint8_t *)"433Mhz_ready",80);
		OLED_ShowString(0,3,(uint8_t *)"High density    equipment",80);
		while(1) 
		{	
			Avale=Accelerator;
			/*ADC���ݲɼ����Ų���   S18-22 L13-17 H12-10 D27-80*/
			if((Avale<26)&&(Avale>18))//S
			{
					string1[2]='e';
			}
		  if((Avale<17)&&(Avale>13))//L
			{
					string1[2]='f';
			}
			if((Avale<12)&&(Avale>10))//H
			{
					string1[2]='g';
			}
			if((Avale<80)&&(Avale>27))//D
			{
					string1[2]='h';
			}
			vale=Direction;
			/*ADC���ݲɼ����򲿷�   S18-23 DR14-17 HR13-10 DL25-30 HL31-95*/
			if((vale<23)&&(vale>18))//S
			{
					string1[1]='s';
			}
				if((vale<17)&&(vale>14))//DR
			{
					string1[1]='a';
			}
			if((vale<13)&&(vale>10))//HR
			{
					string1[1]='b';
			}
			if((vale<34)&&(vale>25))//DL
			{
					string1[1]='c';
			}
			if((vale<95)&&(vale>35))//HL
			{
					string1[1]='d';
			}
					
			/*������Ӧ����*/
			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)==0)
			{
				delay_ms(10);
				if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)==0)
				{
					for(i=0;i<40;i++)
					{
						GPIO_SetBits(GPIOA,GPIO_Pin_8);
						delay_us(400);
						GPIO_ResetBits(GPIOA,GPIO_Pin_8);
						delay_us(400);
					}
					delay_ms(500);
				}
			}
			
			
			/*433Mhz���ݷ��Ͳ���*/               /*ֻ�����һ�ν��յ������ݣ��ҵ��뷨��дһ���жϺ����ж��ĸ�ͨ��ֵ�ı�ͷ��ĸ���
			��˵���ջ��Ǳ��������ֵ����0��*//*sͣL��D��H��*/
			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)==1)
			{
			  GPIO_ResetBits(GPIOC,GPIO_Pin_2);
				printf("%sx",string1);//�ź�
				sprintf((char *)string1,"RL:%d",vale);
				OLED_ShowString(10,7,(uint8_t *)string1,80);
				sprintf((char *)string1,"AA:%d",Avale);
				OLED_ShowString(60,7,(uint8_t *)string1,80);

			}

			/*led��Ӧ����*/
			GPIO_SetBits(GPIOC,GPIO_Pin_2);
			GPIO_ResetBits(GPIOC,GPIO_Pin_1);

			}	
}
