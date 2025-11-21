/*
 * Copyright (C) 2018-2022 Melexis N.V.
 *
 * Software Platform
 *
 */

#include <sensorlib.h>
#include <debug.h>

#include <datatypes.h>

int16_t Compensated_Acceleration_Measurement_X(void);
int16_t Compensated_Acceleration_Measurement_Z(void);

void Direct_Motion_Detection(void);

void Measure_PVT(void);

STATIC INLINE void Process_Pressure(void);
STATIC INLINE void Process_Voltage(void);
STATIC INLINE void Process_Temperature(void);

#define Get_direct_acceleration()  Compensated_Acceleration_Measurement_Z()

SP_MEM uint16_t SP_last_measured_pressure;  /*@@@ to be optimized till size uint8_t */
SP_MEM uint16_t SP_last_measured_voltage;   /*@@@ to be optimized till size uint8_t */
SP_MEM uint8_t  SP_last_measured_temperature;

DP_MEM uint16_t Pressure;
DP_MEM uint16_t Voltage;
DP_MEM int16_t  Acceleration;
DP_MEM uint8_t  Temperature;

/* ----------------------------------------------------------------------------
 * Implement Compensated Acceleration Measurement_X (CAM)
 * Arguments:   none
 * Returns:     16-bit signed result with acceleration expressed in [g*10]
 *              global bool Acceleration_measurements_valid
 * Notes:
 */
int16_t Compensated_Acceleration_Measurement_X(void)
{   /* for X sensitive axis of X / XZ accelerometer */
    #if defined(HAS_ACCEL_X_SENSOR)

    /* Perform compensated acceleration measurements (cast to fit into container type) */
    int16_t acceleration_result = (uint16_t)Phys_AccelerationX();
    if (!Adc_IsError())     /* if no error  .. */
    {   /* Convert to physical units if no error happened: */
        Acceleration_measurements_valid = true;
    }
    else
    {   /* some conversion error happened */
        acceleration_result = ACCELERATION_INVALID;
        Adc_ClearAllErrors();      /* Clear all possible ADC errors */
	    Acceleration_measurements_valid = false;
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
 *              global bool Acceleration_measurements_valid
 * Notes:
 */
int16_t Compensated_Acceleration_Measurement_Z(void)
{   /* for Z sensitive axis of Z / XZ accelerometer */
    #if defined(HAS_ACCEL_Z_SENSOR)

    /* Perform compensated acceleration measurements (cast to fit into container type) */
    int16_t acceleration_result = (uint16_t)Phys_AccelerationZ();
    if (!Adc_IsError())     /* if no error  .. */
    {   /* Convert to physical units if no error happened: */
        Acceleration_measurements_valid = true;
    }
    else
    {   /* some conversion error happened */
        acceleration_result = ACCELERATION_INVALID;
        Adc_ClearAllErrors();      /* Clear all possible ADC errors */
	    Acceleration_measurements_valid = false;
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
    int16_t raw;

    raw = Get_direct_acceleration();

    if (raw > HIGH_CENTRIPETAL_SIGNAL_THRESHOLD)
    {
        SP_state.as_field.rotation = ROTATING;  /* Wheel is rotating */
    }
    else if (raw < LOW_CENTRIPETAL_SIGNAL_THRESHOLD)
    {
        SP_state.as_field.rotation = STOPPED;   /* Wheel is not rotating */
    }
    else { /* Keep previous motion state */ }
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

    Adc_ReadPVT(&raw_pvt);

    if (!Adc_IsError())     /* if no error  .. */
    {   /* Convert to physical units if no error happened: */
        /* Pressure in [kPa] equals to high word of "Calc_Pressure_32b" */
        Pressure = (uint16_t)(Calc_Pressure(raw_pvt.pressure, raw_pvt.temperature, raw_pvt.voltage) >> 16);
        Process_Pressure();
        /* Battery voltage in [V] equals to tmp16 divided by 100 */
        Voltage = Calc_Voltage(raw_pvt.voltage);
        Process_Voltage();
        /* Temperature in [degC] equals to high byte of "Calc_Temperature_16b" - 52degC */
        Temperature = (uint8_t)(Calc_Temperature(raw_pvt.temperature) >> 8);
        Process_Temperature();
        PVT_measurements_valid = true;
        DEBUG_PUT_STRING("PVT.done ");
    }
    else
    {   /* some conversion error happened */
        Adc_ClearAllErrors();      /* Clear all possible ADC errors */
        PVT_measurements_valid = false;
        DEBUG_PUT_STRING("PVT.err ");
    }
}   /* Measure_PVT */

/* ----------------------------------------------------------------------------
 * Process Pressure
 * Arguments:	none
 * Returns:     none
 * Notes:       The function has to be defined by customer's application if needed
 */
STATIC INLINE void Process_Pressure(void)
{
    uint16_t Pressure_change;

    if (Pressure > SP_last_measured_pressure)
    {
        Pressure_change = Pressure - SP_last_measured_pressure;
    }
    else
    {
        Pressure_change = SP_last_measured_pressure - Pressure;
    }
    SP_last_measured_pressure = Pressure;
}   /* Process_Pressure */

/* ----------------------------------------------------------------------------
 * Process Voltage
 * Arguments:	none
 * Returns:     none
 * Notes:       The function has to be defined by customer's application if needed
 */
STATIC INLINE void Process_Voltage(void)
{
    uint16_t Voltage_change;

    if (Voltage > SP_last_measured_voltage)
    {
        Voltage_change = Voltage - SP_last_measured_voltage;
    }
    else
    {
        Voltage_change = SP_last_measured_voltage - Voltage;
    }
    SP_last_measured_voltage = Voltage;
}   /* Process_Voltage */

/* ----------------------------------------------------------------------------
 * Process Temperature
 * Arguments:	none
 * Returns:     none
 * Notes:       The function has to be defined by customer's application if needed
 */
STATIC INLINE void Process_Temperature(void)
{
    uint16_t Temperature_change;

    if (Temperature > SP_last_measured_temperature)
    {
        Temperature_change = Temperature - SP_last_measured_temperature;
    }
    else
    {
        Temperature_change = SP_last_measured_temperature - Temperature;
    }
    SP_last_measured_temperature = Temperature;
}   /* Process_Temperature */

/* ----------------------------------------------------------------------------
 * EOF */
