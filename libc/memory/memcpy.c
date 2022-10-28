#include "memory.h"

void memcpy(void* dest, const void* src, uint64_t n) {
    for (uint64_t i = 0; i < n; i++) {
        ((uint8_t*)dest)[i] = ((char*)src)[i];
    }
}