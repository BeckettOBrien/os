#include "multiboot.h"
#include "kernel/memory/heap.h"
#include "math/math.h"
#include "drivers/video/vga.h"
#include "string/string.h"

extern const struct mb_info* multiboot_info_addr asm("multiboot_info_struct");

struct mb_tag {
    uint32_t type;
    uint32_t size;
    uint8_t data[0];
};

struct mb_info {
    uint32_t size;
    uint32_t reserved;
    struct mb_tag tags[0];
};

struct mb_memmap {
    uint32_t type;
    uint32_t size;
    uint32_t entry_size;
    uint32_t entry_version;
    struct {
        uint64_t base_addr;
        uint64_t length;
        uint32_t type;
        uint32_t reserved;
    } entries[0];
};

char* boot_args;
char* bootloader_name;

void parse_memory_map(struct mb_memmap* memmap);

void read_multiboot_info(void) {
    struct mb_tag* tags = multiboot_info_addr->tags;
    for (int i = 0; tags[i].type != 0;) {
        switch (tags[i].type) {
            case 1:
                boot_args = tags[i].data;
                break;
            case 2:
                bootloader_name = tags[i].data;
                break;
            case 6:
                parse_memory_map(&tags[i]);
                break;
            case 8:
                // TODO: framebuffer info
                break;
        }


        i += ceil(tags[i].size / 8.0);
        // vga_print("next tag is type ");
        // int t = tags[i].type;
        // char type[5];
        // itoa(t, type, 10);
        // vga_println(type);
    }

}

// void print_boot_info(void) {
//     vga_print("Booted by ");
//     vga_print(bootloader_name);
//     vga_print(" with args: ");
//     vga_println(boot_args);
// }

// void print_memory_region(uint64_t base_addr, uint64_t len) {
//     char start[20];
//     char end[20];
//     itoa(base_addr, start, 16);
//     itoa(base_addr + len, end, 16);
//     vga_print("0x");
//     vga_print(start);
//     vga_print("-0x");
//     vga_println(end);
// }

extern uint64_t KERNEL_END;

void parse_memory_map(struct mb_memmap* memmap) {
    int map_length = memmap->size - (sizeof(struct mb_memmap));
    int num_entries = map_length/memmap->entry_size;
    mem_region available_regions[num_entries];
    mem_region reserved_regions[num_entries];
    uint64_t max_addr = 0;
    int num_available = 0;
    int num_reserved = 0;
    for (int i = 0; i < num_entries; i++) {
        if (memmap->entries[i].type == 1) {
            // print_memory_region(memmap->entries[i].base_addr, memmap->entries[i].length);
            available_regions[num_available].base_addr = memmap->entries[i].base_addr;
            available_regions[num_available].len = memmap->entries[i].length;
            num_available++;
            uint64_t end_addr = memmap->entries[i].base_addr + memmap->entries[i].length;
            max_addr = (end_addr > max_addr) ? end_addr : max_addr;
        } else {
            reserved_regions[num_reserved].base_addr = memmap->entries[i].base_addr;
            reserved_regions[num_reserved].len = memmap->entries[i].length;
            num_reserved++;
        }
        // char addr[20];
        // uint64_t base_addr = memmap->entries[i].base_addr;
        // itoa(base_addr, addr, 16);
        // char len[20];
        // uint64_t length = memmap->entries[i].length;
        // itoa(length, len, 10);
        // char type[5];
        // uint32_t t = memmap->entries[i].type;
        // itoa(t, type, 10);
        // char reserved[20];
        // uint32_t res = memmap->entries[i].reserved;
        // itoa(res, reserved, 10);
        // vga_print(len);
        // vga_print(" of type ");
        // vga_print(type);
        // vga_print(" at 0x");
        // vga_print(addr);
        // vga_print(" with reserved ");
        // vga_println(reserved);
    }

    init_heap(available_regions, 2, &KERNEL_END);
}