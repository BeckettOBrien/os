# os

---

This project is my attempt at making my own (very simple) operating system from scratch. I just wanted to try making one to force myself to better understand how actual modern operating systems work, so I doubt it will ever become featured or powerful enough to be useful to anyone, but feel free to read through the code or ask any questions about it if you're interested in learning more yourself.

---

### Current Features and To-Do List
- [X] Booting from a multiboot-compliant bootloader
- [X] Primitive VGA and PS/2 keyboard drivers
- [X] Hardware and software interrupt handlers
- [X] Allocating and mapping pages into kernel virtual memory
- [X] Dynamic heap allocator
- [ ] TTYs with buffered input/output
- [ ] Clock and system time
- [ ] Simple shell interface
- [ ] Basic filesystem
- [ ] Loading programs (ring 0)
- [ ] Better graphics driver
- [ ] Multitaksing

---

### Makefile Commands
- `run`: Runs the operating system through QEMU (x86_64)
- `build`: Builds the project without running. The linked kernel binary is placed at `build/kernel.bin` and a disk image with grub and the os installed is placed at `build/disk.iso`
- `debug`: Runs qemu with a gdb server listening on port `1234`
- `gdb`: Runs gdb, connects to qemu's server, and sets a breakpoint at `kernel_bootstrap`
- `bochs`: Runs the operating system through the [Bochs](https://bochs.sourceforge.io/) emulator for more debugging information
- `clean`: Deletes the `build/` folder and all it's contents

---

Thank's for reading this and I hope this repo has been able to help you in some way! Feel free to email me or create an issue is you have any questions and I hope I can answer them. Have a great day!