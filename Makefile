default: run
.PHONY: default run build clean list

KERNEL_FILES = boot/multiboot boot/entry boot/paging boot/gdt $(basename $(shell find . -type f -name '*.c' | sed 's|^\./||'))
HEADER_FILES = $(shell find . -type f -name '*.h' | sed 's|^\./||')
ASMFLAGS = -f elf64
CFLAGS = -ffreestanding -Iinclude -I.
LDFLAGS = -T linker.ld -nmagic
KERNEL_IMAGE = build/kernel.bin
DISK_IMAGE = build/disk.iso
VM_DISK_PATH = vmdisk

OBJECT_FILES = $(addprefix build/,$(addsuffix .o,$(KERNEL_FILES)))

build/%.o: %.asm
	mkdir -p build/$(dir $^)
	nasm $(ASMFLAGS) $^ -o $@

build/%.o: %.c $(HEADER_FILES)
	mkdir -p build/$(dir $^)
	gcc $(CFLAGS) -c $< -o $@

$(KERNEL_IMAGE): linker.ld $(OBJECT_FILES)
	ld $(LDFLAGS) -o $@ $(OBJECT_FILES)

$(DISK_IMAGE): $(KERNEL_IMAGE)
	cp -r $(VM_DISK_PATH) build/
	cp $^ build/$(VM_DISK_PATH)/boot/
	grub-mkrescue -o $@ build/$(VM_DISK_PATH)

build: $(DISK_IMAGE)

run: $(DISK_IMAGE)
	qemu-system-x86_64 -cdrom $(DISK_IMAGE)

clean:
	rm -rf build

list:
	@echo $(KERNEL_FILES)
	@echo $(HEADER_FILES)