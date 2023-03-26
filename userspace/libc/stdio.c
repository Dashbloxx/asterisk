#include "stdio.h"
#include "syscall.h"
#include "string.h"
#include "stdarg.h"

#define O_FLAGS(flags) ((flags) == 'r' ? O_RDONLY : ((flags) == 'w' ? O_WRONLY | O_CREAT | O_TRUNC : 0))

FILE *fopen(const char *path, const char *mode) {
    FILE *fp = (FILE *)malloc(sizeof(FILE));

    fp->fd = open(path, O_FLAGS(*mode), 0644);
    fp->flags = *mode;
    fp->buf = (char *)malloc(BUFSIZ);
    fp->buf_size = BUFSIZ;
    fp->buf_pos = 0;

    return fp;
}

int fclose(FILE *fp) {
    int result = close(fp->fd);

    free(fp->buf);
    free(fp);

    return result;
}

size_t fread(void *ptr, size_t size, size_t count, FILE *fp) {
    size_t bytes_to_read = size * count;
    size_t bytes_read = 0;

    if (bytes_to_read < fp->buf_size) {
        if (fp->buf_pos + bytes_to_read > fp->buf_size) {
            bytes_to_read = fp->buf_size - fp->buf_pos;
        }

        memcpy(ptr, fp->buf + fp->buf_pos, bytes_to_read);
        fp->buf_pos += bytes_to_read;
        bytes_read += bytes_to_read;
    }

    if (bytes_read < size * count) {
        size_t bytes_remaining = size * count - bytes_read;
        size_t bytes_read_from_file = read(fp->fd, ptr + bytes_read, bytes_remaining);

        if (bytes_read_from_file < 0) {
            return bytes_read_from_file;
        }

        bytes_read += bytes_read_from_file;
    }

    return bytes_read / size;
}

size_t fwrite(const void *ptr, size_t size, size_t count, FILE *fp) {
    size_t bytes_to_write = size * count;
    size_t bytes_written = 0;

    if (bytes_to_write < fp->buf_size) {
        if (fp->buf_pos + bytes_to_write > fp->buf_size) {
            fwrite(fp->buf, 1, fp->buf_pos, fp);
            fp->buf_pos = 0;
        }

        memcpy(fp->buf + fp->buf_pos, ptr, bytes_to_write);
        fp->buf_pos += bytes_to_write;
        bytes_written += bytes_to_write;
    }

    if (fp->buf_pos == fp->buf_size) {
        fwrite(fp->buf, 1, fp->buf_size, fp);
        fp->buf_pos = 0;
    }

    if (bytes_written < size * count) {
        size_t bytes_remaining = size * count - bytes_written;
        size_t bytes_written_to_file = write(fp->fd, ptr + bytes_written, bytes_remaining);

        if (bytes_written_to_file < 0) {
            return bytes_written_to_file;
        }

        bytes_written += bytes_written_to_file;
    }

    return bytes_written / size;
}