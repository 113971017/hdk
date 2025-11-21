/*
 * Copyright (C) 2018 Melexis N.V.
 *
 * Software Platform
 *
 */

#ifndef DIAG_H
#define DIAG_H

/**
    @defgroup Diag Diagnostic Interface, <diag.h>

    Monitor the signal to be within specified range using two comparators with
    configurable thresholds and hysteresis.

    The purpose is to have 4 parallel interfaces available with programmable
    reference and channel selection. This will allow the system to monitor a
    set of four different signals and verify that they are inside a given
    window based on an upper and lower threshold independently programmable
    as ratios of the selected reference.
 */

#include <stdint.h>
#include <stdbool.h>

#include <compiler_abstraction.h>
#include <static_assert.h>
#include <io.h>
#include <delay.h>

/** @ingroup Diag */
/** Diagnostic Unit ID */
typedef enum {
    DIAG0 = 0,          /**< Diagnostic channel 0 */
    DIAG1 = 1,          /**< Diagnostic channel 1 */
    DIAG2 = 2,          /**< Diagnostic channel 2 */
    DIAG3 = 3           /**< Diagnostic channel 3 */
} DiagUnitId_Type;

/** @ingroup Diag */
/** Diagnostic event mode */
typedef enum {
    DIAG_DISABLED                           = 0,    /**< no events, diagnostic interface is OFF  */
    DIAG_ENABLED_WITH_NO_EVENTS             = 1,    /**< no events, diagnostic interface is ON (polling mode) */
    DIAG_EVENT_WHILE_BELOW_LOWER_THRESHOLD  = 2,    /**< wake-up event <b>as long as</b> diagnostic interface is below lower threshold */
    DIAG_EVENT_WHILE_ABOVE_LOWER_THRESHOLD  = 3,    /**< wake-up event <b>as long as</b> diagnostic interface is above lower threshold */
    DIAG_EVENT_WHILE_ABOVE_UPPER_THRESHOLD  = 4,    /**< wake-up event <b>as long as</b> diagnostic interface is above upper threshold */
    DIAG_EVENT_WHILE_BELOW_UPPER_THRESHOLD  = 5,    /**< wake-up event <b>as long as</b> diagnostic interface is below upper threshold */
    DIAG_EVENT_WHILE_OUT_OF_RANGE           = 6,    /**< wake-up event <b>as long as</b> diagnostic interface is outside specified range */
    DIAG_EVENT_WHILE_IN_RANGE               = 7     /**< wake-up event <b>as long as</b> diagnostic interface is within specified range */
} DiagMode_Type;

/** @ingroup Diag */
/** Diagnostic channel type */
typedef enum {
    DIAG_CH_GPIO0               = 0,    /**< AIO<0> (GPIO0 configured in analog IO mode)    */
    DIAG_CH_GPIO1               = 1,    /**< AIO<1> (GPIO1 configured in analog IO mode)    */
    DIAG_CH_GPIO2               = 2,    /**< AIO<2> (GPIO2 configured in analog IO mode)    */
    DIAG_CH_GPIO3               = 3,    /**< AIO<3> (GPIO3 configured in analog IO mode)    */
    DIAG_CH_PRESURE_POSITIVE    = 4,    /**< PS_SP (Pressure sensor sensing node positive)  */
    DIAG_CH_PRESURE_NEGATIVE    = 5,    /**< PS_SN (Pressure sensor sensing node negative)  */
    DIAG_CH_RF_POWER_DETECTOR   = 6,    /**< RF_VPD: RF power detector output   */
    /* 7: Reserved for future use (currently connected to VSSA) */
    DIAG_CH_VDD_X_0_25          = 8,    /**< VDD  * 0.25        */
    DIAG_CH_VANA_X_0_6          = 9,    /**< VANA * 0.6         */
    DIAG_CH_VPA_X_0_6           = 10,   /**< VPA  * 0.6         */
    DIAG_CH_VDIG_X_0_6          = 11,   /**< VDIG * 0.6         */
    DIAG_CH_VDIG_RAM_X_0_6      = 12,   /**< VDIG_RAM * 0.6     */
    DIAG_CH_VNVM_0_25           = 13,   /**< VNVM * 0.25        */
    DIAG_CH_TEMPERATURE         = 14,   /**< SENS_VTEMP (temperature sensor) */
    /* 15:  Reserved for internal test mode configuration
            OUTH and OUTL comparator: comp- = VREFH, comp+ = VREFL
     */
} DiagChannel_Type;

/** @ingroup Diag */
/** Diagnostic reference source */
typedef enum {
    DIAG_REF_VDD     = 0,           /**< VDD    */
    DIAG_REF_VBG     = 1,           /**< VBG (internal bandgap reference voltage 1.2V) */
    DIAG_REF_VANA    = 2,           /**< VANA   */
    DIAG_REF_GPIO0   = 3,           /**< AIO<0>: GPIO0 configured in analog IO mode */
    DIAG_REF_GPIO1   = 4,           /**< AIO<1>: GPIO1 configured in analog IO mode */
    DIAG_REF_GPIO2   = 5,           /**< AIO<2>: GPIO2 configured in analog IO mode */
    DIAG_REF_GPIO3   = 6,           /**< AIO<3>: GPIO3 configured in analog IO mode */
    DIAG_REF_VSSA    = 7            /**< VSSA */
}   DiagRefSource_Type;             /* defined in [1] above */

/** @ingroup Diag */
typedef uint8_t DiagThreshold_Type; /**< Diagnostic threshold value (7-bit) */

/** @ingroup Diag */
/** Diagnostic hysteresis type */
typedef enum {
    DIAG_HYST_OFF   = 0,            /**< no hysteresis */
    DIAG_HYST_1_LSB = 1,            /**< 1 LSB of reference DAC */
    DIAG_HYST_2_LSB = 2,            /**< 2 LSB of reference DAC */
    DIAG_HYST_4_LSB = 3             /**< 4 LSB of reference DAC */
}  DiagHyst_Type;                   /* as per version 1.15 of the system specification */

/** @ingroup Diag */
/** Diagnostic status type */
typedef enum {
    DIAG_STATUS_IN_RANGE    = 0,    /**< signal is in range;    DIAGx_HIGH = 0, DIAGx_LOW = 0   */
    DIAG_STATUS_BELOW_RANGE = 1,    /**< signal is below range; DIAGx_HIGH = 0, DIAGx_LOW = 1   */
    DIAG_STATUS_ABOVE_RANGE = 2     /**< signal is above range; DIAGx_HIGH = 1, DIAGx_LOW = 0   */
    /* 3 */                         /* NA;                      DIAGx_HIGH = 1, DIAGx_LOW = 1   */
} DiagStatus_Type;

/** @ingroup Diag */
/** Diagnostic status for all channels  */
typedef struct __attribute__((packed)) {
    DiagStatus_Type DIAG0 : 2;      /**< status for channel #0 */
    DiagStatus_Type DIAG1 : 2;      /**< status for channel #1 */
    DiagStatus_Type DIAG2 : 2;      /**< status for channel #2 */
    DiagStatus_Type DIAG3 : 2;      /**< status for channel #3 */
} DiagStatusAll_Type;
ASSERT(sizeof(DiagStatusAll_Type) == 1u);

/** @ingroup Diag */
/** Diagnostic configuration */
typedef union {
    /** Diagnostic configuration */
    struct {
        DiagStatus_Type     /* DIAG0 */     : 2;
        DiagStatus_Type     /* DIAG1 */     : 2;
        DiagStatus_Type     /* DIAG2 */     : 2;
        DiagStatus_Type     /* DIAG3 */     : 2;
        DiagRefSource_Type  DIAG_REF_SEL    : 3;    /**< reference voltage selection for diagnostic interfaces */
        uint16_t                            : 5;

        DiagChannel_Type    DIAG0_CH_SEL    : 4;    /**< channel selection for diagnostic interface #0 */
        uint16_t                            : 2;
        DiagHyst_Type       DIAG0_HYST      : 2;    /**< hysteresis setting for diagnostic interface #0 */
        DiagChannel_Type    DIAG1_CH_SEL    : 4;    /**< channel selection for diagnostic interface #1 */
        uint16_t                            : 2;
        DiagHyst_Type       DIAG1_HYST      : 2;    /**< hysteresis setting for diagnostic interface #1 */

        DiagChannel_Type    DIAG2_CH_SEL    : 4;    /**< channel selection for diagnostic interface #2 */
        uint16_t                            : 2;
        DiagHyst_Type       DIAG2_HYST      : 2;    /**< hysteresis setting for diagnostic interface #2 */
        DiagChannel_Type    DIAG3_CH_SEL    : 4;    /**< channel selection for diagnostic interface #3 */
        uint16_t                            : 2;
        DiagHyst_Type       DIAG3_HYST      : 2;    /**< hysteresis setting for diagnostic interface #3 */

        DiagThreshold_Type  DIAG0_VREFL     : 7;    /**< lower threshold level for diagnostic interface #0 */
        uint16_t                            : 1;
        DiagThreshold_Type  DIAG0_VREFH     : 7;    /**< upper threshold level for diagnostic interface #0 */
        uint16_t                            : 1;

        DiagThreshold_Type  DIAG1_VREFL     : 7;    /**< lower threshold level for diagnostic interface #1 */
        uint16_t                            : 1;
        DiagThreshold_Type  DIAG1_VREFH     : 7;    /**< upper threshold level for diagnostic interface #1 */
        uint16_t                            : 1;

        DiagThreshold_Type  DIAG2_VREFL     : 7;    /**< lower threshold level for diagnostic interface #2 */
        uint16_t                            : 1;
        DiagThreshold_Type  DIAG2_VREFH     : 7;    /**< upper threshold level for diagnostic interface #2 */
        uint16_t                            : 1;

        DiagThreshold_Type  DIAG3_VREFL     : 7;    /**< lower threshold level for diagnostic interface #3 */
        uint16_t                            : 1;
        DiagThreshold_Type  DIAG3_VREFH     : 7;    /**< upper threshold level for diagnostic interface #3 */
        uint16_t                            : 1;
    };
} DiagConfig_Type;
ASSERT(sizeof(DiagConfig_Type) == 14u);

/* Validate IO ports order in memory */
ASSERT((IO_ADDRESS(CUSTOM_91804, DIAG0_CH_SEL) - IO_ADDRESS(CUSTOM_91804, DIAG0_LOW))    == 2u);
ASSERT((IO_ADDRESS(CUSTOM_91804, DIAG2_CH_SEL) - IO_ADDRESS(CUSTOM_91804, DIAG0_CH_SEL)) == 2u);
ASSERT((IO_ADDRESS(CUSTOM_91804, DIAG0_VREFL)  - IO_ADDRESS(CUSTOM_91804, DIAG2_CH_SEL)) == 2u);
ASSERT((IO_ADDRESS(CUSTOM_91804, DIAG1_VREFL)  - IO_ADDRESS(CUSTOM_91804, DIAG0_VREFL))  == 2u);
ASSERT((IO_ADDRESS(CUSTOM_91804, DIAG2_VREFL)  - IO_ADDRESS(CUSTOM_91804, DIAG1_VREFL))  == 2u);
ASSERT((IO_ADDRESS(CUSTOM_91804, DIAG3_VREFL)  - IO_ADDRESS(CUSTOM_91804, DIAG2_VREFL))  == 2u);


/* API */
static INLINE void Diag_Init(const DiagConfig_Type *cfg);

/* Control */
static INLINE void Diag_SetReferenceSource(DiagRefSource_Type ref);
static INLINE void Diag_ConfigChannel(DiagUnitId_Type unit, DiagChannel_Type channel, DiagThreshold_Type th_low, DiagThreshold_Type th_high, DiagHyst_Type hyst);
static INLINE void Diag_SetMode(DiagUnitId_Type unit, DiagMode_Type mode);

/* Status */
static INLINE DiagStatusAll_Type Diag_GetStatusAll(void);
static INLINE bool Diag_IsInRangeAll(void);
static INLINE bool Diag_IsInRange(DiagUnitId_Type unit);

/* --- Implementation ----------------------------------------------------------- */
/* Helper function template */
#define DIAG_CONFIG_CHANNEL_(unit, channel, th_low, th_high, hyst)  \
    do {                                                            \
        IO_SET(CUSTOM_91804,                                        \
            DIAG##unit##_CH_SEL, channel,                           \
            DIAG##unit##_HYST,   hyst);                             \
        IO_SET(CUSTOM_91804,                                        \
            DIAG##unit##_VREFL, th_low,                             \
            DIAG##unit##_VREFH, th_high);                           \
    } while(false)

#define DIAG_SET_MODE_(unit, mode)                                  \
    do {                                                            \
        IO_SET(CUSTOM_91804,                                        \
            POL_DIAG##unit, mode);                                  \
    } while(false)

/** @ingroup Diag
    Initialize Diagnostic Interface

    @param  cfg     Pointer to configuration data structure (see ::DiagConfig_Type for details)

    @pre
    It is strongly recommended to assign all values directly in the
    configuration data structure. Being not assigned, the value takes on zero.
 */
static INLINE
void Diag_Init(const DiagConfig_Type *cfg)
{
    extern volatile DiagConfig_Type port_diag_cfg_  __attribute__((addr(IO_BYTE_ADDRESS(CUSTOM_91804, DIAG0_LOW))));
    port_diag_cfg_ = *cfg;
}

/** @ingroup Diag
    Set global reference for all diagnostic channels

    @param   ref    3-bit reference voltage selection
 */
static INLINE
void Diag_SetReferenceSource(DiagRefSource_Type ref)
{
    IO_SET(CUSTOM_91804, DIAG_REF_SEL, ref);
}

/** @ingroup Diag
    Configure diagnostic unit

    @param   unit        Id of the Diagnostic unit (see ::DiagUnitId_Type)
    @param   channel     channel for monitoring (see ::DiagChannel_Type)
    @param   th_low      low threshold (see ::DiagThreshold_Type)
    @param   th_high     high threshold (see ::DiagThreshold_Type)
    @param   hyst        hysteresis (see ::DiagHyst_Type)
 */
static INLINE
void Diag_ConfigChannel(DiagUnitId_Type unit,
                      DiagChannel_Type channel,
                      DiagThreshold_Type th_low,
                      DiagThreshold_Type th_high,
                      DiagHyst_Type hyst)
{
    switch (unit) {
        case DIAG0: DIAG_CONFIG_CHANNEL_(0, channel, th_low, th_high, hyst); break;
        case DIAG1: DIAG_CONFIG_CHANNEL_(1, channel, th_low, th_high, hyst); break;
        case DIAG2: DIAG_CONFIG_CHANNEL_(2, channel, th_low, th_high, hyst); break;
        case DIAG3: DIAG_CONFIG_CHANNEL_(3, channel, th_low, th_high, hyst); break;
        default: /* ignore incorrect unit */ break;
    }
}


/** @ingroup Diag
    Set diagnostic interface mode

    @param  unit    Id of the Diagnostic unit (see ::DiagUnitId_Type)
    @param  mode    Diagnostic Interface Mode (see ::DiagMode_Type for details)

    @note
    -   Any mode (except DIAG_DISABLED and DIAG_ENABLED_WITH_NO_EVENTS)
        also enables generation of diagnostic events <b>as long as</b> configured
        condition is met. Events will further trigger wake-up and/or interrupt
        depending on current power down mode.
 */
static INLINE
void Diag_SetMode(DiagUnitId_Type unit, DiagMode_Type mode)
{
    switch (unit) {
        case DIAG0: DIAG_SET_MODE_(0, mode); break;
        case DIAG1: DIAG_SET_MODE_(1, mode); break;
        case DIAG2: DIAG_SET_MODE_(2, mode); break;
        case DIAG3: DIAG_SET_MODE_(3, mode); break;
        default: /* ignore incorrect unit */ break;
    }
    /* The settling time of the Diagnostic Interface (gitlab #151) is chosen
     * to ensure 10us delay at the highest possible MCU frequency = 10.4 MHz
     * The delay is formed by the MCU commands after the diagnostic interface
     * configuration and before possible read of the diagnostic status
     * The main part of the settling time is created due to the inline function
     * delay_nops adding a delay with a duration according to the formula below:
     * Delay expressed in the HFO periods = 5 + 5 + 4*(n-2) = 4*n + 2
     * where "n" is the value of the input parameter
     * So, with the input parameter = 26u the function delay_nops at 10.4 MHz
     * creates a delay = (4*26 + 2) / 10.4 MHz = 10.2 us that is > 10 us
     * Actually the real delay will be even longer because of the neighbour
     * MCU instructions (e.g. due to passing of the input parameter "n") */
    delay_nops(26u);
}

/** @ingroup Diag
    Returns status of all Diagnostic Interface units

    @returns Diagnostic status of all units (see ::DiagStatusAll_Type)

    @note
    -   To get diagnostic from the individual diagnostic unit, use `Diag_GetStatusAll().DIAG<n>`,
        where `<n>` is an index of the diagnostic unit (0..3)
 */
static INLINE
DiagStatusAll_Type Diag_GetStatusAll(void)
{
    extern const volatile DiagStatusAll_Type port_diag_all_ __attribute__((addr(IO_ADDRESS(CUSTOM_91804, DIAG0_LOW))));
    return port_diag_all_;
}


/** @ingroup Diag
    Returns true if all Diagnostics Interfaces are in range

    @returns true if all Diagnostics Interfaces are in range (Ok)
 */
static INLINE
bool Diag_IsInRangeAll(void)
{
    DiagStatusAll_Type s = Diag_GetStatusAll();
    return (s.DIAG0 == DIAG_STATUS_IN_RANGE)
        && (s.DIAG1 == DIAG_STATUS_IN_RANGE)
        && (s.DIAG2 == DIAG_STATUS_IN_RANGE)
        && (s.DIAG3 == DIAG_STATUS_IN_RANGE);
}


/** @ingroup Diag
    Returns true if signal monitored by the Diagnostic Interface is in specified range

    @param  unit    Id of the Diagnostic unit (see ::DiagUnitId_Type)

    @returns true if diagnostic is in specified range (Ok)
 */
static INLINE
bool Diag_IsInRange(DiagUnitId_Type unit)
{
    switch (unit) {
        case DIAG0: return Diag_GetStatusAll().DIAG0 == DIAG_STATUS_IN_RANGE; break;
        case DIAG1: return Diag_GetStatusAll().DIAG1 == DIAG_STATUS_IN_RANGE; break;
        case DIAG2: return Diag_GetStatusAll().DIAG2 == DIAG_STATUS_IN_RANGE; break;
        case DIAG3: return Diag_GetStatusAll().DIAG3 == DIAG_STATUS_IN_RANGE; break;
        default: /* ignore incorrect unit */ break;
    }
    return false;
}

#endif /* DIAG_H */
