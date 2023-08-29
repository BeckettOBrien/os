#include "paging.h"
#include "heap.h"
#include "../boot/multiboot.h"
#include "string/string.h"
#include "drivers/video/vga.h"

#define PAGE_SIZE 0x1000
#define ALIGN_PAGE_LOWER(x) (x & ~0xFFF)
#define PAGE_TABLE_LIMIT (512 * PAGE_SIZE)

static inline void flush_tlb(uint64_t addr) {
    asm volatile("invlpg (%0)" : : "r" (addr) : "memory");
}

uint64_t create_table(void) {
    uint64_t empty_page = allocate_page();
    return empty_page;
}

// Returns the virtual address of the p1 table for the given address
// Creates any necessary non-present tables
uint64_t* get_p1_table(uint64_t virtaddr) {
    uint64_t sign = (uint64_t)0xFFFF << 48;
    uint64_t r = 0x1FF; // Recursive index
    int p4_idx = (virtaddr >> 39) & 0x1FF;
    uint64_t *p4_table = sign | (r << 39) | (r << 30) | (r << 21) | (r << 12);
    if (!(p4_table[p4_idx] & 1)) {
        p4_table[p4_idx] = create_table() | WRITABLE | PRESENT;
    }
    uint64_t *p3_table = sign | (r << 39) | (r << 30) | (r << 21) | (p4_idx << 12);
    int p3_idx = (virtaddr >> 30) & 0x1FF;
    if (!(p3_table[p3_idx] & 1)) {
        p3_table[p3_idx] = create_table() | WRITABLE | PRESENT;
    }
    uint64_t *p2_table = sign | (r << 39) | (r << 30) | (p4_idx << 21) | (p3_idx << 12);
    int p2_idx = (virtaddr >> 21) & 0x1FF;
    if (!(p2_table[p2_idx] & 1)) {
        p2_table[p2_idx] = create_table() | WRITABLE | PRESENT;
    }
    uint64_t *p1_table = sign | (r << 39) | (p4_idx << 30) | (p3_idx << 21) | (p2_idx << 12);
    return p1_table;
}

void map_page_to_kernel(uint64_t realaddr, uint64_t virtaddr, uint16_t flags) {
    uint64_t *p1_table = get_p1_table(virtaddr);
    uint32_t p1_idx = (virtaddr >> 12) & 0x1FF;
    uint64_t entry = ALIGN_PAGE_LOWER(realaddr) | (flags & 0xFFF);
    p1_table[p1_idx] = entry | PRESENT;
    flush_tlb(virtaddr); // TODO: More efficient cache flushing
}

void unmap_page_kernel(uint64_t virtaddr) {
    uint64_t *p1_table = get_p1_table(virtaddr);
    uint32_t p1_idx = (virtaddr >> 12) & 0x1FF;
    p1_table[p1_idx] = 0;
    flush_tlb(virtaddr); // TODO: More efficient cache flushing
}
