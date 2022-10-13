#include "drivers/video/vga.h"

void kernel_bootstrap(void) {
    clear_screen();
    vga_print("Hello World!\n");
    vga_print("Newline test...\n");
    vga_print("Wrap and character test: !@#$%^&*()-+./:\'\"1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
    vga_print("\nColor test: ");
    for (uint8_t color = 0; color <= 16; color++) {
        vga_print_color("  ", VGA_COLOR(BLACK, color));
    }
}