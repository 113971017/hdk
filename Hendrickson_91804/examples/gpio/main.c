/*
 * Copyright (C) 2016 Melexis N.V.
 *
 * Software Platform
 */

/*
    GPIO example.

    -   Toggles GPIO0 in the background loop with periodicity about 180 ms
*/

#include <sensorlib.h>
#include <delay.h>

/*
 * Main
 */
int main(void)
{
    Itc_Enable(ITC_AWD_ATT);

    static const GpioConfig_Type cfg = 
    {{
        .GPIO0_CH_SEL = GPIO_DIG_OUTPUT_0,
    }};

    Gpio_Init(&cfg);

    while (true) {
        (void)Gpio_TogglePin(GPIO0);

        delay_nops(60000u); /* delay about 30 ms @ 8 MHz */
        delay_nops(60000u); /* delay about 30 ms @ 8 MHz */
        delay_nops(60000u); /* delay about 30 ms @ 8 MHz */
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
