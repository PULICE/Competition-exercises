#include "compare.h"

//��ʱ����
uint32_t TimingDelay = 0;

//��ѹת������
float  ucTmpeValue=0 ,TmpeValue=0;
//���ڱȽ��������

uint16_t  CONT=0;
uint32_t ledflag=1;
uint8_t string[20];
uint8_t LED_Status=0xff;
uint32_t  LEDvalue;

#define Compare_base   178
#define Compare_led    121
/*�����ַ������ʵ��ʵ��
 *���ڽ�����ֳ��ֵ��Ⱥ�˳��һ������ӵĽ��ȴһ��������������
 *����ͬλ�õ��ַ���λȨ��
*************************************/
uint16_t Compare_Number (uint8_t* test)
{
	uint16_t cont=0, i=0;
	i=0;
	cont=0;
	Delay_Ms(10);
	while((test[i]!='\0')&&(test[i]!='\r')&&(test[i]!='\n'))
	{
	cont+=test[i];
	
//	printf(" %c\r\n",test[i]);
	i++;
	}
//	printf("�ȽϽ���ʱi=%d\r\n",i);
	return cont;
	

}
/***************************************************/


//�ڴ��ڼӱ�־λ����Ӧ�������
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
								 LED_CPY(RxBuffer[2]-'0'-1);
								// printf("%d\r\n",(RxBuffer[2]-'0'-1));

						break;
						default:
							     printf("erro");
						break;
					}

}
//K1�������̰�ʵ�ֺ���
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
		Delay_Ms(1);
		temp ++;
		LCD_DisplayStringLine(Line4,(uint8_t *)" B1:P");
			if(temp>800)
			{
				GPIOC->ODR =0XFFFF;
				GPIOC->ODR &=(~LEDvalue)<<8;//�ڶ��ν���ȡ��
				//printf("%X\r\n",~(((~LEDvalue)&0xFF)));
				LED_LOCK;
				LEDvalue=((GPIO_ReadInputData(GPIOC)>>8) & 0XFF);
				sprintf((char*)string," LED:%02X",(~LEDvalue)&0xFF);
				LCD_DisplayStringLine(Line8,string);
        Delay_Ms(500);
		  }
  }
 if((temp>1)&&(temp<800))
	{
			GPIOC->ODR = 0XFFFF;
			LED_LOCK;
			LCD_DisplayStringLine(Line8,(uint8_t *)" LED:00");
	}
	
	LCD_DisplayStringLine(Line4,(uint8_t *)" B1:R");
	LCD_DisplayStringLine(Line5,(uint8_t *)" B2:R");
	LCD_DisplayStringLine(Line6,(uint8_t *)" B3:R");
	LCD_DisplayStringLine(Line7,(uint8_t *)" B4:R");
}
/*LED���ֵıȽ����ݻ���ֲ���׷β������Ҫ��������*/
void LED_CPY(uint16_t GPIO_Pin)
{
	//������Ҫ����һ��8λ�ı�������������LED��״ֵ̬������������ȫ�ֵģ������ı仯��LED_Status
			if(RxBuffer[4] == '1')
		{
			LED_Status &=~(1<<GPIO_Pin);
		}
		else if(RxBuffer[4] == '0')
		{
			LED_Status |= (1<<GPIO_Pin);

		}
		else if(RxBuffer[4] == '2')
		{
	//		printf("�Ѿ�ִ��Ϊ״̬��ת");
			if(ledflag==1)
			{
         LED_Status &=~(1<<GPIO_Pin);
			}
			else if(ledflag!=1)
			{
         LED_Status |=(1<<GPIO_Pin);
		
			}
		}
	GPIO_Write(GPIOC,LED_Status<<8);
	LED_LOCK;
}

//��ʱ����
void Delay_Ms(uint32_t nTime)
{
	TimingDelay = nTime;
	while(TimingDelay != 0);	
}

