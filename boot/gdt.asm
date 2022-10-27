; Basic GDT setup
[bits 32]

; Flag definitions
%define GDT_FLAG_RW (1 << 41)
%define GDT_FLAG_EX (1 << 43)
%define GDT_FLAG_DT (1 << 44)
%define GDT_FLAG_PRESENT (1 << 47)
%define GDT_FLAG_64BIT (1 << 53)
; Combination flag definitions
%define GDT_CS_FLAGS (GDT_FLAG_RW | GDT_FLAG_EX | GDT_FLAG_DT | GDT_FLAG_PRESENT | GDT_FLAG_64BIT)
%define GDT_DS_FLAGS (GDT_FLAG_RW | GDT_FLAG_DT | GDT_FLAG_PRESENT)

section .rodata
global gdt.code
gdt:
    dq 0                        ; Zero entry
.code: equ $ - gdt              ; Define label by offset
    dq GDT_CS_FLAGS             ; Code segment flags
.data: equ $ - gdt              ; Define label by offset
    dq GDT_DS_FLAGS             ; Data segment flags
.descriptor:                    ; Structure to load GDT with
    dw .descriptor - gdt - 1    ; 1 less than length to prevent overflow
    dq gdt                      ; Point to GDT

section .text

extern kernel_far_entry
global load_gdt
load_gdt:
    ; Load the actual gdt
    lgdt [gdt.descriptor]
    ; Load segment registers with dummy gdt data address
    mov ax, gdt.data ; Load 16-bit a-register with gdt data address
    ; Copy data address to each segment register
    mov ss, ax ; Stack segment
    mov ds, ax ; Data segment
    mov es, ax ; Extra segment

    ; Far jump to long mode kernel
    jmp gdt.code:kernel_far_entry