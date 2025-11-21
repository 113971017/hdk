/*
 * Copyright (C) 2018-2022 Melexis N.V.
 *
 * Software Platform
 *
 */

#include <platformlib.h>
#include <sensorlib.h>
#include <debug.h>

#include <datatypes.h>
#include <measurements.h>
#include <communication.h>

void TPMS_state_Measurements(void);
void TPMS_state_Motion_Detection(void);
void TPMS_state_processing_end(void) __attribute__ ((noreturn));
void TPMS_state_processing_and_update(void);
void TPMS_state_Init(tpms_state_t state);

STATIC INLINE void TPMS_state_OFF_processing(void);
STATIC INLINE void TPMS_state_STATIONARY_processing(void);
STATIC INLINE void TPMS_state_NORMAL_DRIVE_processing(void);
STATIC INLINE void TPMS_state_ALERT_AT_STATIONARY_processing(void);
STATIC INLINE void TPMS_state_ALERT_AT_NORMAL_DRIVE_processing(void);
STATIC INLINE void TPMS_state_LEARN_processing(void);
STATIC INLINE void TPMS_state_DEMO_processing(void);

STATIC SP_MEM uint16_t SP_timer_between_RFTx_Reports;
STATIC SP_MEM uint8_t  SP_timer_between_Measurements;
STATIC SP_MEM uint8_t  SP_timer_between_Motion_Detections;

/* ----------------------------------------------------------------------------
 * TPMS states parameters
 */
#define RFTX_DEFAULT_FORMAT RFTX_PT_REPORT_W

RODATA_MEM tpms_state_property_t TPMS_STATE_PARAMETERS[NUMBER_OF_TPMS_STATES]  = {
    {   /* [0] => Parameters of the TPMS state OFF: */
        .WUT_main_period_mant       = 243u, /* 8-bit mantissa (max 255) */
        .WUT_main_period_exp        = 6u,   /* 4-bit exponent (max 12)  */
        .LFRx_enabled               = 1u,   /* Single bit: "1" enables LFRx reception */
        .LFO_calibration_needed     = 0u,   /* Single bit: "1" enables LFO calibration */
        .HFO_calibration_needed     = 0u,   /* Single bit: "1" enables HFO calibration */
        .Measurement_period_in_STT  = 1u,   /* 8-bit number of STT periods between PVT measurements */
        .MotionDetect_period_in_STT = 0u,   /* 8-bit number of STT periods between Motion Detections */
        .RFTx_reports_period_in_STT = 0u,   /* 16-bit number of STT periods between RFTx reports */
        .RFTx_datagram_format_id    = RFTX_DEFAULT_FORMAT,
        .RFTx_datagrams_per_report  = 0u    /* 8-bit number of RFTx datagrams per RFTx report */
    },
    {   /* [1] => Parameters of the TPMS state STATIONARY: */
        .WUT_main_period_mant       = 243u, /* 8-bit mantissa (max 255) */
        .WUT_main_period_exp        = 3u,   /* 4-bit exponent (max 12)  */
        .LFRx_enabled               = 1u,   /* Single bit: "1" enables LFRx reception */
        .LFO_calibration_needed     = 0u,   /* Single bit: "1" enables LFO calibration */
        .HFO_calibration_needed     = 0u,   /* Single bit: "1" enables HFO calibration */
        .Measurement_period_in_STT  = 5u,   /* 8-bit number of STT periods between PVT measurements */
        .MotionDetect_period_in_STT = 1u,   /* 8-bit number of STT periods between Motion Detections */
        .RFTx_reports_period_in_STT = 0u,   /* 16-bit number of STT periods between RFTx reports */
        .RFTx_datagram_format_id    = RFTX_DEFAULT_FORMAT,
        .RFTx_datagrams_per_report  = 0u    /* 8-bit number of RFTx datagrams per RFTx report */
    },
    {    /* [2] => Parameters of the TPMS state NORMAL_DRIVE: */
        .WUT_main_period_mant       = 243u, /* 8-bit mantissa (max 255) */
        .WUT_main_period_exp        = 3u,   /* 4-bit exponent (max 12)  */
        .LFRx_enabled               = 0u,   /* Single bit: "1" enables LFRx reception */
        .LFO_calibration_needed     = 0u,   /* Single bit: "1" enables LFO calibration */
        .HFO_calibration_needed     = 0u,   /* Single bit: "1" enables HFO calibration */
        .Measurement_period_in_STT  = 1u,   /* 8-bit number of STT periods between PVT measurements */
        .MotionDetect_period_in_STT = 3u,   /* 8-bit number of STT periods between Motion Detections */
        .RFTx_reports_period_in_STT = 30u,  /* 16-bit number of STT periods between RFTx reports */
        .RFTx_datagram_format_id    = RFTX_PT_REPORT_W,
        .RFTx_datagrams_per_report  = 4u    /* 8-bit number of RFTx datagrams per RFTx report */
    },
    {   /* [3] => Parameters of the TPMS state ALERT_AT_STATIONARY: */
        .WUT_main_period_mant       = 243u, /* 8-bit mantissa (max 255) */
        .WUT_main_period_exp        = 3u,   /* 4-bit exponent (max 12)  */
        .LFRx_enabled               = 1u,   /* Single bit: "1" enables LFRx reception */
        .LFO_calibration_needed     = 0u,   /* Single bit: "1" enables LFO calibration */
        .HFO_calibration_needed     = 0u,   /* Single bit: "1" enables HFO calibration */
        .Measurement_period_in_STT  = 1u,   /* 8-bit number of STT periods between PVT measurements */
        .MotionDetect_period_in_STT = 1u,   /* 8-bit number of STT periods between Motion Detections */
        .RFTx_reports_period_in_STT = 1u,   /* 16-bit number of STT periods between RFTx reports */
        .RFTx_datagram_format_id    = RFTX_PVT_REPORT,
        .RFTx_datagrams_per_report  = 6u    /* 8-bit number of RFTx datagrams per RFTx report */
    },
    {   /* [4] => Parameters of the TPMS state ALERT_AT_NORMAL_DRIVE: */
        .WUT_main_period_mant       = 243u, /* 8-bit mantissa (max 255) */
        .WUT_main_period_exp        = 3u,   /* 4-bit exponent (max 12)  */
        .LFRx_enabled               = 0u,   /* Single bit: "1" enables LFRx reception */
        .LFO_calibration_needed     = 0u,   /* Single bit: "1" enables LFO calibration */
        .HFO_calibration_needed     = 0u,   /* Single bit: "1" enables HFO calibration */
        .Measurement_period_in_STT  = 1u,   /* 8-bit number of STT periods between PVT measurements */
        .MotionDetect_period_in_STT = 1u,   /* 8-bit number of STT periods between Motion Detections */
        .RFTx_reports_period_in_STT = 1u,   /* 16-bit number of STT periods between RFTx reports */
        .RFTx_datagram_format_id    = RFTX_PVT_REPORT,
        .RFTx_datagrams_per_report  = 8u    /* 8-bit number of RFTx datagrams per RFTx report */
    },
    {   /* [5] => Parameters of the TPMS state LEARN: */
        .WUT_main_period_mant       = 243u, /* 8-bit mantissa (max 255) */
        .WUT_main_period_exp        = 3u,   /* 4-bit exponent (max 12)  */
        .LFRx_enabled               = 1u,   /* Single bit: "1" enables LFRx reception */
        .LFO_calibration_needed     = 0u,   /* Single bit: "1" enables LFO calibration */
        .HFO_calibration_needed     = 0u,   /* Single bit: "1" enables HFO calibration */
        .Measurement_period_in_STT  = 1u,   /* 8-bit number of STT periods between PVT measurements */
        .MotionDetect_period_in_STT = 1u,   /* 8-bit number of STT periods between Motion Detections */
        .RFTx_reports_period_in_STT = 20u,  /* 16-bit number of STT periods between RFTx reports */
        .RFTx_datagram_format_id    = RFTX_LEARN_REPORT,
        .RFTx_datagrams_per_report  = 4u    /* 8-bit number of RFTx datagrams per RFTx report */
    },
    {   /* [6] => Parameters of the TPMS state FACTORY_STATIONARY: */
        .WUT_main_period_mant       = 243u, /* 8-bit mantissa (max 255) */
        .WUT_main_period_exp        = 3u,   /* 4-bit exponent (max 12)  */
        .LFRx_enabled               = 1u,   /* Single bit: "1" enables LFRx reception */
        .LFO_calibration_needed     = 0u,   /* Single bit: "1" enables LFO calibration */
        .HFO_calibration_needed     = 0u,   /* Single bit: "1" enables HFO calibration */
        .Measurement_period_in_STT  = 5u,   /* 8-bit number of STT periods between PVT measurements */
        .MotionDetect_period_in_STT = 1u,   /* 8-bit number of STT periods between Motion Detections */
        .RFTx_reports_period_in_STT = 0u,   /* 16-bit number of STT periods between RFTx reports */
        .RFTx_datagram_format_id    = RFTX_DEFAULT_FORMAT,
        .RFTx_datagrams_per_report  = 0u    /* 8-bit number of RFTx datagrams per RFTx report */
    },
    {   /* [7] => Parameters of the TPMS state FACTORY_NORMAL_DRIVE: */
        .WUT_main_period_mant       = 243u, /* 8-bit mantissa (max 255) */
        .WUT_main_period_exp        = 3u,   /* 4-bit exponent (max 12)  */
        .LFRx_enabled               = 0u,   /* Single bit: "1" enables LFRx reception */
        .LFO_calibration_needed     = 0u,   /* Single bit: "1" enables LFO calibration */
        .HFO_calibration_needed     = 0u,   /* Single bit: "1" enables HFO calibration */
        .Measurement_period_in_STT  = 1u,   /* 8-bit number of STT periods between PVT measurements */
        .MotionDetect_period_in_STT = 3u,   /* 8-bit number of STT periods between Motion Detections */
        .RFTx_reports_period_in_STT = 6u,   /* 16-bit number of STT periods between RFTx reports */
        .RFTx_datagram_format_id    = RFTX_PT_REPORT_W,
        .RFTx_datagrams_per_report  = 4u    /* 8-bit number of RFTx datagrams per RFTx report */
    },
    {   /* [8] => Parameters of the TPMS state FACTORY_ALERT_AT_STATIONARY: */
        .WUT_main_period_mant       = 243u, /* 8-bit mantissa (max 255) */
        .WUT_main_period_exp        = 2u,   /* 4-bit exponent (max 12)  */
        .LFRx_enabled               = 1u,   /* Single bit: "1" enables LFRx reception */
        .LFO_calibration_needed     = 0u,   /* Single bit: "1" enables LFO calibration */
        .HFO_calibration_needed     = 0u,   /* Single bit: "1" enables HFO calibration */
        .Measurement_period_in_STT  = 1u,   /* 8-bit number of STT periods between PVT measurements */
        .MotionDetect_period_in_STT = 1u,   /* 8-bit number of STT periods between Motion Detections */
        .RFTx_reports_period_in_STT = 1u,   /* 16-bit number of STT periods between RFTx reports */
        .RFTx_datagram_format_id    = RFTX_PVT_REPORT,
        .RFTx_datagrams_per_report  = 6u    /* 8-bit number of RFTx datagrams per RFTx report */
    },
    {   /* [9] => Parameters of the TPMS state FACTORY_ALERT_AT_DRIVE: */
        .WUT_main_period_mant       = 243u, /* 8-bit mantissa (max 255) */
        .WUT_main_period_exp        = 2u,   /* 4-bit exponent (max 12)  */
        .LFRx_enabled               = 0u,   /* Single bit: "1" enables LFRx reception */
        .LFO_calibration_needed     = 0u,   /* Single bit: "1" enables LFO calibration */
        .HFO_calibration_needed     = 0u,   /* Single bit: "1" enables HFO calibration */
        .Measurement_period_in_STT  = 1u,   /* 8-bit number of STT periods between PVT measurements */
        .MotionDetect_period_in_STT = 1u,   /* 8-bit number of STT periods between Motion Detections */
        .RFTx_reports_period_in_STT = 1u,   /* 16-bit number of STT periods between RFTx reports */
        .RFTx_datagram_format_id    = RFTX_PVT_REPORT,
        .RFTx_datagrams_per_report  = 8u    /* 8-bit number of RFTx datagrams per RFTx report */
    },
    {   /* [10] => Parameters of the TPMS state FACTORY_LEARN: */
        .WUT_main_period_mant       = 243u, /* 8-bit mantissa (max 255) */
        .WUT_main_period_exp        = 3u,   /* 4-bit exponent (max 12)  */
        .LFRx_enabled               = 1u,   /* Single bit: "1" enables LFRx reception */
        .LFO_calibration_needed     = 0u,   /* Single bit: "1" enables LFO calibration */
        .HFO_calibration_needed     = 0u,   /* Single bit: "1" enables HFO calibration */
        .Measurement_period_in_STT  = 1u,   /* 8-bit number of STT periods between PVT measurements */
        .MotionDetect_period_in_STT = 1u,   /* 8-bit number of STT periods between Motion Detections */
        .RFTx_reports_period_in_STT = 20u,  /* 16-bit number of STT periods between RFTx reports */
        .RFTx_datagram_format_id    = RFTX_LEARN_REPORT,
        .RFTx_datagrams_per_report  = 4u    /* 8-bit number of RFTx datagrams per RFTx report */
    },
    {   /* [11] => Parameters of the TPMS state DEMO: */
        .WUT_main_period_mant       = 243u, /* 8-bit mantissa (max 255) */
        .WUT_main_period_exp        = 2u,   /* 4-bit exponent (max 12)  */
        .LFRx_enabled               = 1u,   /* Single bit: "1" enables LFRx reception */
        .LFO_calibration_needed     = 0u,   /* Single bit: "1" enables LFO calibration */
        .HFO_calibration_needed     = 0u,   /* Single bit: "1" enables HFO calibration */
        .Measurement_period_in_STT  = 1u,   /* 8-bit number of STT periods between PVT measurements */
        .MotionDetect_period_in_STT = 2u,   /* 8-bit number of STT periods between Motion Detections */
        .RFTx_reports_period_in_STT = 4u,   /* 16-bit number of STT periods between RFTx reports */
        .RFTx_datagram_format_id    = RFTX_DEMO,
        .RFTx_datagrams_per_report  = 5u    /* 8-bit number of RFTx datagrams per RFTx report */
    },
    {   /* [12] => Parameters of the TPMS state DISABLED: */
        .WUT_main_period_mant       = 255u, /* 8-bit mantissa (max 255) */
        .WUT_main_period_exp        = 12u,  /* 4-bit exponent (max 12)  */
        .LFRx_enabled               = 1u,   /* Single bit: "1" enables LFRx reception */
        .LFO_calibration_needed     = 0u,   /* Single bit: "1" enables LFO calibration */
        .HFO_calibration_needed     = 0u,   /* Single bit: "1" enables HFO calibration */
        .Measurement_period_in_STT  = 0u,   /* 8-bit number of STT periods between PVT measurements */
        .MotionDetect_period_in_STT = 0u,   /* 8-bit number of STT periods between Motion Detections */
        .RFTx_reports_period_in_STT = 0u,   /* 16-bit number of STT periods between RFTx reports */
        .RFTx_datagram_format_id    = RFTX_DEFAULT_FORMAT,
        .RFTx_datagrams_per_report  = 0u    /* 8-bit number of RFTx datagrams per RFTx report */
    }
};

/* ----------------------------------------------------------------------------
 * PVT and a simple acceleration measurements at TPMS state
 * Arguments:   none
 * Returns:     none
 * Notes:
 */
void TPMS_state_Measurements(void)
{
    if (TPMS_STATE_PARAMETERS[SP_state.as_field.tpms_state].Measurement_period_in_STT != 0u)
    {
        if (SP_timer_between_Measurements < (uint8_t)(TPMS_STATE_PARAMETERS[SP_state.as_field.tpms_state].Measurement_period_in_STT - 1u))
        {
            SP_timer_between_Measurements += 1u;
            DEBUG_PUT_STRING("PVT.miss ");
        }
        else
        {
	        Measure_PVT();
	        if (PVT_measurements_valid == true)
	        {
	            SP_timer_between_Measurements = 0u;
            }
            else { /* Keep actual timer value to provoke PVT measurements at the next STT */ }
        }
    }
    else { /* Measurements are not assigned for this TPMS mode */ }

} /* TPMS_state_Measurements */

/* ----------------------------------------------------------------------------
 * Motion Detection at TPMS state
 * Arguments:   none
 * Returns:     none
 * Notes:
 */
void TPMS_state_Motion_Detection(void)
{
    if (TPMS_STATE_PARAMETERS[SP_state.as_field.tpms_state].MotionDetect_period_in_STT != 0u)
    {
        if (SP_timer_between_Motion_Detections < (uint8_t)(TPMS_STATE_PARAMETERS[SP_state.as_field.tpms_state].MotionDetect_period_in_STT - 1u))
        {
            SP_timer_between_Motion_Detections += 1u;
        }
        else
        {
	        SP_timer_between_Motion_Detections = 0u;
	        Direct_Motion_Detection();      /* to define a motion status without going to sleep mode */
            DEBUG_PUT_STRING("MD.done ");
        }
    }
    else { /* Motion Detections are not assigned for this TPMS mode */ }

} /* TPMS_state_Motion_Detection */

/* ----------------------------------------------------------------------------
 * Final stage of the STT wake-up with or without RFTx report and LFRx listening
 * Arguments:   none
 * Returns:     none
 * Notes:
 */
void TPMS_state_processing_end(void)
{
    if ( (TPMS_STATE_PARAMETERS[SP_state.as_field.tpms_state].RFTx_reports_period_in_STT != 0u)
    &&   (TPMS_STATE_PARAMETERS[SP_state.as_field.tpms_state].RFTx_datagrams_per_report != 0u) )
    {
        if (SP_timer_between_RFTx_Reports < (uint16_t)(TPMS_STATE_PARAMETERS[SP_state.as_field.tpms_state].RFTx_reports_period_in_STT - 1u))
        {
            SP_timer_between_RFTx_Reports += 1u;
        }
        else
        {
	        SP_timer_between_RFTx_Reports = 0u;
            RFTx_delay_is_random = true;
            RFTx_Datagram_Counter = (uint8_t)TPMS_STATE_PARAMETERS[SP_state.as_field.tpms_state].RFTx_datagrams_per_report;
            Start_RFTx_reporting(TPMS_STATE_PARAMETERS[SP_state.as_field.tpms_state].RFTx_datagram_format_id);  /* no return */
        }
    }
    else { /* RFTx reporting is not assigned for this TPMS state */ }

    WakeupTimer_Restart((WakeupTimerPeriod_Type){
        .mant     = TPMS_STATE_PARAMETERS[SP_state.as_field.tpms_state].WUT_main_period_mant,
        .exponent = TPMS_STATE_PARAMETERS[SP_state.as_field.tpms_state].WUT_main_period_exp});
    if (TPMS_STATE_PARAMETERS[SP_state.as_field.tpms_state].LFRx_enabled == 1)
    {
        DEBUG_PUT_STRING("DSleepLF ");
        LfRx_FifoFlush();
        Sys_DeepSleepWith(LF_RECEIVE);
    }
    else
    {
        DEBUG_PUT_STRING("DSleep ");
        Sys_DeepSleepWith(LF_DISABLED);
    }
    while(true) { /* This dummy cycle ensures for compiler there is no return */
        /* Actually this cycle is not needed because a sleep mode is set before */
    }
} /* TPMS_state_processing_end */

/* ----------------------------------------------------------------------------
 * Update TPMS state
 * Arguments:   none
 * Returns:     none
 * Notes:
 */
void TPMS_state_processing_and_update(void)
{
    switch (SP_state.as_field.tpms_state)
    {
        case OFF:
        /* Standby TPMS mode (waiting for LF activation or for a pressure change) */
            TPMS_state_OFF_processing();
            break;

        case STATIONARY:
        /* Confirmed stationary / parking state (not just a short stop) */
        case FACTORY_STATIONARY:
        /* Artificial STATIONARY needed for the factory tests */
            TPMS_state_STATIONARY_processing();
            break;

        case NORMAL_DRIVE:
        /* Confirmed driving state */
        case FACTORY_NORMAL_DRIVE:
        /* Artificial DRIVING needed for the factory tests */
            TPMS_state_NORMAL_DRIVE_processing();
            break;

        case ALERT_AT_STATIONARY:
        /* Fast pressure change at stationary / parking (can be filling assist) */
        case FACTORY_ALERT_AT_STATIONARY:
        /* Artificial ALERT_AT_STATIONARY needed for the factory tests */
            TPMS_state_ALERT_AT_STATIONARY_processing();
            break;

        case ALERT_AT_NORMAL_DRIVE:
        /* Fast pressure change at driving (can be pressure drop due to tire damage) */
        case FACTORY_ALERT_AT_DRIVE:
        /* Artificial ALERT_AT_NORMAL_DRIVE needed for the factory tests */
            TPMS_state_ALERT_AT_NORMAL_DRIVE_processing();
            break;

        case LEARN:
        /* Learn Auto-Location procedure */
        case FACTORY_LEARN:
        /* Artificial LEARN mode is needed for the factory tests */
            TPMS_state_LEARN_processing();
            break;

        case DEMO:
            /* Melexis TPMS Demo mode */
            TPMS_state_DEMO_processing();
            break;

        case DISABLED:
        /* Special state of fully disabled TPMS that is only waiting for LF activation */
        default:
        /* Nothing to do => just skip all possible actions */
            break;

    }  /* END of switch (rftx_format_id) */
} /* TPMS_state_processing_and_update */

/* ----------------------------------------------------------------------------
 * Processing of the TPMS state OFF
 * Arguments:   none
 * Returns:     none
 * Notes:
 */
STATIC INLINE void TPMS_state_OFF_processing(void)
{
    if (PVT_measurements_valid == true)
    {
	    /* Please put here an application code to process TPMS states transitions */
    }
    else { /* optional branch */ }
    if (Acceleration_measurements_valid == true)
    {
	    /* Please put here an application code to process TPMS states transitions */
    }
    else { /* optional branch */ }
} /* TPMS_state_OFF_processing */

/* ----------------------------------------------------------------------------
 * Processing of the TPMS state STATIONARY and FACTORY_STATIONARY
 * Arguments:   none
 * Returns:     none
 * Notes:
 */
STATIC INLINE void TPMS_state_STATIONARY_processing(void)
{
    if (PVT_measurements_valid == true)
    {
	    /* Please put here an application code to process TPMS states transitions */
    }
    else { /* optional branch */ }
    if (Acceleration_measurements_valid == true)
    {
	    /* Please put here an application code to process TPMS states transitions */
    }
    else { /* optional branch */ }
} /* TPMS_state_STATIONARY_processing */

/* ----------------------------------------------------------------------------
 * Processing of the TPMS state NORMAL_DRIVE and FACTORY_NORMAL_DRIVE
 * Arguments:   none
 * Returns:     none
 * Notes:
 */
STATIC INLINE void TPMS_state_NORMAL_DRIVE_processing(void)
{
    if (PVT_measurements_valid == true)
    {
	    /* Please put here an application code to process TPMS states transitions */
    }
    else { /* optional branch */ }
    if (Acceleration_measurements_valid == true)
    {
	    /* Please put here an application code to process TPMS states transitions */
    }
    else { /* optional branch */ }
} /* TPMS_state_NORMAL_DRIVE_processing */

/* ----------------------------------------------------------------------------
 * Processing of the TPMS state ALERT_AT_STATIONARY and FACTORY_ALERT_AT_STATIONARY
 * Arguments:   none
 * Returns:     none
 * Notes:
 */
STATIC INLINE void TPMS_state_ALERT_AT_STATIONARY_processing(void)
{
    if (PVT_measurements_valid == true)
    {
	    /* Please put here an application code to process TPMS states transitions */
    }
    else { /* optional branch */ }
    if (Acceleration_measurements_valid == true)
    {
	    /* Please put here an application code to process TPMS states transitions */
    }
    else { /* optional branch */ }
} /* TPMS_state_ALERT_AT_STATIONARY_processing */

/* ----------------------------------------------------------------------------
 * Processing of the TPMS state ALERT_AT_NORMAL_DRIVE and FACTORY_ALERT_AT_DRIVE
 * Arguments:   none
 * Returns:     none
 * Notes:
 */
STATIC INLINE void TPMS_state_ALERT_AT_NORMAL_DRIVE_processing(void)
{
    if (PVT_measurements_valid == true)
    {
	    /* Please put here an application code to process TPMS states transitions */
    }
    else { /* optional branch */ }
    if (Acceleration_measurements_valid == true)
    {
	    /* Please put here an application code to process TPMS states transitions */
    }
    else { /* optional branch */ }
} /* TPMS_state_ALERT_AT_NORMAL_DRIVE_processing */

/* ----------------------------------------------------------------------------
 * Processing of the TPMS state LEARN and FACTORY_LEARN
 * Arguments:   none
 * Returns:     none
 * Notes:
 */
STATIC INLINE void TPMS_state_LEARN_processing(void)
{
    if (PVT_measurements_valid == true)
    {
	    /* Please put here an application code to process TPMS states transitions */
    }
    else { /* optional branch */ }
    if (Acceleration_measurements_valid == true)
    {
	    /* Please put here an application code to process TPMS states transitions */
    }
    else { /* optional branch */ }
} /* TPMS_state_LEARN_processing */

/* ----------------------------------------------------------------------------
 * Processing of the TPMS state DEMO
 * Arguments:   none
 * Returns:     none
 * Notes:
 */
STATIC INLINE void TPMS_state_DEMO_processing(void)
{
    if (PVT_measurements_valid == true)
    {
	    /* Please put here an application code to process TPMS states transitions */;
    }
    else { /* optional branch */ }
    if (Acceleration_measurements_valid == true)
    {
	    /* Please put here an application code to process TPMS states transitions */
    }
    else { /* optional branch */ }
} /* TPMS_state_DEMO_processing */

/* ----------------------------------------------------------------------------
 * TPMS state configuration
 * Arguments:   TPMS state to be set
 * Returns:     none
 * Notes:
 */
void TPMS_state_Init(tpms_state_t state)
{
    SP_state.as_field.tpms_state = state;
    SP_timer_between_Measurements = 0u;
    SP_timer_between_Motion_Detections = 0u;
    SP_timer_between_RFTx_Reports = 0u;
} /* TPMS_state_Init */

/* EOF */
