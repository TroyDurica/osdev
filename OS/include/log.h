#ifndef LOG_H
#define LOG_H 0

#include "stdint.h"
#include "isr.h"

#define	PR_LJ	0x01
#define	PR_CA	0x02
#define	PR_SG	0x04
#define	PR_32	0x08
#define	PR_16	0x10
#define	PR_WS	0x20
#define	PR_LZ	0x40
#define	PR_FP	0x80
#define	PR_BUFLEN	16

#define LOG_INFO 0
#define LOG_WRN 1
#define LOG_ERR 2
#define LOG_NPRE 3

#define COM1 0x3F8

#ifdef __cplusplus
extern "C"
{
#endif

extern void log(uint8_t s, const char* fmt, ...);
extern int reg_dump(registers_t *r);

#ifdef __cplusplus
}
#endif

#endif