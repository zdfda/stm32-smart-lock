#ifndef MPU6050_H
#define MPU6050_H
#include "stm32f1xx_hal.h"
#define mpu6050_address 0xD0
extern TIM_HandleTypeDef htim2;
void tim2_init(void);
void mpu6050_init(void);
int mpu6050_is_ok(void);
void I2C_ScanBus(I2C_HandleTypeDef *hi2c);
extern float mpu_x,mpu_y,mpu_z,mpu_ax,mpu_ay,mpu_az,mpu_t;
extern float yaw,pitch,roll;
void mpu6050_dataup(void);
void mpu6050_get0(void);
#endif
