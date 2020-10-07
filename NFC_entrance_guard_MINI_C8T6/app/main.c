#include "app.h"
#include "stm32f10x.h"               
#include <stdio.h>
#include "delay.h"
int main()
{	
	IWDG_Config(IWDG_Prescaler_64 ,2000);//原值625
	
	/* 检查是否为独立看门狗复位 */
  if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
  {
    /* 独立看门狗复位 */
    /* 清除标志 */
    RCC_ClearFlag();
		/*如果一直不喂狗，会一直复位，*/
  }
	/*系统初始化*/
	system_Init();
	/*为舵机提供的PWM信号配置*/
	PWM_IO_Config();
	/*综合应用程序,内部是一个非常大的死循环*/
	application();
while(1);

}


