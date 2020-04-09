#include "gdt.h"
#include "task.h"

volatile task_t *current_task;

void switch_to_user_mode()
{
  // Set up our kernel stack.
  set_kernel_stack(0x300000);

  // Set up a stack structure for switching to user mode.
  asm volatile("  \
      cli; \
      mov $0x23, %ax; \
      mov %ax, %ds; \
      mov %ax, %es; \
      mov %ax, %fs; \
      mov %ax, %gs; \
                    \
       \
      mov %esp, %eax; \
      pushl $0x23; \
      pushl %esp; \
      pushf; \
      pushl $0x1B; \
      push $1f; \
      iret; \
    1: \
      ");
}
