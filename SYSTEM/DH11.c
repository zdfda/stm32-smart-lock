#include "DH11.h"
#include "stm32f1xx_hal.h"
#include "stdio.h"

TIM_HandleTypeDef htim3;
uint8_t h_g,h_d,t_g,t_d;
uint8_t dh11_start_flag=0;
void tim3_init(void)
{
	__HAL_RCC_TIM3_CLK_ENABLE();
	htim3.Instance=TIM3;
	htim3.Init.AutoReloadPreload=TIM_AUTORELOAD_PRELOAD_ENABLE;
	htim3.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
	htim3.Init.CounterMode=TIM_COUNTERMODE_UP;
	htim3.Init.Period=0xFFFF;
	htim3.Init.Prescaler=72-1;
	HAL_TIM_Base_Init(&htim3);
	HAL_TIM_Base_Start(&htim3);
}
uint8_t dh11_start(void)
{
	__HAL_RCC_GPIOC_CLK_ENABLE();
	GPIO_InitTypeDef A;
	A.Mode=GPIO_MODE_OUTPUT_PP;
	A.Pin=dh11_pin;
	A.Pull=GPIO_NOPULL;
	A.Speed=GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(dh11_port,&A);
	HAL_GPIO_WritePin(dh11_port,dh11_pin,GPIO_PIN_RESET);
	delay_us(18000);
	HAL_GPIO_WritePin(dh11_port,dh11_pin,GPIO_PIN_SET);
	delay_us(40);
	A.Mode=GPIO_MODE_INPUT;
	HAL_GPIO_Init(dh11_port,&A);
	if(HAL_GPIO_ReadPin(dh11_port,dh11_pin)==GPIO_PIN_SET)
	{
		printf("DH start fail\n");
		return 0;
	}
	delay_us(80);
	if(HAL_GPIO_ReadPin(dh11_port,dh11_pin)==GPIO_PIN_RESET)
	{
		printf("DH start fail\n");
		return 0;
	}
	return 1;
}
void delay_us(uint32_t us)
{
	uint32_t start = __HAL_TIM_GET_COUNTER(&htim3);
    while ((__HAL_TIM_GET_COUNTER(&htim3) - start)<us);
}
void dh11_read(void)
{
    uint8_t data[5] = {0},a;
	if(dh11_start()==0){return;}
	uint32_t t;
    for (int i = 0; i < 40; i++)
	{
		while(HAL_GPIO_ReadPin(dh11_port,dh11_pin)!=GPIO_PIN_SET);
		t=__HAL_TIM_GET_COUNTER(&htim3);
		while(HAL_GPIO_ReadPin(dh11_port,dh11_pin)!=GPIO_PIN_RESET);
		a= __HAL_TIM_GET_COUNTER(&htim3)-t;
        if(a>50){data[i/8]|=1;}
		data[i/8]<<=1;
    }
	h_g=data[0];
    h_d=data[1];
    t_g=data[2];
    t_d=data[3];
}

