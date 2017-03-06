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


/*
Entry point
*/
void hatch() {

    int status = read_port(0x64);

    char ss[32];
    itoa(status, ss, 10);
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
