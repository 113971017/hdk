/*
 * Copyright (C) 2016-2022 Melexis N.V.
 *
 * Software Platform
 */
#include <atomic.h>
#include <sensorlib.h>

STATIC volatile uint16_t value_atomic = 4u;

/*
 * Main
 */
int main(void)
{
    Itc_Enable(ITC_AWD_ATT);

    value_atomic = 0u;

    while (true) {

        ENTER_SECTION(ATOMIC_KEEP_MODE);
        value_atomic++;
        EXIT_SECTION();

        ENTER_SECTION(ATOMIC_SYSTEM_MODE);
        value_atomic--;
        EXIT_SECTION();

        ENTER_SECTION(SYSTEM_MODE);
        value_atomic = 0xFFFFu;
        EXIT_SECTION();
    }

    return 0;
}


__attribute__((interrupt))
void ISR_AWD_ATT(void)
{
    AbsWatchdog_Restart();
}

/* EOF */
