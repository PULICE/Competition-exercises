#include "adc.h"

float Avg_Slope=4.3/1000;//max=4.6,min=4.0;平均斜率mV/C因需要将单位转为mmV/C所以除以1000；
float V25=1.43;//max=1.52,min=1.34;在25C时的电压
float Voltage_Ratio =3.3/4095;//就是12位分辨率
/*	
*由数据手册公式(温度)={(V25-V_SENSE)/Avg_Slope}+25
*Value(温度)=(float)((V25-Tp_ucValue*Voltage_Ratio)/Avg_Slope)+25;温度转换公式
*等价公式 Value=(float)25+(5855.85-3.3*(Tp_ucValue/10))/17.6085;
*/

void Adc_GPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	ADC_InitTypeDef  ADC_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1,ENABLE);
	
// 	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//分频因子6时钟为72M/6=12MHz
// 	ADC_DeInit(ADC1);  //将外设 ADC1 的全部寄存器重设为缺省值
	
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_6;
 	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;  //ADC工作模式:ADC1和ADC2工作在独立模式
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;   //模数转换工作在单通道模式
	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE; //模数转换工作在单次转换模式
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;  //ADC数据右对齐
	ADC_InitStruct.ADC_NbrOfChannel = 1; //顺序进行规则转换的ADC通道的数目
  ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //转换由软件而不是外部触发启动
	ADC_Init(ADC1, &ADC_InitStruct); //根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器
/*————————————————
版权声明：本文为CSDN博主「雷克斯派」的原创文章，遵循 CC 4.0 BY-SA 版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/windows260/article/details/51136072*/
//  //配置电压采集8通道
//	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,1,ADC_SampleTime_1Cycles5);
//	ADC_RegularChannelConfig(ADC1,ADC_Channel_2,2,ADC_SampleTime_1Cycles5);
//	ADC_RegularChannelConfig(ADC1,ADC_Channel_3,3,ADC_SampleTime_1Cycles5);
//	ADC_RegularChannelConfig(ADC1,ADC_Channel_6,4,ADC_SampleTime_1Cycles5);

	ADC_Cmd(ADC1,ENABLE); //使能指定的ADC1
	ADC_StartCalibration(ADC1);//启动选定的ADC校准；
	while(ADC_GetCalibrationStatus(ADC1));//ADC1校准完成后返回RESET；
}

unsigned int ADC1_Conv(int Channel)
{
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);//使能ADC软启动转换；
	ADC_RegularChannelConfig(ADC1,Channel,1,ADC_SampleTime_239Cycles5);
	while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));//等待转换结束逻辑取反！即为1条件为假；
	ADC_ClearFlag(ADC1,ADC_FLAG_EOC);//清除转换结束标志
	return ADC_GetConversionValue(ADC1);//返回最后一次常规通道转换的结果数据；
}

