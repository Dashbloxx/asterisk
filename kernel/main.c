#include "descriptortables.h"
#include "timer.h"
#include "multiboot.h"
#include "fs.h"
#include "syscalls.h"
#include "serial.h"
#include "isr.h"
#include "vmm.h"
#include "alloc.h"
#include "process.h"
#include "keyboard.h"
#include "devfs.h"
#include "systemfs.h"
#include "pipe.h"
#include "sharedmemory.h"
#include "random.h"
#include "null.h"
#include "elf.h"
#include "log.h"
#include "ramdisk.h"
#include "fatfilesystem.h"
#include "vbe.h"
#include "fifobuffer.h"
#include "gfx.h"
#include "mouse.h"
#include "sleep.h"
#include "console.h"
#include "terminal.h"
#include "socket.h"

extern uint32_t _start;
extern uint32_t _end;
uint32_t g_physical_kernel_start_address = (uint32_t)&_start;
uint32_t g_physical_kernel_end_address = (uint32_t)&_end;

static void* locate_initrd(struct Multiboot *mbi, uint32_t* size)
{
    if (mbi->mods_count > 0)
    {
        uint32_t start_location = *((uint32_t*)mbi->mods_addr);
        uint32_t end_location = *(uint32_t*)(mbi->mods_addr + 4);

        *size = end_location - start_location;

        return (void*)start_location;
    }

    return NULL;
}

int execute_file(const char *path, char *const argv[], char *const envp[], filesystem_node* tty)
{
    int result = -1;

    Process* process = thread_get_current()->owner;
    if (process)
    {
        filesystem_node* node = fs_get_node_absolute_or_relative(path, process);
        if (node)
        {
            File* f = fs_open(node, 0);
            if (f)
            {
                void* image = kmalloc(node->length);

                int32_t bytes_read = fs_read(f, node->length, image);

                if (bytes_read > 0)
                {
                    char* name = "userProcess";
                    if (NULL != argv && NULL != argv[0])
                    {
                        name = argv[0];
                    }
                    Process* new_process = process_create_from_elf_data(name, image, argv, envp, process, tty);

                    if (new_process)
                    {
                        result = new_process->pid;
                    }
                }
                fs_close(f);

                kfree(image);
            }

        }
    }

    return result;
}

/*
 *  Here we're using the multiboot protocol, which most bootloaders support. Multiboot allows us to have the
 *  bootloader fetch us some information (which we can't get as the kernel) while jumping to the kernel.
 */
int kmain(struct Multiboot *mboot_ptr)
{
    int stack = 5;

    /*
     *  Initialize the GDT (Global Descriptor Table), and other descriptor tables. These allow the kernel to
     *  describe certain segments of memory & other stuff to the CPU.
     */
    descriptor_tables_initialize();

    uint32_t memory_kb = mboot_ptr->mem_upper;//96*1024;
    vmm_initialize(memory_kb);

    /*
     *  Initialize the general filesystem-related stuff, and initialize devfs, which are files that don't
     *  contain anything, but allow us to recieve and send data to...
     */
    fs_initialize();
    devfs_initialize();

    /*
     *  Determine if we're able to use graphics...
     *  If we're testing with QEMU, we won't have graphics, but if a bootloader is combined, we most likely
     *  will have some sort of graphics...
     */
    BOOL graphics_mode = (MULTIBOOT_FRAMEBUFFER_TYPE_RGB == mboot_ptr->framebuffer_type);

    /*
     *  If graphics are available, let's initialize kernel's side of graphics handling...
     */
    if (graphics_mode)
    {
        gfx_initialize((uint32_t*)(uint32_t)mboot_ptr->framebuffer_addr, mboot_ptr->framebuffer_width, mboot_ptr->framebuffer_height, mboot_ptr->framebuffer_bpp / 8, mboot_ptr->framebuffer_pitch);
    }

    /*
     *  Let's initialize the console, and while doing that let's pass whether or not graphics are available
     *  to the console, so that it can do it's own thing. See `console.c` & `console.h`...
     */
    console_initialize(graphics_mode);

    /*
     *  Let's print out when the kernel was built. GCC's preprocessor sets these values right before compiling
     *  the kernel...
     */
    printkf("Kernel built on %s %s\n", __DATE__, __TIME__);
    
    /*
     *  Let's initialize `systemfs`. See `systemfs.c` & `systemfs.h` for the code regarding systemfs. I am
     *  still reading this code, and will later document what it does.
     */
    systemfs_initialize();

    pipe_initialize();
    sharedmemory_initialize();

    tasking_initialize();

    /*
     *  Initialize syscalls. Syscalls are useful, because they allow userspace binaries to call certain
     *  functions from the kernel while running with limited permissions.
     *  See `syscalls.c` & `syscalls.h` aswell as `syscalltable.h`. The first two files contain code defining
     *  basic syscalls, and the third file contains a list of syscalls. Reading this code will make the process
     *  of porting a libc thousands of times easier...
     */
    syscalls_initialize();

    timer_initialize();

    keyboard_initialize();
    initialize_mouse();

    if (0 != mboot_ptr->cmdline)
    {
        printkf("Kernel cmdline:%s\n", (char*)mboot_ptr->cmdline);
    }

    serial_initialize();

    log_initialize("/dev/com1");
    //log_initialize("/dev/ptty9");

    log_printf("Kernel built on %s %s\r\n", __DATE__, __TIME__);

    random_initialize();
    null_initialize();

    ramdisk_create("ramdisk1", 20*1024*1024);

    fatfs_initialize();

    /*
     *  Initialize sockets (UNIX sockets). This allows different processes to communicate with each other. Do
     *  not confuse this with TCP/IP communication!
     */
    net_initialize();

    printkf("System started!\n");

    char* argv[] = {"shell", NULL};
    char* envp[] = {"HOME=/", "PATH=/initrd", NULL};

    uint32_t initrd_size = 0;
    uint8_t* initrd_location = locate_initrd(mboot_ptr, &initrd_size);
    uint8_t* initrd_end_location = initrd_location + initrd_size;
    if (initrd_location == NULL)
    {
        PANIC("Initrd not found!\n");
    }
    else
    {
        printkf("Initrd found at %x - %x (%d bytes)\n", initrd_location, initrd_end_location, initrd_size);
        if ((uint32_t)KERN_PD_AREA_BEGIN < (uint32_t)initrd_end_location)
        {
            printkf("Initrd must reside below %x !!!\n", KERN_PD_AREA_BEGIN);
            PANIC("Initrd image is too big!");
        }
        memcpy((uint8_t*)*(uint32_t*)fs_get_node("/dev/ramdisk1")->private_node_data, initrd_location, initrd_size);
        BOOL mountSuccess = fs_mount("/dev/ramdisk1", "/initrd", "fat", 0, 0);

        if (mountSuccess)
        {
            /*
             *  Run a shell for each open TTY...
             */
            execute_file("/initrd/shell", argv, envp, fs_get_node("/dev/ptty1"));
            execute_file("/initrd/shell", argv, envp, fs_get_node("/dev/ptty2"));
            execute_file("/initrd/shell", argv, envp, fs_get_node("/dev/ptty3"));
            execute_file("/initrd/shell", argv, envp, fs_get_node("/dev/ptty4"));
            execute_file("/initrd/shell", argv, envp, fs_get_node("/dev/ptty7"));

            /*
             *  Let's run our logging service on the COM1 serial port so that we can see some logs through QEMU!
             */
            execute_file("/initrd/log", argv, envp, fs_get_node("/dev/com1"));
        }
        else
        {
            PANIC("Mounting initrd failed!\n");
        }
    }

    pipe_create("pipe0", 8);

    scheduler_enable();

    enable_interrupts();

    while(TRUE)
    {
        //Idle thread

        halt();
    }

    return 0;
}
