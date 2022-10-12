; Kernel Entry Point
[bits 32]

section .text

extern identity_map_kernel
extern enable_paging
extern load_gdt

global _start
_start:
    ; Set up page table for identity mapping
    call identity_map_kernel
    call enable_paging
    ; Load GDT
    jmp load_gdt

[bits 64]
global kernel_far_entry
kernel_far_entry:
    ; Hello world
    mov word [0xb8000], 0x0f48 ; H
    mov word [0xb8002], 0x0f65 ; e
    mov word [0xb8004], 0x0f6c ; l
    mov word [0xb8006], 0x0f6c ; l
    mov word [0xb8008], 0x0f6f ; o
    mov word [0xb800a], 0x0f2c ; ,
    mov word [0xb800c], 0x0f20 ;
    mov word [0xb800e], 0x0f77 ; w
    mov word [0xb8010], 0x0f6f ; o
    mov word [0xb8012], 0x0f72 ; r
    mov word [0xb8014], 0x0f6c ; l
    mov word [0xb8016], 0x0f64 ; d
    mov word [0xb8018], 0x0f21 ; !
    ; Halt
    cli
    hlt