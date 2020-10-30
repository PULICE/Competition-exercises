/********************************************************************************	 
*
*  文 件 名   : main.c
*  功能描述   :RC遥控器控制
*              说明: 
*              ----------------------------------------------------------------
*              GND   电源地
*          
*             -----------------------------------------------------------------
*pb7//宁外一根白线
*pb8//黑线
*pb9//好像是信号
*
*
*
*红//vcc
*灰//GND
*
*相对于主控
*绿//tx
*蓝//rx
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
/* 获取缓冲区的长度 */
#define TxBufferSize1   (countof(TxBuffer1) - 1)
#define RxBufferSize1   (countof(TxBuffer1) - 1)
#define countof(a)      (sizeof(a) / sizeof(*(a)))
#define  BufferSize (countof(Tx_Buffer)-1)

#define  FLASH_WriteAddress     0x00000
#define  FLASH_ReadAddress      FLASH_WriteAddress
#define  FLASH_SectorToErase    FLASH_WriteAddress

#define Direction    ADC1_Conv(1)/10+(ADC1_Conv(1)%10)
#define Accelerator  ADC1_Conv(6)/10+(ADC1_Conv(6)%10)
	
/* 发送缓冲区初始化 */
uint8_t Tx_Buffer[] = "标准双语测试 for English\r\n";
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
		delay_init();	    	     //延时函数初始化	  
		NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级 	LED_Init();			     //LED端口初始化
		OLED_Init();			       //初始化OLED 
	  OLED_Clear();
	  Adc_GPIOInit();
		LED_Init();
	 	uart_init(115200);
		OLED_ShowString(0,0,(uint8_t *)"  Initializing ",80);
	 	/* 8M串行flash W25Q64初始化 */
	 SPI_FLASH_Init();
	/* 获取 Flash Device ID */
	DeviceID = SPI_FLASH_ReadDeviceID();	
	Delay( 200 );

	/* 获取 SPI Flash ID */
	FlashID = SPI_FLASH_ReadID();	
	printf("\r\n FlashID is 0x%X,\
	Manufacturer Device ID is 0x%X\r\n", FlashID, DeviceID);
	
	/* 检验 SPI Flash ID */
	if (FlashID ==sFLASH_ID)
	{	
		printf("\r\n 检测到串行flash W25Q16 !\r\n");
		BEEP_DDON;
		
    SPI_FLASH_BulkErase();
		
		/* 擦除将要写入的 SPI FLASH 扇区，FLASH写入前要先擦除 */
		// 这里擦除4K，即一个扇区，擦除的最小单位是扇区
		SPI_FLASH_SectorErase(FLASH_SectorToErase);	 	 
		
		/* 将发送缓冲区的数据写到flash中 */
		// 这里写一页，一页的大小为256个字节
		SPI_FLASH_BufferWrite(Tx_Buffer, FLASH_WriteAddress, BufferSize);		
		printf("\r\n 写入的数据为：%s \r\t", Tx_Buffer);

		/* 将刚刚写入的数据读出来放到接收缓冲区中 */
		SPI_FLASH_BufferRead(Rx_Buffer, FLASH_ReadAddress, BufferSize);
		printf("\r\n 读出的数据为：%s \r\n", Rx_Buffer);
		
		/* 检查写入的数据与读出的数据是否相等 */
		TransferStatus1 = Buffercmp(Tx_Buffer, Rx_Buffer, BufferSize);
		
		if( PASSED == TransferStatus1 )
		{ 
			LED2_ON;
			printf("\r\n 8M串行flash(W25Q16)测试成功!\n\r");
			OLED_Clear();
		  OLED_ShowString(6,3,(uint8_t*)"W25Q16 test is success ",100);//最后一个参数是字符的大小
			BEEP_InitON;
		}
		else
		{        
			LED1_ON;
			printf("\r\n 8M串行flash(W25Q16)测试失败!\n\r");
			OLED_Clear();
		  OLED_ShowString(6,3,(uint8_t*)"W25Q16 test is failed ",100);//最后一个参数是字符的大小
		}
	}
	else
	{ 
		LED1_ON;
		printf("\r\n 获取不到 W25Q16 ID!\n\r");
		OLED_Clear();
		OLED_ShowString(6,3,(uint8_t*)"no device detected! ",100);//最后一个参数是字符的大小
	}
	delay_ms(600);
		OLED_Clear();
		OLED_ShowString(15,0,(uint8_t *)"433Mhz_ready",80);
		OLED_ShowString(0,3,(uint8_t *)"High density    devices",80);
		while(1) 
		{	
			
			Avale=Accelerator;
			/*ADC数据采集油门部分   S18-22 L13-17 H12-10 D27-80*/
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
			/*ADC数据采集方向部分   S18-23 DR14-17 HR13-10 DL25-30 HL31-95*/
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
					
			/*按键响应部分*/
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
			
			
			/*433Mhz数据发送部分*/               /*只发最后一次接收到的数据，我的想法是写一个判断函数判断哪个通道值改变就发哪个，
			再说接收机那边如果不传值就是0，*//*s停L低D中H高*/
			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)==1)
			{
//			  GPIO_ResetBits(GPIOC,GPIO_Pin_2);
				GPIO_ResetBits(GPIOC,GPIO_Pin_1);
				printf("%sx",string1);//信号
				printf("%cx",string1[2]);//信号
//				sprintf((char *)string1,"RL:%d ",vale);
				OLED_ShowString(10,7,(uint8_t *)string1,80);
//				sprintf((char *)string1,"%d ",Avale);
				OLED_ShowString(60,7,(uint8_t *)string1,80);
			}

			/*led响应部分*/
//			GPIO_SetBits(GPIOC,GPIO_Pin_2);
			GPIO_SetBits(GPIOC,GPIO_Pin_1);

			}	
}
 

/*
 * 函数名：Buffercmp
 * 描述  ：比较两个缓冲区中的数据是否相等
 * 输入  ：-pBuffer1     src缓冲区指针
 *         -pBuffer2     dst缓冲区指针
 *         -BufferLength 缓冲区长度
 * 输出  ：无
 * 返回  ：-PASSED pBuffer1 等于   pBuffer2
 *         -FAILED pBuffer1 不同于 pBuffer2
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

/*基本定时器练习*/
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
	TIM_Cmd(TIM2,DISABLE);/*注意：应用需要这里先关掉定时器*/
	
}
/*定时器2的中断服务,每秒进一次中断*/
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

