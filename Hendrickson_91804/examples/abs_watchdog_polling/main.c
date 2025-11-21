/*
 * Copyright (C) 2016-2022 Melexis N.V.
 *
 * Software Platform
 */

/*
    Absolute watchdog test example.

    Step 1. Watchdog is acknowledged by polling open window state
        -   Window is opened after 1/2 of the watchdog period, i.e. after ~130 ms
        -   During each acknowledgment, pin GPIO0 is toggled

    Step 2. Watchdog is NOT acknowledged which eventually causes chip reset
        -   While waiting for reset in the background loop, GPIO0 pin is toggled
 */

#include <sensorlib.h>
#include <debug.h>

STATIC uint16_t cnt = 8u;   /* Number of Watchdog acknowledgements defining Step 1 */

/*
 * Main
 */
int main(void)
{
    DEBUG_PIN_INIT(LOGIC_LOW);

    while (true) {
        if (cnt != 0u)
        {
            if (AbsWatchdog_IsWindowOpen())
            {
                AbsWatchdog_Restart();
                DEBUG_PIN_TOGGLE(1u);
                cnt--;
            }
        }
        else {
            DEBUG_PIN_TOGGLE(1u);
        }
    }

    return 0;
}

/* EOF */
