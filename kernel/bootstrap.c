#include "drivers/video/vga.h"
#include "interrupts.h"

void kernel_bootstrap(void) {
    clear_screen();
    vga_println("Hello World!");
    vga_test();
    initialize_interrupts();
    install_exception_handlers();
    interrupt_test();
    while (1) {}
}

void vga_test(void) {
    vga_print("Newline test...\n");
    vga_println("Wrap and character test: !@#$%^&*()-+./:\'\"1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
    vga_print("Color test: ");
    for (uint8_t color = 0; color <= 16; color++) {
        vga_print_color("  ", VGA_COLOR(BLACK, color));
    }
    vga_print("\n");
}

void interrupt_test(void) {
    asm volatile("int3");
    vga_println("POINT");
    vga_print("Divide by zero test: ");
    int t = 1 / 0;
    vga_println("Done");
}