#pragma once
#include "types.h"

enum KEY_TYPE {
    CHARACTER,
    MODIFIER,
    MEDIA
};

enum KEY_IDS {
    L_SHIFT, R_SHIFT,
    L_CONTROL, R_CONTROL,
    L_ALT, R_ALT,
    CAPS,
    NUM_MODS
};

struct key {
    enum KEY_TYPE type;
    union {
        char ascii;
        enum KEY_IDS id;
    };
};

typedef struct key key_t;

typedef void (*simple_keyhandler_t)(key_t, const uint8_t[]);
typedef void (*raw_keyhandler_t)(key_t);

int register_simple_keyboard_listener(simple_keyhandler_t handler);
int register_raw_keydown_listener(raw_keyhandler_t handler);
int register_raw_keyup_listener(raw_keyhandler_t handler);

void handle_keydown_raw(key_t pressed_key);
void handle_keyup_raw(key_t released_key);
void handle_keypress_simple(key_t pressed_key, const uint8_t modifiers[]);