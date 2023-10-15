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

key_t key_table[0x58] = {
   NULL, ESCAPE, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, ZERO, MINUS, EQUALS, BACKSPACE,
   TAB, Q, W, E, R, T, Y, U, I, O, P, LBRACKET, RBRACKET, ENTER,
   LCTRL, A, S, D, F, G, H, J, K, L, SEMICOLON, SINGLEQUOTE, BACKTICK,
   LSHFT, BACKSLASH, Z, X, C, V, B, N, M, COMMA, PERIOD, SLASH, RSHFT, STAR,
   LALT, SPACE
};

char key_table_shifted[0x58] = {
    0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', 0,
    0, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', 0,
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',
    0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, 0,
    0, 0
};

uint8_t pressed_keys[BITMAP_SIZE(0x58)];
uint8_t active_modifiers[BITMAP_SIZE(NUM_MODS)];

void process_keydown(uint8_t keycode) {
    bool repeated = BITMAP_GET(pressed_keys, keycode);
    // Add key to held keys
    BITMAP_SET(pressed_keys, keycode);
    key_t pressed_key = key_table[keycode];
    if (!repeated) {
        handle_keydown_raw(pressed_key);
    }
    if (BITMAP_GET(pressed_keys, 0x2A) | BITMAP_GET(pressed_keys, 0x36)) {
        if ((pressed_key.type == CHARACTER) && (key_table_shifted[keycode] != 0)) {
            pressed_key.ascii = key_table_shifted[keycode];
        }
    }
    if (pressed_key.type == MODIFIER) {
        BITMAP_SET(active_modifiers, pressed_key.id);
    }
    handle_keypress_simple(pressed_key, active_modifiers);
}

void process_keyup(uint8_t keycode) {
    // Remove key from held keys
    BITMAP_CLEAR(pressed_keys, keycode);
    key_t released_key = key_table[keycode];
    handle_keyup_raw(released_key);
    if (released_key.type == MODIFIER) {
        BITMAP_CLEAR(active_modifiers, released_key.id);
    }
}

void ps2_interrupt_handler(interrupt_state state) {
    uint8_t scancode = inb(PS2_IO_ADDR);
    if (scancode < 0x58) {
        process_keydown(scancode);
    } else if ((scancode > 0x80) && (scancode < 0x80 + 0x58)) {
        process_keyup(scancode - 0x80);
    }
}