#include "pit.h"
#include "isr.h"
#include "log.h"
#include "terminal.h"
#include "serial.h"
#include "task.h"

uint32_t timer_ticks = 0;
uint32_t freq;

isr_t pit_callback(registers_t *r)
{
    switch_task();
    timer_ticks++;
}

void pit_wait(uint32_t ticks)
{
    uint32_t t = timer_ticks;
    uint32_t eticks = t + ticks;
    while(timer_ticks < eticks) printk(""); //Without doing something in this loop, it won't ever return to the function that slept
}

void init_pit(uint32_t frequency)
{
    freq = frequency;

	register_interrupt_handler(IRQ0, pit_callback);

	int divisor = 1193180 / frequency;       /* Calculate our divisor */
    outb(0x43, 0x36);             /* Set our command byte 0x36 */
    outb(0x40, divisor & 0xFF);   /* Set low byte of divisor */
    outb(0x40, divisor >> 8);   
}