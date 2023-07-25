CC := i686-elf-gcc
CFLAGS := -nostdlib -ffreestanding -O2 -m32 -Ilibc
LDFLAGS := -L. -lc
LIBC := libc.a

USERLAND_DIR := userland
INITRD_DIR := initrd

USERLAND_SOURCES_C := $(wildcard $(USERLAND_DIR)/*.c)
USERLAND_BINARIES := $(patsubst $(USERLAND_DIR)/%.c,$(INITRD_DIR)/%,$(USERLAND_SOURCES_C))

.PHONY: all clean

all: $(INITRD_DIR) $(USERLAND_BINARIES)

$(INITRD_DIR):
	mkdir -p $(INITRD_DIR)

$(INITRD_DIR)/%: $(USERLAND_DIR)/%.c $(LIBC)
	$(CC) $(CFLAGS) -T userland/link.ld -o $@ $< $(LDFLAGS)

clean:
	rm -rf $(INITRD_DIR)
