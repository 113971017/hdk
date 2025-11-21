/*
 * Copyright (C) 2016-2022 Melexis N.V.
 *
 * Software Platform
 */

/*
    GPIO toggling using DEEP SLEEP mode.

    -   Wakeup timer defines the sleep interval
    -   GPIO0 is toggled each time when chip is woken up
 */
#include <sensorlib.h>
#include <debug.h>

/*
 * Main
 */
int main(void)
{
    if (Sys_IsColdBoot()) {
        DEBUG_PIN_INIT(LOGIC_LOW);
        WakeupTimer_Restart(WT_PERIOD_30_MS);
    }
    else { /* Nothing to do in case of the warm boot (wake-up) */ }

    WakeupTimer_IntClear();
    DEBUG_PIN_TOGGLE(1u);
    Sys_DeepSleepWith(LF_DISABLED);

    return 0;
}


/* EOF */
