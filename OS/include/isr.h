#ifndef IRQ_H
#define IRQ_H 0

#include "stdint.h"
#include "serial.h"

#define MAX_NESTEXC 5

#define IRQ0 32
#define IRQ1 33
#define IRQ2 34
#define IRQ3 35
#define IRQ4 36
#define IRQ5 37
#define IRQ6 38
#define IRQ7 39
#define IRQ8 40
#define IRQ9 41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

typedef struct
{
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
} registers_t;

typedef void (*isr_t)(registers_t*);

#ifdef __cplusplus
extern "C"
{
#endif

extern void isr_init();
extern void register_interrupt_handler(uint8_t n, isr_t handler);

#ifdef __cplusplus
}
#endif

#endif