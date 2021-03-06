use16
org 0x7c00

mov cx, 3				      ; retry 3 times
__load_kernel:

	xor ah, ah			      ; reset disk system
	int 0x13

	mov ax, ds
	mov es, ax
	mov bx, __kernel_base
	mov dl, 0x80			   ; first drive
	mov dh, 0			       ; head
	mov al, 32			       ; number of sectors to read

	push cx
	mov ch, 0
	mov cl, 2			      ; sector number (1-17)
	mov ah, 0x2
	int 0x13
	jnc __housekeeping			; execute

	pop cx
	dec cx
	jne __load_kernel			; loop end

__housekeeping:

	; load memory map for later usage
	; at 0x8000

	mov ecx, 24
	mov edx, dword[magic]
	mov di, 0x8000
	xor ebx, ebx

	@@:
	mov eax, 0x0000E820
	int 0x15
	add di, cx
	mov ecx, 20

	cmp ebx, 0
	jne @r

	bios_print success_msg

	jmp __switch_mode

magic dd 'PAMS' 			  ; system memory map
success_msg db 'Egg!',0

__switch_mode:

	mov ax,3				  ; VGA text mode
	int 0x10

	; https://software.intel.com/en-us/articles/intel-sdm
	; 9.8 software initialization for protected mode

	cli
	lgdt [gdtr]				  ; load GDT register
	lidt [idtr]				  ; load IDT register

	mov eax, cr0
	or eax, 1
	mov cr0, eax				  ; set CRO PE flag (bit 0)

	;smsw ax
	;or ax, 1
	;lmsw ax

	jmp 0x08:__protected_mode

	db ($$ - $) + 446 dup 0	  ; align

	db 0x80 				  ; Active (bootable)
	db 0					  ; Head	 (H)
	db 0					  ; Sector	 (S)
	db 0					  ; Cylinder	 (C)
	db 0x7f 				  ; System ID
	db 1					  ; Last H
	db 00011111b			  ; Last S  (31) C9C8  S5....S0
	db 00011111b			  ; Last C  (31)     C7......C0
	dd 0					  ; LBA
	dd 0x800				  ; Number of sectors (1MB/512kb)

	db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0	  ; unused
	db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0

	db 0x55, 0xAA				  ; signature
