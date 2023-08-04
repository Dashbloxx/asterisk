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
 
#include "ramdisk.h"
#include "alloc.h"
#include "fs.h"
#include "devfs.h"

typedef struct Ramdisk
{
    uint8_t* buffer;
    uint32_t size;
} Ramdisk;

#define RAMDISK_BLOCKSIZE 512

static BOOL open(File *file, uint32_t flags);
static void close(File *file);
static int32_t read_block(filesystem_node* node, uint32_t block_number, uint32_t count, uint8_t* buffer);
static int32_t write_block(filesystem_node* node, uint32_t block_number, uint32_t count, uint8_t* buffer);
static int32_t ioctl(File *node, int32_t request, void * argp);

BOOL ramdisk_create(const char* devName, uint32_t size)
{
    Ramdisk* ramdisk = kmalloc(sizeof(Ramdisk));
    ramdisk->size = size;
    ramdisk->buffer = kmalloc(size);

    Device device;
    memset((uint8_t*)&device, 0, sizeof(device));
    strcpy(device.name, devName);
    device.device_type = FT_BLOCK_DEVICE;
    device.open = open;
    device.close = close;
    device.read_block = read_block;
    device.write_block = write_block;
    device.ioctl = ioctl;
    device.private_data = ramdisk;

    if (devfs_register_device(&device))
    {
        return TRUE;
    }

    kfree(ramdisk->buffer);
    kfree(ramdisk);

    return FALSE;
}

static BOOL open(File *file, uint32_t flags)
{
    return TRUE;
}

static void close(File *file)
{
}

static int32_t read_block(filesystem_node* node, uint32_t block_number, uint32_t count, uint8_t* buffer)
{
    Ramdisk* ramdisk = (Ramdisk*)node->private_node_data;

    uint32_t location = block_number * RAMDISK_BLOCKSIZE;
    uint32_t size = count * RAMDISK_BLOCKSIZE;

    if (location + size > ramdisk->size)
    {
        return -1;
    }

    begin_critical_section();

    memcpy(buffer, ramdisk->buffer + location, size);

    end_critical_section();

    return 0;
}

static int32_t write_block(filesystem_node* node, uint32_t block_number, uint32_t count, uint8_t* buffer)
{
    Ramdisk* ramdisk = (Ramdisk*)node->private_node_data;

    uint32_t location = block_number * RAMDISK_BLOCKSIZE;
    uint32_t size = count * RAMDISK_BLOCKSIZE;

    if (location + size > ramdisk->size)
    {
        return -1;
    }

    begin_critical_section();

    memcpy(ramdisk->buffer + location, buffer, size);

    end_critical_section();

    return 0;
}

static int32_t ioctl(File *node, int32_t request, void * argp)
{
    Ramdisk* ramdisk = (Ramdisk*)node->node->private_node_data;

    uint32_t* result = (uint32_t*)argp;

    switch (request)
    {
    case IC_GET_SECTOR_COUNT:
        *result = ramdisk->size / RAMDISK_BLOCKSIZE;
        return 0;
        break;
    case IC_GET_SECTOR_SIZE_BYTES:
        *result = RAMDISK_BLOCKSIZE;
        return 0;
        break;
    default:
        break;
    }

    return -1;
}
