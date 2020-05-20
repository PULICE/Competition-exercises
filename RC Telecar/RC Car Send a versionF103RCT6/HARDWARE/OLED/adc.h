#ifndef __ADC_H_
#define __ADC_H_


#include "stm32f10x_adc.h"

void Adc_GPIOInit(void);
unsigned int ADC1_Conv(int Channel);

extern float Avg_Slope;//max=4.6,min=4.0;ƽ��б��mV/C
extern float V25;//max=1.52,min=1.34;��25Cʱ�ĵ�ѹ
extern float Voltage_Ratio;//����12λ�ֱ���
#endif
