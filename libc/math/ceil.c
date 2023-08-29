#include "math.h"

double ceil(double x) {
    uint64_t floor = (uint64_t)x;
    if (x == (double)floor) return x;
    return (double)(floor + 1);
}