/*
 * Copyright (C) 2018-2019 Melexis N.V.
 *
 * Software Platform
 *
 */
#ifndef PROCESSING_H
#define PROCESSING_H

extern void TPMS_state_Measurements(void);
extern void TPMS_state_Motion_Detection(void);
extern void TPMS_state_processing_and_update(void);
extern void TPMS_state_processing_end(void);
extern void TPMS_state_Init(tpms_state_t state);

extern RODATA_MEM tpms_state_property_t TPMS_STATE_PARAMETERS[NUMBER_OF_TPMS_STATES];

/* ----------------------------------------------------------------------------
 */
#endif /* PROCESSING_H */
