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

#include "stdint.h"
#include "stdlib.h"
#include "syscall.h"
#include "stdio.h"

#define BLOCK_SIZE sizeof(struct block)

struct block {
    unsigned int size;
    struct block *next;
    int free;
};

struct block *head = NULL;
struct block *tail = NULL;

void *malloc(unsigned int size) {
    struct block *curr;
    void *ptr;

    if (size == 0) {
        return NULL;
    }

    /* Check if there is a free block */
    for (curr = head; curr != NULL; curr = curr->next) {
        if (curr->free && curr->size >= size) {
            curr->free = 0;
            return (void *)(curr + 1);
        }
    }

    /* Allocate a new block */
    curr = (struct block *)sbrk(BLOCK_SIZE + size);
    if (curr == (void *)-1) {
        return NULL;
    }
    curr->size = size;
    curr->next = NULL;
    curr->free = 0;

    /* Add the block to the end of the list */
    if (tail == NULL) {
        head = tail = curr;
    } else {
        tail->next = curr;
        tail = curr;
    }

    return (void *)(curr + 1);
}

void free(void *ptr) {
    struct block *curr;
    struct block *prev = NULL;

    if (ptr == NULL) {
        return;
    }

    curr = (struct block *)ptr - 1;

    /* Find the block in the list */
    for (prev = NULL, curr = head; curr != NULL; prev = curr, curr = curr->next) {
        if (curr == (struct block *)ptr - 1) {
            break;
        }
    }

    if (curr == NULL) {
        /* Block not found */
        return;
    }

    /* Free the block */
    curr->free = 1;

    /* Merge adjacent free blocks */
    if (prev != NULL && prev->free) {
        prev->size += BLOCK_SIZE + curr->size;
        prev->next = curr->next;
        curr = prev;
    }
    if (curr->next != NULL && curr->next->free) {
        curr->size += BLOCK_SIZE + curr->next->size;
        curr->next = curr->next->next;
    }

    /* Remove the block from the list if it is free */
    if (curr->free && curr == head) {
        head = curr->next;
        if (head == NULL) {
            tail = NULL;
        }
    }
}

void __init_libc() {
    stderr = malloc(sizeof(FILE));
    stdin = malloc(sizeof(FILE));
    stdout = malloc(sizeof(FILE));
    stderr->fd = 0;
    stdin->fd = 1;
    stdout->fd = 2;
}