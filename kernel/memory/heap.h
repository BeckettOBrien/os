#pragma once
#include "types.h"

#define HEAP_VIRTADDR 0xffffff0000000000

typedef struct {
    uint64_t base_addr;
    uint64_t len;
} mem_region;

void init_heap(mem_region* memmap, int map_entries, uint64_t kernel_end);
uint64_t allocate_page(void);
void free_page(uint64_t addr);
uint64_t extend_heap(void);
void shrink_heap(void);