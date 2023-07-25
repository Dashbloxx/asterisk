KERNEL_SOURCES_C = $(patsubst kernel/%.c,kernel/%.o,$(wildcard kernel/*.c))
KERNEL_SOURCES_ASM = $(patsubst kernel/%.asm,kernel/%.o,$(wildcard kernel/*.asm))
KERNEL_SOURCES_PSF = $(patsubst kernel/%.psf,kernel/%.o,$(wildcard kernel/*.psf))

CC = i686-elf-gcc
LD = i686-elf-ld
OBJCOPY = i686-elf-objcopy
CFLAGS = -c
LDFLAGS = -Tkernel/link.ld
ASFLAGS = -felf
CPFLAGS = -I binary -O elf32-i386 -B i386

KERNEL_OBJ = $(KERNEL_SOURCES_ASM) $(KERNEL_SOURCES_C) $(KERNEL_SOURCES_PSF)

all: $(KERNEL_OBJ) kernel.bin

clean:
	-rm kernel/*.o kernel.bin

kernel.bin:
	$(LD) $(LDFLAGS) -o $@ $(KERNEL_OBJ)

kernel/%.o:kernel/%.c
	$(CC) $(CFLAGS) $< -o $@

kernel/%.o:kernel/%.asm
	nasm $(ASFLAGS) $< -o $@

kernel/%.o:kernel/%.psf
	$(OBJCOPY) $(CPFLAGS) $< $@