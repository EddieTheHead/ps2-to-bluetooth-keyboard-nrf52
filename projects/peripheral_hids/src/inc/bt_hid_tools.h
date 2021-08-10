#ifndef USB_HID_TOOLS_H
#define USB_HID_TOOLS_H
#include <stdint.h>

int ascii_to_hid(uint8_t ascii);
void keyboard_print(const char* str);

#endif /*USB_HID_TOOLS_H*/