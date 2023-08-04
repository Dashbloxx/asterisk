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
CFLAGS := -nostdlib -ffreestanding -O2 -m32 -Ilibc
LDFLAGS := -L. -lc

USERLAND_DIR := userland
INITRD_DIR := initrd

USERLAND_SOURCES_C := $(wildcard userland/*.c)
USERLAND_BINARIES := $(patsubst userland/%.c,initrd/%,$(USERLAND_SOURCES_C))

.PHONY: all clean

all: $(INITRD_DIR) $(USERLAND_BINARIES)

$(INITRD_DIR):
	mkdir -p $(INITRD_DIR)

$(INITRD_DIR)/%: $(USERLAND_DIR)/%.c libc.a
	$(CC) $(CFLAGS) -T userland/link.ld -o $@ $< $(LDFLAGS)

clean:
	rm -rf $(INITRD_DIR)
