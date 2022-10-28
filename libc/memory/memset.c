#include "memory.h"

void* memset(void* dest, int val, uint64_t n) {
    for (uint64_t i = 0; i < n; i++) {
        ((uint8_t*)dest)[i] = (uint8_t)val;
    }
    return dest;
}