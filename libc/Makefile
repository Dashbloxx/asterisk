SOURCES_C=$(patsubst %.c,%.o,$(wildcard *.c))
SOURCES_ASM=$(patsubst %.S,%.o,$(wildcard *.S))

CC=i686-elf-gcc
AR=i686-elf-ar
CFLAGS=-c
ARFLAGS=rcs ../libc.a -Tlink.ld

OBJ = $(SOURCES_ASM) $(SOURCES_C)

all: $(OBJ) lib

clean:
	-rm *.o ../libc.a

lib:
	$(AR) $(ARFLAGS) $(OBJ)

%.o:%.c
	$(CC) $(CFLAGS) $< -o $@

%.o:%.S
	$(CC) $(CFLAGS) $< -o $@
