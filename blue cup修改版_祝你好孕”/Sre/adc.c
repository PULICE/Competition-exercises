#include "adc.h"
#include "stm32f10x.h"
  
void Adc_GPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	ADC_InitTypeDef  ADC_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_ADC1,ENABLE);
	
// 	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//��Ƶ����6ʱ��Ϊ72M/6=12MHz
// 	ADC_DeInit(ADC1);  //������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ
	
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0;
 	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;  //ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;   //ģ��ת�������ڵ�ͨ��ģʽ
	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE; //ģ��ת�������ڵ���ת��ģʽ
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;  //ADC�����Ҷ���
	ADC_InitStruct.ADC_NbrOfChannel = 1; //˳����й���ת����ADCͨ������Ŀ
  ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //ת��������������ⲿ��������
	ADC_Init(ADC1, &ADC_InitStruct); //����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���
/*��������������������������������
��Ȩ����������ΪCSDN�������׿�˹�ɡ���ԭ�����£���ѭ CC 4.0 BY-SA ��ȨЭ�飬ת���븽��ԭ�ĳ������Ӽ���������
ԭ�����ӣ�https://blog.csdn.net/windows260/article/details/51136072*/
  //���õ�ѹ�ɼ�8ͨ��
 	ADC_RegularChannelConfig(ADC1,ADC_Channel_8,1,ADC_SampleTime_1Cycles5);
	ADC_Cmd(ADC1,ENABLE); //ʹ��ָ����ADC1
	ADC_StartCalibration(ADC1);//����ѡ����ADCУ׼��
	while(ADC_GetCalibrationStatus(ADC1));//ADC1У׼��ɺ󷵻�RESET��
}

unsigned int ADC1_Conv(void)
{
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);//ʹ��ADC������ת����
	while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));//�ȴ�ת�������߼�ȡ������Ϊ1����Ϊ�٣�
	ADC_ClearFlag(ADC1,ADC_FLAG_EOC);//���ת��������־
	return ADC_GetConversionValue(ADC1);//�������һ�γ���ͨ��ת���Ľ�����ݣ�
}




