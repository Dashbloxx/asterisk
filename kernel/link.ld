/*
 *      This file is very essential; it allows us to map where different aspects of the kernel are placed in memory. It also 
 *      allows us to set the entry function, which is `_start` (you can find the definition of `_start` in `boot.asm`).
 */

ENTRY(_start)

SECTIONS
{
        . = 1M;

        /* Code. */
        .text BLOCK(4K) : ALIGN(4K)
        {
                *(.multiboot)
                *(.text)
        }

        /* Read-only data. */
        .rodata BLOCK(4K) : ALIGN(4K)
        {
                *(.rodata)
        }

        /* Read-write data (initialized) */
        .data BLOCK(4K) : ALIGN(4K)
        {
                *(.data)
        }

        /* Read-write data (uninitialized) and stack */
        .bss BLOCK(4K) : ALIGN(4K)
        {
                *(COMMON)
                *(.bss)
        }

       _end = .;
}
