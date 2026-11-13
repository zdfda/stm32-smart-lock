#include "usart1.h"
UART_HandleTypeDef husart1;
uint8_t rx1_date; 
uint8_t usart1_buf[256];
void usart1_init(uint32_t baudrate)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_USART1_CLK_ENABLE();
	GPIO_InitTypeDef A;
	A.Pin=usart1tx_pin;
	A.Mode=GPIO_MODE_AF_PP;
	A.Speed=GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(usart1tx_port,&A);

	A.Pin=usart1rx_pin;
	A.Mode=GPIO_MODE_AF_INPUT;
	A.Pull=GPIO_PULLUP;
	A.Speed=0;
	HAL_GPIO_Init(usart1rx_port,&A);

	husart1.Instance=USART1;
	husart1.Init.BaudRate=baudrate;
	husart1.Init.WordLength=UART_WORDLENGTH_8B;
	husart1.Init.StopBits=UART_STOPBITS_1;
	husart1.Init.Parity=UART_PARITY_NONE;
	husart1.Init.Mode=UART_MODE_TX_RX;
	husart1.Init.HwFlowCtl=UART_HWCONTROL_NONE;
	husart1.Init.OverSampling=UART_OVERSAMPLING_16;
	HAL_UART_Init(&husart1);
	HAL_NVIC_SetPriority(USART1_IRQn,1,1);
	HAL_NVIC_EnableIRQ(USART1_IRQn);
	HAL_UART_Receive_IT(&husart1,&rx1_date,1);
}
