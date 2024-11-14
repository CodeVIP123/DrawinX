nasm -f elf32 -o boot.o loader/boot.asm
gcc -ffreestanding -m32 -c src/kernel.c -o kernel.o
ld -m elf_i386 -Ttext 0x1000 -T linker/linker.ld -o kernel.bin boot.o kernel.o
mv kernel.bin iso/boot/kernel.bin
grub-mkrescue -o DrawinX.iso iso
qemu-system-i386 -hda DrawinX.iso



