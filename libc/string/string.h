#pragma once
#include "types.h"
#include <stdarg.h>

int itoa(uint64_t val, char* buf, int radix);
int vsnprintf(char* str, int n, const char* fmt, va_list args);
int snprintf(char* str, int n, const char* fmt, ...);
int vsprintf(char* str, const char* fmt, va_list args);
int sprintf(char* str, const char* fmt, ...);
int vasprintf(char** str, const char* fmt, va_list args);
int asprintf(char** str, const char* fmt, ...);