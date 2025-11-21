/*
 * Copyright (C) 2016-2022 Melexis N.V.
 *
 * Software Platform
 *
 */

#include <delay.h>
#include <sensorlib.h>
#include <io.h>

#if 0
/* Short time to be used just for debugging */
#define TIME_BEFORE_RESET   (WT_PERIOD_1_S)
#else
/* Normal value that has to be as long as possible to ensure juintless RFTx measurements */
#define TIME_BEFORE_RESET   (WT_PERIOD_MAX)
#endif

STATIC INLINE void InitRFTx (void);

/* ----------------------------------------------------------------------------
 * MAIN
 */
int main (void)
{
    Gpio_SetPinMode(GPIO2, GPIO_DIG_INPUT_WITH_PULLDOWN);
    /* Configure Wake-Up Timer (needed in case of debug only, because by default
     * the Wake-Up Timer's period is set to its maximal value (about 17 minutes) */
    WakeupTimer_Restart(TIME_BEFORE_RESET);

    InitRFTx();         /* Configure RFTx to generate central RF carrier frequency */

    delay_nops(10000u); /* Delay about 5 ms @ 8 MHz */

    /* Change RFTx configuration according to the GPIO2 level */
    if (Gpio_ReadPin(GPIO2))                            /* if GPIO2 is pulled up */
    {   /* Rewrite the RFTX_FSK_NOOK and RFTX_EN_MANCHESTER to get a frequency deviation */
        IO_SET(CUSTOM_91804, RFTX_FSK_NOOK, 1u);        /* set FSK */
        IO_SET(CUSTOM_91804, RFTX_EN_MANCHESTER, 1u);   /* set manchester coding */
    }
    else { /* GPIO2 is pulled down or floating => configuration of InitRFTx() is kept */ }

    (void)RfTx_Calibrate();

/* Note: Because the RF carrier frequency generation cannot be completed itself,
 * in 17 min (default WUT period) after power-on the Wake-Up Timer will wake-up the HFO
 * from STOP mode and the enabled watchdog timer (AWD) will finally reset the device
 * As a result the main function will be repeated approximately every 17 minutes
 * Note: Below the function RfTx_Run will never return because
 * after wake-up the STOP mode will not be ended without enabled interrupt from WUT */

    RfTx_Run();     /* With set RFTx config this function is ended by AWD reset */

    return 0;
}

/* ----------------------------------------------------------------------------
 * RFTx configuration
 * Arguments:   none
 * Returns:     none
 * Notes:
 */

STATIC INLINE void InitRFTx (void)
{
static const RfTxConfig_Type cfg_rf =
    {{  /* Note: All unconfigured fields keeps their default zero values */
/* 3-bit of modulation source selection:
0 = Fixed logic 0, 1 = All raw data from FIFO, no preamble, sync pattern or packet
2 = Preamble and sync pattern, no packet, 3 = Preamble, sync pattern and packet,
4 = GPIO0 input, 5 = GPIO1 input, 6 = GPIO2 input, 7 = GPIO3 input */
        .RFTX_DIRECT_MOD = 0,
/* Time to wait before declaring PLL in-lock (2-bits): 0 = 40 us, 1 = 80 us, 2 and 3 = reserved */
        .RFTX_WAIT_LOCK = 1,
/* Baud rate = Crystal frequency / ((240 + RFTX_DR_MANT) * 2^(RFTX_DR_EXP-1)), half of this for Manchester encoding */
        .RFTX_DR_MANT = 0x63,       /* 8-bit mantissa to set the baud rate, set for 19200 baud, Fcrystal = 26 MHz */
        .RFTX_DR_EXP = 0x03,        /* 3-bit exponent to set the baud rate, set for 19200 baud, Fcrystal = 26 MHz */
        .RFTX_EN_MANCHESTER = 0,    /* Enable Manchester coding of the data bits: 0 = NRZ, 1 = Manchester */
/* 4-bit mantissa of the coefficient that multiplies the signal */
        .RFTX_MULT_MANT = 13,
/* 4-bit exponential scaling to be applied to the pulse shaping coefficients */
        .RFTX_MULT_EXP = 2,
        .RFTX_BIT_INVERT = 1,       /* Invert the polarity of the data bits if set */
        .RFTX_FSK_NOOK = 0          /* Select the modulation type: 0 = OOK, 1 = FSK */
    }};

    RfTx_Init(&cfg_rf);                     /* Apply parameters specified in the "cfg_rf" */
} /* InitRFTx */

/* EOF */
