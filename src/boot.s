.set MAGIC,         0xE85250D6
.set ARCHITECTURE,  0x00000000
.set HEADER_LENGTH, multiboot_header_end - multiboot_header
.set CHECKSUM,      -(MAGIC + ARCHITECTURE + HEADER_LENGTH)

.section .multiboot
.align 8

multiboot_header:
.long MAGIC
.long ARCHITECTURE
.long HEADER_LENGTH
.long CHECKSUM

mutliboot_end_tag:
.word 0
.word 0
.long mutliboot_end_tag_end - mutliboot_end_tag
mutliboot_end_tag_end:
multiboot_header_end:

.section .bss
.align 16
stack_bottom:
.skip 0x4000
stack_top:

.section .text
.global _start
.type _start, @function
_start:
        mov $stack_top, %esp
        call main
        cli
.loop:  hlt
        jmp .loop

.size _start, . - _start
