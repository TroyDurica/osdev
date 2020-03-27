#include "terminal.h"
#include "log.h"
#include "string.h"
#include "serial.h"
#include "idt.h"
#include "pit.h"
#include "isr.h"
#include "page.h"
#include "common.h"
#include "gdt.h"
#include "kheap.h"
#include "task.h"
#include "stack.h"
#include "common.h"
#include "multiboot.h"
#include "vfs.h"
#include "initrd.h"
#include "syscall.h"
#include "keyboard.h"
#include "cli.h"
#include "common.h"

#ifdef __linux__
#error "Can't compiler with Linux compiler, use cross-compiler instead"
#endif

#ifndef __i386__
#error "Compiler target must be ix86"
#endif

uint32_t initial_esp;

void kernel_main(multiboot_header_t *mboot_ptr, uint32_t esp) 
{
	initial_esp = esp;
	terminal_initialize();

	init_gdt();
	init_idt(0xFC, 0xFF);
	isr_init();

	ASSERT(mboot_ptr->mods_count == 1);
	uint32_t initrd_location = *(uint32_t*)(mboot_ptr->mods_addr);
	uint32_t initrd_end = *(uint32_t*)(mboot_ptr->mods_addr+4);

	fs_root = initialise_initrd(initrd_location, initrd_end);

	initialise_paging();
	init_pit(1000);
	initialise_tasking();
	initialise_syscalls();

	for (int i = 0; i < 80; i++) printk("=");
	printk("\n");

	list_vfs();

	//initialise_cli();
	initialise_keyboard();

	uint32_t parentpid = getpid();

	switch_to_user_mode();

	syscall_fork();

	if (getpid() == parentpid) printf("Parent\n");
	else printf("Child\n");

	while(1);
}