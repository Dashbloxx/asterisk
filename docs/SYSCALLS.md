# Syscalls
## File-related system calls
### `SYS_open` & `SYS_close`
The `open` syscall allows a process to obtain access to a file. You can feed it a path to a file, and get an `int` value in return. This `int` value represents the file. When writing or reading to the file, you will just provide that `int` value that was given to you by the `open` syscall. You will later need to use the `close` syscall to declare that you no longer need access to that file that you asked the `open` syscall for.
### `SYS_read` & `SYS_write`
These two syscalls allow you to read and/or write to a file using the `int` value that the `open` syscall gives you.
### Example
This is an example that utilizes the file-related system calls:
```c
/*
 *  Open `/dev/tty`, so that we can write and read to the terminal...
 */
int fd = open("/dev/tty");
/*
 *  Let's print "Hello, world!" with a newline to the terminal...
 */
write(fd, "Hello, world!\n");
/*
 *  Let's now wait for user input, and after the input is sent, let's stop close the access to `/dev/tty` and exit...
 */
read(fd);
close(fd);
```