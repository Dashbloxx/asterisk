#include "stdio.h"
#include "syscall.h"
#include "stdlib.h"
#include "stdint.h"
#include "stdarg.h"
#include "errno.h"

FILE *stderr;
FILE *stdin;
FILE *stdout;

/*
 *  Functions related to file I/O...
 */

FILE *fopen(const char *filename, const char *mode) {
    FILE *fp = malloc(sizeof(FILE));
    if(mode == "r") {
        fp->fd = open(filename, O_RDONLY);
    }
    else if(mode = "w") {
        fp->fd = open(filename, O_WRONLY);
    }
    else if(mode == "r+") {
        fp->fd = open(filename, O_RDWR);
    }
    return fp;
}

void fclose(FILE *file) {
    close(file->fd);
}

size_t fwrite(const void *ptr, size_t size, size_t count, FILE *stream) {
    int write_output = write(stream->fd, ptr, count);
    return count;
}

void fread(void *ptr, size_t size, size_t count, FILE *stream) {
    read(stream->fd, ptr, count);
}

int fputc(int c, FILE *stream) {
    unsigned char ch = (unsigned char)c;
    int result = write(stream->fd, &ch, 1);
    return result;
}

int fgetc(FILE *stream) {
    unsigned char c;
    int result = read(stream->fd, &c, 1);

    if (result != 1) {
        return -1;
    }

    return c;
}

char *fgets(char *str, int n, FILE *stream) {
    int i = 0;
    int c;

    while (i < n - 1) {
        c = fgetc(stream);
        if (c == -1) {
            break;
        } else if (c == '\n') {
            str[i++] = c;
            break;
        } else {
            str[i++] = c;
        }
    }

    str[i] = '\0';

    if (i == 0 && c == -1) {
        return NULL;
    }

    return str;
}

int fprintf(FILE *stream, const char *format, ...) {
    va_list args;
    va_start(args, format); // initialize the variable argument list

    int n = 0; // number of characters written
    char c;
    while ((c = *format++) != '\0') {
        if (c == '%') {
            c = *format++; // read the next character after '%'
            if (c == '\0') {
                break;
            }
            if (c != 's' && c != 'd' && c != 'c') {
                // unsupported format specifier
                return -1;
            }
            if (c == 's') {
                char *s = va_arg(args, char *);
                while (*s != '\0') {
                    if (fputc(*s, stream) == -1) {
                        return -1; // error writing character
                    }
                    s++;
                    n++;
                }
            } else if (c == 'd') {
                int d = va_arg(args, int);
                char buffer[20]; // large enough to hold any int
                int i = 0;
                if (d < 0) {
                    buffer[i++] = '-';
                    d = -d;
                }
                do {
                    buffer[i++] = (d % 10) + '0';
                    d /= 10;
                } while (d > 0);
                while (--i >= 0) {
                    if (fputc(buffer[i], stream) == -1) {
                        return -1; // error writing character
                    }
                    n++;
                }
            } else if (c == 'c') {
                char ch = va_arg(args, int);
                if (fputc(ch, stream) == -1) {
                    return -1; // error writing character
                }
                n++;
            }
        } else {
            if (fputc(c, stream) == -1) {
                return -1; // error writing character
            }
            n++;
        }
    }

    va_end(args); // cleanup the variable argument list

    return n; // return number of characters written
}

/*
 *  Functions related to terminal I/O...
 */

int putc(int c) {
    unsigned char ch = (unsigned char)c;
    int result = write(stdout->fd, &ch, 1);
    return result;
}

int getc() {
    unsigned char c;
    int result = read(stdin, &c, 1);

    if (result != 1) {
        return -1;
    }

    return c;
}

int printf(const char *format, ...) {
    va_list args;
    va_start(args, format); // initialize the variable argument list

    int n = 0; // number of characters written
    char c;
    while ((c = *format++) != '\0') {
        if (c == '%') {
            c = *format++; // read the next character after '%'
            if (c == '\0') {
                break;
            }
            if (c != 's' && c != 'd' && c != 'c') {
                // unsupported format specifier
                return -1;
            }
            if (c == 's') {
                char *s = va_arg(args, char *);
                while (*s != '\0') {
                    if (fputc(*s, stdout) == -1) {
                        return -1; // error writing character
                    }
                    s++;
                    n++;
                }
            } else if (c == 'd') {
                int d = va_arg(args, int);
                char buffer[20]; // large enough to hold any int
                int i = 0;
                if (d < 0) {
                    buffer[i++] = '-';
                    d = -d;
                }
                do {
                    buffer[i++] = (d % 10) + '0';
                    d /= 10;
                } while (d > 0);
                while (--i >= 0) {
                    if (fputc(buffer[i], stdout) == -1) {
                        return -1; // error writing character
                    }
                    n++;
                }
            } else if (c == 'c') {
                char ch = va_arg(args, int);
                if (fputc(ch, stdout) == -1) {
                    return -1; // error writing character
                }
                n++;
            }
        } else {
            if (fputc(c, stdout) == -1) {
                return -1; // error writing character
            }
            n++;
        }
    }

    va_end(args); // cleanup the variable argument list

    return n; // return number of characters written
}

char *gets(char *str, int n) {
    int i = 0;
    int c;

    while (i < n - 1) {
        c = fgetc(stdin);
        if (c == -1) {
            break;
        } else if (c == '\n') {
            str[i++] = c;
            break;
        } else {
            str[i++] = c;
        }
    }

    str[i] = '\0';

    if (i == 0 && c == -1) {
        return NULL;
    }

    return str;
}