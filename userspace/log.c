#include "libc/syscall.h"
#include "libc/syscalltable.h"
#include "libc/stdio.h"

#define VERSION "1.0"

void main(int argc, char* const argv[], char* const envp[]) {
    printf("\x1b[30;47m .d8b.  .d8888. d888888b d88888b d8888b. d888888b .d8888. db   dD \r\nd8' `8b 88'  YP `~~88~~' 88'     88  `8D   `88'   88'  YP 88 ,8P' \r\n88ooo88 `8bo.      88    88ooooo 88oobY'    88    `8bo.   88,8P   \r\n88~~~88   `Y8b.    88    88~~~~~ 88`8b      88      `Y8b. 88`8b   \r\n88   88 db   8D    88    88.     88 `88.   .88.   db   8D 88 `88. \r\nYP   YP `8888Y'    YP    Y88888P 88   YD Y888888P `8888Y' YP   YD \r\n\x1b[0mThanks for using the Asterisk operating system!\r\nVersion: " VERSION "\r\nRepository: https://github.com/Dashbloxx/asterisk\r\n");
    while(1)
    {
        /*
         *  Infinite loop where we just wait & wait until we get a message from the kernel or another process. When we do get a message,
         *  we just print it out...
         */
    }
}