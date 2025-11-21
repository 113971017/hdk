/*
 * Copyright (C) 2016-2022 Melexis N.V.
 *
 * Software Platform
 */

/* This firmware example implements the following endless sequence of bootstraps:

 * 1) POR (after power on)
 * 2) Wake-Ups from the Wake-Up timer (Warm boots)
 *    8 times with delay 500 ms, deep sleep between the wake-ups
 * 3) SW resets -> 5 times
 * 4) Resets from Absolute Watchdog timer (AWD) -> 2 times
 * 5) Go to item 2 above

 * The wake-up number is stored in the word Warm_Boot_Counter located in the scratchpad
 * to retain its value between wake-ups during the deep sleep.
 * The resets' number is stored in the variable Reset_Counter that is assigned
 * either in the section NOINIT_MEM or at the last word of the RAM for the case
 * when the application is compiled and linked together with the LF bootloader.
 * As a result, the variable Reset_Counter is not cleaned neither by HW (unlike of
 * the scratchpads after resets), nor by the pre-main firmware (when it cleans
 * the global C-variables after the cold boot) -> it stores data between resets.
*/

#include <sensorlib.h>
#include <debug.h>

#define WARM_BOOT_NUMBER    (8u)
#define SW_RESET_NUMBER     (5u)
#define AWD_RESET_NUMBER    (2u)

STATIC INLINE void Actions_after_SW_Reset(void);
STATIC INLINE void Actions_after_AWD_Reset(void);

STATIC uint16_t SP_MEM Warm_Boot_Counter;

/* The variable "Reset_Counter" can be used to retain firmware state after resets */
#if !defined (__APPLICATION_WITH_LOADER__)
volatile uint16_t Reset_Counter NOINIT_MEM;
#else
/* The assignment below replaces the noinit section of the application
 * compiled with LF  bootloader. It can be used after a careful analysis only
 * and is not recommended in a final application:
 * Reset_Counter is assigned to the word with a highest address in the RAM
 * by presuming the stack will never reach this address */
extern volatile uint16_t Reset_Counter __attribute__((addr(0x21FE)));
#endif

/* ----------------------------------------------------------------------------
 * MAIN
 */
int main (void)
{
    if (Sys_IsColdBoot())
    {   /* Cold boot: */
        DEBUG_PIN_INIT(LOGIC_HIGH);
        WakeupTimer_Restart(WT_PERIOD_500_MS);  /* Configure Wake-Up Timer */
        Warm_Boot_Counter = 0u; /* it's not needed as anyway scratchpad is cleared at cold boot */

        ResetReason_Type reset_reason = Sys_GetResetReason();
        if ((reset_reason & RESET_REASON_POR) != 0u)
        {   /* Bootstrap after POR or brownout */
            DEBUG_PUT_STRING("POR\n");
        }
        else if ((reset_reason & RESET_REASON_SOFT) != 0u)
        {   /* Bootstrap after SW reset provoked by firmware */
            Actions_after_SW_Reset();
        }
        else if ((reset_reason & RESET_REASON_AWD) != 0u)
        {   /* Bootstrap after reset from watchdog */
            Actions_after_AWD_Reset();
        }
        else
        {   /* Another Reset */
            DEBUG_PUT_STRING("Error: Unexpected reset!\n");
        }
    }
    else
    {   /* Warm boot: */
        WakeupTimer_IntClear();     /* Clear WUPTMR_FLAG and corresponding PEND flag */
        Warm_Boot_Counter += 1u;
        DEBUG_PUT_STRING("Warm boot = ");
        DEBUG_PUT_WORD_HEX(Warm_Boot_Counter);
        DEBUG_PUT_STRING("\n");
        if (Warm_Boot_Counter == WARM_BOOT_NUMBER)
        {
	        Warm_Boot_Counter = 0u; /* it's not needed as anyway scratchpad is cleared after reset */
	        Reset_Counter = 0u;
            Sys_ResetCpu();         /* Provoke SW reset */
        }
    }

    Sys_DeepSleepWith(LF_DISABLED);

    return 0;
}   /* main */

/* ----------------------------------------------------------------------------
 * To specify actions after SW reset
 */
STATIC INLINE void Actions_after_SW_Reset(void)
{
    Reset_Counter += 1u;
    DEBUG_PUT_STRING("SW reset = ");
    DEBUG_PUT_WORD_HEX(Reset_Counter);
    DEBUG_PUT_STRING("\n");
    if (Reset_Counter == SW_RESET_NUMBER)
    {
	    Reset_Counter = 0u;
        while(true) {   /* Provoke reset from AWD */ }
    }
    else
        Sys_ResetCpu();     /* Provoke SW reset */
}   /* Actions_after_SW_Reset */

/* ----------------------------------------------------------------------------
 * To specify actions after a reset from AWD
 */
STATIC INLINE void Actions_after_AWD_Reset(void)
{
    Reset_Counter += 1u;
    DEBUG_PUT_STRING("AWD reset = ");
    DEBUG_PUT_WORD_HEX(Reset_Counter);
    DEBUG_PUT_STRING("\n");
    if (Reset_Counter == AWD_RESET_NUMBER)
    {   /* Switch to wake-ups from the Wake-Up timer */
	    WakeupTimer_Restart(WT_PERIOD_500_MS);      /* Configure Wake-Up Timer */
    }
    else
        while(true) {   /* Provoke reset from AWD */ }
}   /* Actions_after_AWD_Reset */

/* ----------------------------------------------------------------------------
 * EOF */
