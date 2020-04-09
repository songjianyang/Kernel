#include "idt.h"
#include "string.h"

extern void idt_flush(u32);

struct idt_descriptor idt[256];
struct idtr_struct idtr;

void idt_set_descriptor(int num, u32 offset, u16 seg, u8 flags);

void init_idt()
{
	outb(0x20, 0x11);
	outb(0xA0, 0x11);
	outb(0x21, 0x20);
	outb(0xA1, 0x28);
	outb(0x21, 0x04);
	outb(0xA1, 0x02);
	outb(0x21, 0x01);
	outb(0xA1, 0x01);
	outb(0x21, 0x0);
	outb(0xA1, 0x0);

	idtr.limit = sizeof(struct idt_descriptor) * 256 - 1;
	idtr.base = (u32)&idt;

	bzero(&idt, sizeof(struct idt_descriptor) * 256);

	idt_set_descriptor(0, (u32)isr0, 0x08, 0x8E);
	idt_set_descriptor(1, (u32)isr1, 0x08, 0x8E);
	idt_set_descriptor(2, (u32)isr2, 0x08, 0x8E);
	idt_set_descriptor(3, (u32)isr3, 0x08, 0x8E);
	idt_set_descriptor(4, (u32)isr4, 0x08, 0x8E);
	idt_set_descriptor(5, (u32)isr5, 0x08, 0x8E);
	idt_set_descriptor(6, (u32)isr6, 0x08, 0x8E);
	idt_set_descriptor(7, (u32)isr7, 0x08, 0x8E);
	idt_set_descriptor(8, (u32)isr8, 0x08, 0x8E);
	idt_set_descriptor(9, (u32)isr9, 0x08, 0x8E);
	idt_set_descriptor(10, (u32)isr10, 0x08, 0x8E);
	idt_set_descriptor(11, (u32)isr11, 0x08, 0x8E);
	idt_set_descriptor(12, (u32)isr12, 0x08, 0x8E);
	idt_set_descriptor(13, (u32)isr13, 0x08, 0x8E);
	idt_set_descriptor(14, (u32)isr14, 0x08, 0x8E);
	idt_set_descriptor(15, (u32)isr15, 0x08, 0x8E);
	idt_set_descriptor(16, (u32)isr16, 0x08, 0x8E);
	idt_set_descriptor(17, (u32)isr17, 0x08, 0x8E);
	idt_set_descriptor(18, (u32)isr18, 0x08, 0x8E);
	idt_set_descriptor(19, (u32)isr19, 0x08, 0x8E);
	idt_set_descriptor(20, (u32)isr20, 0x08, 0x8E);
	idt_set_descriptor(21, (u32)isr21, 0x08, 0x8E);
	idt_set_descriptor(22, (u32)isr22, 0x08, 0x8E);
	idt_set_descriptor(23, (u32)isr23, 0x08, 0x8E);
	idt_set_descriptor(24, (u32)isr24, 0x08, 0x8E);
	idt_set_descriptor(25, (u32)isr25, 0x08, 0x8E);
	idt_set_descriptor(26, (u32)isr26, 0x08, 0x8E);
	idt_set_descriptor(27, (u32)isr27, 0x08, 0x8E);
	idt_set_descriptor(28, (u32)isr28, 0x08, 0x8E);
	idt_set_descriptor(29, (u32)isr29, 0x08, 0x8E);
	idt_set_descriptor(30, (u32)isr30, 0x08, 0x8E);
	idt_set_descriptor(31, (u32)isr31, 0x08, 0x8E);
	idt_set_descriptor(128, (u32)isr128, 0x08, 0x8E);

	idt_set_descriptor(32, (uint32_t)irq0, 0x08, 0x8E);
	idt_set_descriptor(33, (uint32_t)irq1, 0x08, 0x8E);
	idt_set_descriptor(34, (uint32_t)irq2, 0x08, 0x8E);
	idt_set_descriptor(35, (uint32_t)irq3, 0x08, 0x8E);
	idt_set_descriptor(36, (uint32_t)irq4, 0x08, 0x8E);
	idt_set_descriptor(37, (uint32_t)irq5, 0x08, 0x8E);
	idt_set_descriptor(38, (uint32_t)irq6, 0x08, 0x8E);
	idt_set_descriptor(39, (uint32_t)irq7, 0x08, 0x8E);
	idt_set_descriptor(40, (uint32_t)irq8, 0x08, 0x8E);
	idt_set_descriptor(41, (uint32_t)irq9, 0x08, 0x8E);
	idt_set_descriptor(42, (uint32_t)irq10, 0x08, 0x8E);
	idt_set_descriptor(43, (uint32_t)irq11, 0x08, 0x8E);
	idt_set_descriptor(44, (uint32_t)irq12, 0x08, 0x8E);
	idt_set_descriptor(45, (uint32_t)irq13, 0x08, 0x8E);
	idt_set_descriptor(46, (uint32_t)irq14, 0x08, 0x8E);
	idt_set_descriptor(47, (uint32_t)irq15, 0x08, 0x8E);

	idt_set_descriptor(255, (uint32_t)isr255, 0x08, 0x8E);
	idt_flush((u32)&idtr);
}

void idt_set_descriptor(int num, u32 offset, u16 seg, u8 flags)
{
	idt[num].offset_low = offset & 0xFFFF;
	idt[num].offset_high = (offset >> 16) & 0xFFFF;

	idt[num].seg = seg;
	idt[num].zero = 0;
	idt[num].flags = flags | 0x60;
}
