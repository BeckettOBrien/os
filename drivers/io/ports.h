#pragma once
#include "types.h"

static inline void outb(uint16_t port, uint8_t byte) {
    asm("outb %0, %1" : : "a" (byte), "Nd" (port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm("inb %1, %0" : "=a" (ret) : "Nd" (port));
    return ret;
}