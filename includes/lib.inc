exit:
	mov al, 1
	mov ebx, 0
	int 0x80		  ; system interrupt
	ret
fork:
	mov al, 2
	int 0x80
	ret

log_addr 'print: 0x'
print:
	mov edi, 0

	.put_char:
	mov ah, byte[esi]
	cmp ah, 0
	je .exit
	mov byte[0xb8000 + edi], ah
	mov byte[0xb8000 + edi + 1], 0x1F
	inc esi
	add edi, 2
	jmp .put_char

	.exit: ret

read:
	push eax
	push ebx
	mov eax, 0x0f250f25
	mov ebx, 0x10
	call print
	pop ebx
	pop eax
	ret
