[global idt_flush]

idt_flush:
	mov eax,[esp+4]
	lidt [eax]
	ret
.end:

%macro ISR_NOERRCODE 1  ; define a macro, taking one parameter
  [GLOBAL isr%1]        ; %1 accesses the first parameter.
  isr%1:
    cli
    push byte 0
    push  %1
    jmp isr_common_stub
%endmacro

%macro ISR_ERRCODE 1
  [GLOBAL isr%1]
  isr%1:
    cli
    push  %1
    jmp isr_common_stub
%endmacro


ISR_NOERRCODE  0 	
ISR_NOERRCODE  1 	
ISR_NOERRCODE  2 	
ISR_NOERRCODE  3 	
ISR_NOERRCODE  4 	
ISR_NOERRCODE  5 	
ISR_NOERRCODE  6 	
ISR_NOERRCODE  7 	
ISR_ERRCODE    8 	
ISR_NOERRCODE  9 	
ISR_ERRCODE   10 	
ISR_ERRCODE   11 	
ISR_ERRCODE   12 	
ISR_ERRCODE   13 	
ISR_ERRCODE   14 	
ISR_NOERRCODE 15 	
ISR_NOERRCODE 16 	
ISR_ERRCODE   17 	
ISR_NOERRCODE 18 	
ISR_NOERRCODE 19 	


ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31
ISR_NOERRCODE 128

ISR_NOERRCODE 255

[GLOBAL isr_common_stub]
[EXTERN isr_handler]

isr_common_stub:
	pusha                    ; Pushes edi, esi, ebp, esp, ebx, edx, ecx, eax
	mov ax, ds
	push eax                ; 保存数据段描述符
	
	mov ax, 0x10            ; 加载内核数据段描述符表
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	;mov ss, ax
	
	push esp		; 此时的 esp 寄存器的值等价于 pt_regs 结构体的指针
	call isr_handler        ; 在 C 语言代码里
	add esp, 4 		; 清除压入的参数
	
	pop ebx                 ; 恢复原来的数据段描述符
	mov ds, bx
	mov es, bx
	mov fs, bx
	mov gs, bx
	;mov ss, bx
	
	popa                     ; Pops edi, esi, ebp, esp, ebx, edx, ecx, eax
	add esp, 8               ; 清理栈里的 error code 和 ISR
	iret
.end:

%macro IRQ 2
[GLOBAL irq%1]
irq%1:
	cli
	push 0
	push %2
	jmp irq_common_stub
%endmacro


IRQ   0,	32
IRQ   1,    33	
IRQ   2,    34	
IRQ   3,    35	
IRQ   4,    36	
IRQ   5,    37	
IRQ   6,    38	
IRQ   7,    39	
IRQ   8,    40	
IRQ   9,    41	
IRQ  10,    42	
IRQ  11,    43	
IRQ  12,    44
IRQ  13,    45
IRQ  14,    46	
IRQ  15,    47	


[GLOBAL irq_common_stub]
[EXTERN irq_handler]

irq_common_stub:
	pusha                    ; pushes edi, esi, ebp, esp, ebx, edx, ecx, eax
	
	mov ax, ds
	push eax                 ; 保存数据段描述符
	
	mov ax, 0x10  		 ; 加载内核数据段描述符
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	;mov ss, ax
	
	push esp
	call irq_handler
	add esp, 4
	
	pop ebx                   ; 恢复原来的数据段描述符
	mov ds, bx
	mov es, bx
	mov fs, bx
	mov gs, bx
	;mov ss, bx
	
	popa                     ; Pops edi,esi,ebp...
	add esp, 8     		 ; 清理压栈的 错误代码 和 ISR 编号
	iret          		 ; 出栈 CS, EIP, EFLAGS, SS, ESP
.end:


