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
#include "timer.h"
#include "common.h"
#include "errno.h"
#include "log.h"
#include "isr.h"
#include "syscall_select.h"

void select_update(Thread* thread)
{
    Process* process = thread->owner;

    int total_ready = 0;
    uint32_t count = (uint32_t)thread->select.nfds;
    count = MIN(count, ASTERISK_MAX_OPENED_FILES);

    for (uint32_t fd = 0; fd < count; ++fd)
    {
        File* file = process->fd[fd];

        if (file)
        {
            if (FD_ISSET(fd, &thread->select.read_set))
            {
                if (file->node->read_test_ready && file->node->read_test_ready(file))
                {
                    FD_SET(fd, &thread->select.read_set_result);

                    ++total_ready;
                }
            }

            if (FD_ISSET(fd, &thread->select.write_set))
            {
                if (file->node->write_test_ready && file->node->write_test_ready(file))
                {
                    FD_SET(fd, &thread->select.write_set_result);

                    ++total_ready;
                }
            }
        }
    }

    if (total_ready > 0)
    {
        thread->select.result = total_ready;
        thread->select.select_state = SS_FINISHED;
    }
    else if (thread->select.target_time > 0)
    {
        time_t now = get_uptime_milliseconds64();

        if (now > thread->select.target_time)
        {
            thread->select.result = 0;
            thread->select.select_state = SS_FINISHED;
        }
    }
}

static int select_finish(Thread* thread, fd_set* rfds, fd_set* wfds)
{
    if (rfds)
    {
        *rfds = thread->select.read_set_result;
    }

    if (wfds)
    {
        *wfds = thread->select.write_set_result;
    }

    int result = thread->select.result;
    memset((uint8_t*)&thread->select, 0, sizeof(thread->select));

    return result;
}

int syscall_select(int n, fd_set* rfds, fd_set* wfds, fd_set* efds, struct timeval* tv)
{
    if (!check_user_access(rfds))
    {
        return -EFAULT;
    }

    if (!check_user_access(wfds))
    {
        return -EFAULT;
    }

    if (!check_user_access(efds))
    {
        return -EFAULT;
    }

    if (!check_user_access(tv))
    {
        return -EFAULT;
    }

    Thread* thread = thread_get_current();
    Process* process = thread->owner;

    if (process)
    {
        thread->select.select_state = SS_STARTED;
        thread->select.nfds = n;
        FD_ZERO(&thread->select.read_set_result);
        FD_ZERO(&thread->select.write_set_result);

        FD_ZERO(&thread->select.read_set);
        if (rfds)
        {
            thread->select.read_set = *rfds;
            FD_ZERO(rfds);
        }

        FD_ZERO(&thread->select.write_set);
        if (wfds)
        {
            thread->select.write_set = *wfds;
            FD_ZERO(wfds);
        }

        thread->select.result = -1;

        thread->select.target_time = 0;
        if (tv)
        {
            thread->select.target_time = get_uptime_milliseconds64() + tv->tv_sec * 1000 + tv->tv_usec / 1000;
        }

        while (TRUE)
        {
            disable_interrupts();

            select_update(thread);

            if (thread->select.select_state == SS_FINISHED)
            {
                int result = select_finish(thread, rfds, wfds);

                thread_resume(thread);

                return result;
            }

            thread_change_state(thread, TS_SELECT, NULL);
            enable_interrupts();
            halt();
        }
    }

    return -1;
}