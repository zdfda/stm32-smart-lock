#ifndef USART3_H
#define USART3_H
#include "stm32f1xx_hal.h"



#define usart3tx_port GPIOB
#define usart3tx_pin GPIO_PIN_10
#define usart3rx_port GPIOB
#define usart3rx_pin GPIO_PIN_11

extern UART_HandleTypeDef husart3;
extern uint8_t rx3_date; 
extern uint8_t usart3_buf[512];
extern volatile uint8_t* usart3_head;
extern volatile uint8_t* usart3_tail;

extern uint8_t usart3_buf2[512];
extern volatile uint8_t* usart3_head2;
extern volatile uint8_t* usart3_tail2;

extern uint8_t usart3_flag;
void usart3_init(uint32_t baudrate);

#endif
