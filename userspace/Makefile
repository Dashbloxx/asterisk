LIBC_OBJ = libc/syscall.o libc/string.o libc/stdio.o libc/math.o libc/stdlib.o libc/ctype.o libc/errno.o libc/unistd.o libc/crt0.o
OBJ = $(LIBC_OBJ) test.o sh.o

CC = i686-elf-gcc
LD = i686-elf-ld
CFLAGS = -nostdinc -std=gnu99 -ffreestanding -O2
LDFLAGS = -Tlink.ld

all: $(OBJ)
	$(LD) $(LDFLAGS) -o bin/test $(LIBC_OBJ) test.o
	$(LD) $(LDFLAGS) -o bin/sh $(LIBC_OBJ) sh.o

libc/syscall.o: libc/syscall.c
	$(CC) $(CFLAGS) -c libc/syscall.c -o libc/syscall.o

libc/string.o: libc/string.c
	$(CC) $(CFLAGS) -c libc/string.c -o libc/string.o

libc/stdio.o: libc/stdio.c
	$(CC) $(CFLAGS) -c libc/stdio.c -o libc/stdio.o

libc/math.o: libc/math.c
	$(CC) $(CFLAGS) -c libc/math.c -o libc/math.o

libc/stdlib.o: libc/stdlib.c
	$(CC) $(CFLAGS) -c libc/stdlib.c -o libc/stdlib.o

libc/crt0.o: libc/crt0.c
	$(CC) $(CFLAGS) -c libc/crt0.c -o libc/crt0.o

libc/unistd.o: libc/unistd.c
	$(CC) $(CFLAGS) -c libc/unistd.c -o libc/unistd.o

libc/crt0.o: libc/crt0.c
	$(CC) $(CFLAGS) -c libc/crt0.c -o libc/crt0.o

libc/ctype.o: libc/ctype.c
	$(CC) $(CFLAGS) -c libc/ctype.c -o libc/ctype.o

test.o: test.c
	$(CC) $(CFLAGS) -c test.c -o test.o

clean:
	rm *.o