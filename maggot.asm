
include 'macros.inc'

;format ELF
;section '.text' writable executable

_kernel_base equ 0x5000

include 'boot.inc'				  ; 512 bytes

use32
org _kernel_base
kernel_start:

__flush:
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	jmp 0x18:test_program	   ; jump to user space
__recover:
	lea esi, byte[oops]
	call print
	jmp $
__success:
	jmp $
include "idt.inc"
include "gdt.inc"

test_program:
	lea esi, byte[hello]
	call print

	;mov eax, 0				  ; test interrupt service routines
	;div eax				  ; divide by zero
	;dw 0xFFFF				  ; invalid op
	;jmp 0x24:0xffff			   ; tts

	;call fork
	call exit

hello	db 'Protected mode',0
done	db 'Task completed!!!',0
oops	db 'Some error occured!',0

include 'lib.inc'

db kernel_start + 1024 * 4 - $ dup 0				 ; align
db 1020 * 1024 + 512 dup 0				; vboxmanage wants bigger files