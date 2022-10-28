#include "ps2.h"
#include "cpu/pic.h"
#include "cpu/isr.h"
#include "drivers/io/ports.h"
#include "drivers/video/vga.h"

#define PS2_IO_ADDR 0x60
#define PS2_IRQ_ID 1

void initialize_keyboard(void) {
    register_handler(IRQ_IDX(PS2_IRQ_ID), (isr_t)ps2_interrupt_handler);
    pic_unmask(PS2_IRQ_ID);
}

// TODO: Add support for shifted keys and modifier keys
char key_table[0x3A] = {
    0, 0, // null, escape
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    0, // tab
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, // lctrl
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, // lshift
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',
    0, // rshift
    '*',
    0, // lalt
    ' ',
    0 // caps
};

// TODO: Add support for holding keys
void ps2_interrupt_handler(interrupt_state state) {
    // vga_println("KEY");
    uint8_t scancode = inb(PS2_IO_ADDR);
    if (scancode > sizeof(key_table)) return;
    char buf[2];
    buf[0] = key_table[scancode];
    if (buf[0] != 0) {
        vga_print(buf);
    }
}