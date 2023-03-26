#include "stdio.h"
#include "syscall.h"

void puts(const char *text) {
    write(1, text, strlen(text));
}

void putc(char character) {
    write(1, &character, 1);
}

char getchar() {
    char c;
    read(1, &c, 1);
    return c;
}