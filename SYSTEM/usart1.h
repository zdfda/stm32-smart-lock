#ifndef USART1_H
#define USART1_H
#include "stm32f1xx_hal.h"



#define usart1tx_port GPIOA
#define usart1tx_pin GPIO_PIN_9
#define usart1rx_port GPIOA
#define usart1rx_pin GPIO_PIN_10

extern UART_HandleTypeDef husart1;
extern uint8_t rx1_date; 
extern uint8_t usart1_buf[256];
void usart1_init(uint32_t baudrate);
#endif
