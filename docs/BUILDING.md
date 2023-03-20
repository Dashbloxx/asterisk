# Building
## About
Building Asterisk is actually not a hard process. First you build the kernel, and then you build the userspace binaries, which can be placed into the initrd. After the kernel and the initrd is set up, you have finished building the project from source. Each step needed for building Asterisk is listed in order below.
## Building the toolchain
In order to build the project successfully, we need a cross compiler, which generates code for the targeted platform. Building the toolchain is fairly easy, and all you have to do is run the following command in the root directory of this project:
```
make toolchain
```
Building the toolchain may take a while, but not too much. After that is done, you will want to add the toolchain to the PATH environment variable:
```
export PATH="/opt/i686-elf-cross/bin:$PATH"
```
Add the command above to the end of `~/.bashrc` if you haven't yet, so that you don't have to run this everytime that you need to run this command every time that you restart your environment. If you added it to the end of `~/.bashrc`, make sure to restart BASH so that the toolchain executes normally.
## Building the kernel
After you have the toolchain built & setted up correctly, building the kernel will be even easier. To build the kernel, simply run the following command in the root directory of the repo:
```
make kernel
```
This builds the kernel, which can be found in the root directory of the project/repo as `kernel.bin`.
## Building the userspace programs
Building the userspace binaries/programs is as easy as building the kernel. All you need to do is run the following command in the root directory of the project/repo:
```
make userspace
```
## Making the initrd (Initial RAMDisk)
The initrd is the filesystem that the kernel loads for basic programs, (in the future it may be simple programs/drivers that help load the persistent filesystem). To build the initrd, you will need sudo privileges, and will need to make sure that theres a directory in the root of your filesystem called `mnt`. After that, simply run the following command in the root directory of the project/repo:
```
sudo make initrd
```
## Testing Asterisk
Now, you can test Asterisk using QEMU by running the following command in the root directory of your repo/project:
```
make test
```
The only issue, is that the framebuffer is not available. If you want access to the framebuffer, then you will need to build a CD-ROM image, because when building the CD-ROM image, it comes with the GRUB bootloader, which supplies us with help setting up video-related stuff.
## Building a bootable CD-ROM image
To build a bootable CD-ROM image (which can be tested with real hardware!), simply run the following command in the root directory of this repo/project:
```
make iso
```
You will get a file named `asterisk.iso`, which you can flash to a CD, DVD, or a USB drive. Flashing these CD-ROM images to a USB drive will be documented later aswell as testing on real hardware.