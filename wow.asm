use16
macro display_char arg1 {
      mov al, arg1
      mov ah, 0x0e
      mov bh, 0x00
      mov bl, 0x07

      int 0x10
}
macro vga_char arg1 {
	mov edi, 0xb8000
	mov byte [edi], arg1
	mov byte [edi + 1], 00001111b
}

;;;;;;;;;;;;;;;;;
; Load sector 2 ;
;;;;;;;;;;;;;;;;;


mov cx, 3
retry_start:
	xor ah, ah
	int 0x13

	mov ax, ds
	mov es, ax
	mov bx, 0x6000 ; 0x7c00 ; 0xb8000 has to be in protected mode
	mov dl, 0x80 ; first drive
	mov dh, 0
	mov al, 1
	mov ch, 0
	mov cl, 2     ; second sector
	mov ah, 0x2
	int 0x13
	jnc 0x6000		       ; execute
	loop retry_start

	display_char 'F'
	jmp $

	db 510 - $ dup 0 ; align
	db 0x55, 0xAA		; signature
;;;;;;;;;;;;;;;;;
; second sector ;
;;;;;;;;;;;;;;;;;

page1:
;-------------------------------------------------------------------------------
	display_char '!'
	;jmp $

; VGA text mode
mov ax,3
int 0x10

;jmp $

;;;;;;;;;;;;;;;;;;
; protected mode ;
;;;;;;;;;;;;;;;;;;


	cli
	lgdt [gdtr + 0x6000 - page1]  ; load GDT register
	lidt [idtr + 0x6000 - page1]  ; load IDT register

	mov eax, cr0
	or eax, 1
	mov cr0, eax	    ; set CRO PE bit to 1

	smsw ax
	or ax, 1
	lmsw ax

	jmp 8:0x6000 - page1 + main
	jmp 8:0x6000 - page1 + program

	db '__START_GDT__' ; marker
gdtr:
	dw gdt_end - gdt - 1
	dd 0x6000 + gdt - page1

gdt:
	db 0, 0, 0, 0, 0, 0, 0, 0

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
	db 11001111b
	db 0
gdt_end:
	db '__END_GDT__'

use32
main:

	vga_char '('

	;; generate fault
	int 3
	jmp $

	ret

program:
	mov edi, 0xb8000
	mov byte [edi], ':'
	mov byte [edi + 1], 00001111b
	mov byte [edi + 2], ')'
	mov byte [edi + 3], 00001111b
	jmp $

	ret

greet:
	hello_str db 'Protected Mode!',0 ; null terminated string
include "idt.inc"

db 1024 - $ dup 0		 ; align
;-------------------------------------------------------------------------------
page2:
;-------------------------------------------------------------------------------
db 1023 * 1024 dup 0		 ; vboxmanage wants bigger files