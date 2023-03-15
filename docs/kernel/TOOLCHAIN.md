# Building a cross-compiler for building the kernel
The kernel needs a `i686-elf` (or `i386-elf`) toolchain in order to be built correctly. The fyre repo comes with a shell script which builds you the toolchain (`scripts/i686-elf-toolchain.sh`). Running:
```
make kernel-toolchain
```
will build the cross-compiler needed for building the kernel.