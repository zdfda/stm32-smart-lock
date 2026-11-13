#include "stm32f1xx_hal.h"
#include "IIC1.h"
#include "stdio.h"
I2C_HandleTypeDef hi2c1;
void iic1_init()
{
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitTypeDef A;
	A.Mode=GPIO_MODE_AF_OD;
	A.Pull=GPIO_PULLUP;
	A.Pin=iicscl_pin;
	A.Speed=GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(iicscl_port,&A);
	A.Pin=iicsda_pin;
	HAL_GPIO_Init(iicsda_port,&A);
	__HAL_RCC_I2C1_CLK_ENABLE();
	hi2c1.Instance=I2C1;
	hi2c1.Init.AddressingMode=I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.ClockSpeed=100000;
	hi2c1.Init.DualAddressMode=I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.DutyCycle=I2C_DUTYCYCLE_16_9;
	hi2c1.Init.GeneralCallMode=I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode=I2C_NOSTRETCH_DISABLE;
	if(HAL_I2C_Init(&hi2c1)!=HAL_OK)
	{
		while(1)
		{
			printf("I2C init error\n");
			HAL_Delay(1000);
		}

	}
}
int iic1_send(uint16_t address,uint8_t* date,uint16_t size)
{
	return HAL_I2C_Master_Transmit(&hi2c1,address,date,size,100);
}
int iic1_receive(uint16_t address,uint8_t* date,uint16_t size)
{
	return HAL_I2C_Master_Receive(&hi2c1,address,date,size,1000);
}
int iic1_write(uint16_t address,uint16_t ex_address,uint8_t* date,uint16_t size)
{
	return HAL_I2C_Mem_Write(&hi2c1,address,ex_address,I2C_MEMADD_SIZE_8BIT,date,size,1000);
}
int iic1_read(uint16_t address,uint16_t ex_address,uint8_t* date,uint16_t size)
{
	return HAL_I2C_Mem_Read(&hi2c1,address,ex_address,I2C_MEMADD_SIZE_8BIT,date,size,1000);
}

