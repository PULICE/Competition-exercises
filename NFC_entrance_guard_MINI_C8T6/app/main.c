#include "app.h"
#include "stm32f10x.h"               
#include <stdio.h>
#include "delay.h"
int main()
{	
	IWDG_Config(IWDG_Prescaler_64 ,2000);//ԭֵ625
	
	/* ����Ƿ�Ϊ�������Ź���λ */
  if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
  {
    /* �������Ź���λ */
    /* �����־ */
    RCC_ClearFlag();
		/*���һֱ��ι������һֱ��λ��*/
  }
	/*ϵͳ��ʼ��*/
	system_Init();
	/*Ϊ����ṩ��PWM�ź�����*/
	PWM_IO_Config();
	/*�ۺ�Ӧ�ó���,�ڲ���һ���ǳ������ѭ��*/
	application();
while(1);

}


