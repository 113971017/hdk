/*
 * Copyright (C) 2016-2023 Melexis N.V.
 *
 * Software Platform
 */

#include <sensorlib.h>
#include <platformlib.h>
#include "biphase.h"

/*
    Bi-phase encoding example

    Salea-Logic settings:
    - Manchester
    - Bi-Phase Space Code (FM0)
    - 4800 bps
    - 8-Bit per Trnasfer
    - MSBit sent first
    - Tolerance: 25%

 */

STATIC const uint8_t src_data[] = {0xC0u, 0xDEu, 0xCAu, 0xFEu};

STATIC uint8_t dst_data[sizeof(src_data) * 2];

STATIC void rftx_initialize(void);
STATIC void rftx_write_fifo(uint8_t const *buf, uint16_t size);

STATIC void init_gpio(void);

int main (void)
{
    if (Sys_IsColdBoot())
    {   /* Cold boot: */
        init_gpio();
        rftx_initialize();
        WakeupTimer_Restart(WT_PERIOD_1_S); /* Set sleep period */
    }
    else
    {   /* Warm boot: */
	    WakeupTimer_IntClear();     /* clean wake/interrupt timer's flags; interval is autoreloaded */
    }

    RfTx_FifoFlush();

    encode_to_biphase_space(dst_data, src_data, sizeof(dst_data), 0);
    /* alt: encode_to_biphase_mark */

    rftx_write_fifo(dst_data, sizeof(dst_data));

    (void)RfTx_Run();
    Sys_DeepSleepWith(LF_DISABLED);

    return 0;
}

STATIC void rftx_initialize(void)
{
    STATIC const RfTxExtraConfig_Type rftx_freq_cfg =
        RFTX_EXTRA_CONFIG(433920,
                          RFTX_PRESET_434MHZ_5_DBM_BYPASS_OFF,
                          XTAL_CAP_INT_6PF);

    STATIC const RfTxConfig_Type cfg =      /* Note: All unconfigured fields are initialized by zero values */
    {{
        .RFTX_DIRECT_MOD = 1,               /* raw data */
        .RFTX_PACKET_LEN_FIX = 1,           /* Packet length: 0 = variable, 1 = fixed */

        .RFTX_DR_MANT = 0x63,               /* 8-bit mantissa and 3-bit exponent to set a baud rate */
        .RFTX_DR_EXP  = 0x03,               /* With Fcrystal = 26 MHz it's set to get 19200 baud rate (9600 bit rate) */

        .RFTX_LSB_FIRST      = 0,           /* Select the bit order: 0 = MSB first, 1 = LSB first */
        .RFTX_EN_MANCHESTER  = 0,           /* Enable Manchester coding of the data bits: 0 = NRZ, 1 = Manchester */
        .RFTX_BIT_INVERT     = 0,           /* Invert the polarity of the data bits if set */
        .RFTX_FSK_NOOK       = 0,           /* Select the modulation type: 0 = OOK, 1 = FSK */
        .extra = &rftx_freq_cfg
    }};

    RfTx_Init(&cfg);
}

STATIC void rftx_write_fifo(uint8_t const *buf, uint16_t size)
{
    do {
        RfTx_FifoWriteByte(*buf++);
    } while (--size != 0u);
}

STATIC void init_gpio(void)
{
    STATIC const GpioConfig_Type cfg =
    {{
        .GPIO0_DRV = 2,     /* Output driving strength settings */
        .GPIO1_DRV = 2,     /* Available levels:  0, 1, 2 and 3 */
        .GPIO2_DRV = 2,
        .GPIO3_DRV = 2,

        .GPIO0_CH_SEL = GPIO_RF_TX_OOK,      /* RFTx OOK modulation signal; use `GPIO_RF_TX_OOK | 0x02` to invert */
        .GPIO1_CH_SEL = GPIO_DISABLED,
        .GPIO2_CH_SEL = GPIO_DISABLED,
        .GPIO3_CH_SEL = GPIO_DISABLED
    }};

    Gpio_Init(&cfg);
}



/* EOF */
