#include "time.h"
#include "stm32f1xx_hal.h"
#include "esp8266.h"
#include "stdio.h"
uint64_t sec=0;
uint8_t mm=0,hh=0,month=0,week=0,day=0;
uint32_t days=0;
uint16_t year=0;
uint64_t uinx_time=0;
void timedata_up(void)
{
	if(uinx_time==0){return;}
	uint64_t uinx_time1=uinx_time/1000ULL;
    uint64_t t=uinx_time1+8ULL*3600ULL;
    sec=t%60;t/=60;
    mm=(uint8_t)(t%60);t/=60;
    hh=(uint8_t)(t%24);t/=24;
    days=(uint32_t)t;
    week=(uint8_t)((days+4)%7);
    year=1970;
    while (1)
	{
        uint8_t leap=((year%4==0&&year%100!=0)||(year%400==0))?1:0;
        uint16_t yd=leap?366:365;
        if(days<yd)break;
        days-=yd;
        year++;
    }
    uint8_t leap=((year%4==0&&year%100!=0)||(year%400==0))?1:0;
    month=1;
    while (1)
	{
        uint8_t md;
        switch(month)
		{
            case 1:case 3:case 5:case 7:
            case 8:case 10:case 12:md=31;break;
            case 4:case 6:case 9:case 11:md=30;break;
            case 2: md=leap?29:28;break;
            default:md=30; break;
        }
        if(days<md) break;
        days-=md;
        month++;
    }
    day = (uint8_t)(days + 1);
}

