#include "interrupts.h"
#include "cpu/isr.h"
#include "cpu/pic.h"
#include "cpu/idt.h"
#include "string/string.h"
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

void general_protection_fault_handler(interrupt_state state) {
    vga_print("General Protection Fault detected at instruction 0x");
    char addr[20];
    itoa(state.ip, addr, 16);
    vga_println(addr);
    asm volatile("hlt");
    while (1) {}
}

void page_fault_handler(interrupt_state state) {
    vga_println("Page Fault!");
    uint64_t addr;
    asm volatile("movq %%cr2, %0" : "=r" (addr));
    char addr_out[20];
    itoa(addr, addr_out, 16);
    vga_print("Attempted access: 0x");
    vga_println(addr_out);
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
    register_handler(0x0, (isr_t)division_overflow_handler);
    register_handler(0x3, (isr_t)breakpoint_handler);
    register_handler(0x8, (isr_t)double_fault_handler);
    register_handler(0xD, (isr_t)general_protection_fault_handler);
    register_handler(0xE, (isr_t)page_fault_handler);
}