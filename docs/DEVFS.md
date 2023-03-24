# Device filesystem
DevFS (also known as Device filesystem) is a directory in the root of the filesystem called `dev`, which contains other files inside which represent hardware. For example, `/dev/fb0` represents the monitor's framebuffer, which can be used to draw graphics to the screen. Another example, is `/dev/tty`, which allows you to send and recieve from the terminal.
The DevFS is initialized using the function:
```c
void devfs_initialize();
```
You can only initialize DevFS if `fs_initialize` was called previously.
To register devices to the DevFS, you can use the following function:
```c
FileSystemNode* devfs_register_device(Device* device);
```