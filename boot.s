.set MAGIC, 0x1BADB002
.set FLAGS, (1<<0 | 1<<1 | 1<<2)
.set CHECKSUM, -(MAGIC + FLAGS)

/* This tells the emulator to turn on the 320x200 graphics screen */
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM
.long 0, 0, 0, 0, 0
.long 0
.long 320, 200, 8

/* This sets up a safe memory space and calls your C main() function */
.section .text
.global _start
_start:
    mov $stack_top, %esp
    call main
    cli
1:  hlt
    jmp 1b

.section .bss
.align 16
stack_bottom:
.skip 16384
stack_top:
