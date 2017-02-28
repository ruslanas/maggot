use16
;;;;;;;;;;;;;;;;;
; Load sector 2 ;
;;;;;;;;;;;;;;;;;

start:

mov cx, 3				      ; retry 3 times
load_kernel:

	xor ah, ah			      ; reset disk system
	int 0x13

	mov ax, ds
	mov es, ax
	mov bx, _kernel_base + 512	      ;
	mov dl, 0x80			      ; first drive
	mov dh, 0			      ; head
	mov al, 9			      ; number of sectors to read

	push cx
	mov ch, 0
	mov cl, 2			      ; sector number (1-17)
	mov ah, 0x2
	int 0x13
	jnc _kernel_base		      ; execute

	pop cx
	dec cx
	jne load_kernel 		      ; loop end

	display_char 'F'		      ; Fail
	jmp $

	db 446 - $ dup 0		      ; align

	db 0x80,0,00000001b,0,0x7f,0,0,0      ; active partition
	dd 0x0
	dd 0x800

	db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0

	db 0x55, 0xAA			      ; signature
