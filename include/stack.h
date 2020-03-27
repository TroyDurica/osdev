#ifndef STACK_H
#define STACK_H 0

#include "stdint.h"

struct stackframe {
    struct stackframe* ebp;
    uint32_t eip;
};

#ifdef __cplusplus
extern "C"
{
#endif

extern void stack_trace(uint32_t max_frames);

#ifdef __cplusplus
}
#endif

#endif