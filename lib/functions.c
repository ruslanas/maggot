#include "functions.h"

void strcat(char * dest, char * src) {
    int len = strlen(dest);
    int i = 0;
    for(;src[i]!='\0';i++) {
        dest[len+i-1] = src[i];
    }
    dest[i] = '\0';
}

int strlen(char * str) {
    int len = 0;
    for(int i=0;str[i]!='\0';i++) {
        len++;
    }
    return len;
}

void reverse(char * str) {
    int len = strlen(str);
    for(int i=0;i<len/2;i++) {
        char tmp = str[i];
        str[i] = str[len-1-i];
        str[len-1-i] = tmp;
    }
}

char * itoa(int num, char * str, int base) {
    int i = 0;
    short isNegative = 0;
    if(num == 0) {
        str[i++] = '0';
        str[i] = '\0';
    }
    if(num<0 && base==10) {
        isNegative = 1;
        num = -num;
    }

    while(num!=0) {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'A' : rem + '0';
        num = num / base;
    }

    if(isNegative) {
        str[i++] = '-';
    }
    str[i] = '\0';
    reverse(str);
}
