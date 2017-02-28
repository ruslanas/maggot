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
	jnc switch_mode 		     ; execute

	pop cx
	dec cx
	jne load_kernel 		      ; loop end

	display_char 'F'		      ; Fail
	jmp $

switch_mode:

	mov ax,1				  ; VGA text mode
	int 0x10

	; https://software.intel.com/en-us/articles/intel-sdm
	; 9.8 software initialization for protected mode

	cli
	lgdt [gdtr + _kernel_base]		  ; load GDT register
	lidt [idtr + _kernel_base]		  ; load IDT register

	mov eax, cr0
	or eax, 1
	mov cr0, eax				  ; set CRO PE flag (bit 0)

	;smsw ax
	;or ax, 1
	;lmsw ax

	jmp 0x08:__flush + _kernel_base

	db 446 - $ dup 0		      ; align

	db 0x80,0,00000001b,0,0x7f,0,0,0      ; active partition
	dd 0x0
	dd 0x800

	db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0

	db 0x55, 0xAA			      ; signature
