.PHONY: all

all: build/os.iso
	qemu-system-i386 -kernel build/os.bin

build/boot.o: src/boot.s build
	i686-elf-as src/boot.s -o build/boot.o

build/main.o: src/main.c build
	i686-elf-gcc -c src/main.c -o build/main.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

build/os.bin: src/linker.ld build/boot.o build/main.o
	i686-elf-gcc -T src/linker.ld -o build/os.bin -ffreestanding -O2 -nostdlib build/boot.o build/main.o -lgcc
	grub-file --is-x86-multiboot build/os.bin

build/os.iso: build/os.bin src/grub.cfg
	mkdir -p build/iso/boot/grub
	cp build/os.bin build/iso/boot/os.bin
	cp src/grub.cfg build/iso/boot/grub/grub.cfg
	grub-mkrescue -o build/os.iso build/iso

build:
	mkdir build
