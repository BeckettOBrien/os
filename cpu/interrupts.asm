; Assembly Interrupt Handlers
[bits 64]

%macro SAVE_STATE 0
    push rax
    push rcx
    push rdx
    push r8
    push r9
    push r10
    push r11
%endmacro

%macro RESTORE_STATE 0
    pop r11
    pop r10
    pop r9
    pop r8
    pop rdx
    pop rcx
    pop rax
%endmacro

section .rodata

global isr_transfers
isr_transfers:
%assign i 0
%rep 32
dq isr %+ i
%assign i i+1
%endrep
%assign i 0
%rep 16
dq irq %+ i
%assign i i+1
%endrep

section .text

extern isr_handle
extern irq_handle

isr_transfer:
    SAVE_STATE
    call isr_handle
    RESTORE_STATE
    add rsp, 16
    iretq

irq_transfer:
    SAVE_STATE
    call irq_handle
    RESTORE_STATE
    add rsp, 16
    iretq

%assign i 0
%rep 32
isr %+ i:
    %if !((i == 8) | (i == 10) | (i == 11) | (i == 12) | (i == 13) | (i == 14))
        push byte 0
    %endif
    push byte i
    jmp isr_transfer
%assign i i+1
%endrep

%assign i 0
%rep 16
irq %+ i:
    push byte i
    push byte (32 + i)
    jmp irq_transfer
%assign i i+1
%endrep