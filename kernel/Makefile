SOURCES_C=$(patsubst %.c,%.o,$(wildcard *.c))
SOURCES_ASM=$(patsubst %.asm,%.o,$(wildcard *.asm))

CC=i686-elf-gcc
LD=i686-elf-ld
CFLAGS=-c
LDFLAGS=-Tlink.ld
ASFLAGS=-felf

OBJ = $(SOURCES_ASM) $(SOURCES_C)

all: $(OBJ) link

clean:
	-rm *.o ../kernel.bin

link:
	$(LD) $(LDFLAGS) -o ../kernel.bin $(OBJ) ../font/font.o

%.o:%.c
	$(CC) $(CFLAGS) $< -o $@

%.o:%.asm
	nasm $(ASFLAGS) $< -o $@
