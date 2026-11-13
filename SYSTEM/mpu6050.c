#include "stm32f1xx_hal.h"
#include "IIC1.h"
#include "mpu6050.h"
#include "stdio.h"
#include "math.h"
TIM_HandleTypeDef htim2;
float yaw=0.0f;
float pitch= 0.0f;
float roll= 0.0f;
void tim2_init(void)
{
    __HAL_RCC_TIM2_CLK_ENABLE();
    htim2.Instance=TIM2;
    htim2.Init.Prescaler=72-1;     
    htim2.Init.CounterMode=TIM_COUNTERMODE_UP;
    htim2.Init.Period=50000-1;   
    htim2.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload=TIM_AUTORELOAD_PRELOAD_ENABLE;
    HAL_TIM_Base_Init(&htim2);
    HAL_NVIC_SetPriority(TIM2_IRQn, 3, 0);     
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
    HAL_TIM_Base_Start_IT(&htim2);
}
float mpu_x=0,mpu_y=0,mpu_z=0,mpu_ax=0,mpu_ay=0,mpu_az=0,mpu_t=0;
int mpu6050_is_ok()
{
	return HAL_I2C_IsDeviceReady(&hi2c1,mpu6050_address,3,100);
}
void mpu6050_write(uint8_t a,uint8_t b)
{
	uint8_t buf[2];
	buf[0]=a;
	buf[1]=b;
	iic1_send(mpu6050_address,buf,2);
}
uint8_t mpu6050_read(uint8_t a)
{
	uint8_t ch;
    iic1_read(mpu6050_address,a,&ch,1);
	return ch;
}
void mpu6050_init()
{
	mpu6050_write(0x6b,0x80);
	HAL_Delay(100);
	mpu6050_write(0x6b,0x00);
	mpu6050_write(0x1b,0x18);
	mpu6050_write(0x1c,0x00);
	
	mpu6050_write(0x19, 0x09);   /* 采样率 1kHz/(1+9) = 100Hz */
	mpu6050_write(0x1A, 0x03); 
}
void mpu6050_dataup(void)
{
	mpu_ax=(float)(int16_t)((mpu6050_read(0x3b)<<8)+mpu6050_read(0x3c));
	mpu_ay=(float)(int16_t)((mpu6050_read(0x3d)<<8)+mpu6050_read(0x3e));
	mpu_az=(float)(int16_t)((mpu6050_read(0x3f)<<8)+mpu6050_read(0x40));
	mpu_ax =mpu_ax*6.1035e-5f;
    mpu_ay =mpu_ay*6.1035e-5f;
    mpu_az =mpu_az*6.1035e-5f;
	mpu_t=(int16_t)((mpu6050_read(0x41)<<8)+mpu6050_read(0x42));
	mpu_t=mpu_t/340.0+36.53;
	mpu_x=(int16_t)((mpu6050_read(0x43)<<8)+mpu6050_read(0x44));
	mpu_y=(int16_t)((mpu6050_read(0x45)<<8)+mpu6050_read(0x46));
	mpu_z=(int16_t)((mpu6050_read(0x47)<<8)+mpu6050_read(0x48));
	mpu_x=mpu_x*6.09756e-2f;
	mpu_y=mpu_y*6.09756e-2f;
	mpu_z=mpu_z*6.09756e-2f;
}
void mpu6050_get0(void)
{
	float yaw_g=yaw+(mpu_z-0.133f)*0.05f;  
    float pitch_g=pitch+(mpu_x+7.438f)*0.05f;   
    float roll_g=roll+(mpu_y+1.102f)*0.05f;  
	float pitch_a=atan2f(mpu_ay,mpu_az)/3.1415927f*180.0f;
	float roll_a=atan2f(mpu_ax,mpu_az)/3.1415927f*180.0f;
	yaw=yaw_g;
	pitch=0.95238*pitch_a+(1-0.95238)*pitch_g;
	roll=0.95238*roll_a+(1-0.95238)*roll_g;
}
