OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386)
ENTRY(hatch)
SECTIONS {
    . = 0x200;
    .text ALIGN(0x200) : { *(.text) }
    .data ALIGN(0x200): { *(.data) }
    .bss ALIGN(0x200) : { *(.bss) }
}
