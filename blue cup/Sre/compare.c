#include "compare.h"

//延时变量
uint32_t TimingDelay = 0;

//电压转换变量
float  ucTmpeValue=0 ,TmpeValue=0;
//串口比较命令变量
uint8_t B1[]={"B1"};
uint8_t B2[]={"B2"};
uint8_t B3[]={"B3"};
uint8_t B4[]={"B4"};
uint8_t ADC[]={"ADC"};
uint8_t LD1[]={"LD1"};
uint8_t LD2[]={"LD2"};
uint8_t LD3[]={"LD3"};
uint8_t LD4[]={"LD4"};
uint8_t LD5[]={"LD5"};
uint8_t LD6[]={"LD6"};
uint8_t LD7[]={"LD7"};
uint8_t LD8[]={"LD8"};
uint8_t *ucpy[]={B1,B2,B3,B4,ADC,LD1,LD2,LD3,LD4,LD5,LD6,LD7,LD8};

uint8_t  CONT=0;
uint32_t ledflag=1;
uint8_t string[20];
uint32_t  LEDvalue=0;


static int SCpy (uint8_t* cpy1,uint8_t* cpy2)
{
	uint8_t i=0,cflag=0;
  while((cpy1[i] != '?')&&(cpy1[i] != '\0')&&(cpy1[i] != ':'))
	{
		cflag =1; 
		if(cpy1[i] != cpy2[i])
		{
			return 0; 
		}
		else
		{
		 i++;
		}
	}
	if(cflag == 1)
	{
		cflag = 0;
	  return 1;//满足所有条件
	}
	return 0; 
}

//在串口加标志位来响应这个函数
void Compare_CMD (void)
{
	
	uint8_t i=0,flag=0;
	ledflag = ~ledflag;
	Delay_Ms(7);
//	printf ("已入比较函数\r\n");
	for(i=0;i<((sizeof(ucpy))/4);i++)
	{  //LED的状态显示可以用端口查询函数，查出结果右移8位，强制类型转换为16进制保存到string
	  if(SCpy(RxBuffer,ucpy[CONT])==1)
		{
//			printf ("for中i值为%d\r\n",i);
			flag = 1;
			i=((sizeof(ucpy))/4);
			printf ("执行选择的是：%d\r\n",CONT);
			switch(CONT)
					{
					
   					 case 0: //LCD_DisplayStringLine(Line4," B1:R");
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
					   case 1: //LCD_DisplayStringLine(Line5," B2:R");
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
						 case 2: //LCD_DisplayStringLine(Line6," B3:P");
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
						 case 3: //LCD_DisplayStringLine(Line7," B4:R");
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
						case 4: //LCD_DisplayStringLine(Line2,"ADC");
									 ucTmpeValue=ADC1_Conv();
									 TmpeValue = ucTmpeValue*3.3/4096;
									 sprintf((char*)string," ADC=%.2fV ",TmpeValue);
									 LCD_DisplayStringLine(Line3,string);
									 printf((char*)string);
						       printf("\r\n");
						break;
						case 5: //LCD_DisplayStringLine(Line2,"LD1:n");
									LED_CPY(GPIO_Pin_8);
						break;
						case 6: //LCD_DisplayStringLine(Line2,"LD2:n");
									LED_CPY(GPIO_Pin_9);
						break;
						case 7: //LCD_DisplayStringLine(Line2,"LD3:n");
									LED_CPY(GPIO_Pin_10);
						break;
						case 8: //LCD_DisplayStringLine(Line2,"LD4:n");
									LED_CPY(GPIO_Pin_11);
						break;
						case 9: //LCD_DisplayStringLine(Line2,"LD5:n");
								 LED_CPY(GPIO_Pin_12);
						break;
						case 10: //LCD_DisplayStringLine(Line2,"LD6:n");
								 LED_CPY(GPIO_Pin_13);
						break;
						case 11: //LCD_DisplayStringLine(Line2,"LD7:n");
									LED_CPY(GPIO_Pin_14);
						break;
						case 12: //LCD_DisplayStringLine(Line2,"LD8:n");
									LED_CPY(GPIO_Pin_15);
						break;
						default:
						break;
					}
    }
		else if(SCpy(RxBuffer,ucpy[CONT])==0)
		{
			 CONT++;
		}
 } 
 CONT = 0;
	if(flag == 0)
  {		
   printf("erro\r\n");
	}
}

void KEY_B1 (void)
{
	unsigned short temp = 0;
  while(((GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))==Bit_RESET)&&(RXFLAG !=1))
	{
		temp ++;
		Delay_Ms(1);
		if(temp>800)
		{
		GPIOC->ODR &=~(LEDvalue<<8);
		LED_LOCK;
		Delay_Ms(5);
		}
		else if(temp>1)
		{
				GPIOC->ODR = 0XFFFF;
				LED_LOCK;

		}
	}
}
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

