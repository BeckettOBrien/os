#include "interrupts.h"
#include "cpu/isr.h"
#include "cpu/pic.h"
#include "cpu/idt.h"
#include "drivers/video/vga.h"

// Basic interrupt handlers
void division_overflow_handler(interrupt_state state) {
    vga_println("Division overflow exception.");
    asm volatile("hlt");
    while (1) {}
}

void breakpoint_handler(interrupt_state state) {
    vga_print("BREAK");
}

void double_fault_handler(interrupt_state state) {
    vga_println("Double Fault Detected. Halting...");
    asm volatile("hlt");
    while (1) {}
}
// ------------------------

void initialize_interrupts() {
    pic_remap(32, 32 + 8);
    fill_idt();
    load_idt();
    asm volatile("sti");
}

void install_exception_handlers() {
    register_handler(0, (isr_t)division_overflow_handler);
    register_handler(3, (isr_t)breakpoint_handler);
    register_handler(8, (isr_t)double_fault_handler);
}