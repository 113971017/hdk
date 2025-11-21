#include <stdio.h>

#include <sensorlib.h>
#include <platformlib.h>
#include "datatypes.h"
#include "sg_common.h"
#include "sg_dbg.h"





void delay_ms(uint16_t delayMs)
{
    while (delayMs-- != 0u) {
    	DELAY_US( SG_DELAY_NOPS_1_MS );
    }
}


void wtimer_restart_ms(uint16_t periodMs)
{
	if( periodMs == 50 ){
		//WakeupTimer_Restart(WT_PERIOD_50_MS);
		WakeupTimer_Init(MANT_50ms, EXP_50ms);
	}
	else if( periodMs == 100 ){
		//WakeupTimer_Restart(WT_PERIOD_100_MS);
		WakeupTimer_Init(MANT_100ms, EXP_100ms);
	}
	else {
		//WakeupTimer_Restart(WT_PERIOD_100_MS);
		WakeupTimer_Init(MANT_100ms, EXP_100ms);
#ifdef SG_ENABLE_DEBUG_UART
		dbg_string("Err:reset_ms");
#endif
	}

}



void wtimer_restart_sec(uint16_t periodSec)
{
	/* 2024/01/04
	 * WakeupTimer_Restart() change to WakeupTimer_Init();
	 */
	if( 15 == periodSec ){		// for storage
		WakeupTimer_Init(MANT_15s, EXP_15s);
	}
	else if( 5 == periodSec ){	// for PVT
		WakeupTimer_Init(MANT_5s, EXP_5s);
	}
	else if( 10 == periodSec ){	// for Learn Drive Mode
		WakeupTimer_Init(MANT_10s, EXP_10s);
	}
	else if( 1 == periodSec ){
		WakeupTimer_Init(MANT_1s, EXP_1s);
	}
	else if( 2 == periodSec ){
		WakeupTimer_Init(MANT_2s, EXP_2s);
	}
	else if( 3 == periodSec ){
		WakeupTimer_Init(MANT_3s, EXP_3s);
	}
	else if( 4 == periodSec ){
		WakeupTimer_Init(MANT_4s, EXP_4s);
	}
	else if( 6 == periodSec ){
		WakeupTimer_Init(MANT_6s, EXP_6s);
	}
	else if( 7 == periodSec ){
		WakeupTimer_Init(MANT_7s, EXP_7s);
	}
	else if( 8 == periodSec ){
		WakeupTimer_Init(MANT_8s, EXP_8s);
	}
	else if( 9 == periodSec ){
		WakeupTimer_Init(MANT_9s, EXP_9s);
	}
	else if( 20 == periodSec ){
		WakeupTimer_Init(MANT_20s, EXP_20s);
	}
	else if( 30 == periodSec ){
		WakeupTimer_Init(MANT_30s, EXP_30s);
	}
	else if( 60 == periodSec ){
		WakeupTimer_Init(MANT_60s, EXP_60s);
	}
	else {
		// Storage mode
		WakeupTimer_Init(MANT_15s, EXP_15s);
#ifdef SG_ENABLE_DEBUG_UART
		dbg_string("unknow_WTsec");
#endif
	}

}


/* ----------------------------------------------------------------------------
 * Convert pressure value to spi transform format
 * Arguments:	none
 * Returns:     unsigned 16-bit pressure value
 * Notes:
 */
uint16_t get_cmd_pressure(uint16_t press)
{
    uint16_t m_val = PKG_PRESSURE_INVALID;


	if (press < PRESSURE_RANGE_LOW) {		// If Pressure is below 100 kPa
		m_val = 0;	//PKG_PRESSURE_UNDERFLOW;
	}
	else {
		m_val = press;
	}
#if defined( SG_ENABLE_MEASURE_CHK_LIMIT_P )
	if( get_pressgre_range() == PRESSURE_RANGE_100_1400_KPA ) {
		if( press > (PRESSURE_RANGE_HIGH_TRUCK-PRESSURE_RANGE_LOW) ) {
			press = PKG_PRESSURE_OVERFLOW;
		}
	}
	else {
		if( press > (PRESSURE_RANGE_HIGH-PRESSURE_RANGE_LOW) ) {
			press = PKG_PRESSURE_OVERFLOW;
		}
	}
#endif

    return m_val;
}

/* ----------------------------------------------------------------------------
 * Convert temperature value to spi transform format
 * Arguments:	none
 * Returns:     unsigned 8-bit temperature value
 * Notes:
 */
uint8_t get_cmd_temperature(uint8_t temp)
{
    uint8_t m_val = PKG_TEMPERATURE_INVALID;

    if (measurements_valid & DET_PVT)
    {
        if (temp < (TEMPERATURE_RANGE_LOW + PKG_TEMPERATURE_OFFSET)) {        // If Temperature is below -40 degC
        	m_val = PKG_TEMPERATURE_UNDERFLOW;
        }
        else if (temp > (TEMPERATURE_RANGE_HIGH + PKG_TEMPERATURE_OFFSET)) {  // If Temperature is above +125 degC
        	m_val = PKG_TEMPERATURE_OVERFLOW;
        }
        else {																  // Temperature value is within the acceptable range
        	m_val = temp + PKG_TEMPERATURE_OFFSET;
        }
    }

    return m_val;
}

/* ----------------------------------------------------------------------------
 * Convert battery voltage value to 8-bit spi transform format
 * Arguments:	none
 * Returns:     unsigned 8-bit voltage value
 * Notes:
 */
uint8_t get_cmd_voltage(uint16_t volt)
{
    uint8_t m_val = PKG_VOLTAGE_INVALID;

    if (measurements_valid & DET_PVT)
    {
        if (volt <= VOLTAGE_RANGE_LOW) {		// If Voltage is below 1.8 Volt
        	m_val = PKG_VOLTAGE_UNDERFLOW;
        }
        else if (volt > VOLTAGE_RANGE_HIGH) {	// If Voltage is bigger 3.6 Volt
        	m_val = PKG_VOLTAGE_OVERFLOW;
        }
        else {    								// Normal voltage range
        	m_val = (uint8_t)(volt - 122);
        }
    }

    return m_val;
}


uint8_t get_pressgre_range(void)
{
	uint8_t pressure_range = Sys_GetPressureRange();

#if 0	// Test : simulation truck or not
	pressure_range = PRESSURE_RANGE_100_900_KPA;
#endif
	if( PRESSURE_RANGE_100_1400_KPA == pressure_range ) {
		SP_drive_mode_report_period_spi = (DEFAULT_DRIVE_MODE_REPORT_PERIOD_SEC_TRUCK/DEFAULT_REPORT_PERIOD_STEP_SEC);
	}
	else {
		SP_drive_mode_report_period_spi = (DEFAULT_DRIVE_MODE_REPORT_PERIOD_SEC/DEFAULT_REPORT_PERIOD_STEP_SEC);
	}

	return (uint8_t)pressure_range;
}

