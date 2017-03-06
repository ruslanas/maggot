/*
We are hacking here, you know. Expect a lot of crap.
*/

#include "lib/functions.h"

unsigned volatile short * video = (unsigned volatile short *) 0xb8000;
char gX[20] = "___data___";

int printAt(int x, int y, const char str[]) {

    for(int i=0;str[i] != '\0';i++) {
        video[y*80 + x+i] = 0x0F00 | str[i];
    }

    return 0;
}

int read_port(int x) {
    int r = 0;
    __asm__(
        "mov %0, %%edx\n\t"
        "in %%dx, %1\n\t"
        :"=r"(r) // output operand
        :"r"(x)  // input operand
    );
    return r;
}

int write_port(int port, unsigned char data) {
    __asm__(
        "mov %0, %%edx\n\t"
        "mov %1, %%al\n\t"
        "out %%al, %%dx\n\t"
        :
        :"r"(port), "r"(data)
    );
}

void clear_screen() {
    char space[] = " ";
    for(int i=0;i<80;i++) {
        for(int j=0;j<43;j++) {
            printAt(i, j, space);
        }
    }
}

/*
Entry point
*/

void hatch() {

    char buffer[1024] = "Press ESC to continue...";
    printAt(0, 1, buffer);

    while(1) {
        int i = read_port(0x60);
        i = i & 0x000000FF;
        itoa(i, buffer, 10);
        printAt(1,0, buffer);
        if(i == 129) {
            break;
        }
    }

    clear_screen();

    // ICW1 (Initialization Command Word One)
    write_port(0x20, 0x11);
    write_port(0xA0, 0x11);

    // remap
    write_port(0x21, 0x20);
    write_port(0xA1, 0x28);

    // ICW3 - cascading
    write_port(0x21, 0x00);
    write_port(0xA1, 0x00);

    // ICW4 - setup cascading
    write_port(0x21, 0x01);
    write_port(0xA1, 0x01);

    // mask interrupt
    write_port(0x21, 0xFF);
    write_port(0xA1, 0xFF);

    write_port(0x21, 0xFD);
    write_port(0x20, 0x20);

    int status = read_port(0x64);

    char ss[32];
    itoa(0xABCDEF, ss, 16);
    printAt(2, 11, ss);

    char addr[1024];
    itoa((int)&gX, addr, 16);
    printAt(2, 7, addr);
    printAt(2, 8, gX); // + relocation offset
    itoa((int)&hatch, addr, 16);
    printAt(2, 9, addr);

    char str[] = "Maggot 0.0.1";
    printAt(2, 1, str);
}
