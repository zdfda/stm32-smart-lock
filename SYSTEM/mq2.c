#include "stm32f1xx_hal.h"
#include "MQ2.h"
#include "stdio.h"
ADC_HandleTypeDef  hadc1;
uint32_t mq2_data;
void mq2_init(void)
{
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitTypeDef A;
	A.Mode=GPIO_MODE_ANALOG;
	A.Pin=mq2_pin;
	A.Pull=GPIO_NOPULL;
	A.Speed=0;
	HAL_GPIO_Init(mq2_port,&A);
	__HAL_RCC_ADC1_CLK_ENABLE();  
    hadc1.Instance=ADC1;
	hadc1.Init.ContinuousConvMode=DISABLE;
	hadc1.Init.DataAlign=ADC_DATAALIGN_RIGHT;
	hadc1.Init.DiscontinuousConvMode=DISABLE;
	hadc1.Init.ExternalTrigConv=ADC_SOFTWARE_START;
	hadc1.Init.NbrOfConversion=1;
	hadc1.Init.NbrOfDiscConversion=0;
	hadc1.Init.ScanConvMode=DISABLE;
    HAL_ADC_Init(&hadc1);
    HAL_ADCEx_Calibration_Start(&hadc1);
	ADC_ChannelConfTypeDef sConfig={0};
    sConfig.Channel=ADC_CHANNEL_8;       
    sConfig.Rank=ADC_REGULAR_RANK_1;
    sConfig.SamplingTime=ADC_SAMPLETIME_71CYCLES_5;
    HAL_ADC_ConfigChannel(&hadc1, &sConfig);
}
void getdata_mq2(void)
{
	uint32_t adc_value=0;
	for(uint8_t i=0;i<10;i++)
	{
		HAL_ADC_Start(&hadc1);
		if(HAL_ADC_PollForConversion(&hadc1,100) == HAL_OK)
		{
			adc_value+=HAL_ADC_GetValue(&hadc1);
		}
		else{printf("ADC timeout\n");}
	}
	mq2_data=adc_value/10;
	HAL_ADC_Stop(&hadc1);
}
