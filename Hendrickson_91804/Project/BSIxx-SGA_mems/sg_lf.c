#include <sensorlib.h>
#include <platformlib.h>

#include "WuTimerC.h"
#include "main.h"
#include "sg_lf.h"
#include "datatypes.h"
#include "sg_common.h"
#include "sg_dbg.h"


//unsigned FLfRecv[16];

#ifdef LF_COMPONENT_ENABLE
static const LfConfig_Type CLfCfg =
 {{
		 /* TX config */
		     .LFTX_MODE  = LFTX_MANCHESTER_LSB_FIRST,    /* 3 bits defining the LFTx modulation source */
		     .LFTX_POL   = LFTX_POLARITY_1,              /* LFTx polarity */

		 /* RX config */
		     /* LFRx control */
		     .LF_EN_FD       = 0,        /* 1 bit: being set it enables LFRX fault detection mechanism */
		     .LFRX_STBY_ON   = 0,        /* 1 bit: being set it keeps LF analog interface enabled during standby phase of RX polling */
		     /* LFRX standby interval (single shot if LFRX_STBY_MANT = 0 and LFRX_STBY_EXP = 0 */
		     /* If LFRX_STBY_EXP = 0, Standby interval [ms] =  LFRX_STBY_MANT */
		     /* If LFRX_STBY_EXP > 0, Standby interval [ms] = (LFRX_STBY_MANT+16) * 2^(LFRX_STBY_EXP-1) */
#if 1 //org(BSI-15)
		     .LFRX_STBY_EXP  = 1,	     /* 4-bit exponent and 4-bit mantissa of the Standby interval */
		     .LFRX_STBY_MANT = 2,	     /* The pair {0,13} corresponds to 11 ms */
		     /* Timeout interval limiting preamble detection duration (no timeout, if both mantissa and exponent are zero */
		     /* If LFRX_TIMEOUT_EXP = 0, Timeout [ms] =  LFRX_TIMEOUT_MANT */
		     /* If LFRX_TIMEOUT_EXP > 0, Timeout [ms] = (LFRX_TIMEOUT_MANT+16) * 2^(LFRX_TIMEOUT_EXP-1) */
		     .LFRX_TIMEOUT_EXP   = 0,    /* 4-bit exponent and 4-bit mantissa of the timeout interval */
		     .LFRX_TIMEOUT_MANT  = 1,    /* The pair {3,9} corresponds to 100 ms */
#else // BSIxx-SGA
		     .LFRX_STBY_EXP  = 3,	     /* 4-bit exponent and 4-bit mantissa of the Standby interval */
		     .LFRX_STBY_MANT = 9,	     /* The pair {0,13} corresponds to 11 ms */
		     .LFRX_TIMEOUT_EXP   = 3,    /* 4-bit exponent and 4-bit mantissa of the timeout interval */
		     .LFRX_TIMEOUT_MANT  = 2,    /* The pair {3,9} corresponds to 100 ms */
#endif
		     .LF_GAIN_SEL    = LF_GAIN_MAX,  /* Gain setting for LF analog front-end: LF_GAIN_MAX or LF_GAIN_MIN */

		     /* Manchester parameters */
		     .LFRX_ORDER     = LFRX_ORDER_MSB_FIRST, /* LF RX bit order inside of any byte: 0 = LSB first, 1 = MSB first */
		     .LFRX_POL       = 0,        /* LFRx polarity: if it's zero => high-to-low corresponds to Manchester's "0" */

		     /* Preamble validation */
		     .LFRX_PREAMBLE  = 1,        /* 1 bit to enable preamble validation: "1" to enable, "0" to disable */
		     .LFRX_LONGCNT   = 1,        /* 1 bit specifying a number of 32 kHz periods to validate preamble: "0" - 5 cycles, "1" - 10 cycles */
		     .LFRX_MINCNT    = 22,       /* 6-bit minimum counter value for valid preamble */
		     .LFRX_DELTACNT  = 20,       /* 6-bit (maximum - minimum) counter value for valid preamble */

		     /* Sync word */
		     .LFRX_SYNCLEN       = 9,    /* 4-bit LF RX synchronization word length, up to 16 SYNC DIGITs */
		     .LFRX_SYNCPOL       = 1,    /* Synchronization pattern starts with falling (0) or rising (1) edge */
		     /* 2-bits durations of LFRX synchronization pattern's digits (in Tsymbols): 0 = 0.5, 1 = 1, 2 = 1.5, 3 = 2 */
		     .LFRX_SYNC_DIGIT1   = 0,    /* last     digit  duration, default value = 0 */
		     .LFRX_SYNC_DIGIT2   = 1,    /* last - 1 digit  duration, default value = 1 */
		     .LFRX_SYNC_DIGIT3   = 1,    /* last - 2 digit  duration, default value = 1 */
		     .LFRX_SYNC_DIGIT4   = 1,    /* last - 3 digit  duration, default value = 1 */
		     .LFRX_SYNC_DIGIT5   = 1,    /* last - 4 digit  duration, default value = 1 */
		     .LFRX_SYNC_DIGIT6   = 0,    /* last - 5 digit  duration, default value = 0 */
		     .LFRX_SYNC_DIGIT7   = 0,    /* last - 6 digit  duration, default value = 0 */
		     .LFRX_SYNC_DIGIT8   = 2,    /* last - 7 digit  duration, default value = 2 */
		     .LFRX_SYNC_DIGIT9   = 2,    /* last - 8 digit  duration, default value = 2 */
		     .LFRX_SYNC_DIGIT10  = 0,    /* last - 9 digit  duration, default value = 0 */
		     .LFRX_SYNC_DIGIT11  = 0,    /* last - 10 digit duration, default value = 0 */
		     .LFRX_SYNC_DIGIT12  = 0,    /* last - 11 digit duration, default value = 0 */
		     .LFRX_SYNC_DIGIT13  = 0,    /* last - 12 digit duration, default value = 0 */
		     .LFRX_SYNC_DIGIT14  = 0,    /* last - 13 digit duration, default value = 0 */
		     .LFRX_SYNC_DIGIT15  = 0,    /* last - 14 digit duration, default value = 0 */

		     /* Header */
		     .LFRX_HDRLEN= LFRX_HDR_16_BITS,     /* LF RX header length: "0" - no header, "1" - 1 byte, "2" - 2 bytes, "3" - 4 bytes of header */
#if 0 //org
		     .LFRX_HDR_L = 0x475e,   /* Lowest  16-bit header to be recognized in LFRx with LFRX_HDRLEN = LFRX_HDR_16_BITS */
		     .LFRX_HDR_H = 0x475e,   /* Highest 16-bit header to be recognized in LFRx with LFRX_HDRLEN = LFRX_HDR_16_BITS */
		     /* 32-bit mask for header to be recognized in LF RX. Each bit of the mask means: "0" - don't care, "1" - to be matched */
		     .LFRX_HDR_MASK_L    = 0xFFFF,       /* The mask for the lowest  16-bit header for LFRX_HDRLEN = LFRX_HDR_16_BITS */
		     .LFRX_HDR_MASK_H    = 0xFFFF        /* The mask for the highest 16-bit header for LFRX_HDRLEN = LFRX_HDR_16_BITS => disabled */
#else
			 .LFRX_HDR_L = SG_LFRX_HDR_L,   /* Lowest  16-bit header to be recognized in LFRx with LFRX_HDRLEN = LFRX_HDR_16_BITS */
			 .LFRX_HDR_H = SG_LFRX_HDR_H,   /* Highest 16-bit header to be recognized in LFRx with LFRX_HDRLEN = LFRX_HDR_16_BITS */
		     .LFRX_HDR_MASK_L    = 0xFFFF,       /* The mask for the lowest  16-bit header for LFRX_HDRLEN = LFRX_HDR_16_BITS */
		     .LFRX_HDR_MASK_H    = 0x0000        /* The mask for the highest 16-bit header for LFRX_HDRLEN = LFRX_HDR_16_BITS => disabled */
#endif

 }};




void init_lf ( void )
{
	DBG("init_lf\n");
	Lf_Init(&CLfCfg);

#if 0
	Itc_ClearPending(ITC_LFRX_TIMEOUT);
	Itc_Enable(ITC_LFRX_TIMEOUT);
	Wakeup_Enable(WAKEUP_LFRX_TIMEOUT);
	Wakeup_Enable(WAKEUP_LF_DONE);
#endif

	 // 1. Disable LF interrupts
	    Itc_Disable(ITC_LFRX_TIMEOUT);
	    Itc_ClearPending(ITC_LFRX_TIMEOUT);

	    // 2. Disable wakeup sources related to LF
	    Wakeup_Disable(WAKEUP_LFRX_TIMEOUT);
	    Wakeup_Disable(WAKEUP_LF_DONE);

	    // 3. disable the LF module
	    Lf_Disable();  // Lf_Disable() depending on API

}

#endif
