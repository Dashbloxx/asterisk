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
 
#include "devfs.h"
#include "common.h"
#include "fs.h"
#include "alloc.h"
#include "device.h"
#include "list.h"
#include "spinlock.h"

/*
 *  DevFS, also known as device filesystem, allows processes and/or the user to send data and/or recieve data from files in the `/dev` directory, which contains
 *  special files which represent hardware.
 */

static filesystem_node* g_dev_root = NULL;

static List* g_device_list = NULL;
static Spinlock g_device_list_lock;

static BOOL devfs_open(File *node, uint32_t flags);
static filesystem_dirent *devfs_readdir(filesystem_node *node, uint32_t index);
static filesystem_node *devfs_finddir(filesystem_node *node, char *name);

static filesystem_dirent g_dirent;

/* Initialize DevFS, and create the `/dev` directory. If the directory isn't created, then make a kernel panic... */
void devfs_initialize()
{
    g_dev_root = kmalloc(sizeof(filesystem_node));
    memset((uint8_t*)g_dev_root, 0, sizeof(filesystem_node));

    g_dev_root->node_type = FT_DIRECTORY;

    filesystem_node* root_node = fs_get_root_node();

    filesystem_node* dev_node = fs_finddir(root_node, "dev");

    if (dev_node)
    {
        dev_node->node_type |= FT_MOUNT_POINT;
        dev_node->mount_point = g_dev_root;
        g_dev_root->parent = dev_node->parent;
        strcpy(g_dev_root->name, dev_node->name);
    }
    else
    {
        PANIC("/dev does not exist!");
    }

    g_dev_root->open = devfs_open;
    g_dev_root->finddir = devfs_finddir;
    g_dev_root->readdir = devfs_readdir;

    g_device_list = list_create();
    spinlock_init(&g_device_list_lock);
}

/* Not implemented yet... */
static BOOL devfs_open(File *node, uint32_t flags)
{
    return TRUE;
}

static filesystem_dirent *devfs_readdir(filesystem_node *node, uint32_t index)
{
    filesystem_dirent * result = NULL;

    uint32_t counter = 0;

    spinlock_lock(&g_device_list_lock);

    list_foreach(n, g_device_list)
    {
        if (index == counter)
        {
            filesystem_node* device_node = (filesystem_node*)n->data;
            strcpy(g_dirent.name, device_node->name);
            g_dirent.file_type = device_node->node_type;
            g_dirent.inode = index;
            result = &g_dirent;
            break;
        }

        ++counter;
    }
    spinlock_unlock(&g_device_list_lock);

    return result;
}

static filesystem_node *devfs_finddir(filesystem_node *node, char *name)
{
    filesystem_node* result = NULL;


    spinlock_lock(&g_device_list_lock);

    list_foreach(n, g_device_list)
    {
        filesystem_node* deviceNode = (filesystem_node*)n->data;

        if (strcmp(name, deviceNode->name) == 0)
        {
            result = deviceNode;
            break;
        }
    }

    spinlock_unlock(&g_device_list_lock);

    return result;
}

filesystem_node* devfs_register_device(Device* device)
{
    spinlock_lock(&g_device_list_lock);

    list_foreach(n, g_device_list)
    {
        filesystem_node* device_node = (filesystem_node*)n->data;

        if (strcmp(device->name, device_node->name) == 0)
        {
            //There is already a device with the same name
            spinlock_unlock(&g_device_list_lock);
            return NULL;
        }
    }

    filesystem_node* device_node = (filesystem_node*)kmalloc(sizeof(filesystem_node));
    memset((uint8_t*)device_node, 0, sizeof(filesystem_node));
    strcpy(device_node->name, device->name);
    device_node->node_type = device->device_type;
    device_node->open = device->open;
    device_node->close = device->close;
    device_node->read_block = device->read_block;
    device_node->write_block = device->write_block;
    device_node->read = device->read;
    device_node->write = device->write;
    device_node->read_test_ready = device->read_test_ready;
    device_node->write_test_ready = device->write_test_ready;
    device_node->ioctl = device->ioctl;
    device_node->ftruncate = device->ftruncate;
    device_node->mmap = device->mmap;
    device_node->munmap = device->munmap;
    device_node->private_node_data = device->private_data;
    device_node->parent = g_dev_root;

    list_append(g_device_list, device_node);

    spinlock_unlock(&g_device_list_lock);

    return device_node;
}
