#ifndef PAJ7620_H
#define PAJ7620_H

#include <stdint.h>

#define paj7620_address 0xE6

#define PAJ7620_REG_BANK 0xEF
#define PAJ7620_REG_GESTURE 0x43

typedef enum
{
    PAJ_G_NONE=0,
    PAJ_G_RIGHT,
    PAJ_G_LEFT,
    PAJ_G_UP,
    PAJ_G_DOWN,
    PAJ_G_FORWARD,
    PAJ_G_BACKWARD,
    PAJ_G_CW,
    PAJ_G_CCW,
    PAJ_G_WAVE
} paj_gesture_t;

int paj7620_is_ok(void);
int paj7620_init(void);
int paj7620_read_raw(uint8_t *g0, uint8_t *g1);
paj_gesture_t paj7620_read_gesture(void);
void paj7620_demo_loop(void);
extern uint8_t move_state;
#endif
