#include "vga.h"
#include "memory/memory.h"
#include "string/string.h"
#include <stdarg.h>

#define VGA_ADDR 0xb8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

VGA_CHAR* VGA_MEM = (VGA_CHAR*)VGA_ADDR;
uint32_t VGA_CURSOR_X = 0;
uint32_t VGA_CURSOR_Y = 0;

void clear_screen(void) {
    for (int i = 0; i < (VGA_WIDTH * VGA_HEIGHT); i++) {
        VGA_MEM[i].character = ' ';
        VGA_MEM[i].color = VGA_COLOR(WHITE, BLACK);
    }
}

void vga_printf(char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    char* buf;
    vasprintf(&buf, fmt, args);
    vga_print(buf);
    kfree(buf);
}

void vga_println(char* str) {
    vga_print(str);
    vga_print("\n");
}

void vga_print(char* str) {
    vga_print_color(str, VGA_COLOR(WHITE, BLACK));
}

void vga_print_color(char* str, uint8_t color) {
    int i = 0;
    while (str[i] != '\0') {
        char c = str[i];
        if (c == '\n' || (VGA_CURSOR_X >= VGA_WIDTH)) {
            VGA_CURSOR_Y++;
            VGA_CURSOR_X = 0;
        }
        if (c == '\n') {
            i++;
            vga_scroll();
            continue;
        }
        if (c == '\b') {
            i++;
            if ((VGA_CURSOR_X > 0)) {
                VGA_CURSOR_X--;
                vga_print_at(' ', color, VGA_CURSOR_X, VGA_CURSOR_Y);
            }
            continue;
        }
        if (c == '\t') {
            i++;
            vga_print("    ");
            continue;
        }
        vga_scroll();
        vga_print_at(c, color, VGA_CURSOR_X, VGA_CURSOR_Y);
        VGA_CURSOR_X++;
        i++;
    }
}

void vga_print_at(char character, uint8_t color, uint32_t x, uint32_t y) {
    uint32_t idx = (VGA_WIDTH * y) + x;
    if (idx > (VGA_WIDTH * VGA_HEIGHT)) return;
    VGA_MEM[idx].character = character;
    VGA_MEM[idx].color = color;
}

void vga_scroll(void) {
    while (VGA_CURSOR_Y >= VGA_HEIGHT) {
        for (int i = 1; i < VGA_HEIGHT; i++) {
            void* dest = VGA_MEM + ((i-1) * VGA_WIDTH);
            void* src = VGA_MEM + (i * VGA_WIDTH);
            memcpy(dest, src, VGA_WIDTH * sizeof(VGA_CHAR));
        }
        VGA_CURSOR_Y--;
        memset(&VGA_MEM[VGA_CURSOR_Y*VGA_WIDTH], 0, VGA_WIDTH * sizeof(VGA_CHAR));
    }
}