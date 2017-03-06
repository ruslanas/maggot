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

    // __asm__("cli");
    // __asm__("sti");
    int code = 0;
    while(code != 129) {
        code = read_port(0x60); // poll
        code = code & 0x000000FF;
        itoa(code, buffer, 10);
        printAt(1,0, buffer);

        // write_port(0x70, 0x80 & 0xA);    // select CMOS register
        // write_port(0x71, 0x20); // write to CMOS/RTC RAM
        // int t = read_port(0x71);
        // itoa(t, buffer, 10);
        // printAt(1, 4, buffer);
    }

    clear_screen();

    itoa((int)&gX, buffer, 16);
    printAt(2, 7, buffer);
    printAt(2, 8, gX);
    itoa((int)&hatch, buffer, 16);
    printAt(2, 9, buffer);

    char str[] = "Maggot 0.0.1";
    printAt(2, 1, str);
}
