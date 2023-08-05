# Building
## About
Building Asterisk is actually not a hard process. First you build the kernel, and then you build the userspace binaries, which can be placed into the initrd. After the kernel and the initrd is set up, you have finished building the project from source. Each step needed for building Asterisk is listed in order below.
## Building the toolchain
In order to build the project successfully, we need a cross compiler, which generates code for the targeted platform. Building the toolchain is fairly easy, and all you have to do is run the following command in the root directory of this project:
```
scripts/i686-elf-toolchain.sh
```
Building the toolchain may take a while, but not too much. After that is done, you will want to add the toolchain to the PATH environment variable:
```
export PATH="/opt/i686-elf-cross/bin:$PATH"
```
Add the command above to the end of `~/.bashrc` if you haven't yet, so that you don't have to run this everytime that you need to run this command every time that you restart your environment. If you added it to the end of `~/.bashrc`, make sure to restart BASH so that the toolchain executes normally.
## Building the kernel
After you have the toolchain built & setted up correctly, building the kernel will be even easier. To build the kernel, simply run the following command in the root directory of the repo:
```
make -f kernel.mk
```
This builds the kernel, which can be found in the root directory of the project/repo as `kernel.bin`.
## Building the libc
Asterisk comes with a custom standard C library, so you'll need that to build it's userland binaries unless if you are porting newlib to Asterisk. To build Asterisk's standard C library, just run:
```
make -f libc.mk
```
## Building the userspace programs
Building the userspace binaries/programs is as easy as building the kernel. All you need to do is run the following command in the root directory of the project/repo:
```
make -f userland.mk
```
## Making the initrd (Initial RAMDisk)
The initrd is the filesystem that the kernel loads for basic programs, (in the future it may be simple programs/drivers that help load the persistent filesystem). To build the initrd, you will need sudo privileges, and will need to make sure that theres a directory in the root of your filesystem called `mnt`. After that, simply run the following command in the root directory of the project/repo:
```
sudo scripts/initrd.sh
```
## Testing Asterisk
Now, you can test Asterisk using QEMU by running the following command in the root directory of your repo/project:
```
qemu-system-i386 -kernel kernel.bin -initrd initrd.fat
```
If you want to test Asterisk on real hardware, you can get a bootloader like [GRUB](https://www.gnu.org/software/grub/) or [limine](https://limine-bootloader.org/). Making a bootable USB drive that can run Asterisk will be documented later.