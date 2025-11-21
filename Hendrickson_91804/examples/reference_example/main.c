/*
 * Copyright (C) 2018-2022 Melexis N.V.
 *
 * Software Platform
 *
 */

#include <platformlib.h>
#include <sensorlib.h>
#include <debug.h>

#include <datatypes.h>
#include <processing.h>
#include <measurements.h>
#include <communication.h>

STATIC INLINE void InitRandomGenerator(void);
STATIC INLINE void InitIOports(void);

SP_MEM uint16_t SP_random_base; /* SP_SP_random_base and SP_SP_rand_lfsr */
SP_MEM uint16_t SP_rand_lfsr;   /* are used to calculate a random delay */
SP_MEM uint8_t  SP_temperature_at_calibration;
SP_MEM uint8_t  SP_RFTx_Message_counter;
SP_MEM State_t  SP_state;

DP_MEM bool     PVT_measurements_valid;
DP_MEM bool     Acceleration_measurements_valid;

/* ----------------------------------------------------------------------------
 * MAIN
 */
int main (void)
{
    if (SP_state.as_field.action_state == NO_ACTION)
    {
        Acceleration_measurements_valid = false;
        PVT_measurements_valid = false;
    }
    else { /* Keep previous values for "valid" flags */ }
    /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
    if (Sys_IsColdBoot())
    {   /* Cold boot: */
        InitIOports();
        DEBUG_PIN_INIT(LOGIC_HIGH); /* overwrite the Test Pin configuration when DEBUG is enabled */
        DEBUG_PUT_STRING("Cold boot ");
        InitRFTx();
        InitLFRx();
        InitRandomGenerator();
        SP_temperature_at_calibration = UNKNOWN_TEMPERATURE;
        SP_state.as_field.action_state = NO_ACTION;
        TPMS_state_Init(DEFAULT_TPMS_STATE);
    }
    /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
    else if (Itc_IsPending(ITC_LFRX_DONE))
    {   /* Part was woken up by the end of LFRx */
        DEBUG_PUT_STRING("LF_boot ");
        NvRam_Recall();             /* nvRAM recall is needed before any read from nvRAM: at measurements and RFTx */
        LF_Message_processing();    /* no return => ended by Deep Sleep */
    }
    /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
    else if (Itc_IsPending(ITC_WUPTMR))
    {   /* Part was woken up by the Wake-Up Timer supporting either STT or a delay between Action stages */
        DEBUG_PUT_STRING("WUP_boot ");
        WakeupTimer_IntClear();     /* Just clear WUPTMR_FLAG and corresponding PEND flag */
        NvRam_Recall();             /* nvRAM recall is needed before any read from nvRAM: at measurements and RFTx */
        if (SP_state.as_field.action_state == RESETTING)
        {
            DEBUG_PUT_STRING("SW Reset ");
            Sys_ResetCpu();         /* no return => generates a SW reset (e.g. to start LF Bootloader)*/
        }
        else if (SP_state.as_field.action_state == TRANSMITTING)
        {   /* Wake-up to generate the next RFTx datagram of the report message */
            /* Note: it's presumed that the WUT period is always longer than RFTx datagram duration and */
            /* the RFTx calibration is implemented before the first datagram of the message if requested */
            RFTx_Fifo_filling();    /* Prepare FIFO with RFTx datagram data */
            RFTx_reporting();       /* always no return */
        }
        else { /* no high priority action is requested */ }
    }
    /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
    else
    {   /* FAULT branch, since other wake-ups (from Diagn interface, termo-sensor, GPIOs) are not expected */
        DEBUG_PUT_STRING("Unknown_boot ");
        Sys_ResetCpu();             /* no return => generates a SW reset */
    }
    /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

    TPMS_state_Motion_Detection();
    TPMS_state_Measurements();
    TPMS_state_processing_and_update();
    TPMS_state_processing_end();    /* no return */

    return 0;
}   /* main */

/* ----------------------------------------------------------------------------
 * Init Random generator
 * Arguments:   none
 * Returns:     none
 * Notes:
 */
STATIC INLINE void InitRandomGenerator(void)
{
    uint16_t IDwordLow;

    IDwordLow = Sys_GetChipId().as_word[0];
    if (IDwordLow != 0u)
    {   /* In the scratchpad memory init a linear feedback shift register SP_rand_lfsr */
        SP_rand_lfsr = IDwordLow;
    }
    else
    {
        SP_rand_lfsr = 0x0001u;
    }
    SP_random_base = 0u;
} /* InitRandomGenerator*/

/* ----------------------------------------------------------------------------
 * IO ports configuration
 * Arguments:   none
 * Returns:     none
 * Notes:
 */
STATIC INLINE void InitIOports(void)
{
    STATIC const GpioConfig_Type cfg_io =
    {{
        .GPIO0_DRV = 2, /* Output driving strength settings */
        .GPIO1_DRV = 2, /* Available levels:  0, 1, 2 and 3 */
        .GPIO2_DRV = 2,
        .GPIO3_DRV = 2,
        /* GPIOs initial states: */
        .GPIO0_CH_SEL = GPIO_DISABLED,
        .GPIO1_CH_SEL = GPIO_DISABLED,
        .GPIO2_CH_SEL = GPIO_DISABLED,
        .GPIO3_CH_SEL = GPIO_DISABLED   /* Note: GPIO3 is not available */
    }};
    Gpio_Init(&cfg_io);
    DEBUG_PIN_INIT(LOGIC_LOW);          /* Reinit Test Pin with enabled DEBUG */
} /* InitIOports */

/* EOF */
