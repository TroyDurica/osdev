#include "syscall.h"
#include "isr.h"
#include "log.h"
#include "terminal.h"
#include "stdint.h"
#include "task.h"

static void syscall_handler(registers_t *regs);

DEFN_SYSCALL2(printk, 0, const char*, va_list);
DEFN_SYSCALL0(fork, 1);

void printf(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	syscall_printk(fmt, args);

	va_end(args);
}

static void *syscalls[2] =
{
    &vprintk,
    &fork
};
uint32_t num_syscalls = 2;

void initialise_syscalls()
{
    // Register our syscall handler.
    register_interrupt_handler (0x80, &syscall_handler);
}

void syscall_handler(registers_t *regs)
{
    // Firstly, check if the requested syscall number is valid.
    // The syscall number is found in EAX.
    if (regs->eax >= num_syscalls)
        return;

    // Get the required syscall location.
    void *location = syscalls[regs->eax];

    // We don't know how many parameters the function wants, so we just
    // push them all onto the stack in the correct order. The function will
    // use all the parameters it wants, and we can pop them all back off afterwards.
    int ret;
    asm (" \
        push %1; \
        push %2; \
        push %3; \
        push %4; \
        push %5; \
        call *%6; \
        pop %%ebx; \
        pop %%ebx; \
        pop %%ebx; \
        pop %%ebx; \
        pop %%ebx; \
    " : "=a" (ret) : "r" (regs->edi), "r" (regs->esi), "r" (regs->edx), "r" (regs->ecx), "r" (regs->ebx), "r" (location));
    regs->eax = ret;
}