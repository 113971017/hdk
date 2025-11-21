/*
 * Acceleration sensor diagnostic example
 *
 * Copyright (C) 2018-2022 Melexis N.V.
 *
 */

/* Main function generates different number of pulses on the DEBUG pin (GPIO0)
 * depending on the acceleration sensor state
 *
 * Single Z axis sensor:                * Single X axis sensor:
 * Z = OK/Not_tested    - 1 pulse       * X=OK/Not_tested   - 3 pulses
 * Z = NOK              - 2 pulses      * X=NOK             - 6 pulses
 *
 * Dual XZ acceleration sensor:
 * Z=OK/Not_tested, X=OK/Not_tested - 1+3=4 pulses
 * Z=OK/Not_tested, X=NOK           - 1+6=7 pulses
 * Z=NOK,           X=OK/Not_tested - 2+3=5 pulses
 * Z=NOK,           X=NOK           - 2+6=8 pulses
 *
 * The test is repeated after any wake-up from the WUT
 */

#include <sensorlib.h>
#include <debug.h>

#define MIN_DX_X    (400u)
#define MAX_DX_X    (2500u)

#define MIN_DX_Z    (350u)
#define MAX_DX_Z    (3400u)

#define MIN_SENS_X  (125u)
#define MAX_SENS_X  (148u)

#define MIN_SENS_Z  (92u)
#define MAX_SENS_Z  (136u)

#if defined (HAS_ACCEL_X_SENSOR)
static INLINE bool X_accel_can_be_tested(void);
#endif
#if defined (HAS_ACCEL_Z_SENSOR)
static INLINE bool Z_accel_can_be_tested(void);
#endif

/*
 * Main
 */
int main(void) {

    if (Sys_IsColdBoot()) { /* Cold bootstrap */
        WakeupTimer_Restart(WT_PERIOD_1_S);
        DEBUG_PIN_INIT(LOGIC_LOW);
    }
    else {  /* Warm bootstrap */
        WakeupTimer_IntClear();
        /* nvRAM recall is STRICTLY needed before using functions
         * Check_IsBadAccelZ or Check_IsBadAccelX ! */
        NvRam_Recall();
    }
    
#if defined (HAS_ACCEL_Z_SENSOR)
    /* Z-axis acceleration sensor is assigned in profile */
    /* for Z sensitive axis of Z / XZ accelerometer */
    if (Z_accel_can_be_tested() == true) {
        if (Check_IsBadAccelZ(MIN_DX_Z, MAX_DX_Z) == true) {
            /* Z axis of the acceleration sensor failed */
            DEBUG_PIN_PULSE(2u);
        }
        else {
            /* Z axis of the acceleration sensor is Ok */
            DEBUG_PIN_PULSE(1u);
        }
    }
    else {  /* Sensor cannot be tested */
        DEBUG_PIN_PULSE(1u);
    }
#endif

#if defined (HAS_ACCEL_X_SENSOR)
    /* X-axis acceleration sensor is assigned in profile */
    /* for X sensitive axis of X / XZ accelerometer */
    if (X_accel_can_be_tested() == true) {
        if (Check_IsBadAccelX(MIN_DX_X, MAX_DX_X) == true) {
            /* X axis of the acceleration sensor failed */
            DEBUG_PIN_PULSE(6u);
        }
        else {
            /* X axis of the acceleration sensor is Ok */
            DEBUG_PIN_PULSE(3u);
        }
    }
    else {  /* Sensor cannot be tested */
        DEBUG_PIN_PULSE(3u);
    }
#endif

    Sys_DeepSleepWith(LF_DISABLED);

    return 0;
}

#if defined (HAS_ACCEL_X_SENSOR)

extern volatile uint8_t accel_cal_gain_x  __attribute__((nodp, addr(0x1064)));

static INLINE bool X_accel_can_be_tested(void) {
    bool X_in_range_flag;
    if ((accel_cal_gain_x < MIN_SENS_X) || (accel_cal_gain_x > MAX_SENS_X)) {
        X_in_range_flag = false;
    }
    else {
        X_in_range_flag = true;
    }
    return X_in_range_flag;
}

#endif

#if defined (HAS_ACCEL_Z_SENSOR)

extern volatile uint8_t accel_cal_gain_z  __attribute__((nodp, addr(0x1065)));

static INLINE bool Z_accel_can_be_tested(void) {
    bool Z_in_range_flag;
    if ((accel_cal_gain_z < MIN_SENS_Z) || (accel_cal_gain_z > MAX_SENS_Z)) {
        Z_in_range_flag = false;
    }
    else {
        Z_in_range_flag = true;
    }
    return Z_in_range_flag;
}

#endif

/* EOF */
