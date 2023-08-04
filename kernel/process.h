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

#define KERNELMODE	0
#define USERMODE	1

#define ASTERISK_MAX_OPENED_FILES 20

#define ASTERISK_PROCESS_NAME_MAX 32

#include "common.h"
#include "fs.h"
#include "syscall_select.h"
#include "fifobuffer.h"
#include "spinlock.h"
#include "signal.h"

typedef enum
{
    TS_RUN,
    TS_WAITIO,
    TS_WAITCHILD,
    TS_SLEEP,
    TS_SELECT,
    TS_SUSPEND,
    TS_CRITICAL,
    TS_UNINTERRUPTIBLE,
    TS_DEAD,
} thread_state_t;

typedef enum SelectState
{
    SS_NOTSTARTED,
    SS_STARTED,
    SS_FINISHED,
    SS_ERROR
} SelectState;

struct Process
{
    char name[ASTERISK_PROCESS_NAME_MAX];

    uint32_t pid;


    uint32_t *pd;

    uint32_t b_exec;
    uint32_t e_exec;
    uint32_t b_bss;
    uint32_t e_bss;

    char *brk_begin;
    char *brk_end;
    char *brk_next_unallocated_page_begin;

    uint8_t mmapped_virtual_memory[RAM_AS_4K_PAGES / 8];

    filesystem_node* tty;

    filesystem_node* working_directory;

    Process* parent;

    File* fd[ASTERISK_MAX_OPENED_FILES];

} __attribute__ ((packed));

typedef struct Process Process;

struct Thread
{
    uint32_t threadId;

    struct
    {
        uint32_t eax, ecx, edx, ebx;
        uint32_t esp, ebp, esi, edi;
        uint32_t eip, eflags;
        uint32_t cs:16, ss:16, ds:16, es:16, fs:16, gs:16;
        uint32_t cr3;
    } regs __attribute__ ((packed));

    struct
    {
        uint32_t esp0;
        uint16_t ss0;
        uint32_t stack_start;
    } kstack __attribute__ ((packed));

    struct
    {
        int nfds;
        fd_set read_set;
        fd_set write_set;
        fd_set read_set_result;
        fd_set write_set_result;
        time_t target_time;
        SelectState select_state;
        int result;
    } select;

    uint32_t user_mode;

    FifoBuffer* signals;//no need to lock as this always accessed in interrupts disabled
    uint32_t pending_signal_count;

    thread_state_t state;
    void* state_privateData;

    Process* owner;

    uint32_t birth_time;
    uint32_t context_switch_count;
    uint32_t context_start_time;
    uint32_t context_end_time;
    uint32_t consumed_cpu_time_ms;
    uint32_t consumed_cpu_time_ms_at_prev_mark;
    uint32_t usage_cpu; //FromPrevMark
    uint32_t called_syscall_count;


    FifoBuffer* message_queue;
    Spinlock message_queue_lock;

    struct Thread* next;

};

typedef struct Thread Thread;

typedef struct TimerInt_Registers
{
    uint32_t gs, fs, es, ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; //pushed by pushad
    uint32_t eip, cs, eflags, esp_if_privilege_change, ss_if_privilege_change; //pushed by the CPU
} TimerInt_Registers;

typedef void (*Function0)();

void tasking_initialize();
void thread_create_kthread(Function0 func);
Process* process_create_from_elf_data(const char* name, uint8_t* elf_data, char *const argv[], char *const envp[], Process* parent, filesystem_node* tty);
Process* process_create_from_function(const char* name, Function0 func, char *const argv[], char *const envp[], Process* parent, filesystem_node* tty);
Process* process_create_ex(const char* name, uint32_t process_id, uint32_t thread_id, Function0 func, uint8_t* elf_data, char *const argv[], char *const envp[], Process* parent, filesystem_node* tty);
void thread_destroy(Thread* thread);
void process_destroy(Process* process);
void process_change_state(Process* process, thread_state_t state);
void thread_change_state(Thread* thread, thread_state_t state, void* private_data);
void thread_resume(Thread* thread);
BOOL thread_signal(Thread* thread, uint8_t signal);
BOOL process_signal(uint32_t pid, uint8_t signal);
void thread_state_to_string(thread_state_t state, uint8_t* buffer, uint32_t buffer_size);
void wait_for_schedule();
int32_t process_get_empty_fd(Process* process);
int32_t process_add_file(Process* process, File* file);
int32_t process_remove_file(Process* process, File* file);
File* process_find_file(Process* process, filesystem_node* node);
Thread* thread_get_by_id(uint32_t thread_id);
Thread* thread_get_previous(Thread* thread);
Thread* thread_get_first();
Thread* thread_get_current();
void schedule(TimerInt_Registers* registers);
BOOL thread_is_valid(Thread* thread);
BOOL process_is_valid(Process* process);
uint32_t get_system_context_switch_count();

extern Thread* g_current_thread;
extern Thread* g_previous_scheduled_thread;