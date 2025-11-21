/*
 * Pressure sensor diagnostic example
 *
 * Copyright (C) 2019 Melexis N.V.
 *
 */

#include <sensorlib.h>
#include <debug.h>

int main(void)
{
    Itc_Enable(ITC_AWD_ATT);

    while (true) {
        if (Check_IsBadPressure()) {
            /* Pressure sensor failed */
            DEBUG_PIN_INIT(LOGIC_LOW);
        }
        else {
            /* Pressure sensor is Ok */
            DEBUG_PIN_INIT(LOGIC_HIGH);
        }
    }
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
