#include "compare.h"

//延时变量
uint32_t TimingDelay = 0;

//电压转换变量
float  ucTmpeValue=0 ,TmpeValue=0;
//串口比较命令变量

uint16_t  CONT=0;
uint32_t ledflag=1;
uint8_t string[20];
uint32_t  LEDvalue=0;

#define Compare_base   178
#define Compare_led    121
/*测试字符串相加实体实现**************************************/
uint16_t Compare_Number (uint8_t* test)
{
	uint16_t cont=0, i=0;
	i=0;
	cont=0;
	Delay_Ms(10);
	while((test[i]!='\r')&&(test[i]!='\n')&&(test[i]!='\0'))
	{
	cont+=test[i];
	i++;
	}
	return cont;
	

}
/***************************************************/


//在串口加标志位来响应这个函数
void Compare_CMD (void)
{
	
	    CONT=Compare_Number(RxBuffer);
			if((CONT>=299)&&(CONT<=308))
			{
				CONT=299;
			}
			switch(CONT)
					{
					
   					 case Compare_base+0: //LCD_DisplayStringLine(Line4," B1:R");
									if((GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))==Bit_RESET)
										{
										 LCD_DisplayStringLine(Line4,(uint8_t *)" B1:P");
										printf("B1:P\r\n");
										}
										else
										{
										LCD_DisplayStringLine(Line4,(uint8_t *)" B1:R");
										printf("B1:R\r\n");
										}
						 break;
					   case Compare_base+1: //LCD_DisplayStringLine(Line5," B2:R");
									if((GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8))==Bit_RESET)
										{
										 LCD_DisplayStringLine(Line5,(uint8_t *)" B2:P");
										 printf("B2:P\r\n");
										}
										else
										{
										LCD_DisplayStringLine(Line5,(uint8_t *)" B2:R");
										printf("B2:R\r\n");
										}
						 break;
						 case Compare_base+2: //LCD_DisplayStringLine(Line6," B3:P");
									if((GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1))==Bit_RESET)
										{
										 LCD_DisplayStringLine(Line6,(uint8_t *)" B3:P");
										 printf("B3:P\r\n");
										}
										else
										{
										LCD_DisplayStringLine(Line6,(uint8_t *)" B3:R");
										printf("B3:R\r\n");
										}
						 break;
						 case Compare_base+3: //LCD_DisplayStringLine(Line7," B4:R");
									if((GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_2))==Bit_RESET)
										{
										 LCD_DisplayStringLine(Line7,(uint8_t *)" B4:P");
										 printf("B4:P\r\n");
										}
										else
										{
										LCD_DisplayStringLine(Line7,(uint8_t *)" B4:R");
										printf("B4:R\r\n");
										}
						break;
						case Compare_base+85: //LCD_DisplayStringLine(Line2,"ADC");
									 ucTmpeValue=ADC1_Conv();
									 TmpeValue = ucTmpeValue*3.3/4096;
									 sprintf((char*)string," ADC=%.2fV ",TmpeValue);
									 LCD_DisplayStringLine(Line3,string);
									 printf((char *) string);
						       printf("\r\n");
						break;
						case Compare_base+Compare_led: //LCD_DisplayStringLine(Line2,"LD1:n");
									ledflag=!ledflag;
									if (RxBuffer[2] == '1')
									    LED_CPY(GPIO_Pin_8);
									else if (RxBuffer[2] == '2')
									         LED_CPY(GPIO_Pin_9);
									else if (RxBuffer[2] == '3')
									         LED_CPY(GPIO_Pin_10);
									else if (RxBuffer[2] == '4')
									         LED_CPY(GPIO_Pin_11);
									else if (RxBuffer[2] == '5')
									         LED_CPY(GPIO_Pin_12);
									else if (RxBuffer[2] == '6')
									         LED_CPY(GPIO_Pin_13);
									else if (RxBuffer[2] == '7')
									         LED_CPY(GPIO_Pin_14);
									else if (RxBuffer[2] == '8')
									         LED_CPY(GPIO_Pin_15);

						break;
						default:
							     printf("erro\r\n");
						break;
					}

}

void KEY_B1 (void)
{
	unsigned short temp = 0;
	while(((GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8))==Bit_RESET)&&(RXFLAG !=1))
	{
			LCD_DisplayStringLine(Line5,(uint8_t *)" B2:P");
	    Delay_Ms(15);
	}
  while(((GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1))==Bit_RESET)&&(RXFLAG !=1))
	{
			LCD_DisplayStringLine(Line6,(uint8_t *)" B3:P");
		  Delay_Ms(15);
	}
	while(((GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_2))==Bit_RESET)&&(RXFLAG !=1))
	{
			LCD_DisplayStringLine(Line7,(uint8_t *)" B4:P");
		  Delay_Ms(15);
	}
  while(((GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))==Bit_RESET)&&(RXFLAG !=1))
	{
		temp ++;
		Delay_Ms(1);
		if(temp>800)
		{
		GPIOC->ODR =0XFFFF;
		GPIOC->ODR &=(~LEDvalue)<<8;
			printf("%X\r\n",~(((~LEDvalue)&0xFF)));
		LED_LOCK;
		Delay_Ms(5);
		}
		else if(temp>1)
		{
				GPIOC->ODR = 0XFFFF;
				LED_LOCK;
		LCD_DisplayStringLine(Line4,(uint8_t *)" B1:P");

		}
	}
	
	LCD_DisplayStringLine(Line4,(uint8_t *)" B1:R");
	LCD_DisplayStringLine(Line5,(uint8_t *)" B2:R");
	LCD_DisplayStringLine(Line6,(uint8_t *)" B3:R");
	LCD_DisplayStringLine(Line7,(uint8_t *)" B4:R");
}
/*LED部分的比较数据会出现部分追尾现象需要二级处理*/
void LED_CPY(uint16_t GPIO_Pin)
{
							        if(RxBuffer[4] == '1')
										{
											GPIOC->ODR =0XFFFF;
											GPIO_WriteBit(GPIOC, GPIO_Pin,Bit_RESET );
									
											LED_LOCK;
									  }
						        else if(RxBuffer[4] == '0')
										{
											GPIOC->ODR =0XFFFF;
											GPIO_WriteBit(GPIOC, GPIO_Pin,Bit_SET );
											LED_LOCK;
									  }
										else if(RxBuffer[4] == '2')
										{
									//		printf("已经执行为状态反转");
											if(ledflag==1)
											{
											GPIOC->ODR =0XFFFF;
											GPIO_WriteBit(GPIOC, GPIO_Pin,Bit_SET  );
											LED_LOCK;
										
											}
											else if(ledflag!=1)
											{
											GPIOC->ODR =0XFFFF;
											GPIO_WriteBit(GPIOC, GPIO_Pin,Bit_RESET  );
											LED_LOCK;
										
											}
									  }
}

//延时函数
void Delay_Ms(uint32_t nTime)
{
	TimingDelay = nTime;
	while(TimingDelay != 0);	
}

