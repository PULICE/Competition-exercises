#include <stdio.h>

#include "pn532.h"


//extern void Usart2_sendBuf(unsigned char* buf,int len);
//extern unsigned char *rxBuf;
//extern int cnt;
//extern unsigned char idle;
//unsigned char pn532_bfuf[64] = {0};

//unsigned char data1[]={0x55,0x55,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x03,0xFD,0xD4,0x14,0x01,0x17,0x00};  
//unsigned char data2[]={0x00,0x00,0xFF,0x04,0xFC,0xD4,0x4A,0x01,0x00,0xE1,0x00}; 

//void mcpy(unsigned char* target,unsigned char* src,int len)
//{
//	int i =0;
//	for(i=0;i<len;i++)
//	{
//		target[i] = src[i];
//	}
//}

//void mset(unsigned char* target,unsigned char src,int len)
//{
//	int i =0;
//	for(i=0;i<len;i++)
//	{
//		target[i] = src;
//	}
//}

//PN532_STA pn532Read()
//{
//	int i=0;
//	//Usart2_sendBuf(data1,sizeof(data1));
//	if(idle == 1)//等待空闲中断
//	{
//		idle = 0;
//		//printf("cnt = %d\r\n",cnt);
//		/*mcpy(pn532_bfuf,rxBuf,64);//拷贝接收数据
//		printf("cnt = %d\r\n",cnt);
//		mset(rxBuf,0,64);//清空接收缓冲*/
//		//数据都拷贝到这里pn532_buf
//		for(i=0;i<64;i++)
//		{
//			printf("%x",rxBuf[i]);
//		}
//		//mset(pn532_bfuf,0,64);//清空接收缓冲
//		
//		
//	}
//	
//	return PN532_OK;
//}
