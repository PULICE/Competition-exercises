#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "oled.h"
#include "bmp.h"
#include "includes.h"
/************************************************
 ALIENTEK战舰STM32开发板UCOS实验
 UCOSII移植
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/

//START 任务
//设置任务优先级
#define START_TASK_PRIO			10  ///开始任务的优先级为最低
//设置任务堆栈大小
#define START_STK_SIZE			128
//任务任务堆栈
OS_STK START_TASK_STK[START_STK_SIZE];
//任务函数
void start_task(void *pdata);

//LED0任务
//设置任务优先级
#define LED0_TASK_PRIO			4
//设置任务堆栈大小
#define LED0_STK_SIZE			128
//任务堆栈
OS_STK LED0_TASK_STK[LED0_STK_SIZE];
//任务函数
void led0_task(void *pdata);

//LED1任务
//设置任务优先级
#define LED1_TASK_PRIO			6
//设置任务堆栈大小
#define LED1_STK_SIZE			128
//任务堆栈
OS_STK LED1_TASK_STK[LED1_STK_SIZE];
//任务函数
void led1_task(void *pdata);

//浮点测试任务
#define FLOAT_TASK_PRIO			5
//设置任务堆栈大小
#define FLOAT_STK_SIZE			128
//任务堆栈
//如果任务中使用printf来打印浮点数据的话一点要8字节对齐
__align(8) OS_STK FLOAT_TASK_STK[FLOAT_STK_SIZE]; 
//任务函数
void float_task(void *pdata);

//OLED任务
//设置任务优先级
#define OLED_TASK_PRIO			7
//设置任务堆栈大小
#define OLED_STK_SIZE			128
//任务堆栈
OS_STK OLED_TASK_STK[OLED_STK_SIZE];
//任务函数
void oled_task(void *pdata);

int main(void)
{
	delay_init();       	//延时初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //中断分组配置
	uart_init(115200);    	//串口波特率设置
	LED_Init();  						//LED初始化
	OLED_Init();			       //初始化OLED  
	OLED_Clear(); 						//屏幕清理

	
	OSInit();  				//UCOS初始化
	OSTaskCreate(start_task,(void*)0,(OS_STK*)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO); //创建开始任务
	OSStart(); 				//开始任务
}

//开始任务
void start_task(void *pdata)
{
	OS_CPU_SR cpu_sr=0;
	pdata=pdata;
	OSStatInit();  //开启统计任务
	
	OS_ENTER_CRITICAL();  //进入临界区(关闭中断)
	OSTaskCreate(led0_task,(void*)0,(OS_STK*)&LED0_TASK_STK[LED0_STK_SIZE-1],LED0_TASK_PRIO);//创建LED0任务
	OSTaskCreate(led1_task,(void*)0,(OS_STK*)&LED1_TASK_STK[LED1_STK_SIZE-1],LED1_TASK_PRIO);//创建LED1任务
	OSTaskCreate(float_task,(void*)0,(OS_STK*)&FLOAT_TASK_STK[FLOAT_STK_SIZE-1],FLOAT_TASK_PRIO);//创建浮点测试任务
	OSTaskCreate(oled_task,(void*)0,(OS_STK*)&OLED_TASK_STK[OLED_STK_SIZE-1],OLED_TASK_PRIO);//创建LED0任务
	OSTaskSuspend(START_TASK_PRIO);//挂起开始任务
	OS_EXIT_CRITICAL();  //退出临界区(开中断)
}
 

//LED0任务
void led0_task(void *pdata)
{
	while(1)
	{
		LED0=0; 
		delay_ms(90);
		LED0=1;
		delay_ms(100);
	}
}

//LED1任务
void led1_task(void *pdata)
{
	while(1)
	{
		LED1=0;
		delay_ms(80);
		LED1=1;
		delay_ms(400);
	}
}

//浮点测试任务
void float_task(void *pdata)
{
	OS_CPU_SR cpu_sr=0;
	static float float_num=0.01;
	while(1)
	{
		float_num+=0.01f;
		OS_ENTER_CRITICAL();	//进入临界区(关闭中断)
		printf("float_num的值为: %.4f\r\n",float_num); //串口打印结果
		OS_EXIT_CRITICAL();		//退出临界区(开中断)
		delay_ms(500);
	}
}
//OLED显示任务
void oled_task(void *pdata)
{
	while(1)
	{
		OLED_Clear();
		OLED_ShowString(6,3,(uint8_t*)" STM32f103RC",100);//最后一个参数是字符的大小
		delay_ms(1000);	
		OLED_Clear();
		OLED_DrawBMP(0,0,128,8,BMP1);  //图片显示(图片显示慎用，生成的字表较大，会占用较多空间，FLASH空间8K以下慎用)
		delay_ms(1000);	
	}
}
