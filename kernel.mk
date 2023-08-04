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

KERNEL_SOURCES_C = $(patsubst kernel/%.c,kernel/%.o,$(wildcard kernel/*.c))
KERNEL_SOURCES_ASM = $(patsubst kernel/%.asm,kernel/%.o,$(wildcard kernel/*.asm))
KERNEL_SOURCES_ARCH_ASM = $(patsubst kernel/arch/i386/%.asm,kernel/arch/i386/%.o,$(wildcard kernel/arch/i386/*.asm))
KERNEL_SOURCES_PSF = $(patsubst kernel/%.psf,kernel/%.o,$(wildcard kernel/*.psf))

CC = i686-elf-gcc
LD = i686-elf-ld
OBJCOPY = i686-elf-objcopy
CFLAGS = -c
LDFLAGS = -Tkernel/link.ld
ASFLAGS = -felf
CPFLAGS = -I binary -O elf32-i386 -B i386

KERNEL_OBJ = $(KERNEL_SOURCES_ARCH_ASM) $(KERNEL_SOURCES_ASM) $(KERNEL_SOURCES_C) $(KERNEL_SOURCES_PSF)

all: $(KERNEL_OBJ) link

clean:
	-rm kernel/*.o kernel/arch/i386/*.o kernel.bin

link:
	$(LD) $(LDFLAGS) -o kernel.bin $(KERNEL_OBJ)

kernel/%.o:kernel/%.c
	$(CC) $(CFLAGS) $< -o $@

kernel/%.o:kernel/%.asm
	nasm $(ASFLAGS) $< -o $@

kernel/arch/i386/%.o:kernel/arch/i386/%.asm
	nasm $(ASFLAGS) $< -o $@

kernel/%.o:kernel/%.psf
	$(OBJCOPY) $(CPFLAGS) $< $@