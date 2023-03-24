LIBC_OBJ = libc/syscall.o libc/string.o libc/stdio.o libc/stdint.o libc/crt0.o
OBJ = $(LIBC_OBJ) test.o

CC = i686-elf-gcc
LD = i686-elf-ld
CFLAGS = -nostdinc
LDFLAGS = -Tlink.ld

all: $(OBJ)
	$(LD) $(LDFLAGS) -o bin/test $(LIBC_OBJ) test.o
	$(LD) $(LDFLAGS) -o bin/log $(LIBC_OBJ) log.o

libc/syscall.o: libc/syscall.c
	$(CC) $(CFLAGS) -c libc/syscall.c -o libc/syscall.o

libc/string.o: libc/string.c
	$(CC) $(CFLAGS) -c libc/string.c -o libc/string.o

libc/stdio.o: libc/stdio.c
	$(CC) $(CFLAGS) -c libc/stdio.c -o libc/stdio.o

libc/stdint.o: libc/stdint.c
	$(CC) $(CFLAGS) -c libc/stdint.c -o libc/stdint.o

libc/crt0.o: libc/crt0.c
	$(CC) $(CFLAGS) -c libc/crt0.c -o libc/crt0.o

test.o: test.c
	$(CC) $(CFLAGS) -c test.c -o test.o