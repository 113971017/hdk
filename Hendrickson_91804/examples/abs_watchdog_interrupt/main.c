/*
 * Copyright (C) 2016-2022 Melexis N.V.
 *
 * Software Platform
 */

/*
    Absolute watchdog example.

    Stage 1. Watchdog is [unconditionally] acknowledged from the Watchdog
             Attention interrupt ISR (ISR_AWD_ATT)
         -   AWD_ATT interrupt is triggered at the end of watchdog period
             (last change to acknowledge the watchdog)
         -   Watchdog period is ~260 ms (defined at design time and can not be
             changed)
         -   During each acknowledgment, pin GPIO0 is toggled

    Stage 2. Watchdog is NOT acknowledged because interrupt AWD_ATT is disabled,
             that eventually causes chip reset
         -   While waiting for reset in the background loop, GPIO0 pin
             is toggled
 */

#include <sensorlib.h>
#include <debug.h>

STATIC volatile bool is_stage_2 = false;

/*
 * Main
 */
int main(void)
{
    DEBUG_PIN_INIT(LOGIC_LOW);
    Itc_Enable(ITC_AWD_ATT);
    is_stage_2 = false;

    while (true) {
        if (is_stage_2) {
            DEBUG_PIN_TOGGLE(1u);
        }
    }

    return 0;
}


/*
    Watchdog Attention Interrupt handler

    @note
    - Watchdog is acknowledged unconditionally for the purpose of the example
 */
__attribute__((interrupt))
void ISR_AWD_ATT(void)
{
    STATIC uint16_t cnt = 8u;   /* Number of Watchdog acknowledgements defining Step 1 */

    AbsWatchdog_Restart();
    DEBUG_PIN_TOGGLE(1u);

    cnt--;
    if (cnt == 0u) {
        Itc_Disable(ITC_AWD_ATT);
        is_stage_2 = true;
    }
}

/* EOF */
