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

#include "common.h"
#include "alloc.h"
#include "vmm.h"
#include "process.h"
#include "log.h"
#include "serial.h"

#define KMALLOC_MINSIZE 16

extern uint32_t *g_kernel_page_directory;

static char *g_kernel_heap = NULL;
static uint32_t g_kernel_heap_used = 0;


void initialize_kernel_heap()
{
    g_kernel_heap = (char *) KERN_HEAP_BEGIN;

    ksbrk_page(1);
}

void *ksbrk_page(int n)
{
    struct malloc_header *chunk;
    uint32_t p_addr;
    int i;

    if ((g_kernel_heap + (n * PAGESIZE_4K)) > (char *) KERN_HEAP_END) {
        //Screen_PrintF("ERROR: ksbrk(): no virtual memory left for kernel heap !\n");
        return (char *) -1;
    }

    chunk = (struct malloc_header *) g_kernel_heap;

    for (i = 0; i < n; i++)
    {
        p_addr = vmm_acquire_page_frame_4k();

        if ((int)(p_addr) < 0)
        {
            PANIC("PANIC: ksbrk_page(): no free page frame available !");
            return (char *) -1;
        }

        vmm_add_page_to_pd(g_kernel_heap, p_addr, 0); //add PG_USER to allow user programs to read kernel heap

        g_kernel_heap += PAGESIZE_4K;
    }

    chunk->size = PAGESIZE_4K * n;
    chunk->used = 0;

    return chunk;
}

void *kmalloc(uint32_t size)
{
    if (size == 0)
    {
        return 0;
    }

    unsigned long realsize;
    struct malloc_header *chunk, *other;

    if ((realsize = sizeof(struct malloc_header) + size) < KMALLOC_MINSIZE)
    {
        realsize = KMALLOC_MINSIZE;
    }

    chunk = (struct malloc_header *) KERN_HEAP_BEGIN;
    while (chunk->used || chunk->size < realsize)
    {
        if (chunk->size == 0)
        {
            kprintf("\nPANIC: kmalloc(): corrupted chunk on %x with null size (heap %x) !\nSystem halted\n", chunk, g_kernel_heap);

            PANIC("kmalloc()");

            return 0;
        }

        chunk = (struct malloc_header *)((char *)chunk + chunk->size);

        if (chunk == (struct malloc_header *) g_kernel_heap)
        {
            if ((int)(ksbrk_page((realsize / PAGESIZE_4K) + 1)) < 0)
            {
                PANIC("kmalloc(): no memory left for kernel !\nSystem halted\n");

                return 0;
            }
        }
        else if (chunk > (struct malloc_header *) g_kernel_heap)
        {
            kprintf("\nPANIC: kmalloc(): chunk on %x while heap limit is on %x !\nSystem halted\n", chunk, g_kernel_heap);

            PANIC("kmalloc()");

            return 0;
        }
    }


    if (chunk->size - realsize < KMALLOC_MINSIZE)
    {
        chunk->used = 1;
    }
    else
    {
        other = (struct malloc_header *)((char *) chunk + realsize);
        other->size = chunk->size - realsize;
        other->used = 0;

        chunk->size = realsize;
        chunk->used = 1;
    }

    g_kernel_heap_used += realsize;

    return (char *) chunk + sizeof(struct malloc_header);
}

void kfree(void *v_addr)
{
    if (v_addr == (void*)0)
    {
        return;
    }

    struct malloc_header *chunk, *other;

    chunk = (struct malloc_header *)((uint32_t)v_addr - sizeof(struct malloc_header));
    chunk->used = 0;

    g_kernel_heap_used -= chunk->size;

    //Merge free block with next free block
    while ((other = (struct malloc_header *)((char *)chunk + chunk->size))
           && other < (struct malloc_header *)g_kernel_heap
           && other->used == 0)
    {
        chunk->size += other->size;
    }
}

static void sbrk_page(Process* proc, int page_count)
{
    if (page_count > 0)
    {
        for (int i = 0; i < page_count; ++i)
        {
            if ((proc->brk_next_unallocated_page_begin + PAGESIZE_4K) > (char*)(MEMORY_END - PAGESIZE_4K))
            {
                return;
            }

            uint32_t p_addr = vmm_acquire_page_frame_4k();

            if ((int)(p_addr) < 0)
            {
                //PANIC("sbrk_page(): no free page frame available !");
                return;
            }

            vmm_add_page_to_pd(proc->brk_next_unallocated_page_begin, p_addr, PG_USER | PG_OWNED);

            SET_PAGEFRAME_USED(proc->mmapped_virtual_memory, PAGE_INDEX_4K((uint32_t)proc->brk_next_unallocated_page_begin));

            proc->brk_next_unallocated_page_begin += PAGESIZE_4K;
        }
    }
    else if (page_count < 0)
    {
        page_count *= -1;

        for (int i = 0; i < page_count; ++i)
        {
            if (proc->brk_next_unallocated_page_begin - PAGESIZE_4K >= proc->brk_begin)
            {
                proc->brk_next_unallocated_page_begin -= PAGESIZE_4K;

                //This also releases the page frame
                vmm_remove_page_from_pd(proc->brk_next_unallocated_page_begin);

                SET_PAGEFRAME_UNUSED(proc->mmapped_virtual_memory, (uint32_t)proc->brk_next_unallocated_page_begin);
            }
        }
    }
}

void initialize_program_break(Process* proc, uint32_t size)
{
    proc->brk_begin = (char*) USER_OFFSET;
    proc->brk_end = proc->brk_begin;
    proc->brk_next_unallocated_page_begin = proc->brk_begin;

    //Userland programs (their code, data,..) start from USER_OFFSET
    //Lets allocate some space for them by moving program break.

    sbrk(proc, size);
}

void *sbrk(Process* proc, int n_bytes)
{
    char* previous_break = proc->brk_end;

    if (n_bytes > 0)
    {
        int remainingInThePage = proc->brk_next_unallocated_page_begin - proc->brk_end;

        if (n_bytes > remainingInThePage)
        {
            int bytesNeededInNewPages = n_bytes - remainingInThePage;
            int neededNewPageCount = ((bytesNeededInNewPages-1) / PAGESIZE_4K) + 1;

            uint32_t freePages = vmm_get_free_page_count();
            if ((uint32_t)neededNewPageCount + 1 > freePages)
            {
                return (void*)-1;
            }

            sbrk_page(proc, neededNewPageCount);
        }
    }
    else if (n_bytes < 0)
    {
        char* currentPageBegin = proc->brk_next_unallocated_page_begin - PAGESIZE_4K;

        int remainingInThePage = proc->brk_end - currentPageBegin;

        if (-n_bytes > remainingInThePage)
        {
            int bytesInPreviousPages = -n_bytes - remainingInThePage;
            int neededNewPageCount = ((bytesInPreviousPages-1) / PAGESIZE_4K) + 1;

            sbrk_page(proc, -neededNewPageCount);
        }
    }

    proc->brk_end += n_bytes;

    return previous_break;
}

uint32_t get_kernel_heap_used()
{
    return g_kernel_heap_used;
}
