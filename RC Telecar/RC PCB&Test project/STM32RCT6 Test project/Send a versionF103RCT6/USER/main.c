//////////////////////////////////////////////////////////////////////////////////	 
//
//  文 件 名   : main.c
//  功能描述   : OLED IIC (模拟IIC)接口演示例程
//              说明: 
//              ----------------------------------------------------------------
//              GND   电源地
//              VCC   接5V或3.3v电源
//              SCL   接PA5（SCL）
//              SDA   接PA7（SDA）            
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
		delay_init();	    	     //延时函数初始化	  
		NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级 	LED_Init();			     //LED端口初始化
		OLED_Init();			       //初始化OLED 
	  OLED_Clear();
	  Adc_GPIOInit();
		LED_test();
	 	uart_init(115200);
	 	drv_spi_init( );
		NRF24L01_Gpio_Init( );     //RF24L01引脚初始化
	  printf("Check the connection....\r\n");
		NRF24L01_check( );	 //检测nRF24L01		
	  printf("Check the end of connection\r\n");
		RF24L01_Init( );
		OLED_Clear(); 
	  RF24L01_Set_Mode( MODE_TX );		//发送模式
		while(1) 
		{	
			int i=0;
			/*下面是OLED显示部分*/
			delay_ms(100);
			OLED_ShowString(0,7,(uint8_t*)string,200);/*后面的200是在OLED上显示的大小*/
			sprintf(string," Channel 1: %d ",ADC1_Conv(1));
			OLED_ShowString(0,1,(uint8_t*)string,200);
			sprintf(string," Channel 2: %d ",ADC1_Conv(2));
			OLED_ShowString(0,3,(uint8_t*)string,200);
			sprintf(string," Channel 3: %d ",ADC1_Conv(3)-3000);
			OLED_ShowString(0,5,(uint8_t*)string,200);
			sprintf(string," Channel 4: %d ",ADC1_Conv(6)-3000);
			if ((ADC1_Conv(2)>320)||(ADC1_Conv(2)<200))
			{
		/*无源蜂鸣器实现部分*/
		for(i=0;i<40;i++)
		{
			GPIO_SetBits(GPIOA,GPIO_Pin_8);
			delay_us(ADC1_Conv(2));
			GPIO_ResetBits(GPIOA,GPIO_Pin_8);
			delay_us(ADC1_Conv(2));
		}
	}
//			OLED_DrawBMP(0,0,128,8,BMP1);  //图片显示(图片显示慎用，生成的字表较大，会占用较多空间，FLASH空间8K以下慎用)
	/*按键实现部分*/		
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)==0)
			{
				delay_ms(10);
				if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)==0)
				{
					/*NRF24L01_TxPacket该函数是发送一包数据*/
			    NRF24L01_TxPacket( UART_BUFFER , 7 );		/*模式1发送固定字符,1S一包，后面那个7是发送多少个字节*/
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
			/*LED实现部分*/
			GPIO_SetBits(GPIOC,GPIO_Pin_1);
			GPIO_ResetBits(GPIOC,GPIO_Pin_2);
			delay_ms(10);
			GPIO_SetBits(GPIOC,GPIO_Pin_2);
			GPIO_ResetBits(GPIOC,GPIO_Pin_1);
			delay_ms(10);
//		  OLED_Clear(); 
			}
/*				 RF24L01_Set_Mode( MODE_RX );		//接收模式
			 j = NRF24L01_RxPacket( RF24L01RxBuffer );		//接收字节
			if( 0 != j )
			{
				OLED_ShowString(0,5,(uint8_t*)"In the receiving",100);
				printf("接收到的数据%s\r\n",RF24L01RxBuffer);
				OLED_ShowString(0,num,RF24L01RxBuffer,8);		
     	}	  
	*/
}
void LED_test(void)
{
	 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA, ENABLE);	 //使能PC,D,G端口时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2;	 //PD13,PD15推挽输出  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
 	GPIO_Init(GPIOC, &GPIO_InitStructure);	  //初始化GPIOD3,6
 	GPIO_SetBits(GPIOC,GPIO_Pin_1|GPIO_Pin_2);	//PD3,PD6 输出高
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	 //PD13,PD15推挽输出  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
 	GPIO_Init(GPIOA, &GPIO_InitStructure);	  //初始化GPIOD3,6
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;	 //PD13,PD15推挽输出  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
 	GPIO_Init(GPIOA, &GPIO_InitStructure);	  //初始化GPIOD3,6
}

