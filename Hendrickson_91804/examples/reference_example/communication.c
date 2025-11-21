/*
 * Copyright (C) 2018-2022 Melexis N.V.
 *
 * Software Platform
 *
 */

#include <sensorlib.h>
#include <io.h>
#include <debug.h>

#include <datatypes.h>
#include <processing.h>
#include <measurements.h>

STATIC INLINE void RFTx_Calibrate_if_needed(void);
STATIC void WakeupTimer_Random_Init(void);

void LF_Message_processing(void) __attribute__ ((noreturn));
void Start_RFTx_reporting(rftx_t rftx_format_id) __attribute__ ((noreturn));
void RFTx_reporting(void) __attribute__ ((noreturn));
void RFTx_Fifo_filling(void);

STATIC INLINE void RFTx_payload_preparation(rftx_t rftx_format_id);
STATIC INLINE uint8_t CalcCRC8(uint8_t const *data, uint8_t len);

STATIC uint8_t Get_RFTx_pressure(void);
STATIC uint8_t Get_RFTx_temperature(void);
STATIC uint8_t Get_RFTx_voltage(void);
STATIC uint8_t Get_RFTx_status(void);

DP_MEM uint8_t RFTx_Datagram_Counter;

DP_MEM lfcmd_t LFRx_payload;

DP_MEM bool RFTx_delay_is_random;

STATIC DP_MEM uint8_t RFTx_payload_data[RFTX_MAXIMAL_PAYLOAD_SIZE];

STATIC NVRAM_MEM uint16_t nvRAM_in_words[32]; /* @@@ */

/* ----------------------------------------------------------------------------
 * RFTx datagrams' types
 */

RODATA_MEM rftx_datagram_property_t RFTX_DATAGRAM_PARAMETERS[NUMBER_OF_RFTX_FORMATS] = {
    {   /* [0] -> RFTX_PT_REPORT_W: Basic VDA report with just Pressure and Temperature in working mode */
        .RFTx_VDA_packet_ID     = VDA_PACKET_ID_PT_REPORT_W,
        .RFTx_payload_length    = RFTX_PAYLOAD_SIZE_PT
    },
    {   /* [1] -> RFTX_PT_RESPONSE: Response on the basic LF command with just Pressure and Temperature */
        .RFTx_VDA_packet_ID     = VDA_PACKET_ID_PT_RESPONSE,
        .RFTx_payload_length    = RFTX_PAYLOAD_SIZE_PT
    },
    {   /* [2] -> RFTX_PT_REPORT_L: Basic VDA report with just Pressure and Temperature in learning mode */
        .RFTx_VDA_packet_ID     = VDA_PACKET_ID_PT_REPORT_L,
        .RFTx_payload_length    = RFTX_PAYLOAD_SIZE_PT
    },
    {   /* [3] -> RFTX_TIN_CODES: VDA report with TIN CODES */
        .RFTx_VDA_packet_ID     = VDA_PACKET_ID_TIN_CODES,
        .RFTx_payload_length    = RFTX_PAYLOAD_SIZE_TIN
    },
    {   /* [4] -> RFTX_TIN_DATE: VDA report with TIN DATE */
        .RFTx_VDA_packet_ID     = VDA_PACKET_ID_TIN_DATE,
        .RFTx_payload_length    = RFTX_PAYLOAD_SIZE_TIN
    },
    {   /* [5] -> RFTX_TIRE_DATA: VDA report with information about the TIRE DIMENSION and TYPE */
        .RFTx_VDA_packet_ID     = VDA_PACKET_ID_TIRE_DATA,
        .RFTx_payload_length    = RFTX_PAYLOAD_SIZE_TARE
    },
    {   /* [6] -> RFTX_MILEAGE: VDA report with MILEAGE and MILEAGE UNDERINFLATED information */
        .RFTx_VDA_packet_ID     = VDA_PACKET_ID_MILEAGE,
        .RFTx_payload_length    = RFTX_PAYLOAD_SIZE_MILEAGE
    },
    {   /* [7] -> RFTX_DEMO: Demo specific report */
        .RFTx_VDA_packet_ID     = VDA_PACKET_ID_DEMO,
        .RFTx_payload_length    = RFTX_PAYLOAD_SIZE_DEMO
    },
    {   /* [8] -> RFTX_PVT_REPORT: Application specific report with PVT data */
        .RFTx_VDA_packet_ID     = VDA_PACKET_ID_PVT_REPORT,
        .RFTx_payload_length    = RFTX_PAYLOAD_SIZE_PVT
    },
    {   /* [9] -> RFTX_LEARN_REPORT: Application specific report with LEARN data */
        .RFTx_VDA_packet_ID     = VDA_PACKET_ID_LEARN_REPORT,
        .RFTx_payload_length    = RFTX_PAYLOAD_SIZE_LEARN
    },
    {   /* [10] -> RFTX_CONF_RESPONSE: Application specific LF command confirmation response */
        .RFTx_VDA_packet_ID     = VDA_PACKET_ID_CONF_RESPONSE,
        .RFTx_payload_length    = RFTX_PAYLOAD_SIZE_CONF
    },
    {   /* [11] -> RFTX_VERSION_DATA: Application specific report with Version data */
        .RFTx_VDA_packet_ID     = VDA_PACKET_ID_VERSION_DATA,
        .RFTx_payload_length    = RFTX_PAYLOAD_SIZE_VERSION
    },
    {   /* [12] -> RFTX_MEMORY_DATA: Application specific report with nvRAM data */
        .RFTx_VDA_packet_ID     = VDA_PACKET_ID_MEMORY_DATA,
        .RFTx_payload_length    = RFTX_PAYLOAD_SIZE_MEMORY
    }
};

/* ----------------------------------------------------------------------------
 * LF command parsing
 * Arguments:   None
 * Returns:     None
 * Notes:
 */
void LF_Message_processing(void)
{
    uint8_t payload_size = LfRx_FifoCount();
    if ( (payload_size >= LFRx_VDA_MIN_PAYLOAD_SIZE) && (!LfRx_IsFifoOverrun()) )
    {   /* LFRx FIFO is not overrun and includes enough data to parse */
        for (uint16_t loop_index = 0u; loop_index < payload_size; loop_index++)
        {   /* Move data from the LFRx FIFO to the LFRx_payload buffer */
            LFRx_payload.in_bytes[loop_index] = LfRx_FifoReadByte();
        }
        uint8_t crc8 = CalcCRC8(LFRx_payload.in_bytes, (payload_size - 1u));
	    if (crc8 == LFRx_payload.in_bytes[payload_size - 1])
	    {   /* CRC of received payload is correct */
		    if ((LFRx_payload.service_id == LFRx_VDA_PT_SID) && (LFRx_payload.local_id == LFCMD_PT))
            {   /* Correct VDA LF command LFCMD_PT is received - send RFTx PT response */
                Measure_PVT();              /* Provide a fresh PVT data */
                RFTx_Datagram_Counter = 1u; /* Assign a single datagram */
                RFTx_delay_is_random = false;
                Start_RFTx_reporting(RFTX_PT_RESPONSE); /* No return */
	        }
	        else if (LFRx_payload.service_id == LFRx_VDA_APP_SID)
            {   /* The received payload has a correct SID and CRC */
                WakeupTimer_Restart((WakeupTimerPeriod_Type){.mant     = LFRx_payload.wut_mant,
                                                             .exponent = (LFRx_payload.wut_exp & 0x0Fu)});
                if (LFRx_payload.local_id == LFCMD_JUMP)
                {   /* Move to the requested TPMS state or generate SW reset after RFTx confirmation datagram */
                    if (LFRx_payload.action_id == LFRx_ACTION_RESET)
                    {   /* Prepare the requested SW reset: */
	                    SP_state.as_field.action_state = RESETTING;
	                    Wakeup_Disable(WAKEUP_LF_DONE);
	                    Sys_DeepSleepWith(LF_DISABLED);
                    }
                    else if (LFRx_payload.action_id < NUMBER_OF_TPMS_STATES)
                    {   /* Initialize a new TPMS state */
                        TPMS_state_Init((tpms_state_t)LFRx_payload.action_id);
                    }
                    else { /* Ignore the received data because of a wrong action_id */ }
                }
                else if (LFRx_payload.local_id == LFCMD_RFTX)
                {   /* Transmit RFTx datagram(s) with a didicated RF format (includes nvRAM reading) */
                    Measure_PVT();  /* Some datagrams require a fresh PVT data */
                    if (LFRx_payload.rftx_id < NUMBER_OF_RFTX_FORMATS)
                    {   /* rftx_id is correct and points to the RF format from the RFTX_DATAGRAM_PARAMETERS list */
                        RFTx_delay_is_random = false;                   /* With fixed delay between datagrams */
                        /* Note: If (LFRx_payload.rftx_id == VDA_PACKET_ID_MEMORY_DATA)
                         * only 3 lowest bits of the RFTx_Datagram_Counter define the datagrams number,
                         * while 5 highest bits of the RFTx_Datagram_Counter are used to specify
                         * an address of 4-byte portion inside of the 128-byte nvRAM */
                        RFTx_Datagram_Counter = LFRx_payload.frames_number;
                        Start_RFTx_reporting(LFRx_payload.rftx_id);     /* No return */
                    }
                    else { /* Ignore the received data because of a wrong rftx_id */ }
                }
                else if (LFRx_payload.local_id == LFCMD_CW)
                {   /* Provoke CW carrier frequency (no RFTx confirmation datagram) */
                    if (LFRx_payload.cw_type < NUMBER_OF_CW_MODES)
                    {
                        IO_SET(CUSTOM_91804, RFTX_DIRECT_MOD, 0u);      /* Modulation source 0 means a fixed logic 0 */
                        if (LFRx_payload.cw_type == CW_FREQUENCY_FSK_0)
                        {   /* Low FSK frequency: Set FSK, no inversion of NRZ zero data */
                            IO_SET(CUSTOM_91804, RFTX_EN_MANCHESTER, 0u);   /* NRZ data coding */
                            IO_SET(CUSTOM_91804, RFTX_BIT_INVERT, 0u,
                                                 RFTX_FSK_NOOK,   1u);
                        }
                        else if (LFRx_payload.cw_type == CW_FREQUENCY_FSK_1)
                        {   /* High FSK frequency: Set FSK with inversion of NRZ zero data */
                            IO_SET(CUSTOM_91804, RFTX_EN_MANCHESTER, 0u);   /* NRZ data coding */
                            IO_SET(CUSTOM_91804, RFTX_BIT_INVERT, 1u,
                                                 RFTX_FSK_NOOK,   1u);
                        }
                        else if (LFRx_payload.cw_type == CW_FREQUENCY_OOK_1)
                        {   /* Central RF carrier frequency: Set OOK with inversion of NRZ zero data */
                            IO_SET(CUSTOM_91804, RFTX_EN_MANCHESTER, 0u);   /* NRZ data coding */
                            IO_SET(CUSTOM_91804, RFTX_BIT_INVERT, 1u,
                                                 RFTX_FSK_NOOK,   0u);
                        }
                        else if (LFRx_payload.cw_type == CW_FREQUENCY_FSK_MEANDER)
                        {   /* Meander to be sent with FSK: Set FSK of zero data coded in Manchester */
                            IO_SET(CUSTOM_91804, RFTX_EN_MANCHESTER, 1u);   /* Manchester data coding */
                            IO_SET(CUSTOM_91804, RFTX_FSK_NOOK, 1u);
                        }
                        else
                        {   /* Meander to be sent with OOK: Set OOK of zero data coded in Manchester */
                            IO_SET(CUSTOM_91804, RFTX_EN_MANCHESTER, 1u);   /* Manchester data coding */
                            IO_SET(CUSTOM_91804, RFTX_FSK_NOOK, 0u);
                        }
                        Itc_Enable(ITC_WUPTMR); /* Enable WUPTMR interrupt to finalize the RF carrier generation */
                        RFTx_Calibrate_if_needed();
                        RfTx_Run();
                        Sys_DeepSleepWith(LF_DISABLED); /* no return */
                    }
                    else { /* Ignore the received data because of a wrong cw_type */ }
                }
                else if (LFRx_payload.local_id == LFCMD_WR_NVRAM)
                {   /* Write word into the nvRAM with the next RFTx confirmation */
                    if ((LFRx_payload.nvram_addr & 0x3E) == LFRx_payload.nvram_addr)
                    {   /* Received address matches the internal address of the user's part of the nvRAM */
                        /* To avoid writing a random data into the nvRAM
                         * NvRam_Recall was executed before calling LF_Message_processing */
                        nvRAM_in_words[LFRx_payload.nvram_addr >> 1] =
                            ((uint16_t)LFRx_payload.high_byte << 8) +
                             (uint16_t)LFRx_payload.low_byte;
                        NvRam_Store();
                        RFTx_delay_is_random = false;
                        /* Find an internal nvRAM address of 4-byte portion covering the written word (will be in a range 0...15) */
                        uint8_t portion_address = LFRx_payload.nvram_addr >> 2;
                        /* For RFTX_MEMORY_DATA datagrams 5 highest bits of the RFTx_Datagram_Counter
                         * are used to specify an address of 4-byte portion inside of the nvRAM */
                        RFTx_Datagram_Counter = (FRAMES_NUMBER_FOR_WR_NVRAM & 0x07u) /* Number of datagrams in 3 lowest bits */
                                              | (portion_address << 3); /* and address of 4-byte portion in 5 highest bits */
                        Start_RFTx_reporting(RFTX_MEMORY_DATA); /* No return */
                    }
                    else { /* Ignore the received data because nvram_addr is out of range */ }
                }
                else { /* Ignore the received data because of a wrong LID value */ }
            }
            else { /* Ignore the received data because of a wrong SID or SID&LID value */ }
        }
        else
        {   /* Ignore the received data because of a wrong CRC */
            DEBUG_PUT_STRING("CRC=");           /* In case of crc mismatch */
            DEBUG_PUT_WORD_HEX((uint16_t)crc8); /* print out the expected CRC value in debug mode */
        }
    }
    else { /* Ignore the received data because its size is too small or LFRx FIFO is overrun */ }
    LfRx_FifoFlush();
    Sys_DeepSleepWith(LF_RECEIVE);
    while(true) { /* This dummy cycle ensures for compiler there is no return */
        /* Actually this cycle is not needed because a sleep mode is set before */
    }
}  /* LF_Message_processing */

/* ----------------------------------------------------------------------------
 * RFTx payload preparation
 * Arguments:	rftx_format_id specifying type of RFTx datagram from the list RFTX_DATAGRAM_PARAMETERS
 * Returns:     none
 * Notes:       Can be executed after NvRam_Recall() only!
 */
STATIC INLINE void RFTx_payload_preparation(rftx_t rftx_format_id)
{
    uint16_t low_chipID;
    uint16_t high_chipID;
    uint16_t vda_packet_id;
    uint16_t payload_length;
    uint16_t data16;
    uint8_t  tmp;

    /* Set a corresponding payload length: */
    payload_length = RFTX_DATAGRAM_PARAMETERS[rftx_format_id].RFTx_payload_length;
    IO_SET(CUSTOM_91804, RFTX_PACKET_LEN, payload_length);
    /* Set a corresponding VDA packet ID: */
    vda_packet_id  = RFTX_DATAGRAM_PARAMETERS[rftx_format_id].RFTx_VDA_packet_ID;
    RFTx_payload_data[0] = vda_packet_id;

    /* Read Chip ID factory stored in the NVRAM, LSB first */
    low_chipID  = Sys_GetChipId().as_word[0];
    high_chipID = Sys_GetChipId().as_word[1];
    RFTx_payload_data[1] = (VDA_SUPPLIER_ID << 4) | ((uint8_t)(high_chipID >> 8) & 0x0Fu);  /* MSB */
    RFTx_payload_data[2] = (uint8_t) high_chipID;
    RFTx_payload_data[3] = (uint8_t)(low_chipID >> 8);
    RFTx_payload_data[4] = (uint8_t) low_chipID;                                            /* LSB */

    switch (rftx_format_id)
    {
        case 0: /* VDA_PACKET_ID_PT_REPORT_W */
        case 1: /* VDA_PACKET_ID_PT_REPORT_L */
        case 2: /* VDA_PACKET_ID_PT_RESPONSE */
            RFTx_payload_data[5] = Get_RFTx_pressure();
            RFTx_payload_data[6] = Get_RFTx_temperature();
            break;

        case 3: /* VDA_PACKET_ID_TIN_CODES */
            for (tmp = 0u; tmp < VDA_DATA_SIZE; tmp++) {
                RFTx_payload_data[tmp+5] = TIN_CODES[tmp];
            }
            break;

        case 4: /* VDA_PACKET_ID_TIN_DATE */
            for (tmp = 0u; tmp < VDA_DATA_SIZE; tmp++) {
                RFTx_payload_data[tmp+5] = TIN_DATE[tmp];
            }
            break;

        case 5: /* VDA_PACKET_ID_TIRE_DATA */
            for (tmp = 0u; tmp < VDA_DATA_SIZE; tmp++) {
                RFTx_payload_data[tmp+5] = TIRE_DATA[tmp];
            }
            break;

        case 6: /* VDA_PACKET_ID_MILEAGE */
            RFTx_payload_data[5] = TIRE_MILEAGE;
            RFTx_payload_data[6] = TIRE_MILEAGE_UNDERINFLATED;
            for (tmp = 7u; tmp < payload_length; tmp++) {
                RFTx_payload_data[tmp] = 0x00u;  /* Reserved in VDA standard */
            }
            break;

        case 7: /* VDA_PACKET_ID_DEMO */
            RFTx_payload_data[5] = Get_RFTx_pressure();
            RFTx_payload_data[6] = Get_RFTx_temperature();
            RFTx_payload_data[7] = Get_RFTx_voltage();
            {   /* add acceleration measurements to the RFTx report: */
                int16_t acceleration_x = Compensated_Acceleration_Measurement_X();
                int16_t acceleration_z = Compensated_Acceleration_Measurement_Z();
                RFTx_payload_data[8]  = (uint8_t)(acceleration_x >> 8);
                RFTx_payload_data[9]  = (uint8_t)acceleration_x;
                RFTx_payload_data[10] = (uint8_t)(acceleration_z >> 8);
                RFTx_payload_data[11] = (uint8_t)acceleration_z;
            }
            RFTx_payload_data[12] = SP_RFTx_Message_counter;
            break;

        case 8: /* VDA_PACKET_ID_PVT_REPORT */
            RFTx_payload_data[5] = Get_RFTx_pressure();
            RFTx_payload_data[6] = Get_RFTx_temperature();
            RFTx_payload_data[7] = Get_RFTx_voltage();
            RFTx_payload_data[8] = Get_RFTx_status();
            break;

        case 9: /* VDA_PACKET_ID_LEARN_REPORT */
            RFTx_payload_data[5] = Get_RFTx_pressure();
            RFTx_payload_data[6] = Get_RFTx_temperature();
            /* The next 3 bytes represent ALOC specific data
            that has to be updated according to the customer's ALOC implementation */
            RFTx_payload_data[7] = 0x01u;   /* temporarily fixed */
            RFTx_payload_data[8] = 0x02u;   /* temporarily fixed */
            RFTx_payload_data[9] = 0x03u;   /* temporarily fixed */
            break;

        case 10: /* VDA_PACKET_ID_CONF_RESPONSE */
            for (tmp = 0u; tmp < LFRx_FIFO_SIZE; tmp++) {
                RFTx_payload_data[tmp+5] = LFRx_payload.in_bytes[tmp];
            }
            break;

        case 11: /* VDA_PACKET_ID_VERSION_DATA */
            RFTx_payload_data[5]  = (uint8_t)(LIB_GIT_HASH >> 24);
            RFTx_payload_data[6]  = (uint8_t)(LIB_GIT_HASH >> 16);
            RFTx_payload_data[7]  = (uint8_t)(LIB_GIT_HASH >> 8);
            RFTx_payload_data[8]  = (uint8_t)(LIB_GIT_HASH >> 0);
            RFTx_payload_data[9]  = LIB_VERSION_MAJOR;
            RFTx_payload_data[10] = LIB_VERSION_MINOR;
            RFTx_payload_data[11] = LIB_VERSION_PATCH;
            RFTx_payload_data[12] = APP_VERSION;
            break;

        case 12: /* VDA_PACKET_ID_MEMORY_DATA (can be provoked by LF command only */
            NvRam_Recall();                 /* To ensure always a fresh read data */
            RFTx_payload_data[5] = 0x10u;   /* Memory_address_high */
            /* For RFTX_MEMORY_DATA datagrams 5 highest bits of the RFTx_Datagram_Counter
             * are used to specify an address of 4-byte portion inside of the 128-byte nvRAM */
            RFTx_payload_data[6] = (RFTx_Datagram_Counter >> 1) & 0x7Cu;/* Memory_address_low */
            /* Then rectify the RFTx_Datagram_Counter from the address in the highest nibble: */
            RFTx_Datagram_Counter &= 0x07u;
            tmp = RFTx_payload_data[6] >> 1;
	        data16 = nvRAM_in_words[tmp];
            RFTx_payload_data[7]  = (uint8_t)data16;
            RFTx_payload_data[8]  = (uint8_t)(data16 >> 8);
            tmp += 1u;
	        data16 = nvRAM_in_words[tmp];
            RFTx_payload_data[9]  = (uint8_t)data16;
            RFTx_payload_data[10] = (uint8_t)(data16 >> 8);
            break;

        default:
            Sys_ResetCpu(); /* This case should never happen! */
            break;
    }  /* END of switch (rftx_format_id) */
} /* RFTx_payload_preparation */

/* ----------------------------------------------------------------------------
 * Convert pressure value to RFTx VDA format
 * Arguments:	none
 * Returns:     unsigned 8-bit pressure value in VDA RFTx format
 * Notes:
 */
STATIC uint8_t Get_RFTx_pressure(void)
{
    uint8_t vda_pressure;

    if (PVT_measurements_valid == true)
    {   /* Pressure is valid -> convert it to the RFTx VDA format */
        if (Pressure < 100u)
        {   /* If Pressure is below 100 kPa */
            vda_pressure = VDA_PRESSURE_UNDERFLOW;
        }
        else if (Pressure > 730u)
        {   /* If Pressure is above 730 kPa */
            vda_pressure = VDA_PRESSURE_OVERFLOW;
        }
        else
        {   /* vda_pressure equals to (Pressure - 100) divided by 2.5 plus 2 kPa */
            Pressure = ((Pressure - 100u) * (10u * 2u)) / 25u;
            /* Rounding, removing odd LSB and adding 2 kPa to comply VDA standard: */
            if ((Pressure & 1u) == 0u)
            {
                vda_pressure = (uint8_t)(Pressure >> 1) + 2u;
            }
            else
            {
                vda_pressure = (uint8_t)(Pressure >> 1) + 2u + 1u;
            }
        }
    }
    else
    {   /* Use VDA_PRESSURE_INVALID because measured data is not valid */
        vda_pressure    = VDA_PRESSURE_INVALID;;
    }
    return vda_pressure;
} /* Get_RFTx_pressure */

/* ----------------------------------------------------------------------------
 * Convert temperature value to RFTx VDA format
 * Arguments:	none
 * Returns:     unsigned 8-bit temperature value in VDA RFTx format
 * Notes:
 */
STATIC uint8_t Get_RFTx_temperature(void)
{
    uint8_t vda_temperature;

    if (PVT_measurements_valid == true)
    {   /* Temperature is valid -> convert it to the RFTx VDA format */
        if (Temperature < (-40 + VDA_TEMPERATURE_OFFSET))
        {   /* If Temperature is below -40 degC */
            vda_temperature = VDA_TEMPERATURE_UNDERFLOW;
        }
        else if (Temperature > (125u + VDA_TEMPERATURE_OFFSET))
        {   /* If Temperature is above +125 degC */
            vda_temperature = VDA_TEMPERATURE_OVERFLOW;
        }
        else
        {   /* Temperature value is within the acceptable range */
            vda_temperature = Temperature;
        }
    }
    else
    {   /* Use VDA_TEMPERATURE_INVALID because measured data is not valid */
        vda_temperature = VDA_TEMPERATURE_INVALID;
    }
    return vda_temperature;
} /* Get_RFTx_temperature */

/* ----------------------------------------------------------------------------
 * Convert battery voltage value to 8-bit format suitable for RF report
 * Arguments:	none
 * Returns:     unsigned 8-bit voltage value to be reported
 * Notes:
 */
STATIC uint8_t Get_RFTx_voltage(void)
{
    uint8_t rftx_voltage;

    if (PVT_measurements_valid == true)
    {   /* Battery voltage is valid -> convert it to the RFTx format */
        if (Voltage <= 180u)
        {   /* If Voltage is below 1.8 Volt */
            rftx_voltage = RFTX_VOLTAGE_UNDERFLOW;
        }
        else if (Voltage > 360u)
        {   /* If Voltage is bigger 3.6 Volt */
            rftx_voltage = RFTX_VOLTAGE_OVERFLOW;
        }
        else
        {   /* Normal voltage range */
            rftx_voltage = (uint8_t)(Voltage - 180u);
        }   /* Battery voltage in [V] equals to "rftx_voltage"/100 plus 1.8V */
    }
    else
    {   /* Use RFTX_VOLTAGE_INVALID because measured data is not valid */
        rftx_voltage = RFTX_VOLTAGE_INVALID;
    }
    return rftx_voltage;
} /* Get_RFTx_voltage */

/* ----------------------------------------------------------------------------
 * Generate 8-bit value of status used in the RF reports
 * Arguments:	none
 * Returns:     unsigned 8-bit status
 * Notes:
 */
STATIC uint8_t Get_RFTx_status(void)
{
    /* Below application code has to define a real status value instead of the artificial 0x5A */
    uint8_t status = 0x5Au;
    return status;
} /* Get_RFTx_status */

/* ----------------------------------------------------------------------------
 * RFTx FIFO filling from the prepared RFTx_payload_data array
 * Arguments:	none
 * Returns:     none
 * Notes:
 */
void RFTx_Fifo_filling(void)
{
    uint8_t packet_len = IO_GET(CUSTOM_91804, RFTX_PACKET_LEN);
    RfTx_FifoFlush();
    /* Fill RFTx FIFO by RFTx_payload_data[]: */
    for (uint16_t cnt = 0u; cnt < packet_len; cnt++)
    {
        RfTx_FifoWriteByte(RFTx_payload_data[cnt]);
    }
} /* RFTx_Fifo_filling */

/* ----------------------------------------------------------------------------
 * Start sending RFTx datagrams of report or response to LF command
 * Arguments:	rftx_format_id specifies a type of RFTx datagram from the list RFTX_DATAGRAM_PARAMETERS
 * Returns:     none
 * Notes:
 */
void Start_RFTx_reporting(rftx_t rftx_format_id)
{
    RFTx_payload_preparation(rftx_format_id);
    RFTx_Fifo_filling();            /* Prepare FIFO with RFTx datagram data */
    RFTx_Calibrate_if_needed();
    RFTx_reporting();               /* no return */
} /* Start_RFTx_reporting */

/* ----------------------------------------------------------------------------
 * Sending RFTx datagrams of report
 * Arguments:	none
 * Returns:     none
 * Notes:
 */
void RFTx_reporting(void)
{
    DEBUG_PUT_WORD_HEX((uint16_t)RFTx_Datagram_Counter);
    DEBUG_PUT_STRING(".Tx ");
    if (RFTx_Datagram_Counter > 1u)
    {   /* Intermediate datagram of the message has to be sent with the next random delay */
        RfTx_Run();                 /* Transmit a datagram of the RF message */
        RFTx_Datagram_Counter -= 1u;/* Decrement datagram counter */
        if (RFTx_delay_is_random == true)
        {   /* Configure a random delay before the next RFTx datagram */
            WakeupTimer_Random_Init();
        }
        else
        { /* Fixed delay between RFTx datagrams based on the latest WUT configuration */ }
        SP_state.as_field.action_state = TRANSMITTING;
        /* Go to sleep mode with disabled LFRx
         * that is needed to complete the RFTx action having the highest priority
         * LF commands should not affect the RFTx action */
        DEBUG_PUT_STRING("Sleep ");
        Sys_SleepWith(LF_DISABLED); /* no return */
    }
    else if (RFTx_Datagram_Counter == 1u)
    {   /* The last or just a single datagram of the message has to be sent */
        RfTx_Run();                 /* Transmit a datagram of the RF message */
        SP_RFTx_Message_counter += 1u;
    }
    else
    { /* zero: just follow the branch below that is common
       * with the case when (RFTx_Datagram_Counter == 1u) */
    }
    SP_state.as_field.action_state = NO_ACTION;
    WakeupTimer_Restart((WakeupTimerPeriod_Type){   /* Confifure STT delay */
        .mant     = TPMS_STATE_PARAMETERS[SP_state.as_field.tpms_state].WUT_main_period_mant,
        .exponent = TPMS_STATE_PARAMETERS[SP_state.as_field.tpms_state].WUT_main_period_exp});
    if (TPMS_STATE_PARAMETERS[SP_state.as_field.tpms_state].LFRx_enabled == 1)
    {   /* Go to deep sleep with LFRx enabled */
        DEBUG_PUT_STRING("DSleepLF ");
        LfRx_FifoFlush();
        Sys_DeepSleepWith(LF_RECEIVE);  /* no return */
    }
    else
    {   /* Go to deep sleep with LFRx disabled */
        DEBUG_PUT_STRING("DSleep ");
        Sys_DeepSleepWith(LF_DISABLED); /* no return */
    }
    while(true) { /* This dummy cycle ensures for compiler there is no return */
        /* Actually this cycle is not needed because a sleep mode is set before */
    }
} /* RFTx_reporting */

/* ----------------------------------------------------------------------------
 * Implement RFTx calibration if it is needed just before RF transmission
 * Arguments:   none, based on 3 global variables:
 *              Temperature, SP_temperature_at_calibration and PVT_measurements_valid
 * Returns:     none, but can modify a global SP_MEM variable SP_temperature_at_calibration
 * Notes:
 */
STATIC INLINE void RFTx_Calibrate_if_needed(void)
{   /* For power saving the calibration should be done as close as possible to RFTx start */
    uint8_t difference;

    if (PVT_measurements_valid == false)
    {   /* Temperature is unknown (no or faulty measurements after bootstrap) */
        SP_temperature_at_calibration = UNKNOWN_TEMPERATURE;    /* calibration is needed and will be done */
    }
    else if (SP_temperature_at_calibration != UNKNOWN_TEMPERATURE)
    {   /* PVT measurements are valid -> compare a temperature change with threshold */
        if (SP_temperature_at_calibration > Temperature)
        {
            difference = SP_temperature_at_calibration - Temperature;
        }
        else
        {
            difference = Temperature - SP_temperature_at_calibration;
        }
        if (difference > CALIB_TEMPERATURE_THRESHOLD)
        {   /* RFTx calibration is needed */
            SP_temperature_at_calibration = Temperature;    /* Store the temperature with a new RFTx calibration */
        }
        else
        { /* RFTx calibration is not needed -> skip it by return */
            return;
        }
    }
    else
    {   /* Unknown SP_temperature_at_calibration, while the current PVT measurements are valid */
        SP_temperature_at_calibration = Temperature;
    }
    DEBUG_PUT_STRING("RF.calib ");
    (void)RfTx_Calibrate();
}  /* RFTx_Calibrate_if_needed */

/* ----------------------------------------------------------------------------
 * WakeupTimer_Random_Init
 * Arguments:   none
 * Returns:     none
 * Notes:
 */
STATIC void WakeupTimer_Random_Init(void)
{
    uint8_t WUP_mant;

    /* taps: 16 14 13 11; characteristic polynomial: x^16 + x^14 + x^13 + x^11 + 1 */
    SP_rand_lfsr = (SP_rand_lfsr >> 1) ^ ( ((-(SP_rand_lfsr & (uint16_t)0x0001))) & (uint16_t)0xB400);  /* LDRA: negation used for optimization */
    SP_random_base = (SP_random_base << 1) | (SP_rand_lfsr & (uint16_t)0x0001);
    WUP_mant = (uint8_t)(SP_random_base & (uint16_t)0x007F) + (RFTx_MIN_RANDOM_DELAY - 1u);             /* random delay in ms */
    WakeupTimer_Restart((WakeupTimerPeriod_Type){.mant = WUP_mant, .exponent = 0u});
}  /* WakeupTimer_Random_Init */

/* ----------------------------------------------------------------------------
 * 8-bit CRC calculation according to the Baicheva polynomial x^8 + x^5 + x^3 + x^2 + x + 1
 * Arguments:    uint8 const *data  - pointer to the buffer with data to be covered by CRC
 *               uint8 len          - number of bytes to be covered by CRC
 * Returns:     The computed unsigned 8-bit CRC value of the buffer's data
 * Notes:       If needed, this function can be replaced byy the shortest version
 *              that requires more calculation time
 */
STATIC const uint8_t CRC8_Table[256] =
{ /* 0  |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  A  |  B  |  C  |  D  |  E  |  F  */
	0x00, 0x2f, 0x5e, 0x71, 0xbc, 0x93, 0xe2, 0xcd, 0x57, 0x78, 0x09, 0x26, 0xeb, 0xc4, 0xb5, 0x9a,
	0xae, 0x81, 0xf0, 0xdf, 0x12, 0x3d, 0x4c, 0x63, 0xf9, 0xd6, 0xa7, 0x88, 0x45, 0x6a, 0x1b, 0x34,
	0x73, 0x5c, 0x2d, 0x02, 0xcf, 0xe0, 0x91, 0xbe, 0x24, 0x0b, 0x7a, 0x55, 0x98, 0xb7, 0xc6, 0xe9,
	0xdd, 0xf2, 0x83, 0xac, 0x61, 0x4e, 0x3f, 0x10, 0x8a, 0xa5, 0xd4, 0xfb, 0x36, 0x19, 0x68, 0x47,
	0xe6, 0xc9, 0xb8, 0x97, 0x5a, 0x75, 0x04, 0x2b, 0xb1, 0x9e, 0xef, 0xc0, 0x0d, 0x22, 0x53, 0x7c,
	0x48, 0x67, 0x16, 0x39, 0xf4, 0xdb, 0xaa, 0x85, 0x1f, 0x30, 0x41, 0x6e, 0xa3, 0x8c, 0xfd, 0xd2,
	0x95, 0xba, 0xcb, 0xe4, 0x29, 0x06, 0x77, 0x58, 0xc2, 0xed, 0x9c, 0xb3, 0x7e, 0x51, 0x20, 0x0f,
	0x3b, 0x14, 0x65, 0x4a, 0x87, 0xa8, 0xd9, 0xf6, 0x6c, 0x43, 0x32, 0x1d, 0xd0, 0xff, 0x8e, 0xa1,
	0xe3, 0xcc, 0xbd, 0x92, 0x5f, 0x70, 0x01, 0x2e, 0xb4, 0x9b, 0xea, 0xc5, 0x08, 0x27, 0x56, 0x79,
	0x4d, 0x62, 0x13, 0x3c, 0xf1, 0xde, 0xaf, 0x80, 0x1a, 0x35, 0x44, 0x6b, 0xa6, 0x89, 0xf8, 0xd7,
	0x90, 0xbf, 0xce, 0xe1, 0x2c, 0x03, 0x72, 0x5d, 0xc7, 0xe8, 0x99, 0xb6, 0x7b, 0x54, 0x25, 0x0a,
	0x3e, 0x11, 0x60, 0x4f, 0x82, 0xad, 0xdc, 0xf3, 0x69, 0x46, 0x37, 0x18, 0xd5, 0xfa, 0x8b, 0xa4,
	0x05, 0x2a, 0x5b, 0x74, 0xb9, 0x96, 0xe7, 0xc8, 0x52, 0x7d, 0x0c, 0x23, 0xee, 0xc1, 0xb0, 0x9f,
	0xab, 0x84, 0xf5, 0xda, 0x17, 0x38, 0x49, 0x66, 0xfc, 0xd3, 0xa2, 0x8d, 0x40, 0x6f, 0x1e, 0x31,
	0x76, 0x59, 0x28, 0x07, 0xca, 0xe5, 0x94, 0xbb, 0x21, 0x0e, 0x7f, 0x50, 0x9d, 0xb2, 0xc3, 0xec,
	0xd8, 0xf7, 0x86, 0xa9, 0x64, 0x4b, 0x3a, 0x15, 0x8f, 0xa0, 0xd1, 0xfe, 0x33, 0x1c, 0x6d, 0x42
};

#define PRELOAD_CRC_REGISTER    (0xAAu)

STATIC INLINE uint8_t CalcCRC8(uint8_t const *data, uint8_t len)
{
    uint8_t crc = PRELOAD_CRC_REGISTER;

    while ((len--) != 0u)
    {
        crc ^= *data;
        crc = CRC8_Table[crc];
        data++;
    }
    return crc;
} /* End of CalcCRC8 */

/* ----------------------------------------------------------------------------
 * WUPTMR interrupt handler is used to finalize
 * the RF carrier generation started by LFCMD_CW LF command
 */
__attribute__((interrupt)) void ISR_WUPTMR(void)
{
    Radio_Disable();
    /* Restore the RFTX values that could be distorted after CW mode: */
    IO_SET(CUSTOM_91804, RFTX_DIRECT_MOD,    CONFIG_RFTX_DIRECT_MOD);
    IO_SET(CUSTOM_91804, RFTX_EN_MANCHESTER, CONFIG_RFTX_EN_MANCHESTER);
    IO_SET(CUSTOM_91804, RFTX_BIT_INVERT,    CONFIG_RFTX_BIT_INVERT,
                         RFTX_FSK_NOOK,      CONFIG_RFTX_FSK_NOOK);
    Sys_DeepSleepWith(LF_DISABLED);
    /* Since the WUP flag is still set it will provoke immediate wake-up */
}

/* EOF */


