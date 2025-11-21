/*
 * Copyright (C) 2016-2022 Melexis N.V.
 *
 * Software Platform
 */

/* LFRx and SW UART Example:
 *
 * This example is to count received LF messages with a fixed format that presumes:
 * Standard TPMS Sync, Wake-Up ID (header) = 0x618F and a fixed payload:
 * {0x01, 0x02, 0x03, 0x04, 0x44}
 * The 16-bit counter Message_Counter is allocated in the Scratchpad memory
 * Its value is transmitted once per 4 seconds via GPIO0 in two bytes
 * (highest byte first)
 * Each byte is transmitted via GPIO0 by using the debug interface of the SW library
 * in 10 serial bits according to the well-known RS232 compatible format
 * with the first szero tart bit and the last stop bit that is always one
 * Between the start and stop bits the 8 payload data is sent with LSB-first
 * The transmitted bits are NRZ encoded @ 9600 bps
 * Note:
 * The HFO calibration procedure can be used to keep the used SW UART bitrate
 * stable over a wide temperature range
*/
#include <sensorlib.h>
#include <debug.h>

STATIC INLINE void InitLFRx(void);          /* LFRx configuration */
STATIC void GPIO_configuration(void);

STATIC volatile SP_MEM uint16_t Message_Counter;

/* ----------------------------------------------------------------------------
 * Main
 */
int main(void)
{
    #define MESSAGE_SIZE    (5u)
    STATIC const uint8_t REF_PAYLOAD[MESSAGE_SIZE] = {0x01u, 0x02u, 0x03u, 0x04u, 0x44u};

    /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
    if (Sys_IsColdBoot())
    {   /* Cold boot: */
        WakeupTimer_Restart(WT_PERIOD_4_S); /* Configure Wake-Up Timer */
        InitLFRx();
        GPIO_configuration();               /* Includes GPIO0 configuration to support DEBUG functions */
        Message_Counter = 0u;
    }
    /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
    else if (Itc_IsPending(ITC_LFRX_DONE))
    {   /* Part was woken up by the end of LFRx */
        if (LfRx_FifoCount() >= MESSAGE_SIZE)
        {
            uint16_t index;
            for (index = 0u; index < MESSAGE_SIZE; index++)
            {   /* Validate the received payload */
                if (REF_PAYLOAD[index] != LfRx_FifoReadByte())
                {   /* Break the loop because of mismatch */
                    break;
                }
                else { /* continue */ }
            }
            if (index == MESSAGE_SIZE)
            {
                Message_Counter += 1u;  /* Valid message was received => increment counter */
            }
            else { /* Discard the received message because its content doesn't match REF_PAYLOAD */ }
        }
        else { /* Discard the received message because it is too short */ }
        LfRx_FifoFlush();
    }
    /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
    else if (Itc_IsPending(ITC_WUPTMR))
    {   /* Part was woken up by the Wake-Up Timer */
        WakeupTimer_IntClear();     /* clear wake-up flag */
        DEBUG_PUT_WORD(Message_Counter);    /* Report with Message_Counter */
    }
    /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
    else
    {   /* FAULT branch, since other wake-ups are not expected */
        Sys_ResetCpu();             /* no return => generates a SW reset */
    }
    /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

    Sys_DeepSleepWith(LF_RECEIVE);  /* Go to Deep Sleep by keeping LF receive */

    return 0;
} /* main */

/* ----------------------------------------------------------------------------
 * LFRx configuration
 * Arguments:   none
 * Returns:     none
 * Notes:
 */
STATIC INLINE void InitLFRx(void)
{
    STATIC const LfConfig_Type cfg_lf =
    {{
/* TX config */
    .LFTX_MODE  = LFTX_MANCHESTER_LSB_FIRST,    /* 3 bits defining the LFTx modulation source */
    .LFTX_POL   = LFTX_POLARITY_1,              /* LFTx polarity */

/* RX config */
    /* LFRx control */
    .LF_EN_FD       = 0,        /* 1 bit: being set it enables LFRX fault detection mechanism */
    .LFRX_STBY_ON   = 0,        /* 1 bit: being set it keeps LF analog interface enabled during standby phase of RX polling */
    /* LFRX standby interval (single shot if LFRX_STBY_MANT = 0 and LFRX_STBY_EXP = 0 */
    /* If LFRX_STBY_EXP = 0, Standby interval [ms] =  LFRX_STBY_MANT */
    /* If LFRX_STBY_EXP > 0, Standby interval [ms] = (LFRX_STBY_MANT+16) * 2^(LFRX_STBY_EXP-1) */
    .LFRX_STBY_EXP  = 3,        /* 4-bit exponent of the Standby interval */
    .LFRX_STBY_MANT = 9,        /* 4-bit mantissa of the Standby interval */
    /* Timeout interval limiting preamble detection duration (no timeout, if both mantissa and exponent are zero */
    /* If LFRX_TIMEOUT_EXP = 0, Timeout [ms] =  LFRX_TIMEOUT_MANT */
    /* If LFRX_TIMEOUT_EXP > 0, Timeout [ms] = (LFRX_TIMEOUT_MANT+16) * 2^(LFRX_TIMEOUT_EXP-1) */
    .LFRX_TIMEOUT_EXP   = 3,    /* 4-bit exponent of the timeout interval */
    .LFRX_TIMEOUT_MANT  = 2,    /* 4-bit mantissa of the timeout interval */
    .LF_GAIN_SEL    = LF_GAIN_MAX,  /* Gain setting for LF analog front-end: LF_GAIN_MAX or LF_GAIN_MIN */

    /* Manchester parameters */
    .LFRX_ORDER     = LFRX_ORDER_MSB_FIRST, /* LF RX bit order inside of any byte: 0 = LSB first, 1 = MSB first */
    .LFRX_POL       = 0,        /* LFRx polarity: if it's zero => high-to-low corresponds to Manchester's "0" */

    /* Preamble validation */
    .LFRX_PREAMBLE  = 1,        /* 1 bit to enable preamble validation: "1" to enable, "0" to disable */
    .LFRX_LONGCNT   = 1,        /* 1 bit specifying a number of 32 kHz periods to validate preamble: "0" - 5 cycles, "1" - 10 cycles */
    .LFRX_MINCNT    = 22,       /* 6-bit minimum counter value for valid preamble */
    .LFRX_DELTACNT  = 20,       /* 6-bit (maximum - minimum) counter value for valid preamble */

    /* Sync word */
    .LFRX_SYNCLEN       = 9,    /* 4-bit LF RX synchronization word length, 0...15 SYNC DIGITs */
    .LFRX_SYNCPOL       = 1,    /* Synchronization pattern starts with falling (0) or rising (1) edge */
    /* 2-bits durations of LFRX synchronization pattern's digits (in Tsymbols): 0 = 0.5, 1 = 1, 2 = 1.5, 3 = 2 */
    .LFRX_SYNC_DIGIT1   = 0,    /* last     digit  duration, default value = 0 */
    .LFRX_SYNC_DIGIT2   = 1,    /* last - 1 digit  duration, default value = 1 */
    .LFRX_SYNC_DIGIT3   = 1,    /* last - 2 digit  duration, default value = 1 */
    .LFRX_SYNC_DIGIT4   = 1,    /* last - 3 digit  duration, default value = 1 */
    .LFRX_SYNC_DIGIT5   = 1,    /* last - 4 digit  duration, default value = 1 */
    .LFRX_SYNC_DIGIT6   = 0,    /* last - 5 digit  duration, default value = 0 */
    .LFRX_SYNC_DIGIT7   = 0,    /* last - 6 digit  duration, default value = 0 */
    .LFRX_SYNC_DIGIT8   = 2,    /* last - 7 digit  duration, default value = 2 */
    .LFRX_SYNC_DIGIT9   = 2,    /* last - 8 digit  duration, default value = 2 */
    .LFRX_SYNC_DIGIT10  = 0,    /* last - 9 digit  duration, default value = 0 */
    .LFRX_SYNC_DIGIT11  = 0,    /* last - 10 digit duration, default value = 0 */
    .LFRX_SYNC_DIGIT12  = 0,    /* last - 11 digit duration, default value = 0 */
    .LFRX_SYNC_DIGIT13  = 0,    /* last - 12 digit duration, default value = 0 */
    .LFRX_SYNC_DIGIT14  = 0,    /* last - 13 digit duration, default value = 0 */
    .LFRX_SYNC_DIGIT15  = 0,    /* last - 14 digit duration, default value = 0 */

    /* Header */
    .LFRX_HDRLEN    = LFRX_HDR_16_BITS, /* LF RX header length: "0" - no header, "1" - 1 byte, "2" - 2 bytes, "3" - 4 bytes of header */
    .LFRX_HDR_L     = 0x618F,           /* Lowest  16-bit header to be recognized in LFRx with LFRX_HDRLEN = LFRX_HDR_16_BITS */
    .LFRX_HDR_H     = 0x618F,           /* Highest 16-bit header to be recognized in LFRx with LFRX_HDRLEN = LFRX_HDR_16_BITS */
    /* 32-bit mask for header to be recognized in LF RX. Each bit of the mask means: "0" - don't care, "1" - to be matched */
    .LFRX_HDR_MASK_L    = 0xFFFF,       /* The mask for the lowest  16-bit header for LFRX_HDRLEN = LFRX_HDR_16_BITS */
    .LFRX_HDR_MASK_H    = 0xFFFF        /* The mask for the highest 16-bit header for LFRX_HDRLEN = LFRX_HDR_16_BITS */
    }};

    Lf_Init(&cfg_lf);

    Wakeup_Enable(WAKEUP_LF_DONE);
} /* InitLFRx */

/* ----------------------------------------------------------------------------
 * IO ports configuration
 * Arguments:   none
 * Returns:     none
 * Notes:
 */
STATIC void GPIO_configuration(void)
{
    STATIC const GpioConfig_Type cfg =
    {{
        .GPIO0_DRV = 2,     /* Output driving strength settings */
        .GPIO1_DRV = 2,     /* Available levels:  0, 1, 2 and 3 */
        .GPIO2_DRV = 2,
        .GPIO3_DRV = 2,
        /* GPIO0 initial state: */
        .GPIO0_CH_SEL = GPIO_DIG_OUTPUT_1,          /* DEBUG_PIN_INIT(LOGIC_HIGH); */
        /* GPIO1 initial state: */
        .GPIO1_CH_SEL = GPIO_LF_RX_DECODED_DATA,    /* LFRX Manchester decoded data */
        /* The value GPIO_LF_RX_IN_PREAMBLE can be used above to see on GPIO1
         * when LF transceiver is validating preamble */
        /* The value GPIO_DISABLED can be used above to disable using GPIO1 */
        /* GPIO2 initial state: */
        .GPIO2_CH_SEL = GPIO_DISABLED,
        /* GPIO3 initial state: */
        .GPIO3_CH_SEL = GPIO_DISABLED               /* Note: GPIO3 is not available */
    }};
    Gpio_Init(&cfg);
} /* GPIO_configuration */

/* ----------------------------------------------------------------------------
 * EOF */





