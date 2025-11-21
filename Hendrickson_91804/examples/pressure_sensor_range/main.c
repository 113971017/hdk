/*
 * Pressure sensor diagnostic example
 *
 * Copyright (C) 2022 Melexis N.V.
 *
 */

#include <sensorlib.h>
#include <platformlib.h>
#include <debug.h>

int main(void)
{
    DEBUG_PIN_INIT(LOGIC_LOW);
    Itc_Enable(ITC_AWD_ATT);

    while (true) {
        PressureRange_Type PressureRange = Sys_GetPressureRange();
        if (PressureRange == PRESSURE_RANGE_100_500_KPA) {
            DEBUG_PIN_PULSE(1u);
        }
        else if (PressureRange == PRESSURE_RANGE_100_900_KPA) {
            DEBUG_PIN_PULSE(2u);
        }
        else if (PressureRange == PRESSURE_RANGE_100_1400_KPA) {
            DEBUG_PIN_PULSE(3u);
        }
        else {  /* PressureRange == PRESSURE_RANGE_UNKNOWN */
            DEBUG_PIN_PULSE(4u);
        }
        delay_nops (400u);
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
