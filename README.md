# Asterisk
## About
Asterisk is an operating system written in C. It uses some code from the discontinued project (asterisk)[https://github.com/ozkl/asterisk], and currently only supports `i386+` processors.
## Building
Now you can build asterisk's kernel by running `make kernel`. You will need a cross-compiler for this though, and luckily asterisk comes with a script that can build you a cross-compiler for building asterisk. See (this)[docs/TOOLCHAIN.md] page before building the kernel.
Running `make all` or simply `make` will build the kernel, and a CD-ROM image that can be used to test with QEMU. If you build the CD-ROM, it will copy a file called `initrd.fat` (which I took from (asterisk)[https://github.com/ozkl/asterisk] and can't be built right now).
## Testing
After building the kernel and making the CD-ROM image, you can test asterisk:
```
make test
```
## License
Asterisk is licensed under the BSD-2-Clause license.