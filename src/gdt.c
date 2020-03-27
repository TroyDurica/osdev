#include "gdt.h"

extern void gdt_flush(uint32_t);
extern void tss_flush();

void init_gdt();
static void write_tss(sint32_t num, uint16_t ss0, uint32_t esp0);
static void gdt_set_gate(sint32_t,uint32_t,uint32_t,uint8_t,uint8_t);

gdt_entry_t gdt_entries[6];
gdt_ptr_t   gdt_ptr;

tss_entry_t tss_entry;

void init_gdt()
{
	gdt_ptr.limit = (sizeof(gdt_entry_t) * 6) - 1;
	gdt_ptr.base  = (uint32_t)&gdt_entries;

	gdt_set_gate(0, 0, 0, 0, 0);                // Null segment
	gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Kernel Code segment
	gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Kernel Data segment
	gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment
	gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment
	write_tss(5, 0x10, 0x0);                    // TSS

   gdt_flush((uint32_t)&gdt_ptr);
   tss_flush();
}

static void write_tss(sint32_t num, uint16_t ss0, uint32_t esp0)
{
	// Firstly, let's compute the base and limit of our entry into the GDT.
	uint32_t base = (uint32_t) &tss_entry;
	uint32_t limit = base + sizeof(tss_entry);

	// Now, add our TSS descriptor's address to the GDT.
	gdt_set_gate(num, base, limit, 0xE9, 0x00);

	// Ensure the descriptor is initially zero.
	memset(&tss_entry, 0, sizeof(tss_entry));

	tss_entry.ss0  = ss0;  // Set the kernel stack segment.
	tss_entry.esp0 = esp0; // Set the kernel stack pointer.

	tss_entry.cs   = 0x0b;
	tss_entry.ss = tss_entry.ds = tss_entry.es = tss_entry.fs = tss_entry.gs = 0x13;
}

// Set the value of one GDT entry.
static void gdt_set_gate(sint32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
	gdt_entries[num].base_low    = (base & 0xFFFF);
	gdt_entries[num].base_middle = (base >> 16) & 0xFF;
	gdt_entries[num].base_high   = (base >> 24) & 0xFF;

	gdt_entries[num].limit_low   = (limit & 0xFFFF);
	gdt_entries[num].granularity = (limit >> 16) & 0x0F;

	gdt_entries[num].granularity |= gran & 0xF0;
	gdt_entries[num].access      = access;
}

void set_kernel_stack(uint32_t stack)
{
   	tss_entry.esp0 = stack;
}
