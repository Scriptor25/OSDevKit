AS = i686-elf-as
GCC = i686-elf-gcc
QEMU = qemu-system-i386

GCCFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra

SRC = src
BUILD = build

OSNAME = scriptos
KERNEL = $(BUILD)/kernel.bin
ISO = $(BUILD)/$(OSNAME).iso

ASMOBJS = $(patsubst $(SRC)/%.s,$(BUILD)/%.s.o,$(wildcard $(SRC)/*.s))
COBJS = $(patsubst $(SRC)/%.c,$(BUILD)/%.c.o,$(wildcard $(SRC)/*.c))
OBJS = $(ASMOBJS) $(COBJS)

.PHONY: all clean

all: $(ISO)
	$(QEMU) -cdrom $(ISO)

clean:
	-rm -rf $(BUILD)

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/%.s.o: $(SRC)/%.s $(BUILD)
	$(AS) $< -o $@

$(BUILD)/%.c.o: $(SRC)/%.c $(BUILD)
	$(GCC) -c $< -o $@ $(GCCFLAGS)

$(KERNEL): $(SRC)/linker.ld $(OBJS)
	$(GCC) -o $(KERNEL) -ffreestanding -O2 -nostdlib -T $^ -lgcc
	grub-file --is-x86-multiboot2 $(KERNEL)

$(ISO): $(KERNEL) $(SRC)/grub.cfg
	mkdir -p $(BUILD)/iso/boot/grub
	cp $(KERNEL) $(BUILD)/iso/boot/kernel.bin
	cp $(SRC)/grub.cfg $(BUILD)/iso/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO) $(BUILD)/iso
