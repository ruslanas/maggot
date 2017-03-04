CCFLAGS=-nostdinc -m32
CC=gcc
VBM=VBoxManage
QEMU=qemu-system-i386
all: clean larva.img
larva.img: maggot.bin egg.img
	cat maggot.bin egg.img > larva.img
	truncate --size 1048576 larva.img
egg.img: lib.o linker.ld
	ld --script linker.ld lib.o --output egg.img
lib.o: lib.c
	$(CC) $(CCFLAGS) -c lib.c -o lib.o
maggot.bin: maggot.asm includes/mbr.inc includes/lib.inc includes/macros.inc
	fasm maggot.asm
clean:
	rm --force lib.o maggot.bin egg.img larva.img
hatch: larva.img
	$(QEMU) -d guest_errors -drive format=raw,file=larva.img
vdi: larva.img
	rm --force larva.vdi
	$(VBM) convertdd larva.img larva.vdi --format VDI
