#ifndef COMMON_H
#define COMMON_H 0

#include "log.h"
#include "terminal.h"

#define PANIC(msg) \
{ \
asm("cli"); \
log(LOG_ERR, "PANIC(%s) at %s : %d", msg, __FILE__, __LINE__); \
printk("PANIC(%s) at %s : %d", msg, __FILE__, __LINE__); \
for (;;); \
}
#define ASSERT(b) if (!(b)) PANIC(#b) else (void)0;

typedef int (*fnptr_t)(char, void**);
typedef void (*call_module_t)(void);

#ifdef __cplusplus
extern "C"
{
#endif

extern uint32_t placement_address;

extern void late_init();

#ifdef __cplusplus
}
#endif

#endif