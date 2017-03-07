exit:
	mov al, 1
	mov ebx, 0
	int 0x80		  ; system interrupt
	ret

fork:
	mov al, 2
	int 0x80
	ret

log_addr 'print(): 0x'
print:
	push ebp
	mov ebp, esp
	sub esp, 12
	push esi
	push edi
	push edx

	mov esi, [ebp + 4 + 4]
	mov edi, 0
	.put_char:
	mov dh, byte[esi]
	cmp dh, 0
	je .exit

	mov byte[0xb8000 + 24 * 80 * 2 + edi], dh
	mov byte[0xb8001 + 24 * 80 * 2 + edi], 0x1F

	add edi, 2
	inc esi
	jmp .put_char

	.exit:
	pop edx
	pop edi
	pop esi
	mov esp, ebp
	pop ebp
	ret

line dd 11
msg_not_implemented db 'Not implemented',0
read:
	mov ebp, esp
	lea eax, [msg_not_implemented]
	push eax
	call print
	pop eax
	mov esp, ebp
	ret
