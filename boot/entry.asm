; Kernel Entry Point
[bits 32]

section .text

extern identity_map_kernel
extern enable_paging
extern load_gdt
extern kernel_bootstrap

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
    call kernel_bootstrap
    ; Halt if we ever return
    cli
    hlt