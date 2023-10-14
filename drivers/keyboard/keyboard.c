#include "keyboard.h"
#include "bitmap.h"
#include "drivers/video/vga.h"

//TODO: Create dynamic arrays of keyboard listeners (raw and preformatted)

void handle_keydown_raw(key pressedkey) {

}

void handle_keyup_raw(key releasedkey) {

}

void handle_keypress_simple(key pressedkey, const uint8_t modifiers[]) {
    if (pressedkey.type == CHARACTER) {
        if (BITMAP_GET(modifiers, L_CONTROL) && (pressedkey.ascii == 'c')) {
            vga_print("^C");
            return;
        }
        vga_printf("%c", pressedkey.ascii);
    }
}