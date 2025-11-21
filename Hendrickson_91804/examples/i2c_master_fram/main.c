/*
 * Copyright (C) 2021-2022 Melexis N.V.
 *
 * Software Platform
 *
 * This firmware provides an example showing how to use the I2C functions
 * to communicate with an external memory device.
 * In this particular case the external device is FRAM CY15B256J-SXE,
 * operating as I2C slave. The CY15B256J-SXE is Automotive Qualified and hence
 * can be used in any automotive application.
 * The FRAM is packaged in 8-pin SOIC. In order to use the example, the FRAM pins
 * have to be connected according to the list below:
 * pin 1, 2, 3 (A0..2)  => GND
 * pin 4 (VSS)          => GND
 * pin 5 (SDA)          => GPIO1
 * pin 6 (SCL)          => GPIO0
 * pin 7 (WP)           => floating
 * pin 8 (VDD)          => VDD
 * Please note that external pull-up resistors are required on SDA, SCL pins.
 * The firmware example initializes FRAM, writes data to FRAM and then reads it back.
 * The read data is sent over the RFTx wireless communication channel to the host device.
 * The RFTx report includes the data read from the FRAM, FRAM device ID with its size in front
 * and the operation status byte. The report can be received by the Melexis Multitool PCB
 * or by any other properly configured RF receiver.
 * All the actions are implemented at every wake-up from the Deep Sleep mode
 * with a period defined at the WUT_PERIOD.
 */

#include "fram.h"

#define WUT_PERIOD  (WT_PERIOD_2_S)

/* Constants and globals */

#define DATA_SIZE       (7u + 1u)   /* 1 NULL byte is used to complete the string */
STATIC const uint8_t WR_DATA[DATA_SIZE] = {"Melexis"};

#define RFTX_SIZE       (DATA_SIZE + (1u + sizeof(Fram_DeviceId_t)) + 1u)

#define DUMMY_BYTE      (0x00u)

typedef union __attribute__((packed)) {
    struct __attribute__((packed)) {
        uint8_t WakeUp_Ok       : 1;
        uint8_t ReadId_Ok       : 1;
        uint8_t WriteBytes_Ok   : 1;    
        uint8_t ReadBytes_Ok    : 1;
        uint8_t FramSleep_Ok    : 1;
        uint8_t                 : 3;
    };
    uint8_t as_byte;
} status_t;

STATIC void InitRFTx(void);

/* ----------------------------------------------------------------------------
 * MAIN
 */
int main(void)
{
    Fram_MemAddress_t   write_address = {.Address = 0u};
    Fram_DeviceId_t     fram_id;
    uint8_t             rx_buffer[DATA_SIZE];
    status_t            op_status;
    uint8_t             tmp;

    if (Sys_IsColdBoot()) {
        WakeupTimer_Restart(WUT_PERIOD);
        InitRFTx();
    }
    else {
        WakeupTimer_IntClear();                 /* Clear WUPTMR_FLAG and corresponding PEND flag */
    }

    for (tmp = 0; tmp < DATA_SIZE; tmp++) {     /* Fill the rx_buffer by the dummy value */
        rx_buffer[tmp] = DUMMY_BYTE;
    }

    for (tmp = 0; tmp < sizeof(Fram_DeviceId_t); tmp++) {
        fram_id.as_bytes[tmp] = DUMMY_BYTE;
    }

    op_status.as_byte = 0u;                     /* Clean op_status */

    /* Activate FRAM memory */
    op_status.WakeUp_Ok = Fram_WakeUp();
    /* Read FRAM device ID */
    op_status.ReadId_Ok = Fram_ReadId(&fram_id);
    /* Write WR_DATA[] to the FRAM memory */
    op_status.WriteBytes_Ok = Fram_WriteBytes(write_address, WR_DATA, DATA_SIZE);
    /* Read FRAM memory to the rx_buffer[] */
    op_status.ReadBytes_Ok = Fram_ReadBytes(write_address, rx_buffer, DATA_SIZE);
    /* Put FRAM memory to its sleep mode */
    op_status.FramSleep_Ok = Fram_Sleep();

    /* Fill RFTx FIFO buffer starting from the rx_buffer */
    for (tmp = 0u; tmp < DATA_SIZE; tmp++) {
        RfTx_FifoWriteByte(rx_buffer[tmp]);
    }
    /* Then add the read fram_id with its size in front */
    RfTx_FifoWriteByte(sizeof(Fram_DeviceId_t));
    for (tmp = 0; tmp < sizeof(Fram_DeviceId_t); tmp++) {
        RfTx_FifoWriteByte(fram_id.as_bytes[tmp]);
    }
    /* And finish the RFTx report with op_status */
    RfTx_FifoWriteByte(op_status.as_byte);

    (void)RfTx_Calibrate();

    RfTx_Run();

    Sys_DeepSleepWith(LF_DISABLED);

    return 0;
} /* main */

/* ----------------------------------------------------------------------------
 * InitRFTx() initializes the RFTx interface
 * Arguments:   none
 * Returns:     none
 * Notes:
 */
STATIC void InitRFTx(void)
{
STATIC const RfTxConfig_Type cfg_rf =
    {{  /* Note: All unconfigured fields keeps their default zero values */
        .RFTX_PATTERN_LEN = 3,      /* 2 bits a length of the synchronization pattern : 0 = 8 bits, 1 = 16 bits, 2 = 24 bits, 3 = 32 bits */
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
        .RFTX_PREAMBLE_LEN = 7,     /* 8-bit that defines a length of the preamble to be transmitted (number+1 of RFTX_PREAMBLE bytes in NRZ) */

        .RFTX_PATTERN_L = 0xB24D,   /* Low  word of 32-bit RF TX synchronization word (in NRZ): 0x55A6 corresponds to 0xF2 in Manchester */
        .RFTX_PATTERN_H = 0x4DB2,   /* High word of 32-bit RF TX synchronization word */
        .RFTX_PACKET_LEN = RFTX_SIZE, /* 8-bit packet length in fixed packet length mode */
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
        .RFTX_DR_MANT =176,          /* 8-bit mantissa to set the baud rate, set for 62500 baud, Fcrystal = 26 MHz */
        .RFTX_DR_EXP = 1,            /* 8-bit mantissa to set the baud rate, set for 62500 baud, Fcrystal = 26 MHz */
        .RFTX_LSB_FIRST = 0,         /* Select the bit order: 0 = MSB first, 1 = LSB first */
        .RFTX_EN_MANCHESTER = 1,     /* Enable Manchester coding of the data bits: 0 = NRZ, 1 = Manchester */
/* TX ramp up/down duration: 0 = no ramp up/down, 1 = 40us, 2 = 80us, 3 = 160us, 4 = 320us, 5 = 640us, 6 = 1.3ms, 7 = 2.6ms */
        .RFTX_RAMP = 0,

        .RFTX_MANCHESTER_STOP_WORD = 0x00,  /* 8-bit stop word for Manchester encoding */
/* 2-bit length of stop word in case of Manchester encoding: 0 = 0 bit, i.e. no stop word, 1 = 2 bits, 2 = 4 bits, 3 = 8 bits */
        .RFTX_MANCHESTER_STOP_LEN = 0,

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
