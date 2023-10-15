#include "keyboard.h"
#include "bitmap.h"
#include "array/array.h"
#include "drivers/video/vga.h"

array_t simple_listeners = {0};
array_t raw_down_listeners = {0};
array_t raw_up_listeners = {0};

int register_simple_keyboard_listener(simple_keyhandler_t handler) {
    if (simple_listeners.elements == NULL) simple_listeners = array_alloc(sizeof(simple_keyhandler_t));
    return array_push(&simple_listeners, &handler);
}

int register_raw_keydown_listener(raw_keyhandler_t handler) {
    if (raw_down_listeners.elements == NULL) raw_down_listeners = array_alloc(sizeof(raw_keyhandler_t));
    return array_push(&raw_down_listeners, &handler);
}

int register_raw_keyup_listener(raw_keyhandler_t handler) {
    if (raw_up_listeners.elements == NULL) raw_up_listeners = array_alloc(sizeof(raw_keyhandler_t));
    return array_push(&raw_up_listeners, &handler);
}

void handle_keypress_simple(key_t pressed_key, const uint8_t modifiers[]) {
    for (int i = 0; i < simple_listeners.length; i++) {
        simple_keyhandler_t* handler = array_get(&simple_listeners, i);
        (*handler)(pressed_key, modifiers);
    }
}

void handle_keydown_raw(key_t pressed_key) {
    for (int i = 0; i < raw_down_listeners.length; i++) {
        raw_keyhandler_t* handler = array_get(&raw_down_listeners, i);
        (*handler)(pressed_key);
    }
}

void handle_keyup_raw(key_t released_key) {
    for (int i = 0; i < raw_up_listeners.length; i++) {
        raw_keyhandler_t* handler = array_get(&raw_up_listeners, i);
        (*handler)(released_key);
    }
}