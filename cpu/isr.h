#pragma once
#include "types.h"

#define IRQ_IDX(irq) (32 + irq)

typedef struct {
    uint64_t r11, r10, r9, r8, rdx, rcx, rax;
    uint8_t id, errno;
    uint64_t ip, cs, flags, sp, ss; // CPU pushed values
} __attribute__((packed)) interrupt_state;

typedef void (*isr_t)(interrupt_state);

void register_handler(int id, isr_t handler);