; Interrupt Descriptor Table

use32

db '__START__IDT__'
idtr:
	dw end_idt - idt
	dd idt

idt:
	gate_entry 0x08,  div_by_zero_irs
	gate_entry 0x08,  exception
	gate_entry 0x08,  exception
	gate_entry 0x08,  breakpoint_irs
	gate_entry 0x08,  exception
	gate_entry 0x08,  exception
	gate_entry 0x08,  invalid_opcode_irs
	gate_entry 0x08,  exception
	gate_entry 0x08,  exception
	gate_entry 0x08,  exception
	gate_entry 0x08,  invalid_tss_irs   ; 10 - invalid_tss_irs
	gate_entry 0x08,  exception	    ; 11 - segment not present
	gate_entry 0x08,  exception
	gate_entry 0x08,  gp_fault_irs	    ; 0xD general protection fault #GP
	repeat 0x80 - 14
	       gate_entry 0x08, exception
	end repeat
	gate_entry 0x08, system_interrupt  ; int 0x80
end_idt:
db '__END__IDT__'

; Interrupt Service Routines

breakpoint_irs:
	cli
	vga_char 'b'
	jmp $
	iret

gp_fault_irs:
	cli
	mov edi, 0xb8000
	mov byte[edi + 14], 0x47
	mov byte[edi + 15], 0x0f
	int 0x80
	iret

invalid_tss_irs:
	cli
	vga_char 'T'
	int 0x80
	iret

invalid_opcode_irs:
	cli
	lea esi, [invalid_op]
	call print
	call exit
	iret

div_by_zero_irs:
	cli
	lea esi, [div_error]
	call print
	call exit
	iret

system_interrupt:
	cli

	cmp al, 1
	je .exit

	cmp al, 2
	je .posix_fork

	cmp al, 3
	je .posix_read

	.posix_fork:					   ; stub
		lea esi, [not_implemented]
		call print
		call exit
	.posix_read:					   ; stub
		call read
		jmp .break
	.exit:
		jmp __success
	.default:
	.break:
		jmp 0x08:__recover

exception:
	cli
	vga_char 'X'
	int 0x80
	iret

invalid_op	db 'Invalid operation',0
div_error	db 'Division by zero',0
not_implemented db 'Function call not implemented',0
