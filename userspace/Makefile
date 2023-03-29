LIBC_OBJ = libc/syscall.o libc/string.o libc/stdio.o libc/math.o libc/stdlib.o libc/errno.o libc/crt0.o
OBJ = $(LIBC_OBJ) sh.o cat.o

CC = i686-elf-gcc
LD = i686-elf-ld
CFLAGS = -nostdinc -std=gnu99 -ffreestanding -O2
LDFLAGS = -Tlink.ld

all: $(OBJ)
	$(LD) $(LDFLAGS) -o bin/sh $(LIBC_OBJ) sh.o
	$(LD) $(LDFLAGS) -o bin/cat $(LIBC_OBJ) cat.o

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

libc/errno.o: libc/errno.c
	$(CC) $(CFLAGS) -c libc/errno.c -o libc/errno.o

sh.o: sh.c
	$(CC) $(CFLAGS) -c sh.c -o sh.o

cat.o: cat.c
	$(CC) $(CFLAGS) -c cat.c -o cat.o

clean:
	rm *.o