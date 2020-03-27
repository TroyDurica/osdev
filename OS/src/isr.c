#include "isr.h"
#include "stdint.h"
#include "log.h"
#include "string.h"
#include "terminal.h"
#include "common.h"
#include "stack.h"

isr_t interrupt_handlers[256];

isr_t exc_divide_by_zero(registers_t *r)
{
	reg_dump(r);
	stack_trace(0x1000);
	PANIC("EXCEPTION: DIVIDE_BY_ZERO")
}
isr_t exc_debug(registers_t *r)
{
	reg_dump(r);
	stack_trace(0x1000);
	PANIC("EXCEPTION: DEBUG");
}
isr_t exc_non_maskable_interrupt(registers_t *r)
{
	reg_dump(r);
	stack_trace(0x1000);
	PANIC("EXCEPTION: NON_MASKABLE_INTERRUPT");
}
isr_t exc_breakpoint(registers_t *r)
{
	reg_dump(r);
	stack_trace(0x1000);
	PANIC("EXCEPTION: BREAKPOINT");
}
isr_t exc_into_detected_overflow(registers_t *r)
{
	reg_dump(r);
	stack_trace(0x1000);
	PANIC("EXCEPTION: INTO_DETECTED_OVERFLOW");
}
isr_t exc_out_of_bounds(registers_t *r)
{
	reg_dump(r);
	stack_trace(0x1000);
	PANIC("EXCEPTION: OUT_OF_BOUNDS");
}
isr_t exc_invalid_opcode(registers_t *r)
{
	reg_dump(r);
	stack_trace(0x1000);
	PANIC("EXCEPTION: INVALID_OPCODE");
}
isr_t exc_no_coprocessor(registers_t *r)
{
	reg_dump(r);
	stack_trace(0x1000);
	PANIC("EXCEPTION: NO_COPROCESSOR");
}
isr_t exc_double_fault(registers_t *r)
{
	reg_dump(r);
	stack_trace(0x1000);
	PANIC("EXCEPTION: DOUBLE_FAULT");
}
isr_t exc_coprocessor_segment_overrun(registers_t *r)
{
	reg_dump(r);
	stack_trace(0x1000);
	PANIC("EXCEPTION: COPROCESSOR_SEGMENT_OVERRUN");
}
isr_t exc_invalid_tss(registers_t *r)
{
	reg_dump(r);
	stack_trace(0x1000);
	PANIC("EXCEPTION: INVALID_TSS");
}
isr_t exc_segment_not_present(registers_t *r)
{
	reg_dump(r);
	stack_trace(0x1000);
	PANIC("EXCEPTION: SEGMENT_NOT_PRESENT");
}
isr_t exc_stack_fault(registers_t *r)
{
	reg_dump(r);
	stack_trace(0x1000);
	PANIC("EXCEPTION: STACK_FAULT");
}
isr_t exc_general_protection_fault(registers_t *r)
{
	reg_dump(r);
	stack_trace(0x1000);
	PANIC("EXCEPTION: GENERAL_PROTECTION_FAULT");
}
isr_t exc_page_fault(registers_t *r)
{
	reg_dump(r);
	stack_trace(0x1000);
	PANIC("EXCEPTION: PAGE_FAULT");
}
isr_t exc_unknown(registers_t *r)
{
	reg_dump(r);
	stack_trace(0x1000);
	PANIC("EXCEPTION: UNKNOWN");
}
isr_t exc_coprocessor_fault(registers_t *r)
{
	reg_dump(r);
	stack_trace(0x1000);
	PANIC("EXCEPTION: COPROCESSOR_FAULT");
}
isr_t exc_alignment_check(registers_t *r)
{
	reg_dump(r);
	stack_trace(0x1000);
	PANIC("EXCEPTION: ALIGNMENT_CHECK");
}
isr_t exc_machine_check(registers_t *r)
{
	reg_dump(r);
	stack_trace(0x1000);
	PANIC("EXCEPTION: MACHINE_CHECK");
}
isr_t exc_reserved_1(registers_t *r)
{
	reg_dump(r);
	stack_trace(0x1000);
	PANIC("EXCEPTION: RESERVED_1");
}
isr_t exc_reserved_2(registers_t *r)
{
	reg_dump(r);
	stack_trace(0x1000);
	PANIC("EXCEPTION: RESERVED_2");
}
isr_t exc_reserved_3(registers_t *r)
{
	reg_dump(r);
	stack_trace(0x1000);
	PANIC("EXCEPTION: RESERVED_3");
}
isr_t exc_reserved_4(registers_t *r)
{
	reg_dump(r);
	stack_trace(0x1000);
	PANIC("EXCEPTION: RESERVED_4");
}
isr_t exc_reserved_5(registers_t *r)
{
	reg_dump(r);
	stack_trace(0x1000);
	PANIC("EXCEPTION: RESERVED_5");
}
isr_t exc_reserved_6(registers_t *r)
{
	reg_dump(r);
	stack_trace(0x1000);
	PANIC("EXCEPTION: RESERVED_6");
}
isr_t exc_reserved_7(registers_t *r)
{
	reg_dump(r);
	stack_trace(0x1000);
	PANIC("EXCEPTION: RESERVED_7");
}
isr_t exc_reserved_8(registers_t *r)
{
	reg_dump(r);
	stack_trace(0x1000);
	PANIC("EXCEPTION: RESERVED_8");
}
isr_t exc_reserved_9(registers_t *r)
{
	reg_dump(r);
	stack_trace(0x1000);
	PANIC("EXCEPTION: RESERVED_9");
}
isr_t exc_reserved_10(registers_t *r)
{
	reg_dump(r);
	stack_trace(0x1000);
	PANIC("EXCEPTION: RESERVED_10");
}
isr_t exc_reserved_11(registers_t *r)
{
	reg_dump(r);
	stack_trace(0x1000);
	PANIC("EXCEPTION: RESERVED_11");
}
isr_t exc_reserved_12(registers_t *r)
{
	reg_dump(r);
	stack_trace(0x1000);
	PANIC("EXCEPTION: RESERVED_12");
}

void isr_init()
{
	register_interrupt_handler(0, exc_divide_by_zero);
	register_interrupt_handler(1, exc_debug);
	register_interrupt_handler(2, exc_breakpoint);
	register_interrupt_handler(3, exc_non_maskable_interrupt);
	register_interrupt_handler(4, exc_into_detected_overflow);
	register_interrupt_handler(5, exc_out_of_bounds);
	register_interrupt_handler(6, exc_invalid_opcode);
	register_interrupt_handler(7, exc_no_coprocessor);
	register_interrupt_handler(8, exc_double_fault);
	register_interrupt_handler(9, exc_coprocessor_segment_overrun);
	register_interrupt_handler(10, exc_invalid_tss);
	register_interrupt_handler(11, exc_segment_not_present);
	register_interrupt_handler(12, exc_stack_fault);
	register_interrupt_handler(13, exc_general_protection_fault);
	register_interrupt_handler(14, exc_page_fault);
	register_interrupt_handler(15, exc_unknown);
	register_interrupt_handler(16, exc_coprocessor_fault);
	register_interrupt_handler(17, exc_alignment_check);
	register_interrupt_handler(18, exc_machine_check);
	register_interrupt_handler(19, exc_reserved_1);
	register_interrupt_handler(20, exc_reserved_2);
	register_interrupt_handler(21, exc_reserved_3);
	register_interrupt_handler(22, exc_reserved_4);
	register_interrupt_handler(23, exc_reserved_5);
	register_interrupt_handler(24, exc_reserved_6);
	register_interrupt_handler(25, exc_reserved_7);
	register_interrupt_handler(26, exc_reserved_8);
	register_interrupt_handler(27, exc_reserved_9);
	register_interrupt_handler(28, exc_reserved_10);
	register_interrupt_handler(29, exc_reserved_11);
	register_interrupt_handler(30, exc_reserved_11);
	register_interrupt_handler(31, exc_reserved_12);
}

void register_interrupt_handler(uint8_t n, isr_t handler)
{
  interrupt_handlers[n] = handler;
}

void isr_handler(registers_t *r)
{
	if (interrupt_handlers[r->int_no] != 0)
	{
		isr_t handler = interrupt_handlers[r->int_no];
		if (r->int_no != 128)
		{
			uint8_t orig = terminal_getcolor();
			terminal_setcolor(VGA_COLOR_RED + (VGA_COLOR_WHITE << 4));
			handler(r);
			terminal_setcolor(orig);
		}
		else handler(r);
	}
	else
	{
		log(LOG_ERR, "Unknown interrupt <0x%x>\n", r->int_no);
		PANIC("Empty ISR")
	}
	
}

void irq_handler(registers_t *r)
{
	if (r->int_no >= 40)
	{
		outb(0xA0, 0x20);
	}

	outb(0x20, 0x20);

	if (interrupt_handlers[r->int_no] != 0)
	{
		isr_t handler = interrupt_handlers[r->int_no];
		handler(r);
	}
}
