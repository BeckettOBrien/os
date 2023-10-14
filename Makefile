default: run
.PHONY: default run debug build clean
MAKEFLAGS := --jobs=$(shell nproc)

KERNEL_FILES := $(basename $(shell find . -type f -name '*.asm' | sed 's|^\./||')) $(basename $(shell find . -type f -name '*.c' | sed 's|^\./||'))
ASMFLAGS = -f elf64
CFLAGS = -ffreestanding -Iinclude -Ilibc -I. -g -Wall -Wextra -MMD -MP
LDFLAGS = -T linker.ld -nmagic
QEMUFLAGS = -monitor stdio -m 128M
KERNEL_IMAGE = build/kernel.bin
DISK_IMAGE = build/disk.iso
VM_DISK_PATH = vmdisk

OBJECTS := $(addprefix build/,$(addsuffix .o,$(KERNEL_FILES)))
DEPENDS := $(addprefix build/,$(addsuffix .d,$(KERNEL_FILES)))

build/%.o: %.asm Makefile
	@mkdir -p build/$(dir $<)
	nasm $(ASMFLAGS) $< -o $@

-include $(DEPENDS)

build/%.o: %.c Makefile
	@mkdir -p build/$(dir $<)
	gcc $(CFLAGS) -c $< -o $@

$(KERNEL_IMAGE): linker.ld $(OBJECTS)
	ld $(LDFLAGS) -o $@ $(OBJECTS)

$(DISK_IMAGE): $(KERNEL_IMAGE)
	cp -r $(VM_DISK_PATH) build/
	cp $^ build/$(VM_DISK_PATH)/boot/
	grub-mkrescue -o $@ build/$(VM_DISK_PATH)

build: $(DISK_IMAGE)

run: $(DISK_IMAGE)
	qemu-system-x86_64 -cdrom $(DISK_IMAGE) $(QEMUFLAGS)

debug: $(DISK_IMAGE) $(KERNEL_IMAGE)
	qemu-system-x86_64 -cdrom $(DISK_IMAGE) $(QEMUFLAGS) -s -S

gdb: $(KERNEL_IMAGE)
	gdb $(KERNEL_IMAGE) -ex 'target remote localhost:1234' -ex 'b kernel_bootstrap'

bochs: $(DISK_IMAGE)
	bochs

clean:
	rm -rf build