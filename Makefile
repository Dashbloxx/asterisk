all: kernel iso

kernel:
	make -C kernel
clean:
	make -C kernel clean
iso:
	cp kernel.bin bootdisk-root/boot/
	cp initrd.fat bootdisk-root/boot/
	grub-mkrescue -o cdrom.iso bootdisk-root
test:
	qemu-system-i386 -cdrom cdrom.iso