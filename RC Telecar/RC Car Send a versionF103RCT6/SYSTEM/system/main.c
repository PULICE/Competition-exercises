/********************************************************************************	 
*
*  �� �� ��   : main.c
*  ��������   :RCң��������
*              ˵��: 
*              ----------------------------------------------------------------
*              GND   ��Դ��
*          
*             -----------------------------------------------------------------
*pb7//����һ������
*pb8//����
*pb9//�������ź�
*
*
*
*��//vcc
*��//GND
*
*���������
*��//tx
*��//rx
********************************************************************************/
#include "delay.h"
#include "sys.h"
#include "adc.h"
#include "oled.h"
#include "bmp.h"
#include "usart.h"
#include "bsp_spi_flash.h"
#include "stm32f10x_tim.h"
/*enum*/
typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;

/*variable*/
char string1[3];
volatile u8 flagstatus=0;

/* macro*/
/*LED*/
#define LED1_ON   GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_RESET);
#define LED1_OFF  GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_SET);

#define LED2_ON   GPIO_WriteBit(GPIOC, GPIO_Pin_2, Bit_RESET);
#define LED2_OFF  GPIO_WriteBit(GPIOC, GPIO_Pin_2, Bit_SET);
/*BEEP*/
#define BEEP_InitON	{\
										Timinit(7);TIM_Cmd(TIM2,ENABLE);delay_ms(100);\
										TIM_Cmd(TIM2,DISABLE);delay_ms(70);\
										Timinit(3);TIM_Cmd(TIM2,ENABLE);delay_ms(100);\
										TIM_Cmd(TIM2,DISABLE);delay_ms(170);\
                    Timinit(2);TIM_Cmd(TIM2,ENABLE);delay_ms(270);\
	                  TIM_Cmd(TIM2,DISABLE);\
}
#define BEEP_DDON	{\
										Timinit(4);TIM_Cmd(TIM2,ENABLE);delay_ms(100);\
										TIM_Cmd(TIM2,DISABLE);delay_ms(40);\
                    Timinit(4);TIM_Cmd(TIM2,ENABLE);delay_ms(270);\
	                  TIM_Cmd(TIM2,DISABLE);\
}
/* ��ȡ�������ĳ��� */
#define TxBufferSize1   (countof(TxBuffer1) - 1)
#define RxBufferSize1   (countof(TxBuffer1) - 1)
#define countof(a)      (sizeof(a) / sizeof(*(a)))
#define  BufferSize (countof(Tx_Buffer)-1)

#define  FLASH_WriteAddress     0x00000
#define  FLASH_ReadAddress      FLASH_WriteAddress
#define  FLASH_SectorToErase    FLASH_WriteAddress

#define Direction    ADC1_Conv(1)/10+(ADC1_Conv(1)%10)
#define Accelerator  ADC1_Conv(6)/10+(ADC1_Conv(6)%10)
	
/* ���ͻ�������ʼ�� */
uint8_t Tx_Buffer[] = "��׼˫����� for English\r\n";
uint8_t Rx_Buffer[BufferSize];

__IO uint32_t DeviceID = 0;
__IO uint32_t FlashID = 0;
__IO TestStatus TransferStatus1 = FAILED;

/* function */
void LED_GPIO_Config(void);
void Timinit(const int tim_pre);
void Delay(__IO uint32_t nCount);
TestStatus Buffercmp(uint8_t* pBuffer1,uint8_t* pBuffer2, uint16_t BufferLength);

 int main(void)
 {
	  int i=0,Avale=0,vale=0;
		delay_init();	    	     //��ʱ������ʼ��	  
		NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ� 	LED_Init();			     //LED�˿ڳ�ʼ��
		OLED_Init();			       //��ʼ��OLED 
	  OLED_Clear();
	  Adc_GPIOInit();
		LED_Init();
	 	uart_init(115200);
		OLED_ShowString(0,0,(uint8_t *)"  Initializing ",80);
	 	/* 8M����flash W25Q64��ʼ�� */
	 SPI_FLASH_Init();
	/* ��ȡ Flash Device ID */
	DeviceID = SPI_FLASH_ReadDeviceID();	
	Delay( 200 );

	/* ��ȡ SPI Flash ID */
	FlashID = SPI_FLASH_ReadID();	
	printf("\r\n FlashID is 0x%X,\
	Manufacturer Device ID is 0x%X\r\n", FlashID, DeviceID);
	
	/* ���� SPI Flash ID */
	if (FlashID ==sFLASH_ID)
	{	
		printf("\r\n ��⵽����flash W25Q16 !\r\n");
		BEEP_DDON;
		
    SPI_FLASH_BulkErase();
		
		/* ������Ҫд��� SPI FLASH ������FLASHд��ǰҪ�Ȳ��� */
		// �������4K����һ����������������С��λ������
		SPI_FLASH_SectorErase(FLASH_SectorToErase);	 	 
		
		/* �����ͻ�����������д��flash�� */
		// ����дһҳ��һҳ�Ĵ�СΪ256���ֽ�
		SPI_FLASH_BufferWrite(Tx_Buffer, FLASH_WriteAddress, BufferSize);		
		printf("\r\n д�������Ϊ��%s \r\t", Tx_Buffer);

		/* ���ո�д������ݶ������ŵ����ջ������� */
		SPI_FLASH_BufferRead(Rx_Buffer, FLASH_ReadAddress, BufferSize);
		printf("\r\n ����������Ϊ��%s \r\n", Rx_Buffer);
		
		/* ���д�������������������Ƿ���� */
		TransferStatus1 = Buffercmp(Tx_Buffer, Rx_Buffer, BufferSize);
		
		if( PASSED == TransferStatus1 )
		{ 
			LED2_ON;
			printf("\r\n 8M����flash(W25Q16)���Գɹ�!\n\r");
			OLED_Clear();
		  OLED_ShowString(6,3,(uint8_t*)"W25Q16 test is success ",100);//���һ���������ַ��Ĵ�С
			BEEP_InitON;
		}
		else
		{        
			LED1_ON;
			printf("\r\n 8M����flash(W25Q16)����ʧ��!\n\r");
			OLED_Clear();
		  OLED_ShowString(6,3,(uint8_t*)"W25Q16 test is failed ",100);//���һ���������ַ��Ĵ�С
		}
	}
	else
	{ 
		LED1_ON;
		printf("\r\n ��ȡ���� W25Q16 ID!\n\r");
		OLED_Clear();
		OLED_ShowString(6,3,(uint8_t*)"no device detected! ",100);//���һ���������ַ��Ĵ�С
	}
	delay_ms(600);
		OLED_Clear();
		OLED_ShowString(15,0,(uint8_t *)"433Mhz_ready",80);
		OLED_ShowString(0,3,(uint8_t *)"High density    devices",80);
		while(1) 
		{	
			
			Avale=Accelerator;
			/*ADC���ݲɼ����Ų���   S18-22 L13-17 H12-10 D27-80*/
			if((Avale<35)&&(Avale>21))//S
			{
					string1[1]='e';
			}
		  if((Avale<58)&&(Avale>35))//L
			{
					string1[1]='f';
			}
			if((Avale<150)&&(Avale>58))//H
			{
					string1[1]='g';
			}
			if((Avale<21)&&(Avale>9))//D
			{
					string1[1]='h';
			}
			vale=Direction;
			/*ADC���ݲɼ����򲿷�   S18-23 DR14-17 HR13-10 DL25-30 HL31-95*/
			if((vale<40)&&(vale>30))//S
			{
					string1[0]='s';
			}
			if((vale<99)&&(vale>57))//DL
			{
					string1[0]='d';
			}
			if((vale<30)&&(vale>1))//HL
			{
					string1[0]='b';
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
//			  GPIO_ResetBits(GPIOC,GPIO_Pin_2);
				GPIO_ResetBits(GPIOC,GPIO_Pin_1);
				printf("%sx",string1);//�ź�
				printf("%cx",string1[2]);//�ź�
//				sprintf((char *)string1,"RL:%d ",vale);
				OLED_ShowString(10,7,(uint8_t *)string1,80);
//				sprintf((char *)string1,"%d ",Avale);
				OLED_ShowString(60,7,(uint8_t *)string1,80);
			}

			/*led��Ӧ����*/
//			GPIO_SetBits(GPIOC,GPIO_Pin_2);
			GPIO_SetBits(GPIOC,GPIO_Pin_1);

			}	
}
 

/*
 * ��������Buffercmp
 * ����  ���Ƚ������������е������Ƿ����
 * ����  ��-pBuffer1     src������ָ��
 *         -pBuffer2     dst������ָ��
 *         -BufferLength ����������
 * ���  ����
 * ����  ��-PASSED pBuffer1 ����   pBuffer2
 *         -FAILED pBuffer1 ��ͬ�� pBuffer2
 */
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while(BufferLength--)
  {
    if(*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }

    pBuffer1++;
    pBuffer2++;
  }
  return PASSED;
}

void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}

void LED_GPIO_Config(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA, ENABLE);
	  /* Configure GPIOC as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_WriteBit(GPIOC, GPIO_Pin_1|GPIO_Pin_2, Bit_SET);
	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_WriteBit(GPIOA, GPIO_Pin_8, Bit_RESET);
}

/*������ʱ����ϰ*/
void Timinit(const int tim_pre){
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_TimeBaseInitStruct.TIM_Period = 7200-1;
  TIM_TimeBaseInitStruct.TIM_Prescaler =tim_pre;
//  TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
 // TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
 // TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0x0000;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM2,DISABLE);/*ע�⣺Ӧ����Ҫ�����ȹص���ʱ��*/
	
}
/*��ʱ��2���жϷ���,ÿ���һ���ж�*/
void TIM2_IRQHandler(void){
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET){
			TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
			flagstatus=!flagstatus;
			switch(flagstatus){
				case 0:GPIO_WriteBit(GPIOA, GPIO_Pin_8, Bit_SET);
				break;
				case 1:GPIO_WriteBit(GPIOA, GPIO_Pin_8, Bit_RESET);
				break;
				default:
				break;
			}
	}
}
/*********************************************END OF FILE**********************/

