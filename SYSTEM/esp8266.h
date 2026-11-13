#ifndef ESP8266_H
#define ESP8266_H
#include "stm32f1xx_hal.h"

extern uint8_t stm32_mode;
void esp8266_init(void);
int send_esp_cmp(const char* pData,const char* comparedate1,uint32_t timeout_ms);
void up_state(void);
void debug_printf(void);
void state_up(void);
#endif
