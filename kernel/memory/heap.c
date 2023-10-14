#include "heap.h"
#include "bitmap.h"
#include "paging.h"
#include "kmalloc.h"
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
uint8_t free_memory[BITMAP_SIZE(MAX_HEAP_MEMORY)];
uint64_t heap_start;
uint64_t current_mapped_heap_length;

void init_heap(mem_region* memmap, int map_entries, uint64_t kernel_end) {
    find_heap_start(memmap, map_entries, kernel_end);
    fill_bitmap(memmap, map_entries, kernel_end);
    current_mapped_heap_length = 0;
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
            BITMAP_SET(free_memory, offset + j);
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
        if (BITMAP_GET(free_memory, idx)) break;
    }
    if (!BITMAP_GET(free_memory, idx)) {
        //TODO: Error propogation
        vga_println("ERR: No more physical heap memory!");
        panic();
    }
    BITMAP_CLEAR(free_memory, idx);
    return heap_start + PAGES_TO_BYTES(idx);
}

void free_page(uint64_t addr) {
    int idx = BYTES_TO_PAGES(addr - heap_start);
    if (BITMAP_GET(free_memory, idx)) {
        vga_println("WARNING: double free detected!");
    }
    BITMAP_SET(free_memory, idx);
}

// Maps an additional page as part of the kernel heap
// Returns the current size of the heap in bytes
uint64_t extend_heap(void) {
    uint64_t new_page = allocate_page();
    map_page_to_kernel(new_page, HEAP_VIRTADDR + PAGES_TO_BYTES(current_mapped_heap_length++),  WRITABLE);
    return PAGES_TO_BYTES(current_mapped_heap_length);
}

// Removes and frees the highest page of the heap
void shrink_heap(void) {
    if (current_mapped_heap_length < 1) {
        vga_println("ERR: cannot shrink heap with length zero");
        return;
    }
    unmap_page_kernel(HEAP_VIRTADDR + PAGES_TO_BYTES(--current_mapped_heap_length));
    free_page(current_mapped_heap_length);
}