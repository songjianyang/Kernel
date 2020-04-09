#include "timer.h"
#include "isr.h"
#include "console.h"
#include "idt.h"

u32 i = 0;

void timer_callback(registers_t regs)
{
	i++;
	printk("Tick: %d\n", i);
}

void init_timer(u32 frequency)
{
	register_interrupt_handler(IRQ0, timer_callback);

	u32 divisor = 1193180 / frequency;

	outb(0x43, 0x36);
	u8 low = (u8)(divisor & 0xFF);
	u8 high = (u8)((divisor >> 8) & 0xFF);

	outb(0x40, low);
	outb(0x40, high);
}
