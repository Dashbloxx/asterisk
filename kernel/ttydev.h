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
#include "spinlock.h"
#include "termios.h"
#include "fs.h"

#define TTYDEV_LINEBUFFER_SIZE 4096

typedef struct filesystem_node filesystem_node;
typedef struct FifoBuffer FifoBuffer;
typedef struct List List;
typedef struct Thread Thread;

typedef struct winsize_t
{
    uint16_t ws_row;	/* rows, in characters */
    uint16_t ws_col;	/* columns, in characters */
    uint16_t ws_xpixel;	/* horizontal size, pixels */
    uint16_t ws_ypixel;	/* vertical size, pixels */
} winsize_t;

typedef struct TtyDev TtyDev;

typedef void (*TtyIOReady)(TtyDev* tty, uint32_t size);
typedef struct TtyDev
{
    filesystem_node* master_node;
    filesystem_node* slave_node;
    void* private_data;
    int32_t controlling_process;
    int32_t foreground_process;
    winsize_t winsize;
    FifoBuffer* buffer_master_write;
    Spinlock buffer_master_write_lock;
    FifoBuffer* buffer_master_read;
    Spinlock buffer_master_read_lock;
    FifoBuffer* buffer_echo; //used in only echoing by master_write, no need lock
    List* slave_readers;
    Spinlock slave_readers_lock;
    Thread* master_reader;
    TtyIOReady master_read_ready; //used for kernel terminal, because it does not read like a user process
    uint8_t line_buffer[TTYDEV_LINEBUFFER_SIZE];
    uint32_t line_buffer_index;
    struct termios term;

} TtyDev;

filesystem_node* ttydev_create();

int32_t ttydev_master_read_nonblock(File *file, uint32_t size, uint8_t *buffer);
