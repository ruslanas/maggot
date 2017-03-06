CCFLAGS=-nostdinc -nostdlib -fno-builtin -m32
CC=gcc
VBM=VBoxManage
QEMU=qemu-system-i386
FILES=gdt.inc idt.inc isr.inc lib.inc macros.inc mbr.inc
INCLUDES=$(FILES:%=includes/%)

all: clean larva.img

maggot.bin: maggot.asm $(INCLUDES)
	fasm maggot.asm
egg.img: sys.o linker.ld
	ld --script linker.ld sys.o functions.o --output egg.img
larva.img: maggot.bin egg.img
	cat maggot.bin egg.img > larva.img
	truncate --size 1048576 larva.img
sys.o: sys.c functions.o
	$(CC) $(CCFLAGS) -c sys.c -o sys.o
functions.o: lib/functions.c
	$(CC) $(CCFLAGS) -c lib/functions.c -o functions.o
	
clean:
	rm --force *.o maggot.bin *.img
hatch: larva.img
	$(QEMU) -d guest_errors -drive format=raw,file=larva.img
vdi: larva.img
	rm --force larva.vdi
	$(VBM) convertdd larva.img larva.vdi --format VDI
