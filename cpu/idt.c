#include "idt.h"
#include "drivers/video/vga.h"

#define INT_GATE 0xE
#define TRAP_GATE 0xF
#define DPL(dpl) (dpl << 5)
#define PRESENT (1 << 7)

#define LOW_16(addr) (uint16_t)(addr & 0xFFFF)
#define HIGH_16(addr) (uint16_t)((addr >> 16) & 0xFFFF)
#define LOW_32(addr) (uint32_t)(addr & 0xFFFFFFFF)
#define HIGH_32(addr) (uint32_t)((addr >> 32) & 0xFFFFFFFF)

#define IDT_ENTRIES 256
extern const uint16_t CS asm("gdt.code");
extern const uint64_t isr_transfers[48] asm("isr_transfers");

struct idt_descriptor_t {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

struct idt_entry {
    uint16_t offset_low;
    uint16_t cs;
    uint8_t ist; // Interrupt stack table
    uint8_t attributes; // Gate type, DPL, Present
    uint16_t offset_mid;
    uint32_t offset_high;
    uint32_t zero;
} __attribute__((packed));

struct idt_entry idt[IDT_ENTRIES]; 
struct idt_descriptor_t idt_descriptor;

// TODO: Fix DPL level, un-hardcode TRAP_GATE, and fix ist
void set_idt_entry(int n, uint64_t handler) {
    idt[n].offset_low = LOW_16(handler);
    idt[n].cs = &CS;
    idt[n].ist = 0;
    idt[n].attributes = PRESENT | DPL(0) | TRAP_GATE;
    idt[n].offset_mid = HIGH_16(handler);
    idt[n].offset_high = HIGH_32(handler);
    idt[n].zero = 0;
}

void fill_idt(void) {
    for (int i = 0; i < 48; i++) {
        set_idt_entry(i, isr_transfers[i]);
    }
}

void load_idt(void) {
    idt_descriptor.base = &idt;
    idt_descriptor.limit = IDT_ENTRIES * sizeof(struct idt_entry) - 1;
    asm volatile("lidt (%0)" : : "r" (&idt_descriptor));
}