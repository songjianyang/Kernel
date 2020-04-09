#include "syscall.h"
#include "isr.h"
#include "gdt.h"
#include "console.h"

static void syscall_handler(registers_t *regs);
static void syscall_handler1();

DEFN_SYSCALL1(printk, 0, const char *);

static void *syscalls[1] =
    {
        &printk,
};

u32 num_syscalls = 1;

void initialise_syscalls()
{
  register_interrupt_handler(0x80, &syscall_handler);
}

void syscall_handler(registers_t *regs)
{
  if (regs->eax >= num_syscalls)
  {
    return;
  }

  void *location = syscalls[regs->eax];

  int ret;

  asm volatile(" \
     push %1; \
     push %2; \
     push %3; \
     push %4; \
     push %5; \
     call *%6; \
     pop %%ebx; \
     pop %%ebx; \
     pop %%ebx; \
     pop %%ebx; \
     pop %%ebx; \
   "
               : "=a"(ret)
               : "r"(regs->edi), "r"(regs->esi), "r"(regs->edx), "r"(regs->ecx), "r"(regs->ebx), "r"(location));
  regs->eax = ret;
}
