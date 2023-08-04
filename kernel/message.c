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
 
#include "message.h"
#include "process.h"
#include "fifobuffer.h"


void message_send(Thread* thread, AsteriskMessage* message)
{
    spinlock_lock(&(thread->message_queue_lock));

    fifobuffer_enqueue(thread->message_queue, (uint8_t*)message, sizeof(AsteriskMessage));

    spinlock_unlock(&(thread->message_queue_lock));
}

uint32_t message_get_queue_count(Thread* thread)
{
    int result = 0;

    spinlock_lock(&(thread->message_queue_lock));

    result = fifobuffer_get_size(thread->message_queue) / sizeof(AsteriskMessage);

    spinlock_unlock(&(thread->message_queue_lock));

    return result;
}

//returns remaining message count
int32_t message_get_next(Thread* thread, AsteriskMessage* message)
{
    uint32_t result = -1;

    spinlock_lock(&(thread->message_queue_lock));

    result = fifobuffer_get_size(thread->message_queue) / sizeof(AsteriskMessage);

    if (result > 0)
    {
        fifobuffer_dequeue(thread->message_queue, (uint8_t*)message, sizeof(AsteriskMessage));

        --result;
    }
    else
    {
        result = -1;
    }

    spinlock_unlock(&(thread->message_queue_lock));

    return result;
}
