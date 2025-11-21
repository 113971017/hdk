/*
 * Copyright (C) 2014-2022 Melexis N.V.
 *
 * Software Platform
 *
 */
#ifndef TPMSLIB_H
#define TPMSLIB_H

#include <stdint.h>
#include <stdbool.h>

/*
 * TPMS library API
 *
 * note:
 *  -   If `tpmslib.h` file is included into the application, all top-level API
 * functions below will be called as non-inline regular C functions. It's
 * important that all other library header files (like accel.h, rf.h` etc)
 * SHALL NOT be included!
 */

/* Version/hash are updated automatically, from the Makefile */
#define LIB_VERSION_MAJOR   1
#define LIB_VERSION_MINOR   10
#define LIB_VERSION_PATCH   11
#define LIB_VERSION_STRING  "1.10.11-20241213-1124"
#define LIB_GIT_HASH        0x076a8d1e

#define SP_MEM      __attribute__ ((section (".scratchpad")))   /**< data attribute to link to scratchpad area (battery supplied) */

/* Helper macro for 64-bit integer division with result rounding and converting to Q19 format (UQ45.19)
    - converting to Q19 is implemented as multiplying by 2**19
    - rounding is implemented as (a + b/2) / b
 */
#define DIV64_ROUNDED_RESULT_Q19(a, b)     (((uint64_t)(a) * ((uint64_t)1 << 19) + ((uint64_t)b / 2)) / (uint64_t)(b))
#define RFTX_CALC_DIV_RATIO(f)             DIV64_ROUNDED_RESULT_Q19((f), RF_XTAL_FREQ_IN_KHZ)

/* Helper macro function to initialize ::RfTxExtraConfig_Type structure */
#define RFTX_EXTRA_CONFIG(RFTX_FREQ_IN_KHZ, PRESET_ID, XTAL_CAP_ID) \
    { \
        .RFTX_CENTER_FREQ_L  = (uint16_t)(RFTX_CALC_DIV_RATIO(RFTX_FREQ_IN_KHZ) & 0xFFFFu),         \
        .RFTX_CENTER_FREQ_H  = (uint8_t)((RFTX_CALC_DIV_RATIO(RFTX_FREQ_IN_KHZ) >> 16) & 0xFFu),    \
        .RFTX_CENTER_FREQ_U  = (uint8_t)((RFTX_CALC_DIV_RATIO(RFTX_FREQ_IN_KHZ) >> 24) & 1u),       \
        .preset_id           = PRESET_ID,   \
        .PLL_EN_XTAL_INT_CAP = XTAL_CAP_ID  \
    }

/*
    Wakeup Timer Period contants

    Period (in ms) = mant + 1, if exp = 0
    Period (in ms) = (mant + 257) * 2^(exp-1), if exp > 0
    Note: the first period after timer restart is longer for about 3 ms
 */
#define WT_PERIOD_MAX       (WakeupTimerPeriod_Type){.mant = 255u, .exponent = 12u}

#define WT_PERIOD_16_M      (WakeupTimerPeriod_Type){.mant = 212u, .exponent = 12u}
#define WT_PERIOD_10_M      (WakeupTimerPeriod_Type){.mant =  36u, .exponent = 12u}
#define WT_PERIOD_8_M       (WakeupTimerPeriod_Type){.mant = 212u, .exponent = 11u}
#define WT_PERIOD_7_M       (WakeupTimerPeriod_Type){.mant = 153u, .exponent = 11u}
#define WT_PERIOD_6_M       (WakeupTimerPeriod_Type){.mant =  95u, .exponent = 11u}
#define WT_PERIOD_5_M       (WakeupTimerPeriod_Type){.mant =  36u, .exponent = 11u}
#define WT_PERIOD_4_M       (WakeupTimerPeriod_Type){.mant = 212u, .exponent = 10u}
#define WT_PERIOD_3_M       (WakeupTimerPeriod_Type){.mant =  95u, .exponent = 10u}
#define WT_PERIOD_2_M       (WakeupTimerPeriod_Type){.mant = 212u, .exponent =  9u}
#define WT_PERIOD_1_M       (WakeupTimerPeriod_Type){.mant = 212u, .exponent =  8u}

#define WT_PERIOD_60_S      WT_PERIOD_1_M
#define WT_PERIOD_30_S      (WakeupTimerPeriod_Type){.mant = 212u, .exponent =  7u}
#define WT_PERIOD_20_S      (WakeupTimerPeriod_Type){.mant =  55u, .exponent =  7u}    /* value mant=56 results the same absolute error */
#define WT_PERIOD_16_S      (WakeupTimerPeriod_Type){.mant = 243u, .exponent =  6u}
#define WT_PERIOD_15_S      (WakeupTimerPeriod_Type){.mant = 212u, .exponent =  6u}
#define WT_PERIOD_14_S      (WakeupTimerPeriod_Type){.mant = 180u, .exponent =  6u}    /* value mant=181 results the same absolute error */
#define WT_PERIOD_13_S      (WakeupTimerPeriod_Type){.mant = 149u, .exponent =  6u}
#define WT_PERIOD_12_S      (WakeupTimerPeriod_Type){.mant = 118u, .exponent =  6u}
#define WT_PERIOD_11_S      (WakeupTimerPeriod_Type){.mant =  87u, .exponent =  6u}
#define WT_PERIOD_10_S      (WakeupTimerPeriod_Type){.mant =  55u, .exponent =  6u}    /* value mant=56 results the same absolute error */
#define WT_PERIOD_9_S       (WakeupTimerPeriod_Type){.mant =  24u, .exponent =  6u}
#define WT_PERIOD_8_S       (WakeupTimerPeriod_Type){.mant = 243u, .exponent =  5u}
#define WT_PERIOD_7_S       (WakeupTimerPeriod_Type){.mant = 180u, .exponent =  5u}    /* value mant=181 results the same absolute error */
#define WT_PERIOD_6_S       (WakeupTimerPeriod_Type){.mant = 118u, .exponent =  5u}
#define WT_PERIOD_5_S       (WakeupTimerPeriod_Type){.mant =  55u, .exponent =  5u}    /* value mant=56 results the same absolute error */
#define WT_PERIOD_4_S       (WakeupTimerPeriod_Type){.mant = 243u, .exponent =  4u}
#define WT_PERIOD_3_S       (WakeupTimerPeriod_Type){.mant = 118u, .exponent =  4u}
#define WT_PERIOD_2_5_S     (WakeupTimerPeriod_Type){.mant =  55u, .exponent =  4u}
#define WT_PERIOD_2_S       (WakeupTimerPeriod_Type){.mant = 243u, .exponent =  3u}
#define WT_PERIOD_1_S       (WakeupTimerPeriod_Type){.mant = 243u, .exponent =  2u}

#define WT_PERIOD_500_MS    (WakeupTimerPeriod_Type){.mant = 243u, .exponent =  1u}
#define WT_PERIOD_250_MS    (WakeupTimerPeriod_Type){.mant = 249u, .exponent =  0u}
#define WT_PERIOD_150_MS    (WakeupTimerPeriod_Type){.mant = 149u, .exponent =  0u}
#define WT_PERIOD_100_MS    (WakeupTimerPeriod_Type){.mant =  99u, .exponent =  0u}
#define WT_PERIOD_50_MS     (WakeupTimerPeriod_Type){.mant =  49u, .exponent =  0u}
#define WT_PERIOD_30_MS     (WakeupTimerPeriod_Type){.mant =  29u, .exponent =  0u}
#define WT_PERIOD_15_MS     (WakeupTimerPeriod_Type){.mant =  14u, .exponent =  0u}
#define WT_PERIOD_5_MS      (WakeupTimerPeriod_Type){.mant =   4u, .exponent =  0u}
#define WT_PERIOD_3_MS      (WakeupTimerPeriod_Type){.mant =   2u, .exponent =  0u}
#define WT_PERIOD_2_MS      (WakeupTimerPeriod_Type){.mant =   1u, .exponent =  0u}

/* Wrappers to mimic old API */
#define Itc_SetPrio(iname, prio)            ITC_SET_PRIO_HELPER(iname, prio)
#define ITC_SET_PRIO_HELPER(iname, prio)    Itc_SetPrio_ ## iname (prio)

#define Itc_Enable(iname)                   ITC_ENABLE_HELPER(iname)
#define ITC_ENABLE_HELPER(iname)            Itc_Enable_ ## iname()

#define Itc_Disable(iname)                  ITC_DISABLE_HELPER(iname)
#define ITC_DISABLE_HELPER(iname)           Itc_Disable_ ## iname()

#define Itc_IsPending(iname)                ITC_IS_PENDING_HELPER(iname)
#define ITC_IS_PENDING_HELPER(iname)        Itc_IsPending_ ## iname()

#define Itc_ClearPending(iname)             ITC_CLEAR_PENDING_HELPER(iname)
#define ITC_CLEAR_PENDING_HELPER(iname)     Itc_ClearPending_ ## iname()

#define Wakeup_Enable(event)                WAKEUP_ENABLE_HELPER(event)
#define WAKEUP_ENABLE_HELPER(event)         Wakeup_Enable_  ## event()

#define Wakeup_Disable(event)               WAKEUP_DISABLE_HELPER(event)
#define WAKEUP_DISABLE_HELPER(event)        Wakeup_Disable_ ## event()

typedef enum {
    HWF_TYPE_0 = 7,
    HWF_TYPE_1 = 6,
    HWF_TYPE_2 = 5,
    HWF_TYPE_3 = 4
} HWFilterSetup_Type;

typedef struct {
    uint16_t pressure;
    uint16_t voltage;
    uint16_t temperature;
} Adc_RawPvt_Type;

typedef enum {
    DATA_VALID        = 0u,
    DATA_INVALID      = 1u,
    DATA_CHECK_FAILED = 2u
} DataCheck_Type;

typedef enum {
    GPIO0 = 0,
    GPIO1 = 1,
    GPIO2 = 2,
    GPIO3 = 3
} GpioPinId_Type;

typedef enum {
    GPIO_DISABLED = 0x00,
    GPIO_DIG_INPUT_WITH_PULLDOWN = 0x01,
    GPIO_DIG_INPUT_WITH_PULLUP = 0x02,
    GPIO_ANA_INPUT = 0x03,
    GPIO_DIG_INPUT = 0x04,
    GPIO_DIG_OUTPUT_HI_Z = GPIO_DIG_INPUT,
    GPIO_DIG_OUTPUT_0 = 0x05,
    GPIO_DIG_OUTPUT_1 = 0x06,
    GPIO_SENS_ACCEL_RDY = 0x20,
    GPIO_MCU_HALTED = 0x24,
    GPIO_VDD_PORB = 0x28,
    GPIO_HVDIG_OK = 0x2C,
    GPIO_VDIG_OK = 0x30,
    GPIO_VANA_OK = 0x34,
    GPIO_VPA_OK = 0x38,

    GPIO_DTB0 = 0x40,
    GPIO_DTB1 = 0x44,
    GPIO_DTB2 = 0x48,
    GPIO_DTB3 = 0x4C,
    GPIO_1_KHZ_CLOCK = 0x50,
    GPIO_32_KHZ_CLOCK = 0x54,
    GPIO_PLL_IN_LOCK = 0x58,
    GPIO_XTAL_RUN_OK = 0x5C,
    GPIO_DIAG0_OUTH = 0x60,
    GPIO_DIAG0_OUTL = 0x64,
    GPIO_DIAG1_OUTH = 0x68,
    GPIO_DIAG1_OUTL = 0x6C,
    GPIO_DIAG2_OUTH = 0x70,
    GPIO_DIAG2_OUTL = 0x74,
    GPIO_DIAG3_OUTH = 0x78,
    GPIO_DIAG3_OUTL = 0x7C,
    GPIO_RF_TX_FIFO_EMPTY = 0x80,
    GPIO_RF_TX_FIFO_ERROR = 0x84,
    GPIO_RF_TX_ACTIVE = 0x88,
    GPIO_RF_TX_STOPPED = 0x8C,
    GPIO_RF_TX_OOK = 0x90,
    GPIO_RF_TX_SYMBOL_CLOCK = 0x94,
    GPIO_RF_POWER_AMPLIFIER_ON = 0x98,

    GPIO_LF_TX_EMPTY = 0xA0,
    GPIO_LF_TX_ERROR = 0xA4,
    GPIO_LF_TX_ACTIVE = 0xA8,
    GPIO_LF_TX_STOPPED = 0xAC,
    GPIO_LF_TX_OOK = 0xB0,
    GPIO_LF_TX_SYMBOL_CLOCK = 0xB4,

    GPIO_LF_RX_FIFO_EMPTY = 0xC0,
    GPIO_LF_RX_FIFO_ERROR = 0xC4,
    GPIO_LF_RX_ACTIVE = 0xC8,
    GPIO_LF_RX_STOPPED = 0xCC,
    GPIO_LF_RX_DECODED_DATA = 0xD0,
    GPIO_LF_RX_DECODED_SYMBOL_CLOCK = 0xD4,
    GPIO_LF_RX_IN_PREAMBLE = 0xD8,
    GPIO_LF_RX_IN_SYNC_WORD = 0xDC,
    GPIO_LF_RX_IN_HEADER = 0xE0,
    GPIO_LF_RX_IN_PAYLOAD = 0xE4,
    GPIO_LF_RX_BUSY = 0xE8,
    GPIO_LF_RX_STANDBY = 0xEC,
    GPIO_LF_RX_ENVELOPE = 0xF0,
    GPIO_LF_RX_RECOVERED_CARRIER = 0xF4,
    GPIO_LF_FAULT = 0xF8,
} GpioPinMode_Type;

typedef union {
    struct {
        GpioPinMode_Type GPIO0_CH_SEL : 8;
        GpioPinMode_Type GPIO1_CH_SEL : 8;
        GpioPinMode_Type GPIO2_CH_SEL : 8;
        GpioPinMode_Type GPIO3_CH_SEL : 8;
    };

    uint8_t GPIO_CH_SEL[4];
} GpioChannelConfig_Type;

typedef struct __attribute__((packed))
{
    uint8_t GPIO0_DRV : 2;
    uint8_t GPIO1_DRV : 2;
    uint8_t GPIO2_DRV : 2;
    uint8_t GPIO3_DRV : 2;
} GpioDriveConfig_Type;

typedef union __attribute__((aligned(2), packed))
{
    struct __attribute__((aligned(2), packed))
    {
        GpioPinMode_Type GPIO0_CH_SEL : 8;
        GpioPinMode_Type GPIO1_CH_SEL : 8;
        GpioPinMode_Type GPIO2_CH_SEL : 8;
        GpioPinMode_Type GPIO3_CH_SEL : 8;

        uint8_t GPIO0_DRV : 2;
        uint8_t GPIO1_DRV : 2;
        uint8_t GPIO2_DRV : 2;
        uint8_t GPIO3_DRV : 2;

        uint8_t : 8;
    };

    struct __attribute__((aligned(2), packed))
    {
        GpioChannelConfig_Type channel;
        GpioDriveConfig_Type drive;
        uint8_t : 8;
    };
} GpioConfig_Type;

typedef uint8_t GpioPortLevel_Type;

typedef enum {
    LF_DISABLED = 0u,
    LF_TRANSMIT = 1u,
    LF_RSSI = 2u,
    LF_RECEIVE = 3u
} LfMode_Type;

typedef enum {
    LFTX_NRZ_LSB_FIRST = 0,
    LFTX_NRZ_MSB_FIRST = 1,
    LFTX_MANCHESTER_LSB_FIRST = 2,
    LFTX_MANCHESTER_MSB_FIRST = 3,
    LFTX_DIRECT_FROM_GPIO0 = 4,
    LFTX_DIRECT_FROM_GPIO1 = 5,
    LFTX_DIRECT_FROM_GPIO2 = 6,
    LFTX_DIRECT_FROM_GPIO3 = 7,
} LfTxMode_Type;

typedef enum {
    LFTX_POLARITY_0 = 0,
    LFTX_POLARITY_1 = 1
} LfTxPolarity_Type;

typedef enum {
    LFRX_ORDER_LSB_FIRST = 0,
    LFRX_ORDER_MSB_FIRST = 1
} LfRxOrder_Type;

typedef enum {
    LFRX_HDR_DISABLED = 0u,
    LFRX_HDR_8_BITS   = 1u,
    LFRX_HDR_16_BITS  = 2u,
    LFRX_HDR_32_BITS  = 3u
} LfRxHdrLen_Type;

typedef enum {
    LFTX_ON_HALF_FIFO = 2u,
    LFRX_ON_SNIFF     = 3u,
    LFRX_ON_TIMEOUT   = 4u,
    LFRX_ON_SYNC      = 5u,
    LFRX_ON_DATA      = 6u,
    LF_ON_STOP        = 7u
} LfEvent_Type;

typedef enum {
    LFRX_STATE_OFF = 0u,
    LFRX_STATE_STOPPED = 1u,
    LFRX_STATE_STANDBY = 2u,
    LFRX_STATE_WAIT_PREAMBLE = 3u,
    LFRX_STATE_PREAMBLE_FOUND = 4u,
    LFRX_STATE_WAIT_SYNC = 5u,
    LFRX_STATE_WAIT_HEADER = 6u,
    LFRX_STATE_RECEIVE_DATA = 7u
} LfRxState_Type;

typedef enum {
    LFRX_SYNC_0_5_TSYMBOL = 0u,
    LFRX_SYNC_1_0_TSYMBOL = 1u,
    LFRX_SYNC_1_5_TSYMBOL = 2u,
    LFRX_SYNC_2_0_TSYMBOL = 3u
} LfRxSyncDigit_Type;

typedef enum {
    LF_GAIN_MAX = 1u,
    LF_GAIN_MID = 2u,
    LF_GAIN_MIN = 3u
} LfGain_Type;

typedef union
{
    struct
    {
        LfTxPolarity_Type LFTX_POL : 1;
        LfTxMode_Type LFTX_MODE : 3;
        uint16_t : 3;
        uint8_t LF_EN_FD : 1;
        uint16_t : 3;
        uint16_t : 5;
        uint16_t : 8;
        uint16_t : 8;
        uint16_t LFRX_TIMEOUT_MANT : 4;
        uint16_t LFRX_TIMEOUT_EXP : 4;
        uint16_t LFRX_STBY_MANT : 4;
        uint16_t LFRX_STBY_EXP : 4;

        uint8_t LFRX_STBY_ON : 1;
        uint8_t : 1;
        uint8_t LFRX_PREAMBLE : 1;
        uint8_t : 1;
        LfGain_Type LF_GAIN_SEL         : 2;
        uint8_t : 1;
        LfRxOrder_Type LFRX_ORDER : 1;
        LfRxHdrLen_Type LFRX_HDRLEN : 2;
        uint8_t LFRX_POL : 1;

        uint8_t LFRX_SYNCPOL : 1;
        uint16_t LFRX_SYNCLEN : 4;

        uint16_t : 2;
        LfRxSyncDigit_Type LFRX_SYNC_DIGIT1 : 2;
        LfRxSyncDigit_Type LFRX_SYNC_DIGIT2 : 2;
        LfRxSyncDigit_Type LFRX_SYNC_DIGIT3 : 2;
        LfRxSyncDigit_Type LFRX_SYNC_DIGIT4 : 2;
        LfRxSyncDigit_Type LFRX_SYNC_DIGIT5 : 2;
        LfRxSyncDigit_Type LFRX_SYNC_DIGIT6 : 2;
        LfRxSyncDigit_Type LFRX_SYNC_DIGIT7 : 2;

        LfRxSyncDigit_Type LFRX_SYNC_DIGIT8 : 2;
        LfRxSyncDigit_Type LFRX_SYNC_DIGIT9 : 2;
        LfRxSyncDigit_Type LFRX_SYNC_DIGIT10: 2;
        LfRxSyncDigit_Type LFRX_SYNC_DIGIT11: 2;
        LfRxSyncDigit_Type LFRX_SYNC_DIGIT12: 2;
        LfRxSyncDigit_Type LFRX_SYNC_DIGIT13: 2;
        LfRxSyncDigit_Type LFRX_SYNC_DIGIT14: 2;
        LfRxSyncDigit_Type LFRX_SYNC_DIGIT15: 2;

        uint16_t LFRX_MINCNT : 6;
        uint16_t : 2;
        uint16_t LFRX_DELTACNT : 6;
        uint16_t : 1;
        uint16_t LFRX_LONGCNT : 1;

        uint16_t LFRX_HDR_L : 16;
        uint16_t LFRX_HDR_H : 16;
        uint16_t LFRX_HDR_MASK_L : 16;
        uint16_t LFRX_HDR_MASK_H : 16;
    };
} LfConfig_Type;

typedef struct {
    uint16_t RFTX_PATTERN_LEN : 2;
    uint8_t : 1;
    uint8_t RFTX_MULTI_FRAME : 1;
    uint16_t RFTX_DIRECT_MOD : 3;
    uint8_t RF_EN_PD : 1;

    uint16_t : 3;
    uint8_t : 1;
    uint8_t : 1;
    uint8_t : 1;
    uint16_t RFTX_WAIT_LOCK : 2;

    uint16_t RFTX_PREAMBLE : 8;
    uint16_t RFTX_PREAMBLE_LEN : 8;

    uint16_t RFTX_PATTERN_L : 16;
    uint16_t RFTX_PATTERN_H : 16;

    uint16_t RFTX_PACKET_LEN : 8;
    uint16_t RFTX_PACKET_LEN_CORR : 4;
    uint16_t RFTX_PACKET_LEN_POS : 2;

    uint8_t : 1;
    uint16_t RFTX_PACKET_LEN_FIX : 1;

    uint16_t RFTX_ADDRESS : 8;
    uint8_t RFTX_EN_ADDRESS : 1;
    uint8_t RFTX_CRC_INV : 1;
    uint8_t RFTX_CRC_16_N8 : 1;

    uint8_t RFTX_EN_CRC : 1;
    uint16_t : 4;

    uint16_t RFTX_CRC_INIT : 16;

    uint16_t RFTX_DR_MANT : 8;

    uint16_t RFTX_DR_EXP : 3;
    uint8_t RFTX_LSB_FIRST : 1;
    uint8_t RFTX_EN_MANCHESTER : 1;
    uint16_t RFTX_RAMP : 3;
    uint16_t RFTX_MANCHESTER_STOP_WORD : 8;
    uint16_t RFTX_MANCHESTER_STOP_LEN : 2;

    uint16_t : 6;

    uint16_t RFTX_MULT_MANT : 4;

    uint16_t RFTX_MULT_EXP : 4;

    uint8_t RFTX_BIT_INVERT : 1;
    uint8_t RFTX_PN9_MODE : 1;
    uint8_t RFTX_PN9_REVERSE : 1;

    uint8_t RFTX_EN_DATAWHITE : 1;
    uint16_t RFTX_EN_INTERP : 2;

    uint8_t RFTX_EN_GAUSSIAN : 1;
    uint8_t RFTX_FSK_NOOK : 1;
} RfTxGeneralConfig_Type;

typedef enum {
    RFTX_PRESET_434MHZ_5_DBM_BYPASS_ON = 0,
    RFTX_PRESET_434MHZ_8_DBM_BYPASS_ON = 1,
    RFTX_PRESET_434MHZ_5_DBM_BYPASS_OFF = 2,
    RFTX_PRESET_434MHZ_8_DBM_BYPASS_OFF = 3,

    RFTX_PRESET_315MHZ_5_DBM_BYPASS_ON = 4,
    RFTX_PRESET_315MHZ_8_DBM_BYPASS_ON = 5,
    RFTX_PRESET_315MHZ_5_DBM_BYPASS_OFF = 6,
    RFTX_PRESET_315MHZ_8_DBM_BYPASS_OFF = RFTX_PRESET_434MHZ_8_DBM_BYPASS_OFF
} RfTxPresetId_Type;

typedef enum {
    XTAL_CAP_EXT      = 0,
    XTAL_CAP_INT_6PF  = 1,
    XTAL_CAP_INT_8PF  = 2,
    XTAL_CAP_INT_12PF = 3
} XtalCap_Type;

typedef struct {
    uint16_t RFTX_CENTER_FREQ_L : 16;
    uint16_t RFTX_CENTER_FREQ_H : 8;
    uint16_t RFTX_CENTER_FREQ_U : 1;
    RfTxPresetId_Type preset_id : 3;
    uint16_t : 2;
    XtalCap_Type PLL_EN_XTAL_INT_CAP: 2;
} RfTxExtraConfig_Type;

typedef union {
    struct {
        uint16_t RFTX_PATTERN_LEN : 2;
        uint8_t : 1;
        uint8_t RFTX_MULTI_FRAME : 1;
        uint16_t RFTX_DIRECT_MOD : 3;
        uint8_t RF_EN_PD : 1;
        uint16_t : 3;
        uint8_t : 1;
        uint8_t : 1;
        uint8_t : 1;
        uint16_t RFTX_WAIT_LOCK : 2;
        uint16_t RFTX_PREAMBLE : 8;
        uint16_t RFTX_PREAMBLE_LEN : 8;

        uint16_t RFTX_PATTERN_L : 16;
        uint16_t RFTX_PATTERN_H : 16;

        uint16_t RFTX_PACKET_LEN : 8;
        uint16_t RFTX_PACKET_LEN_CORR : 4;
        uint16_t RFTX_PACKET_LEN_POS : 2;

        uint8_t : 1;
        uint16_t RFTX_PACKET_LEN_FIX : 1;

        uint16_t RFTX_ADDRESS : 8;
        uint8_t RFTX_EN_ADDRESS : 1;
        uint8_t RFTX_CRC_INV : 1;
        uint8_t RFTX_CRC_16_N8 : 1;

        uint8_t RFTX_EN_CRC : 1;
        uint16_t : 4;

        uint16_t RFTX_CRC_INIT : 16;

        uint16_t RFTX_DR_MANT : 8;

        uint16_t RFTX_DR_EXP : 3;
        uint8_t RFTX_LSB_FIRST : 1;
        uint8_t RFTX_EN_MANCHESTER : 1;
        uint16_t RFTX_RAMP : 3;
        uint16_t RFTX_MANCHESTER_STOP_WORD : 8;
        uint16_t RFTX_MANCHESTER_STOP_LEN : 2;

        uint16_t : 6;

        uint16_t RFTX_MULT_MANT : 4;

        uint16_t RFTX_MULT_EXP : 4;

        uint8_t RFTX_BIT_INVERT : 1;
        uint8_t RFTX_PN9_MODE : 1;
        uint8_t RFTX_PN9_REVERSE : 1;

        uint8_t RFTX_EN_DATAWHITE : 1;
        uint16_t RFTX_EN_INTERP : 2;

        uint8_t RFTX_EN_GAUSSIAN : 1;
        uint8_t RFTX_FSK_NOOK : 1;

        RfTxExtraConfig_Type const *extra;
    };

    struct {
        RfTxGeneralConfig_Type general;
    };

} RfTxConfig_Type;

typedef enum {
    RFTX_KEEP_MODE = 0u,
    RFTX_DISABLED = 1u,

    RFTX_ENABLE_CRYSTAL = 3u,
    RFTX_ENABLE_CRYSTAL_THEN_VCO = 4u,
    RFTX_ENABLE_CRYSTAL_WITH_VCO = 5u,

    RFTX_ENABLE_CRYSTAL_THEN_VCO_THEN_TRANSMIT_THEN_OFF = 8u,
    RFTX_ENABLE_CRYSTAL_WITH_VCO_THEN_TRANSMIT_THEN_OFF = 9u,

    RFTX_ENABLE_CRYSTAL_THEN_VCO_THEN_TRANSMIT_KEEP_CRYSTAL = 10u,
    RFTX_ENABLE_CRYSTAL_WITH_VCO_THEN_TRANSMIT_KEEP_CRYSTAL = 11u,

    RFTX_ENABLE_CRYSTAL_THEN_VCO_THEN_TRANSMIT_KEEP_PLL = 12u,
    RFTX_ENABLE_CRYSTAL_WITH_VCO_THEN_TRANSMIT_KEEP_PLL = 13u,

    RFTX_ENABLE_CRYSTAL_THEN_VCO_THEN_WAKEUP_ON_PLL_LOCK = 14u,
    RFTX_ENABLE_CRYSTAL_WITH_VCO_THEN_WAKEUP_ON_PLL_LOCK = 15u
} RfTxMode_Type;

typedef struct __attribute__((packed))
{
    enum {
        RFTX_OFF = 0,
        RFTX_POWERUP = 1,
        RFTX_WAIT_FOR_PLL = 2,
        RFTX_WAIT_FOR_FIFO = 3,
        RFTX_TRANS_PAYLOAD = 4,
        RFTX_FINISHED = 5,
        RFTX_ERROR_PLL_OR_SUPPLY = 6,
        RFTX_ERROR_FIFO = 7
    } RFTX_INFO : 3;
    uint8_t PLL_LOCKED : 1;
    uint8_t : 1;
    uint8_t PLL_XTAL_RUN : 1;
    uint8_t RFTX_WAIT_LOCK : 2;
} RfTxStatus_Type;

typedef enum {
    SIMPLE_TIMER_DISABLED = 0,
    SIMPLE_TIMER_CPU_CLOCK = 1,
    SIMPLE_TIMER_1US_CLOCK = 2,
    SIMPLE_TIMER_100US_CLOCK = 3
} SimpleTimerClock_Type;

typedef union {
    uint32_t as_dword;
    uint16_t as_word[2];
    uint8_t as_byte[4];
} ChipId_Type;

typedef union {
    uint16_t as_word;
    uint8_t  as_byte[2];
} ChipIdHi_Type;

typedef enum {
    RESET_REASON_UNKNOWN = 0,
    RESET_REASON_AWD = (1u << 0),
    RESET_REASON_SOFT = (1u << 1),
    RESET_REASON_HVDIG = (1u << 2),
    RESET_REASON_DBG = (1u << 3),

    RESET_REASON_POR = (1u << 6),
    RESET_REASON_WAKEUP = (1u << 7)
} ResetReason_Type;

typedef enum {
    PRESSURE_RANGE_UNKNOWN      =   0u,
    PRESSURE_RANGE_100_500_KPA  =   1u,
    PRESSURE_RANGE_100_900_KPA  =   2u,
    PRESSURE_RANGE_100_1400_KPA =   3u
} PressureRange_Type;

typedef enum {
    TEMPSENS_DISABLED = 0u,
    TEMPSENS_ENABLED_WITH_NO_EVENTS = 1u,
    TEMPSENS_EVENT_WHILE_BELOW_LOW_THRESHOLD = 2u,
    TEMPSENS_EVENT_WHILE_ABOVE_LOW_THRESHOLD = 3u,
    TEMPSENS_EVENT_WHILE_ABOVE_HIGH_THRESHOLD = 4u,
    TEMPSENS_EVENT_WHILE_BELOW_HIGH_THRESHOLD = 5u,
    TEMPSENS_EVENT_WHILE_OUT_OF_RANGE = 6u,
    TEMPSENS_EVENT_WHILE_IN_RANGE = 7u
} TempSensorMode_Type;

typedef enum {
    TEMPSENS_IN_RANGE = 0u,
    TEMPSENS_BELOW_RANGE = 1u,
    TEMPSENS_ABOVE_RANGE = 2u
} TempSensorStatus_Type;

typedef struct {
    uint8_t mant     : 8;
    uint8_t exponent : 4;
    uint8_t          : 4;
} WakeupTimerPeriod_Type;

bool AbsWatchdog_IsWindowOpen(void);
void AbsWatchdog_Restart(void);

#if defined(HAS_ACCEL_X_SENSOR)

int16_t Accel_GToRawX(int16_t accel_phys);
int16_t Accel_RawToGX(int16_t accel_raw);

#if defined (HAS_ACCEL_CAPFILTER_REFRESH_BY_HW)
void Accel_InitFilterForX(HWFilterSetup_Type filter_id,
                                     uint16_t user_init_time,
                                     uint16_t period);
#else
void Accel_InitHWFilterForXAxis(HWFilterSetup_Type filter_id,
                                uint16_t user_init_time,
                                uint16_t period);
#endif
int16_t Phys_AccelerationX(void);

#endif /* HAS_ACCEL_X_SENSOR */

#if defined(HAS_ACCEL_Z_SENSOR)

int16_t Accel_GToRawZ(int16_t accel_phys);
int16_t Accel_RawToGZ(int16_t accel_raw);

#if defined (HAS_ACCEL_CAPFILTER_REFRESH_BY_HW)
void Accel_InitFilterForZ(HWFilterSetup_Type filter_id,
                                     uint16_t user_init_time,
                                     uint16_t period);
#else
void Accel_InitHWFilterForZAxis(HWFilterSetup_Type filter_id,
                                uint16_t user_init_time,
                                uint16_t period);
#endif
int16_t Phys_AccelerationZ(void);

#endif /* HAS_ACCEL_Z_SENSOR */

#if defined(HAS_ACCEL_X_SENSOR) || defined(HAS_ACCEL_Z_SENSOR)

void Accel_DeinitFilter(void);
int16_t Accel_WaitAndReadNext(void);
int16_t Accel_GetLatestMeasurement(void);
uint16_t Accel_GetSampleCnt(void);
int16_t Accel_ReadNext(void);
int16_t Accel_PeekNext(void);

#endif /* HAS_ACCEL_X_SENSOR or HAS_ACCEL_Z_SENSOR */

uint16_t Adc_ReadPressure(void);
uint16_t low_power_adc_read_pressure(void);
void Adc_ReadPVT(Adc_RawPvt_Type *raw_pvt);
uint16_t Adc_ReadVoltage(void);
uint16_t Adc_ReadTemperature(void);
uint16_t Adc_ReadGpio(GpioPinId_Type gpio_id);
bool Adc_IsError(void);
void Adc_ClearAllErrors(void);

uint32_t Calc_Pressure(uint16_t adc_raw_pressure,
                              uint16_t adc_raw_temp,
                              uint16_t adc_raw_volt);
uint16_t Calc_Temperature(uint16_t adc_raw_temp);
uint16_t Calc_Voltage(uint16_t adc_raw_volt);
uint32_t Calc_RomBistFor(uint16_t start_addr,
                         uint16_t stop_addr);

bool Check_IsBadAccelZ(int16_t min_threshold, int16_t max_threshold);
bool Check_IsBadAccelX(int16_t min_threshold, int16_t max_threshold);
bool Check_IsBadPressure(void);
DataCheck_Type Check_IsBadMlxData(void);

bool Flash_PageWrite(uint16_t address,
                                uint16_t secure_1,
                                uint16_t secure_2,
                                const uint16_t data[]);
bool Flash_SectorErase(uint16_t address,
                                  uint16_t secure_1,
                                  uint16_t secure_2);
uint16_t Flash_GetSingleErrorCounter(void);
void Flash_ClearSingleErrorCounter(void);

void Gpio_Init(const GpioConfig_Type *cfg);
void Gpio_SetPinMode(GpioPinId_Type pin, GpioPinMode_Type mode);
bool Gpio_ReadPin(GpioPinId_Type pin);
GpioPortLevel_Type Gpio_ReadPort(void);
void Gpio_WritePin(GpioPinId_Type pin, uint16_t value);
bool Gpio_TogglePin(GpioPinId_Type pin);

void Lf_Init(const LfConfig_Type *cfg);
void Lf_Start(LfMode_Type lf_mode);
void Lf_Disable(void);
LfRxState_Type Lf_GetState(void);

uint8_t LfRx_FifoReadByte(void);
uint8_t LfRx_FifoPeekByte(void);
void LfRx_FifoFlush(void);
bool LfRx_IsFifoFull(void);
bool LfRx_IsFifoOverrun(void);
bool LfRx_IsFifoUnderrun(void);
uint8_t LfRx_FifoCount(void);
uint32_t LfRx_ReceivedHeader(void);

void NvRam_Recall(void);
void NvRam_Store(void);
bool NvRam_IsSingleErrorCorrected(void);
void NvRam_ClearSingleError(void);
bool NvRam_IsDoubleErrorDetected(void);
void NvRam_ClearDoubleError(void);
void NvRam_RestoreOscTrimming(void);
void NvRam_RestoreHfoTrimming(void);
void NvRam_RestoreLfoTrimming(void);
void NvRam_RestoreUlpoTrimming(void);

void Radio_Disable(void);

void RfTx_Init(const RfTxConfig_Type *cfg);
bool RfTx_Calibrate(void);
bool RfTx_Run(void);
bool RfTx_RunFromBuf(const uint8_t payload_data[], uint16_t payload_size);
void RfTx_FifoWriteByte(uint8_t data);
void RfTx_FifoFlush(void);
uint8_t RfTx_FifoCount(void);
bool RfTx_IsFifoFull(void);
bool RfTx_IsFifoOverrun(void);
bool RfTx_IsFifoUnderrun(void);
RfTxStatus_Type RfTx_GetStatus(void);
/*
 * Normally the function rftx_precharge below is used inside of the library
 * before any RFTx start in the functions RfTx_Run and RfTx_RunFromBuf
 * Nevertheless, the function rftx_precharge can be needed for some special
 * applications represented by the available example "measure_VDD_at_RFTx"
 */
bool rftx_precharge(void);

void SimpleTimer1_Start(SimpleTimerClock_Type clock_source,
                                   uint16_t period);
void SimpleTimer1_Stop(void);
uint16_t SimpleTimer1_GetCounter(void);
void SimpleTimer2_Start(SimpleTimerClock_Type clock_source,
                                   uint16_t period);
void SimpleTimer2_Stop(void);
uint16_t SimpleTimer2_GetCounter(void);

void Sys_ResetCpu(void) __attribute__((noreturn));
bool Sys_CalibrateHfo(void);
bool Sys_CalibrateLfo(void);
bool Sys_CalibrateUlpo(void);
bool Sys_CalibrateHfoByExtClk(void);
bool Sys_CalibrateLfoByExtClk(void);
bool Sys_CalibrateUlpoByExtClk(void);
void Sys_SetNormalLfo(void);
void Sys_SetUlpLfo(void);
ChipId_Type Sys_GetChipId(void);
ChipIdHi_Type Sys_GetChipIdHi(void);
PressureRange_Type Sys_GetPressureRange(void);
void Sys_HaltWith(LfMode_Type lf_mode);
void Sys_StopWith(LfMode_Type lf_mode);
void Sys_SleepWith(LfMode_Type lf_mode) __attribute__ ((noreturn));
void Sys_DeepSleepWith(LfMode_Type lf_mode) __attribute__ ((noreturn));

bool Sys_IsColdBoot(void);
ResetReason_Type Sys_GetResetReason(void);

void TempSensor_SetMode(TempSensorMode_Type mode);
TempSensorMode_Type TempSensor_GetMode(void);
TempSensorStatus_Type TempSensor_GetStatus(void);
bool TempSensor_IsInRange(void);

void WakeupTimer_Init(uint16_t mant, uint16_t exponent);
void WakeupTimer_Restart(WakeupTimerPeriod_Type period);
void WakeupTimer_IntClear(void);

/* Itc */
void Itc_SetPrio_ITC_ROM_SHELL_MEM_ERR(uint16_t prio);
void Itc_SetPrio_ITC_FLASH_SHELL_ERR(uint16_t prio);
void Itc_SetPrio_ITC_RAM_SHELL_MEM_ERR(uint16_t prio);
void Itc_SetPrio_ITC_NVRAM_SHELL_STORE(uint16_t prio);
void Itc_SetPrio_ITC_STIMER1(uint16_t prio);
void Itc_SetPrio_ITC_STIMER2(uint16_t prio);
void Itc_SetPrio_ITC_ADC_XH018(uint16_t prio);
void Itc_SetPrio_ITC_GPIO0(uint16_t prio);
void Itc_SetPrio_ITC_GPIO1(uint16_t prio);
void Itc_SetPrio_ITC_GPIO2(uint16_t prio);
void Itc_SetPrio_ITC_GPIO3(uint16_t prio);
void Itc_SetPrio_ITC_DIAG0(uint16_t prio);
void Itc_SetPrio_ITC_DIAG1(uint16_t prio);
void Itc_SetPrio_ITC_DIAG2(uint16_t prio);
void Itc_SetPrio_ITC_DIAG3(uint16_t prio);
void Itc_SetPrio_ITC_WUPTMR(uint16_t prio);
void Itc_SetPrio_ITC_TEMPSENS(uint16_t prio);
void Itc_SetPrio_ITC_RFTX_LOCKED(uint16_t prio);
void Itc_SetPrio_ITC_LFTX_HALF(uint16_t prio);
void Itc_SetPrio_ITC_LFTX_DONE(uint16_t prio);
void Itc_SetPrio_ITC_LFRX_SNIFF(uint16_t prio);
void Itc_SetPrio_ITC_LFRX_TIMEOUT(uint16_t prio);
void Itc_SetPrio_ITC_LFRX_SYNC(uint16_t prio);
void Itc_SetPrio_ITC_LFRX_DATA(uint16_t prio);
void Itc_SetPrio_ITC_LFRX_DONE(uint16_t prio);
void Itc_SetPrio_ITC_FREQCNT_DONE(uint16_t prio);

void Itc_Enable_ITC_ROM_SHELL_MEM_ERR(void);
void Itc_Enable_ITC_FLASH_SHELL_ERR(void);
void Itc_Enable_ITC_RAM_SHELL_MEM_ERR(void);
void Itc_Enable_ITC_NVRAM_SHELL_STORE(void);
void Itc_Enable_ITC_STIMER1(void);
void Itc_Enable_ITC_STIMER2(void);
void Itc_Enable_ITC_ADC_XH018(void);
void Itc_Enable_ITC_GPIO0(void);
void Itc_Enable_ITC_GPIO1(void);
void Itc_Enable_ITC_GPIO2(void);
void Itc_Enable_ITC_GPIO3(void);
void Itc_Enable_ITC_DIAG0(void);
void Itc_Enable_ITC_DIAG1(void);
void Itc_Enable_ITC_DIAG2(void);
void Itc_Enable_ITC_DIAG3(void);
void Itc_Enable_ITC_WUPTMR(void);
void Itc_Enable_ITC_TEMPSENS(void);
void Itc_Enable_ITC_RFTX_LOCKED(void);
void Itc_Enable_ITC_LFTX_HALF(void);
void Itc_Enable_ITC_LFTX_DONE(void);
void Itc_Enable_ITC_LFRX_SNIFF(void);
void Itc_Enable_ITC_LFRX_TIMEOUT(void);
void Itc_Enable_ITC_LFRX_SYNC(void);
void Itc_Enable_ITC_LFRX_DATA(void);
void Itc_Enable_ITC_LFRX_DONE(void);
void Itc_Enable_ITC_FREQCNT_DONE(void);
void Itc_Enable_ITC_MLX16_EXCHG(void);
void Itc_Enable_ITC_MLX16_DMAERR(void);
void Itc_Enable_ITC_AWD_ATT(void);
void Itc_Enable_ITC_MLX16_SOFT(void);

void Itc_Disable_ITC_ROM_SHELL_MEM_ERR(void);
void Itc_Disable_ITC_FLASH_SHELL_ERR(void);
void Itc_Disable_ITC_RAM_SHELL_MEM_ERR(void);
void Itc_Disable_ITC_NVRAM_SHELL_STORE(void);
void Itc_Disable_ITC_STIMER1(void);
void Itc_Disable_ITC_STIMER2(void);
void Itc_Disable_ITC_ADC_XH018(void);
void Itc_Disable_ITC_GPIO0(void);
void Itc_Disable_ITC_GPIO1(void);
void Itc_Disable_ITC_GPIO2(void);
void Itc_Disable_ITC_GPIO3(void);
void Itc_Disable_ITC_DIAG0(void);
void Itc_Disable_ITC_DIAG1(void);
void Itc_Disable_ITC_DIAG2(void);
void Itc_Disable_ITC_DIAG3(void);
void Itc_Disable_ITC_WUPTMR(void);
void Itc_Disable_ITC_TEMPSENS(void);
void Itc_Disable_ITC_RFTX_LOCKED(void);
void Itc_Disable_ITC_LFTX_HALF(void);
void Itc_Disable_ITC_LFTX_DONE(void);
void Itc_Disable_ITC_LFRX_SNIFF(void);
void Itc_Disable_ITC_LFRX_TIMEOUT(void);
void Itc_Disable_ITC_LFRX_SYNC(void);
void Itc_Disable_ITC_LFRX_DATA(void);
void Itc_Disable_ITC_LFRX_DONE(void);
void Itc_Disable_ITC_FREQCNT_DONE(void);
void Itc_Disable_ITC_MLX16_EXCHG(void);
void Itc_Disable_ITC_MLX16_DMAERR(void);
void Itc_Disable_ITC_AWD_ATT(void);
void Itc_Disable_ITC_MLX16_SOFT(void);

uint16_t Itc_IsPending_ITC_ROM_SHELL_MEM_ERR(void);
uint16_t Itc_IsPending_ITC_FLASH_SHELL_ERR(void);
uint16_t Itc_IsPending_ITC_RAM_SHELL_MEM_ERR(void);
uint16_t Itc_IsPending_ITC_NVRAM_SHELL_STORE(void);
uint16_t Itc_IsPending_ITC_STIMER1(void);
uint16_t Itc_IsPending_ITC_STIMER2(void);
uint16_t Itc_IsPending_ITC_ADC_XH018(void);
uint16_t Itc_IsPending_ITC_GPIO0(void);
uint16_t Itc_IsPending_ITC_GPIO1(void);
uint16_t Itc_IsPending_ITC_GPIO2(void);
uint16_t Itc_IsPending_ITC_GPIO3(void);
uint16_t Itc_IsPending_ITC_DIAG0(void);
uint16_t Itc_IsPending_ITC_DIAG1(void);
uint16_t Itc_IsPending_ITC_DIAG2(void);
uint16_t Itc_IsPending_ITC_DIAG3(void);
uint16_t Itc_IsPending_ITC_WUPTMR(void);
uint16_t Itc_IsPending_ITC_TEMPSENS(void);
uint16_t Itc_IsPending_ITC_RFTX_LOCKED(void);
uint16_t Itc_IsPending_ITC_LFTX_HALF(void);
uint16_t Itc_IsPending_ITC_LFTX_DONE(void);
uint16_t Itc_IsPending_ITC_LFRX_SNIFF(void);
uint16_t Itc_IsPending_ITC_LFRX_TIMEOUT(void);
uint16_t Itc_IsPending_ITC_LFRX_SYNC(void);
uint16_t Itc_IsPending_ITC_LFRX_DATA(void);
uint16_t Itc_IsPending_ITC_LFRX_DONE(void);
uint16_t Itc_IsPending_ITC_FREQCNT_DONE(void);
uint16_t Itc_IsPending_ITC_MLX16_EXCHG(void);
uint16_t Itc_IsPending_ITC_MLX16_DMAERR(void);
uint16_t Itc_IsPending_ITC_AWD_ATT(void);
uint16_t Itc_IsPending_ITC_MLX16_SOFT(void);

void Itc_ClearPending_ITC_ROM_SHELL_MEM_ERR(void);
void Itc_ClearPending_ITC_FLASH_SHELL_ERR(void);
void Itc_ClearPending_ITC_RAM_SHELL_MEM_ERR(void);
void Itc_ClearPending_ITC_NVRAM_SHELL_STORE(void);
void Itc_ClearPending_ITC_STIMER1(void);
void Itc_ClearPending_ITC_STIMER2(void);
void Itc_ClearPending_ITC_ADC_XH018(void);
void Itc_ClearPending_ITC_GPIO0(void);
void Itc_ClearPending_ITC_GPIO1(void);
void Itc_ClearPending_ITC_GPIO2(void);
void Itc_ClearPending_ITC_GPIO3(void);
void Itc_ClearPending_ITC_DIAG0(void);
void Itc_ClearPending_ITC_DIAG1(void);
void Itc_ClearPending_ITC_DIAG2(void);
void Itc_ClearPending_ITC_DIAG3(void);
void Itc_ClearPending_ITC_WUPTMR(void);
void Itc_ClearPending_ITC_TEMPSENS(void);
void Itc_ClearPending_ITC_RFTX_LOCKED(void);
void Itc_ClearPending_ITC_LFTX_HALF(void);
void Itc_ClearPending_ITC_LFTX_DONE(void);
void Itc_ClearPending_ITC_LFRX_SNIFF(void);
void Itc_ClearPending_ITC_LFRX_TIMEOUT(void);
void Itc_ClearPending_ITC_LFRX_SYNC(void);
void Itc_ClearPending_ITC_LFRX_DATA(void);
void Itc_ClearPending_ITC_LFRX_DONE(void);
void Itc_ClearPending_ITC_FREQCNT_DONE(void);
void Itc_ClearPending_ITC_MLX16_EXCHG(void);
void Itc_ClearPending_ITC_MLX16_DMAERR(void);
void Itc_ClearPending_ITC_AWD_ATT(void);
void Itc_ClearPending_ITC_MLX16_SOFT(void);

/* Wakeup */
void Wakeup_Enable_WAKEUP_LFTX_HALF(void);
void Wakeup_Enable_WAKEUP_LFRX_SNIFF(void);
void Wakeup_Enable_WAKEUP_LFRX_TIMEOUT(void);
void Wakeup_Enable_WAKEUP_LFRX_SYNC(void);
void Wakeup_Enable_WAKEUP_LFRX_DATA(void);
void Wakeup_Enable_WAKEUP_LF_DONE(void);
void Wakeup_Enable_WAKEUP_GPIO0_LOW(void);
void Wakeup_Enable_WAKEUP_GPIO0_HIGH(void);
void Wakeup_Enable_WAKEUP_GPIO1_LOW(void);
void Wakeup_Enable_WAKEUP_GPIO1_HIGH(void);
void Wakeup_Enable_WAKEUP_GPIO2_LOW(void);
void Wakeup_Enable_WAKEUP_GPIO2_HIGH(void);
void Wakeup_Enable_WAKEUP_GPIO3_LOW(void);
void Wakeup_Enable_WAKEUP_GPIO3_HIGH(void);
void Wakeup_Enable_TEMP_BELOW_LOW_THRESHOLD(void);
void Wakeup_Enable_TEMP_ABOVE_LOW_THRESHOLD(void);
void Wakeup_Enable_TEMP_ABOVE_HIGH_THRESHOLD(void);
void Wakeup_Enable_TEMP_BELOW_HIGH_THRESHOLD(void);
void Wakeup_Enable_TEMP_OUT_OF_RANGE(void);
void Wakeup_Enable_TEMP_IN_RANGE(void);

void Wakeup_Disable_WAKEUP_LFTX_HALF(void);
void Wakeup_Disable_WAKEUP_LFRX_SNIFF(void);
void Wakeup_Disable_WAKEUP_LFRX_TIMEOUT(void);
void Wakeup_Disable_WAKEUP_LFRX_SYNC(void);
void Wakeup_Disable_WAKEUP_LFRX_DATA(void);
void Wakeup_Disable_WAKEUP_LF_DONE(void);
void Wakeup_Disable_WAKEUP_GPIO0_LOW(void);
void Wakeup_Disable_WAKEUP_GPIO0_HIGH(void);
void Wakeup_Disable_WAKEUP_GPIO1_LOW(void);
void Wakeup_Disable_WAKEUP_GPIO1_HIGH(void);
void Wakeup_Disable_WAKEUP_GPIO2_LOW(void);
void Wakeup_Disable_WAKEUP_GPIO2_HIGH(void);
void Wakeup_Disable_WAKEUP_GPIO3_LOW(void);
void Wakeup_Disable_WAKEUP_GPIO3_HIGH(void);
void Wakeup_Disable_TEMP_BELOW_LOW_THRESHOLD(void);
void Wakeup_Disable_TEMP_ABOVE_LOW_THRESHOLD(void);
void Wakeup_Disable_TEMP_ABOVE_HIGH_THRESHOLD(void);
void Wakeup_Disable_TEMP_BELOW_HIGH_THRESHOLD(void);
void Wakeup_Disable_TEMP_OUT_OF_RANGE(void);
void Wakeup_Disable_TEMP_IN_RANGE(void);

#endif /* TPMSLIB_H */
