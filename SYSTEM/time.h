#ifndef TIME_H
#define TIME_H
#include "stm32f1xx_hal.h"
extern uint64_t uinx_time;
extern uint64_t sec;
extern uint8_t mm,hh,month,week,day;
extern uint32_t days;
extern uint16_t year;
void timedata_up(void);

#endif
