default: run
.PHONY: default run build clean

KERNEL_FILES = boot/multiboot boot/entry
ASMFLAGS = -f elf64
CFLAGS = -ffreestanding
LDFLAGS = -T linker.ld -nmagic
KERNEL_IMAGE = build/kernel.bin
DISK_IMAGE = build/disk.iso
VM_DISK_PATH = vmdisk

OBJECT_FILES = $(addprefix build/,$(addsuffix .o,$(KERNEL_FILES)))

build/%.o: %.asm
	mkdir -p build/$(dir $^)
	nasm $(ASMFLAGS) $^ -o $@

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