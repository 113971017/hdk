/*
 * Copyright (C) 2019-2022 Melexis N.V.
 *
 * Software Platform
 *
 * This firmware example provides periodical measurements of
 * the battery voltage (VDD) before & during the RFTx reporting
 * The reports' periodicity is about 1 second
 * Any RF telegram includes 4 bytes of Chip ID, 1 byte with normal VDD
 * and 1 byte with VDD measured during RFTx
 * The VDD value measured at RFTx is put into the report
 * following the report it is measured (so, with 1 second delay)
 * Note: the first report after the cold boot is sent with similar VDD values
 * measured before RFTx
 * In order to save energy, the part stays in the deep sleep mode
 * when the MCU and RFTx are not used
 * The example uses GPIO1 to mark different stages of the firmware execution
 */

#include <sensorlib.h>
#include <io.h>
#include <delay.h>
#include <debug.h>

#define TX_SIZE (9u)

STATIC uint8_t SP_MEM VDD_at_RFTx;

STATIC INLINE void InitRFTx(void);
STATIC INLINE void InitIOports(void);
STATIC uint8_t Calculate_VDD(uint16_t raw_v);

/* ----------------------------------------------------------------------------
 * MAIN
 */
int main(void)
{
    uint16_t raw_temperature;
    uint16_t raw_vdd;
    uint16_t chipID_15_0;
    uint16_t chipID_31_16;
    uint16_t chipID_47_32;
    uint8_t  Normal_VDD;
    uint8_t  temperature;

    if (Sys_IsColdBoot())
    {   /* Cold boot */
        /* Clear WUPTMR_FLAG with corresponding PEND flag and set WUT period */
        WakeupTimer_Restart(WT_PERIOD_1_S); /* Configure Wake-Up Timer */
        InitIOports();
        DEBUG_PIN_INIT(LOGIC_HIGH);         /* To overwrite the Test Pin configuration if needed */
        InitRFTx();
        /* Enable RFTX_DONE wake-up to ensure exit from the STOP mode */
        IO_SET(CUSTOM_91804, WAKE_RFTX_DONE, 1);
    }
    else
    {   /* Wake-Up event from WUT */
        WakeupTimer_IntClear();             /* Clear WUPTMR_FLAG and corresponding PEND flag */
        /* In this example "Calc_xxxx" functions are executed every warm boot => NvRam_Recall is needed */
        NvRam_Recall();
    }

    raw_temperature = Adc_ReadTemperature();
    temperature = (uint8_t)(Calc_Temperature(raw_temperature) >> 8);

    raw_vdd = Adc_ReadVoltage();              /* First measure VDD voltage at normal condition */
    Normal_VDD = Calculate_VDD(raw_vdd);

    if (Sys_IsColdBoot())
    {   /* Define an initial false value for VDD_at_RFTx */
        VDD_at_RFTx = Normal_VDD;
    }
    else
    {   /* VDD_at_RFTx should keep its previous value */ }

    /* Prepare content of the RF report in FIFO */
    RfTx_FifoFlush();

    /* Read Chip ID factory stored in the NVRAM, LSB first */
    chipID_15_0  = Sys_GetChipId().as_word[0];
    chipID_31_16 = Sys_GetChipId().as_word[1];
    chipID_47_32 = Sys_GetChipIdHi().as_word;

    RfTx_FifoWriteByte((uint8_t)(chipID_47_32 >> 8));   /* 1: MSB of ID */
    RfTx_FifoWriteByte((uint8_t)chipID_47_32);          /* 2 */
    RfTx_FifoWriteByte((uint8_t)(chipID_31_16 >> 8));   /* 3 */
    RfTx_FifoWriteByte((uint8_t)chipID_31_16);          /* 4 */
    RfTx_FifoWriteByte((uint8_t)(chipID_15_0 >> 8));    /* 5 */
    RfTx_FifoWriteByte((uint8_t)chipID_15_0);           /* 6: LSB of ID */
    RfTx_FifoWriteByte(temperature);                    /* 7 */
    RfTx_FifoWriteByte(Normal_VDD);                     /* 8 */
    RfTx_FifoWriteByte(VDD_at_RFTx);                    /* 9 */

    Radio_Disable();                    /* First disable RFTx and LF to set it to its default state */
    IO_SET(MLX16, RFTX_DONE_PEND, 1);   /* and remaining RFTX_DONE peding bit if they were set before */
    /* Enable RFTX_DONE interrupt to ensure exit from the STOP mode when RFTx is completed */
    IO_SET(MLX16, RFTX_DONE_ITC, 1);

    DEBUG_PIN_TOGGLE(2u);               /* 1st pulse */

    /* Note:
     * 1) In the real application the RfTx_Calibrate() is needed
     *    only after essential temperature change (e.g. > 20 degC)
     * 2) To decrease power consumption the time between RfTx_Calibrate()
     *    and RFTx start should be as short as possible */
    bool calibration_failed = RfTx_Calibrate();

    DEBUG_PIN_TOGGLE(2u);               /* 2nd pulse */

    if (calibration_failed || rftx_precharge()) {
        /* Calibration or precharge failed: skip reporting */
    }
    else
    {   /* Otherwise, calibration and PLL precharge succeeded */
        DEBUG_PIN_TOGGLE(2u);           /* 3d pulse */
        /* Start RF transmission by keeping EXEC CPU mode: */
        IO_SET(CUSTOM_91804,            /* Start dual stages RFTx reporting to measure VDD */
            /* Any value can be set for HALT_ACTION because no MCU halt instruction is below */
            HALT_ACTION, 0u,            /* POWER_DOWN_HALT */
            RFTX_MODE,   9u,            /* FTX_ENABLE_CRYSTAL_WITH_VCO_THEN_TRANSMIT_THEN_OFF */
            LF_MODE, LF_DISABLED);

        delay_nops(3000u);              /* Wait till RF power amplifier turns on */

        DEBUG_PIN_TOGGLE(2u);           /* 4th pulse */

        raw_vdd = Adc_ReadVoltage();    /* Measure VDD voltage during RFTx */
        VDD_at_RFTx = Calculate_VDD(raw_vdd);

        Sys_StopWith(LF_DISABLED);      /* Keep RFTx on background of the next STOP power down mode */
                              
        DEBUG_PIN_TOGGLE(2u);           /* 5th pulse */
    }

    Sys_DeepSleepWith(LF_DISABLED);     /* Go to the Deep Sleep */

    return 0;

} /* main */

/* ----------------------------------------------------------------------------
 * Measure and calculate VDD value
 * Arguments:   none
 * Returns:     16-bit unsigned
 * Notes:
 */
#define FALSE_VDD_VALUE             (0xFFu)
/* Define the reported VDD range from 1.5V till 4.05V */
#define HIGHEST_REPORT_VALUE        (255u)  /* it has to be < 256 */
#define LOWEST_REPORT_VALUE         (0u)    /* it has to be < HIGEST_REPORT_VALUE */
#define LOWEST_VDD_VALUE            (1500u) /* expressed in [mV] */
#define HIGHEST_VDD_VALUE           (LOWEST_VDD_VALUE + (HIGHEST_REPORT_VALUE - LOWEST_REPORT_VALUE) * 10u)
#define ADJUSTED_LOWEST_VDD_VALUE   (LOWEST_VDD_VALUE  / 10u)
#define ADJUSTED_HIGHEST_VDD_VALUE  (HIGHEST_VDD_VALUE / 10u)

STATIC uint8_t Calculate_VDD(uint16_t raw_v)
{
    uint16_t processed_v;
    uint8_t VDD_value;

    if (!Adc_IsError())                     /* If no error reported .. */
    {
        /* Real battery voltage in [V] equals to "processed_v" divided by 100 */
        processed_v = Calc_Voltage(raw_v);
        if (processed_v <= ADJUSTED_LOWEST_VDD_VALUE)
        {   /* if measured battery voltage <= LOWEST_VDD_VALUE */
            VDD_value = LOWEST_REPORT_VALUE; /* too low VDD value */
        }
        else if (processed_v >= ADJUSTED_HIGHEST_VDD_VALUE)
        {   /* if measured battery voltage >= HIGHEST_VDD_VALUE */
            VDD_value = HIGHEST_REPORT_VALUE; /* too high VDD value */
        }
        else /* Battery voltage in [V] equals to VDD_value plus ADJUSTED_HIGHEST_VDD_VALUE divided by 100 */
        {    /* Saturation is not processed */
            VDD_value = (uint8_t)(processed_v - ADJUSTED_HIGHEST_VDD_VALUE);
        }
    }
    else
    {   /* .. error handler */
        VDD_value = FALSE_VDD_VALUE;
        Adc_ClearAllErrors();               /* Clear all possible ADC errors */
    }
    return VDD_value;
} /* Calculate_VDD */

/* ----------------------------------------------------------------------------
 * RFTx configuration
 * Arguments:   none
 * Returns:     none
 * Notes:
 */
void InitRFTx(void)
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

        .RFTX_PREAMBLE = 0x55,      /* 8-bit preamble pattern */
        .RFTX_PREAMBLE_LEN = 4,     /* 8-bit that defines a length of the preamble to be transmitted (number+1 of RFTX_PREAMBLE bytes in NRZ) */

        .RFTX_PATTERN_L = 0x55A6,   /* Low  word of 32-bit RF TX synchronization word (in NRZ) */
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
        .RFTX_DR_MANT = 0x63,       /* 8-bit mantissa seting 38348 baud rate to approach 19200 bps with 26 MHz crystal */
        .RFTX_DR_EXP = 0x02,        /* 3-bit exponent seting 38348 baud rate to approach 19200 bps with 26 MHz crystal */

        .RFTX_LSB_FIRST = 0,        /* Select the bit order: 0 = MSB first, 1 = LSB first */
        .RFTX_EN_MANCHESTER = 1,    /* Enable Manchester coding of the data bits: 0 = NRZ, 1 = Manchester */
/* TX ramp up/down duration: 0 = no ramp up/down, 1 = 40us, 2 = 80us, 3 = 160us, 4 = 320us, 5 = 640us, 6 = 1.3ms, 7 = 2.6ms */
        .RFTX_RAMP = 0,

        .RFTX_MANCHESTER_STOP_WORD = 0x00,  /* 8-bit stop word for Manchester encoding */
/* 2-bit length of stop word in case of Manchester encoding: 0 = 0 bit, i.e. no stop word, 1 = 2 bits, 2 = 4 bits, 3 = 8 bits */
        .RFTX_MANCHESTER_STOP_LEN = 0,

/* 4-bit values RFTX_MULT_MANT and RFTX_MULT_EXP define FSK frequency deviation */
        .RFTX_MULT_MANT = 8,
        .RFTX_MULT_EXP = 2,
/* Invert the polarity of the data bits if set. In case of Manchester encoding: 1 = direct, 0 = inverse */
        .RFTX_BIT_INVERT = 1,
        .RFTX_PN9_MODE = 0,         /* Select TI-compatible PN9 sequence */
        .RFTX_PN9_REVERSE = 0,      /* Generate PN9 sequence: 0 = LSB first, 1 = MSB first (TI-compatible) */
        .RFTX_EN_DATAWHITE = 0,     /* Enable data whitening (if it's = 1) */
/* Interpolator on TX modulator output: 0 = OFF; 1 = ON, at 13 MHz; 2 = ON, at 6.5 MHz; 3 = ON, at 3.25 MHz */
        .RFTX_EN_INTERP = 0,
        .RFTX_EN_GAUSSIAN = 0,      /* Enable Guassian pulse shaping */
        .RFTX_FSK_NOOK = 1,         /* Select the modulation type: 0 = OOK, 1 = FSK */
    }};

    RfTx_Init(&cfg_rf);             /* Apply parameters specified in the "cfg_rf" */
} /* InitRFTx */

/* ----------------------------------------------------------------------------
 * IO ports configuration
 * Arguments:   none
 * Returns:     none
 * Notes:
 */
static void InitIOports (void)
{
    static const GpioConfig_Type cfg =
    {{
        .GPIO0_DRV = 2,                 /* Output driving strength settings */
        .GPIO1_DRV = 2,                 /* Available levels:  0, 1, 2 and 3 */
        .GPIO2_DRV = 2,
        .GPIO3_DRV = 2,
        /* GPIOs initial states: */
        .GPIO0_CH_SEL = GPIO_DISABLED,
        .GPIO1_CH_SEL = GPIO_DISABLED,
        .GPIO2_CH_SEL = GPIO_DISABLED,
        .GPIO3_CH_SEL = GPIO_DISABLED   /* Note: GPIO3 is not available */
    }};
    Gpio_Init(&cfg);
    DEBUG_PIN_INIT(LOGIC_LOW);          /* Reinit Test Pin with enabled DEBUG */
} /* InitIOports */

/* EOF */
