/*
 * Copyright (C) 2016-2022 Melexis N.V.
 *
 * Software Platform
 */

/*
    Wake-up timer test example with DEEP SLEEP mode or SLEEP mode when RAM keeps own content

    -   DEEP SLEEP mode can be defined by means of the first "# if" below
        otherwise, if the SLEEP mode is used in the example
    -   Timer wake ups the chip from the DEEP SLEEP or SLEEP state after specified time interval
    -   At cold boot, timer is initialized for 10 ms period. Gradually, period is increased
        by 1 ms after each wakeup
    -   GPIO0 is toggled each time chip is woken up
    -   GPIO1 is assigned for the GPIO_VDIG_OK signal to represent the VDIG voltage domain
        that is used to supply Amalthea core with MCU (in the Deep Sleep mode the VDIG is disabled)
    -   Since RAM is not preserved in Deep Sleep state, a scratchpad memory is used to preserve
        the current period of the timer. Otherwise, if SLEEP mode is defined between the wake-ups
        the data in the RAM is preserved, and hence it can be used to preserve the current period
        of the wake-up timer
 */

#include <sensorlib.h>
#include <debug.h>

#if 1   /* If 1, set DEEP SLEEP mode between wake-ups */

#define USE_DEEP_SLEEP_MODE
STATIC SP_MEM uint16_t sleep_time;

#else   /* Set SLEEP mode between wake-ups */

STATIC uint16_t sleep_time;

#endif

/*
 * Main
 */
int main(void)
{
    if (Sys_IsColdBoot()) {
        DEBUG_PIN_INIT(LOGIC_LOW);
        Gpio_SetPinMode(GPIO1, GPIO_VDIG_OK);   /* to monitor CPU power */
        sleep_time = 9u;
    }
    else {
        sleep_time += 1u;
    }

    DEBUG_PIN_TOGGLE(1u);

    WakeupTimer_Restart((WakeupTimerPeriod_Type){.mant = sleep_time, .exponent = 0u});

    #if defined (USE_DEEP_SLEEP_MODE)
    /* Go to DEEP SLEEP mode with RAM turned off when the variable
     * "sleep_time" keeps own value in scratchpad memory only */
    Sys_DeepSleepWith(LF_DISABLED);
    #else
    /* Go to SLEEP mode with RAM preserved and that is why the variable
     * "sleep_time" keeps own value being located in RAM */
    Sys_SleepWith(LF_DISABLED);
    #endif  /* USE_DEEP_SLEEP_MODE */

    return 0;
}

/* EOF */
