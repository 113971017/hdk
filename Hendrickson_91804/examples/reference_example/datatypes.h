/*
 * Copyright (C) 2018-2019 Melexis N.V.
 *
 * Software Platform
 *
 */
#ifndef DATATYPES_H
#define DATATYPES_H

#include <stdint.h>
#include <stdbool.h>
#include <platformlib.h>

/* ----------------------------------------------------------------------------
 * Reference Example version number
 */
#define APP_VERSION_MAJOR   1UL /* from 0 till 15 */
#define APP_VERSION_MINOR   0UL /* from 0 till 15 */
#define APP_VERSION         (((APP_VERSION_MAJOR << 4) & 0x00F0) | (APP_VERSION_MINOR & 0x000F))
/* Version range validation: */
#if    (APP_VERSION_MAJOR > 15) || (APP_VERSION_MINOR > 15)
#error "Version numbers should be < 16"
#endif
                            
/* ----------------------------------------------------------------------------
 * RFTX_DATAGRAM_PARAMETERS section definition
 * The section is used to specify constants in the program memory
 */
#define RODATA_MEM __attribute__ ((section(".rodata,\"a\",@progbits ;"))) volatile
/* #define RODATA_MEM const => to be used for the mlx16-gcc3 */

/* ----------------------------------------------------------------------------
 * General constants */

/* Acceptable range for VDA supplier ID is 0...15, value 15 is used for non-listed companies */
#define VDA_SUPPLIER_ID             (15u)

#define DEFAULT_TPMS_STATE          DEMO    /* @@@ can be OFF */

#define OFF_PRESSURE_THRESHOLD      (130u)  /* kPa */

/* ----------------------------------------------------------------------------
 * RFTx communication constants */

/* 3-bit of modulation source selection:
0 = Fixed logic 0, 1 = All raw data from FIFO, no preamble, sync pattern or packet
2 = Preamble and sync pattern, no packet, 3 = Preamble, sync pattern and packet,
4 = GPIO0 input, 5 = GPIO1 input, 6 = GPIO2 input, 7 = GPIO3 input */
#define CONFIG_RFTX_DIRECT_MOD      (3u)    /* Preamble, sync pattern and packet */
/* Enable Manchester coding of the data bits: 0 = NRZ, 1 = Manchester */
#define CONFIG_RFTX_EN_MANCHESTER   (1u)    /* Manchester data coding */
/* Configure the polarity of the data bits: 1 = with inversion, 0 = without inversion */
#define CONFIG_RFTX_BIT_INVERT      (1u)    /* With data inversion */
/* Select the modulation type: 0 = OOK, 1 = FSK */
#define CONFIG_RFTX_FSK_NOOK        (1u)    /* FSK */

/* Maximal temperature deviation in [degC] that doesn't require RFTx calibration */
#define CALIB_TEMPERATURE_THRESHOLD (20u)   /* DegC */
#define UNKNOWN_TEMPERATURE         (0xFFu)

#define RFTx_MIN_RANDOM_DELAY       (50u)   /* Fixed minimal random delay in msec (must be < 128) */

#define VDA_ID_SIZE                 (5u)    /* 1 byte of the Packet ID + 4 bytes of module / chip ID */
#define VDA_DATA_SIZE               (8u)    /* Maximal data size allowed by VDA after the ID data */

#define RFTX_MAXIMAL_PAYLOAD_SIZE   (VDA_ID_SIZE+VDA_DATA_SIZE)

typedef enum {              /* List of supported RFTx datagram formats: */
    RFTX_PT_REPORT_W,       /* Basic VDA report with just Pressure and Temperature in working mode */
    RFTX_PT_RESPONSE,       /* Response on the basic LF command with just Pressure and Temperature */
    RFTX_PT_REPORT_L,       /* Basic VDA report with just Pressure and Temperature in learning mode */
    RFTX_TIN_CODES,         /* VDA report with TIN CODES */
    RFTX_TIN_DATE,          /* VDA report with TIN DATE) */
    RFTX_TIRE_DATA,         /* VDA report with information about the TIRE DIMENSION and TYPE */
    RFTX_MILEAGE,           /* VDA report with MILEAGE and MILEAGE UNDERINFLATED information */
    RFTX_DEMO,              /* Demo specific report */
    RFTX_PVT_REPORT,        /* Application specific report with PVT data */
    RFTX_LEARN_REPORT,      /* Application specific report with LEARN data */
    RFTX_CONF_RESPONSE,     /* Application specific LF command confirmation response */
    RFTX_VERSION_DATA,      /* Application specific report with Version data */
    RFTX_MEMORY_DATA,       /* Application specific report with 4 bytes of nvRAM data */
    NUMBER_OF_RFTX_FORMATS  /* Number of RFTx datagrams should be always at the end of the list */
} rftx_t;

/* Data structure to specify the RFTx datagram format (formats' properties) */
typedef struct {
    enum {                                  /* The Packet Identifiers below follows the VDA TPMS standard: */
        VDA_PACKET_ID_PT_REPORT_W   = 0x00, /* Basic VDA report with just Pressure and Temperature in working mode */
        VDA_PACKET_ID_PT_RESPONSE   = 0x01, /* Response on the basic LF command with just Pressure and Temperature */
        VDA_PACKET_ID_PT_REPORT_L   = 0x20, /* Basic VDA report with just Pressure and Temperature in learning mode */
        VDA_PACKET_ID_TIN_CODES     = 0x50, /* VDA report with TIN CODES */
        VDA_PACKET_ID_TIN_DATE      = 0x51, /* VDA report with TIN DATE */
        VDA_PACKET_ID_TIRE_DATA     = 0x52, /* VDA report with information about the TIRE DIMENSION and TYPE */
        VDA_PACKET_ID_MILEAGE       = 0x53, /* VDA report with MILEAGE and MILEAGE UNDERINFLATED information */
        VDA_PACKET_ID_DEMO          = 0xC2, /* Demo specific report */
        VDA_PACKET_ID_PVT_REPORT    = 0xC3, /* Application specific report with PVT data */
        VDA_PACKET_ID_LEARN_REPORT  = 0xC4, /* Application specific report with LEARN data */
        VDA_PACKET_ID_CONF_RESPONSE = 0xC5, /* Application specific confirmation response for LF command */
        VDA_PACKET_ID_VERSION_DATA  = 0xC6, /* Application specific report with Version data */
        VDA_PACKET_ID_MEMORY_DATA   = 0xC7  /* Application specific report with Memory data */
    }   RFTx_VDA_packet_ID          : 8;    /* 8-bit Packet ID of VDA RF format */
    enum {                                  /* The Packet Identifiers below follows the VDA TPMS standard: */
        RFTX_PAYLOAD_SIZE_PT        = (VDA_ID_SIZE+2),              /* It combines all 3 PT datagrams */
        RFTX_PAYLOAD_SIZE_TIN       = (VDA_ID_SIZE+VDA_DATA_SIZE),  /* VDA report with TIN CODES / DATE */
        RFTX_PAYLOAD_SIZE_TARE      = (VDA_ID_SIZE+VDA_DATA_SIZE),  /* Data with TIRE DIMENSION and TYPE */
        RFTX_PAYLOAD_SIZE_MILEAGE   = (VDA_ID_SIZE+VDA_DATA_SIZE),  /* Data with MILEAGE and MILEAGE UNDERINFLATED */
        RFTX_PAYLOAD_SIZE_DEMO      = (VDA_ID_SIZE+8),              /* Demo specific report */
        RFTX_PAYLOAD_SIZE_PVT       = (VDA_ID_SIZE+4),              /* Datagram with PVT data */
        RFTX_PAYLOAD_SIZE_LEARN     = (VDA_ID_SIZE+5),              /* Datagram with LEARN data */
        RFTX_PAYLOAD_SIZE_CONF      = (VDA_ID_SIZE+8),              /* @@@ Confirmation response for LF command */
        RFTX_PAYLOAD_SIZE_VERSION   = (VDA_ID_SIZE+8),              /* Datagram with versions data: application version and library version */
        RFTX_PAYLOAD_SIZE_MEMORY    = (VDA_ID_SIZE+2+4)             /* Datagram with nvRAM data: addr word and 4 data bytes */
    }   RFTx_payload_length         : 8;    /* 8-bit length of payload of the RFTx datagram */
} rftx_datagram_property_t;

/* TIN CODES Example: CP5H AFRP W */
static const uint8_t TIN_CODES[VDA_DATA_SIZE] = {0x23, 0xC1, 0x5A, 0x21, 0x9B, 0x2C, 0x37, 0x00};
/* TIN DATE Example: Week 23 = 0x17,Year 2013 = 13 = 0x0D */
static const uint8_t TIN_DATE[VDA_DATA_SIZE]  = {0x17, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
/* Example for Tire: 255/30 R19 91Y XL M+S, run flat tire, fixed rolling direction, symmetrical */
static const uint8_t TIRE_DATA[VDA_DATA_SIZE] = {0x8B, 0xB9, 0x67, 0xFB, 0x79, 0x94, 0x04, 0x00};
/* Example for TIRE MILEAGE data: */
#define TIRE_MILEAGE                (123u)  /* artificially fixed at this value */
#define TIRE_MILEAGE_UNDERINFLATED  (45u)   /* artificially fixed at this value */

#define VDA_PRESSURE_INVALID        (0x00u) /* VDA code for invalid pressure */
#define VDA_PRESSURE_UNDERFLOW      (0x01u) /* VDA code for pressure below 100 kPa */
#define VDA_PRESSURE_OVERFLOW       (0xFFu) /* VDA code for pressure above 730 kPa */

/* VDA standard uses the next offset to convert the negative temperature range to positive scale */
#define VDA_TEMPERATURE_OFFSET      (52u) 
#define VDA_TEMPERATURE_INVALID     (0x00u) /* VDA code for invalid temperatute */
#define VDA_TEMPERATURE_UNDERFLOW   (0x01u) /* VDA code for temperature below -40  degC */
#define VDA_TEMPERATURE_OVERFLOW    (0xF8u) /* VDA code for temperature above +125 degC */

#define RFTX_VOLTAGE_INVALID        (0xFFu) /* Code to report via RF about invalid voltage */
#define RFTX_VOLTAGE_UNDERFLOW      (0xF0u) /* Code to report via RF about voltage below 1.8 Volt */
#define RFTX_VOLTAGE_OVERFLOW       (0xF8u) /* Code to report via RF about voltage above 3.6 Volt */

/* Code to report via RF about invalid acceleration: */
#define ACCELERATION_INVALID        (0x8000u)

/* The constant below specifies a number of RFTx datagrams assigned for the LF response of the LFCMD_WR_NVRAM command */
#define FRAMES_NUMBER_FOR_WR_NVRAM  (4u)    /* Should be in a range from 0 till 7 */

/* ----------------------------------------------------------------------------
 * Data describing LFRx communication */
 
/* NOTE: The LFRx Wake-Up ID (Header) below is specified as a 16-bit word with transposed (!) bytes */
#define LFRx_VDA_WAKEUP_ID          (0x7F25u)

#define LFRx_FIFO_SIZE              (8u)    /* Fixed by MLX91803/4/5 design! */

#define LFRx_VDA_MIN_PAYLOAD_SIZE   (3u)    /* SID < 0xF0, LID and CRC => 3 bytes */  

/* When active_id is received with the next value, the LF command LFCMD_JUMP demands SW reset */
/* This value has to be > NUMBER_OF_TPMS_STATES to avoid overlapping with TPMS states numbers */
#define LFRx_ACTION_RESET           (0xFFu)

/* ---- Structure of the LF commands ------------------------------------- */

typedef union {
    uint8_t in_bytes[LFRx_FIFO_SIZE];       /* All possible LFRx bytes */

    struct __attribute__ ((packed)) {       /* Structure of all supported LF commands */
        enum {                              /* Command groups: */
            LFRx_VDA_PT_SID  = 0x1E,        /* Mandatory VDA specified LF command LFCMD_PT */
            LFRx_VDA_APP_SID = 0xC0         /* Application specific VDA compatible extention LF commands */
        }       service_id          : 8;    /* Byte 0 => VDA Service Identifier (SID) */
        enum {                              /* Command types: */
            LFCMD_PT        = 0x01,         /* Generate standard VDA RF datagram with Pressure and Temperature */
            /* Move to the requested TPMS state or generate SW reset after RFTx confirmation datagram */
            LFCMD_JUMP      = (0x0 << 4) | VDA_SUPPLIER_ID,
            /* Transmit RFTx datagram(s) with a didicated RF format (includes nvRAM reading) */
            LFCMD_RFTX      = (0x1 << 4) | VDA_SUPPLIER_ID,
            /* Provoke CW carrier frequency (no RFTx confirmation datagram) */
            LFCMD_CW        = (0x2 << 4) | VDA_SUPPLIER_ID,
            /* Write word into the nvRAM with the next RFTx confirmation */
            LFCMD_WR_NVRAM  = (0x3 << 4) | VDA_SUPPLIER_ID
        }       local_id            : 8;    /* Byte 1 => VDA Local Identifier (LID) if SID < 0xF0 */
        union __attribute__ ((packed)) {
            uint8_t crc8_pt         : 8;    /* Byte 2 (LFCMD_PT) => CRC8 Baicheva poly: x8+x5+x3+x2+x+1 with preload 0xAA */
            uint8_t wut_exp         : 8;    /* Byte 2 (other commands) => Bits [3:0] are used to configure WUPTMR_EXP */
        };
        uint8_t wut_mant            : 8;    /* Byte 3 => all 8 bits are used to configure WUPTMR_MANT */
        union {
            struct __attribute__ ((packed)) { /* Structure for LF command LFCMD_JUMP */
                uint8_t action_id   : 8;    /* Byte 4 => @@@ tpms_state_t & LFRx_ACTION_RESET */
                uint8_t crc8_jmp    : 8;    /* Byte 5 => CRC8 Baicheva poly: x8+x5+x3+x2+x+1 with preload 0xAA */
                uint8_t             : 8;    /* Byte 6 => not used */
                uint8_t             : 8;    /* Byte 7 => not used */
            };
            struct __attribute__ ((packed)) {   /* Structure for LF command LFCMD_RFTX */
                rftx_t  rftx_id     : 8;    /* Byte 4 => @@@ RFTx format ID */
                uint8_t frames_number : 8;  /* Byte 5 => Number of datagrams to be sent */
                uint8_t crc8_rftx   : 8;    /* Byte 6 => CRC8 Baicheva poly: x8+x5+x3+x2+x+1 with preload 0xAA */
                uint8_t             : 8;    /* Byte 7 => not used */
            };
            struct __attribute__ ((packed)) { /* Structure for LF command LFCMD_CW */
                enum {                      /* All modes of the RF CW frequency generation: */
                    CW_FREQUENCY_FSK_0,     /* Low  FSK frequency */
                    CW_FREQUENCY_FSK_1,     /* High FSK frequency */
                    CW_FREQUENCY_OOK_1,     /* Central RF carrier frequency */
                    CW_FREQUENCY_FSK_MEANDER,/* Meander to be sent with FSK */
                    CW_FREQUENCY_OOK_MEANDER,/* Meander to be sent with OOK */
                    NUMBER_OF_CW_MODES      /* This value should be located always at the end of this list */
                    /* The values from NUMBER_OF_CW_MODES till 255 are not used */
                }       cw_type     : 8;    /* Byte 4 => Action Identifier to specify parameter of LF command */
                uint8_t crc8_cw     : 8;    /* Byte 5 => CRC8 Baicheva poly: x8+x5+x3+x2+x+1 with preload 0xAA */
                uint8_t             : 8;    /* Byte 6 => not used */
                uint8_t             : 8;    /* Byte 7 => not used */
            };
            struct __attribute__ ((packed)) { /* Structure for LF command LFCMD_WR_NVRAM */
                uint8_t nvram_addr  : 8;    /* Byte 4 => word address in nvRAM from the range [0...0x3E] */
                uint8_t high_byte   : 8;    /* Byte 5 => High byte of the word to be written into the nvRAM */
                uint8_t low_byte    : 8;    /* Byte 6 => Low  byte of the word to be written into the nvRAM */
                uint8_t crc8_wr     : 8;    /* Byte 7 => CRC8 Baicheva poly: x8+x5+x3+x2+x+1 with preload 0xAA */
            };                              /* LFRx_FIFO_SIZE */
        };
    };

} lfcmd_t;

ASSERT(sizeof(lfcmd_t) == LFRx_FIFO_SIZE);

/* ----------------------------------------------------------------------------
 * Data structures of the TPMS states and their properties
 */

typedef enum {                  /* The high level system states: */
    OFF,                        /* Standby TPMS mode (waiting for LF activation or for a pressure change) */
    STATIONARY,                 /* Confirmed stationary / parking state (not just a short stop) */
    NORMAL_DRIVE,               /* Confirmed driving state */
    ALERT_AT_STATIONARY,        /* Fast pressure change at stationary / parking (can be filling assist) */
    ALERT_AT_NORMAL_DRIVE,      /* Fast pressure change at driving (can be pressure drop due to tire damage) */
    LEARN,                      /* Learn Auto-Location procedure */
    /* All Factory modes are to simulate the real application modes */
    FACTORY_STATIONARY,         /* Artificial STATIONARY needed for the factory tests */
    FACTORY_NORMAL_DRIVE,       /* Artificial DRIVING needed for the factory tests */
    FACTORY_ALERT_AT_STATIONARY,/* Artificial ALERT_AT_STATIONARY needed for the factory tests */
    FACTORY_ALERT_AT_DRIVE,     /* Artificial ALERT_AT_NORMAL_DRIVE needed for the factory tests */
    FACTORY_LEARN,              /* Artificial LEARN mode is needed for the factory tests */
    DEMO,                       /* Melexis TPMS Demo mode */
    DISABLED,                   /* Special state of fully disabled TPMS that is only waiting for LF activation */
    NUMBER_OF_TPMS_STATES       /* The dummy state that should be always located at the end of state list */
    /* TPMS states values from 13 till 15 are reserved */
} tpms_state_t;

typedef struct __attribute__ ((packed)) {
    uint8_t     WUT_main_period_mant        : 8;    /* 8-bit mantissa (max 255) */
    uint8_t     WUT_main_period_exp         : 4;    /* 4-bit exponent (max 12)  */
    uint8_t     LFRx_enabled                : 1;    /* Single bit: "1" enables LFRx reception */
    uint8_t     LFO_calibration_needed      : 1;    /* Single bit: "1" enables LFO calibration */
    uint8_t     HFO_calibration_needed      : 1;    /* Single bit: "1" enables HFO calibration */
    uint8_t                                 : 1;    /* reserved bit field */
    uint8_t     Measurement_period_in_STT   : 8;    /* 8-bit number of STT periods between PVT measurements */
    uint8_t     MotionDetect_period_in_STT  : 8;    /* 8-bit number of STT periods between Motion Detections */
    uint16_t    RFTx_reports_period_in_STT  : 16;   /* 16-bit number of STT periods between RFTx reports */
    rftx_t      RFTx_datagram_format_id     : 8;    /* 8-bit ID of the RFTx format in the list "RFTX_DATAGRAMS" */
    uint8_t     RFTx_datagrams_per_report   : 8;    /* 8-bit number of RFTx datagrams per RFTx report */
} tpms_state_property_t;

/* ----------------------------------------------------------------------------
 * Structure to describe all application states
 */

typedef union {
    struct __attribute__((packed)) StateAsPort_s {
        uint8_t     System_State;
        uint8_t     Motion_State;
    } as_port;

    struct __attribute__((packed))  StateAsField_s {
        tpms_state_t tpms_state         : 4;    /* Byte 0 */
        enum { /* Intermediate states are needed to implement some actions that require sleep modes to optimize power consumption */
            NO_ACTION,                  /* It's a normal state when no special action is requested */
            TRANSMITTING,               /* Generating a multiply datagrams of the RF report or response to LF command */
            THERMAL_SHUTDOWN,           /* Keeping device in the Thermal Shutdown mode (not implemented @@@) */
            RESETTING                   /* Waiting for a delay before the requested SW reset (the delay is needed to support LF bootloader) */
            /* Action states values from 3 till 15 are reserved */
        }   action_state                : 4;    /* Byte 0 */

        enum {
            R0, /* not defined */
            R1, /* around zero, TL < ACAM < TH */
            R2, /* above zero,  TH < ACAM < T0H */
            R3, /* low speed range with confident ACAM, T0H < ACAM < TLP */
            R4, /* high speed range where LPAM is reliable, ACAM/LPAM > TLP*/
            R5, /* optional acceleration range that exist when Error_Margin is not zero, TERR < ACAM < TL */
            R6, /* error range, ACAM < TERR */
            R7  /* reserved */
        }   acceleration_range          : 3;    /* Byte 1 */
        enum {
            STOPPED,                    /* Wheel is not rotating */
            ROTATING                    /* Wheel is rotating */
        }   rotation                    : 1;    /* Byte 1 */
        uint8_t     Reserved_spmem_bits : 4;    /* Byte 1 */
    } as_field;

} State_t;

ASSERT (sizeof(State_t) == 2u);
/* To ensure the same size for both union structures: */
ASSERT (sizeof(struct StateAsPort_s) == sizeof(struct StateAsField_s));

/* ----------------------------------------------------------------------------
 * Parameters of the Motion Detection based on the direct acceleration measurements */

#define LOW_CENTRIPETAL_SIGNAL_THRESHOLD    (6) /* expressed in [g] */
#define HIGH_CENTRIPETAL_SIGNAL_THRESHOLD   (7) /* expressed in [g] */

/* ----------------------------------------------------------------------------
 * Scratchpad data
 */

extern SP_MEM uint16_t SP_random_base; /* SP_SP_random_base and SP_SP_rand_lfsr */
extern SP_MEM uint16_t SP_rand_lfsr;   /* are used to calculate a random delay */

extern SP_MEM State_t SP_state;

extern SP_MEM uint8_t  SP_temperature_at_calibration;
extern SP_MEM uint8_t  SP_RFTx_Message_counter;

extern SP_MEM uint16_t SP_last_measured_pressure;   /*@@@ to be optimized till size uint8_t */
extern SP_MEM uint16_t SP_last_measured_voltage;    /*@@@ to be optimized till size uint8_t */
extern SP_MEM uint8_t  SP_last_measured_temperature;

/* ----------------------------------------------------------------------------
 * Global variables in RAM
 */

extern DP_MEM int16_t  Acceleration;
extern DP_MEM uint16_t Pressure;
extern DP_MEM uint16_t Voltage;
extern DP_MEM uint8_t  Temperature;

extern DP_MEM lfcmd_t LFRx_payload;

extern DP_MEM uint8_t  RFTx_Datagram_Counter;

extern DP_MEM bool PVT_measurements_valid;
extern DP_MEM bool Acceleration_measurements_valid;

/* ----------------------------------------------------------------------------
 */
#endif /* DATATYPES_H */
