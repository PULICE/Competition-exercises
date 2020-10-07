#include "app.h"
#include "sys.h"
#include "oled.h"
#include "bmp.h"

#define PWM_DELAY 1800
uint8_t rxBuf[64];
__IO int cnt = 0;
__IO  uint8_t idle = 0;//1:���ڿ���
extern void Usart2_sendBuf(unsigned char* buf,int len);
/*����*/
uint8_t data1[]={0x55,0x55,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x03,0xFD,0xD4,0x14,0x01,0x17,0x00};  //�����
uint8_t data2[]={0x00,0x00,0xFF,0x04,0xFC,0xD4,0x4A,0x01,0x00,0xE1,0x00}; //�����ID

/*IC������
//uint8_t card1[18] = {0x00,0x00,0xff,0x0c,0xf4,0xd5,0x4b,0x01,0x01,0x00,0x04,0x08,0x04,0xC9,0x02,0x15,0xF3,0xFB};
������δ���Ż��ıȽ����ݣ���������ݾ����Ż�֮��ֻ��ԭʼ����19���г��5�����ݽ��бȽ�*/
uint8_t card1[] = {0xC9,0x02,0x15,0xF3,0xFB};
uint8_t card2[] = {0xFB,0xE7,0x04,0x10,0xD8};
//uint8_t card2[] = {0xDD,0x14,0x3E,0x19,0x86};
uint8_t card3[] = {0xd7,0x0a,0xa4,0x3f,0x0a};
uint8_t card4[] = {0Xab,0X5d,0Xf6,0X20,0Xb0};
uint8_t card5[] = {0X7d,0Xad,0X55,0X19,0X36};
uint8_t card6[] = {0X87,0X7f,0X47,0X3b,0X46};
uint8_t *pn532List[] = {card1,card2,card3,card4,card5,card6};//��ID�����б�
//72 000 000  //8 000 000
/*�ڴ濽��*/
void mcpy(uint8_t* target,uint8_t* src,int len)
{
	int i =0;
	for(i=0;i<len;i++)
	{
		target[i] = src[i];/*���������ԭʼ����*/
	}
}


/*�ڴ����*/
void mset(uint8_t* target,uint8_t  src,int len)
{
	int i =0;
	for(i=0;i<len;i++)
	{
		target[i] = src;
		
	}
}

/*pn532��ȡ����*/
void readData(uint8_t* readbuf,int* readLen)
{
//������Ҫһ����ʱ���ƣ���Ȼ���û�����ݽ����ͻῨ��������
	int timeout = 0xffff;
	while(timeout--)
	{
		if(idle == 1)
		{
			idle = 0;
			mset(readbuf,0,sizeof(rxBuf));//ʹ���ڴ�֮ǰ������գ���ֹ���ڴ�
			
			mcpy(readbuf,rxBuf,cnt);//������������
			//printf("\r");
			mset(rxBuf,0,sizeof(rxBuf));//��ս��ջ���

			*readLen = cnt;//����������ݳ���
			cnt = 0;
			return;
		}
	}
}


/*UID�ȽϺ��� 1:��ȣ�0�����*/
int Compaer_ID(uint8_t* cmp1,uint8_t* cmp2,int len)
{
	/*����У׼����	
	printf("cmp1 17:=%x",cmp1[15]);
	printf("cmp2 4:=%x\r\n",cmp2[4]);
	*/
	delay_ms(1);
	while(len--)
	{

		if(cmp1[len+13] == cmp2[len])//�ж����ݿ⣻��Ϊֻϣ���Ƚϲ���λ���������ǶԽ��յ���ԭʼ������13λƫ��
			continue;
			return 0;
	}
	return 1;
}

void application (void)
{
	int readlen =0,i;
	unsigned char pn532_bfuf[24] = {0};
	OLED_Init();			       //��ʼ��OLED  
	OLED_Clear();

	while(1)
	{

		do{
				IWDG_Feed();//ι������
				Usart2_sendBuf(data1,sizeof(data1));//���ͻ�������
				readData(pn532_bfuf,&readlen);//�ȴ���������
			  Usart2_sendBuf(data2,sizeof(data2));//���Ͷ�������
				readData(pn532_bfuf,&readlen);//�ȴ�����IC������	
			  readData(pn532_bfuf,&readlen);//�ȴ��������ݣ���Ϊ�ڽ��������������һ��idle,�������û�����ݣ��Ῠ������
			//printf("working:  %d\n\r",readlen);
		}while(readlen<18);//�����������ͳɹ�����յ��ӻ��ķ���ֵ����Ȼ���Բ�Ҫ���while����Ϊ�����ֻ��ѺͶ����������б�Ҫ����д
	//��Ϊ�����ݳ��ȵ���19ʱ���յ��Ĳ��ǿ�ƬID���ݣ�����while�ж�������19�����Ǽ�������ģ���ͨ�����ڵ����������ݿ��ܲ���׼ȷ����Ŀǰ����û���κ�����
		printf("working%d\n\r",readlen);
//		/*��ʾUID*/

		for(i=13;i<readlen-1;i++)//��ʾ��������
		{
			printf("%x  ",pn532_bfuf[i]);
		}

			
	for(i=0;i<sizeof(pn532List);i++)
	{
			if(Compaer_ID(pn532_bfuf,pn532List[i],4))//�ӻ���ȡ����UID��24λ��������ֻ��ȽϺ�5λ-1������ʵ��ֻ�Ƚ�4λ
			{
						switch(i)
						{
							case 0:
							//OLED_Clear();
							//OLED_ShowString(12,0,(uint8_t*)"Jurisdiction   Administrator",100);//����ڼ��ſ�B12
							TIM_Config(500);
							delay_ms(PWM_DELAY);
							TIM_Config(1500);
							delay_ms(PWM_DELAY);

							break;
							case 1:
							OLED_Clear();
							OLED_ShowString(12,0,(uint8_t*)"Jurisdiction  Temporary member",100);//����ڼ��ſ�
							TIM_Config(500);
							delay_ms(PWM_DELAY);
							TIM_Config(1500);
							delay_ms(PWM_DELAY);
							break;
//							case 2:
//							OLED_Clear();
//							OLED_ShowString(12,0,(uint8_t*)"Jurisdiction   Rao pnog",100);//����ڼ��ſ�
//							TIM_Config(500);
//							delay_ms(1000);
//							TIM_Config(1500);
//							delay_ms(1000);
//							break;
							case 2:
							OLED_Clear();
							OLED_ShowString(12,0,(uint8_t*)"Jurisdiction   wang Jian",100);//����ڼ��ſ�
							TIM_Config(500);
							delay_ms(PWM_DELAY);
							TIM_Config(1500);
							delay_ms(PWM_DELAY);
							break;	
							case 3:
							OLED_Clear();
							OLED_ShowString(12,0,(uint8_t*)"Jurisdiction:   Zhang Li",100);//����ڼ��ſ�
							TIM_Config(500);
							delay_ms(PWM_DELAY);
							TIM_Config(1500);
							delay_ms(PWM_DELAY);
							break;	
							case 4:
							OLED_Clear();
							OLED_ShowString(12,0,(uint8_t*)"Jurisdiction:   wang jing wen",100);//����ڼ��ſ�
							TIM_Config(500);
							delay_ms(PWM_DELAY);
							TIM_Config(1500);
							delay_ms(PWM_DELAY);
							break;
							case 5:
							OLED_Clear();
							OLED_ShowString(12,0,(uint8_t*)"Jurisdiction:   dou fu bing",100);//����ڼ��ſ�
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

/*�ӻ�ʹ�ô���2��Ϊ���ݴ��䲨����115200*/
void USART2_IRQHandler()
{
	if(USART_GetITStatus(USART2,USART_IT_RXNE)!= RESET)//�����ж�
	{
		USART_ClearITPendingBit(USART2,USART_IT_RXNE); //����жϱ�־
		rxBuf[cnt++] = USART2->DR;		
	}
	if(USART_GetITStatus(USART2,USART_IT_IDLE)!= RESET)//�����ж�
	{
		USART2->DR;
		USART2->SR;	
		idle = 1;	
	}
}















