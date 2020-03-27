#include "stack.h"
#include "log.h"
#include "kheap.h"
#include "string.h"

void stack_trace(uint32_t max_frames)
{
	log(LOG_INFO, "Stack Trace:\n");

    uint32_t ebp, esp;
    uint32_t* data;
    asm("movl %%ebp, %0" : "=r"(ebp));
    asm("movl %%esp, %0" : "=r"(esp));
    data = (uint32_t*)esp;

	for (int i = 0; i < max_frames; i++)
    {
		log(LOG_INFO, "0x%x (ESP+0x%x): 0x%x\n", esp, i, *data);
		*data++;
		esp++;
		if (esp == ebp)
		{
			log(LOG_INFO, "Reached end of stack at address 0x%x\n", esp);
			break;
		}
    }

	struct stackframe *stk;
    asm("movl %%ebp,%0" : "=r"(stk));
    log(LOG_INFO, "Stack trace:\n");
    for(unsigned int frame = 0; stk && frame < max_frames; ++frame)
    {
        // Unwind to previous stack frame
        log(LOG_INFO, "  0x%x     \n", stk->eip);
        stk = stk->ebp;
    }
}