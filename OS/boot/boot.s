.set ALIGN,    1<<0
.set MEMINFO,  1<<1
.set FLAGS,    ALIGN | MEMINFO
.set MAGIC,    0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.section .bss
.align 4
stack_bottom:
.skip 16384
stack_top:

.section .data
gdt_start:
gdt_null:
.quad 0x0

gdt_kcode:
.word 0xFFFF
.word 0x0
.byte 0x0
.byte 0x9A
.byte 0b11001111
.byte 0x0

gdt_kdata:
.word 0xFFFF
.word 0x0
.byte 0x0
.byte 0x92
.byte 0b11001111
.byte 0x0
gdt_end:

gdtr:
.word (gdt_end - gdt_start - 1)
.long gdt_start

.section .text

.global loadPageDirectory
loadPageDirectory:
push %ebp
mov %esp, %ebp
mov 8(%esp), %eax
mov %eax, %cr3
mov %ebp, %esp
pop %ebp
ret

.global enablePaging
enablePaging:
push %ebp
mov %esp, %ebp
mov %cr0, %eax
or $0x80000000, %eax
mov %eax, %cr0
mov %ebp, %esp
pop %ebp
ret

.extern log
.global _start
.type _start, @function
_start:
	mov $stack_top, %esp

flush_gdt:
   	cli
   	lgdt (gdtr)
   	movw $0x10, %ax
   	movw %ax, %ds
   	movw %ax, %es
   	movw %ax, %fs
   	movw %ax, %gs
   	movw %ax, %ss
   	ljmp $0x08,$flush_end
flush_end:

boot_kernel:
	mov $stack_top, %esp
	
	push %esp
    push %ebx

	call kernel_main

	cli
end_loop:
	hlt
	jmp end_loop

.size _start, . - _start
