#ifndef LDT
#define LDT

#include "common.h"
#include "isr.h"

struct idt_descriptor
{
	u16 offset_low;
	u16 seg;
	u8 zero;
	u8 flags;
	u16 offset_high;
} __attribute__((packed));

struct idtr_struct
{
	u16 limit;
	u32 base;
} __attribute__((packed));

void init_idt();

void isr0();
void isr1();
void isr2();
void isr3();
void isr4();
void isr5();
void isr6();
void isr7();
void isr8();
void isr9();
void isr10();
void isr11();
void isr12();
void isr13();
void isr14();
void isr15();
void isr16();
void isr17();
void isr18();
void isr19();

void isr20();
void isr21();
void isr22();
void isr23();
void isr24();
void isr25();
void isr26();
void isr27();
void isr28();
void isr29();
void isr30();
void isr31();
void isr128();

void isr255();

void irq_handler(registers_t *regs);

// 声明 IRQ 函数
// IRQ:中断请求(Interrupt Request)
void irq0();  // 电脑系统计时器
void irq1();  // 键盘
void irq2();  // 与 IRQ9 相接，MPU-401 MD 使用
void irq3();  // 串口设备
void irq4();  // 串口设备
void irq5();  // 建议声卡使用
void irq6();  // 软驱传输控制使用
void irq7();  // 打印机传输控制使用
void irq8();  // 即时时钟
void irq9();  // 与 IRQ2 相接，可设定给其他硬件
void irq10(); // 建议网卡使用
void irq11(); // 建议 AGP 显卡使用
void irq12(); // 接 PS/2 鼠标，也可设定给其他硬件
void irq13(); // 协处理器使用
void irq14(); // IDE0 传输控制使用
void irq15(); // IDE1 传输控制使用

#endif
