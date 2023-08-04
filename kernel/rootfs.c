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
 
#include "rootfs.h"
#include "alloc.h"

static BOOL rootfs_open(File *node, uint32_t flags);
static void rootfs_close(File *file);
static filesystem_node *rootfs_finddir(filesystem_node *node, char *name);
static struct filesystem_dirent *rootfs_readdir(filesystem_node *node, uint32_t index);
static BOOL rootfs_mkdir(filesystem_node *node, const char *name, uint32_t flags);

filesystem_node* rootfs_initialize()
{
    filesystem_node* root = (filesystem_node*)kmalloc(sizeof(filesystem_node));
    memset((uint8_t*)root, 0, sizeof(filesystem_node));
    root->node_type = FT_DIRECTORY;
    root->open = rootfs_open;
    root->close = rootfs_close;
    root->readdir = rootfs_readdir;
    root->finddir = rootfs_finddir;
    root->mkdir = rootfs_mkdir;

    return root;
}

static filesystem_dirent g_dirent;

static BOOL rootfs_open(File *node, uint32_t flags)
{
    return TRUE;
}

static void rootfs_close(File *file)
{

}

static struct filesystem_dirent *rootfs_readdir(filesystem_node *node, uint32_t index)
{
    filesystem_node *n = node->first_child;
    uint32_t i = 0;
    while (NULL != n)
    {
        if (index == i)
        {
            g_dirent.file_type = n->node_type;
            g_dirent.inode = n->inode;
            strcpy(g_dirent.name, n->name);

            return &g_dirent;
        }
        n = n->next_sibling;
        ++i;
    }

    return NULL;
}

static filesystem_node *rootfs_finddir(filesystem_node *node, char *name)
{
    filesystem_node *n = node->first_child;
    while (NULL != n)
    {
        if (strcmp(name, n->name) == 0)
        {
            return n;
        }
        n = n->next_sibling;
    }

    return NULL;
}

static BOOL rootfs_mkdir(filesystem_node *node, const char *name, uint32_t flags)
{
    filesystem_node *n = node->first_child;
    while (NULL != n)
    {
        if (strcmp(name, n->name) == 0)
        {
            return FALSE;
        }
        n = n->next_sibling;
    }

    filesystem_node* new_node = (filesystem_node*)kmalloc(sizeof(filesystem_node));
    memset((uint8_t*)new_node, 0, sizeof(filesystem_node));
    strcpy(new_node->name, name);
    new_node->node_type = FT_DIRECTORY;
    new_node->open = rootfs_open;
    new_node->close = rootfs_close;
    new_node->readdir = rootfs_readdir;
    new_node->finddir = rootfs_finddir;
    new_node->mkdir = rootfs_mkdir;
    new_node->parent = node;

    if (node->first_child == NULL)
    {
        node->first_child = new_node;
    }
    else
    {
        filesystem_node *n = node->first_child;
        while (NULL != n->next_sibling)
        {
            n = n->next_sibling;
        }
        n->next_sibling = new_node;
    }

    return TRUE;
}

