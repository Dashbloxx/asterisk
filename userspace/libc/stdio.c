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

/* Open a file and return a FILE pointer... */
FILE *fopen(const char *filename, const char *mode) {
    if(strcmp(mode, "r")) {
        FILE *fp;
        int fd = open(filename, O_RDONLY, mode);
        /* Check if `open` returned an error (-1). If not, just set the file struct's `fd` to what `open` returned. */
        if(fd != -1) {
            fp->fd = fd;
        }
        else {
            return NULL;
        }
        /* Now, if everything went fine, we can return the file struct... */
        return fp;
    }
    else if(strcmp(mode, "w")) {
        FILE *fp;
        /* This time, we ask `open` to make the file write-only, and to create it if it doesn't exist... */
        int fd = open(filename, O_WRONLY | O_CREAT, mode);
        /* Check if `open` returned an error (-1). If not, just set the file struct's `fd` to what `open` returned. */
        if(fd != -1) {
            fp->fd = fd;
        }
        else {
            return NULL;
        }
        /* Now, if everything went fine, we can return the file struct... */
        return fp;
    }
    else if(strcmp(mode, "r+")) {
        FILE *fp;
        /* This time, we ask `open` to make the file write-only, and to create it if it doesn't exist... */
        int fd = open(filename, O_RDWR, mode);
        /* Check if `open` returned an error (-1). If not, just set the file struct's `fd` to what `open` returned. */
        if(fd != -1) {
            fp->fd = fd;
        }
        else {
            return NULL;
        }
        /* Now, if everything went fine, we can return the file struct... */
        return fp;
    }
    else if(strcmp(mode, "w+")) {
        FILE *fp;
        /* This time, we ask `open` to make the file write-only, and to create it if it doesn't exist... */
        int fd = open(filename, O_RDWR | O_CREAT, mode);
        /* Check if `open` returned an error (-1). If not, just set the file struct's `fd` to what `open` returned. */
        if(fd != -1) {
            fp->fd = fd;
        }
        else {
            return NULL;
        }
        /* Now, if everything went fine, we can return the file struct... */
        return fp;
    }
    else {
        return NULL;
    }
}

/* Close a file by it's `FILE` struct... */
int fclose(FILE *file) {
    return close(file->fd);
}

/* Write to a file by it's `FILE` struct... */
unsigned int fwrite(const void *ptr, unsigned int size, unsigned int count, FILE *stream) {
    unsigned int total_bytes_written = 0;

    /* Calculate the amount of bytes that are to be written... */
    unsigned int num_bytes_to_write = size * count;

    int bytes_written = write(stream->fd, ptr, num_bytes_to_write);

    /* Return an error of the amount if bytes written are less than zero... */
    if (bytes_written < 0) {
        return -1;
    }

    /* Update and return the amount of bytes that were written... */
    total_bytes_written += bytes_written;
    return total_bytes_written;
}

/* Read from file using `FILE` type... */
unsigned int fread(void *ptr, unsigned int size, unsigned int count, FILE *stream) {
    /* Calculate the amount of total bytes by multiplying the size of items by count of items... */
    unsigned int total_bytes = size * count;
    unsigned int bytes_read = 0;
    char *buffer = (char *) ptr;

    /* Get unique number that represents file... */
    int fd = stream->fd;

    while (bytes_read < total_bytes) {
        unsigned int remaining_bytes = total_bytes - bytes_read;
        int result = read(fd, buffer + bytes_read, remaining_bytes);

        if (result < 0) {
            break;
        }

        bytes_read += result;

        if (result == 0) {
            break;
        }
    }

    return bytes_read / size;
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
    va_start(args, format);
    int chars_written = 0;

    while (*format != '\0') {
        if (*format == '%') {
            format++;

            // Handle format specifier
            switch (*format) {
                case 'c': {
                    char c = (char) va_arg(args, int);
                    chars_written += write(1, &c, 1);
                    break;
                }
                case 's': {
                    char *str = va_arg(args, char *);
                    chars_written += write(1, str, strlen(str));
                    break;
                }
                case 'd': {
                    int num = va_arg(args, int);
                    char buffer[20];
                    int i = 0;
                    if (num == 0) {
                        buffer[i++] = '0';
                    }
                    else if (num < 0) {
                        chars_written += write(1, "-", 1);
                        num = -num;
                    }
                    while (num > 0) {
                        buffer[i++] = (num % 10) + '0';
                        num /= 10;
                    }
                    buffer[i] = '\0';

                    // Reverse the buffer
                    for (int j = 0, k = i - 1; j < k; j++, k--) {
                        char temp = buffer[j];
                        buffer[j] = buffer[k];
                        buffer[k] = temp;
                    }

                    chars_written += write(1, buffer, strlen(buffer));
                    break;
                }
                default:
                    // Unknown format specifier, ignore it
                    break;
            }

        } else {
            // Regular character
            chars_written += write(1, format, 1);
        }

        format++;
    }

    va_end(args);
    return chars_written;
}

char *gets(char *s) {
    char c;
    int i = 0;

    while (read(0, &c, 1) == 1 && c != '\n') {
        s[i++] = c;
    }

    s[i] = '\0';

    return s;
}