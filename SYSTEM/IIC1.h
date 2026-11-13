#ifndef IIC1_H
#define IIC1_H
#include "stm32f1xx_hal.h"
#define iicscl_port GPIOB
#define iicscl_pin GPIO_PIN_6
#define iicsda_port GPIOB
#define iicsda_pin GPIO_PIN_7
extern I2C_HandleTypeDef hi2c1;

void iic1_init(void);
int iic1_send(uint16_t address,uint8_t* date,uint16_t size);
int iic1_receive(uint16_t address,uint8_t* date,uint16_t size);
int iic1_write(uint16_t address,uint16_t ex_address,uint8_t* date,uint16_t size);
int iic1_read(uint16_t address,uint16_t ex_address,uint8_t* date,uint16_t size);


#endif
