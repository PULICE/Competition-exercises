//////////////////////////////////////////////////////////////////////////////////	 
//
//  �� �� ��   : main.c
//  ��������   : 2.4G RB���մ���
//              ˵��: 
//              ----------------------------------------------------------------
//              GND   ��Դ��
//              VCC   ��5V��3.3v��Դ
//              SCL   ��PB13��SCL��
//              SDA   ��PB15��SDA��            
//              ----------------------------------------------------------------
//******************************************************************************/

#include "delay.h"
#include "sys.h"
#include "oled.h"
#include "bmp.h"
#include "usart.h"
#include "spi.h"
#include "RF24L01.h"
#include "lcd.h"
void LED_test(void);
char *content = "y";
/*function*/
#define LOCK {\
              GPIOD->ODR=(1<<2);\
							GPIOD->ODR=~(1<<2);\
}
uint8_t  UART_BUFFER[64]={0},RF24L01RxBuffer[64]={0};
 int main(void)
 {
		delay_init();	    	     //��ʱ������ʼ��	  
		NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ� 	LED_Init();			     //LED�˿ڳ�ʼ��
		OLED_Init();			       //��ʼ��OLED 
		LED_test();
	 	uart_init(115200);
	 	STM3210B_LCD_Init();
	  LCD_Clear(Green);
	  LCD_SetBackColor(Green);
	  LCD_SetTextColor(White);
	 	drv_spi_init( );
		NRF24L01_Gpio_Init( );     //RF24L01���ų�ʼ��
	  printf("Check the connection....\r\n");
		NRF24L01_check( );	 //���nRF24L01		
	  printf("Check the end of connection\r\n");
		RF24L01_Init( );
		//OLED_Clear();
	  LCD_Clear(Green);
	  LCD_DisplayStringLine(Line4 ,(uint8_t*)"    test RF2L01   ");
		GPIOC->ODR=0XFFFF;
	  LOCK
	while(1) 
	{	
		int i=0;
		uint8_t num,j;
			 GPIO_SetBits(GPIOB,GPIO_Pin_4);
			 RF24L01_Set_Mode( MODE_RX );		//����ģʽ
			 j = NRF24L01_RxPacket( RF24L01RxBuffer );		//�����ֽ�
			if( 0 != j )
			{

				printf("Received content : %s\r\n",RF24L01RxBuffer);
				LCD_ClearLine(Line6);
				LCD_DisplayStringLine(Line6 ,(uint8_t*)  RF24L01RxBuffer );
				num++;
				if(num==8)  num=0;
				GPIO_SetBits(GPIOC,GPIO_Pin_8);
				GPIO_ResetBits(GPIOC,GPIO_Pin_12);
				LOCK
					for(i=0;i<40;i++)
				{
					GPIO_SetBits(GPIOB,GPIO_Pin_4);
					delay_us(400);
					GPIO_ResetBits(GPIOB,GPIO_Pin_4);
					delay_us(400);
				}
					GPIO_SetBits(GPIOC,GPIO_Pin_12);
					GPIO_ResetBits(GPIOC,GPIO_Pin_8);
					GPIOC->ODR=0XFFFF;
					LOCK
			}	

	}	  
	
}

void LED_test(void)
{
	 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);	 //ʹ��PC,D,G�˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;	 //PD13,PD15�������  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�50MHz
 	GPIO_Init(GPIOC, &GPIO_InitStructure);	  //��ʼ��GPIOD3,6
 	GPIO_SetBits(GPIOC,GPIO_Pin_All);	//PD3,PD6 �����
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_NoJTRST,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	 //PD13,PD15�������  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�50MHz
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	  //��ʼ��GPIOD3,6
	//������������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	 //PD13,PD15�������  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�50MHz
 	GPIO_Init(GPIOD, &GPIO_InitStructure);	  //��ʼ��GPIOD3,6
	GPIO_SetBits(GPIOD,GPIO_Pin_2);
}

