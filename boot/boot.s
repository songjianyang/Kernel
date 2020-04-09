
[bits 32]
section .text
 		align 4
        dd 0x1BADB002            ;magic
        dd 0x00                  ;flags
        dd - (0x1BADB002 + 0x00) ;checksum. m+f+c should be zero

[GLOBAL start]
[extern kmain]


start:
	cli
	call kmain
	jmp $

;stop:
; hlt
;	jmp stop


