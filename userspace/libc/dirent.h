#pragma once

#define NAME_MAX 255

struct dirent {
    unsigned int d_ino;
    unsigned int d_off;
    unsigned short d_reclen;
    unsigned char d_type;
    char d_name[NAME_MAX+1];
};

typedef struct {
    int fd;
    struct dirent ent;
} DIR;