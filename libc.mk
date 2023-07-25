CC := i686-elf-gcc
AR := i686-elf-ar
CFLAGS := -nostdinc -std=gnu99 -ffreestanding -O2

LIBC_SOURCES_C := $(wildcard libc/*.c)
LIBC_OBJ := $(patsubst libc/%.c,libc/%.o,$(LIBC_SOURCES_C))

libc.a: $(LIBC_OBJ)
	$(AR) rcs $@ $^

libc/%.o: libc/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f libc/*.o libc.a
