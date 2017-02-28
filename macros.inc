macro display_char arg1 {
      mov al, arg1
      mov ah, 0x0e
      mov bh, 0x00
      mov bl, 0x07

      int 0x10
}

macro vga_char arg1 {
	mov edi, 0xb8000
	mov byte [edi + 2], arg1
	mov byte [edi + 3], 00001111b
}

macro gate_entry segment, offset {
	dw offset; offset_bits 0..15
	dw segment ; code segment selector in GDT
	db 0 ; 0 - unused;
	db 11100111b ; type and attributes of the descriptor
	; 1110 0xe - 80386 32-bit interrupt gate
	; 0
	; 00 - call protection
	; 1
	dw 0 ; offset bits 16..31
}
