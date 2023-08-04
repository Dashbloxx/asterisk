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
 
#include "fifobuffer.h"
#include "alloc.h"

FifoBuffer* fifobuffer_create(uint32_t capacity)
{
    FifoBuffer* fifo = (FifoBuffer*)kmalloc(sizeof(FifoBuffer));
    memset((uint8_t*)fifo, 0, sizeof(FifoBuffer));
    fifo->data = (uint8_t*)kmalloc(capacity);
    memset((uint8_t*)fifo->data, 0, capacity);
    fifo->capacity= capacity;

    return fifo;
}

void fifobuffer_destroy(FifoBuffer* fifo_buffer)
{
    kfree(fifo_buffer->data);
    kfree(fifo_buffer);
}

void fifobuffer_clear(FifoBuffer* fifo_buffer)
{
    fifo_buffer->used_bytes = 0;
    fifo_buffer->read_index = 0;
    fifo_buffer->write_index = 0;
}

BOOL fifobuffer_is_empty(FifoBuffer* fifo_buffer)
{
    if (0 == fifo_buffer->used_bytes)
    {
        return TRUE;
    }

    return FALSE;
}

uint32_t fifobuffer_get_size(FifoBuffer* fifo_buffer)
{
    return fifo_buffer->used_bytes;
}

uint32_t fifobuffer_get_capacity(FifoBuffer* fifo_buffer)
{
    return fifo_buffer->capacity;
}

uint32_t fifobuffer_get_free(FifoBuffer* fifo_buffer)
{
    return fifo_buffer->capacity - fifo_buffer->used_bytes;
}

int32_t fifobuffer_enqueue(FifoBuffer* fifo_buffer, uint8_t* data, uint32_t size)
{
    if (size == 0)
    {
        return -1;
    }

    uint32_t bytes_available = fifo_buffer->capacity - fifo_buffer->used_bytes;

    uint32_t i = 0;
    while (fifo_buffer->used_bytes < fifo_buffer->capacity && i < size)
    {
        fifo_buffer->data[fifo_buffer->write_index] = data[i++];
        fifo_buffer->used_bytes++;
        fifo_buffer->write_index++;
        fifo_buffer->write_index %= fifo_buffer->capacity;
    }

    return (int32_t)i;
}

int32_t fifobuffer_dequeue(FifoBuffer* fifo_buffer, uint8_t* data, uint32_t size)
{
    if (size == 0)
    {
        return -1;
    }

    if (0 == fifo_buffer->used_bytes)
    {
        //Buffer is empty
        return 0;
    }

    uint32_t i = 0;
    while (fifo_buffer->used_bytes > 0 && i < size)
    {
        data[i++] = fifo_buffer->data[fifo_buffer->read_index];
        fifo_buffer->used_bytes--;
        fifo_buffer->read_index++;
        fifo_buffer->read_index %= fifo_buffer->capacity;
    }

    return i;
}

int32_t fifobuffer_enqueue_from_other(FifoBuffer* fifo_buffer, FifoBuffer* other)
{
    uint32_t otherSize = fifobuffer_get_size(other);

    if (otherSize == 0)
    {
        return 0;
    }

    uint32_t bytes_available = fifo_buffer->capacity - fifo_buffer->used_bytes;

    uint32_t i = 0;
    while (fifo_buffer->used_bytes < fifo_buffer->capacity && other->used_bytes > 0)
    {
        fifo_buffer->data[fifo_buffer->write_index] = other->data[other->read_index];
        fifo_buffer->used_bytes++;
        fifo_buffer->write_index++;
        fifo_buffer->write_index %= fifo_buffer->capacity;

        other->used_bytes--;
        other->read_index++;
        other->read_index %= other->capacity;

        i++;
    }

    return (int32_t)i;
}