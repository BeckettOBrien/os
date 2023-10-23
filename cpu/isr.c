#include "isr.h"
#include "pic.h"
#include "drivers/video/vga.h"
#include "string/string.h"

isr_t handlers[256];

void register_handler(int id, isr_t handler) {
    handlers[id] = handler;
}

void isr_handle(interrupt_state state) {
    isr_t handler = handlers[state.id];
    if (handler) {
        handler(state);
    } else {
        vga_print("Unhandled interrupt: 0x");
        char out[4];
        itoa(state.id, out, 16);
        vga_println(out);
        while (1) {}
        asm volatile("cli; hlt");
    }
}

void irq_handle(interrupt_state state) {
    // pic_eoi(state.id); // Better to let each irq handle this on its own so each one can decide to be blocking/nonblocking
    isr_handle(state);
}