; Interrupt Service Routines

breakpoint_irs:
	cli
	lea esi, [msg_breakpoint]
	call print
	call exit
	iret

gp_fault_irs:
	cli
	mov edi, 0xb8000
	lea esi, [gp_fault]
	call print
	call exit
	iret

invalid_tss_irs:
	cli
	lea esi, [tss_error]
	call print
	call exit
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
	jmp .break

	.posix_fork:					   ; stub
		lea esi, [not_implemented]
		call print
		call exit
	.posix_read:					   ; stub
		call read
		jmp .break
	.exit:
		jmp __wait
	.default:
	.break:
		jmp 0x08:__recover

exception:
	cli
	lea esi, [msg_exception]
	call print
	;call exit
	iret

invalid_op      db 'Invalid operation',0
div_error       db 'Division by zero',0
not_implemented db 'Function call not implemented',0
tss_error       db 'TSS fault',0
msg_breakpoint  db 'Breakpoint',0
msg_exception   db 'Unknown error',0
gp_fault        db 'General protection fault',0
