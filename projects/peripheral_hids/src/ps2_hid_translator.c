#include "keys_by_ps2_scancode.h"
#include <stdbool.h>
#include <stdint.h>
#include <zephyr.h>
#include "hog.h"
#include "ps2_connector.h"

#define EXTENDED_CODE 0xe0
#define BREAK_CODE 0xf0

static const uint8_t pause_break_code[] = {0xE1, 0x14, 0x77, 0xE1, 0xF0, 0x14, 0xf0, 0x77};

static const uint8_t sys_req_make[] = { 0xE0, 0x12, 0xE0, 0x7C};
static const uint8_t sys_req_break[] = { 0xE0, 0xF0, 0x7C, 0xE0, 0xF0, 0x12};

static struct hids_report_keyboard key_report = { .modifier = 0};

static int pos_at_pressed_keys(uint8_t key){
    for( int i = 0; i <6; ++i){
        if(key_report.keycode[i] == key){
            return i;
        }          
    }
    return -1;    
}

static void press_mod_key(uint8_t key){
    // printk("press mod key: %x\n", key);
    key_report.modifier |= key;
    hog_set_keyboard_input(&key_report);
}

static void release_mod_key(uint8_t key){
    // printk("relase mod: %x\n", key);
    key_report.modifier &= ~key;
    hog_set_keyboard_input(&key_report);
}

static void press_key(uint8_t key){
    // printk("press key: %x\n", key);
    if(key == KEY_UNKNOWN) return;
    if(pos_at_pressed_keys(key) == -1){
        for(int i = 5; i >=0; --i){
            if(key_report.keycode[i] == 0){
               key_report.keycode[i] = key;
               hog_set_keyboard_input(&key_report);
               return; 
            }
        }
        /* there's more than 6 keys pressed at once, 
           drop the oldest*/
        key_report.keycode[5] = key;
        hog_set_keyboard_input(&key_report);
    }
}

static void release_key(uint8_t key){
    int pos; 
    // printk("relase key: %x\n", key);
    if(key == KEY_UNKNOWN) return;

    pos = pos_at_pressed_keys(key);
    
    if( pos == -1) return;
    key_report.keycode[pos] = 0;
    hog_set_keyboard_input(&key_report);
}

static void press_pause_break(){
    // printk("press pause / break\n");
    press_key(KEY_PAUSE);
    k_msleep(10);
    release_key(KEY_PAUSE);
}

static uint8_t ps2_extended_code_to_hid_key(uint8_t code){
    switch (code)
    {
        case 0x1F: return KEY_LEFTMETA;
        case 0x11: return KEY_RIGHTALT;
        case 0x27: return KEY_RIGHTMETA;
        case 0x2F: return KEY_PROPS;
        case 0x14: return KEY_RIGHTCTRL;
        case 0x70: return KEY_INSERT;
        case 0x6C: return KEY_HOME;
        case 0x7D: return KEY_PAGEUP;
        case 0x71: return KEY_DELETE;
        case 0x69: return KEY_END;
        case 0x7A: return KEY_PAGEDOWN;
        case 0x75: return KEY_UP;
        case 0x6B: return KEY_LEFT;
        case 0x72: return KEY_DOWN;
        case 0x74: return KEY_RIGHT;
        case 0x4A: return KEY_KPSLASH;
        case 0x5A: return KEY_ENTER;
        default: return KEY_UNKNOWN;
    }
}

void ps2_hid_translator_parse_next_scancode(){
    uint8_t code; //read from que
    uint8_t key;
    int i;

    code = ps2_connector_get_next_code();

    switch (code)
    {
        
    case 0x11: /* LALT*/
    case 0x12: /* LSHIFT*/ 
    case 0x14: /* LCTRL*/
    case 0x59: /* RSHIFT*/ 
        key = ps2_scan_code_to_usb_hid_key[code];
        press_mod_key(key);        
        break;
    case EXTENDED_CODE:
        code = ps2_connector_get_next_code();
        if(code == BREAK_CODE) {
            code = ps2_connector_get_next_code();
            if(code == sys_req_break[2]){
                for( i = 3; i< sizeof(sys_req_break); ++i ){
                    code = ps2_connector_get_next_code();
                    if(sys_req_break[i] != code){
                        return;
                    }
                }
                release_key(KEY_SYSRQ);
                return;         
            }  
            key = ps2_extended_code_to_hid_key(code); 
            if(key == KEY_RIGHTALT  ||
               key == KEY_RIGHTCTRL ||
               key == KEY_LEFTMETA  ||
               key == KEY_RIGHTMETA ){
                release_mod_key(key);
            }
            else{
                release_key(key);
            }         
        }
        else if(code == sys_req_make[1]){

            for( i = 2; i< sizeof(sys_req_make); ++i ){
                code = ps2_connector_get_next_code();
                if(sys_req_make[i] != code){
                    return;
                }
            }
            press_key(KEY_SYSRQ);          
        }      
        else{
            key = ps2_extended_code_to_hid_key(code);
            if(key == KEY_RIGHTALT  ||
               key == KEY_RIGHTCTRL ||
               key == KEY_LEFTMETA  ||
               key == KEY_RIGHTMETA ){
                press_mod_key(key);
            }
            else
            {
                press_key(key); 
            }     
        }
        break;
    case BREAK_CODE:
        code = ps2_connector_get_next_code();
        key = ps2_scan_code_to_usb_hid_key[code];
        if( code == 0x11 || /* LALT*/
            code == 0x12 || /* LSHIFT*/ 
            code == 0x14 || /* LCTRL*/
            code == 0x59    /* RSHIFT*/ ){
                release_mod_key(key);
                break;
            }
        release_key(key);
        break; 
    case 0xE1: /* PAUSE_BREAK*/
        // printk("Got 0xe1\n");
        for( i = 1; i< sizeof(pause_break_code); ++i ){
            code = ps2_connector_get_next_code();
            if(pause_break_code[i] != code){
                return;
            }
        }
        press_pause_break();
        break;      
    default:
        key = ps2_scan_code_to_usb_hid_key[code];
        press_key(key);
        break;
    }
}