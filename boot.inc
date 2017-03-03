use16
;;;;;;;;;;;;;;;;;
; Load sector 2 ;
;;;;;;;;;;;;;;;;;

org 0x7c00
start:

mov cx, 3				      ; retry 3 times
load_kernel:

	xor ah, ah			      ; reset disk system
	int 0x13

	mov ax, ds
	mov es, ax
	mov bx, _kernel_base		      ;
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
	lgdt [gdtr]		   ; load GDT register
	lidt [idtr]		   ; load IDT register

	mov eax, cr0
	or eax, 1
	mov cr0, eax				  ; set CRO PE flag (bit 0)

	;smsw ax
	;or ax, 1
	;lmsw ax

	jmp 0x08:__flush

	db start + 446 - $ dup 0			  ; align

	db 0x80 				  ; Active (bootable)
	db 0					  ; Head	 (H)
	db 0					  ; Sector	 (S)
	db 0					  ; Cylinder	 (C)
	db 0x7f 				  ; System ID
	db 1					  ; Last H
	db 00011111b				  ; Last S  (31) C9C8  S5....S0
	db 00011111b				  ; Last C  (31)     C7......C0
	dd 0					  ; LBA
	dd 0x800				  ; Number of sectors (1MB/512kb)

	db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0	  ; unused
	db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0

	db 0x55, 0xAA			      ; signature
