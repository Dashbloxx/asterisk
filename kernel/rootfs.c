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

