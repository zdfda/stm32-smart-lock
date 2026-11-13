/** 
  ******************************************************************************
  * @file    GPIO/GPIO_IOToggle/Src/stm32f1xx_it.c
  * @author  MCD Application Team
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "stm32f1xx_it.h"
#include "usart1.h"
#include "usart3.h"
#include "ledkeybuzzer.h"
#include "string.h"
#include "stdio.h"
#include "dh11.h"
#include "esp8266.h"
#include "time.h"
#include "mpu6050.h"
/** @addtogroup STM32F1xx_HAL_Examples
  * @{
  */

/** @addtogroup GPIO_IOToggle
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
volatile uint32_t hf_sp = 0;
volatile uint32_t hf_pc = 0;
volatile uint32_t hf_lr = 0;
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
	uint32_t *sp = (uint32_t *)__get_MSP();
    hf_sp  = (uint32_t)sp;
    hf_lr  = sp[5];
    hf_pc  = sp[6];
	printf("1");
  while (1)
  {

  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  HAL_IncTick();
}

/******************************************************************************/
/*                 STM32F1xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f1xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/


/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
void USART1_IRQHandler()
{
	HAL_UART_IRQHandler(&husart1);
}
void USART3_IRQHandler()
{
	HAL_UART_IRQHandler(&husart3);
}
void TIM1_UP_IRQHandler()
{
	HAL_TIM_IRQHandler(&htim1);
}
void TIM2_IRQHandler()
{
	HAL_TIM_IRQHandler(&htim2);
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	
    if (htim->Instance == TIM1)
	{	
		if(uinx_time!=0){uinx_time++;}
		//dh11开启标志位,5s反转一次标志位
		static uint32_t dh11_cnt=0;
		if(dh11_start_flag==0)dh11_cnt++;
		if(dh11_cnt>=5000){dh11_start_flag=1;dh11_cnt=0;}
		//按键的消抖
        static uint32_t k1_cnt=0,k2_cnt = 0,k3_cnt=0,k4_cnt=0;
		static uint8_t  k1_lat=0,k2_lat = 0,k3_lat=0,k4_lat=0;
		if (read_key1()==GPIO_PIN_RESET){if(k1_cnt<50)k1_cnt++;}else{k1_cnt=0;k1_lat=0;}
		if (read_key2()==GPIO_PIN_RESET){if(k2_cnt<50)k2_cnt++;}else{k2_cnt=0;k2_lat=0;}
		if (read_key3()==GPIO_PIN_RESET){if(k3_cnt<50)k3_cnt++;}else{k3_cnt=0;k3_lat=0;}
		if (read_key4()==GPIO_PIN_RESET){if(k4_cnt<50)k4_cnt++;}else{k4_cnt=0;k4_lat=0;}
		if(k1_cnt>=50&&!k1_lat)
		{
			k1_lat=1;
			if(key_mode==0){key_mode=1;}
			else if(key_mode==1){key_mode=5;}
			else if(key_mode==5){key_mode=6;}
			else if(key_mode==6){key_mode=7;}
			else{key_mode=0;}
		}
		else if(k2_cnt>=50&&!k2_lat)
		{
			k2_lat=1;
			key_mode=(key_mode==2)?0:2;
		}
		else if(k3_cnt>=50&&!k3_lat)
		{
			k3_lat=1;
			key_mode =(key_mode==3)?0:3;
		}
		else if(k4_cnt>=50&&!k4_lat)
		{
			k4_lat=1;
			key_mode=(key_mode==4)?0:4;
		}
    }
	if (htim->Instance==TIM2)
	{
		mpu6050_get0();
	}
}

static uint8_t usart1_cnt=0;
uint8_t usart3_printfbuf[256];
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1)
	{
		//1. 处理收到的数据
		usart1_buf[usart1_cnt++] =rx1_date;
		if(usart1_cnt >= 1 && usart1_buf[0]!=':')//判断帧头，帧头以 : 开始
		{
			usart1_cnt = 0;//帧头错误，丢弃接收到的数据
			printf("input error\n");
		}
		if(rx1_date=='.')//接收到帧尾，数据包接收完成
		{
			if(strncmp((char*)usart1_buf, ":led1 on.", 9) == 0)
			{
				led1_on();
			}
			if(strncmp((char*)usart1_buf, ":led1 off.", 10) == 0)
			{
				led1_off();
			}
			usart1_cnt=0;
		}
		HAL_UART_Receive_IT(&husart1,&rx1_date, 1);
	}
	if(huart->Instance==USART3)
	{
		if(((usart3_tail+1-usart3_buf)%256)!=(usart3_head-usart3_buf))
		{
			*usart3_tail=rx3_date;
			if (++usart3_tail>=usart3_buf+256)
				usart3_tail = usart3_buf;
		}
		if(((usart3_tail2+1-usart3_buf2)%256)!=(usart3_head2-usart3_buf2))
		{
			*usart3_tail2=rx3_date;
			if(++usart3_tail2>=usart3_buf2+256)
			usart3_tail2 = usart3_buf2;
		}
		HAL_UART_Receive_IT(&husart3,&rx3_date,1);
	}
	
}
