/*
 *      dP      Asterisk is an operating system written fully in C and Intel-syntax
 *  8b. 88 .d8  assembly. It strives to be POSIX-compliant, and a faster & lightweight
 *   `8b88d8'   alternative to Linux for i386 processors.
 *   .8P88Y8.   
 *  8P' 88 `Y8  
 *      dP      
 *
 *  Copyright (c) 2023 Nexuss
 *  All rights reserved.
 */

#pragma once

/*
 *  This header contains type definitions for important types that may be used to store time, size of stuff, IDs, etc.
 */

/*
 *  The types `size_t` and `ssize_t` are used for measuring purposes (obviously). The type `size_t is an unsigned integer, meaning that
 *  it cannot be negative, and the type `ssize_t` is a signed integer, meaning that it can be either negative or positive.
 */
typedef unsigned int size_t;
typedef int ssize_t;

/* This type is useful for storing the time since the epoch */
typedef int time_t;

/* This type is used for storing file offsets */
typedef int off_t;

/* This type stores process IDs */
typedef int pid_t;

/* This type stores user IDs */
typedef unsigned int uid_t;

/* This type stores group IDs */
typedef unsigned int uid_t;

/* This type stores file modes & permissions */
typedef unsigned int mode_t;

/* This type stores device IDs */
typedef unsigned int dev_t;

/* This type stores "inode numbers" */
typedef unsigned int ino_t;

/* This type is "used for0 file link counts" */
typedef unsigned short nlink_t;