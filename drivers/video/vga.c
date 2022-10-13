#include "vga.h"

VGA_CHAR* VGA_MEM = (VGA_CHAR*)VGA_ADDR;
uint32_t VGA_CURSOR_X = 0;
uint32_t VGA_CURSOR_Y = 0;

void clear_screen(void) {
    for (int i = 0; i < (VGA_WIDTH * VGA_HEIGHT); i++) {
        VGA_MEM[i].character = ' ';
        VGA_MEM[i].color = VGA_COLOR(WHITE, BLACK);
    }
}

void vga_print(char* str) {
    vga_print_color(str, VGA_COLOR(WHITE, BLACK));
}

void vga_print_color(char* str, uint8_t color) {
    int i = 0;
    while (str[i] != '\0') {
        if (str[i] == '\n' || (VGA_CURSOR_X >= VGA_WIDTH)) {
            VGA_CURSOR_Y++;
            VGA_CURSOR_X = 0;
        }
        if (str[i] == '\n') {
            i++;
            continue;
        }
        vga_print_at(str[i], color, VGA_CURSOR_X, VGA_CURSOR_Y);
        VGA_CURSOR_X++;
        i++;
    }
}

void vga_print_at(char character, uint8_t color, uint32_t x, uint32_t y) {
    uint32_t idx = (VGA_WIDTH * y) + x;
    VGA_MEM[idx].character = character;
    VGA_MEM[idx].color = color;
}