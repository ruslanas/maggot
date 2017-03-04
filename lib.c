int hatch() {
    unsigned volatile short * video = (unsigned volatile short *) 0xb8000;
    unsigned char * str = "Caterpillar\0";

    for(int i=0;str[i] != '\0';++i) {
        video[i] = (video[i] & 0xFF00) | 'A';
    }

    return 0;
}
