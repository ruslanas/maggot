use16
macro sum arg1, arg2
{
      add ax, arg1
      add ax, arg2
}

mov al, 'E'
mov ah, 0x0e
mov bh, 0x00
mov bl, 0x07

int 0x10
jmp $


buffer db 498 dup 0x41
db 0x55, 0xAA

space db 1024 * 1024 dup 0