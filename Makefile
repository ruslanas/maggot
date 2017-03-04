CCPARAMS=-nostdinc -m32
CC=gcc
VBM=VBoxManage
QEMU=qemu-system-i386
all: clean larva.img
larva.img: maggot.bin egg.img
	cat maggot.bin egg.img > larva.img
	truncate -s 1048576 larva.img
egg.img: lib.o linker.ld
	ld -T linker.ld lib.o -o egg.img
lib.o: lib.c
	$(CC) $(CCPARAMS) -c lib.c -o lib.o
maggot.bin: maggot.asm
	fasm maggot.asm
clean:
	rm --force lib.o maggot.bin egg.img larva.img
hatch: larva.img
	$(QEMU) -d guest_errors -drive format=raw,file=larva.img
vdi: larva.img
	$(VBM) convertdd larva.img larva.vdi --format VDI
