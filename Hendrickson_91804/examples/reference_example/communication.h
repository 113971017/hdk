/*
 * Copyright (C) 2018-2019 Melexis N.V.
 *
 * Software Platform
 *
 */
#ifndef COMMUNICATION_H
#define COMMUNICATION_H

extern void LF_Message_processing(void) __attribute__ ((noreturn));
extern void Start_RFTx_reporting(rftx_t rftx_format_id) __attribute__ ((noreturn));
extern void RFTx_reporting(void) __attribute__ ((noreturn));
extern void RFTx_Fifo_filling(void);

extern DP_MEM bool RFTx_delay_is_random;

/* ----------------------------------------------------------------------------
 * RFTx configuration
 * Arguments:   none
 * Returns:     none
 * Notes:
 */
static INLINE void InitRFTx(void)
{
static const RfTxConfig_Type cfg_rf =
    {{  /* Note: All unconfigured fields keeps their default zero values */
        .RFTX_PATTERN_LEN = 1,      /* 2 bits a length of the synchronization pattern : 0 = 8 bits, 1 = 16 bits, 2 = 24 bits, 3 = 32 bits */
        .RFTX_MULTI_FRAME = 0,      /* 1 bit, not used (Enable the transmission of packets consisting of multiple frames) */
/* 3-bit of modulation source selection:
0 = Fixed logic 0, 1 = All raw data from FIFO, no preamble, sync pattern or packet
2 = Preamble and sync pattern, no packet, 3 = Preamble, sync pattern and packet,
4 = GPIO0 input, 5 = GPIO1 input, 6 = GPIO2 input, 7 = GPIO3 input */
        .RFTX_DIRECT_MOD = CONFIG_RFTX_DIRECT_MOD,
        .RF_EN_PD = 0,              /* 1 bit to enable RF TX power detector (if it's 1) */

/* Time to wait before declaring PLL in-lock (2-bits): 0 = 40 us, 1 = 80 us, 2 and 3 = reserved */
        .RFTX_WAIT_LOCK = 1,

        .RFTX_PREAMBLE = 0x55,      /* 8-bit preamble pattern */
        .RFTX_PREAMBLE_LEN = 1,     /* 8-bit that defines a length of the preamble to be transmitted (number+1 of RFTX_PREAMBLE bytes in NRZ) */

        .RFTX_PATTERN_L = 0x55A6,   /* Low  word of 32-bit RF TX synchronization word (in NRZ) */
        .RFTX_PATTERN_H = 0x0000,   /* High word of 32-bit RF TX synchronization word */
        .RFTX_PACKET_LEN = RFTX_PAYLOAD_SIZE_PT, /* 8-bit packet length in fixed packet length mode */
        .RFTX_PACKET_LEN_CORR = 0,  /* 4-bit signed word that specifies how the packet length should be corrected */
/* 2-bit position of the packet length byte, starting from the synchronization word (only applicable when the packet length is not fixed) */
        .RFTX_PACKET_LEN_POS = 0,
        .RFTX_PACKET_LEN_FIX = 1,   /* Packet length: 0 = variable, 1 = fixed */

        .RFTX_ADDRESS = 0x00,       /* 8-bit address to be transmitted when address byte is enabled */
        .RFTX_EN_ADDRESS = 0,       /* Add address byte to transmitted packets */

        .RFTX_CRC_INV = 0,          /* Optional inversion of the CRC value */
        .RFTX_CRC_16_N8 = 0,        /* CRC length and type: 0 = 8 bit, Baicheva polynomial, 1 = 16 bit, CCITT */
        .RFTX_EN_CRC = 1,           /* Add CRC to transmitted packets */
        .RFTX_CRC_INIT = 0xAA00,    /* 16-bit initial value for CRC calculation. In case of 8-bit CRC, the init value is in RFTX_CRC_INIT[15:8] */

/* Baud rate = Crystal frequency / ((240 + RFTX_DR_MANT) * 2^(RFTX_DR_EXP-1)), half of this for Manchester encoding */
        .RFTX_DR_MANT = 0x63,       /* 8-bit mantissa seting 38348 baud rate to approach 19200 bps with 26 MHz crystal */
        .RFTX_DR_EXP = 0x02,        /* 3-bit exponent seting 38348 baud rate to approach 19200 bps with 26 MHz crystal */

        .RFTX_LSB_FIRST = 0,        /* Select the bit order: 0 = MSB first, 1 = LSB first */
/* Enable Manchester coding of the data bits: 0 = NRZ, 1 = Manchester */
        .RFTX_EN_MANCHESTER = CONFIG_RFTX_EN_MANCHESTER,
/* TX ramp up/down duration: 0 = no ramp up/down, 1 = 40us, 2 = 80us, 3 = 160us, 4 = 320us, 5 = 640us, 6 = 1.3ms, 7 = 2.6ms */
        .RFTX_RAMP = 0,

        .RFTX_MANCHESTER_STOP_WORD = 0xFF,  /* 8-bit stop word for Manchester encoding */
/* 2-bit length of stop word in case of Manchester encoding: 0 = 0 bit, i.e. no stop word, 1 = 2 bits, 2 = 4 bits, 3 = 8 bits */
        .RFTX_MANCHESTER_STOP_LEN = 2,

/* 4-bit values RFTX_MULT_MANT and RFTX_MULT_EXP define FSK frequency deviation */
        .RFTX_MULT_MANT = 8,
        .RFTX_MULT_EXP = 2,
/* Invert the polarity of the data bits if set. In case of Manchester encoding: 1 = direct, 0 = inverse */
        .RFTX_BIT_INVERT = CONFIG_RFTX_BIT_INVERT,
        .RFTX_PN9_MODE = 0,         /* Select TI-compatible PN9 sequence */
        .RFTX_PN9_REVERSE = 0,      /* Generate PN9 sequence: 0 = LSB first, 1 = MSB first (TI-compatible) */
        .RFTX_EN_DATAWHITE = 0,     /* Enable data whitening (if it's = 1) */
/* Interpolator on TX modulator output: 0 = OFF; 1 = ON, at 13 MHz; 2 = ON, at 6.5 MHz; 3 = ON, at 3.25 MHz */
        .RFTX_EN_INTERP = 0,
        .RFTX_EN_GAUSSIAN = 0,      /* Enable Guassian pulse shaping */
/* Select the modulation type: 0 = OOK, 1 = FSK */
        .RFTX_FSK_NOOK = CONFIG_RFTX_FSK_NOOK,
    }};

    RfTx_Init(&cfg_rf);             /* Apply parameters specified in the "cfg_rf" */
} /* InitRFTx */

/* ----------------------------------------------------------------------------
 * LFRx configuration
 * Arguments:   none
 * Returns:     none
 * Notes:
 */
static INLINE void InitLFRx(void)
{
    static const LfConfig_Type cfg_lf =
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
    .LFRX_STBY_EXP  = 0,        /* 4-bit exponent and 4-bit mantissa of the Standby interval */
    .LFRX_STBY_MANT = 11,       /* The pair {0,13} corresponds to 11 ms */
    /* Timeout interval limiting preamble detection duration (no timeout, if both mantissa and exponent are zero */
    /* If LFRX_TIMEOUT_EXP = 0, Timeout [ms] =  LFRX_TIMEOUT_MANT */
    /* If LFRX_TIMEOUT_EXP > 0, Timeout [ms] = (LFRX_TIMEOUT_MANT+16) * 2^(LFRX_TIMEOUT_EXP-1) */
    .LFRX_TIMEOUT_EXP   = 3,    /* 4-bit exponent and 4-bit mantissa of the timeout interval */
    .LFRX_TIMEOUT_MANT  = 9,    /* The pair {3,9} corresponds to 100 ms */
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
    .LFRX_SYNCLEN       = 9,    /* 4-bit LF RX synchronization word length, up to 16 SYNC DIGITs */
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
    .LFRX_HDRLEN= LFRX_HDR_16_BITS,     /* LF RX header length: "0" - no header, "1" - 1 byte, "2" - 2 bytes, "3" - 4 bytes of header */
    .LFRX_HDR_L = LFRx_VDA_WAKEUP_ID,   /* Lowest  16-bit header to be recognized in LFRx with LFRX_HDRLEN = LFRX_HDR_16_BITS */
    .LFRX_HDR_H = LFRx_VDA_WAKEUP_ID,   /* Highest 16-bit header to be recognized in LFRx with LFRX_HDRLEN = LFRX_HDR_16_BITS */
    /* 32-bit mask for header to be recognized in LF RX. Each bit of the mask means: "0" - don't care, "1" - to be matched */
    .LFRX_HDR_MASK_L    = 0xFFFF,       /* The mask for the lowest  16-bit header with LFRX_HDRLEN = LFRX_HDR_16_BITS */
    .LFRX_HDR_MASK_H    = 0xFFFF        /* The mask for the highest 16-bit header with LFRX_HDRLEN = LFRX_HDR_16_BITS */
    }};

    Lf_Init(&cfg_lf);

    Wakeup_Enable(WAKEUP_LF_DONE);
} /* InitLFRx */

/* ----------------------------------------------------------------------------
 */
#endif /* COMMUNICATION_H */
