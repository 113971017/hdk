/*
 * Copyright (C) 2016-2023 Melexis N.V.
 *
 * Software Platform
 *
 * This firmware example shows how to transmit RF report
 * with a payload exceeding the RFTx FIFO size = 32 bytes
 * The reports' periodicity is about 1s.
 * The part stays in the deep sleep mode between RF reports
 */

#include <sensorlib.h>
#include <platformlib.h>

#define TX_SIZE  (100u)         /* Should be < 256 */

STATIC uint8_t Tx_buf[TX_SIZE]; /* Assigned to prepare RFTx meassage */

STATIC INLINE void InitRFTx(void);

/* ----------------------------------------------------------------------------
 * MAIN
 */
int main(void)
{
    if (Sys_IsColdBoot()) {     /* Cold boot */
        /* Clear WUPTMR_FLAG with corresponding PEND flag and set WUT period = about 4 second */
        WakeupTimer_Restart(WT_PERIOD_1_S);
        InitRFTx();
    }
    else {                      /* Wake-Up event from WUT */
        WakeupTimer_IntClear(); /* Clear WUPTMR_FLAG and corresponding PEND flag */
    }
   
    for (uint16_t tmp = 0u; tmp < TX_SIZE; tmp++) {
        Tx_buf[tmp] = tmp;      /* Fill the payload byffer somehow */
    }

    /* For power saving the calibration should be done as close as possible to the RfTx_Run() */
    (void)RfTx_Calibrate();
    RfTx_RunFromBuf(Tx_buf, TX_SIZE);

    Sys_DeepSleepWith(LF_DISABLED);

    return 0;

} /* main */

/* ----------------------------------------------------------------------------
 * RFTx configuration
 * Arguments:   none
 * Returns:     none
 * Notes:
 */
STATIC INLINE void InitRFTx(void)
{
static const RfTxConfig_Type cfg_rf =
    {{  /* Note: All unconfigured fields keeps their default zero values */
        .RFTX_PATTERN_LEN = 1,      /* 2 bits a length of the synchronization pattern : 0 = 8 bits, 1 = 16 bits, 2 = 24 bits, 3 = 32 bits */
        .RFTX_MULTI_FRAME = 0,      /* 1 bit, not used (Enable the transmission of packets consisting of multiple frames) */
/* 3-bit of modulation source selection:
0 = Fixed logic 0, 1 = All raw data from FIFO, no preamble, sync pattern or packet
2 = Preamble and sync pattern, no packet, 3 = Preamble, sync pattern and packet,
4 = GPIO0 input, 5 = GPIO1 input, 6 = GPIO2 input, 7 = GPIO3 input */
        .RFTX_DIRECT_MOD = 3,
        .RF_EN_PD = 0,              /* 1 bit to enable RF TX power detector (if it's 1) */

/* Time to wait before declaring PLL in-lock (2-bits): 0 = 40 us, 1 = 80 us, 2 and 3 = reserved */
        .RFTX_WAIT_LOCK = 1,

        .RFTX_PREAMBLE = 0x55,      /* 8-bit preamble pattern: 0x55 corresponds to 0xF in Manchester */
        .RFTX_PREAMBLE_LEN = 1,     /* 8-bit that defines a length of the preamble to be transmitted (number+1 of RFTX_PREAMBLE bytes in NRZ) */

        .RFTX_PATTERN_L = 0x55A6,   /* Low  word of 32-bit RF TX synchronization word (in NRZ): 0x55A6 corresponds to 0xF2 in Manchester */
        .RFTX_PATTERN_H = 0x0000,   /* High word of 32-bit RF TX synchronization word */
        .RFTX_PACKET_LEN = TX_SIZE, /* 8-bit packet length in fixed packet length mode */
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
        .RFTX_DR_MANT = 0x63,       /* 8-bit mantissa to set the baud rate, set for 38400 baud, Fcrystal = 26 MHz */
        .RFTX_DR_EXP = 0x02,        /* 3-bit exponent to set the baud rate, set for 38400 baud, Fcrystal = 26 MHz */

        .RFTX_LSB_FIRST = 0,        /* Select the bit order: 0 = MSB first, 1 = LSB first */
        .RFTX_EN_MANCHESTER = 1,    /* Enable Manchester coding of the data bits: 0 = NRZ, 1 = Manchester */
/* TX ramp up/down duration: 0 = no ramp up/down, 1 = 40us, 2 = 80us, 3 = 160us, 4 = 320us, 5 = 640us, 6 = 1.3ms, 7 = 2.6ms */
        .RFTX_RAMP = 0,

        .RFTX_MANCHESTER_STOP_WORD = 0xFF,  /* 8-bit stop word for Manchester encoding */
/* 2-bit length of stop word in case of Manchester encoding: 0 = 0 bit, i.e. no stop word, 1 = 2 bits, 2 = 4 bits, 3 = 8 bits */
        .RFTX_MANCHESTER_STOP_LEN = 2,

/* 4-bit mantissa of the coefficient that multiplies the signal, equal to (16 + RFTX_MULT_MANT) / 16 */
        .RFTX_MULT_MANT = 8,
/* 4-bit exponential scaling to be applied to the pulse shaping coefficients by shifting them in the modulator, equivalent to 2^RFTX_MULT_EXP */
        .RFTX_MULT_EXP = 2,
/* Invert the polarity of the data bits if set. In case of Manchester encoding: 1 = direct, 0 = inverse */
        .RFTX_BIT_INVERT = 1,
        .RFTX_PN9_MODE = 0,         /* Select TI-compatible PN9 sequence */
        .RFTX_PN9_REVERSE = 0,      /* Generate PN9 sequence: 0 = LSB first, 1 = MSB first (TI-compatible) */
        .RFTX_EN_DATAWHITE = 0,     /* Enable data whitening (if it's = 1) */
/* Interpolator on TX modulator output: 0 = OFF; 1 = ON, at 13 MHz; 2 = ON, at 6.5 MHz; 3 = ON, at 3.25 MHz */
        .RFTX_EN_INTERP = 0,
        .RFTX_EN_GAUSSIAN = 0,      /* Enable Guassian pulse shaping */
        .RFTX_FSK_NOOK = 1          /* Select the modulation type: 0 = OOK, 1 = FSK */
    }};

    RfTx_Init(&cfg_rf);             /* Apply parameters specified in the "cfg_rf" */
} /* InitRFTx */

/* EOF */
