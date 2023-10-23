#include "ps2.h"
#include "types.h"
#include "bitmap.h"
#include "keyboard.h"
#include "key_defs.h"
#include "cpu/pic.h"
#include "cpu/isr.h"
#include "drivers/io/ports.h"
#include "drivers/video/vga.h"

#define PS2_IO_ADDR 0x60
#define PS2_IRQ_ID 1

//TODO: n-key rollover

void initialize_keyboard(void) {
    register_handler(IRQ_IDX(PS2_IRQ_ID), (isr_t)ps2_interrupt_handler);
    pic_unmask(PS2_IRQ_ID);
}

// This should probably be handled by a keyboard layout system in the future
key_t key_table[0x145] = {
   NULL, ESCAPE, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, ZERO, MINUS, EQUALS, BACKSPACE,
   TAB, Q, W, E, R, T, Y, U, I, O, P, LBRACKET, RBRACKET, ENTER,
   LCTRL, A, S, D, F, G, H, J, K, L, SEMICOLON, SINGLEQUOTE, BACKTICK,
   LSHFT, BACKSLASH, Z, X, C, V, B, N, M, COMMA, PERIOD, SLASH, RSHFT, STAR,
   LALT, SPACE, CAPS, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, NLOCK, SLOCK,
   SEVEN, EIGHT, NINE, MINUS, FOUR, FIVE, SIX, PLUS, ONE, TWO, THREE, ZERO, PERIOD, NULL, NULL, NULL, F11, F12,
   PREV, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, SKIP, NULL, NULL, ENTER, RCTRL, NULL, NULL,
   MUTE, CALC, PLAY, NULL, STOP, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, VOLDOWN, NULL, VOLUP,
   NULL, WWWHOME, NULL, NULL, NULL, NULL, NULL, RALT, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
   HOME, UP, PAGEUP, NULL, LEFT, NULL, RIGHT, NULL, END, DOWN, PAGEDOWN, INSRT, DEL,
   NULL, NULL, NULL, NULL, NULL, NULL, NULL, LGUI, RGUI
};

char key_table_shifted[0x58] = {
    0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', 0,
    0, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', 0,
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',
    0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, 0,
    0, 0
};

uint8_t pressed_keys[BITMAP_SIZE(0x145)];
uint8_t active_modifiers[BITMAP_SIZE(NUM_MODS)];

bool extended = false;

void process_keydown(uint16_t keycode) {
    bool repeated = BITMAP_GET(pressed_keys, keycode);
    // Add key to held keys
    BITMAP_SET(pressed_keys, keycode);
    key_t pressed_key = key_table[keycode];
    if (!repeated) {
        handle_keydown_raw(pressed_key);
    }
    //TODO: move all this logic to the generic half of the driver
    if (BITMAP_GET(pressed_keys, 0x2A) | BITMAP_GET(pressed_keys, 0x36)) {
        if ((pressed_key.type == CHARACTER) && (keycode < 0x58) && (key_table_shifted[keycode] != 0)) {
            pressed_key.ascii = key_table_shifted[keycode];
        }
    }
    if (pressed_key.type == MODIFIER) {
        BITMAP_SET(active_modifiers, pressed_key.id);
    }
    handle_keypress_simple(pressed_key, active_modifiers);
}

void process_keyup(uint16_t keycode) {
    // Remove key from held keys
    BITMAP_CLEAR(pressed_keys, keycode);
    key_t released_key = key_table[keycode];
    handle_keyup_raw(released_key);
    if (released_key.type == MODIFIER) {
        BITMAP_CLEAR(active_modifiers, released_key.id);
    }
}

void ps2_interrupt_handler(interrupt_state state) {
    uint16_t scancode = inb(PS2_IO_ADDR);
    if (extended) {
        extended = false;
        if (scancode <= 0x5C) {
            process_keydown(scancode + 0x58 - 0xF);
        } else if ((scancode >= 0x90) && (scancode <= 0xDC)) {
            process_keyup(scancode + 0x58 - 0x8F);
        }
    } else if (scancode <= 0x58) {
        process_keydown(scancode);
    } else if ((scancode > 0x80) && (scancode <= 0xD8)) {
        process_keyup(scancode - 0x80);
    } else if (scancode == 0xE0) {
        extended = true;
    }
    pic_eoi(state.id);
}