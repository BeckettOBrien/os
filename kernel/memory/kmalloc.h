#pragma once
#include "types.h"

int kmalloc_init(void);
void* kmalloc(uint64_t bytes);
void kfree(void* ptr);
// void heap_list();