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
 
#include "log.h"
#include "common.h"
#include "fs.h"

static File* g_file = NULL;

void log_initialize(const char* file_name)
{
    filesystem_node* node = fs_get_node(file_name);

    if (node)
    {
      g_file = fs_open(node, 0);
    }
}

void log_printf(const char *format, ...)
{
    char **arg = (char **) &format;
    char c;
    char buf[20];
    char buffer[512];

    int buffer_index = 0;

    //arg++;
    __builtin_va_list vl;
    __builtin_va_start(vl, format);

    while ((c = *format++) != 0)
      {
        if (buffer_index > 510)
        {
            break;
        }

        if (c != '%')
          buffer[buffer_index++] = c;
        else
          {
            char *p;

            c = *format++;
            switch (c)
              {
              case 'x':
                 buf[0] = '0';
                 buf[1] = 'x';
                 //itoa (buf + 2, c, *((int *) arg++));
                 itoa (buf + 2, c, __builtin_va_arg(vl, int));
                 p = buf;
                 goto string;
                 break;
              case 'd':
              case 'u':
                //itoa (buf, c, *((int *) arg++));
                itoa (buf, c, __builtin_va_arg(vl, int));
                p = buf;
                goto string;
                break;

              case 's':
                //p = *arg++;
                p = __builtin_va_arg(vl, char*);
                if (! p)
                  p = "(null)";

              string:
                while (*p)
                  buffer[buffer_index++] = (*p++);
                break;

              default:
                //buffer[buffer_index++] = (*((int *) arg++));
                buffer[buffer_index++] = __builtin_va_arg(vl, int);
                break;
              }
          }
      }

    buffer[buffer_index] = '\0';

    if (g_file)
    {
        fs_write(g_file, strlen(buffer), (uint8_t*)buffer);
    }

    __builtin_va_end(vl);
}
