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
        vga_print("Unhandled interrupt: ");
        char out[4];
        itoa(out, state.id);
        vga_println(out);
        while (1) {}
        asm volatile("cli; hlt");
    }
}

void irq_handle(interrupt_state state) {
    pic_eoi(state.id);
    isr_handle(state);
}