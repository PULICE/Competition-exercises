#include "app.h"
#include "sys.h"
#include "oled.h"
#include "bmp.h"

#define PWM_DELAY 1800
uint8_t rxBuf[64];
__IO int cnt = 0;
__IO  uint8_t idle = 0;//1:串口空闲
extern void Usart2_sendBuf(unsigned char* buf,int len);
/*命令*/
uint8_t data1[]={0x55,0x55,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x03,0xFD,0xD4,0x14,0x01,0x17,0x00};  //命令唤醒
uint8_t data2[]={0x00,0x00,0xFF,0x04,0xFC,0xD4,0x4A,0x01,0x00,0xE1,0x00}; //命令读ID

/*IC卡数据
//uint8_t card1[18] = {0x00,0x00,0xff,0x0c,0xf4,0xd5,0x4b,0x01,0x01,0x00,0x04,0x08,0x04,0xC9,0x02,0x15,0xF3,0xFB};
以上是未经优化的比较数据，下面的数据经过优化之后只从原始数据19个中抽出5个数据进行比较*/
uint8_t card1[] = {0xC9,0x02,0x15,0xF3,0xFB};
uint8_t card2[] = {0xFB,0xE7,0x04,0x10,0xD8};
//uint8_t card2[] = {0xDD,0x14,0x3E,0x19,0x86};
uint8_t card3[] = {0xd7,0x0a,0xa4,0x3f,0x0a};
uint8_t card4[] = {0Xab,0X5d,0Xf6,0X20,0Xb0};
uint8_t card5[] = {0X7d,0Xad,0X55,0X19,0X36};
uint8_t card6[] = {0X87,0X7f,0X47,0X3b,0X46};
uint8_t *pn532List[] = {card1,card2,card3,card4,card5,card6};//卡ID容器列表
//72 000 000  //8 000 000
/*内存拷贝*/
void mcpy(uint8_t* target,uint8_t* src,int len)
{
	int i =0;
	for(i=0;i<len;i++)
	{
		target[i] = src[i];/*这个数据是原始数据*/
	}
}


/*内存清空*/
void mset(uint8_t* target,uint8_t  src,int len)
{
	int i =0;
	for(i=0;i<len;i++)
	{
		target[i] = src;
		
	}
}

/*pn532读取数据*/
void readData(uint8_t* readbuf,int* readLen)
{
//这里需要一个超时机制，不然如果没有数据进来就会卡死在这里
	int timeout = 0xffff;
	while(timeout--)
	{
		if(idle == 1)
		{
			idle = 0;
			mset(readbuf,0,sizeof(rxBuf));//使用内存之前首先清空，防止脏内存
			
			mcpy(readbuf,rxBuf,cnt);//拷贝接收数据
			//printf("\r");
			mset(rxBuf,0,sizeof(rxBuf));//清空接收缓冲

			*readLen = cnt;//输出接收数据长度
			cnt = 0;
			return;
		}
	}
}


/*UID比较函数 1:相等，0不相等*/
int Compaer_ID(uint8_t* cmp1,uint8_t* cmp2,int len)
{
	/*数据校准代码	
	printf("cmp1 17:=%x",cmp1[15]);
	printf("cmp2 4:=%x\r\n",cmp2[4]);
	*/
	delay_ms(1);
	while(len--)
	{

		if(cmp1[len+13] == cmp2[len])//判断数据库；因为只希望比较部分位，所以我们对接收到的原始数据做13位偏移
			continue;
			return 0;
	}
	return 1;
}

void application (void)
{
	int readlen =0,i;
	unsigned char pn532_bfuf[24] = {0};
	OLED_Init();			       //初始化OLED  
	OLED_Clear();

	while(1)
	{

		do{
				IWDG_Feed();//喂狗函数
				Usart2_sendBuf(data1,sizeof(data1));//发送唤醒命令
				readData(pn532_bfuf,&readlen);//等待接收数据
			  Usart2_sendBuf(data2,sizeof(data2));//发送读卡命令
				readData(pn532_bfuf,&readlen);//等待接收IC卡数据	
			  readData(pn532_bfuf,&readlen);//等待接收数据，因为在接收数据哪里存在一个idle,所以如果没有数据，会卡在哪里
			//printf("working:  %d\n\r",readlen);
		}while(readlen<18);//如果唤醒命令发送成功会接收到从机的返回值，虽然可以不要这个while但是为了区分唤醒和读卡，还是有必要这样写
	//因为在数据长度等于19时接收到的才是卡片ID数据，所以while判断里面是19，不是计算出来的，是通过串口调试所以数据可能并不准确不过目前测试没有任何问题
		printf("working%d\n\r",readlen);
//		/*显示UID*/

		for(i=13;i<readlen-1;i++)//显示接收数据
		{
			printf("%x  ",pn532_bfuf[i]);
		}

			
	for(i=0;i<sizeof(pn532List);i++)
	{
			if(Compaer_ID(pn532_bfuf,pn532List[i],4))//从机读取到的UID有24位，但我们只想比较后5位-1，所以实际只比较4位
			{
						switch(i)
						{
							case 0:
							//OLED_Clear();
							//OLED_ShowString(12,0,(uint8_t*)"Jurisdiction   Administrator",100);//输出第几张卡B12
							TIM_Config(500);
							delay_ms(PWM_DELAY);
							TIM_Config(1500);
							delay_ms(PWM_DELAY);

							break;
							case 1:
							OLED_Clear();
							OLED_ShowString(12,0,(uint8_t*)"Jurisdiction  Temporary member",100);//输出第几张卡
							TIM_Config(500);
							delay_ms(PWM_DELAY);
							TIM_Config(1500);
							delay_ms(PWM_DELAY);
							break;
//							case 2:
//							OLED_Clear();
//							OLED_ShowString(12,0,(uint8_t*)"Jurisdiction   Rao pnog",100);//输出第几张卡
//							TIM_Config(500);
//							delay_ms(1000);
//							TIM_Config(1500);
//							delay_ms(1000);
//							break;
							case 2:
							OLED_Clear();
							OLED_ShowString(12,0,(uint8_t*)"Jurisdiction   wang Jian",100);//输出第几张卡
							TIM_Config(500);
							delay_ms(PWM_DELAY);
							TIM_Config(1500);
							delay_ms(PWM_DELAY);
							break;	
							case 3:
							OLED_Clear();
							OLED_ShowString(12,0,(uint8_t*)"Jurisdiction:   Zhang Li",100);//输出第几张卡
							TIM_Config(500);
							delay_ms(PWM_DELAY);
							TIM_Config(1500);
							delay_ms(PWM_DELAY);
							break;	
							case 4:
							OLED_Clear();
							OLED_ShowString(12,0,(uint8_t*)"Jurisdiction:   wang jing wen",100);//输出第几张卡
							TIM_Config(500);
							delay_ms(PWM_DELAY);
							TIM_Config(1500);
							delay_ms(PWM_DELAY);
							break;
							case 5:
							OLED_Clear();
							OLED_ShowString(12,0,(uint8_t*)"Jurisdiction:   dou fu bing",100);//输出第几张卡
							TIM_Config(500);
							delay_ms(PWM_DELAY);
							TIM_Config(1500);
							delay_ms(PWM_DELAY);
							break;
						}			
			}
	 }
 }
}

/*从机使用串口2作为数据传输波特率115200*/
void USART2_IRQHandler()
{
	if(USART_GetITStatus(USART2,USART_IT_RXNE)!= RESET)//接收中断
	{
		USART_ClearITPendingBit(USART2,USART_IT_RXNE); //清除中断标志
		rxBuf[cnt++] = USART2->DR;		
	}
	if(USART_GetITStatus(USART2,USART_IT_IDLE)!= RESET)//空闲中断
	{
		USART2->DR;
		USART2->SR;	
		idle = 1;	
	}
}















