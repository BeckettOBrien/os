; Multiboot Header

%define MAGIC_NUMBER 0xe85250d6
%define ARCHITECTURE 0

section .multiboot_header
multiboot_header:
    dd MAGIC_NUMBER                                 ; magic number
    dd ARCHITECTURE                                 ; architecture
    dd (multiboot_header.end - multiboot_header)    ; length
    dd -(MAGIC_NUMBER + ARCHITECTURE + (multiboot_header.end - multiboot_header)) ; checksum
.tags:
    ; End tag
    dw 0                  ; type
    dw 0                  ; flags
    dd 8                  ; size
.end: