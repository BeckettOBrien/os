#include "string.h"

int digits(uint64_t val) {
    int out = 1;
    while (val /= 10) out++;
    return out;
}

void itoa(char* buf, uint64_t val) {
    int i = digits(val);
    buf[i] = '\0';
    do {
        buf[--i] = '0' + (val % 10);
    } while ((val /= 10) > 0);
}