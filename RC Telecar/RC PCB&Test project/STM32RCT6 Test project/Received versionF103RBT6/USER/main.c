//////////////////////////////////////////////////////////////////////////////////	 
//
//  文 件 名   : main.c
//  功能描述   : 2.4G RB接收代码
//              说明: 
//              ----------------------------------------------------------------
//              GND   电源地
//              VCC   接5V或3.3v电源
//              SCL   接PB13（SCL）
//              SDA   接PB15（SDA）            
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
		delay_init();	    	     //延时函数初始化	  
		NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级 	LED_Init();			     //LED端口初始化
		OLED_Init();			       //初始化OLED 
		LED_test();
	 	uart_init(115200);
	 	STM3210B_LCD_Init();
	  LCD_Clear(Green);
	  LCD_SetBackColor(Green);
	  LCD_SetTextColor(White);
	 	drv_spi_init( );
		NRF24L01_Gpio_Init( );     //RF24L01引脚初始化
	  printf("Check the connection....\r\n");
		NRF24L01_check( );	 //检测nRF24L01		
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
			 RF24L01_Set_Mode( MODE_RX );		//接收模式
			 j = NRF24L01_RxPacket( RF24L01RxBuffer );		//接收字节
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
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);	 //使能PC,D,G端口时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;	 //PD13,PD15推挽输出  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
 	GPIO_Init(GPIOC, &GPIO_InitStructure);	  //初始化GPIOD3,6
 	GPIO_SetBits(GPIOC,GPIO_Pin_All);	//PD3,PD6 输出高
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_NoJTRST,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	 //PD13,PD15推挽输出  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	  //初始化GPIOD3,6
	//锁存引脚配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	 //PD13,PD15推挽输出  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
 	GPIO_Init(GPIOD, &GPIO_InitStructure);	  //初始化GPIOD3,6
	GPIO_SetBits(GPIOD,GPIO_Pin_2);
}

