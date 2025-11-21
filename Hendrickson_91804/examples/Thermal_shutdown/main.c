/*
 * Copyright (C) 2016-2022 Melexis N.V.
 *
 * Software Platform
 *
*/

#include <sensorlib.h>
#include <debug.h>
#include <delay.h>
 
/* The value below should be higher the specified maximum
 * of the Upper threshold at decreasing temperature */
#define HIGH_TEMPERATURE_THRESHOLD  (120u)  /* in [degC] */

#define WUT_PERIOD_BETWEEN_MEASUREMENTS (WT_PERIOD_1_S)

#if 0
/* Short time to be used just for debug */
#define WUT_PERIOD_AT_THERMAL_SHUTDOWN  (WT_PERIOD_250_MS)
#else
/* Normal value that has to be as long as possible in a final application */
#define WUT_PERIOD_AT_THERMAL_SHUTDOWN  (WT_PERIOD_MAX)
#endif

#define other_wake_up_events_of_application  (false)

#define TC_START_UP_TIME    (133u)

/* ----------------------------------------------------------------------------
 * MAIN
 */
int main(void)
{
    uint16_t raw_temperature;
    uint16_t temperature;

    if (Sys_IsColdBoot())
    {   /* Cold boot */
        DEBUG_PIN_INIT(LOGIC_HIGH);
        DEBUG_PIN_PULSE(1u);        /* Generate 1 pulse to mark the cold bootstrap */
        /* Clear WUPTMR_FLAG with corresponding pending flag and */
        /* configure the Wake-Up Timer (WUT) to some desired period */
        WakeupTimer_Restart(WUT_PERIOD_BETWEEN_MEASUREMENTS);
        /* Some code initializing an application can be put here */
    }
    else
    {
        if (Itc_IsPending(ITC_WUPTMR))
        {   /* Wake-Up event from Wake-Up Timer */
            WakeupTimer_IntClear(); /* Clear WUPTMR_FLAG and corresponding PEND flag */
            if (TempSensor_GetMode() != TEMPSENS_EVENT_WHILE_BELOW_HIGH_THRESHOLD)
            {   /* Wake-Up from WUT at normal operation */
                /* After the warm bootstrap the nvRAM recall is needed before measurements */
                NvRam_Recall();
                /* Anyway the TPMS application implements the temperature measurements */
                /* It makes sense to use such measurements to detect the high temperature threshold */
                raw_temperature = Adc_ReadTemperature();            /* Raw ADC measurement */
                /* Temperature in [degC] equals to high byte of "temperature" - 52 degC */
                temperature = Calc_Temperature(raw_temperature);    /* Conversion to degC unit */
                if ((uint8_t)(temperature >> 8) > (HIGH_TEMPERATURE_THRESHOLD + 52u))
                {   /* Too high temperature is detected: initialize the thermal shutdown mode ASAP!
                     * enable wake-ups from the thermal shutdown circuit to be happened
                     * when the temperature recovers below the threshold */
                    TempSensor_SetMode(TEMPSENS_EVENT_WHILE_BELOW_HIGH_THRESHOLD);
                    /* Delay to provide a start-up time for the temperature sensor: */
                    delay_nops(TC_START_UP_TIME);
                    /* Confirm that the enabled shutdown circuit also sees
                     * the system out of the normal temperature range: */
                    if (TempSensor_IsInRange() == true)
                    {   /* Temperature is in range, but close to the temperature threshold:
                         * disable back the wake-ups from the thermal shutdown circuit because
                         * the state of the temperature comparators are still in range */
                        TempSensor_SetMode(TEMPSENS_DISABLED);
                    }
                    else
                    {   /* Temperaure is out of range => for this case, too high temperature is confirmed:
                         * set the longest period for the Wake-Up Timer to minimize
                         * undesired wake-ups during the thermal shutdown mode */
                        WakeupTimer_Restart(WUT_PERIOD_AT_THERMAL_SHUTDOWN);
                        /* Because the temperature is too high -> go to the Deep Sleep mode ASAP! */
                    }
                }
                else
                {   /* Normal temperature range */
                    /* Three lines below are used just to monitor the temperature at DEBUG
                     * before entering to the thermal shutdown mode */
                    DEBUG_PUT_STRING("Temperature Code = ");
                    DEBUG_PUT_WORD_HEX(temperature);
                    DEBUG_PUT_STRING("\n"); /* Create a new line */
                    /* In the final application some application code can be put here */
                    /* Go to the Deep Sleep after execution of the normal branch of application */
                }
            }
            else
            {   /* Wake-Up from Wake-Up Timer during the thermal shutdown mode
                 * The number of such wake-ups should be minimized by setting the Wake-Up Timer
                 * for its longest period */
                DEBUG_PIN_PULSE(5u);
                /* In the real application this undesired wake-up should be as short as possible
                 * It means that as soon as this wake-up event is detected,
                 * the application has to go to the thermal shutdown mode ASAP! */
            }
        }
        else if (other_wake_up_events_of_application)
        {   /* This is just a dummy branch assigned for other wake-ups needed for the real application */
            /* Some application code can be put here */
        }
        else if ( (TempSensor_GetMode() == TEMPSENS_EVENT_WHILE_BELOW_HIGH_THRESHOLD)
               || (TempSensor_GetStatus() != TEMPSENS_ABOVE_RANGE) )
        {   /* Wake-up event from the thermal shutdown circuit that occurs
             * because the temperature becomes below the threshold */
            DEBUG_PIN_PULSE(7u);    /* Signal about going back to the normal operating mode */
            Sys_ResetCpu();         /* after thermal shutdown and provoke the SW reset */
            /* The SW reset above fully reinitializes the system to prevent any distortion
             * of the hardware configuration that could be provoked by too high temperature.
             * As well, after reinitialization both Wake-Up timer and Temperature sensor
             * will be configured for their normal operation modes
             * Pay attention: in the final application this branch can be jointed with
             * the "else" condition below because both call the Sys_ResetCpu function */
        }
        else
        {   /* Unknown wake-up event -> hardware error -> reset device */
            /* Normally the code below will be never executed */
            DEBUG_PIN_PULSE(50u);   /* Generate a big amount of pulses to signal about abnormal case */
            Sys_ResetCpu();         /* Provoke SW reset because of unknown wake-up event */
        }
    }

    Sys_DeepSleepWith(LF_DISABLED);

    return 0;
} /* main */

/* EOF */
