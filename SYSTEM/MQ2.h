#ifndef MQ2_H
#define MQ2_H
#include "stm32f1xx_hal.h"
#define mq2_port GPIOB
#define mq2_pin GPIO_PIN_0
extern uint32_t mq2_data;
void mq2_init(void);
void getdata_mq2(void);
#endif
