; Global descripor table
gdtr:
	dw gdt_end - gdt - 1
	dd gdt

gdt:
	db 0, 0, 0, 0, 0, 0, 0, 0

	;;;;;;;;;;;;;;;;
	; kernel space ;
	;;;;;;;;;;;;;;;;
	dw 0xffff
	dw 0
	db 0
	db 10011010b
	db 11001111b
	db 0

	dw 0xffff
	dw 0
	db 0
	db 10010010b
	db 11111111b	  ; xxxx - [xxxx
	db 0		  ; x] - xxxxxxx

	;;;;;;;;;;;;;;
	; user space ;
	;;;;;;;;;;;;;;
	dw 0xffff
	dw 0
	db 0
	db 10011010b
	db 11001111b
	db 0

	dw 0xffff
	dw 0
	db 0
	db 10010010b
	db 11111111b
	db 0

gdt_end:
