#ifndef DH11_H
#define DH11_H
#include "stm32f1xx_hal.h"
#define dh11_pin GPIO_PIN_0
#define dh11_port GPIOC

extern TIM_HandleTypeDef htim3;
uint8_t dh11_start(void);
extern void delay_us(uint32_t us);
void tim3_init(void);
void dh11_read(void);
extern uint8_t h_g,h_d,t_g,t_d;
extern uint8_t dh11_start_flag;
#endif
