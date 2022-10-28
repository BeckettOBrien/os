#pragma once
#include "types.h"

void pic_remap(int offset1, int offset2);
void pic_eoi(uint8_t irq);
void pic_mask(uint8_t irq);
void pic_unmask(uint8_t irq);