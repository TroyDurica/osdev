#ifndef PAGE_H
#define PAGE_H 0

#include "stdint.h"
#include "isr.h"

typedef struct page
{
	uint32_t present    : 1;   // Page present in memory
	uint32_t rw         : 1;   // Read-only if clear, readwrite if set
	uint32_t user       : 1;   // Supervisor level only if clear
	uint32_t accessed   : 1;   // Has the page been accessed since last refresh?
	uint32_t dirty      : 1;   // Has the page been written to since last refresh?
	uint32_t unused     : 7;   // Amalgamation of unused and reserved bits
	uint32_t frame      : 20;  // Frame address (shifted right 12 bits)
} page_t;

typedef struct page_table
{
	page_t pages[1024];
} page_table_t;

typedef struct page_directory
{
	page_table_t *tables[1024];
	uint32_t tablesPhysical[1024];
	uint32_t physicalAddr;
} page_directory_t;

#ifdef __cplusplus
extern "C"
{
#endif

extern page_directory_t *kernel_directory;

extern void initialise_paging();
extern void switch_page_directory(page_directory_t *new);
extern page_t *get_page(uint32_t address, int make, page_directory_t *dir);
extern void alloc_frame(page_t*, int is_kernel, int is_writeable);
extern void page_fault(registers_t *regs);
extern page_directory_t *clone_directory(page_directory_t *src);

#ifdef __cplusplus
}
#endif

#endif