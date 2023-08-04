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
 
#include "process.h"
#include "common.h"
#include "errno.h"
#include "syscall_getthreads.h"


int32_t syscall_getthreads(ThreadInfo* threads, uint32_t max_count, uint32_t flags)
{
    if (!check_user_access(threads))
    {
        return -EFAULT;
    }

    //interrupts are disabled

    memset((uint8_t*)threads, 0, sizeof(ThreadInfo) * max_count);

    ThreadInfo* info = threads;

    Thread* t = thread_get_first();
    uint32_t i = 0;
    while (t && i < max_count)
    {
        info->thread_id = t->threadId;
        info->process_id = t->owner->pid;
        info->state = t->state;
        info->user_mode = t->user_mode;
        info->birth_time = t->birth_time;
        info->context_switch_count = t->context_switch_count;
        info->context_start_time = t->context_start_time;
        info->context_end_time = t->context_end_time;
        info->consumed_cpu_time_ms = t->consumed_cpu_time_ms;
        info->usage_cpu = t->usage_cpu;
        info->called_syscall_count = t->called_syscall_count;

        t = t->next;
        i++;
        info++;
    }

    return i;
}


static BOOL exists_in_info_list(uint32_t pid, ProcInfo* procs, uint32_t count)
{
    ProcInfo* info = procs;

    uint32_t i = 0;

    while (i < count)
    {
        if (info->process_id == pid)
        {
            return TRUE;
        }
        i++;
        info++;
    }

    return FALSE;
}

int32_t syscall_getprocs(ProcInfo* procs, uint32_t max_count, uint32_t flags)
{
    if (!check_user_access(procs))
    {
        return -EFAULT;
    }

    //interrupts are disabled

    memset((uint8_t*)procs, 0, sizeof(ProcInfo) * max_count);

    ProcInfo* info = procs;

    Thread* t = thread_get_first();
    uint32_t i = 0;
    uint32_t process_count = 0;
    while (t && i < max_count)
    {
        Process* process = t->owner;

        if (!exists_in_info_list(process->pid, procs, process_count))
        {
            info->process_id = process->pid;
            info->parent_process_id = -1;
            if (process->parent)
            {
                info->parent_process_id = process->parent->pid;
            }

            for (uint32_t k = 0; k < ASTERISK_MAX_OPENED_FILES; ++k)
            {
                File* file = process->fd[k];
                if (file && file->node)
                {
                    info->fd[k] = file->node->node_type;
                }
            }

            memcpy((uint8_t*)info->name, process->name, ASTERISK_PROCESS_NAME_MAX);

            //TODO: tty, working_directory

            info++;
            process_count++;
        }
        

        t = t->next;
        i++;
    }

    return process_count;
}
