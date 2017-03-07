/*
We are hacking here, you know. Expect a lot of crap.
*/

#include "lib/functions.h"

unsigned volatile short * video = (unsigned volatile short *) 0xb8000;
char gX[22] = "___Great_day_today___";

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

void sleep() {
    char buffer[16];
    int code = 0;
    while(code != 129) {
        code = read_port(0x60); // poll
        code = code & 0x000000FF;
        itoa(code, buffer, 10);
        printAt(39, 12, buffer);
    }

}

/*
Entry point
*/

int hatch() {

    char buffer[64] = "Press ESC to continue...";
    printAt(0, 1, buffer);

    int (*stderr)(const char *);
    stderr = (int (*)(const char *))(0xB9D); // address found in make output

    sleep();

    clear_screen();

    stderr(gX);

    char str[] = "Maggot 0.0.1";
    printAt(2, 1, str);

    return 0;
}
