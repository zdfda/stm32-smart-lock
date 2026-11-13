#include "stm32f1xx_HAL.h"
#include "sg90.h"
TIM_HandleTypeDef htim4;
//此处配置了tim4，时基单元为20ms
void sg90_init()
{
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitTypeDef A;
	A.Mode=GPIO_MODE_AF_PP;
	A.Pin=sg90_pin;
	A.Pull=GPIO_NOPULL;
	A.Speed=GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(sg90_port,&A);
	__HAL_RCC_TIM4_CLK_ENABLE();
	htim4.Instance=TIM4;
	htim4.Init.AutoReloadPreload=TIM_AUTORELOAD_PRELOAD_ENABLE;
	htim4.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
	htim4.Init.CounterMode=TIM_COUNTERMODE_UP;
	htim4.Init.Period=20000-1;
	htim4.Init.Prescaler=72-1;
	HAL_TIM_Base_Init(&htim4);
	TIM_OC_InitTypeDef sConfig;
	sConfig.OCMode=TIM_OCMODE_PWM1;
	sConfig.Pulse=0;
	sConfig.OCPolarity=TIM_OCPOLARITY_HIGH;
	HAL_TIM_PWM_ConfigChannel(&htim4,&sConfig,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_3);
	HAL_TIM_PWM_ConfigChannel(&htim4,&sConfig,TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_4);
	
}
void ctrl_sg90(uint8_t a)
{
	a%=181;
	__HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_3,(int)(a*2000/180)+500);
	__HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_4,(int)((180-a)*2000/180)+500);
}
