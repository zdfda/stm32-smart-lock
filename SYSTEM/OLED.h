#ifndef OLED_H
#define OLED_H
#include "stm32f1xx_hal.h"
#define oled_address  0x78
int oled_is_ok(void);
void oled_send_cmd(uint8_t a);
void oled_send_data(uint8_t a);
void oled_init(void);
void oled_clear(void);
void oled_test(void);
void oled_set_pos(uint8_t page, uint8_t col);
void oled_putc(uint8_t x, uint8_t y, char ch);
void oled_printf(uint8_t x, uint8_t y, const char *buf);
void oled_show_image(const uint8_t *img);
#endif
