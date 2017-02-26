db '__START__IDT__'

macro gate_entry {
	dw 0x6000 - page1 + exception; offset_bits 0..15
	dw 8 ; code segment selector in GDT
	db 0 ; 0 - unused;
	db 11100111b ; type and attributes of the descriptor
	; 1110 0xe - 80386 32-bit interrupt gate
	; 0
	; 00 - call protection
	; 1
	dw 0 ; offset bits 16..31
}

idtr:
	dw end_idt - idt
	dd 0x6000 + idt - page1
idt:
	gate_entry
	gate_entry
	gate_entry
	gate_entry
	gate_entry
	gate_entry
	gate_entry
	gate_entry
	gate_entry
	gate_entry
	gate_entry
	gate_entry
	gate_entry
	gate_entry	      ; 0xD general protection fault #GP
end_idt:
db '__END__IDT__'
dw 0x6000 + exception - page1
exception: ; ISR
	mov eax, 0x777 ; ! i'm here
	vga_char 'X'
	;jmp $ + 0x6000 - page1
	;;;;;;;;;;;;;;;;;;;;;;;;;
	; Proper return
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	iret

base_addr dw 0x6000
