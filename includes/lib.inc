exit:
	mov al, 1
	mov ebx, 0
	int 0x80		  ; system interrupt
	ret
fork:
	mov al, 2
	int 0x80
	ret

print:
	mov edi, 0
	.l:
	mov ah, byte[esi]
	cmp ah, 0
	je .end
	mov byte[0xb8000 + edi * 2], ah
	mov byte[0xb8000 + edi * 2 + 1], 0x1F
	inc esi
	inc edi
	jmp .l
	.end:
	ret

read:
	push eax
	push ebx
	mov eax, 0x0f250f25
	mov ebx, 0x10
	call print
	pop ebx
	pop eax
	ret