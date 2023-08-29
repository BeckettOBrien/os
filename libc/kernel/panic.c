#include "panic.h"
#include "drivers/video/vga.h"

__attribute__((noreturn)) void panic(void) {
    vga_print("Kernel panic! Halting...");
    asm volatile("cli; hlt");
    while (1) {}
}