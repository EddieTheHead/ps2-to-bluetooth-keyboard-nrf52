#include "ps2_keyboard.h"

#include <zephyr.h>
#include <device.h>
#include <drivers/gpio.h>
#include <sys/util.h>
#include <sys/printk.h>
#include <inttypes.h>
#include <kernel.h>
#include <stdint.h>

#include "hog.h"
#include "usb_hid_keys.h"
#include "bt_hid_tools.h"
#include "ps2_connector.h"
#include "ps2_hid_translator.h"

#define PS2_KEYBOARD_THREAD_PRIORITY 5

#define SCROLL_LOCK_LED_MASK 0x01
#define NUM_LOCK_LED_MASK (0x01<<1)
#define CAPS_LOCK_LED_MASK (0x01<<2)

#define PS2_CMD_SET_LEDS 0xed 

void ps2_keyboard_set_leds(uint8_t scroll_lock, uint8_t num_lock, uint8_t caps_lock ){
	uint8_t command = 0;
	if(scroll_lock) command |= SCROLL_LOCK_LED_MASK;
	if(num_lock) command |= NUM_LOCK_LED_MASK;
	if(caps_lock) command |= CAPS_LOCK_LED_MASK;

	ps2_connector_send_byte(PS2_CMD_SET_LEDS);
	ps2_connector_send_byte(command);
} 

static void ps2_keyboard_thread(void *p1, void *p2, void *p3){
	while(1){
		ps2_hid_translator_parse_next_scancode();
		k_msleep(1);
	}
}

static K_THREAD_STACK_DEFINE(ps2_keyboard_thread_stack, 32 * 1024);
static struct k_thread ps2_keyboard_thread_data;

void ps2_keyboard_start_worker_thread(){
	k_thread_create(&ps2_keyboard_thread_data, ps2_keyboard_thread_stack,
		K_THREAD_STACK_SIZEOF(ps2_keyboard_thread_stack), ps2_keyboard_thread,
		NULL, NULL, NULL, PS2_KEYBOARD_THREAD_PRIORITY, 0, K_NO_WAIT);   
}

void ps2_keyboard_init()
{
	ps2_connector_init();
	ps2_keyboard_start_worker_thread();
}



