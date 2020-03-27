#include "log.h"
#include "isr.h"
#include "serial.h"
#include "string.h"
#include "common.h"

#include "stddef.h"
#include "stdint.h"
#include "stdarg.h"

int do_printl(const char *fmt, va_list args, fnptr_t fn, void *ptr)
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
				fn(*fmt, &ptr);
				count++;
				break;
			}
			state++;
			fmt++;
		case 1:
			if(*fmt == '%')
			{
				fn(*fmt, &ptr);
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
						num = va_arg(args, short);
					else
						num = va_arg(args, unsigned short);
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
					unsigned char);
				actual_wd = 1;
				goto EMIT2;
			case 's':
				flags &= ~PR_LZ;
				where = va_arg(args, unsigned char *);
EMIT:
				actual_wd = strlen(where);
				if(flags & PR_WS)
					actual_wd++;
				if((flags & (PR_WS | PR_LZ)) ==
					(PR_WS | PR_LZ))
				{
					fn('-', &ptr);
					count++;
				}
EMIT2:				if((flags & PR_LJ) == 0)
				{
					while(given_wd > actual_wd)
					{
						fn(flags & PR_LZ ? '0' :
							' ', &ptr);
						count++;
						given_wd--;
					}
				}
				if((flags & (PR_WS | PR_LZ)) == PR_WS)
				{
					fn('-', &ptr);
					count++;
				}
				while(*where != '\0')
				{
					fn(*where++, &ptr);
					count++;
				}
				if(given_wd < actual_wd)
					given_wd = 0;
				else given_wd -= actual_wd;
				for(; given_wd; given_wd--)
				{
					fn(' ', &ptr);
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

int vprintl_help(unsigned char c, void **ptr)
{
	log_int(c);
	return 0;
}

int vprintl(const char* fmt, va_list args)
{
	return do_printl(fmt, args, vprintl_help, NULL);
}

int printl(const char* fmt, ...)
{
	va_list args;
	int ret_val;

	va_start(args, fmt);
	ret_val = vprintl(fmt, args);
	va_end(args);
	return ret_val;
}

void log_int(const char* c)
{
	outb(COM1, c);
}

void log(uint8_t s, const char* fmt, ...)
{
	switch (s)
	{
		case LOG_INFO:
			printl("[INFO] ");
			break;
		case LOG_WRN:
			printl("[WARN] ");
			break;
		case LOG_ERR:
			printl("[ERROR] ");
			break;
		default:
			break;
	}

	va_list args;
	va_start(args, fmt);

	vprintl(fmt, args);

	va_end(args);
}

int reg_dump(registers_t *r)
{
	log(LOG_INFO, "Register dump:\n");
	log(LOG_INFO, "DS:0x%x\n", r->ds);
	log(LOG_INFO, "EDI:0x%x ESI:0x%x EBP:0x%x ESP:0x%x EBX:0x%x EDX:0x%x ECX:0x%x EAX:0x%x\n", r->edi, r->esi, r->ebp, r->esp, r->ebx, r->edx, r->ecx, r->eax);
	log(LOG_INFO, "INT_NO:0x%x ERR_CODE:0x%x\n", r->int_no, r->err_code);
	log(LOG_INFO, "EIP:0x%x CS:0x%x EFLAGS:0x%x USERESP:0x%x SS:0x%x\n", r->eip, r->cs, r->eflags, r->useresp, r->ss);
}