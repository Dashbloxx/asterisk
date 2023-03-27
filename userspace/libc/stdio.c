#include "stdio.h"
#include "syscall.h"
#include "stdlib.h"
#include "stdint.h"
#include "stdarg.h"
#include "errno.h"

FILE *stderr;
FILE *stdin;
FILE *stdout;

FILE *fopen(const char *filename, const char *mode) {
    FILE *fp = _malloc(sizeof(FILE));
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

int fflush(FILE *stream) {
    int result = 0;

    // If the stream is not open for writing, return an error
    if (stream->mode == O_RDONLY) {
        errno = EBADF;
        return -1;
    }

    // If the buffer is empty, return success
    if (stream->buffer_size == 0) {
        return 0;
    }

    // If the stream is in no-buffering mode, write the data to the file immediately
    if (stream->buffer_mode == _IONBF) {
        ssize_t bytes_written = write(stream->fd, stream->buffer, stream->buffer_size);

        if (bytes_written == -1) {
            result = -1;
        }
        else {
            stream->buffer_size = 0;
        }
    }

    // If the stream is in line-buffering or full-buffering mode, write the data to the buffer
    else {
        // Write the buffer contents to the file
        ssize_t bytes_written = write(stream->fd, stream->buffer, stream->buffer_size);

        if (bytes_written == -1) {
            result = -1;
        }
        else {
            stream->buffer_size = 0;
        }

        // Flush the buffer by seeking to the current position in the file
        if (lseek(stream->fd, 0, SEEK_CUR) == -1) {
            result = -1;
        }
    }

    return result;
}

int fclose(FILE *stream) {
    if(stream == NULL) {
        return -1;
    }

    int result = fflush(stream);

    if(close(stream->fd) == -1) {
        result = -1;
    }

    _free(stream);
    return result;
}

static int print_int(int fd, int num);
static int print_double(int fd, double num);

static int print_int(int fd, int num) {
    int len = 0;
    char buf[MAX_INT_LEN];
    int i = 0;
    if (num < 0) {
        len += write(fd, "-", 1);
        num = -num;
    }
    do {
        buf[i++] = num % 10 + '0';
        num /= 10;
    } while (num > 0);
    while (i > 0) {
        len += write(fd, &buf[--i], 1);
    }
    return len;
}

static int print_double(int fd, double num) {
    int len = 0;
    char buf[MAX_DOUBLE_LEN];
    int i, sign;

    if (num < 0) {
        len += write(fd, "-", 1);
        num = -num;
    }

    int integer = (int)num;
    double fraction = num - integer;

    i = 0;
    do {
        buf[i++] = integer % 10 + '0';
        integer /= 10;
    } while (integer > 0);
    while (i > 0) {
        len += write(fd, &buf[--i], 1);
    }

    if (fraction > 0.0) {
        len += write(fd, ".", 1);
        i = 0;
        do {
            fraction *= 10.0;
            int digit = (int)fraction;
            buf[i++] = digit + '0';
            fraction -= digit;
        } while (fraction > 0.0);
        while (i > 0) {
            len += write(fd, &buf[--i], 1);
        }
    }

    return len;
}

int _fprintf(FILE *stream, const char *format, ...) {
    int fd = stream->fd;
    char ch;
    int num, len = 0;
    double dnum;
    char *str;
    va_list args;

    va_start(args, format);

    while ((ch = *format++) != '\0') {
        if (ch != '%') {
            len += write(fd, &ch, 1);
            continue;
        }
        ch = *format++;
        switch (ch) {
            case 'd':
                num = va_arg(args, int);
                len += print_int(fd, num);
                break;
            case 'f':
                dnum = va_arg(args, double);
                len += print_double(fd, dnum);
                break;
            case 's':
                str = va_arg(args, char *);
                len += write(fd, str, strlen(str));
                break;
            default:
                len += write(fd, &ch, 1);
                break;
        }
    }

    va_end(args);

    return len;
}