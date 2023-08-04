/*
 *      dP      Asterisk is an operating system written fully in C and Intel-syntax
 *  8b. 88 .d8  assembly. It strives to be POSIX-compliant, and a faster & lightweight
 *   `8b88d8'   alternative to Linux for i386 processors.
 *   .8P88Y8.   
 *  8P' 88 `Y8  
 *      dP      
 *
 *  BSD 2-Clause License
 *  Copyright (c) 2023 Nexuss
 *  All rights reserved.
 */

#pragma once

#define S_IRUSR 0400
#define S_IWUSR 0200
#define S_IXUSR 0100
#define S_IRGRP 0040
#define S_IWGRP 0020
#define S_IXGRP 0010
#define S_IROTH 0004
#define S_IWOTH 0002
#define S_IXOTH 0001

struct stat
{
    unsigned short/*dev_t      */ st_dev;     /* ID of device containing file */
    unsigned short/*ino_t      */ st_ino;     /* inode number */
    unsigned int/*mode_t     */ st_mode;    /* protection */
    unsigned short/*nlink_t    */ st_nlink;   /* number of hard links */
    unsigned short/*uid_t      */ st_uid;     /* user ID of owner */
    unsigned short/*gid_t      */ st_gid;     /* group ID of owner */
    unsigned short/*dev_t      */ st_rdev;    /* device ID (if special file) */
    unsigned int/*off_t      */ st_size;    /* total size, in bytes */

    unsigned int/*time_t     */ st_atime;
    unsigned int/*long       */ st_spare1;
    unsigned int/*time_t     */ st_mtime;
    unsigned int/*long       */ st_spare2;
    unsigned int/*time_t     */ st_ctime;
    unsigned int/*long       */ st_spare3;
    unsigned int/*blksize_t  */ st_blksize;
    unsigned int/*blkcnt_t   */ st_blocks;
    unsigned int/*long       */ st_spare4[2];
};