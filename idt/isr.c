#include "common.h"
#include "console.h"
#include "isr.h"

isr_t interrupt_handlers[256];

void isr_handler(registers_t *regs)
{
	u8 int_no = regs->int_no & 0xFF;
	printk("recieved interrupt: %d\n", int_no);

	if (interrupt_handlers[int_no] != 0)
	{

		isr_t handler = interrupt_handlers[int_no];
		handler(regs);
	}
	else
	{
		printk("unhandled interrupt: ");
		printk(int_no);
		printk('\n');
	}
}

void irq_handler(registers_t *regs)
{
	if (regs->int_no >= 40)
	{
		outb(0xA0, 0x20);
	}
	outb(0x20, 0x20);

	if (interrupt_handlers[regs->int_no] != 0)
	{
		interrupt_handlers[regs->int_no](regs);
	}
}

void register_interrupt_handler(u8 n, isr_t handler)
{

	interrupt_handlers[n] = handler;
}
