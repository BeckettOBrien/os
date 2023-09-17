#include "string.h"
#include "types.h"
#include "kernel/memory/kmalloc.h"

int vsnprintf(char* str, int n, const char* fmt, va_list args) {
    int len = 0;
    char ch;
    while (ch = *fmt++) {
        char temp_chars[65];
        char* temp_str = NULL;
        if (ch == 0) {
            str[len] = 0;
            return len;
        }
        if (ch == '%') {
            switch (ch = *fmt++) {
                case '%':
                    temp_chars[0] = '%';
                    temp_chars[1] = 0;
                    temp_str = temp_chars;
                    break;
                case 'c':
                    temp_chars[0] = (char)va_arg(args, int);
                    temp_chars[1] = 0;
                    temp_str = temp_chars;
                    break;
                case 'd':
                case 'i':
                    int temp_int = va_arg(args, int);
                    if (temp_int < 0) {
                        temp_chars[0] = '-';
                        itoa(temp_int * -1, temp_chars + 1, 10);
                    } else {
                        itoa(temp_int, temp_chars, 10);
                    }
                    temp_str = temp_chars;
                    break;
                case 's':
                    temp_str = va_arg(args, char*);
                    break;
                case 'u':
                    itoa(va_arg(args, unsigned int), temp_chars, 10);
                    temp_str = temp_chars;
                    break;
                case 'x':
                    itoa(va_arg(args, unsigned int), temp_chars, 16);
                    temp_str = temp_chars;
                    break;
                case 'p':
                    itoa(va_arg(args, uint64_t), temp_chars, 16);
                    temp_str = temp_chars;
                    break;
            }
            if (temp_str != NULL) {
                int temp_len = 0;
                while ((temp_str[temp_len] != 0)) {
                    if (len < n) str[len] = temp_str[temp_len];
                    str[++len] = 0;
                    temp_len++;
                }
            }
            // TODO: Add more format specifiers and modifiers
        } else {
            if (len < n) str[len] = ch;
            str[++len] = 0;
        }
    }
    str[len] = 0;
    return len;
}

int snprintf(char* str, int n, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    return vsnprintf(str, n, fmt, args);
}

int vsprintf(char* str, const char* fmt, va_list args) {
    return vsnprintf(str, -1, fmt, args);
}

int sprintf(char* str, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    return vsnprintf(str, -1, fmt, args);
}

int vasprintf(char** str, const char* fmt, va_list args) {
    va_list argsDup;
    va_copy(argsDup, args);
    int len = vsnprintf(NULL, 0, fmt, args);
    char* buf = kmalloc(len+1);
    if (buf == NULL) return -1;
    *str = buf;
    return vsnprintf(buf, len, fmt, argsDup);
}

int asprintf(char** str, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    return vasprintf(str, fmt, args);
}