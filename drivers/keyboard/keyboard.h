#pragma once
#include "types.h"

enum KEY_TYPE {
    CHARACTER,
    MODIFIER,
    OTHER // Media and function keys
};

enum MOD_IDS {
    L_SHIFT, R_SHIFT,
    L_CONTROL, R_CONTROL,
    L_ALT, R_ALT,
    CAPS, NUMLOCK, SCRLLOCK,
    NUM_MODS
};

enum OTHER_IDS {
    FN_1, FN_2, FN_3, FN_4, FN_5, FN_6, FN_7, FN_8, FN_9, FN_10, FN_11, FN_12,
    PREV_TRACK, NEXT_TRACK, MEDIA_MUTE, MEDIA_CALCULATOR, MEDIA_PLAY, MEDIA_STOP,
    VOLUME_DOWN, VOLUME_UP, WWW_HOME, HOME, ARROW_UP, PAGE_UP, ARROW_LEFT, ARROW_RIGHT, END, ARROW_DOWN, PAGE_DOWN,
    INSERT, L_GUI, R_GUI
};

struct key {
    enum KEY_TYPE type;
    union {
        char ascii;
        enum MOD_IDS id; // Also used for OTHER_IDS
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