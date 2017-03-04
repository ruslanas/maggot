
include 'includes/macros.inc'

__kernel_base equ 0x500

include "includes/mbr.inc"				   ; 512 bytes MBR

use32
org __kernel_base

__flush:
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	jmp 0x18:test_program			  ; jump to user space
__recover:
	lea esi, byte[error_message]
	call print
__wait:
	jmp $

error_message	db 'Some error occured!',0

include "includes/idt.inc"
include "includes/gdt.inc"
include "includes/isr.inc"
include "includes/lib.inc"

db 1024 * 4 - ($ - __kernel_base) - 0x200 dup 0   ; align 4KB

test_program:
	lea esi, byte[hello]
	call print

	; call c lib function
	; expect to function at fixed location in memory

	;int 3
	;mov eax, 0				  ; test interrupt service routines
	;div eax				  ; divide by zero
	;dw 0xFFFF				  ; invalid op
	;jmp 0x24:0xffff			  ; tss

	;call fork
	call exit

hello	db 'User space!',0

db 1024 * 8 - ($ - __kernel_base) - 0x200 dup 0   ; align 8KB
