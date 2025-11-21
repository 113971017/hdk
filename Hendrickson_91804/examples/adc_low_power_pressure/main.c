/*
 * Copyright (C) 2016-2022 Melexis N.V.
 *
 * Software Platform
 */

#include <sensorlib.h>
#include <debug.h>
#include <delay.h>

STATIC volatile uint16_t raw_p;
STATIC volatile uint16_t raw_p_lp;

/*
 * main
 */
int main(void)
{
    if (Sys_IsColdBoot()) { /* in case of cold boot .. */
        /* - NVRAM is recalled automatically after cold boot
         * - GPIO0 is used to show durations of functions' execution
         */
        DEBUG_PIN_INIT(LOGIC_LOW);
    }
    else {
        NvRam_Recall();     /* in case of warm boot nvRAM has to be recalled */
    }

    while (true) {

        DEBUG_PIN_PULSE(1);
        raw_p = Adc_ReadPressure();
        DEBUG_PIN_PULSE(1);
        raw_p_lp = low_power_adc_read_pressure();
        DEBUG_PIN_PULSE(1);

        delay_nops(1u * 2000u);     /* 1 ms @ 8 MHz */
        /* Acknowledge the watchdog if needed: */
        if (AbsWatchdog_IsWindowOpen()) {
            AbsWatchdog_Restart();
        }
        else { /* watchdog window is not open yet */ }
    }

    return 0;
}

/* EOF */
