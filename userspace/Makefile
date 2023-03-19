OBJ = libc/syscall.o test.o

CC = i686-elf-gcc
LD = i686-elf-ld
CFLAGS = -nostdinc
LDFLAGS = -Tlink.ld

all: $(OBJ)
	$(LD) $(LDFLAGS) -o bin/test $(OBJ)

libc/syscall.o: libc/syscall.c libc/syscall.h
	$(CC) $(CFLAGS) -c libc/syscall.c -o libc/syscall.o

test.o: test.c libc/syscall.h
	$(CC) $(CFLAGS) -c test.c -o test.o