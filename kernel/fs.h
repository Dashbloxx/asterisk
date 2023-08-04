/*
 *      dP      Asterisk is an operating system written fully in C and Intel-syntax
 *  8b. 88 .d8  assembly. It strives to be POSIX-compliant, and a faster & lightweight
 *   `8b88d8'   alternative to Linux for i386 processors.
 *   .8P88Y8.   
 *  8P' 88 `Y8  
 *      dP      
 *
 *  BSD 2-Clause License
 *  Copyright (c) 2017, ozkl, Nexuss
 *  
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *  
 *  * Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *  
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *  
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 *  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 *  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 *  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 *  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
#pragma once

#include "common.h"

#define O_ACCMODE   0003
#define O_RDONLY    00
#define O_WRONLY    01
#define O_RDWR      02
#define CHECK_ACCESS(flags, test) ((flags & O_ACCMODE) == test)

typedef enum FileType
{
    FT_FILE               = 1,
    FT_CHARACTER_DEVICE   = 2,
    FT_BLOCK_DEVICE       = 3,
    FT_PIPE               = 4,
    FT_SYMBOLIC_LINK      = 5,
    FT_SOCKET             = 6,
    FT_DIRECTORY          = 128,
    FT_MOUNT_POINT        = 256
} FileType;

typedef enum IoctlCommand
{
    IC_GET_SECTOR_SIZE_BYTES,
    IC_GET_SECTOR_COUNT,
} IoctlCommand;

typedef struct FileSystem FileSystem;
typedef struct filesystem_node filesystem_node;
typedef struct filesystem_dirent filesystem_dirent;
typedef struct Process Process;
typedef struct Thread Thread;
typedef struct File File;

struct stat;

typedef int32_t (*ReadWriteFunction)(File* file, uint32_t size, uint8_t* buffer);
typedef BOOL (*ReadWriteTestFunction)(File* file);
typedef int32_t (*ReadWriteBlockFunction)(filesystem_node* node, uint32_t block_number, uint32_t count, uint8_t* buffer);
typedef BOOL (*OpenFunction)(File* file, uint32_t flags);
typedef void (*CloseFunction)(File* file);
typedef int32_t (*UnlinkFunction)(filesystem_node* node, uint32_t flags);
typedef int32_t (*IoctlFunction)(File *file, int32_t request, void * argp);
typedef int32_t (*LseekFunction)(File *file, int32_t offset, int32_t whence);
typedef int32_t (*FtruncateFunction)(File *file, int32_t length);
typedef int32_t (*StatFunction)(filesystem_node *node, struct stat *buf);
typedef filesystem_dirent * (*ReadDirFunction)(filesystem_node*,uint32_t);
typedef filesystem_node * (*FindDirFunction)(filesystem_node*,char *name);
typedef BOOL (*MkDirFunction)(filesystem_node* node, const char *name, uint32_t flags);
typedef void* (*MmapFunction)(File* file, uint32_t size, uint32_t offset, uint32_t flags);
typedef BOOL (*MunmapFunction)(File* file, void* address, uint32_t size);

typedef BOOL (*MountFunction)(const char* source_path, const char* target_path, uint32_t flags, void *data);

typedef struct FileSystem
{
    char name[32];
    MountFunction check_mount;
    MountFunction mount;
} FileSystem;

typedef struct filesystem_node
{
    char name[128];
    uint32_t mask;
    uint32_t user_id;
    uint32_t group_id;
    uint32_t node_type;
    uint32_t inode;
    uint32_t length;
    ReadWriteBlockFunction read_block;
    ReadWriteBlockFunction write_block;
    ReadWriteFunction read;
    ReadWriteFunction write;
    ReadWriteTestFunction read_test_ready;
    ReadWriteTestFunction write_test_ready;
    OpenFunction open;
    CloseFunction close;
    UnlinkFunction unlink;
    IoctlFunction ioctl;
    LseekFunction lseek;
    FtruncateFunction ftruncate;
    StatFunction stat;
    ReadDirFunction readdir;
    FindDirFunction finddir;
    MkDirFunction mkdir;
    MmapFunction mmap;
    MunmapFunction munmap;
    filesystem_node *first_child;
    filesystem_node *next_sibling;
    filesystem_node *parent;
    filesystem_node *mount_point;//only used in mounts
    filesystem_node *mount_source;//only used in mounts
    void* private_node_data;
} filesystem_node;

typedef struct filesystem_dirent
{
    char name[128];
    FileType file_type;
    uint32_t inode;
} filesystem_dirent;

//Per open
typedef struct File
{
    filesystem_node* node;
    Process* process;
    Thread* thread;
    int32_t fd;
    uint32_t flags;
    int32_t offset;
    void* private_data;
} File;

struct stat
{
    uint16_t/*dev_t      */ st_dev;     /* ID of device containing file */
    uint16_t/*ino_t      */ st_ino;     /* inode number */
    uint32_t/*mode_t     */ st_mode;    /* protection */
    uint16_t/*nlink_t    */ st_nlink;   /* number of hard links */
    uint16_t/*uid_t      */ st_uid;     /* user ID of owner */
    uint16_t/*gid_t      */ st_gid;     /* group ID of owner */
    uint16_t/*dev_t      */ st_rdev;    /* device ID (if special file) */
    uint32_t/*off_t      */ st_size;    /* total size, in bytes */

    uint32_t/*time_t     */ st_atime;
    uint32_t/*long       */ st_spare1;
    uint32_t/*time_t     */ st_mtime;
    uint32_t/*long       */ st_spare2;
    uint32_t/*time_t     */ st_ctime;
    uint32_t/*long       */ st_spare3;
    uint32_t/*blksize_t  */ st_blksize;
    uint32_t/*blkcnt_t   */ st_blocks;
    uint32_t/*long       */ st_spare4[2];
};


uint32_t fs_read(File* file, uint32_t size, uint8_t* buffer);
uint32_t fs_write(File* file, uint32_t size, uint8_t* buffer);
File* fs_open(filesystem_node* node, uint32_t flags);
File* fs_open_for_process(Thread* thread, filesystem_node* node, uint32_t flags);
void fs_close(File* file);
int32_t fs_unlink(filesystem_node* node, uint32_t flags);
int32_t fs_ioctl(File* file, int32_t request, void* argp);
int32_t fs_lseek(File* file, int32_t offset, int32_t whence);
int32_t fs_ftruncate(File* file, int32_t length);
int32_t fs_stat(filesystem_node *node, struct stat *buf);
filesystem_dirent* fs_readdir(filesystem_node* node, uint32_t index);
filesystem_node* fs_finddir(filesystem_node* node, char* name);
BOOL fs_mkdir(filesystem_node *node, const char* name, uint32_t flags);
void* fs_mmap(File* file, uint32_t size, uint32_t offset, uint32_t flags);
BOOL fs_munmap(File* file, void* address, uint32_t size);
int fs_get_node_path(filesystem_node* node, char* buffer, uint32_t buffer_size);
BOOL fs_resolve_path(const char* path, char* buffer, int buffer_size);

void fs_initialize();
filesystem_node* fs_get_root_node();
filesystem_node* fs_get_node(const char* path);
filesystem_node* fs_get_node_absolute_or_relative(const char* path, Process* process);
filesystem_node* fs_get_node_relative_to_node(const char* path, filesystem_node* relative_to);

BOOL fs_register(FileSystem* fs);
BOOL fs_mount(const char *source, const char *target, const char *fsType, uint32_t flags, void *data);
BOOL fs_check_mount(const char *source, const char *target, const char *fsType, uint32_t flags, void *data);