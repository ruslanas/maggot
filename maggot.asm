include 'macros.inc'

;format ELF
;section '.text' writable executable

_kernel_base equ 0x6000

include 'boot.inc'				  ; 512 bytes

use32

macro vga_char arg1 {
	mov edi, 0xb8000
	mov byte [edi + 2], arg1
	mov byte [edi + 3], 00001111b
}

__flush:
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	jmp 0x18:test_program + _kernel_base	  ; jump to user space
__recover:
	mov edi, 0xb8000
	mov dword[edi + 6], 0x0C6b0C4f		  ; Ok
	jmp $

include "idt.inc"
include "gdt.inc"

test_program:
	mov eax, 0x0f210f21
	mov ebx, 0
	call print

	;mov eax, 0				  ; test interrupt service routines
	;div eax				  ; divide by zero
	;dw 0xFFFF				  ; invalid op
	;jmp 0x24:0xffff			   ; tts

	call read
	call fork
	call exit

include 'lib.inc'

db 1024 * 4 - $ dup 0				  ; align
db 1020 * 1024 dup 0				  ; vboxmanage wants bigger files