OUTPUT_ARCH(i386)
SECTIONS {
    . = 0x2000;
    .text : { *(.text) }
    .data ALIGN(0x200) : { *(.data) }
    .bss ALIGN(0x200) : { *(.bss) }
}
