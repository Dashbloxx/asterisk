#include "stdio.h"
#include "syscall.h"
#include "stdlib.h"
#include "stdint.h"
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

size_t fwrite(const void* ptr, size_t size, size_t count, FILE* stream) {

}
