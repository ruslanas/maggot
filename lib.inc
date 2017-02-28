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
	push eax
	mov [0xb8000 + ebx], eax
	pop eax
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