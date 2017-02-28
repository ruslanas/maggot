; Interrupt Descriptor Table

use32

db '__START__IDT__'
idtr:
	dw end_idt - idt
	dd _kernel_base + idt

idt:
	gate_entry 0x08,  _kernel_base + div_by_zero_irs
	gate_entry 0x08,  _kernel_base + exception
	gate_entry 0x08,  _kernel_base + exception
	gate_entry 0x08,  _kernel_base + breakpoint_irs
	gate_entry 0x08,  _kernel_base + exception
	gate_entry 0x08,  _kernel_base + exception
	gate_entry 0x08,  _kernel_base + invalid_opcode_irs
	gate_entry 0x08,  _kernel_base + exception
	gate_entry 0x08,  _kernel_base + exception
	gate_entry 0x08,  _kernel_base + exception
	gate_entry 0x08,  _kernel_base + invalid_tss_irs   ; 10 - invalid_tss_irs
	gate_entry 0x08,  _kernel_base + exception	   ; 11 - segment not present
	gate_entry 0x08,  _kernel_base + exception
	gate_entry 0x08,  _kernel_base + gp_fault_irs	   ; 0xD general protection fault #GP
	repeat 0x80 - 14
	       gate_entry 0x08,  _kernel_base + exception
	end repeat
	gate_entry 0x08,  _kernel_base + system_interrupt  ; int 0x80
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
	mov dword[edi + 12], 0x0f500f47
	int 0x80
	iret

invalid_tss_irs:
	cli
	vga_char 'T'
	int 0x80
	iret

invalid_opcode_irs:
	cli
	vga_char 'I'
	int 0x80
	iret

div_by_zero_irs:
	cli
	vga_char 'Z'
	int 0x80
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
		mov eax, 0x0f500f50
		mov ebx, 0x0C
		call print
		jmp .break
	.posix_read:					   ; stub
		call read
		jmp .break
	.exit:
	.default:
	.break:
		jmp 0x08:_kernel_base + __recover

exception:
	cli
	vga_char 'X'
	int 0x80
	iret

