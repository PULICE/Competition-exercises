#include "stm32f10x.h"  



#define   SystemFrequency 72000000 
volatile unsigned long time_delay; 
void delay_ms(volatile unsigned long nms)
{
    //SYSTICK分频--1ms的系统时钟中断
    if (SysTick_Config(SystemFrequency/1000))
    {
   
        while (1);
    }
    time_delay=nms;//读取定时时间
    while(time_delay);
    SysTick->CTRL=0x00; //关闭计数器
    SysTick->VAL =0X00; //清空计数器
}
//延时nus
void delay_us(volatile unsigned long nus)
{
 //SYSTICK分频--1us的系统时钟中断
    if (SysTick_Config(SystemFrequency/1000000))
    {
   
        while (1);
    }
    time_delay=nus;//读取定时时间
    while(time_delay);
    SysTick->CTRL=0x00; //关闭计数器
    SysTick->VAL =0X00; //清空计数器
}
    //在中断中将time_delay递减。实现延时
void SysTick_Handler(void)
{
    if(time_delay)
        time_delay--;
}
