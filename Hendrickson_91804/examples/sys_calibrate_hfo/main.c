/*
 * Copyright (C) 2016-2022 Melexis N.V.
 *
 * Software Platform
 */
 
/*
 * HFO calibration example
 *
 * The functions Sys_CalibrateHfo() or Sys_CalibrateHfoByExtClk() can be used
 * in a final application to increase accuracy of the HFO frequency at actual
 * temperature. By applying "1" to GPIO2, one can enable the HFO calibration,
 * while the initial HFO accuracy with the factory calibration can be evaluated
 * by applying "0" or nothing to GPIO2 
 */

//#define USE_EXTERNAL_REFERENCE_CLOCK

//#define RESTORE_ALL

#include <sensorlib.h>
#include <delay.h>

/*
 * Main
 */
int main (void)
{
    Itc_Enable(ITC_AWD_ATT);

    Gpio_SetPinMode(GPIO0, GPIO_DIG_OUTPUT_0);
    /* The next 2 lines are not needed just because the same is done at Cold boot:
    Gpio_SetPinMode(GPIO2, GPIO_DIG_INPUT_WITH_PULLDOWN);
    NvRam_Recall(); */

    SimpleTimer1_Start(SIMPLE_TIMER_CPU_CLOCK, 8000u);
    Itc_Enable(ITC_STIMER1);

    while (true) {
        delay_nops(20000u); /* Delay about 10 ms @ 8 MHz */
        /* Comment the proper line with Sys_CalibrateHfo or Sys_CalibrateHfoByExtClk */
        /* to see the HFO frequency generated with the initial factory calibration */
        if (Gpio_ReadPin(GPIO2) == true) {  /* If high level is applied to GPIO2 */
            #if defined (USE_EXTERNAL_REFERENCE_CLOCK)
            (void)Sys_CalibrateHfoByExtClk();   /* Implement HFO calibration with External 8MHz reference */
            #else
            (void)Sys_CalibrateHfo();           /* Implement HFO calibration with XTAL reference */
            #endif /* USE_EXTERNAL_REFERENCE_CLOCK */
        }
        else { /* Restore the initial value of factory calibration */
            #if defined (RESTORE_ALL)
            NvRam_RestoreOscTrimming();     /* Restore factory calibration for HFO and LFO */
            #else
            NvRam_RestoreHfoTrimming();     /* Restore HFO factory calibration only */
            #endif
        }
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
void ISR_AWD_ATT (void)
{
    AbsWatchdog_Restart();
}

/*
 * STIMER1 Interrupt handler
 */
__attribute__((interrupt))
void ISR_STIMER1 (void)
{
    (void)Gpio_TogglePin(GPIO0);
}

/* EOF */
