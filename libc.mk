#
#     dP      Asterisk is an operating system written fully in C and Intel-syntax
# 8b. 88 .d8  assembly. It strives to be POSIX-compliant, and a faster & lightweight
#  `8b88d8'   alternative to Linux for i386 processors.
#  .8P88Y8.   
# 8P' 88 `Y8  
#     dP      
#
# BSD 2-Clause License
# Copyright (c) 2023 Nexuss
# All rights reserved.
#

CC := i686-elf-gcc
AR := i686-elf-ar
CFLAGS := -nostdinc -std=gnu99 -ffreestanding -O2

LIBC_SOURCES_C := $(wildcard libc/*.c)
LIBC_OBJ := $(patsubst libc/%.c,libc/%.o,$(LIBC_SOURCES_C))

libc.a: $(LIBC_OBJ)
	$(AR) rcs $@ $^

libc/%.o: libc/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f libc/*.o libc.a
