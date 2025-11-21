/*
 * Copyright (C) 2016-2022 Melexis N.V.
 *
 * Software Platform
 *
 * Example of write to nvRAM:
 * It writes some data structure to nvRAM: two cases of data assignments are shown
 * After any write and read operation the system goes to Deep Sleep mode
 * and waits wake-ups either from the Wake-Up Timer (nothing to do in this case)
 * or from GPIO1 (low level on GPIO1 increments a variable GPIO1_Counter in the
 * scratchpad memory and store the result in nvRAM together with some constants
*/

#include <sensorlib.h>
#include <platformlib.h>

#if 1   /* It's better to structure nvRAM data */

/* NOTE: Byte write access to nvRAM is not allowed and will provoke a Memory Error exception */

typedef union
{
    struct __attribute__ ((packed))
    {
        uint32_t    ab;
        uint32_t    cd;
    } as_long;
    struct __attribute__ ((packed))
    {
        uint16_t    a       : 16;   /* LSB */
        uint16_t    b       : 16;
        uint16_t    c       : 16;
        uint16_t    counter : 16;   /* MSB */
    } as_words;
} Parameters_Union;

STATIC Parameters_Union NVRAM_MEM parameters;

#define Parameter_a parameters.as_words.a
#define Parameter_b parameters.as_words.b
#define Parameter_c parameters.as_words.c
#define Counter     parameters.as_words.counter

#else

/* NOTE: Byte write access to nvRAM is not allowed and will provoke a Memory Error exception */

STATIC uint16_t NVRAM_MEM Parameter_a;
STATIC uint16_t NVRAM_MEM Parameter_b;
STATIC uint16_t NVRAM_MEM Parameter_c;
STATIC uint16_t NVRAM_MEM Counter;

#endif

#define MIN_NUMBER  (8u)

STATIC uint16_t SP_MEM GPIO1_Counter;

STATIC volatile uint16_t a_value;
STATIC volatile uint16_t b_value;
STATIC volatile uint16_t c_value;
STATIC volatile uint16_t counter_value;

/*
 * Main
 */
int main(void)
{
    uint16_t anti_bounce;

    Itc_Enable(ITC_AWD_ATT);
    if (Sys_IsColdBoot()) {                 /* Cold boot */
        Gpio_SetPinMode(GPIO1, GPIO_DIG_INPUT_WITH_PULLUP);
        Wakeup_Enable(WAKEUP_GPIO1_LOW);
        GPIO1_Counter = 0u;
        /* The line below is not needed because at cold boot the maximal possible period is set by default:
        WakeupTimer_Restart(WT_PERIOD_MAX);
        */
        /* NvRam_Recall() is not needed here, as it's already done during cold boot by ROM firmware */
    }
    else if(Itc_IsPending(ITC_WUPTMR)) {    /* Wake-Up from Wake-Up Timer */
        WakeupTimer_IntClear();
        Sys_DeepSleepWith(LF_DISABLED);
    }
    else {                                  /* Wake-Up from GPIO1 */
        NvRam_Recall();
        GPIO1_Counter += 1u;
    }

    Parameter_a = 0x0123u;
    Parameter_b = 0x4567u;
    Parameter_c = 0x89ABu;
    Counter     = GPIO1_Counter;

    NvRam_Store();

    a_value = Parameter_a;
    b_value = Parameter_b;
    c_value = Parameter_c;
    counter_value = Counter;

    anti_bounce = 0;

    while (anti_bounce < MIN_NUMBER) {
        if (Gpio_ReadPin(GPIO1) == 1) {
            anti_bounce++;
        }
        else {
            anti_bounce = 0u;
        }
        delay_nops(10000u); /* Delay about 5 ms @ 8 MHz */
    }

    Sys_DeepSleepWith(LF_DISABLED);

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
