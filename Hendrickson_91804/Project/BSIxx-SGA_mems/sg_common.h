#ifndef SG_COMMON_H
#define SG_COMMON_H
#include <platformlib.h>
#include <sensorlib.h>
#include <stdlib.h>
#include "datatypes.h"


#if defined(SG_DEFAULT_CRYSTAL_8MHZ)
	#define SG_DELAY_NOPS_1_US		(2u)    						/* 1us @ 8 MHz */
	#define SG_DELAY_NOPS_1_MS		(1000u * SG_DELAY_NOPS_1_US)	/* 1ms @ 8 MHz */

	/* Wake up timer interval:
	(WUPTMR_MANT + 1) milli-seconds for WUPTMR_EXP = 0
	(WUPTMR_MANT + 257) * 2^(WUPTMR_EXP-1) milli-seconds for WUPTMR_EXP > 0
	(WUPTMR_EXP maximized to 12) */
	#define MANT_50ms   (50-1)
	#define EXP_50ms    0
	#define MANT_100ms  (100-1)
	#define EXP_100ms   0
	#define MANT_250ms  (250-1)
	#define EXP_250ms   0
	#define MANT_500ms  (MANT_1s)
	#define EXP_500ms   (EXP_1s - 1)
	#define MANT_1s     243
	#define EXP_1s      2
	#define MANT_2s     (MANT_500ms)
	#define EXP_2s      (EXP_1s + 1)
	#define MANT_3s     118
	#define EXP_3s      4
	#define MANT_4s     (MANT_1s)
	#define EXP_4s      (EXP_1s + 2)
#if defined(SG_ENABLE_TIMER_CALIBRATION)	//(50+257)*2^4=4912
	#define MANT_5s     50
	#define EXP_5s      5
#else
	#define MANT_5s     55
	#define EXP_5s      5
#endif
	#define MANT_6s     (MANT_3s)
	#define EXP_6s      (EXP_3s + 1)
	#define MANT_7s     180
	#define EXP_7s      5
	#define MANT_8s     (MANT_1s)
	#define EXP_8s      (EXP_1s + 3)
	#define MANT_9s     24
	#define EXP_9s      6
	#define MANT_10s    (MANT_5s)
	#define EXP_10s     (EXP_5s + 1)
	#define MANT_11s    87
	#define EXP_11s     6
	#define MANT_12s    (MANT_3s)
	#define EXP_12s     (EXP_3s + 2)
	#define MANT_13s    149
	#define EXP_13s     6
	#define MANT_14s    (MANT_7s)
	#define EXP_14s     (EXP_7s + 1)
	#define MANT_15s    212
	#define EXP_15s     6
	#define MANT_16s    (MANT_1s)
	#define EXP_16s     (EXP_1s + 4)
	#define MANT_20s    (MANT_5s)
	#define EXP_20s     (EXP_5s + 2)
	#define MANT_30s    (MANT_15s)
	#define EXP_30s     (EXP_15s + 1)
	#define MANT_60s    (MANT_15s)
	#define EXP_60s     (EXP_15s + 2)
#endif


#define DELAY_US(delayUs) do{ \
			delay_nops( delayUs*SG_DELAY_NOPS_1_US ); \
		} while (0)

extern void delay_ms(uint16_t delayMs);
extern void wtimer_restart_ms(uint16_t periodMs);
extern void wtimer_restart_sec(uint16_t periodSec);

extern uint16_t get_cmd_pressure(uint16_t press);
extern uint8_t get_cmd_temperature(uint8_t temp);
extern uint8_t get_cmd_voltage(uint16_t volt);
extern uint8_t get_pressgre_range(void);
#endif
