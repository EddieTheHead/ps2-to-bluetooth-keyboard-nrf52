#include <zephyr.h>
#include <device.h>
#include <drivers/gpio.h>
#include <sys/util.h>
#include <sys/printk.h>
#include <inttypes.h>
#include <kernel.h>
#include "ps2_connector.h"

#define PS2_CLK_NODE DT_ALIAS(ps2clk)
#define PS2_DATA_NODE DT_ALIAS(ps2data)

#if DT_NODE_HAS_STATUS(PS2_CLK_NODE, okay)
#define PS2_CLK_GPIO_LABEL	DT_GPIO_LABEL(PS2_CLK_NODE, gpios)
#define PS2_CLK_GPIO_PIN	DT_GPIO_PIN(PS2_CLK_NODE, gpios)
#endif

#define PS2_GPIO_FLAGS_HIGH (GPIO_INPUT)
#define PS2_GPIO_FLAGS_LOW (GPIO_OUTPUT | GPIO_ACTIVE_HIGH | GPIO_OPEN_DRAIN)

#if DT_NODE_HAS_STATUS(PS2_DATA_NODE, okay)
#define PS2_DATA_GPIO_LABEL	DT_GPIO_LABEL(PS2_DATA_NODE, gpios)
#define PS2_DATA_GPIO_PIN	DT_GPIO_PIN(PS2_DATA_NODE, gpios)
#endif

static struct gpio_callback clk_fall_cb_data;

#define PS2_MSG_QUE_SIZE 20
char __aligned(1) rx_msg_que_buffer[PS2_MSG_QUE_SIZE];
static struct k_msgq rx_msg_que;

static const unsigned char bit_reverse_table[] = {
  0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0, 
  0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8, 0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8, 
  0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4, 0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4, 
  0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC, 0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC, 
  0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2, 0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2, 
  0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA, 0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA,
  0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6, 0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6, 
  0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE, 0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE,
  0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1, 0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1,
  0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9, 0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9, 
  0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5, 0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5,
  0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED, 0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD,
  0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3, 0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3, 
  0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB, 0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB,
  0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7, 0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7, 
  0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF, 0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF
};

uint8_t reverse(uint8_t x){
	return bit_reverse_table[x];
}

typedef enum{
	PS2_STATE_IDLE = 0,
	// PS2_STATE_INIT,
	PS2_STATE_DEV_TO_HOST,
	PS2_STATE_HOST_TO_DEV
} PS2_state_e;


static int current_bit_no = 0;
static uint8_t incoming_data; 
static uint8_t outgoing_data;

static volatile PS2_state_e PS2_state = PS2_STATE_IDLE;

static bool odd_parity_check(uint8_t byte){
	byte ^= byte >> 4;
	byte ^= byte >> 2;
	byte ^= byte >> 1;
	return (-byte & 1);	
}

static inline void set_clk_low(){
	const struct device *clk_line = device_get_binding(PS2_CLK_GPIO_LABEL);
	gpio_pin_configure(clk_line, PS2_CLK_GPIO_PIN, PS2_GPIO_FLAGS_LOW);
	gpio_pin_set(clk_line, PS2_CLK_GPIO_PIN, GPIO_OUTPUT_ACTIVE);
}

static inline void set_clk_high_z(){
	const struct device *clk_line = device_get_binding(PS2_CLK_GPIO_LABEL);
	gpio_pin_configure(clk_line, PS2_CLK_GPIO_PIN, PS2_GPIO_FLAGS_HIGH);
}

static inline void set_data_low(){
	const struct device *data_line = device_get_binding(PS2_DATA_GPIO_LABEL);
	gpio_pin_configure(data_line, PS2_DATA_GPIO_PIN, PS2_GPIO_FLAGS_LOW);
	gpio_pin_set(data_line, PS2_DATA_GPIO_PIN, GPIO_OUTPUT_INACTIVE);
}

static inline void set_data_high_z(){
	const struct device *data_line = device_get_binding(PS2_DATA_GPIO_LABEL);
	gpio_pin_configure(data_line, PS2_DATA_GPIO_PIN, PS2_GPIO_FLAGS_HIGH);
}

static void host_to_dev_init_request_to_send(){
	const struct device *clk_line = device_get_binding(PS2_CLK_GPIO_LABEL);
	// const struct device *data_line = device_get_binding(PS2_DATA_GPIO_LABEL);
	// int err;

	printk("host_to_dev_init_request_to_send\n");

	// pull clock low for at least 100us
	set_clk_low();
	k_usleep(110);
	// pull data low 
	set_data_low();
	k_usleep(10);
	// release clock while keeping data low
	set_clk_high_z();
	
	// prepare data for transmission
	current_bit_no = 0;

	// wait for device to pull clock low 
	printk("wait for device to pull clock low \n");
 	gpio_pin_interrupt_configure(clk_line,
					   PS2_CLK_GPIO_PIN,
					   GPIO_INT_EDGE_TO_INACTIVE);
}

/* blocking function */
void ps2_connector_send_byte(uint8_t msg){
	const struct device *data_line = device_get_binding(PS2_DATA_GPIO_LABEL);
	const struct device *clk_line = device_get_binding(PS2_CLK_GPIO_LABEL);

	outgoing_data = msg;

	PS2_state = PS2_STATE_HOST_TO_DEV;
	host_to_dev_init_request_to_send();
	while( PS2_state != PS2_STATE_IDLE){
		// allow other threads do their job while sending
		k_msleep(1);
	};
	// Wait for the device to release Data and Clock
	while( gpio_pin_get(data_line, PS2_DATA_GPIO_PIN) == 0 ||
		   gpio_pin_get(clk_line, PS2_CLK_GPIO_PIN) == 0 ) {
	}

}

static void host_to_dev_serve_falling_clk(){
	int current_bit = 0;
	printk("host_to_dev_serve_falling_clk\n");
	PS2_state = PS2_STATE_DEV_TO_HOST;
	// start bit was already send by host_to_dev_init_request_to_send
	current_bit = 0x01 & (outgoing_data >> current_bit_no);
	if(current_bit){
		set_data_high_z();
	}
	else
	{
		set_data_low();
	}
	// parity bit
	if(current_bit_no == 8){
		current_bit = !odd_parity_check(outgoing_data);
		if(current_bit){
			set_data_high_z();
		}
		else
		{
			set_data_low();
		}		
	}
	// stop bit 
	if(current_bit_no == 8){
		set_data_high_z();
	}
	if(current_bit_no == 9){
		set_data_high_z();
		// consider transmission done
		PS2_state = PS2_STATE_IDLE;
	}
	current_bit_no++;
	current_bit_no %= 10;

}

static void dev_to_host_serve_falling_clk(){
	const struct device *data_line = device_get_binding(PS2_DATA_GPIO_LABEL);
	int current_bit = 0;

	// printk("a\n");

	// k_usleep(15);
	current_bit = gpio_pin_get(data_line, PS2_DATA_GPIO_PIN);

	// start bit
	if(current_bit_no == 0){
		if(current_bit != 0){
			//generate error
			printk("ERROR: dev_to_host_serve_falling_clk: incoming data start bit check failed!\n");
			current_bit_no = 0;
			return;
		}
	}
	// 8 data bits
	if(current_bit_no < 9){
		// LSB first
		incoming_data <<= 1;
		incoming_data &= 0b11111110;
		incoming_data |= current_bit;
	}
	// parity bit
	if(current_bit_no == 9){
		if(odd_parity_check(incoming_data) == current_bit){
			// generate error
			printk("ERROR: dev_to_host_serve_falling_clk: incoming data parity check failed, data: 0x%x!\n", incoming_data);
			current_bit_no = 0;
			return;			
		}
	}
	// stop bit
	if(current_bit_no == 10){
		if(current_bit != 1){
			//generate error
			printk("ERROR: dev_to_host_serve_falling_clk: incoming data expected high stop bit, got low instead!\n");
			current_bit_no = 0;
			return;			
		}
		else{
			// code ok, push to msg queue
			// printk( "PS2_connector: recived code: 0x%x\n", incoming_data);
			k_msgq_put(&rx_msg_que, (void*) &incoming_data,K_NO_WAIT);
		}
	}
	current_bit_no++;
	// no more than 10
	current_bit_no %= 11;
}

static void clk_fall_cb(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins)
{
	switch (PS2_state)
	{
	case PS2_STATE_DEV_TO_HOST:
	case PS2_STATE_IDLE:
		dev_to_host_serve_falling_clk();
		break;
	case PS2_STATE_HOST_TO_DEV:
		host_to_dev_serve_falling_clk();
		break;
	default:
		break;
	}
}

uint8_t ps2_connector_get_next_code(){
	uint8_t result;
	k_msgq_get(&rx_msg_que, &result, K_FOREVER);
	result = reverse(result);
	// printk("processing code: %x\n", result);
	return result;
}

void ps2_connector_init(){
	const struct device *data_line = device_get_binding(PS2_DATA_GPIO_LABEL);
	const struct device *clk_line = device_get_binding(PS2_CLK_GPIO_LABEL);
	int err = 0;

	k_msgq_init(&rx_msg_que, rx_msg_que_buffer, 1, PS2_MSG_QUE_SIZE);
	// power on keyboard
	
	// wait until data and clock are high 
	printk("wait until data and clock are high\n");
	err = gpio_pin_configure(clk_line, PS2_CLK_GPIO_PIN, GPIO_INPUT);
	if (err != 0) {
		printk("Error %d: failed to configure %s pin %d\n",
		       err, PS2_CLK_GPIO_LABEL, PS2_CLK_GPIO_PIN);
		return;
	}
	err = gpio_pin_configure(data_line, PS2_DATA_GPIO_PIN, GPIO_INPUT);
	if (err != 0) {
		printk("Error %d: failed to configure %s pin %d\n",
		       err, PS2_DATA_GPIO_LABEL, PS2_DATA_GPIO_PIN);
		return;
	}
	while( gpio_pin_get(data_line, PS2_DATA_GPIO_PIN) == 0 ||
		   gpio_pin_get(clk_line, PS2_CLK_GPIO_PIN) == 0 ) {
	}
	// start receiving data
	PS2_state = PS2_STATE_DEV_TO_HOST;
	gpio_pin_interrupt_configure(clk_line,
					   PS2_CLK_GPIO_PIN,
					   GPIO_INT_EDGE_TO_INACTIVE);	
	gpio_init_callback(&clk_fall_cb_data, clk_fall_cb, BIT(PS2_CLK_GPIO_PIN));
	gpio_add_callback(clk_line, &clk_fall_cb_data);

	// wait for 0xAA - BAT code
	// printk("wait for 0xAA - BAT code!\n");

	printk("PS2 keyboard ready!\n");


}