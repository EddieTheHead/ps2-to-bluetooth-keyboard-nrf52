#ifndef PS2_KEYBOARD_H
#define PS2_KEYBOARD_H

#include <stdint.h>

void ps2_keyboard_init();
void ps2_keyboard_start_worker_thread();
void ps2_keyboard_set_leds(uint8_t scroll_lock, uint8_t num_lock, uint8_t caps_lock );

#endif /* PS2_KEYBOARD_H */