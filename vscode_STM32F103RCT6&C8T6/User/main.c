#include "common.h"
#include "sys.h"
#include "oled.h"
#include "bmp.h"
#include "usart.h"

int main (void) {
	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ� 	LED_Init();			     //LED�˿ڳ�ʼ��
	OLED_Init();			       //��ʼ��OLED  
	uart_init(115200);
	OLED_Clear(); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_13;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_WriteBit(GPIOC,GPIO_Pin_2|GPIO_Pin_1,Bit_SET);

	while(1) {
		GPIOA->BRR = GPIO_Pin_8;
		GPIOC->BRR = GPIO_Pin_2|GPIO_Pin_13;
		Delay_ms(50);
		GPIOA->BSRR = GPIO_Pin_8;
		GPIOC->BSRR = GPIO_Pin_2|GPIO_Pin_13;
		Delay_ms(50);	
		OLED_Clear();
		OLED_ShowString(6,3,(uint8_t*)"stm32f103RC",100);//���һ���������ַ��Ĵ�С
		Delay_ms(1000);	
		OLED_Clear();
		OLED_DrawBMP(0,0,128,8,BMP1);  //ͼƬ��ʾ(ͼƬ��ʾ���ã����ɵ��ֱ�ϴ󣬻�ռ�ý϶�ռ䣬FLASH�ռ�8K��������)
		Delay_ms(1000);	
		UART_Sendstring((uint8_t*)"���Ĵ��ڲ���\r\nEnglish serial port test\r\n");
	}
}

