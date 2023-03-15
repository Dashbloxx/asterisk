all:
	make -C kernel all
	cp kernel.bin bootdisk-root/boot/
	cp initrd.fat bootdisk-root/boot/
	grub-mkrescue -o asterisk.iso bootdisk-root

kernel:
	make -C kernel all

initrd:
	dd if=/dev/zero of=initrd.fat bs=8M count=1
	LOOP=`losetup -f`
	sudo losetup $LOOP initrd.fat
	sudo mkfs.vfat $LOOP
	sudo mount $LOOP mnt
	cp -r userspace/bin/* mnt/
	sudo umount mnt
	sudo losetup -d $LOOP
	sync
	sudo chown $SUDO_USER:$SUDO_USER initrd.fat

clean:
	make -C kernel clean

iso:
	cp kernel.bin bootdisk-root/boot/
	cp initrd.fat bootdisk-root/boot/
	grub-mkrescue -o asterisk.iso bootdisk-root

test:
	qemu-system-i386 -accel kvm -cdrom asterisk.iso

kernel-toolchain:
	scripts/i686-elf-toolchain.sh