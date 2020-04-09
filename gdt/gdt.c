#include "gdt.h"
#include "string.h"

extern void gdt_flush(u32);
extern void tss_flush();
static void gdt_set_descriptor(int num, u32 base, u32 limit, u8 access, u8 gran);
static void write_tss(u32, u16, u32);
tss_entry_t tss_entry;

struct gdt_descriptor gdt[5];
struct gdtr_struct gdtr;

void init_gdt()
{

	gdtr.limit = (sizeof(struct gdt_descriptor) * 7 - 1);
	gdtr.base = (u32)&gdt;

	gdt_set_descriptor(0, 0, 0, 0, 0);
	gdt_set_descriptor(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
	gdt_set_descriptor(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
	gdt_set_descriptor(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
	gdt_set_descriptor(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);
	write_tss(5, 0x10, 0x0);
	gdt_set_descriptor(6, 0, 0xFFFFFFFF, 0x9A, 0xCF);

	gdt_flush((u32)&gdtr);
	tss_flush();
}

void gdt_set_descriptor(int num, u32 base, u32 limit, u8 access, u8 gran)
{

	gdt[num].base_low = (base & 0xFFFF);
	gdt[num].base_mid = (base >> 16) & 0xFF;
	gdt[num].base_high = (base >> 24) & 0xFF;

	gdt[num].limit_low = (limit & 0xFFFF);
	gdt[num].granularity = (limit >> 16) & 0xF;
	gdt[num].granularity |= gran & 0xF0;
	gdt[num].access = access;
}

static void write_tss(u32 num, u16 ss0, u32 esp0)
{
	u32 base = (u32)&tss_entry;
	u32 limit = base + sizeof(tss_entry);

	gdt_set_descriptor(num, base, limit, 0xE9, 0x00);

	memset(&tss_entry, 0, sizeof(tss_entry));

	tss_entry.ss0 = ss0;
	tss_entry.esp0 = esp0;

	tss_entry.cs = 0x0b;
	tss_entry.ss = tss_entry.ds = tss_entry.es = tss_entry.fs = tss_entry.gs = 0x13;
}

void set_kernel_stack(u32 stack)
{

	tss_entry.esp0 = stack;
};
