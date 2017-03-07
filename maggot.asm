; (c) 2017 Ruslanas Balciunas

include 'includes/macros.inc'

__kernel_base equ 0x600

include "includes/mbr.inc"				   ; 512 bytes MBR

use32
org __kernel_base
log_addr 'Base: 0x'

__protected_mode:
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	jmp 0x18:__test_program			  ; jump to user space
__recover:
	_macro_message warning
__wait:
	hlt

warning db 'Some error occured!',0

include "includes/idt.inc"
include "includes/gdt.inc"
include "includes/isr.inc"
include "includes/lib.inc"

db 1024 * 4 - ($ - $$) - 0x200 dup 0   ; align 4KB

base = 0x8000                         ; relocate 4 sectors and jump to entry
__test_program:

	; ELF header
	virtual at esi
		ELFMAG db 4 dup ? ; 0x7f ELF
		EI_CLASS db ?
		EI_DATA db ?
		EI_OSABI db ?
		EI_VERSION db ?
		EI_PAD db 8 dup ?
		e_type dw ?
		e_machine dw ?
		e_version dd ?
		e_entry dd ?
	end virtual

	mov esi, offset
	mov ax, [e_type]
	cmp ax, 2
	jne @f
	_macro_message msg_exe
	@@:

	mov ax, [e_machine]
	cmp ax, 3
	jne @f
	_macro_message msg_i386
	@@:

	mov eax, [e_version]
	cmp eax, 1
	jne @f
	_macro_message msg_current
	@@:

	_relocate 0x800, offset, base    ; .text

	mov eax, base
	add eax, [e_entry]
	call eax
	_macro_message msg_terminated

	.end_program:
	call exit

msg_terminated db 'Program terminated',0
msg_exe        db 'Executable',0
msg_i386       db 'Intel 80386',0
msg_current    db 'Current version',0

db 1024 * 8 - ($ - $$) - 0x200 dup 0   ; align 8KB
offset = $                    ; sys image offset from end of MBR
