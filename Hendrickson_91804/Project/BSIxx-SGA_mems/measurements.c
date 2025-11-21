/*
 * Copyright (C) 2018-2019 Melexis N.V.
 *
 * Software Platform
 *
 */
#include <sensorlib.h>
#include <platformlib.h>
#include <stdlib.h>
#include "string.h"

#include "datatypes.h"
#include "measurements.h"
#if defined(SG_ENABLE_SPI)
#include "spi_process.h"
#endif
#include "main.h"
#include "sg_common.h"
#include "sg_dbg.h"
#include "processing.h"


#ifdef SG_ENABLE_NEW_MEASURE_REC
	SP_MEM T_TPMS_DATA_MEASURE_REC SP_measured_rec;
#else
	SP_MEM T_TPMS_DATA_MEASURE SP_measured_rec;
#endif
DP_MEM T_TPMS_DATA_MEASURE measured;

// LFRX Manchester code
#if 0
DP_MEM uint8_t gLfRx_buffer[8];
#else
SP_MEM T_LFRX_MANCHESTER SP_lfrx_rec;
#endif





#define Get_direct_acceleration()  Compensated_Acceleration_Measurement_Z()



/* ----------------------------------------------------------------------------
 * Implement Compensated Acceleration Measurement_X (CAM)
 * Arguments:   none
 * Returns:     16-bit signed result with acceleration expressed in [g*10]
 * Notes:
 */
int16_t Compensated_Acceleration_Measurement_X(void)
{   /* for X sensitive axis of X / XZ accelerometer */
    #if defined(HAS_ACCEL_X_SENSOR)

    /* Perform compensated acceleration measurements (cast to fit into container type) */
    int16_t acceleration_result = (uint16_t)Phys_AccelerationX();
    if (!Adc_IsError())     /* if no error  .. */
    {   /* Convert to physical units if no error happened: */
    }
    else
    {   /* some conversion error happened */
        acceleration_result = ACCELERATION_INVALID;
        Adc_ClearAllErrors();      /* Clear all possible ADC errors */
    }
    /* Acceleration [g] = (float)acceleration_result / 10. */
    return acceleration_result;

    #else

    return ACCELERATION_INVALID;

    #endif /* HAS_ACCEL_X_SENSOR */
}   /* Compensated_Acceleration_Measurement_X */

/* ----------------------------------------------------------------------------
 * Implement Compensated Acceleration Measurement_Z (CAM)
 * Arguments:   none
 * Returns:     16-bit signed result with acceleration expressed in [g*10]
 * Notes:
 */
int16_t Compensated_Acceleration_Measurement_Z(void)
{   /* for Z sensitive axis of Z / XZ accelerometer */
    #if defined(HAS_ACCEL_Z_SENSOR)

    /* Perform compensated acceleration measurements (cast to fit into container type) */
    int16_t acceleration_result = (uint16_t)Phys_AccelerationZ();
    if (!Adc_IsError())     /* if no error  .. */
    {   /* Convert to physical units if no error happened: */
    }
    else
    {   /* some conversion error happened */
        acceleration_result = ACCELERATION_INVALID;
        Adc_ClearAllErrors();      /* Clear all possible ADC errors */
    }
    /* Acceleration in [g] equals to "acceleration_result" divided by 10 */
    return acceleration_result;

    #else

    return ACCELERATION_INVALID;

    #endif /* HAS_ACCEL_Z_SENSOR */
}   /* Compensated_Acceleration_Measurement_Z */

/* ----------------------------------------------------------------------------
 * Direct Motion Detection procedure based on the measurement of the centripetal acceleration
 * Arguments:   none
 * Returns:     none
 * Notes:
 */
void Direct_Motion_Detection(void)
{
    bool bChangeMode = false;
    int16_t m_acceleration = 0;


#ifdef TEST_ENABLE_DUMMY_DATA
    if( SP_dummy_enabled_flag & DBG_FLAG_DUMMY ) {
    	m_acceleration = measured.acceleration_z;
    }
    else
#endif
    {
/*
    	if( measurements_valid & DET_G ) {
    		measured.acceleration_z = ACCELERATION_INVALID;
    	}
*/
    	measured.acceleration_z = Compensated_Acceleration_Measurement_Z();

    	if( measured.acceleration_z == (int16_t)ACCELERATION_INVALID ){
    		measurements_valid = (0xFF^DET_G);
    		return;
    	}
    	measurements_valid |= DET_G;

    	// ABS
    	if( measured.acceleration_z < 0 ){
    		m_acceleration = 0 - measured.acceleration_z;
    	}
    	else {
    		m_acceleration = measured.acceleration_z;
    	}

#ifdef SG_ENABLE_MOTION_DET_XZ

    	measured.acceleration_x = Compensated_Acceleration_Measurement_X();
    	if( measured.acceleration_x != (int16_t)ACCELERATION_INVALID ){
			if( measured.acceleration_x < 0 ) {
				if( (m_acceleration + measured.acceleration_x) < 0 ){
					m_acceleration = 0 - measured.acceleration_x;
				}

			}
			else {
				if( (m_acceleration - measured.acceleration_x) < 0 ){
					m_acceleration = measured.acceleration_x;
				}
			}
    	}
#endif

    }

#if(0)
    if ( m_acceleration >= (int16_t)ACC_Z_MOTION_THRESHOLD_HIGH ) {

    	if( SP_state.as_field.rotation != ROTATING ){
			SP_state.as_field.rotation = ROTATING;
    	}

		if(SP_state.as_field.tpms_state == STATE_DRIVE_INTERIM) {
			SP_state.as_field.tpms_state = STATE_DRIVE;
			bChangeMode = true;
		}
		else if(SP_state.as_field.tpms_state == STATE_STATIONARY) {
			SP_state.as_field.tpms_state = STATE_LEARN;

			bChangeMode = true;
		}
		else if(SP_state.as_field.tpms_state == STATE_LEARN_INTERIM) {
			SP_state.as_field.tpms_state = STATE_LEARN;
			bChangeMode = true;
		}
    }
    else if ( m_acceleration < ACC_Z_MOTION_THRESHOLD_LOW ) {

    	if( SP_state.as_field.rotation != STOPPED &&
    			m_acceleration < ACC_Z_SIGNAL_UNDERFLOW ){
    		SP_state.as_field.rotation = STOPPED;
    	}

		if(SP_state.as_field.tpms_state == STATE_DRIVE) {
			SP_state.as_field.tpms_state = STATE_DRIVE_INTERIM;
			bChangeMode = true;
		}
		else if(SP_state.as_field.tpms_state == STATE_LEARN) {
			SP_state.as_field.tpms_state = STATE_LEARN_INTERIM;
			bChangeMode = true;
		}
		else {
			if( SP_state.as_field.tpms_state == STATE_LEARN_INTERIM &&
				(SP_state_keep_time >= TPMS_STATE_PARAMETERS[STATE_LEARN].state_keep_timeout) ) {
				SP_state.as_field.tpms_state = STATE_DRIVE_INTERIM;
				bChangeMode = true;
			}
		}

    }
    else { /* Keep previous motion state */ }
#endif

	if( bChangeMode ){
		SP_state.as_field.action_state |= AS_STATE_CHANGE;
		SP_state_keep_time = 0;
	}
}   /* Direct_Motion_Detection */


/* ----------------------------------------------------------------------------
 * Measure_PVT
 * Arguments:	none
 * Returns:     none
 * Notes:       Can be executed after NvRam_Recall only!
 */
void Measure_PVT(void)
{
    Adc_RawPvt_Type raw_pvt;

    if(measurements_valid & DET_PVT)
    	return;

	Adc_ReadPVT(&raw_pvt);

	if (!Adc_IsError()) {
		// Pressure in [kPa] equals to high word of "Calc_Pressure_32b"
		measured.pvt.pressure    = (uint16_t)(Calc_Pressure(raw_pvt.pressure, raw_pvt.temperature, raw_pvt.voltage) >> 16);
		// Battery voltage in [V] equals to tmp16 divided by 100
		measured.pvt.voltage     = Calc_Voltage(raw_pvt.voltage);

		// Temperature in [degC] equals to high byte of "Calc_Temperature_16b" - 52degC
		measured.pvt.temperature = (uint8_t)(Calc_Temperature(raw_pvt.temperature) >> 8);

		measurements_valid |= DET_PVT;


		// Check limit
		//measured.pvt.temperature = (-42+PKG_TEMPERATURE_OFFSET); // -40 degC;
		if( measured.pvt.temperature < (TEMPERATURE_RANGE_LOW+PKG_TEMPERATURE_OFFSET) ){
			measured.pvt.temperature = (TEMPERATURE_RANGE_LOW+PKG_TEMPERATURE_OFFSET);
		}
		else if( measured.pvt.temperature > (TEMPERATURE_RANGE_HIGH+PKG_TEMPERATURE_OFFSET) ){
			measured.pvt.temperature = (TEMPERATURE_RANGE_HIGH+PKG_TEMPERATURE_OFFSET);
		}


		if(measured.pvt.pressure < PRESSURE_RANGE_LOW) {
			measured.pvt.pressure = 0;
		}
		else{
			measured.pvt.pressure -= PRESSURE_RANGE_LOW;
		}
#if defined( SG_ENABLE_MEASURE_CHK_LIMIT_P )
		if( get_pressgre_range() == PRESSURE_RANGE_100_1400_KPA ) {
			if( measured.pvt.pressure > (PRESSURE_RANGE_HIGH_TRUCK-PRESSURE_RANGE_LOW) ) {
				measured.pvt.pressure = PRESSURE_RANGE_HIGH_TRUCK;
			}
		}
		else {
			if( measured.pvt.pressure > (PRESSURE_RANGE_HIGH-PRESSURE_RANGE_LOW) ) {
				measured.pvt.pressure = PRESSURE_RANGE_HIGH;
			}
		}
#endif	// defined( SG_ENABLE_MEASURE_CHK_LIMIT_P )
	}
	else {   /* some conversion error happened */
		Adc_ClearAllErrors();      /* Clear all possible ADC errors */
		measurements_valid &= ~DET_PVT;
#if 0	// @2024/06/13
	#ifdef SG_ENABLE_NEW_MEASURE_REC
		SP_measured_rec.pressure = measured.pvt.pressure;
	#else
		memcpy(&measured.pvt, &SP_measured_rec.pvt, sizeof(T_TPMS_DATA_PVT));
	#endif
#else
		measured.pvt.pressure = PKG_PRESSURE_INVALID;
		measured.pvt.voltage = PKG_VOLTAGE_INVALID;
		measured.pvt.temperature = PKG_TEMPERATURE_INVALID;

#endif
	}
}   /* Measure_PVT */



void Measure_ACC(void){

    if(measurements_valid & DET_ACC)
    	return;

	measured.acceleration_x = Compensated_Acceleration_Measurement_X();
	measured.acceleration_z = Compensated_Acceleration_Measurement_Z();
#if 0
	if( measured.acceleration_z < 0 )
		measured.acceleration_z = 0 - measured.acceleration_z;
#endif
    measurements_valid |= DET_ACC;
}



/* ----------------------------------------------------------------------------
 * EOF */
