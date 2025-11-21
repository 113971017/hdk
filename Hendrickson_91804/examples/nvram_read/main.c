/*
 * Copyright (C) 2020-2022 Melexis N.V.
 *
 * Software Platform
 */

/*
    nvRAM read example

    - Example's nvRAM data is collected in one structure (see ::app_nvram_t)
    - The structure is linked into the nvRAM area by adding NVRAM_MEM attribute
      to the declaration
    - The structure initialization data is stored by compiler in .eep file
      (HEX format) which further can be programmed into the nvRAM
*/

#include <sensorlib.h>
#include <platformlib.h>

typedef struct {
    uint16_t a;
    uint16_t b;
    uint8_t  c;
    uint8_t  d;
} app_nvram_t;

STATIC const app_nvram_t nvram NVRAM_MEM = {
    .a = 0x1122u,
    .b = 0x3344u,
    .c = 0x55u,
    .d = 0x66u
};

volatile uint16_t v_out;

/*
 * Main
 */
int main(void)
{
    Itc_Enable(ITC_AWD_ATT);

    NvRam_Recall();     /* recall NVRAM content before first read */

    v_out = nvram.a;
    v_out = nvram.b;
    v_out = nvram.c;
    v_out = nvram.d;

    while (true) {
    }

    return 0;
}

/*
 * Watchdog Attention Interrupt handler
 *
 * @note
 * - Watchdog is acknowledged unconditionally for the purpose of the test example
 */
__attribute__((interrupt))
void ISR_AWD_ATT(void)
{
    AbsWatchdog_Restart();
}

/* EOF */
