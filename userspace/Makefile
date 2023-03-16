SOURCES_C=$(patsubst %.c,%.o,$(wildcard ./*.c))


CC=i686-elf-gcc
LD=i686-elf-ld
CFLAGS=-std=gnu99 -O2 -Wall -Wextra
LDFLAGS=-O2 -nostdlib

OBJ = $(SOURCES_C)

all: $(OBJ)

clean:
	-rm *.o
	-rm -rf bin

%.o:%.c
	mkdir -p bin
	$(CC) -c $< -o $@ $(CFLAGS)
	$(LD) -Tlink.ld -o bin/$* $(LDFLAGS) $@
	$(LD) $@ $(LDFLAGS) -o bin/$*