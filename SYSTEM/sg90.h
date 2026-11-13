#ifndef SG90_h
#define SG90_H
#include "stm32f1xx_hal.h"
extern TIM_HandleTypeDef htim4;
#define sg90_port GPIOB
#define sg90_pin GPIO_PIN_8|GPIO_PIN_9
void sg90_init(void);
void ctrl_sg90(uint8_t a);
#endif
