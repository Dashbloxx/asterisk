# Fyre
## About
Fyre is an operating system written in C that uses code from (soso)[https://github.com/ozkl/soso].
## Building
Now you can build fyre's kernel by running `make kernel`. The kernel builds without issues if you build with the `i686-elf` toolchain.
Running `make all` or simply `make` will build the kernel, and a CD-ROM image that can be used to test with QEMU. If you build the CD-ROM, it will copy a file called `initrd.fat` (which I took from (soso)[https://github.com/ozkl/soso] and can't be built right now).
## Testing
After building the kernel and making the CD-ROM image, you can test fyre:
```
make test
```
## License
Fyre is licensed under the BSD-2-Clause license.