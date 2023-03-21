#include "stdio.h"
#include "syscall.h"
#include "string.h"
#include "stdarg.h"

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

void printf(const char *format, ...) {
    char **arg = (char **)&format;
    int c;
    char buf[32];

    arg++;

    memset(buf, 0, sizeof(buf));
    while ((c = *format++) != 0) {
        if (c != '%')
            putc(c);
        else {
            char *p, *p2;
            int pad0 = 0, pad = 0;

            c = *format++;
            if (c == '0') {
                pad0 = 1;
                c = *format++;
            }

            if (c >= '0' && c <= '9') {
                pad = c - '0';
                c = *format++;
            }

            switch (c) {
                case 'd':
                case 'u':
                case 'x':
                    itoa(buf, c, *((int *)arg++));
                    p = buf;
                    goto string;
                    break;

                case 's':
                    p = *arg++;
                    if (!p)
                        p = "(null)";

                string:
                    for (p2 = p; *p2; p2++)
                        ;
                    for (; p2 < p + pad; p2++)
                        putc(pad0 ? '0' : ' ');
                    while (*p)
                        putc(*p++);
                    break;

                default:
                    putc(*((int *)arg++));
                    break;
            }
        }
    }
}