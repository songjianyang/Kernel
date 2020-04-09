#include "gdt.h"
#include "console.h"
#include "idt.h"
#include "timer.h"
#include "paging.h"
#include "syscall.h"

int kmain(void)
{
	console_clear();

	init_gdt();
	init_idt();

	printk("Hello,kernel;\n");

	//init_timer(200);
	asm volatile("sti");
	initialise_paging();
	
	void *addr1 = kmalloc(50);
	printk("kmalloc 50 byte in 0x%X\n", addr1);

	void *addr2 = kmalloc(500);
	printk("kmalloc 500 byte in 0x%X\n", addr2);

	printk("free mem in 0x%X\n", addr1);
	kfree(addr1);

	printk("free mem in 0x%X\n", addr2);
	kfree(addr2);

	initialise_syscalls();

	switch_to_user_mode();
	syscall_printk("Hello, User World.");

	return 0;
}
