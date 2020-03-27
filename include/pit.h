#ifndef PIT_H
#define PIT_H 0

#include "stdint.h"

#ifdef __cplusplus
extern "C"
{
#endif

extern void init_pit(uint32_t frequency);
extern void pit_wait(uint32_t ticks);

#ifdef __cplusplus
}
#endif

#endif