#include "ledkeybuzzer.h"
#include "stm32f1xx_hal.h"
TIM_HandleTypeDef htim1;//此处配置了tim1，时基单元为1ms
uint8_t key_mode=0;
uint8_t led1_state=0;
uint8_t led2_state=0;
uint8_t led3_state=0;
uint8_t led4_state=0;
uint8_t buzzer_state=0;
void led_init()
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_AFIO_CLK_ENABLE();
	__HAL_AFIO_REMAP_SWJ_DISABLE();
	GPIO_InitTypeDef A;
	A.Pin=led1_pin;
	A.Mode=GPIO_MODE_OUTPUT_OD;
	A.Speed=GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(led1_port,&A);
	A.Pin=led2_pin;
	HAL_GPIO_Init(led2_port,&A);
	A.Pin=led3_pin;
	HAL_GPIO_Init(led3_port,&A);
	A.Pin=led4_pin;
	HAL_GPIO_Init(led4_port,&A);
	led1_off();
	led2_off();
	led3_off();
	led4_off();
}
void key_init()
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	GPIO_InitTypeDef A;
	A.Pin=key1_pin;
	A.Mode=GPIO_MODE_INPUT;
	A.Pull=GPIO_NOPULL;
	HAL_GPIO_Init(key1_port,&A);
	A.Pin=key2_pin;
	HAL_GPIO_Init(key2_port,&A);
	A.Pin=key3_pin;
	HAL_GPIO_Init(key3_port,&A);
	A.Pin=key4_pin;
	HAL_GPIO_Init(key4_port,&A);
}
void beep_pwminit()
{
	__HAL_RCC_TIM1_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef A;
	A.Mode=GPIO_MODE_AF_PP;
	A.Pin=beep_pin;
	A.Speed=GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(beep_port, &A);
	htim1.Instance=TIM1;
	htim1.Init.AutoReloadPreload=TIM_AUTORELOAD_PRELOAD_ENABLE;
	htim1.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
	htim1.Init.CounterMode=TIM_COUNTERMODE_UP;
	htim1.Init.Period=100-1;
	htim1.Init.Prescaler=720-1;
	HAL_TIM_Base_Init(&htim1);
	TIM_OC_InitTypeDef C={0};
	C.OCMode=TIM_OCMODE_PWM1;
	C.Pulse=0;
	C.OCPolarity=TIM_OCPOLARITY_HIGH;
	HAL_TIM_PWM_ConfigChannel(&htim1,&C,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
	HAL_NVIC_SetPriority(TIM1_UP_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(TIM1_UP_IRQn);
	HAL_TIM_Base_Start_IT(&htim1);
	//__HAL_TIM_MOE_ENABLE(&htim1);
}
void led1_on()
{
	HAL_GPIO_WritePin(led1_port,led1_pin,GPIO_PIN_RESET);
	led1_state=1;
}
void led2_on()
{
	HAL_GPIO_WritePin(led2_port,led2_pin,GPIO_PIN_RESET);
	led2_state=1;
}
void led3_on()
{
	HAL_GPIO_WritePin(led3_port,led3_pin,GPIO_PIN_RESET);
	led3_state=1;
}
void led4_on()
{
	HAL_GPIO_WritePin(led4_port,led4_pin,GPIO_PIN_RESET);
	led4_state=1;
}
void led1_off()
{
	HAL_GPIO_WritePin(led1_port,led1_pin,GPIO_PIN_SET);
	led1_state=0;
}
void led2_off()
{
	HAL_GPIO_WritePin(led2_port,led2_pin,GPIO_PIN_SET);
	led2_state=0;
}
void led3_off()
{
	HAL_GPIO_WritePin(led3_port,led3_pin,GPIO_PIN_SET);
	led3_state=0;
}
void led4_off()
{
	HAL_GPIO_WritePin(led4_port,led4_pin,GPIO_PIN_SET);
	led4_state=0;
}
void beep_on(int n)
{
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,n);
	buzzer_state=n;
}
int read_key1()
{
	return HAL_GPIO_ReadPin(key1_port,key1_pin);
}
int read_key2()
{
	return HAL_GPIO_ReadPin(key2_port,key2_pin);
}
int read_key3()
{
	return HAL_GPIO_ReadPin(key3_port,key3_pin);
}
int read_key4()
{
	return HAL_GPIO_ReadPin(key4_port,key4_pin);
}

