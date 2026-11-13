#include "usart3.h"
UART_HandleTypeDef husart3;
uint8_t rx3_date; 
uint8_t usart3_buf[512];
volatile uint8_t* usart3_head=usart3_buf;
volatile uint8_t* usart3_tail=usart3_buf;
uint8_t usart3_flag=0;

uint8_t usart3_buf2[512];
volatile uint8_t* usart3_head2=usart3_buf;
volatile uint8_t* usart3_tail2=usart3_buf;

void usart3_init(uint32_t baudrate)
{
	
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_USART3_CLK_ENABLE();
	GPIO_InitTypeDef A;
	A.Pin=usart3tx_pin;
	A.Mode=GPIO_MODE_AF_PP;
	A.Speed=GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(usart3tx_port,&A);

	A.Pin=usart3rx_pin;
	A.Mode=GPIO_MODE_AF_INPUT;
	A.Pull=GPIO_PULLUP;
	A.Speed=0;
	HAL_GPIO_Init(usart3rx_port,&A);

	husart3.Instance=USART3;
	husart3.Init.BaudRate =baudrate;
	husart3.Init.WordLength=UART_WORDLENGTH_8B;
	husart3.Init.StopBits=UART_STOPBITS_1;
	husart3.Init.Parity=UART_PARITY_NONE;
	husart3.Init.Mode=UART_MODE_TX_RX;
	husart3.Init.HwFlowCtl=UART_HWCONTROL_NONE;
	husart3.Init.OverSampling=UART_OVERSAMPLING_16;
	HAL_UART_Init(&husart3);
	HAL_NVIC_SetPriority(USART3_IRQn,1,1);
	HAL_NVIC_EnableIRQ(USART3_IRQn);
	HAL_UART_Receive_IT(&husart3,&rx3_date,1);
}
