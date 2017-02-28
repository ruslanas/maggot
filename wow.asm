include 'macros.inc'

;format ELF
;section '.text' writable executable

_kernel_base equ 0x6000

include 'boot.inc'				  ; 512 bytes

	mov ax,1				  ; VGA text mode
	int 0x10

	; https://software.intel.com/en-us/articles/intel-sdm
	; 9.8 software initialization for protected mode

	cli
	lgdt [gdtr + _kernel_base]		  ; load GDT register
	lidt [idtr + _kernel_base]		  ; load IDT register

	mov eax, cr0
	or eax, 1
	mov cr0, eax				  ; set CRO PE flag (bit 0)

	;smsw ax
	;or ax, 1
	;lmsw ax

	jmp 0x08:__flush + _kernel_base

use32

__flush:
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
	call read
	call fork
	call exit

include 'lib.inc'

db 1024 * 4 - $ dup 0				  ; align
db 1020 * 1024 dup 0				  ; vboxmanage wants bigger files