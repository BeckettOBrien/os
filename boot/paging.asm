; Basic paging setup
[bits 32]

; Flag definitions
%define PT_FLAG_PRESENT (1 << 0)
%define PT_FLAG_WRITABLE (1 << 1)
%define PT_FLAG_PAGESIZE (1 << 7)
%define CR0_WRITE_PROTECT (1 << 16)
%define CR0_PAGING (1 << 31)
%define CR4_FLAG_PAE (1 << 5)
%define EFER_LONG_MODE (1 << 8)
; Other definitions
%define EFER_MSR_ID 0xC0000080

section .bss

; Page tables need to be page-aligned to use bottom address bits for flags
align 4096

; Reserve enough memory to identity map one GiB (using 2MiB pages so no p1 table)
global KERNEL_PAGE_TABLE
KERNEL_PAGE_TABLE:
p4_table:
    resb 4096
p3_table:
    resb 4096
p2_table:
    resb 4096
p1_table:
    resb 4096

section .text

global identity_map_kernel
identity_map_kernel:
    ; Recursively map the last entry of the page directory (p4) table to itself
    mov eax, p4_table
    or eax, (PT_FLAG_PRESENT | PT_FLAG_WRITABLE)
    mov dword [p4_table + 511 * 8], eax

    ; Point first entry of page directory (p4) table to the p3 table
    mov eax, p3_table
    or eax, (PT_FLAG_PRESENT | PT_FLAG_WRITABLE)
    mov dword [p4_table + 0], eax ; Set index 0 in the page directory (p4) table to the address + flags

    ; Point first entry of p3 table to the p2 table
    mov eax, p2_table
    or eax, (PT_FLAG_PRESENT | PT_FLAG_WRITABLE)
    mov dword [p3_table + 0], eax ; Set index 0 in the p3 table to the address + flags

    ; Point first entry of p2 table to the p1 table
    mov eax, p1_table
    or eax, (PT_FLAG_PRESENT | PT_FLAG_WRITABLE)
    mov dword [p2_table + 0], eax ; Set index 0 in the p2 table to the address + flags

    ; Loop to fill the p2 table to point to actual pages
    mov ecx, 0 ; 512x2MiB=1GiB to fill the table
.create_p1_maps_loop:
    ; Calculate the physical address of each mapping
    mov eax, 0x1000 ; 4 KiB | 2MiB
    mul ecx
    or eax, (PT_FLAG_PRESENT | PT_FLAG_WRITABLE) ; 2 MiB pages  | PT_FLAG_PAGESIZE
    mov [p1_table + (ecx * 8)], eax
    ; Loop
    inc ecx
    cmp ecx, 512 ;512
    jne .create_p1_maps_loop
    ; End of loop
    ret

global enable_paging
enable_paging:
    ; Set control register 3 to the p4 table address
    mov eax, p4_table
    mov cr3, eax
    ; Enable PAE
    mov eax, cr4
    or eax, CR4_FLAG_PAE
    mov cr4, eax
    ; Enable long mode
    mov ecx, EFER_MSR_ID
    rdmsr
    or eax, EFER_LONG_MODE
    wrmsr
    ; Enable paging
    mov eax, cr0
    or eax, (CR0_PAGING | CR0_WRITE_PROTECT)
    mov cr0, eax
    ; We are in compatability mode
    ret