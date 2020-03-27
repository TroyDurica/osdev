#ifndef TERMINAL_H
#define TERMINAL_H 0

#include "stdint.h"
#include "stddef.h"
#include "stdarg.h"

#define	PR_LJ	0x01
#define	PR_CA	0x02
#define	PR_SG	0x04
#define	PR_32	0x08
#define	PR_16	0x10
#define	PR_WS	0x20
#define	PR_LZ	0x40
#define	PR_FP	0x80
#define	PR_BUFLEN	16

typedef enum {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
} vga_color;

#ifdef __cplusplus
extern "C"
{
#endif

extern void terminal_initialize();
extern void terminal_putentryat(char, uint8_t, size_t, size_t);
extern int printk(const char*, ...);
extern void terminal_setcolor(uint8_t);
extern uint8_t terminal_getcolor();
extern int vprintk(const char*, va_list);
extern void terminal_setpos(size_t, size_t);
extern void terminal_clear();

#ifdef __cplusplus
}
#endif

#endif