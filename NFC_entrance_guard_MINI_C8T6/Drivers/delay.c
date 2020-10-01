#include "stm32f10x.h"  



#define   SystemFrequency 72000000 
volatile unsigned long time_delay; 
void delay_ms(volatile unsigned long nms)
{
    //SYSTICK��Ƶ--1ms��ϵͳʱ���ж�
    if (SysTick_Config(SystemFrequency/1000))
    {
   
        while (1);
    }
    time_delay=nms;//��ȡ��ʱʱ��
    while(time_delay);
    SysTick->CTRL=0x00; //�رռ�����
    SysTick->VAL =0X00; //��ռ�����
}
//��ʱnus
void delay_us(volatile unsigned long nus)
{
 //SYSTICK��Ƶ--1us��ϵͳʱ���ж�
    if (SysTick_Config(SystemFrequency/1000000))
    {
   
        while (1);
    }
    time_delay=nus;//��ȡ��ʱʱ��
    while(time_delay);
    SysTick->CTRL=0x00; //�رռ�����
    SysTick->VAL =0X00; //��ռ�����
}
    //���ж��н�time_delay�ݼ���ʵ����ʱ
void SysTick_Handler(void)
{
    if(time_delay)
        time_delay--;
}
