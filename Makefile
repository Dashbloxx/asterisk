all:
	make -C kernel all
	make -C userspace all
	sudo scripts/initrd.sh

kernel:
	make -C kernel all

initrd:
	scripts/initrd.sh

clean:
	make -C kernel clean

userspace:
	make -C userspace all

test:
	qemu-system-i386 -kernel kernel.bin -initrd initrd.fat

toolchain:
	scripts/i686-elf-toolchain.sh
