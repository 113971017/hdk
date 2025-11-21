/*

 * Software Platform Linker script file

 *

 * Copyright (C) 2016-2020 Melexis N.V.

 *

 */
OUTPUT_FORMAT("elf32-mlx16-ex")
OUTPUT_ARCH(mlx16ex)

/*

 * Default libraries which will be used for linking

 * Any additional libraries could be provided either through command

 * line option or in linker script

 */
GROUP(-lc -lgcc) /* Standard library */

EXTERN(__vectors) /* Force searching symbol in libraries if not defined */

MEMORY
{
    scratchpad (rwx) : ORIGIN = 0x0170, LENGTH = 48-1 /* one byte (SCRATCHPAD_BYTE47) is reserved for the boot code */
    eeprom (rwx) : ORIGIN = 0x1000, LENGTH = 64 /* customer area */
    ram (rwx) : ORIGIN = 0x2000, LENGTH = 512
    rom (rx) : ORIGIN = 0x4000, LENGTH = 2K
    flash_latches (rwx) : ORIGIN = 0x5fc0, LENGTH = 64
    flash_config (rx) : ORIGIN = 0xA000, LENGTH = 128 /* configuration pages */
}


/* Program memory configuration for application when Loader is enabled */
MEMORY
{
    flash (rx) : ORIGIN = 0x6000+1k*3, LENGTH = 16k-1k*3 -42+3*2
}

__flash_user_mem_start = ABSOLUTE(ORIGIN(flash)); /* start address of user area */
/* Define direct, extra and io page addresses */
__io__ = 0x0000; /* io page,    max 64 bytes */
__ep__ = ABSOLUTE(ORIGIN(eeprom)); /* extra page, max 256 bytes */
__dp__ = ABSOLUTE(ORIGIN(ram)); /* dp page,    max 256 bytes */

__fp0__ = ABSOLUTE(ORIGIN(rom)); /* interrupt vector table */
__fp1__ = __fp0__ + 0x100;
__fp2__ = __fp0__ + 0x200;
__fp3__ = __fp0__ + 0x300;
__fp4__ = __fp0__ + 0x400;
__fp5__ = __fp0__ + 0x500;
__fp6__ = __fp0__ + 0x600;
__fp7__ = __dp__ + 0x100;

/* _nvram_mlx = __ep__ + 64; => assigned in symb-addr.ld */
_nvram_mlx_data_first_word = __ep__ + 64 + 4; /* address of the first word of the Melexis data in the nvRAM */
_nvram_mlx_data_last_word = __ep__ + 64 + 64 - 2; /* address of the last  word of the Melexis data in the nvRAM */

_flash_latches = ABSOLUTE(ORIGIN(flash_latches)); /* used to access the latches */

/* Helper symbols for memories start/end/size */
__flash_latches_mem_start = ABSOLUTE(ORIGIN(flash_latches));
__flash_latches_mem_size = LENGTH(flash_latches);
__flash_latches_mem_end = __flash_latches_mem_start + __flash_latches_mem_size;

__flash_config_mem_start = ABSOLUTE(ORIGIN(flash_config));
__flash_config_mem_size = LENGTH(flash_config);
__flash_config_mem_end = __flash_config_mem_start + __flash_config_mem_size;





/*

 * Copyright (C) 2017 Melexis N.V.

 *

 * Software Platform

 *

 */
SECTIONS
{
    .text :
    {
        _stext = .;
        __vectors_start = . ;
        KEEP(*(.vectors))
        __vectors_end = . ;
        __vectors_size = __vectors_end - __vectors_start ;
/*

        __dummy = ASSERT (__vectors_size <= (35*8), "Vector table is too big!");

*/
        . = ALIGN (2);

        *(.text)
        *(.text.*)

        *(.rodata)
        *(.rodata.*)
        . = ALIGN (0x2);
        _etext = .;

        /*

         * this should be last linking to 'flash' region

         * the rest of the region will be used to store load data

         * (see "AT > flash" below)

         */
    } > flash

    /* Initialized variables (data) with .dp attribute

     * The initial values are copied by crt from FLASH to RAM

     *

     * NOTES: Output section name should be '.dp.data' otherwise binutils

     * complain about "Dwarf Error"

     */
    .dp.data :
    {
        . = ALIGN (0x2);
        __data_dp_start = . ;
        *(.dp.data)
        . = ALIGN (0x2);
        __data_dp_end = . ;
    } > ram AT > flash
    __data_dp_size = __data_dp_end - __data_dp_start;
    __data_load_start = LOADADDR(.dp.data);

/*

    Using section .dp.data might be dangerous. It is not initialized during Warm Boot, i.e. after Sleep / Deep Sleep modes.

    __dummy = ASSERT(__data_dp_size == 0, ".dp.data section is NOT initialized after Sleep / Deep Sleep modes");

*/
    /* Uninitialized variables (bss) with .dp attribute

     * This memory area is zeroed by crt

     *

     * NOTES: Output section name should be '.dp' otherwise binutils

     * complain about "Dwarf Error"

     */
    .dp :
    {
        . = ALIGN (0x2);
        __bss_dp_start = .;
        *(.dp)
        . = ALIGN (0x2);
        __bss_dp_end = . ;
    } > ram
    __bss_dp_size = __bss_dp_end - __bss_dp_start;

    /* Global initialized variables with nodp attribute

     * The initial values are copied by crt from FLASH

     */
    .data :
    {
        . = ALIGN (2);
        __data_start = . ;
        *(.ramfunc)
        *(.data)
        *(.data.*)
        . = ALIGN (2);
        __data_end = . ;
    } > ram AT > flash
    __data_size = __data_end - __data_start ;

/*

    Using section .data might be dangerous. It is not initialized during Warm Boot, i.e. after Sleep / Deep Sleep modes.

    __dummy = ASSERT(__data_size == 0, ".data section is NOT initialized after Sleep / Deep Sleep modes");



*/
    /* Global variables with nodp attribute

     * This memory area is zeroed by crt

     */
    .bss :
    {
        . = ALIGN (0x2);
        __bss_start = .;
        *(.bss)
        *(.bss.*)
        *(COMMON)
        . = ALIGN (0x2);
        __bss_end = . ;
    } > ram
    __bss_size = __bss_end - __bss_start;


    /* Global data not cleared after reset  */
    .noinit (NOLOAD) :
    {
        . = ALIGN (0x2);
        __noinit_start = .;
        *(.noinit)
        . = ALIGN (0x2);
        __noinit_end = .;
    } > ram
    __noinit_size = __noinit_end - __noinit_start;


    /* The stack starts immediately after the global variables */
    PROVIDE (_stack = .);

    /* This is the starting address of the heap.

     * It is located at the end of RAM and grows downward to the end of stack.

     * In a typical embedded application the heap is not used, but this

     * symbol has to be defined since some library functions need it.

     */
    PROVIDE (__end = ORIGIN(ram) + LENGTH(ram)); /* end of RAM */

}

/********************** Extra Page Section Definitions ***********************/

SECTIONS
{
    .scratchpad (NOLOAD) :
    {
        . = ALIGN (0x2);
        __scratchpad_start = .;
        *(.scratchpad)
        __scratchpad_end = .;
    } > scratchpad
    __scratchpad_size = __scratchpad_end - __scratchpad_start;

    /* Global variables in extra page with no values specified. */
    .ep :
    {
        . = ALIGN (2);
        __bss_ep_start = .;
        *(.ep)
        . = ALIGN (2);
        __bss_ep_end = . ;
    } > eeprom

    .ep.data :
    {
        . = ALIGN (2);
        __data_ep_start = . ;
        *(.ep.data)
        . = ALIGN (2);
        __data_ep_end = . ;
    } > eeprom
}


/*

 * Opcodes for FPn service routings (used by atomic.h)

 * - call FPn:enter_system_mode_keep_prio

 * - call FPn:enter_system_mode_prio_0

 */
OPCODE_CALL_FP_ENTER_SYSTEM_MODE_KEEP_PRIO = 0x5000 | (((enter_system_mode_keep_prio - __vectors_start) / 8) & 0xFF);
OPCODE_CALL_FP_ENTER_SYSTEM_MODE_PRIO_0 = 0x5000 | (((enter_system_mode_prio_0 - __vectors_start) / 8) & 0xFF);


INCLUDE "symb-addr.ld"




INCLUDE "rom-functions-CA.ld"





/* EOF */
