#pragma once
#include "types.h"

#define VGA_COLOR(fg, bg) ((bg << 4) | fg)
// --- VGA Colors ---
#define BLACK 0x0
#define BLUE 0x1
#define GREEN 0x2
#define CYAN 0x3
#define RED 0x4
#define MAGENTA 0x5
#define BROWN 0x6
#define GREY 0x7
#define DARKGREY 0x8
#define BRIGHTBLUE 0x9
#define BRIGHTGREEN 0xA
#define BRIGHTCYAN 0xB
#define BRIGHTRED 0xC
#define BRIGHTMAGENTA 0xD
#define YELLOW 0xE
#define WHITE 0xF
// ------------------

typedef struct {
    char character;
    uint8_t color;
} VGA_CHAR;

void clear_screen(void);
void vga_printf(char* fmt, ...);
void vga_print(char* str);
void vga_println(char* str);
void vga_print_color(char* str, uint8_t color);
void vga_print_at(char character, uint8_t color, uint32_t x, uint32_t y);
void vga_scroll(void);