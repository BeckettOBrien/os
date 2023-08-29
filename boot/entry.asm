; Kernel Entry Point
[bits 32]

; Flag definitions
%define CR0_MCP (1 << 1)
%define CR0_EMULATE (1 << 2)
%define CR4_OSFXSR (1 << 9)
%define CR4_OSXMMEXCPT (1 << 10)

section .bss
global multiboot_info_struct
multiboot_info_struct: resb 4

section .text

extern identity_map_kernel
extern enable_paging
extern load_gdt
extern kernel_bootstrap

global _start
_start:
    ; Store pointer to multiboot information structure
    mov [multiboot_info_struct], ebx
    ; Enable FPU
    mov eax, cr0
    and ax, ~CR0_EMULATE ; Disable emulation
    or ax, CR0_MCP ; Enable coprocessor monitoring
    mov cr0, eax
    mov eax, cr4
    or ax, (CR4_OSFXSR | CR4_OSXMMEXCPT)
    mov cr4, eax
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