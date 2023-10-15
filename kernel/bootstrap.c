#include "drivers/video/vga.h"
#include "drivers/keyboard/keyboard.h"
#include "drivers/keyboard/ps2.h"
#include "interrupts.h"
#include "boot/multiboot.h"
#include "memory/kmalloc.h"
#include "memory/memory.h"
#include "string/string.h"
#include "bitmap.h"

void keypress_handler(key_t pressed_key, const uint8_t modifiers[]) {
    if (pressed_key.type == CHARACTER) {
        if (BITMAP_GET(modifiers, L_CONTROL) && (pressed_key.ascii == 'c')) {
            vga_print("^C");
            return;
        }
        vga_printf("%c", pressed_key.ascii);
    }
}

void keydown(key_t key) {
    vga_print("DOWN");
}

void keyup(key_t key) {
    vga_print("UP");
}

void kernel_bootstrap(void) {
    read_multiboot_info();
    kmalloc_init();
    clear_screen();
    vga_println("Hello World!");
    vga_test();
    initialize_interrupts();
    install_exception_handlers();
    interrupt_test();
    initialize_keyboard();
    vga_printf("printf tests below: %%%%%%\n");
    char* buf = "Hello World!";
    vga_printf("Decimal Test: %d, %i\nHex Test: 0x%x\nChar Test: %c\nString Test: %s\n",
                54321, -123, 0xFEEDFACE, '*', buf);
    register_simple_keyboard_listener(keypress_handler);
    register_raw_keydown_listener(keydown);
    register_raw_keyup_listener(keyup);
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
}