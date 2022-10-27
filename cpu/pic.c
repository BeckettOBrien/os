// Programmable Interrupt Controller Interface
#include "pic.h"
#include "drivers/io/ports.h"

// PIC IO Port Addresses
#define PIC1_BASE 0x20
#define PIC1_CMD  PIC1_BASE
#define PIC1_DATA (PIC1_BASE+1)
#define PIC2_BASE 0xA0
#define PIC2_CMD  PIC2_BASE
#define PIC2_DATA (PIC2_BASE+1)

// PIC Commands
#define PIC_EOI     0x20
#define PIC_INIT    0x10
#define PIC_CASCADE 0x01
#define PIC_MASTER  0x04
#define PIC_SLAVE   0x02
#define PIC_8086    0x01

void pic_remap(int offset1, int offset2) {
    // Initialize both PICs in cascade mode
    outb(PIC1_CMD, (PIC_INIT | PIC_CASCADE));
    outb(PIC2_CMD, (PIC_INIT | PIC_CASCADE));
    // Send new offsets
    outb(PIC1_DATA, offset1);
    outb(PIC2_DATA, offset2);
    // Configure cascade for both PICs
    outb(PIC1_DATA, PIC_MASTER);
    outb(PIC2_DATA, PIC_SLAVE);
    // Tell both PICs to run in 8086 mode
    outb(PIC1_DATA, PIC_8086);
    outb(PIC2_DATA, PIC_8086);
    // Mask both PICs
    outb(PIC1_DATA, -1);
    outb(PIC2_DATA, -1);
}

// Acknowledge last interrupt
void pic_eoi(uint8_t irq) {
    if (irq > 7) outb(PIC2_CMD, PIC_EOI); // Slave PIC (irq > 7) needs to inform both PICs
    outb(PIC1_CMD, PIC_EOI);
}