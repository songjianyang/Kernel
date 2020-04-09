#ifndef ISR
#define ISR

#define IRQ0 32
#define IRQ1 33
#define IRQ2 34
#define IRQ3 35
#define IRQ4 36
#define IRQ5 37
#define IRQ6 38
#define IRQ7 39
#define IRQ8 40
#define IRQ9 41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

typedef struct registers
{
	uint32_t ds;  // 用于保存用户的数据段描述符
	uint32_t edi; // 从 edi 到 eax 由 pusha 指令压入
	uint32_t esi;
	uint32_t ebp;
	uint32_t esp;
	uint32_t ebx;
	uint32_t edx;
	uint32_t ecx;
	uint32_t eax;
	uint32_t int_no;   // 中断号
	uint32_t err_code; // 错误代码(有中断错误代码的中断会由CPU压入)
	uint32_t eip;	   // 以下由处理器自动压入
	uint32_t cs;
	uint32_t eflags;
	uint32_t useresp;
	uint32_t ss;
} registers_t;

typedef void (*isr_t)(registers_t *);

void register_interrupt_handler(u8 n, isr_t handler);

#endif
