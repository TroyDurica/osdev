#ifndef TASK_H
#define TASK_H 0

#include "isr.h"
#include "page.h"
#include "stdint.h"

#define KERNEL_STACK_SIZE 0x2000

typedef struct task
{
    int id;                // Process ID.
    uint32_t esp, ebp;       // Stack and base pointers.
    uint32_t eip;            // Instruction pointer.
    page_directory_t *page_directory; // Page directory.
    uint32_t kernel_stack;
    struct task *next;     // The next task in a linked list.
} task_t;

#ifdef __cplusplus
extern "C"
{
#endif

extern void initialise_tasking();
extern void switch_task();
extern int fork();
extern void move_stack(void *new_stack_start, uint32_t size);
extern int getpid();
extern void switch_to_user_mode();

#ifdef __cplusplus
}
#endif

#endif