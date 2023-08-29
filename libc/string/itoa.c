#include "string.h"

int digits(uint64_t val, int radix) {
    int out = 1;
    while (val /= radix) out++;
    return out;
}

void itoa(uint64_t val, char* buf, int radix) {
    int i = digits(val, radix);
    buf[i] = '\0';
    do {
        int digit = (val % radix);
        char base = '0';
        if (digit >= 10) {
            digit -= 10;
            base = 'A';
        }
        buf[--i] = base + digit;
    } while ((val /= radix) > 0);
}