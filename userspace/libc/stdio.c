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

/* Print formatted text to a file... */
int fprintf(FILE *stream, const char *format, ...) {
    va_list args;
    va_start(args, format);

    int fd = stream->fd;
    if (fd < 0) {
        return -1;
    }

    char buffer[4096];
    int buffer_pos = 0;

    int result = 0;
    for (; *format != '\0'; format++) {
        if (*format == '%') {
            format++;
            if (*format == 'd') {
                int val = va_arg(args, int);
                int digits = 0;
                if (val == 0) {
                    buffer[buffer_pos++] = '0';
                    digits++;
                }
                else {
                    int tmp = val;
                    while (tmp != 0) {
                        tmp /= 10;
                        digits++;
                    }
                    tmp = val;
                    for (int i = digits - 1; i >= 0; i--) {
                        buffer[buffer_pos + i] = '0' + (tmp % 10);
                        tmp /= 10;
                    }
                }
                buffer_pos += digits;
            }
            else if (*format == 's') {
                const char *str = va_arg(args, const char *);
                int len = strlen(str);
                if (len > 4096 - buffer_pos) {
                    // If the remaining buffer is not enough, flush it first
                    result += write(fd, buffer, buffer_pos);
                    if (result < 0) {
                        return -1;
                    }
                    buffer_pos = 0;
                    if (len > 4096) {
                        // If the string is too long, write it directly to the file descriptor
                        result += write(fd, str, len);
                        if (result < 0) {
                            return -1;
                        }
                    }
                }
                else {
                    // Copy the string to the buffer
                    memcpy(buffer + buffer_pos, str, len);
                    buffer_pos += len;
                }
            }
            else {
                // Unsupported format specifier, return -1
                return -1;
            }
        }
        else {
            // Copy the character to the buffer
            buffer[buffer_pos++] = *format;
            if (buffer_pos == 4096) {
                // Flush the buffer if it's full
                result += write(fd, buffer, buffer_pos);
                if (result < 0) {
                    return -1;
                }
                buffer_pos = 0;
            }
        }
    }

    // Flush the remaining buffer
    if (buffer_pos > 0) {
        result += write(fd, buffer, buffer_pos);
        if (result < 0) {
            return -1;
        }
    }

    va_end(args);
    return result;
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