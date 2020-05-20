//////////////////////////////////////////////////////////////////////////////////	 
//
//  �� �� ��   : main.c
//  ��������   : OLED IIC (ģ��IIC)�ӿ���ʾ����
//              ˵��: 
//              ----------------------------------------------------------------
//              GND   ��Դ��
//              VCC   ��5V��3.3v��Դ
//              SCL   ��PA5��SCL��
//              SDA   ��PA7��SDA��            
//              ----------------------------------------------------------------
//******************************************************************************/

#include "delay.h"
#include "sys.h"
#include "oled.h"
#include "bmp.h"
#include "usart.h"
#include "spi.h"
#include "adc.h"
#include "RF24L01.h"
void LED_test(void);
char *content = "y";
char string[40];
uint8_t  UART_BUFFER[64]={"TEST"},RF24L01RxBuffer[64]={0};
 int main(void)
 {
		delay_init();	    	     //��ʱ������ʼ��	  
		NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ� 	LED_Init();			     //LED�˿ڳ�ʼ��
		OLED_Init();			       //��ʼ��OLED 
	  OLED_Clear();
	  Adc_GPIOInit();
		LED_test();
	 	uart_init(115200);
	 	drv_spi_init( );
		NRF24L01_Gpio_Init( );     //RF24L01���ų�ʼ��
	  printf("Check the connection....\r\n");
		NRF24L01_check( );	 //���nRF24L01		
	  printf("Check the end of connection\r\n");
		RF24L01_Init( );
		OLED_Clear(); 
	  RF24L01_Set_Mode( MODE_TX );		//����ģʽ
		while(1) 
		{	
			int i=0;
			/*������OLED��ʾ����*/
			delay_ms(100);
			OLED_ShowString(0,7,(uint8_t*)string,200);/*�����200����OLED����ʾ�Ĵ�С*/
			sprintf(string," Channel 1: %d ",ADC1_Conv(1));
			OLED_ShowString(0,1,(uint8_t*)string,200);
			sprintf(string," Channel 2: %d ",ADC1_Conv(2));
			OLED_ShowString(0,3,(uint8_t*)string,200);
			sprintf(string," Channel 3: %d ",ADC1_Conv(3)-3000);
			OLED_ShowString(0,5,(uint8_t*)string,200);
			sprintf(string," Channel 4: %d ",ADC1_Conv(6)-3000);
			if ((ADC1_Conv(2)>320)||(ADC1_Conv(2)<200))
			{
		/*��Դ������ʵ�ֲ���*/
		for(i=0;i<40;i++)
		{
			GPIO_SetBits(GPIOA,GPIO_Pin_8);
			delay_us(ADC1_Conv(2));
			GPIO_ResetBits(GPIOA,GPIO_Pin_8);
			delay_us(ADC1_Conv(2));
		}
	}
//			OLED_DrawBMP(0,0,128,8,BMP1);  //ͼƬ��ʾ(ͼƬ��ʾ���ã����ɵ��ֱ�ϴ󣬻�ռ�ý϶�ռ䣬FLASH�ռ�8K��������)
	/*����ʵ�ֲ���*/		
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)==0)
			{
				delay_ms(10);
				if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)==0)
				{
					/*NRF24L01_TxPacket�ú����Ƿ���һ������*/
			    NRF24L01_TxPacket( UART_BUFFER , 7 );		/*ģʽ1���͹̶��ַ�,1Sһ���������Ǹ�7�Ƿ��Ͷ��ٸ��ֽ�*/
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
			/*LEDʵ�ֲ���*/
			GPIO_SetBits(GPIOC,GPIO_Pin_1);
			GPIO_ResetBits(GPIOC,GPIO_Pin_2);
			delay_ms(10);
			GPIO_SetBits(GPIOC,GPIO_Pin_2);
			GPIO_ResetBits(GPIOC,GPIO_Pin_1);
			delay_ms(10);
//		  OLED_Clear(); 
			}
/*				 RF24L01_Set_Mode( MODE_RX );		//����ģʽ
			 j = NRF24L01_RxPacket( RF24L01RxBuffer );		//�����ֽ�
			if( 0 != j )
			{
				OLED_ShowString(0,5,(uint8_t*)"In the receiving",100);
				printf("���յ�������%s\r\n",RF24L01RxBuffer);
				OLED_ShowString(0,num,RF24L01RxBuffer,8);		
     	}	  
	*/
}
void LED_test(void)
{
	 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��PC,D,G�˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2;	 //PD13,PD15�������  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�50MHz
 	GPIO_Init(GPIOC, &GPIO_InitStructure);	  //��ʼ��GPIOD3,6
 	GPIO_SetBits(GPIOC,GPIO_Pin_1|GPIO_Pin_2);	//PD3,PD6 �����
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	 //PD13,PD15�������  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�50MHz
 	GPIO_Init(GPIOA, &GPIO_InitStructure);	  //��ʼ��GPIOD3,6
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;	 //PD13,PD15�������  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�50MHz
 	GPIO_Init(GPIOA, &GPIO_InitStructure);	  //��ʼ��GPIOD3,6
}

