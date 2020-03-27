#include "stddef.h"
#include "stdint.h"
#include "stdarg.h"
#include "terminal.h"
#include "string.h"
#include "serial.h"
#include "log.h"
#include "common.h"
#include "syscall.h"
#include "kheap.h"

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
 
size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_framebuffer;

void scroll();

static inline uint8_t vga_entry_color(vga_color fg, vga_color bg) 
{
	return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) 
{
	return (uint16_t) uc | (uint16_t) color << 8;
}

static int update_cursor()
{
	uint32_t pos = (terminal_row * 80) + terminal_column;
	outb(0x3D4, 14);
	outb(0x3D5, ((pos >> 8) & 0xFF));
	outb(0x3D4, 15);
	outb(0x3D5, (pos & 0xFF));
}

void terminal_initialize() 
{
	log(LOG_INFO, "Initializing Terminal\n");
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_BLUE, VGA_COLOR_WHITE);
	terminal_framebuffer = (uint16_t*) 0xB8000;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_framebuffer[index] = vga_entry(' ', terminal_color);
		}
	}
	update_cursor();
}

void terminal_setcolor(uint8_t color) 
{
	terminal_color = color;
}

uint8_t terminal_getcolor()
{
	return terminal_color;
}

void terminal_clear()
{
	for (int y = 0; y < 25; y++)
    {
        for (int x = 0; x < 80; x++)
        {
            terminal_putentryat(' ', terminal_getcolor(), x, y);
        }
    }

	terminal_setpos(0, 0);
}

void terminal_setpos(size_t x, size_t y)
{
	terminal_column = x;
	terminal_row = y;
	update_cursor();
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) 
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_framebuffer[index] = vga_entry(c, color);
	update_cursor();
}

void terminal_putchar(char c) 
{
	terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
	if (++terminal_column == VGA_WIDTH) {
		terminal_column = 0;
		if (++terminal_row == VGA_HEIGHT)
			scroll();
	}
}

void terminal_write(const char* data, size_t size) 
{
	for (size_t i = 0; i < size; i++)
	{
		switch (data[i])
		{
			case '\n':
				terminal_column = 0;
				if (++terminal_row == VGA_HEIGHT)
				{
					scroll();
				}
				break;
			case '\t':
				for (int i = 0; i < 4; i++)
				{
					if (++terminal_column == VGA_WIDTH)
					{
						if (++terminal_row == VGA_HEIGHT)
						{
							scroll();
						}
					}
				}
				break;
			default:
				terminal_putchar(data[i]);
				break;
		}
	}
}

int do_printk(const char *fmt, va_list args, fnptr_t fn, void *ptr)
{
	unsigned char state, radix, *where, buf[PR_BUFLEN];
	unsigned flags, actual_wd, count, given_wd;
	long num;

	state = flags = count = given_wd = 0;
	for(; *fmt; fmt++)
	{
		switch(state)
		{
		case 0:
			if(*fmt != '%')
			{
				fn(*fmt, (void**)&ptr);
				count++;
				break;
			}
			state++;
			fmt++;
		case 1:
			if(*fmt == '%')
			{
				fn(*fmt, (void**)&ptr);
				count++;
				state = flags = given_wd = 0;
				break;
			}
			if(*fmt == '-')
			{
				if(flags & PR_LJ)
					state = flags = given_wd = 0;
				else
					flags |= PR_LJ;
				break;
			}
			state++;
			if(*fmt == '0')
			{
				flags |= PR_LZ;
				fmt++;
			}
		case 2:
			if(*fmt >= '0' && *fmt <= '9')
			{
				given_wd = 10 * given_wd +
					(*fmt - '0');
				break;
			}
			state++;
		case 3:
			if(*fmt == 'F')
			{
				flags |= PR_FP;
				break;
			}
			if(*fmt == 'N')
				break;
			if(*fmt == 'l')
			{
				flags |= PR_32;
				break;
			}
			if(*fmt == 'h')
			{
				flags |= PR_16;
				break;
			}
			state++;
		case 4:
			where = buf + PR_BUFLEN - 1;
			*where = '\0';
			switch(*fmt)
			{
			case 'X':
				flags |= PR_CA;
			case 'x':
			case 'p':
			case 'n':
				radix = 16;
				goto DO_NUM;
			case 'd':
			case 'i':
				flags |= PR_SG;
			case 'u':
				radix = 10;
				goto DO_NUM;
			case 'o':
				radix = 8;
DO_NUM:				if(flags & PR_32)
					num = va_arg(args, unsigned long);
				else if(flags & PR_16)
				{
					if(flags & PR_SG)
						num = va_arg(args, int);
					else
						num = va_arg(args, unsigned int);
				}
				else
				{
					if(flags & PR_SG)
						num = va_arg(args, int);
					else
						num = va_arg(args, unsigned int);
				}

				if(flags & PR_SG)
				{
					if(num < 0)
					{
						flags |= PR_WS;
						num = -num;
					}
				}
				do
				{
					unsigned long temp;

					temp = (unsigned long)num % radix;
					where--;
					if(temp < 10)
						*where = temp + '0';
					else if(flags & PR_CA)
						*where = temp - 10 + 'A';
					else
						*where = temp - 10 + 'a';
					num = (unsigned long)num / radix;
				}
				while(num != 0);
				goto EMIT;
			case 'c':
				flags &= ~PR_LZ;
				where--;
				*where = (unsigned char)va_arg(args,
					unsigned int);
				actual_wd = 1;
				goto EMIT2;
			case 's':
				flags &= ~PR_LZ;
				where = va_arg(args, unsigned char *);
EMIT:
				actual_wd = strlen((const char*)where);
				if(flags & PR_WS)
					actual_wd++;
				if((flags & (PR_WS | PR_LZ)) ==
					(PR_WS | PR_LZ))
				{
					fn('-', (void**)&ptr);
					count++;
				}
EMIT2:				if((flags & PR_LJ) == 0)
				{
					while(given_wd > actual_wd)
					{
						fn(flags & PR_LZ ? '0' :
							' ', (void**)&ptr);
						count++;
						given_wd--;
					}
				}
				if((flags & (PR_WS | PR_LZ)) == PR_WS)
				{
					fn('-', (void**)&ptr);
					count++;
				}
				while(*where != '\0')
				{
					fn(*where++, (void**)&ptr);
					count++;
				}
				if(given_wd < actual_wd)
					given_wd = 0;
				else given_wd -= actual_wd;
				for(; given_wd; given_wd--)
				{
					fn(' ', (void**)&ptr);
					count++;
				}
				break;
			default:
				break;
			}
		default:
			state = flags = given_wd = 0;
			break;
		}
	}
	return count;
}

int vprintk_help(unsigned char c, void** ptr)
{
	switch (c)
	{
		case '\n':
			terminal_column = 0;
			if (++terminal_row == VGA_HEIGHT)
			{
				scroll();
			}
			break;
		case '\t':
			for (int i = 0; i < 4; i++)
			{
				if (++terminal_column == VGA_WIDTH)
				{
					if (++terminal_row == VGA_HEIGHT)
					{
						scroll();
					}
				}
			}
			break;
		case '\b':
			if (terminal_column == 0 && terminal_row == 0);
			else if (terminal_column == 0)
			{
				terminal_column = 79;
				if (terminal_row != 0) --terminal_row;
			}
			else --terminal_column;
			terminal_putentryat(' ', terminal_color, terminal_column, terminal_row);
			update_cursor();
			break;
		default:
			terminal_putchar(c);
			break;
	}
	return 0;
}

int vprintk(const char* fmt, va_list args)
{
	return do_printk(fmt, args, vprintk_help, NULL);
}

int printk(const char* fmt, ...)
{
	va_list args;
	int ret_val;

	va_start(args, fmt);
	ret_val = vprintk(fmt, args);
	va_end(args);
	return ret_val;
}

void scroll()
{
    for(size_t i = 0; i < VGA_HEIGHT - 1; i++)
	{
        for (size_t m = 0; m < VGA_WIDTH; m++)
		{
            terminal_framebuffer[i * VGA_WIDTH + m] = terminal_framebuffer[(i + 1) * VGA_WIDTH + m];
        }
    }

	terminal_row = VGA_HEIGHT - 1;
	terminal_column = 0;

	for (size_t i = 0; i < VGA_WIDTH - 1; i++)
	{
		terminal_framebuffer[24 * 80 + i] = vga_entry(' ', terminal_color);
	}
}