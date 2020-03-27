#ifndef SERIAL_H
#define SERIAL_H 0

#include "stdint.h"

inline uint8_t inb(uint32_t port)
{
    uint8_t ret;
    asm volatile ("inb %%dx,%%al":"=a" (ret):"d" (port));
    return ret;
}

inline uint16_t inw(uint32_t port)
{
    uint16_t ret;
    asm volatile ("inw %%dx,%%ax":"=a" (ret):"d" (port));
    return ret;
}

inline uint32_t inl(uint32_t port)
{
    uint32_t ret;
    asm volatile ("inl %%dx,%%eax":"=a" (ret):"d" (port));
    return ret;
}

inline void outb(uint32_t port, uint8_t value)
{
    asm volatile ("outb %%al,%%dx": :"d" (port), "a" (value));
}

inline void outw(uint32_t port, uint16_t value)
{
    asm volatile ("outw %%ax,%%dx": :"d" (port), "a" (value));
}

inline void outl(uint32_t port, uint32_t value)
{
    asm volatile ("outl %%eax,%%dx": :"d" (port), "a" (value));
}

#endif