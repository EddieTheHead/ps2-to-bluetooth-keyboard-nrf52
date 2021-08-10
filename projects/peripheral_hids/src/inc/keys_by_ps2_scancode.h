
#ifndef KEYS_BY_PS2_SCANCODE_H
#define KEYS_BY_PS2_SCANCODE_H

#include <stdint.h>
#include "usb_hid_keys.h"

#define KEY_UNKNOWN 0x00

const uint8_t ps2_scan_code_to_usb_hid_key[] = { 
    [0x00] = KEY_UNKNOWN,
    [0x01] = KEY_F9,
    [0x02] = KEY_UNKNOWN,
    [0x03] = KEY_F5,
    [0x04] = KEY_F3,
    [0x05] = KEY_F1,
    [0x06] = KEY_F2,
    [0x07] = KEY_F12,
    [0x08] = KEY_UNKNOWN,
    [0x09] = KEY_F10,
    [0x0A] = KEY_F8,
    [0x0B] = KEY_F6,
    [0x0C] = KEY_F4,
    [0x0D] = KEY_TAB,
    [0x0E] = KEY_GRAVE,
    [0x0f] = KEY_UNKNOWN,
    [0x10] = KEY_UNKNOWN,
    [0x11] = KEY_MOD_LALT,
    [0x12] = KEY_MOD_LSHIFT,
    [0x13] = KEY_UNKNOWN,
    [0x14] = KEY_MOD_LCTRL,
    [0x15] = KEY_Q,
    [0x16] = KEY_1,
    [0x17] = KEY_UNKNOWN,
    [0x18] = KEY_UNKNOWN,
    [0x19] = KEY_UNKNOWN,
    [0x1A] = KEY_Z,
    [0x1B] = KEY_S,
    [0x1C] = KEY_A,
    [0x1D] = KEY_W,
    [0x1E] = KEY_2,
    [0x1f] = KEY_UNKNOWN,
    [0x20] = KEY_UNKNOWN,
    [0x21] = KEY_C,
    [0x22] = KEY_X,
    [0x23] = KEY_D,
    [0x24] = KEY_E,
    [0x25] = KEY_4,
    [0x26] = KEY_3,
    [0x27] = KEY_UNKNOWN,
    [0x28] = KEY_UNKNOWN,
    [0x29] = KEY_SPACE,
    [0x2A] = KEY_V,
    [0x2B] = KEY_F,
    [0x2C] = KEY_T,
    [0x2D] = KEY_R,
    [0x2E] = KEY_5,
    [0x2f] = KEY_UNKNOWN,
    [0x30] = KEY_UNKNOWN,
    [0x31] = KEY_N,
    [0x32] = KEY_B,
    [0x33] = KEY_H,
    [0x34] = KEY_G,
    [0x35] = KEY_Y,
    [0x36] = KEY_6,
    [0x37] = KEY_UNKNOWN,
    [0x38] = KEY_UNKNOWN,
    [0x39] = KEY_UNKNOWN,
    [0x3A] = KEY_M,
    [0x3B] = KEY_J,
    [0x3C] = KEY_U,
    [0x3D] = KEY_7,
    [0x3E] = KEY_8,
    [0x3f] = KEY_UNKNOWN,
    [0x40] = KEY_UNKNOWN,
    [0x41] = KEY_COMMA,
    [0x42] = KEY_K,
    [0x43] = KEY_I,
    [0x44] = KEY_O,
    [0x45] = KEY_0,
    [0x46] = KEY_9,
    [0x47] = KEY_UNKNOWN,
    [0x48] = KEY_UNKNOWN,
    [0x49] = KEY_DOT,
    [0x4A] = KEY_SLASH,
    [0x4B] = KEY_L,
    [0x4C] = KEY_SEMICOLON,
    [0x4D] = KEY_P,
    [0x4E] = KEY_MINUS,
    [0x4f] = KEY_UNKNOWN,
    [0x50] = KEY_UNKNOWN,
    [0x51] = KEY_UNKNOWN,
    [0x52] = KEY_APOSTROPHE,
    [0x53] = KEY_UNKNOWN,
    [0x54] = KEY_LEFTBRACE,
    [0x55] = KEY_EQUAL,
    [0x56] = KEY_UNKNOWN,
    [0x57] = KEY_UNKNOWN,
    [0x58] = KEY_CAPSLOCK,
    [0x59] = KEY_MOD_RSHIFT,
    [0x5A] = KEY_ENTER,
    [0x5B] = KEY_RIGHTBRACE,
    [0x5D] = KEY_BACKSLASH,
    [0x5c] = KEY_UNKNOWN,
    [0x5e] = KEY_UNKNOWN,
    [0x5f] = KEY_UNKNOWN,
    [0x60] = KEY_UNKNOWN,
    [0x61] = KEY_UNKNOWN,
    [0x62] = KEY_UNKNOWN,
    [0x63] = KEY_UNKNOWN,
    [0x64] = KEY_UNKNOWN,
    [0x65] = KEY_UNKNOWN,
    [0x66] = KEY_BACKSPACE,
    [0x67] = KEY_UNKNOWN,
    [0x68] = KEY_UNKNOWN,
    [0x69] = KEY_KP1,
    [0x6B] = KEY_KP4,
    [0x6C] = KEY_KP7,
    [0x6a] = KEY_UNKNOWN,
    [0x6d] = KEY_UNKNOWN,
    [0x6e] = KEY_UNKNOWN,
    [0x6f] = KEY_UNKNOWN,
    [0x70] = KEY_KP0,
    [0x71] = KEY_KPDOT,
    [0x72] = KEY_KP2,
    [0x73] = KEY_KP5,
    [0x74] = KEY_KP6,
    [0x75] = KEY_KP8,
    [0x76] = KEY_ESC,
    [0x77] = KEY_NUMLOCK,
    [0x78] = KEY_F11,
    [0x79] = KEY_KPPLUS,
    [0x7A] = KEY_KP3,
    [0x7B] = KEY_KPMINUS,
    [0x7C] = KEY_KPASTERISK,
    [0x7D] = KEY_KP9,
    [0x7E] = KEY_SCROLLLOCK,
    [0x7f] = KEY_UNKNOWN,
    [0x80] = KEY_UNKNOWN,
    [0x81] = KEY_UNKNOWN,
    [0x82] = KEY_UNKNOWN,
    [0x83] = KEY_F7,
    [0x84] = KEY_UNKNOWN,
    [0x85] = KEY_UNKNOWN,
    [0x86] = KEY_UNKNOWN,
    [0x87] = KEY_UNKNOWN,
    [0x88] = KEY_UNKNOWN,
    [0x89] = KEY_UNKNOWN,
    [0x8a] = KEY_UNKNOWN,
    [0x8b] = KEY_UNKNOWN,
    [0x8c] = KEY_UNKNOWN,
    [0x8d] = KEY_UNKNOWN,
    [0x8e] = KEY_UNKNOWN,
    [0x8f] = KEY_UNKNOWN,
    [0x90] = KEY_UNKNOWN,
    [0x91] = KEY_UNKNOWN,
    [0x92] = KEY_UNKNOWN,
    [0x93] = KEY_UNKNOWN,
    [0x94] = KEY_UNKNOWN,
    [0x95] = KEY_UNKNOWN,
    [0x96] = KEY_UNKNOWN,
    [0x97] = KEY_UNKNOWN,
    [0x98] = KEY_UNKNOWN,
    [0x99] = KEY_UNKNOWN,
    [0x9a] = KEY_UNKNOWN,
    [0x9b] = KEY_UNKNOWN,
    [0x9c] = KEY_UNKNOWN,
    [0x9d] = KEY_UNKNOWN,
    [0x9e] = KEY_UNKNOWN,
    [0x9f] = KEY_UNKNOWN,
    [0xa0] = KEY_UNKNOWN,
    [0xa1] = KEY_UNKNOWN,
    [0xa2] = KEY_UNKNOWN,
    [0xa3] = KEY_UNKNOWN,
    [0xa4] = KEY_UNKNOWN,
    [0xa5] = KEY_UNKNOWN,
    [0xa6] = KEY_UNKNOWN,
    [0xa7] = KEY_UNKNOWN,
    [0xa8] = KEY_UNKNOWN,
    [0xa9] = KEY_UNKNOWN,
    [0xaa] = KEY_UNKNOWN,
    [0xab] = KEY_UNKNOWN,
    [0xac] = KEY_UNKNOWN,
    [0xad] = KEY_UNKNOWN,
    [0xae] = KEY_UNKNOWN,
    [0xaf] = KEY_UNKNOWN,
    [0xb0] = KEY_UNKNOWN,
    [0xb1] = KEY_UNKNOWN,
    [0xb2] = KEY_UNKNOWN,
    [0xb3] = KEY_UNKNOWN,
    [0xb4] = KEY_UNKNOWN,
    [0xb5] = KEY_UNKNOWN,
    [0xb6] = KEY_UNKNOWN,
    [0xb7] = KEY_UNKNOWN,
    [0xb8] = KEY_UNKNOWN,
    [0xb9] = KEY_UNKNOWN,
    [0xba] = KEY_UNKNOWN,
    [0xbb] = KEY_UNKNOWN,
    [0xbc] = KEY_UNKNOWN,
    [0xbd] = KEY_UNKNOWN,
    [0xbe] = KEY_UNKNOWN,
    [0xbf] = KEY_UNKNOWN,
    [0xc0] = KEY_UNKNOWN,
    [0xc1] = KEY_UNKNOWN,
    [0xc2] = KEY_UNKNOWN,
    [0xc3] = KEY_UNKNOWN,
    [0xc4] = KEY_UNKNOWN,
    [0xc5] = KEY_UNKNOWN,
    [0xc6] = KEY_UNKNOWN,
    [0xc7] = KEY_UNKNOWN,
    [0xc8] = KEY_UNKNOWN,
    [0xc9] = KEY_UNKNOWN,
    [0xca] = KEY_UNKNOWN,
    [0xcb] = KEY_UNKNOWN,
    [0xcc] = KEY_UNKNOWN,
    [0xcd] = KEY_UNKNOWN,
    [0xce] = KEY_UNKNOWN,
    [0xcf] = KEY_UNKNOWN,
    [0xd0] = KEY_UNKNOWN,
    [0xd1] = KEY_UNKNOWN,
    [0xd2] = KEY_UNKNOWN,
    [0xd3] = KEY_UNKNOWN,
    [0xd4] = KEY_UNKNOWN,
    [0xd5] = KEY_UNKNOWN,
    [0xd6] = KEY_UNKNOWN,
    [0xd7] = KEY_UNKNOWN,
    [0xd8] = KEY_UNKNOWN,
    [0xd9] = KEY_UNKNOWN,
    [0xda] = KEY_UNKNOWN,
    [0xdb] = KEY_UNKNOWN,
    [0xdc] = KEY_UNKNOWN,
    [0xdd] = KEY_UNKNOWN,
    [0xde] = KEY_UNKNOWN,
    [0xdf] = KEY_UNKNOWN,
    [0xe0] = KEY_UNKNOWN,
    [0xe1] = KEY_UNKNOWN,
    [0xe2] = KEY_UNKNOWN,
    [0xe3] = KEY_UNKNOWN,
    [0xe4] = KEY_UNKNOWN,
    [0xe5] = KEY_UNKNOWN,
    [0xe6] = KEY_UNKNOWN,
    [0xe7] = KEY_UNKNOWN,
    [0xe8] = KEY_UNKNOWN,
    [0xe9] = KEY_UNKNOWN,
    [0xea] = KEY_UNKNOWN,
    [0xeb] = KEY_UNKNOWN,
    [0xec] = KEY_UNKNOWN,
    [0xed] = KEY_UNKNOWN,
    [0xee] = KEY_UNKNOWN,
    [0xef] = KEY_UNKNOWN,
    [0xf0] = KEY_UNKNOWN,
    [0xf1] = KEY_UNKNOWN,
    [0xf2] = KEY_UNKNOWN,
    [0xf3] = KEY_UNKNOWN,
    [0xf4] = KEY_UNKNOWN,
    [0xf5] = KEY_UNKNOWN,
    [0xf6] = KEY_UNKNOWN,
    [0xf7] = KEY_UNKNOWN,
    [0xf8] = KEY_UNKNOWN,
    [0xf9] = KEY_UNKNOWN,
    [0xfa] = KEY_UNKNOWN,
    [0xfb] = KEY_UNKNOWN,
    [0xfc] = KEY_UNKNOWN,
    [0xfd] = KEY_UNKNOWN,
    [0xfe] = KEY_UNKNOWN,

};


#endif /* KEYS_BY_SCANCODE_H */
