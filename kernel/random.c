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
 
#include "random.h"
#include "devfs.h"
#include "device.h"
#include "common.h"
#include "process.h"
#include "sleep.h"

static BOOL random_open(File *file, uint32_t flags);
static int32_t random_read(File *file, uint32_t size, uint8_t *buffer);

void random_initialize()
{
    Device device;
    memset((uint8_t*)&device, 0, sizeof(Device));
    strcpy(device.name, "random");
    device.device_type = FT_CHARACTER_DEVICE;
    device.open = random_open;
    device.read = random_read;

    devfs_register_device(&device);
}

static BOOL random_open(File *file, uint32_t flags)
{
    return TRUE;
}

static int32_t random_read(File *file, uint32_t size, uint8_t *buffer)
{
    if (size == 0)
    {
        return 0;
    }

    uint32_t number = rand();

    if (size == 1)
    {
        *buffer = (uint8_t)number;
        return 1;
    }
    else if (size == 2 || size == 3)
    {
        *((uint16_t*)buffer) = (uint16_t)number;
        return 2;
    }
    else if (size >= 4)
    {
        //Screen_PrintF("random_read: buffer is %x, writing %x to buffer\n", buffer, number);

        *((uint32_t*)buffer) = number;
        return 4;
    }

    return 0;
}
