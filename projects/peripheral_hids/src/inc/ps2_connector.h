#ifndef PS2_CONNECTOR_H
#define PS2_CONNECTOR_H

uint8_t ps2_connector_get_next_code();
void ps2_connector_init(); 
void ps2_connector_send_byte(uint8_t msg);

#endif /*PS2_CONNECTOR_H*/