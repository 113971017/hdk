/*
 * Copyright (C) 2016-2022 Melexis N.V.
 *
 * Software Platform
 */

#include <sensorlib.h>

#include <sys_halt_for.h>

/*
    To exist the HALT mode:
    - interrupt shall be unmasked and enabled by priority

    To exist the STOP mode (not used in this example):
    - interrupt shall be unmasked and enabled by priority
    - wakeup for the interrupt shall be enabled (stop is hybrid mode which involves the wakeup)

    The signal GPIO_VDIG_OK represents the VDIG voltage domain supplying Amalthea core with MCU
    In both HALT and STOP modes the VDIG is always active (unlike of sleep modes)
*/

/*
 * Main
 */
int main(void)
{
    Gpio_SetPinMode(GPIO0, GPIO_DIG_OUTPUT_0);
    Gpio_SetPinMode(GPIO1, GPIO_VDIG_OK);

    (void)Gpio_TogglePin(GPIO0);
    (void)Gpio_TogglePin(GPIO0);
    (void)Gpio_TogglePin(GPIO0);
    (void)Gpio_TogglePin(GPIO0);


    while (true) {
        /* Try to acknowledge the watchdog. This allows
         * to smoothly run Sys_HaltFor() function for maximum ~130 ms,
         * if window was closed (or ~260ms if windows was open)
         */
        if (AbsWatchdog_IsWindowOpen()) {   /* if window is open .. */
            AbsWatchdog_Restart();          /* .. acknowledge the watchdog */
        }
        else {
            /* skip watchdog acknowledge */
        }

        Sys_HaltFor(15000u);

        (void)Gpio_TogglePin(GPIO0);
    }

    return 0;
}

/* EOF */
