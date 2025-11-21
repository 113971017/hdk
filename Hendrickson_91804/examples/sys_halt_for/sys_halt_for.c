/*
 * Copyright (C) 2016-2019 Melexis N.V.
 *
 * Software Platform
 */

#include <sensorlib.h>
#include <sys_halt_for.h>

/** @ingroup Sys
    Halt CPU for the specified number of microseconds

    @param  us  Number of microseconds (max 16383); exact halt time will be larger
                than specified by 15-60 us (depending on CPU clock) due to registers
                loading overhead.

    @note
    -   Real halt time
    -   Functions uses Simple Timer 2 to measure the halt time (hence
        maximum halt time is limited to 2^14-1 = 16383 microseconds)
    -   Function disables Simple Timer 2 and its interrupt on exit
*/
void Sys_HaltFor(uint16_t us)
{
    SimpleTimer2_Start(SIMPLE_TIMER_1US_CLOCK, us);

    Itc_ClearPending(ITC_STIMER2);
    Itc_Enable(ITC_STIMER2);    /* enable timer interrupt to exit HALT mode */

    Sys_HaltWith(LF_DISABLED);

    Itc_Disable(ITC_STIMER2);   /* return Timer settings to default  */
    SimpleTimer2_Stop();
}

/* EOF */
