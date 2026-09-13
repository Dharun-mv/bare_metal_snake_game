# Bare-Metal Snake OS

A freestanding 32-bit Operating System written entirely in C and Assembly that directly interfaces with x86 hardware to run a classic Snake game. There is no underlying OS, no standard C library (`<stdlib.h>`), and no high-level graphics API. 

## Technical Architecture

* **Graphics:** VGA Mode 13h (320x200 resolution, 256 colors) via direct memory-mapped I/O at `0xA0000`.
* **Input:** Raw keyboard scancode polling targeting motherboard I/O Port `0x60` using inline AT&T Assembly.
* **Game Loop:** Custom busy-wait active polling loop calibrated to modern CPU clock speeds, ensuring responsive non-blocking WASD movement.
* **Rendering:** Delta-based block erasing using coordinate history arrays to prevent 60Hz VSync screen tearing.
* **Bootloader:** GRUB2 Multiboot-compliant assembly header (`boot.s`).

## Prerequisites

To build and run this engine, you need a Linux environment (Fedora/Ubuntu) with the following packages:
\`\`\`bash
sudo dnf install qemu-system-x86 gcc binutils grub2-tools xorriso
\`\`\`

## Build and Boot Instructions

This project compiles as a freestanding binary and packages into a bootable ISO.

1. **Assemble the bootloader:**
\`\`\`bash
as --32 boot.s -o boot.o
\`\`\`

2. **Compile the freestanding C kernel:**
\`\`\`bash
gcc -m32 -ffreestanding -fno-pic -c kernel.c -o kernel.o
\`\`\`

3. **Link into a raw bootable binary:**
\`\`\`bash
ld -m elf_i386 -T linker.ld -nostdlib boot.o kernel.o -o snake.bin
\`\`\`

4. **Generate the bootable ISO:**
\`\`\`bash
mkdir -p isodir/boot/grub
cp snake.bin isodir/boot/
cat <<EOF > isodir/boot/grub/grub.cfg
menuentry "Bare Metal Snake" {
    multiboot /boot/snake.bin
}
EOF
grub2-mkrescue -o snake.iso isodir
\`\`\`

5. **Run in QEMU Emulator:**
\`\`\`bash
qemu-system-i386 -cdrom snake.iso
\`\`\`
*(Note: Click inside the QEMU window to capture keyboard input).*

## Contact
Created by Dharun M.V. 
For inquiries regarding full-stack web applications or game server development, reach out via Gmail.
