void kernel_bootstrap(void) {
    short* VGA_MEM = ((short*)0xb8000);
    VGA_MEM[0] = 0x0f48; // H
    VGA_MEM[1] = 0x0f65; // e
    VGA_MEM[2] = 0x0f6c; // l
    VGA_MEM[3] = 0x0f6c; // l
    VGA_MEM[4] = 0x0f6f; // o
    VGA_MEM[5] = 0x0f2c; // ,
    VGA_MEM[6] = 0x0f20; // ' '
    VGA_MEM[7] = 0x0f77; // w
    VGA_MEM[8] = 0x0f6f; // o
    VGA_MEM[9] = 0x0f72; // r
    VGA_MEM[10] = 0x0f6c; // l
    VGA_MEM[11] = 0x0f64; // d
    VGA_MEM[12] = 0x0f21; // !
}