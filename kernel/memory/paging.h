#pragma once
#include "types.h"

#define PRESENT (1 << 0)
#define WRITABLE (1 << 1)
#define PAGESIZE (1 << 7)

void map_page_to_kernel(uint64_t realaddr, uint64_t virtaddr, uint16_t flags);
void unmap_page_kernel(uint64_t virtaddr);