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

typedef struct Process Process;
typedef struct List List;

extern uint32_t *g_kernel_page_directory;


#define SET_PAGEFRAME_USED(bitmap, page_index)	bitmap[((uint32_t) page_index)/8] |= (1 << (((uint32_t) page_index)%8))
#define SET_PAGEFRAME_UNUSED(bitmap, p_addr)	bitmap[((uint32_t) p_addr/PAGESIZE_4K)/8] &= ~(1 << (((uint32_t) p_addr/PAGESIZE_4K)%8))
#define IS_PAGEFRAME_USED(bitmap, page_index)	(bitmap[((uint32_t) page_index)/8] & (1 << (((uint32_t) page_index)%8)))

#define CHANGE_PD(pd) asm("mov %0, %%eax ;mov %%eax, %%cr3":: "m"(pd))
#define INVALIDATE(v_addr) asm("invlpg %0"::"m"(v_addr))

uint32_t vmm_acquire_page_frame_4k();
void vmm_release_page_frame_4k(uint32_t p_addr);

void vmm_initialize(uint32_t high_mem);

uint32_t *vmm_acquire_page_directory();
void vmm_destroy_page_directory_with_memory(uint32_t physical_pd);

BOOL vmm_add_page_to_pd(char *v_addr, uint32_t p_addr, int flags);
BOOL vmm_remove_page_from_pd(char *v_addr);

void enable_paging();
void disable_paging();

uint32_t vmm_get_total_page_count();
uint32_t vmm_get_used_page_count();
uint32_t vmm_get_free_page_count();

void vmm_initialize_process_pages(Process* process);
void* vmm_map_memory(Process* process, uint32_t v_address_search_start, uint32_t* p_address_array, uint32_t page_count, BOOL own);
BOOL vmm_unmap_memory(Process* process, uint32_t v_address, uint32_t page_count);