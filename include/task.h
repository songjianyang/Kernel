#ifndef TASK
#define TASK

#include "common.h"
#include "paging.h"

#define KERNEL_STACK_SIZE 2048

typedef struct task
{
    int id;                           // Process ID.
    u32 esp, ebp;                     // Stack and base pointers.
    u32 eip;                          // Instruction pointer.
    page_directory_t *page_directory; // Page directory.
    u32 kernel_stack;                 // Kernel stack location.
    struct task *next;                // The next task in a linked list.
} task_t;

#endif
