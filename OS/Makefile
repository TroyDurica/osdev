CC=i686-elf-gcc
AS=i686-elf-as
NASM=nasm -f elf32
LD=i686-elf-gcc

I=include

CFLAGS=2>TroyOS_build.log -std=gnu99 -ffreestanding -O2 -fno-omit-frame-pointer -Wall -Wextra -c -I $(I)
LDFLAGS=-T link.ld -ffreestanding -O2 -nostdlib -lgcc -Xlinker -Map=TroyOS.map

EXEC_LDFLAGS=-T exec/exec_linker.ld -ffreestanding -O2 -nostdlib

BOOT_DEPS=boot/boot.s
KERNEL_DEPS=src/ordered_array.c src/cli.c src/keyboard.c src/syscall.c src/task.c src/initrd.c src/vfs.c src/stack.c src/kheap.c src/gdt.c src/page.c src/pit.c src/isr.c src/idt.c src/log.c src/string.c src/terminal.c src/kernel.c
ASM_DEPS=src/idt.s
OBJECTS=boot.o kernel.o cli.o keyboard.o task.o syscall.o ordered_array.o initrd.o vfs.o stack.o kheap.o gdt.o page.o pit.o isr.o idt.o log.o string.o terminal.o asmdeps.o

boot:
	$(AS) -o boot.o $(BOOT_DEPS)
	make kernel

kernel:
	$(CC) $(CFLAGS) $(KERNEL_DEPS)
	$(NASM) -o asmdeps.o $(ASM_DEPS)
	make link

link:
	$(LD) -o kernel.elf $(LDFLAGS) $(OBJECTS)
	$(LD) -o kernel.bin $(LDFLAGS) $(OBJECTS)

run_bochs:
	mkdir -p isodir/boot/grub
	./initrd/fs_gen grub/grub.cfg grub.cfg
	cp kernel.elf isodir/boot
	cp initrd.img isodir/boot
	cp grub/grub.cfg isodir/boot/grub
	grub-mkrescue -o TroyOS.iso isodir
	./grub/verify-mboot.sh
	make clean
	bochs -f bochs/bochsrc.txt -q

run:
	mkdir -p isodir/boot/grub
	./initrd/fs_gen grub/grub.cfg grub.cfg
	cp kernel.elf isodir/boot
	cp initrd.img isodir/boot
	cp grub/grub.cfg isodir/boot/grub
	grub-mkrescue -o TroyOS.iso isodir
	./grub/verify-mboot.sh
	make clean
	qemu-system-i386 -m 32M -d int -no-reboot -no-shutdown TroyOS.iso -chardev stdio,id=char0,logfile=TroyOS.log,signal=off -serial chardev:char0

clean:
	rm $(OBJECTS)

.PHONY: boot kernel
