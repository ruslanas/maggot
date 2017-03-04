OUTPUT_FORMAT(binary)
OUTPUT_ARCH(i386)
SECTIONS {
    . = 0x2000;
    .text : { *(.text) }
    .data : { *(.data) }
    .bss ALIGN(0x200) : { *(.bss) }
}
