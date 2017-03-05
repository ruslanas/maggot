; (c) 2017 Ruslanas Balciunas

include 'includes/macros.inc'

__kernel_base equ 0x600

include "includes/mbr.inc"				   ; 512 bytes MBR

use32
org __kernel_base

__protected_mode:
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	jmp 0x18:__test_program			  ; jump to user space
__recover:
	message warning
__wait:
	hlt

warning db 'Some error occured!',0

include "includes/idt.inc"
include "includes/gdt.inc"
include "includes/isr.inc"
include "includes/lib.inc"

db 1024 * 4 - ($ - $$) - 0x200 dup 0   ; align 4KB

__test_program:

	; relocate 3 sectors and jump to entry

	_relocate 0x600, 0x1E00 + $$, 0x10000

	; readelf -h egg.img
	; ...
	; Entry point address:               0x27e
	; ...
	call 0x10000 + 0x027e                   ; hatch()

	.end_program: call exit

hello db 'User space!',0

db 1024 * 8 - ($ - $$) - 0x200 dup 0   ; align 8KB
