OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386)
ENTRY(hatch)
SECTIONS {
    . = 0x200;
    .text ALIGN(0x40) : {
        *(.text);
    }
    .data ALIGN(0x40) : {
        video = 0x2400;
        gX = video + 4;
        *(.data);
    }
    .bss ALIGN(0x40) : { *(.bss) }
}
