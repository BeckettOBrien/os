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

typedef struct key key;

void handle_keydown_raw(key pressedkey);
void handle_keyup_raw(key releasedkey);
void handle_keypress_simple(key pressedkey, const uint8_t modifiers[]);