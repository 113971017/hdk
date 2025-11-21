/*
 * Copyright (C) 2016-2022 Melexis N.V.
 *
 * Software Platform
 */

/*
    Wake-up timer auto-reload example with DEEP SLEEP mode.

    -   Timer wake ups the chip from the DEEP SLEEP state after specified time interval
    -   GPIO0 is toggled each time chip is woken up
    -   1. Exact interval if auto-reload and WakeupTimer_IntClear() is used, otherwise
        2. interval is longer for +3 ms in case of restart by WakeupTimer_Restart()

 */
#include <sensorlib.h>
#include <debug.h>

#define WUT_PERIOD  (WT_PERIOD_5_MS)

/*
 * Main
 */
int main(void)
{
    if (Sys_IsColdBoot()) {
        WakeupTimer_Restart(WUT_PERIOD);
        DEBUG_PIN_INIT(LOGIC_LOW);
    }
    else { /* nothing to execute */ }

    DEBUG_PIN_TOGGLE(1u);

#if 0
    /* Full timer reconfiguration => the first interval after the configuration can deviate
     * due to a random resynchronization time between LFO and HFO clock domains */
    WakeupTimer_Restart(WUT_PERIOD);
#else
    /* clean wake/interrupt flags; interval is autoreloaded => exact interval */
    WakeupTimer_IntClear();
#endif

    Sys_DeepSleepWith(LF_DISABLED);

    return 0;
}

/* EOF */
