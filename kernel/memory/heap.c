#include "heap.h"
#include "memory/memory.h"
#include "math/math.h"
#include "drivers/video/vga.h"
#include "kernel/panic.h"
#include "string/string.h"

#define BYTES_TO_PAGES(x) (x >> 12)
#define PAGES_TO_BYTES(x) (x << 12)
#define ALIGN_PAGE_UPPER(x) ((uint64_t)ceil(x/(float)0x1000) << 12)
#define ALIGN_PAGE_LOWER(x) (x & ~0xFFF)

#define MAX_HEAP_MEMORY 0x200 // Pages (MiB)

// TODO: make this dynamic
uint8_t free_memory[MAX_HEAP_MEMORY/8];
uint64_t heap_start;

void init_heap(mem_region* memmap, int map_entries, uint64_t kernel_end) {
    find_heap_start(memmap, map_entries, kernel_end);
    fill_bitmap(memmap, map_entries, kernel_end);
    // TODO: Set up dynamic heap allocator
}

void find_heap_start(mem_region* memmap, int map_entries, uint64_t kernel_end) { //TODO: fix this entire approach
    for (int i = 0; i < map_entries; i++) {
        if ((memmap[i].base_addr + memmap[i].len) > kernel_end) {
            heap_start = ALIGN_PAGE_UPPER(max(memmap[i].base_addr, kernel_end));
            return;
        }
    }
    vga_println("Couldn't find any suitable memory after kernel end.");
    panic();
}

void fill_bitmap(mem_region* memmap, int map_entries, uint64_t kernel_end) {
    memset(free_memory, 0, MAX_HEAP_MEMORY/8);
    for (int i = 0; i < map_entries; i++) {
        if ((memmap[i].base_addr + memmap[i].len) < heap_start) continue;
        uint64_t base = ALIGN_PAGE_UPPER(max(memmap[i].base_addr, heap_start));
        uint64_t offset = BYTES_TO_PAGES(base - heap_start);
        for (int j = 0; j < BYTES_TO_PAGES(memmap[i].len); j++) {
            if ((base + PAGES_TO_BYTES(j)) < kernel_end) continue;
            if ((offset + j) >= MAX_HEAP_MEMORY) return;
            free_memory[(offset + j)/8] |= (1 << (j%8));
        }
    }
}

void print_heap_info(void) {
    char start[20];
    itoa(heap_start, start, 16);
    vga_print("Heap starts at 0x");
    vga_println(start);
    vga_print("Bitmap: ");
    for (int i = 0; i < MAX_HEAP_MEMORY/8; i++) {
        for (int j = 0; j < 8; j++) {
            char buf[5];
            itoa((free_memory[i] >> j) & 1, buf, 10);
            vga_print(buf);
        }
    }
    vga_println("");
}

// Allocates an unused page and returns its physical address
uint64_t allocate_page(void) {
    int idx = 0;
    for (; idx < MAX_HEAP_MEMORY; idx++) {
        if ((free_memory[idx/8] >> (idx%8)) & 1) break;
    }
    free_memory[idx/8] &= ~(1 << (idx%8));
    return heap_start + PAGES_TO_BYTES(idx);
}

void free_page(uint64_t addr) {
    int offset = BYTES_TO_PAGES(addr - heap_start);
    free_memory[offset/8] |= (1 << (offset%8));
}