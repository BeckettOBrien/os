ENTRY(_start)

SECTIONS {
    . = 1M;

    .boot :
    {
        *(.multiboot_header)
    }

    .text :
    {
        *(.text)
    }

    .rodata :
    {
        *(.rodata)
    }

    .bss :
    {
        *(.bss)
    }

    KERNEL_END = ALIGN(2M);
}