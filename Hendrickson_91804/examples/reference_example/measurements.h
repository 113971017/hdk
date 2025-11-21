/*
 * Copyright (C) 2018-2019 Melexis N.V.
 *
 * Software Platform
 *
 */
#ifndef MEASUREMENTS_H
#define MEASUREMENTS_H

extern int16_t Compensated_Acceleration_Measurement_X(void);
extern int16_t Compensated_Acceleration_Measurement_Z(void);
extern void Direct_Motion_Detection(void);
extern void Measure_PVT(void);

/* ----------------------------------------------------------------------------
 */
#endif /* MEASUREMENTS_H */
