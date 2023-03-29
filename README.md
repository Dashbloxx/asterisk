![](.assets/banner.png)
## About
Asterisk (also known as 386✱) is an operating system written in C, inspired by MINIX. It uses some code from the discontinued project [soso](https://github.com/ozkl/soso), and currently only supports `i386+` processors, but in the future Asterisk will support way more architectures.
For now, our goal is to get command-line arguments working, as well as commenting and documenting the code as much as possible.
## Building
Read [BUILDING.md](docs/BUILDING.md) for learning how to build Asterisk.
## Testing
After building the kernel and making the CD-ROM image, you can test Asterisk:
```
make test
```
## Screenshot
![](.assets/screenshot0.png)
*Asterisk running with [soso](https://github.com/ozkl/soso)'s initrd!*
## Contributors
When I took code from [soso](https://github.com/ozkl/soso), I also took some code from the pull requests that fixed different aspects of the kernel. Although they may not show up in the GitHub repository's contributor list, I will still add them here:
* [Dashbloxx](https://github.com/Dashbloxx) for creating this project
* [rilysh](https://github.com/rilysh) [for this](https://github.com/ozkl/soso/pull/10)
* [sk-io](https://github.com/sk-io) [for this](https://github.com/ozkl/soso/pull/9)
## License
Asterisk is licensed under the BSD-2-Clause license.
