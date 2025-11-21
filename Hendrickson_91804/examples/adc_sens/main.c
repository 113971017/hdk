/*
 * Copyright (C) 2016-2022 Melexis N.V.
 *
 * Software Platform
 */

#include <sensorlib.h>
#include <platformlib.h>
#include <debug.h>

STATIC uint16_t raw_t;
STATIC uint16_t raw_p;
STATIC uint16_t raw_v;

STATIC uint32_t processed_p;
STATIC uint16_t processed_t;
STATIC uint16_t processed_v;

STATIC int16_t processed_accel_x;
STATIC int16_t processed_accel_z;

#define EXECUTE_SEPARATED_PVT_MEASUREMENTS
#define EXECUTE_COMBINED_PVT_MEASUREMENTS
#define EXECUTE_ACCELERATION_MEASUREMENTS

#define USE_SLEEP_FOR_DELAYS

//#define HAS_CUSTOM_PRESSURE_AVERAGING

//#define HAS_CUSTOM_VBAT_AVERAGING

//#define HAS_CUSTOM_TEMPERATURE_AVERAGING

#define HAS_CUSTOM_ACCEL_AVERAGING
#define ACCEL_AVERAGING_X   (1u)
#define ACCEL_AVERAGING_Z   (8u)
/* Global variable defining the averaging deepness for the acceleration measurements
 * in case of defined HAS_CUSTOM_ACCEL_AVERAGING */
STATIC uint16_t accel_averaging;

/*
 * main
 */
int main (void)
{
    if (Sys_IsColdBoot()) {         /* in case of cold boot .. */
        #if defined(USE_SLEEP_FOR_DELAYS)
        WakeupTimer_Restart(WT_PERIOD_5_MS);
        #endif
        DEBUG_PIN_INIT(LOGIC_LOW);
    }
    else {                          /* NVRAM is recalled automatically at cold boot */
        NvRam_Recall();             /* "Calc_xxxx" functions use calibration parameters from NVRAM */
        #if defined(USE_SLEEP_FOR_DELAYS)
        WakeupTimer_IntClear();     /* clean wake-up & interrupt flags; interval is autoreloaded => exact interval */
        #endif
    }

    while (true)
    {
        #if defined(EXECUTE_SEPARATED_PVT_MEASUREMENTS)
        /* Separate PVT measurements */
        DEBUG_PIN_PULSE(2u);

        raw_p = Adc_ReadPressure();
        raw_v = Adc_ReadVoltage();
        raw_t = Adc_ReadTemperature();

        if( !Adc_IsError() )
        {   /* if no error reported .. */
            processed_p = Calc_Pressure(raw_p, raw_t, raw_v);
            processed_t = Calc_Temperature(raw_t);
            processed_v = Calc_Voltage(raw_v);
        }
        else {
            /* .. error handler */
        }
        #endif /* EXECUTE_SEPARATED_PVT_MEASUREMENTS */

        #if defined(EXECUTE_COMBINED_PVT_MEASUREMENTS)
        /* Combined PVT measurements to optimize power consumption */
        Adc_RawPvt_Type raw_pvt;
        DEBUG_PIN_PULSE(2u);
        Adc_ReadPVT(&raw_pvt);
        if( !Adc_IsError() )
        {  /* if no error reported .. */
            processed_p = Calc_Pressure(raw_pvt.pressure,
                                        raw_pvt.temperature,
                                        raw_pvt.voltage);
            processed_t = Calc_Temperature(raw_pvt.temperature);
            processed_v = Calc_Voltage(raw_pvt.voltage);
        }
        else
        {
            /* .. error handler */
        }
        #endif /* EXECUTE_COMBINED_PVT_MEASUREMENTS */

        #if defined(EXECUTE_ACCELERATION_MEASUREMENTS)
        /* Digitizing of the acceleration sensor signal */

        #if defined(HAS_ACCEL_X_SENSOR)
        DEBUG_PIN_PULSE(2u);
        accel_averaging = ACCEL_AVERAGING_X;
        processed_accel_x = Phys_AccelerationX();
        #endif

        #if defined(HAS_ACCEL_Z_SENSOR)
        DEBUG_PIN_PULSE(2u);
        accel_averaging = ACCEL_AVERAGING_Z;
        processed_accel_z = Phys_AccelerationZ();
        #endif

        #endif /* EXECUTE_ACCELERATION_MEASUREMENTS */

        DEBUG_PIN_PULSE(2u);

        #if defined(USE_SLEEP_FOR_DELAYS)
        Sys_DeepSleepWith(LF_DISABLED);
        #else
        delay_nops(3u * 2000u); /* 3 ms @ 8 MHz */
        /* Feed the watchdog if needed: */
        if (AbsWatchdog_IsWindowOpen()) {
            AbsWatchdog_Restart();
        }
        /* else: watchdog window is not open yet */
        #endif /* USE_SLEEP_FOR_DELAYS */
    }

    return 0;
}

/*--------------------------------------------------------------------------*/

#if defined (HAS_CUSTOM_PRESSURE_AVERAGING)

extern uint16_t adc_ll_read_pressure_once(void);
extern uint16_t adc_ll_read_pressure(void);

#define  ADC_PRESSURE_VALUES    (2u)

/*
    Override the library version of the function adc_ll_read_pressure()
    by the custom one below that implements a multiple ADC measurement
    with the next averaging
*/

uint16_t adc_ll_read_pressure(void) {

    uint16_t press;
    uint32_t press_accumulator = 0u;

    for (uint16_t i = 0u; i < ADC_PRESSURE_VALUES; i++) {
        press_accumulator += adc_ll_read_pressure_once();
        DEBUG_PIN_PULSE(1u);
    }
    /* Because ADC_PRESSURE_VALUES = 2u, the compiler will replace
     * the divide below by the right shift */
    press = press_accumulator / ADC_PRESSURE_VALUES;  

    return press;
}
#endif /* HAS_CUSTOM_PRESSURE_AVERAGING */

/*--------------------------------------------------------------------------*/

#if defined (HAS_CUSTOM_VBAT_AVERAGING)

extern uint16_t adc_ll_read_vbat_once(void);
extern uint16_t adc_ll_read_vbat(void);

#define  ADC_VBAT_VALUES        (3u)

/*
    Override the library version of the function adc_ll_read_vbat()
    by the custom one below that implements a multiple ADC measurement
    with the next averaging
*/

uint16_t adc_ll_read_vbat(void) {

    uint16_t vbat;
    uint32_t vbat_accumulator = 0u;

    for (uint16_t i = 0u; i < ADC_VBAT_VALUES; i++) {
        vbat_accumulator += adc_ll_read_vbat_once();
        DEBUG_PIN_PULSE(1u);
    }
    vbat = divU16_U32byU16(vbat_accumulator, ADC_VBAT_VALUES);

    return vbat;
}
#endif /* HAS_CUSTOM_VBAT_AVERAGING */

/*--------------------------------------------------------------------------*/

#if defined (HAS_CUSTOM_TEMPERATURE_AVERAGING)

extern uint16_t adc_ll_read_voltage_once(void);
extern uint16_t adc_ll_read_temperature(void);

#define  ADC_TEMPERATURE_VALUES (4u)

/*
    Override the library version of the function adc_ll_read_temperature()
    by the custom one below that implements a multiple ADC measurement
    with the next averaging
*/

uint16_t adc_ll_read_temperature(void) {

    uint16_t temp;
    uint32_t temp_accumulator = 0u;

    for (uint16_t i = 0u; i < ADC_TEMPERATURE_VALUES; i++) {
        temp_accumulator += adc_ll_read_voltage_once();
        DEBUG_PIN_PULSE(1u);
    }
    /* Because ADC_TEMPERATURE_VALUES = 4u, the compiler will replace
     * the divide below by the right shift */
    temp = temp_accumulator / ADC_TEMPERATURE_VALUES; 

    return temp;
}
#endif /* HAS_CUSTOM_TEMPERATURE_AVERAGING */

/*--------------------------------------------------------------------------*/

#if defined (HAS_CUSTOM_ACCEL_AVERAGING)

extern int16_t adc_ll_read_accel_once(void);
extern int16_t adc_ll_read_accel(void);

/*
    Override the library version of the function adc_ll_read_accel()
    by the custom one below that implements a multiple ADC measurement
    with the next averaging
*/

int16_t adc_ll_read_accel(void) {

    int16_t accel;
    int32_t accel_accumulator = 0u;

    for (uint16_t i = 0u; i < accel_averaging; i++) {
        accel_accumulator += adc_ll_read_accel_once();
        DEBUG_PIN_PULSE(1u);
    }
    accel = divI16_I32byU16(accel_accumulator, accel_averaging);

    return accel;
}
#endif /* HAS_CUSTOM_ACCEL_AVERAGING */

/* EOF */
