macro display_char char {
      mov al, char
      mov ah, 0x0e
      mov bh, 0x00
      mov bl, 0x07
      int 0x10
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

macro log_addr msg {
    bits = 32
    display msg
    repeat bits / 4
        d = '0' + $ shr (bits - %*4) and 0fh
        if d > '9'
            d = d + 'A' - '9' - 1
        end if
        display d
    end repeat
    display 13,10
}

macro message msg {
    lea esi, [msg]
    call print
}
