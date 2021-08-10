#include "bt_hid_tools.h"

#include <stdint.h>
#include <zephyr.h>

#include "hog.h"
#include "usb_hid_keys.h"


void keyboard_print(const char* str){
	struct hids_report_keyboard key_report = { .modifier = 0};

	while(*str){
		key_report.keycode[5] = 0;
		hog_set_keyboard_input(&key_report);
		k_msleep(100);
		key_report.keycode[5] = ascii_to_hid(*str++);
		hog_set_keyboard_input(&key_report);
		k_msleep(100);
	}
	
	key_report.keycode[5] = 0;
	hog_set_keyboard_input(&key_report);	
}

int ascii_to_hid(uint8_t ascii)
{
	if (ascii < 32) {
		/* Character not supported */
		return -1;
	} else if (ascii < 48) {
		/* Special characters */
		switch (ascii) {
		case 32:
			return KEY_SPACE;
		case 33:
			return KEY_1;
		case 34:
			return KEY_APOSTROPHE;
		case 35:
			return KEY_3;
		case 36:
			return KEY_4;
		case 37:
			return KEY_5;
		case 38:
			return KEY_7;
		case 39:
			return KEY_APOSTROPHE;
		case 40:
			return KEY_9;
		case 41:
			return KEY_0;
		case 42:
			return KEY_8;
		case 43:
			return KEY_EQUAL;
		case 44:
			return KEY_COMMA;
		case 45:
			return KEY_MINUS;
		case 46:
			return KEY_DOT;
		case 47:
			return KEY_SLASH;
		default:
			return -1;
		}
	} else if (ascii < 58) {
		/* Numbers */
		if (ascii == 48U) {
			return KEY_0;
		} else {
			return ascii - 19;
		}
	} else if (ascii < 65) {
		/* Special characters #2 */
		switch (ascii) {
		case 58:
			return KEY_SEMICOLON;
		case 59:
			return KEY_SEMICOLON;
		case 60:
			return KEY_COMMA;
		case 61:
			return KEY_EQUAL;
		case 62:
			return KEY_DOT;
		case 63:
			return KEY_SLASH;
		case 64:
			return KEY_2;
		default:
			return -1;
		}
	} else if (ascii < 91) {
		/* Uppercase characters */
		return ascii - 61U;
	} else if (ascii < 97) {
		/* Special characters #3 */
		switch (ascii) {
		case 91:
			return KEY_LEFTBRACE;
		case 92:
			return KEY_BACKSLASH;
		case 93:
			return KEY_RIGHTBRACE;
		case 94:
			return KEY_6;
		case 95:
			return KEY_MINUS;
		case 96:
			return KEY_GRAVE;
		default:
			return -1;
		}
	} else if (ascii < 123) {
		/* Lowercase letters */
		return ascii - 93;
	} else if (ascii < 128) {
		/* Special characters #4 */
		switch (ascii) {
		case 123:
			return KEY_LEFTBRACE;
		case 124:
			return KEY_BACKSLASH;
		case 125:
			return KEY_RIGHTBRACE;
		case 126:
			return KEY_GRAVE;
		case 127:
			return KEY_DELETE;
		default:
			return -1;
		}
	}

	return -1;
}