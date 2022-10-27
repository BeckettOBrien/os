#include "memory.h"

void memcpy(void* dest, const void* src, uint64_t num) {
    for (uint64_t i = 0; i < num; i++) {
        ((char*)dest)[i] = ((char*)src)[i];
    }
}