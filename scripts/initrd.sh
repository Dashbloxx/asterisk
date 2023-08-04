#
#     dP      Asterisk is an operating system written fully in C and Intel-syntax
# 8b. 88 .d8  assembly. It strives to be POSIX-compliant, and a faster & lightweight
#  `8b88d8'   alternative to Linux for i386 processors.
#  .8P88Y8.   
# 8P' 88 `Y8  
#     dP      
#
# BSD 2-Clause License
# Copyright (c) 2023 Nexuss
# All rights reserved.
#

#!/bin/bash

dd if=/dev/zero of=initrd.fat bs=8M count=1
LOOP=`losetup -f`
sudo losetup $LOOP initrd.fat
sudo mkfs.vfat $LOOP
sudo mount $LOOP /mnt
cp -r initrd/* /mnt
sudo umount /mnt
sudo losetup -d $LOOP
sync
sudo chown $SUDO_USER:$SUDO_USER initrd.fat