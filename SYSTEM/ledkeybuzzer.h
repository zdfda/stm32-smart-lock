#ifndef __LEDKEYBUZZER_H__
#define __LEDKEYBUZZER_H__
#include "stm32f1xx_hal.h"
#define  led1_port GPIOA
#define  led2_port GPIOA
#define  led3_port GPIOA
#define  led4_port GPIOA
#define  led1_pin GPIO_PIN_11
#define  led2_pin GPIO_PIN_12
#define  led3_pin GPIO_PIN_13
#define  led4_pin GPIO_PIN_14

#define  beep_port GPIOA
#define  beep_pin GPIO_PIN_8

#define  key1_port GPIOA
#define  key2_port GPIOA
#define  key3_port GPIOC
#define  key4_port GPIOC
#define  key1_pin GPIO_PIN_1
#define  key2_pin GPIO_PIN_7
#define  key3_pin GPIO_PIN_4
#define  key4_pin GPIO_PIN_5

extern uint8_t led1_state;
extern uint8_t led2_state;
extern uint8_t led3_state;
extern uint8_t led4_state;
extern uint8_t buzzer_state;

extern TIM_HandleTypeDef htim1;
extern uint8_t key_mode;
void led_init(void);
void key_init(void);
void beep_pwminit(void);
void led1_on(void);
void led2_on(void);
void led3_on(void);
void led4_on(void);
void led1_off(void);
void led2_off(void);
void led3_off(void);
void led4_off(void);
int read_key1(void);
int read_key2(void);
int read_key3(void);
int read_key4(void);
void beep_on(int n);
#endif
