int printAt(int, int);
int printAt(int x, int y) {

    unsigned char str[] = "Caterpillar!";
    unsigned volatile short * video = (unsigned volatile short *) 0xb8000;
    for(int i=0;str[i] != '\0';i++) {
        video[y*80 + x+i] = 0x2F00 | str[i];
    }

    return 0;
}
void hatch() {
    for(int i=0;i<5;i++) {
        printAt(i + 2, i * 2);
    }
}
