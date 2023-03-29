# Device filesystem
DevFS (also known as Device filesystem) is a directory in the root of the filesystem called `dev`, which contains other files inside which represent hardware. For example, `/dev/fb0` represents the monitor's framebuffer, which can be used to draw graphics to the screen. Another example, is `/dev/tty`, which allows you to send and recieve from the terminal.
The DevFS is initialized using the function:
```c
void devfs_initialize();
```
You can only initialize DevFS if `fs_initialize` was called previously.
To register devices to the DevFS, you can use the following function:
```c
filesystem_node* devfs_register_device(Device* device);
```
This will return you the node of the character device that is registered using the device, and therefore it requires a pointer to a struct that is based on the device that you register to the DevFS.
Here is an example of a device being register to the DevFS (from `framebuffer.c`):
```c
g_fb_physical = p_address;
g_fb_virtual = v_address;

Device device;
memset((uint8_t*)&device, 0, sizeof(Device));
strcpy(device.name, "fb0");
device.device_type = FT_CHARACTER_DEVICE;
device.open = fb_open;
device.read = fb_read;
device.write = fb_write;
device.ioctl = fb_ioctl;
device.mmap = fb_mmap;
device.munmap = fb_munmap;

devfs_register_device(&device);
```
In other words, the DevFS is a directory which contains files that depict actual hardware/modules.