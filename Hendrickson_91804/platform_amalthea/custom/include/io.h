/*
 * Copyright (C) 2018 Melexis N.V.
 */

/*
 * Automatic generation by MlxCCT 1.5.0
 * Template version 2.8.4
 *
 * File sections:
 * 1. General for use macros.
 * 2. Auxillary macros.
 * 3. I/O declarations:
 * 3.1 Hosting variables.
 * 3.2 Get macros.
 * 3.3 Ports.
 * 3.4 Dummy ports.
 * 4. Statistics
 */

#ifndef IO_H
#define IO_H

#include <stdint.h>

/*
	Define overflow behaviour in IO_SET()

	Note:
	-	If NOT disabled by the HAS_NO_IO_SET_TRUNCATION option the oversized value
		will be truncated (by modulo of the field size, i.e. mask) before writing
		into the field. This safe approach replicates bit-field behaviour and
		guarantees that the neighbour fields will not be corrupted by the oversized
		value.
	-	Define '-DHAS_NO_IO_SET_TRUNCATION' in the compiler flags to disable
		explicit truncation. WARNING: In this case it's full responsibility of the
		user to guarantee that value will not be oversized.
*/
#if !defined (HAS_NO_IO_SET_TRUNCATION)
	#define CCT_IO_TRUNC_MASK(B, F)             (CCT_IO_MASK(B, F) | CCT_IO_HOST_UNUSED(B, F))
    #define CCT_IO_TRUNC_MASK_WITHIN_BYTE(B, F)	(CCT_IO_MASK_WITHIN_BYTE(B, F)         | CCT_IO_HOST_UNUSED_WITHIN_BYTE(B, F))
#else
	#define CCT_IO_TRUNC_MASK(B, F)	            (0xFFFF)
    #define CCT_IO_TRUNC_MASK_WITHIN_BYTE(B, F)	(0xFF)
#endif

/*****************************************************************************/
/* 1. General macros. */

/*
 * Get an I/O bit field.
 * @param BLOCK is the digital block holding the given I/O field.
 * @param FIELD is the field to read.
 */

#ifndef IO_GET
#define IO_GET(BLOCK, FIELD) IO_ ## BLOCK ## __ ## FIELD ## _GET ()
#endif

/*
 * Set one or multiple I/O bit fields (ports) to some values. The fields set
 * have to be more than one, but less than 16.
 *
 * @param BLOCK is the digital block holding the given port.
 * @param FIELD the port to set.
 * @param VALUE the value of the port to set (should be in the value's range).
 */

#ifndef IO_SET
#define IO_SET(BLOCK, ...) \
	CCT_IO_SET (IO_ ## BLOCK, __VA_ARGS__, \
		DUMMY2,  0U, DUMMY3,  0U, DUMMY4,  0U, DUMMY5,  0U,  DUMMY6, 0U, \
		DUMMY7,  0U, DUMMY8,  0U, DUMMY9,  0U, DUMMY10, 0U, DUMMY11, 0U, \
		DUMMY12, 0U, DUMMY13, 0U, DUMMY14, 0U, DUMMY15, 0U, DUMMY16, 0U, \
		DUMMY17, 0U)
#endif

/*
 * Reports what is the first address occupied by I/O variable.
 */

#ifndef IO_ADDRESS_BEGIN
#define IO_ADDRESS_BEGIN (0x0000u)
#endif

/*
 * Reports what is the first address past the last one occupied by I/O variable.
 */

#ifndef IO_ADDRESS_END
#define IO_ADDRESS_END (0x0244u)
#endif

/*****************************************************************************/
/* 2. Auxillary (helper) macros. */
/* These macros are subject to change without notice! */

/* TODO: Optimize to always setb/clrb. */

#define CCT_IO_SET(B, \
	    F1,  V1,  F2,  V2,  F3,  V3,  F4,  V4,  F5,  V5,  F6,  V6,  \
	    F7,  V7,  F8,  V8,  F9,  V9,  F10, V10, F11, V11, F12, V12, \
		F13, V13, F14, V14, F15, V15, F16, V16, F17, V17, \
		...) \
    if (CCT_IO_BIT_ACCESSIBLE (B, \
		F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  \
		F9, F10, F11, F12, F13, F14, F15, F16)) \
	{ \
		if (CCT_IO_BIT_ACCESSIBLE (B, F1)) { \
    		if ((V1 ) != 0u) { CCT_IO_SETB(B, F1);  } \
			else             { CCT_IO_CLRB(B, F1);  } \
		} \
		if (!CCT_IO_DUMMY (B, F2) && CCT_IO_BIT_ACCESSIBLE (B, F2)) { \
    		if ((V2 ) != 0u) { CCT_IO_SETB(B, F2);  } \
			else             { CCT_IO_CLRB(B, F2);  } \
		} \
		if (!CCT_IO_DUMMY (B, F3) && CCT_IO_BIT_ACCESSIBLE (B, F3)) { \
    		if ((V3 ) != 0u) { CCT_IO_SETB(B, F3);  } \
			else             { CCT_IO_CLRB(B, F3);  } \
		} \
		if (!CCT_IO_DUMMY (B, F4) && CCT_IO_BIT_ACCESSIBLE (B, F4)) { \
    		if ((V4 ) != 0u) { CCT_IO_SETB(B, F4);  } \
			else             { CCT_IO_CLRB(B, F4);  } \
		} \
		if (!CCT_IO_DUMMY (B, F5) && CCT_IO_BIT_ACCESSIBLE (B, F5)) { \
    		if ((V5 ) != 0u) { CCT_IO_SETB(B, F5);  } \
			else             { CCT_IO_CLRB(B, F5);  } \
		} \
		if (!CCT_IO_DUMMY (B, F6) && CCT_IO_BIT_ACCESSIBLE (B, F6)) { \
    		if ((V6 ) != 0u) { CCT_IO_SETB(B, F6);  } \
			else             { CCT_IO_CLRB(B, F6);  } \
		} \
		if (!CCT_IO_DUMMY (B, F7) && CCT_IO_BIT_ACCESSIBLE (B, F7)) { \
    		if ((V7 ) != 0u) { CCT_IO_SETB(B, F7);  } \
			else             { CCT_IO_CLRB(B, F7);  } \
		} \
		if (!CCT_IO_DUMMY (B, F8) && CCT_IO_BIT_ACCESSIBLE (B, F8)) { \
    		if ((V8 ) != 0u) { CCT_IO_SETB(B, F8);  } \
			else             { CCT_IO_CLRB(B, F8);  } \
		} \
		if (!CCT_IO_DUMMY (B, F9) && CCT_IO_BIT_ACCESSIBLE (B, F9)) { \
    		if ((V9 ) != 0u) { CCT_IO_SETB(B, F9);  } \
			else             { CCT_IO_CLRB(B, F9);  } \
		} \
		if (!CCT_IO_DUMMY (B, F10) && CCT_IO_BIT_ACCESSIBLE (B, F10)) { \
    		if ((V10) != 0u) { CCT_IO_SETB(B, F10);  } \
			else             { CCT_IO_CLRB(B, F10);  } \
		} \
		if (!CCT_IO_DUMMY (B, F11) && CCT_IO_BIT_ACCESSIBLE (B, F11)) { \
    		if ((V11) != 0u) { CCT_IO_SETB(B, F11);  } \
			else             { CCT_IO_CLRB(B, F11);  } \
		} \
		if (!CCT_IO_DUMMY (B, F12) && CCT_IO_BIT_ACCESSIBLE (B, F12)) { \
    		if ((V12) != 0u) { CCT_IO_SETB(B, F12);  } \
			else             { CCT_IO_CLRB(B, F12);  } \
		} \
		if (!CCT_IO_DUMMY (B, F13) && CCT_IO_BIT_ACCESSIBLE (B, F13)) { \
    		if ((V13) != 0u) { CCT_IO_SETB(B, F13);  } \
			else             { CCT_IO_CLRB(B, F13);  } \
		} \
		if (!CCT_IO_DUMMY (B, F14) && CCT_IO_BIT_ACCESSIBLE (B, F14)) { \
    		if ((V14) != 0u) { CCT_IO_SETB(B, F14);  } \
			else             { CCT_IO_CLRB(B, F14);  } \
		} \
		if (!CCT_IO_DUMMY (B, F15) && CCT_IO_BIT_ACCESSIBLE (B, F15)) { \
    		if ((V15) != 0u) { CCT_IO_SETB(B, F15);  } \
			else             { CCT_IO_CLRB(B, F15);  } \
		} \
		if (!CCT_IO_DUMMY (B, F16) && CCT_IO_BIT_ACCESSIBLE (B, F16)) { \
    		if ((V16) != 0u) { CCT_IO_SETB(B, F16);  } \
			else             { CCT_IO_CLRB(B, F16);  } \
		} \
	} \
    else if (CCT_IO_SAME_BYTE(B, \
    	F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  \
		F9, F10, F11, F12, F13, F14, F15, F16)) \
    { \
        CCT_IO_SET_WITHIN_BYTE(B, \
        F1,  V1,  F2,  V2,  F3,  V3,  F4,  V4,  F5,  V5,  F6,  V6,  \
	    F7,  V7,  F8,  V8,  F9,  V9,  \
        ...) \
    } \
    else {  \
        CCT_IO_SET_WITHIN_WORD(B, \
        F1,  V1,  F2,  V2,  F3,  V3,  F4,  V4,  F5,  V5,  F6,  V6,  \
	    F7,  V7,  F8,  V8,  F9,  V9,  F10, V10, F11, V11, F12, V12, \
		F13, V13, F14, V14, F15, V15, F16, V16, F17, V17, \
        ...) \
    }

#define CCT_IO_SET_WITHIN_WORD(B, \
	    F1,  V1,  F2,  V2,  F3,  V3,  F4,  V4,  F5,  V5,  F6,  V6,  \
	    F7,  V7,  F8,  V8,  F9,  V9,  F10, V10, F11, V11, F12, V12, \
		F13, V13, F14, V14, F15, V15, F16, V16, F17, V17, \
		...) \
	if ((CCT_IO_HOST_UNUSED (B, F1) | CCT_IO_HOST_RISED (B, F1) \
		| CCT_IO_MASK (B, \
					F1, F2, F3, F4, F5, F6, F7, F8, \
					F9, F10, F11, F12, F13, F14, F15, F16)) == 0xFFFFU) {\
		CCT_IO_HOST (B, F1) = 0u \
		| ((((uint16_t)(V1 )) << CCT_IO_OFFSET (B, F1 )) & CCT_IO_TRUNC_MASK(B, F1 ))\
		| ((((uint16_t)(V2 )) << CCT_IO_OFFSET (B, F2 )) & CCT_IO_TRUNC_MASK(B, F2 ))\
		| ((((uint16_t)(V3 )) << CCT_IO_OFFSET (B, F3 )) & CCT_IO_TRUNC_MASK(B, F3 ))\
		| ((((uint16_t)(V4 )) << CCT_IO_OFFSET (B, F4 )) & CCT_IO_TRUNC_MASK(B, F4 ))\
		| ((((uint16_t)(V5 )) << CCT_IO_OFFSET (B, F5 )) & CCT_IO_TRUNC_MASK(B, F5 ))\
		| ((((uint16_t)(V6 )) << CCT_IO_OFFSET (B, F6 )) & CCT_IO_TRUNC_MASK(B, F6 ))\
		| ((((uint16_t)(V7 )) << CCT_IO_OFFSET (B, F7 )) & CCT_IO_TRUNC_MASK(B, F7 ))\
		| ((((uint16_t)(V8 )) << CCT_IO_OFFSET (B, F8 )) & CCT_IO_TRUNC_MASK(B, F8 ))\
		| ((((uint16_t)(V9 )) << CCT_IO_OFFSET (B, F9 )) & CCT_IO_TRUNC_MASK(B, F9 ))\
		| ((((uint16_t)(V10)) << CCT_IO_OFFSET (B, F10)) & CCT_IO_TRUNC_MASK(B, F10))\
		| ((((uint16_t)(V11)) << CCT_IO_OFFSET (B, F11)) & CCT_IO_TRUNC_MASK(B, F11))\
		| ((((uint16_t)(V12)) << CCT_IO_OFFSET (B, F12)) & CCT_IO_TRUNC_MASK(B, F12))\
		| ((((uint16_t)(V13)) << CCT_IO_OFFSET (B, F13)) & CCT_IO_TRUNC_MASK(B, F13))\
		| ((((uint16_t)(V14)) << CCT_IO_OFFSET (B, F14)) & CCT_IO_TRUNC_MASK(B, F14))\
		| ((((uint16_t)(V15)) << CCT_IO_OFFSET (B, F15)) & CCT_IO_TRUNC_MASK(B, F15))\
		| ((((uint16_t)(V16)) << CCT_IO_OFFSET (B, F16)) & CCT_IO_TRUNC_MASK(B, F16))\
			; \
	} \
	else { \
		CCT_IO_HOST (B, F1) = (CCT_IO_HOST (B, F1) \
			& ~(CCT_IO_HOST_RISED (B, F1) | CCT_IO_MASK (B, \
					F1, F2, F3, F4, F5, F6, F7, F8,   \
					F9, F10, F11, F12, F13, F14, F15, F16))) \
		| ((((uint16_t)(V1 )) << CCT_IO_OFFSET (B, F1 )) & CCT_IO_TRUNC_MASK(B, F1 ))\
		| ((((uint16_t)(V2 )) << CCT_IO_OFFSET (B, F2 )) & CCT_IO_TRUNC_MASK(B, F2 ))\
		| ((((uint16_t)(V3 )) << CCT_IO_OFFSET (B, F3 )) & CCT_IO_TRUNC_MASK(B, F3 ))\
		| ((((uint16_t)(V4 )) << CCT_IO_OFFSET (B, F4 )) & CCT_IO_TRUNC_MASK(B, F4 ))\
		| ((((uint16_t)(V5 )) << CCT_IO_OFFSET (B, F5 )) & CCT_IO_TRUNC_MASK(B, F5 ))\
		| ((((uint16_t)(V6 )) << CCT_IO_OFFSET (B, F6 )) & CCT_IO_TRUNC_MASK(B, F6 ))\
		| ((((uint16_t)(V7 )) << CCT_IO_OFFSET (B, F7 )) & CCT_IO_TRUNC_MASK(B, F7 ))\
		| ((((uint16_t)(V8 )) << CCT_IO_OFFSET (B, F8 )) & CCT_IO_TRUNC_MASK(B, F8 ))\
		| ((((uint16_t)(V9 )) << CCT_IO_OFFSET (B, F9 )) & CCT_IO_TRUNC_MASK(B, F9 ))\
		| ((((uint16_t)(V10)) << CCT_IO_OFFSET (B, F10)) & CCT_IO_TRUNC_MASK(B, F10))\
		| ((((uint16_t)(V11)) << CCT_IO_OFFSET (B, F11)) & CCT_IO_TRUNC_MASK(B, F11))\
		| ((((uint16_t)(V12)) << CCT_IO_OFFSET (B, F12)) & CCT_IO_TRUNC_MASK(B, F12))\
		| ((((uint16_t)(V13)) << CCT_IO_OFFSET (B, F13)) & CCT_IO_TRUNC_MASK(B, F13))\
		| ((((uint16_t)(V14)) << CCT_IO_OFFSET (B, F14)) & CCT_IO_TRUNC_MASK(B, F14))\
		| ((((uint16_t)(V15)) << CCT_IO_OFFSET (B, F15)) & CCT_IO_TRUNC_MASK(B, F15))\
		| ((((uint16_t)(V16)) << CCT_IO_OFFSET (B, F16)) & CCT_IO_TRUNC_MASK(B, F16))\
			; \
	} \
	CCT_IO_ASSERT_WRITABLES (B, \
			F1, F2, F3, F4, F5, F6, F7, F8, \
			F9, F10, F11, F12, F13, F14, F15, F16); \
	CCT_IO_ASSERT_SAME_HOST (B, \
			F1, F2, F3, F4, F5, F6, F7, F8, \
			F9, F10, F11, F12, F13, F14, F15, F16); \
	CCT_IO_ASSERT_ARGS_FIT  (B, \
			F1, F2, F3, F4, F5, F6, F7, F8, \
			F9, F10, F11, F12, F13, F14, F15, F16, F17);

#define CCT_IO_SET_WITHIN_BYTE(B, \
	    F1,  V1,  F2,  V2,  F3,  V3,  F4,  V4,  \
        F5,  V5,  F6,  V6,  F7,  V7,  F8,  V8,  F9,  V9,  \
		...) \
	if ((CCT_IO_HOST_UNUSED_WITHIN_BYTE (B, F1) | CCT_IO_HOST_RISED_WITHIN_BYTE (B, F1) \
		| CCT_IO_MASK_WITHIN_BYTE (B, \
					F1, F2, F3, F4, F5, F6, F7, F8)) == 0xFFU) { \
		CCT_IO_BYTE_HOST (B, F1) = (uint8_t)(0u \
    	| (((V1 ) << CCT_IO_OFFSET_WITHIN_BYTE (B, F1 )) & CCT_IO_TRUNC_MASK_WITHIN_BYTE(B, F1 ))\
    	| (((V2 ) << CCT_IO_OFFSET_WITHIN_BYTE (B, F2 )) & CCT_IO_TRUNC_MASK_WITHIN_BYTE(B, F2 ))\
    	| (((V3 ) << CCT_IO_OFFSET_WITHIN_BYTE (B, F3 )) & CCT_IO_TRUNC_MASK_WITHIN_BYTE(B, F3 ))\
    	| (((V4 ) << CCT_IO_OFFSET_WITHIN_BYTE (B, F4 )) & CCT_IO_TRUNC_MASK_WITHIN_BYTE(B, F4 ))\
    	| (((V5 ) << CCT_IO_OFFSET_WITHIN_BYTE (B, F5 )) & CCT_IO_TRUNC_MASK_WITHIN_BYTE(B, F5 ))\
    	| (((V6 ) << CCT_IO_OFFSET_WITHIN_BYTE (B, F6 )) & CCT_IO_TRUNC_MASK_WITHIN_BYTE(B, F6 ))\
    	| (((V7 ) << CCT_IO_OFFSET_WITHIN_BYTE (B, F7 )) & CCT_IO_TRUNC_MASK_WITHIN_BYTE(B, F7 ))\
    	| (((V8 ) << CCT_IO_OFFSET_WITHIN_BYTE (B, F8 )) & CCT_IO_TRUNC_MASK_WITHIN_BYTE(B, F8 ))\
			); \
	} \
	else { \
		CCT_IO_BYTE_HOST (B, F1) = (uint8_t)((CCT_IO_BYTE_HOST (B, F1) \
			& ~(CCT_IO_HOST_RISED_WITHIN_BYTE (B, F1) | CCT_IO_MASK_WITHIN_BYTE (B, \
					F1, F2, F3, F4, F5, F6, F7, F8)))   \
    	| (((V1 ) << CCT_IO_OFFSET_WITHIN_BYTE (B, F1 )) & CCT_IO_TRUNC_MASK_WITHIN_BYTE(B, F1 ))\
    	| (((V2 ) << CCT_IO_OFFSET_WITHIN_BYTE (B, F2 )) & CCT_IO_TRUNC_MASK_WITHIN_BYTE(B, F2 ))\
    	| (((V3 ) << CCT_IO_OFFSET_WITHIN_BYTE (B, F3 )) & CCT_IO_TRUNC_MASK_WITHIN_BYTE(B, F3 ))\
    	| (((V4 ) << CCT_IO_OFFSET_WITHIN_BYTE (B, F4 )) & CCT_IO_TRUNC_MASK_WITHIN_BYTE(B, F4 ))\
    	| (((V5 ) << CCT_IO_OFFSET_WITHIN_BYTE (B, F5 )) & CCT_IO_TRUNC_MASK_WITHIN_BYTE(B, F5 ))\
    	| (((V6 ) << CCT_IO_OFFSET_WITHIN_BYTE (B, F6 )) & CCT_IO_TRUNC_MASK_WITHIN_BYTE(B, F6 ))\
    	| (((V7 ) << CCT_IO_OFFSET_WITHIN_BYTE (B, F7 )) & CCT_IO_TRUNC_MASK_WITHIN_BYTE(B, F7 ))\
    	| (((V8 ) << CCT_IO_OFFSET_WITHIN_BYTE (B, F8 )) & CCT_IO_TRUNC_MASK_WITHIN_BYTE(B, F8 ))\
			); \
	}


#define CCT_IO_SETB(B, F)               CCT_IO_SETB2(CCT_IO_BYTE_ADDRESS(B, F), CCT_IO_OFFSET_WITHIN_BYTE(B, F))
#define CCT_IO_SETB2(IO_BYTE, IO_BIT)   CCT_IO_SETB3(IO_BYTE, IO_BIT)
#define CCT_IO_SETB3(IO_BYTE, IO_BIT)   asm volatile ("setb io:" # IO_BYTE "." # IO_BIT "")

#define CCT_IO_CLRB(B, F)               CCT_IO_CLRB2(CCT_IO_BYTE_ADDRESS(B, F), CCT_IO_OFFSET_WITHIN_BYTE(B, F))
#define CCT_IO_CLRB2(IO_BYTE, IO_BIT)   CCT_IO_CLRB3(IO_BYTE, IO_BIT)
#define CCT_IO_CLRB3(IO_BYTE, IO_BIT)   asm volatile ("clrb io:" # IO_BYTE "." # IO_BIT "")


/* Return a dereferenced pointer to the address where the given I/O resides. */

#define IO_HOST(B, F) CCT_IO_HOST2 (IO_ ## B ## __ ## F)
#define CCT_IO_HOST(B, F) CCT_IO_HOST2 (B ## __ ## F)
#define CCT_IO_HOST2(F) CCT_IO_HOST3 (F)
#define CCT_IO_HOST3(...) CCT_IO_HOST4 (__VA_ARGS__)
#define CCT_IO_HOST4(H, HB, A, AB, M, MB, O, OB, W, B, U, R) (H)

/* Return a dereferenced pointer to the byte where the given field F resides. */

#define IO_BYTE_HOST(B, F) CCT_IO_BYTE_HOST2 (IO_ ## B ## __ ## F)
#define CCT_IO_BYTE_HOST(B, F) CCT_IO_BYTE_HOST2 (B ## __ ## F)
#define CCT_IO_BYTE_HOST2(F) CCT_IO_BYTE_HOST3 (F)
#define CCT_IO_BYTE_HOST3(...) CCT_IO_BYTE_HOST4 (__VA_ARGS__)
#define CCT_IO_BYTE_HOST4(H, HB, A, AB, M, MB, O, OB, W, B, U, R) (HB)

/* Return the word address where the given I/O resides. */

#define IO_ADDRESS(B, F) CCT_IO_ADDRESS2 (IO_ ## B ## __ ## F)
#define CCT_IO_ADDRESS(B, F) CCT_IO_ADDRESS2 (B ## __ ## F)
#define CCT_IO_ADDRESS2(F) CCT_IO_ADDRESS3 (F)
#define CCT_IO_ADDRESS3(...) CCT_IO_ADDRESS4 (__VA_ARGS__)
#define CCT_IO_ADDRESS4(H, HB, A, AB, M, MB, O, OB, W, B, U, R) (A)

/* Return the byte address where the given I/O resides. */

#define IO_BYTE_ADDRESS(B, F)  CCT_IO_BYTE_ADDRESS2 (IO_ ## B ## __ ## F)
#define CCT_IO_BYTE_ADDRESS(B, F) CCT_IO_BYTE_ADDRESS2 (B ## __ ## F)
#define CCT_IO_BYTE_ADDRESS2(F)   CCT_IO_BYTE_ADDRESS3 (F)
#define CCT_IO_BYTE_ADDRESS3(...) CCT_IO_BYTE_ADDRESS4 (__VA_ARGS__)
#define CCT_IO_BYTE_ADDRESS4(H, HB, A, AB, M, MB, O, OB, W, B, U, R) (AB)

/* Get a mask of all risable I/Os for the host of the given I/O. */

#define IO_HOST_RISED(B, F) CCT_IO_HOST_RISED2 (IO_ ## B ## __ ## F)
#define CCT_IO_HOST_RISED(B, F) CCT_IO_HOST_RISED2 (B ## __ ## F)
#define CCT_IO_HOST_RISED2(F) CCT_IO_HOST_RISED3 (F)
#define CCT_IO_HOST_RISED3(...) CCT_IO_HOST_RISED4 (__VA_ARGS__)
#define CCT_IO_HOST_RISED4(H, HB, A, AB, M, MB, O, OB, W, B, U, R) (R)

/* Returns a mask of raised (W1C) bits in the byte that stores specified field F */
#define IO_HOST_RISED_WITHIN_BYTE(B, F)     CCT_IO_HOST_RISED_WITHIN_BYTE(IO_ ## B, F)
#define CCT_IO_HOST_RISED_WITHIN_BYTE(B, F) \
    ((((uint16_t)CCT_IO_BYTE_ADDRESS(B, F) & 1u) == 0u) ? \
        (CCT_IO_HOST_RISED(B, F) & 0x00FFu) : \
        ((uint16_t)CCT_IO_HOST_RISED(B, F) >> 8u))

/* Get a mask of all unused host bits for giveb I/O field. */

#define IO_HOST_UNUSED(B, F) CCT_IO_HOST_UNUSED2 (IO_ ## B ## __ ## F)
#define CCT_IO_HOST_UNUSED(B, F) CCT_IO_HOST_UNUSED2 (B ## __ ## F)
#define CCT_IO_HOST_UNUSED2(F) CCT_IO_HOST_UNUSED3 (F)
#define CCT_IO_HOST_UNUSED3(...) CCT_IO_HOST_UNUSED4 (__VA_ARGS__)
#define CCT_IO_HOST_UNUSED4(H, HB, A, AB, M, MB, O, OB, W, B, U, R) (U)

/* Returns a mask of unused bits in the byte that stores specified field F */
#define IO_HOST_UNUSED_WITHIN_BYTE(B, F)     CCT_IO_HOST_UNUSED_WITHIN_BYTE(IO_ ## B, F)
#define CCT_IO_HOST_UNUSED_WITHIN_BYTE(B, F) \
    ((((uint16_t)CCT_IO_BYTE_ADDRESS(B, F) & 1u) == 0u) ? \
        (CCT_IO_HOST_UNUSED(B, F) & 0x00FFu) : \
        ((uint16_t)CCT_IO_HOST_UNUSED(B, F) >> 8u))

/* Get the bit offset (within word) from where the given I/O begins in its host. */

#define IO_OFFSET(B, F) CCT_IO_OFFSET2 (IO_ ## B ## __ ## F)
#define CCT_IO_OFFSET(B, F) CCT_IO_OFFSET2 (B ## __ ## F)
#define CCT_IO_OFFSET2(F) CCT_IO_OFFSET3 (F)
#define CCT_IO_OFFSET3(...) CCT_IO_OFFSET4 (__VA_ARGS__)
#define CCT_IO_OFFSET4(H, HB, A, AB, M, MB, O, OB, W, B, U, R) (O)

/* Get the bit offset (within byte) from where the given I/O begins in its host. */

#define IO_OFFSET_WITHIN_BYTE(B, F)  CCT_IO_OFFSET_WITHIN_BYTE2 (IO_ ## B ## __ ## F)
#define CCT_IO_OFFSET_WITHIN_BYTE(B, F) CCT_IO_OFFSET_WITHIN_BYTE2 (B ## __ ## F)
#define CCT_IO_OFFSET_WITHIN_BYTE2(F)   CCT_IO_OFFSET_WITHIN_BYTE3 (F)
#define CCT_IO_OFFSET_WITHIN_BYTE3(...) CCT_IO_OFFSET_WITHIN_BYTE4 (__VA_ARGS__)
#define CCT_IO_OFFSET_WITHIN_BYTE4(H, HB, A, AB, M, MB, O, OB, W, B, U, R) (OB)

/* Checks whether the given I/O is dummy one i.e. it is a MlxCCT private. */

#define IO_DUMMY(B, F) CCT_IO_DUMMY2 (IO_ ## B ## __ ## F)
#define CCT_IO_DUMMY(B, F) CCT_IO_DUMMY2 (B ## __ ## F)
#define CCT_IO_DUMMY2(F) CCT_IO_DUMMY3 (F)
#define CCT_IO_DUMMY3(...) CCT_IO_DUMMY4 (__VA_ARGS__)
#define CCT_IO_DUMMY4(H, HB, A, AB, M, MB, O, OB, W, B, U, R) ((B) == 0u)

/* Checks whether the given I/Os can be bit accessed (setb, clrb, ...). */

#define IO_BIT_ACCESSIBLE(B, F, ...) \
	CCT_IO_BIT_ACCESSIBLE2 (IO_ ## B, F, __VA_ARGS__, \
	DUMMY2,  DUMMY3,  DUMMY4,  DUMMY5,  DUMMY6,  DUMMY7,  DUMMY8,  DUMMY9, \
	DUMMY10, DUMMY11, DUMMY12, DUMMY13, DUMMY14, DUMMY15, DUMMY16)
#define CCT_IO_BIT_ACCESSIBLE(B, F, ...) \
	CCT_IO_BIT_ACCESSIBLE2 (B, F, __VA_ARGS__, \
	DUMMY2,  DUMMY3,  DUMMY4,  DUMMY5,  DUMMY6,  DUMMY7,  DUMMY8,  DUMMY9, \
	DUMMY10, DUMMY11, DUMMY12, DUMMY13, DUMMY14, DUMMY15, DUMMY16)
#define CCT_IO_BIT_ACCESSIBLE2(B, F1, F2, F3, F4, F5, F6, F7, F8,  \
					F9, F10, F11, F12, F13, F14, F15, F16, ...) \
	(  CCT_IO_BIT_ACCESSIBLE3 (B##__##F1 ) && CCT_IO_BIT_ACCESSIBLE3 (B##__##F2 )\
	&& CCT_IO_BIT_ACCESSIBLE3 (B##__##F3 ) && CCT_IO_BIT_ACCESSIBLE3 (B##__##F4 )\
	&& CCT_IO_BIT_ACCESSIBLE3 (B##__##F5 ) && CCT_IO_BIT_ACCESSIBLE3 (B##__##F6 )\
	&& CCT_IO_BIT_ACCESSIBLE3 (B##__##F7 ) && CCT_IO_BIT_ACCESSIBLE3 (B##__##F8 )\
	&& CCT_IO_BIT_ACCESSIBLE3 (B##__##F9 ) && CCT_IO_BIT_ACCESSIBLE3 (B##__##F10)\
	&& CCT_IO_BIT_ACCESSIBLE3 (B##__##F11) && CCT_IO_BIT_ACCESSIBLE3 (B##__##F12)\
	&& CCT_IO_BIT_ACCESSIBLE3 (B##__##F13) && CCT_IO_BIT_ACCESSIBLE3 (B##__##F14)\
	&& CCT_IO_BIT_ACCESSIBLE3 (B##__##F15) && CCT_IO_BIT_ACCESSIBLE3 (B##__##F16))
#define CCT_IO_BIT_ACCESSIBLE3(...) CCT_IO_BIT_ACCESSIBLE4 (__VA_ARGS__)
#define CCT_IO_BIT_ACCESSIBLE4(H, HB, A, AB, M, MB, O, OB, W, B, U, R) (( ((B) & 1u) != 0u) || ((B) == 0u))

/* Checks if byte access can be used for given I/Os fields. */

#define IO_BYTE_ACCESSIBLE(B, F, ...)  CCT_IO_BYTE_ACCESSIBLE(IO_ ## B, F, __VA_ARGS__)
#define CCT_IO_BYTE_ACCESSIBLE(B, F, ...) \
	CCT_IO_BYTE_ACCESSIBLE2 (B, F, __VA_ARGS__, \
	DUMMY2,  DUMMY3,  DUMMY4,  DUMMY5,  DUMMY6,  DUMMY7,  DUMMY8,  DUMMY9, \
	DUMMY10, DUMMY11, DUMMY12, DUMMY13, DUMMY14, DUMMY15, DUMMY16)
#define CCT_IO_BYTE_ACCESSIBLE2(B, F1, F2, F3, F4, F5, F6, F7, F8,  \
					F9, F10, F11, F12, F13, F14, F15, F16, ...) \
	(  CCT_IO_BYTE_ACCESSIBLE3 (B##__##F1 ) && CCT_IO_BYTE_ACCESSIBLE3 (B##__##F2 )\
	&& CCT_IO_BYTE_ACCESSIBLE3 (B##__##F3 ) && CCT_IO_BYTE_ACCESSIBLE3 (B##__##F4 )\
	&& CCT_IO_BYTE_ACCESSIBLE3 (B##__##F5 ) && CCT_IO_BYTE_ACCESSIBLE3 (B##__##F6 )\
	&& CCT_IO_BYTE_ACCESSIBLE3 (B##__##F7 ) && CCT_IO_BYTE_ACCESSIBLE3 (B##__##F8 )\
	&& CCT_IO_BYTE_ACCESSIBLE3 (B##__##F9 ) && CCT_IO_BYTE_ACCESSIBLE3 (B##__##F10)\
	&& CCT_IO_BYTE_ACCESSIBLE3 (B##__##F11) && CCT_IO_BYTE_ACCESSIBLE3 (B##__##F12)\
	&& CCT_IO_BYTE_ACCESSIBLE3 (B##__##F13) && CCT_IO_BYTE_ACCESSIBLE3 (B##__##F14)\
	&& CCT_IO_BYTE_ACCESSIBLE3 (B##__##F15) && CCT_IO_BYTE_ACCESSIBLE3 (B##__##F16))
#define CCT_IO_BYTE_ACCESSIBLE3(...) CCT_IO_BYTE_ACCESSIBLE4 (__VA_ARGS__)
#define CCT_IO_BYTE_ACCESSIBLE4(H, HB, A, AB, M, MB, O, OB, W, B, U, R) (( ((B) & 4u) != 0u) || ((B) == 0u))

/* Get a compound mask for all the given I/Os. */

#define IO_MASK(B, F, ...) \
	CCT_IO_MASK2 (IO_ ## B, F, __VA_ARGS__, \
	DUMMY2,  DUMMY3,  DUMMY4,  DUMMY5,  DUMMY6,  DUMMY7,  DUMMY8,  DUMMY9, \
	DUMMY10, DUMMY11, DUMMY12, DUMMY13, DUMMY14, DUMMY15, DUMMY16)
#define CCT_IO_MASK(B, F, ...) \
	CCT_IO_MASK2 (B, F, __VA_ARGS__, \
	DUMMY2,  DUMMY3,  DUMMY4,  DUMMY5,  DUMMY6,  DUMMY7,  DUMMY8,  DUMMY9, \
	DUMMY10, DUMMY11, DUMMY12, DUMMY13, DUMMY14, DUMMY15, DUMMY16)
#define CCT_IO_MASK2(B, F1, F2, F3, F4, F5, F6, F7, F8, \
					F9, F10, F11, F12, F13, F14, F15, F16, ...) \
	( CCT_IO_MASK3 (B ## __ ## F1 ) | CCT_IO_MASK3 (B ## __ ## F2 ) \
	| CCT_IO_MASK3 (B ## __ ## F3 ) | CCT_IO_MASK3 (B ## __ ## F4 ) \
	| CCT_IO_MASK3 (B ## __ ## F5 ) | CCT_IO_MASK3 (B ## __ ## F6 ) \
	| CCT_IO_MASK3 (B ## __ ## F7 ) | CCT_IO_MASK3 (B ## __ ## F8 ) \
	| CCT_IO_MASK3 (B ## __ ## F9 ) | CCT_IO_MASK3 (B ## __ ## F10) \
	| CCT_IO_MASK3 (B ## __ ## F11) | CCT_IO_MASK3 (B ## __ ## F12) \
	| CCT_IO_MASK3 (B ## __ ## F13) | CCT_IO_MASK3 (B ## __ ## F14) \
	| CCT_IO_MASK3 (B ## __ ## F15) | CCT_IO_MASK3 (B ## __ ## F16))
#define CCT_IO_MASK3(...) CCT_IO_MASK4 (__VA_ARGS__)
#define CCT_IO_MASK4(H, HB, A, AB, M, MB, O, OB, W, B, U, R) (M)

/* Get a compound mask for all the given I/Os. */

#define IO_MASK_WITHIN_BYTE(B, F, ...) \
	CCT_IO_MASK_WITHIN_BYTE2 (IO_ ## B, F, __VA_ARGS__, \
	DUMMY2,  DUMMY3,  DUMMY4,  DUMMY5,  DUMMY6,  DUMMY7,  DUMMY8)
#define CCT_IO_MASK_WITHIN_BYTE(B, F, ...) \
	CCT_IO_MASK_WITHIN_BYTE2 (B, F, __VA_ARGS__, \
	DUMMY2,  DUMMY3,  DUMMY4,  DUMMY5,  DUMMY6,  DUMMY7,  DUMMY8)
#define CCT_IO_MASK_WITHIN_BYTE2(B, F1, F2, F3, F4, F5, F6, F7, F8, ...) \
	( CCT_IO_MASK_WITHIN_BYTE3 (B ## __ ## F1 ) | CCT_IO_MASK_WITHIN_BYTE3 (B ## __ ## F2 ) \
	| CCT_IO_MASK_WITHIN_BYTE3 (B ## __ ## F3 ) | CCT_IO_MASK_WITHIN_BYTE3 (B ## __ ## F4 ) \
	| CCT_IO_MASK_WITHIN_BYTE3 (B ## __ ## F5 ) | CCT_IO_MASK_WITHIN_BYTE3 (B ## __ ## F6 ) \
	| CCT_IO_MASK_WITHIN_BYTE3 (B ## __ ## F7 ) | CCT_IO_MASK_WITHIN_BYTE3 (B ## __ ## F8 ))
#define CCT_IO_MASK_WITHIN_BYTE3(...) CCT_IO_MASK_WITHIN_BYTE4 (__VA_ARGS__)
#define CCT_IO_MASK_WITHIN_BYTE4(H, HB, A, AB, M, MB, O, OB, W, B, U, R) (MB)

/* Returns true if byte access is possible for the requested fields, i.e.
    - all requested fields (Fn) are located in the byte
    - hadrware supports byte access for requested fileds
*/

#define IO_SAME_BYTE(B, F, ...)  CCT_IO_SAME_BYTE(IO_ ## B, F, __VA_ARGS__)
#define CCT_IO_SAME_BYTE(B, F, ...) \
    CCT_IO_SAME_BYTE2 (B, F, __VA_ARGS__, \
    DUMMY2,  DUMMY3,  DUMMY4,  DUMMY5,  DUMMY6,  DUMMY7,  DUMMY8,  DUMMY9, \
    DUMMY10, DUMMY11, DUMMY12, DUMMY13, DUMMY14, DUMMY15, DUMMY16)
#define CCT_IO_SAME_BYTE2(B, F1, F2, F3, F4, F5, F6, F7, F8, \
                            F9, F10, F11, F12, F13, F14, F15, F16, ...) \
    CCT_IO_BYTE_ACCESSIBLE (B, F1, F2, F3, F4, F5, F6, F7, F8, \
                               F9, F10, F11, F12, F13, F14, F15, F16) \
    && (CCT_IO_DUMMY (B, F2 ) || ((uint16_t)CCT_IO_BYTE_ADDRESS (B, F1)==(uint16_t)CCT_IO_BYTE_ADDRESS (B, F2 ))) \
    && (CCT_IO_DUMMY (B, F3 ) || ((uint16_t)CCT_IO_BYTE_ADDRESS (B, F1)==(uint16_t)CCT_IO_BYTE_ADDRESS (B, F3 ))) \
    && (CCT_IO_DUMMY (B, F4 ) || ((uint16_t)CCT_IO_BYTE_ADDRESS (B, F1)==(uint16_t)CCT_IO_BYTE_ADDRESS (B, F4 ))) \
    && (CCT_IO_DUMMY (B, F5 ) || ((uint16_t)CCT_IO_BYTE_ADDRESS (B, F1)==(uint16_t)CCT_IO_BYTE_ADDRESS (B, F5 ))) \
    && (CCT_IO_DUMMY (B, F6 ) || ((uint16_t)CCT_IO_BYTE_ADDRESS (B, F1)==(uint16_t)CCT_IO_BYTE_ADDRESS (B, F6 ))) \
    && (CCT_IO_DUMMY (B, F7 ) || ((uint16_t)CCT_IO_BYTE_ADDRESS (B, F1)==(uint16_t)CCT_IO_BYTE_ADDRESS (B, F7 ))) \
    && (CCT_IO_DUMMY (B, F8 ) || ((uint16_t)CCT_IO_BYTE_ADDRESS (B, F1)==(uint16_t)CCT_IO_BYTE_ADDRESS (B, F8 ))) \
    && (CCT_IO_DUMMY (B, F9 ) || ((uint16_t)CCT_IO_BYTE_ADDRESS (B, F1)==(uint16_t)CCT_IO_BYTE_ADDRESS (B, F9 ))) \
    && (CCT_IO_DUMMY (B, F10) || ((uint16_t)CCT_IO_BYTE_ADDRESS (B, F1)==(uint16_t)CCT_IO_BYTE_ADDRESS (B, F10))) \
    && (CCT_IO_DUMMY (B, F11) || ((uint16_t)CCT_IO_BYTE_ADDRESS (B, F1)==(uint16_t)CCT_IO_BYTE_ADDRESS (B, F11))) \
    && (CCT_IO_DUMMY (B, F12) || ((uint16_t)CCT_IO_BYTE_ADDRESS (B, F1)==(uint16_t)CCT_IO_BYTE_ADDRESS (B, F12))) \
    && (CCT_IO_DUMMY (B, F13) || ((uint16_t)CCT_IO_BYTE_ADDRESS (B, F1)==(uint16_t)CCT_IO_BYTE_ADDRESS (B, F13))) \
    && (CCT_IO_DUMMY (B, F14) || ((uint16_t)CCT_IO_BYTE_ADDRESS (B, F1)==(uint16_t)CCT_IO_BYTE_ADDRESS (B, F14))) \
    && (CCT_IO_DUMMY (B, F15) || ((uint16_t)CCT_IO_BYTE_ADDRESS (B, F1)==(uint16_t)CCT_IO_BYTE_ADDRESS (B, F15))) \
    && (CCT_IO_DUMMY (B, F16) || ((uint16_t)CCT_IO_BYTE_ADDRESS (B, F1)==(uint16_t)CCT_IO_BYTE_ADDRESS (B, F16)))

/* Type of somewhat expressive, compile-time check. */

#define IO_STATIC_ASSERT(MSG, EXPR) { enum { MSG = 1/(!!(EXPR)) }; }

/* Compile time assert that the given I/Os can be written. */

#define IO_ASSERT_WRITABLES(B, F, ...)  CCT_IO_ASSERT_WRITABLES(IO_ ## B, F, __VA_ARGS__)
#define CCT_IO_ASSERT_WRITABLES(B, F, ...) \
	CCT_IO_WRITABLES2 (B, F, __VA_ARGS__, \
	DUMMY2,  DUMMY3,  DUMMY4,  DUMMY5,  DUMMY6,  DUMMY7,  DUMMY8,  DUMMY9, \
	DUMMY10, DUMMY11, DUMMY12, DUMMY13, DUMMY14, DUMMY15, DUMMY16)
#define CCT_IO_WRITABLES2(B, F1, F2, F3, F4, F5, F6, F7, F8, \
					F9, F10, F11, F12, F13, F14, F15, F16, ...) \
	CCT_IO_WRITABLES3 (F1 , B##__##F1 ) CCT_IO_WRITABLES3 (F2 , B##__##F2 ) \
	CCT_IO_WRITABLES3 (F3 , B##__##F3 ) CCT_IO_WRITABLES3 (F4 , B##__##F4 ) \
	CCT_IO_WRITABLES3 (F5 , B##__##F5 ) CCT_IO_WRITABLES3 (F6 , B##__##F6 ) \
	CCT_IO_WRITABLES3 (F7 , B##__##F7 ) CCT_IO_WRITABLES3 (F8 , B##__##F8 ) \
	CCT_IO_WRITABLES3 (F9 , B##__##F9 ) CCT_IO_WRITABLES3 (F10, B##__##F10) \
	CCT_IO_WRITABLES3 (F11, B##__##F11) CCT_IO_WRITABLES3 (F12, B##__##F12) \
	CCT_IO_WRITABLES3 (F13, B##__##F13) CCT_IO_WRITABLES3 (F14, B##__##F14) \
	CCT_IO_WRITABLES3 (F15, B##__##F15) CCT_IO_WRITABLES3 (F16, B##__##F16)
#define CCT_IO_WRITABLES3(...) CCT_IO_WRITABLES4 (__VA_ARGS__)
#if !defined(UNITTEST)
#define CCT_IO_WRITABLES4(F, H, HB, A, AB, M, MB, O, OB, W, B, U, R) \
	IO_STATIC_ASSERT (F ## _IS_WRITABLE, W)
#else
#define CCT_IO_WRITABLES4(F, H, HB, A, AB, M, MB, O, OB, W, B, U, R)
#endif

/* Compile time assert that the given I/Os share the same host. */

#define IO_ASSERT_SAME_HOST(B, F, ...)  CCT_IO_ASSERT_SAME_HOST(IO_ ## B, F, __VA_ARGS__)
#define CCT_IO_ASSERT_SAME_HOST(B, F, ...) \
	CCT_IO_SAME_HOST2 (B, F, __VA_ARGS__, \
	DUMMY2,  DUMMY3,  DUMMY4,  DUMMY5,  DUMMY6,  DUMMY7,  DUMMY8,  DUMMY9, \
	DUMMY10, DUMMY11, DUMMY12, DUMMY13, DUMMY14, DUMMY15, DUMMY16)
#define CCT_IO_SAME_HOST2(B, F1, F2, F3, F4, F5, F6, F7, F8, \
					F9, F10, F11, F12, F13, F14, F15, F16, ...) \
	IO_STATIC_ASSERT (FIELDS_SHARE_SAME_ADDRESS, \
	   (CCT_IO_DUMMY (B, F2 ) || (CCT_IO_ADDRESS (B, F1)==CCT_IO_ADDRESS (B, F2 ))) \
	&& (CCT_IO_DUMMY (B, F3 ) || (CCT_IO_ADDRESS (B, F1)==CCT_IO_ADDRESS (B, F3 ))) \
	&& (CCT_IO_DUMMY (B, F4 ) || (CCT_IO_ADDRESS (B, F1)==CCT_IO_ADDRESS (B, F4 ))) \
	&& (CCT_IO_DUMMY (B, F5 ) || (CCT_IO_ADDRESS (B, F1)==CCT_IO_ADDRESS (B, F5 ))) \
	&& (CCT_IO_DUMMY (B, F6 ) || (CCT_IO_ADDRESS (B, F1)==CCT_IO_ADDRESS (B, F6 ))) \
	&& (CCT_IO_DUMMY (B, F7 ) || (CCT_IO_ADDRESS (B, F1)==CCT_IO_ADDRESS (B, F7 ))) \
	&& (CCT_IO_DUMMY (B, F8 ) || (CCT_IO_ADDRESS (B, F1)==CCT_IO_ADDRESS (B, F8 ))) \
	&& (CCT_IO_DUMMY (B, F9 ) || (CCT_IO_ADDRESS (B, F1)==CCT_IO_ADDRESS (B, F9 ))) \
	&& (CCT_IO_DUMMY (B, F10) || (CCT_IO_ADDRESS (B, F1)==CCT_IO_ADDRESS (B, F10))) \
	&& (CCT_IO_DUMMY (B, F11) || (CCT_IO_ADDRESS (B, F1)==CCT_IO_ADDRESS (B, F11))) \
	&& (CCT_IO_DUMMY (B, F12) || (CCT_IO_ADDRESS (B, F1)==CCT_IO_ADDRESS (B, F12))) \
	&& (CCT_IO_DUMMY (B, F13) || (CCT_IO_ADDRESS (B, F1)==CCT_IO_ADDRESS (B, F13))) \
	&& (CCT_IO_DUMMY (B, F14) || (CCT_IO_ADDRESS (B, F1)==CCT_IO_ADDRESS (B, F14))) \
	&& (CCT_IO_DUMMY (B, F15) || (CCT_IO_ADDRESS (B, F1)==CCT_IO_ADDRESS (B, F15))) \
	&& (CCT_IO_DUMMY (B, F16) || (CCT_IO_ADDRESS (B, F1)==CCT_IO_ADDRESS (B, F16))))

/* Assert whether these arguments fits in the IO_ macros. */

#define IO_ASSERT_ARGS_FIT(B, F, ...)  CCT_IO_ASSERT_ARGS_FIT(IO_ ## B, F, __VA_ARGS__)
#define CCT_IO_ASSERT_ARGS_FIT(B, F, ...) \
	CCT_IO_ASSERT_ARGS_FIT2 (B, F, __VA_ARGS__, \
	DUMMY2,  DUMMY3,  DUMMY4,  DUMMY5,  DUMMY6,  DUMMY7,  DUMMY8,  DUMMY9, \
	DUMMY10, DUMMY11, DUMMY12, DUMMY13, DUMMY14, DUMMY15, DUMMY16, DUMMY17)
#define CCT_IO_ASSERT_ARGS_FIT2(B, F1, F2, F3, F4, F5, F6, F7, F8, \
					F9, F10, F11, F12, F13, F14, F15, F16, F17, ...) \
	CCT_IO_ASSERT_ARGS_FIT3 (B ## __ ## F17)
#define CCT_IO_ASSERT_ARGS_FIT3(...) CCT_IO_ASSERT_ARGS_FIT4 (__VA_ARGS__)
#define CCT_IO_ASSERT_ARGS_FIT4(H, HB, A, AB, M, MB, O, OB, W, B, U, R) \
	IO_STATIC_ASSERT (ARGUMENTS_FIT_IN_MACRO, (B) == 0u)

/*****************************************************************************/
/* 3. I/O declarations. */

/*****************************************************************************/
/* 3.1 Hosting variables. */

#if !defined(IO_CONST)
#   if defined(UNITTEST)
#       define IO_CONST
#   else
#       define IO_CONST const
#   endif
#endif

/* Word variables (16-bit) */
extern volatile uint16_t io0x0 __attribute__((io, addr(0x0)));
extern volatile uint16_t io0x4 __attribute__((io, addr(0x4)));
extern volatile uint16_t io0x6 __attribute__((io, addr(0x6)));
extern volatile uint16_t io0x8 __attribute__((io, addr(0x8)));
extern volatile uint16_t io0xa __attribute__((io, addr(0xa)));
extern volatile uint16_t error_IO_AWD__ACK_has_no_word_access; /* to catch attempts of word access when not supported */
extern volatile uint16_t error_IO_AWD__WIN_OPEN_has_no_word_access; /* to catch attempts of word access when not supported */
extern volatile uint16_t io0x10 __attribute__((io, addr(0x10)));
extern volatile uint16_t io0x12 __attribute__((io, addr(0x12)));
extern volatile uint16_t io0x14 __attribute__((io, addr(0x14)));
extern volatile uint16_t io0x16 __attribute__((io, addr(0x16)));
extern volatile uint16_t io0x18 __attribute__((io, addr(0x18)));
extern volatile uint16_t io0x1a __attribute__((io, addr(0x1a)));
extern volatile uint16_t io0x1c __attribute__((io, addr(0x1c)));
extern volatile uint16_t io0x1e __attribute__((io, addr(0x1e)));
extern volatile uint16_t io0x20 __attribute__((io, addr(0x20)));
extern volatile uint16_t io0x22 __attribute__((io, addr(0x22)));
extern volatile uint16_t io0x26 __attribute__((io, addr(0x26)));
extern volatile uint16_t io0x28 __attribute__((io, addr(0x28)));
extern volatile uint16_t io0x2a __attribute__((io, addr(0x2a)));
extern IO_CONST volatile uint16_t io0x2c __attribute__((io, addr(0x2c)));
extern volatile uint16_t io0x2e __attribute__((io, addr(0x2e)));
extern IO_CONST volatile uint16_t io0x30 __attribute__((io, addr(0x30)));
extern volatile uint16_t io0x32 __attribute__((io, addr(0x32)));
extern IO_CONST volatile uint16_t io0x38 __attribute__((io, addr(0x38)));
extern volatile uint16_t io0x3a __attribute__((io, addr(0x3a)));
extern IO_CONST volatile uint16_t io0x3c __attribute__((io, addr(0x3c)));
extern IO_CONST volatile uint16_t io0x3e __attribute__((io, addr(0x3e)));
extern volatile uint16_t io0x40 __attribute__((nodp, addr(0x40)));
extern volatile uint16_t io0x42 __attribute__((nodp, addr(0x42)));
extern volatile uint16_t io0x5a __attribute__((nodp, addr(0x5a)));
extern volatile uint16_t io0x5c __attribute__((nodp, addr(0x5c)));
extern volatile uint16_t io0x5e __attribute__((nodp, addr(0x5e)));
extern volatile uint16_t io0x78 __attribute__((nodp, addr(0x78)));
extern volatile uint16_t io0x7a __attribute__((nodp, addr(0x7a)));
extern volatile uint16_t io0x7c __attribute__((nodp, addr(0x7c)));
extern volatile uint16_t io0x7e __attribute__((nodp, addr(0x7e)));
extern volatile uint16_t io0xae __attribute__((nodp, addr(0xae)));
extern volatile uint16_t io0xb0 __attribute__((nodp, addr(0xb0)));
extern volatile uint16_t io0xb2 __attribute__((nodp, addr(0xb2)));
extern volatile uint16_t io0xb4 __attribute__((nodp, addr(0xb4)));
extern volatile uint16_t io0xb6 __attribute__((nodp, addr(0xb6)));
extern volatile uint16_t io0xb8 __attribute__((nodp, addr(0xb8)));
extern volatile uint16_t io0xba __attribute__((nodp, addr(0xba)));
extern volatile uint16_t io0xbc __attribute__((nodp, addr(0xbc)));
extern volatile uint16_t io0xbe __attribute__((nodp, addr(0xbe)));
extern IO_CONST volatile uint16_t io0x100 __attribute__((nodp, addr(0x100)));
extern IO_CONST volatile uint16_t io0x102 __attribute__((nodp, addr(0x102)));
extern volatile uint16_t io0x104 __attribute__((nodp, addr(0x104)));
extern volatile uint16_t io0x106 __attribute__((nodp, addr(0x106)));
extern volatile uint16_t io0x108 __attribute__((nodp, addr(0x108)));
extern volatile uint16_t io0x10a __attribute__((nodp, addr(0x10a)));
extern volatile uint16_t io0x10c __attribute__((nodp, addr(0x10c)));
extern volatile uint16_t io0x10e __attribute__((nodp, addr(0x10e)));
extern volatile uint16_t io0x110 __attribute__((nodp, addr(0x110)));
extern volatile uint16_t io0x112 __attribute__((nodp, addr(0x112)));
extern volatile uint16_t io0x114 __attribute__((nodp, addr(0x114)));
extern volatile uint16_t io0x116 __attribute__((nodp, addr(0x116)));
extern volatile uint16_t io0x11a __attribute__((nodp, addr(0x11a)));
extern volatile uint16_t io0x13a __attribute__((nodp, addr(0x13a)));
extern volatile uint16_t io0x13c __attribute__((nodp, addr(0x13c)));
extern volatile uint16_t io0x13e __attribute__((nodp, addr(0x13e)));
extern IO_CONST volatile uint16_t io0x140 __attribute__((nodp, addr(0x140)));
extern IO_CONST volatile uint16_t io0x142 __attribute__((nodp, addr(0x142)));
extern IO_CONST volatile uint16_t io0x144 __attribute__((nodp, addr(0x144)));
extern IO_CONST volatile uint16_t io0x146 __attribute__((nodp, addr(0x146)));
extern volatile uint16_t io0x148 __attribute__((nodp, addr(0x148)));
extern volatile uint16_t io0x14a __attribute__((nodp, addr(0x14a)));
extern volatile uint16_t io0x14c __attribute__((nodp, addr(0x14c)));
extern volatile uint16_t io0x14e __attribute__((nodp, addr(0x14e)));
extern volatile uint16_t io0x150 __attribute__((nodp, addr(0x150)));
extern volatile uint16_t io0x152 __attribute__((nodp, addr(0x152)));
extern volatile uint16_t io0x154 __attribute__((nodp, addr(0x154)));
extern volatile uint16_t io0x156 __attribute__((nodp, addr(0x156)));
extern volatile uint16_t io0x158 __attribute__((nodp, addr(0x158)));
extern volatile uint16_t io0x15a __attribute__((nodp, addr(0x15a)));
extern IO_CONST volatile uint16_t io0x15c __attribute__((nodp, addr(0x15c)));
extern volatile uint16_t io0x15e __attribute__((nodp, addr(0x15e)));
extern volatile uint16_t io0x160 __attribute__((nodp, addr(0x160)));
extern volatile uint16_t io0x162 __attribute__((nodp, addr(0x162)));
extern volatile uint16_t io0x164 __attribute__((nodp, addr(0x164)));
extern volatile uint16_t io0x166 __attribute__((nodp, addr(0x166)));
extern volatile uint16_t io0x168 __attribute__((nodp, addr(0x168)));
extern volatile uint16_t io0x16a __attribute__((nodp, addr(0x16a)));
extern volatile uint16_t io0x16c __attribute__((nodp, addr(0x16c)));
extern volatile uint16_t io0x16e __attribute__((nodp, addr(0x16e)));
extern volatile uint16_t io0x170 __attribute__((nodp, addr(0x170)));
extern volatile uint16_t io0x172 __attribute__((nodp, addr(0x172)));
extern volatile uint16_t io0x174 __attribute__((nodp, addr(0x174)));
extern volatile uint16_t io0x176 __attribute__((nodp, addr(0x176)));
extern volatile uint16_t io0x178 __attribute__((nodp, addr(0x178)));
extern volatile uint16_t io0x17a __attribute__((nodp, addr(0x17a)));
extern volatile uint16_t io0x17c __attribute__((nodp, addr(0x17c)));
extern volatile uint16_t io0x17e __attribute__((nodp, addr(0x17e)));
extern volatile uint16_t io0x180 __attribute__((nodp, addr(0x180)));
extern volatile uint16_t io0x182 __attribute__((nodp, addr(0x182)));
extern volatile uint16_t io0x184 __attribute__((nodp, addr(0x184)));
extern volatile uint16_t io0x186 __attribute__((nodp, addr(0x186)));
extern volatile uint16_t io0x188 __attribute__((nodp, addr(0x188)));
extern volatile uint16_t io0x18a __attribute__((nodp, addr(0x18a)));
extern volatile uint16_t io0x18c __attribute__((nodp, addr(0x18c)));
extern volatile uint16_t io0x18e __attribute__((nodp, addr(0x18e)));
extern volatile uint16_t io0x190 __attribute__((nodp, addr(0x190)));
extern volatile uint16_t io0x192 __attribute__((nodp, addr(0x192)));
extern volatile uint16_t io0x194 __attribute__((nodp, addr(0x194)));
extern volatile uint16_t io0x196 __attribute__((nodp, addr(0x196)));
extern volatile uint16_t io0x198 __attribute__((nodp, addr(0x198)));
extern volatile uint16_t io0x19a __attribute__((nodp, addr(0x19a)));
extern volatile uint16_t io0x19c __attribute__((nodp, addr(0x19c)));
extern volatile uint16_t io0x19e __attribute__((nodp, addr(0x19e)));
extern volatile uint16_t io0x1a0 __attribute__((nodp, addr(0x1a0)));
extern volatile uint16_t io0x1a2 __attribute__((nodp, addr(0x1a2)));
extern volatile uint16_t io0x1a4 __attribute__((nodp, addr(0x1a4)));
extern volatile uint16_t io0x1a6 __attribute__((nodp, addr(0x1a6)));
extern volatile uint16_t io0x1a8 __attribute__((nodp, addr(0x1a8)));
extern volatile uint16_t io0x1aa __attribute__((nodp, addr(0x1aa)));
extern volatile uint16_t io0x1ac __attribute__((nodp, addr(0x1ac)));
extern volatile uint16_t io0x1ae __attribute__((nodp, addr(0x1ae)));
extern volatile uint16_t io0x1b0 __attribute__((nodp, addr(0x1b0)));
extern volatile uint16_t io0x1b2 __attribute__((nodp, addr(0x1b2)));
extern volatile uint16_t io0x1b4 __attribute__((nodp, addr(0x1b4)));
extern volatile uint16_t io0x1b6 __attribute__((nodp, addr(0x1b6)));
extern volatile uint16_t io0x1b8 __attribute__((nodp, addr(0x1b8)));
extern volatile uint16_t io0x1ba __attribute__((nodp, addr(0x1ba)));
extern volatile uint16_t io0x1bc __attribute__((nodp, addr(0x1bc)));
extern volatile uint16_t io0x1be __attribute__((nodp, addr(0x1be)));
extern IO_CONST volatile uint16_t io0x1c0 __attribute__((nodp, addr(0x1c0)));
extern IO_CONST volatile uint16_t io0x1c2 __attribute__((nodp, addr(0x1c2)));
extern volatile uint16_t io0x1c4 __attribute__((nodp, addr(0x1c4)));
extern volatile uint16_t io0x1c6 __attribute__((nodp, addr(0x1c6)));
extern volatile uint16_t io0x1c8 __attribute__((nodp, addr(0x1c8)));
extern volatile uint16_t io0x1ca __attribute__((nodp, addr(0x1ca)));
extern volatile uint16_t io0x1cc __attribute__((nodp, addr(0x1cc)));
extern volatile uint16_t io0x1ce __attribute__((nodp, addr(0x1ce)));
extern volatile uint16_t io0x1d0 __attribute__((nodp, addr(0x1d0)));
extern volatile uint16_t io0x1d2 __attribute__((nodp, addr(0x1d2)));
extern volatile uint16_t io0x1d4 __attribute__((nodp, addr(0x1d4)));
extern volatile uint16_t io0x1d6 __attribute__((nodp, addr(0x1d6)));
extern volatile uint16_t io0x1d8 __attribute__((nodp, addr(0x1d8)));
extern volatile uint16_t io0x1da __attribute__((nodp, addr(0x1da)));
extern volatile uint16_t io0x1dc __attribute__((nodp, addr(0x1dc)));
extern volatile uint16_t io0x1de __attribute__((nodp, addr(0x1de)));
extern volatile uint16_t io0x1e0 __attribute__((nodp, addr(0x1e0)));
extern volatile uint16_t io0x1e2 __attribute__((nodp, addr(0x1e2)));
extern volatile uint16_t io0x1e4 __attribute__((nodp, addr(0x1e4)));
extern volatile uint16_t io0x1e6 __attribute__((nodp, addr(0x1e6)));
extern volatile uint16_t io0x1e8 __attribute__((nodp, addr(0x1e8)));
extern volatile uint16_t io0x1ea __attribute__((nodp, addr(0x1ea)));
extern volatile uint16_t io0x1ec __attribute__((nodp, addr(0x1ec)));
extern volatile uint16_t io0x1ee __attribute__((nodp, addr(0x1ee)));
extern volatile uint16_t io0x1f0 __attribute__((nodp, addr(0x1f0)));
extern volatile uint16_t io0x1f2 __attribute__((nodp, addr(0x1f2)));
extern volatile uint16_t io0x1f4 __attribute__((nodp, addr(0x1f4)));
extern IO_CONST volatile uint16_t io0x1f6 __attribute__((nodp, addr(0x1f6)));
extern IO_CONST volatile uint16_t io0x1f8 __attribute__((nodp, addr(0x1f8)));
extern volatile uint16_t io0x1fa __attribute__((nodp, addr(0x1fa)));
extern volatile uint16_t io0x1fc __attribute__((nodp, addr(0x1fc)));
extern volatile uint16_t io0x1fe __attribute__((nodp, addr(0x1fe)));
extern volatile uint16_t io0x200 __attribute__((nodp, addr(0x200)));
extern volatile uint16_t io0x202 __attribute__((nodp, addr(0x202)));
extern volatile uint16_t io0x204 __attribute__((nodp, addr(0x204)));
extern volatile uint16_t io0x206 __attribute__((nodp, addr(0x206)));
extern volatile uint16_t io0x208 __attribute__((nodp, addr(0x208)));
extern volatile uint16_t io0x20a __attribute__((nodp, addr(0x20a)));
extern volatile uint16_t io0x20c __attribute__((nodp, addr(0x20c)));
extern volatile uint16_t io0x20e __attribute__((nodp, addr(0x20e)));
extern volatile uint16_t io0x210 __attribute__((nodp, addr(0x210)));
extern volatile uint16_t io0x212 __attribute__((nodp, addr(0x212)));
extern volatile uint16_t io0x214 __attribute__((nodp, addr(0x214)));
extern volatile uint16_t io0x216 __attribute__((nodp, addr(0x216)));
extern volatile uint16_t io0x218 __attribute__((nodp, addr(0x218)));
extern volatile uint16_t io0x21a __attribute__((nodp, addr(0x21a)));
extern volatile uint16_t io0x21c __attribute__((nodp, addr(0x21c)));
extern volatile uint16_t io0x21e __attribute__((nodp, addr(0x21e)));
extern volatile uint16_t io0x220 __attribute__((nodp, addr(0x220)));
extern volatile uint16_t io0x222 __attribute__((nodp, addr(0x222)));
extern volatile uint16_t io0x224 __attribute__((nodp, addr(0x224)));
extern volatile uint16_t io0x226 __attribute__((nodp, addr(0x226)));
extern volatile uint16_t io0x228 __attribute__((nodp, addr(0x228)));
extern volatile uint16_t io0x22a __attribute__((nodp, addr(0x22a)));
extern volatile uint16_t io0x22c __attribute__((nodp, addr(0x22c)));
extern volatile uint16_t io0x22e __attribute__((nodp, addr(0x22e)));
extern volatile uint16_t io0x230 __attribute__((nodp, addr(0x230)));
extern volatile uint16_t io0x232 __attribute__((nodp, addr(0x232)));
extern volatile uint16_t io0x234 __attribute__((nodp, addr(0x234)));
extern volatile uint16_t io0x236 __attribute__((nodp, addr(0x236)));
extern volatile uint16_t io0x238 __attribute__((nodp, addr(0x238)));
extern volatile uint16_t io0x23a __attribute__((nodp, addr(0x23a)));
extern volatile uint16_t io0x23c __attribute__((nodp, addr(0x23c)));
extern volatile uint16_t io0x23e __attribute__((nodp, addr(0x23e)));
extern volatile uint16_t io0x240 __attribute__((nodp, addr(0x240)));
extern volatile uint16_t io0x242 __attribute__((nodp, addr(0x242)));

/* Byte variables (8-bit) */

extern volatile uint8_t io_byte_0x0  __attribute__((io, addr(0x0)));
extern volatile uint8_t io_byte_0x1  __attribute__((io, addr(0x1)));
extern volatile uint8_t io_byte_0x4  __attribute__((io, addr(0x4)));
extern volatile uint8_t io_byte_0x5  __attribute__((io, addr(0x5)));
extern volatile uint8_t error_IO_FUNC_TEST__TEST_UNPROT_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_MUPET__PTC_RECEIVE_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_MUPET__PTC_SEND_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0xa  __attribute__((io, addr(0xa)));
extern volatile uint8_t io_byte_0xb  __attribute__((io, addr(0xb)));
extern volatile uint8_t io_byte_0xc  __attribute__((io, bit_access, addr(0xc)));
extern volatile uint8_t io_byte_0xd  __attribute__((io, bit_access, addr(0xd)));
extern volatile uint8_t error_IO_ROM_BIST__ADD_START_L_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x12  __attribute__((io, addr(0x12)));
extern volatile uint8_t error_IO_ROM_BIST__ADD_STOP_L_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x16  __attribute__((io, addr(0x16)));
extern volatile uint8_t error_IO_ROM_BIST__SIG_EXPECTED_L_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x1a  __attribute__((io, addr(0x1a)));
extern volatile uint8_t io_byte_0x1c  __attribute__((io, addr(0x1c)));
extern volatile uint8_t io_byte_0x1d  __attribute__((io, addr(0x1d)));
extern volatile uint8_t error_IO_ROM_BIST__START_BIST_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_ROM_BIST__SIG_RECEIVED_L_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x22  __attribute__((io, addr(0x22)));
extern volatile uint8_t io_byte_0x26  __attribute__((io, addr(0x26)));
extern volatile uint8_t io_byte_0x27  __attribute__((io, addr(0x27)));
extern volatile uint8_t io_byte_0x29  __attribute__((io, addr(0x29)));
extern volatile uint8_t io_byte_0x2a  __attribute__((io, addr(0x2a)));
extern volatile uint8_t error_IO_NVRAM_SHELL__TM_NVCP_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x2b  __attribute__((io, addr(0x2b)));
extern IO_CONST volatile uint8_t error_IO_STIMER1__CURRENT_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_STIMER1__VALUE_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_STIMER1__MODE_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_STIMER2__CURRENT_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_STIMER2__VALUE_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_STIMER2__MODE_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_MLX16__XCG_STAT_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_MLX16__XCG_DR1_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_MLX16__XCG_DR3_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_MLX16__SHELL_VERSION_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_MLX16__ITC_PEND0_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x40  __attribute__((nodp, addr(0x40)));
extern volatile uint8_t io_byte_0x41  __attribute__((nodp, addr(0x41)));
extern volatile uint8_t io_byte_0x42  __attribute__((nodp, addr(0x42)));
extern volatile uint8_t error_IO_MLX16__ITC_PEND1_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x43  __attribute__((nodp, addr(0x43)));
extern volatile uint8_t io_byte_0x5b  __attribute__((nodp, addr(0x5b)));
extern volatile uint8_t error_IO_MLX16__ITC_MASK0_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x5c  __attribute__((nodp, addr(0x5c)));
extern volatile uint8_t io_byte_0x5d  __attribute__((nodp, addr(0x5d)));
extern volatile uint8_t io_byte_0x5e  __attribute__((nodp, addr(0x5e)));
extern volatile uint8_t error_IO_MLX16__ITC_MASK1_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x5f  __attribute__((nodp, addr(0x5f)));
extern volatile uint8_t error_IO_MLX16__ITC_PRIO0_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x78  __attribute__((nodp, addr(0x78)));
extern volatile uint8_t io_byte_0x79  __attribute__((nodp, addr(0x79)));
extern volatile uint8_t io_byte_0x7a  __attribute__((nodp, addr(0x7a)));
extern volatile uint8_t error_IO_MLX16__ITC_PRIO1_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x7b  __attribute__((nodp, addr(0x7b)));
extern volatile uint8_t error_IO_MLX16__ITC_PRIO2_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x7c  __attribute__((nodp, addr(0x7c)));
extern volatile uint8_t io_byte_0x7d  __attribute__((nodp, addr(0x7d)));
extern volatile uint8_t io_byte_0x7e  __attribute__((nodp, addr(0x7e)));
extern volatile uint8_t error_IO_MLX16__CPU_FP0ADR_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_MLX16__PATCH_ADDR0_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_MLX16__PATCH_INSTR0_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_MLX16__PATCH_ADDR1_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_MLX16__PATCH_INSTR1_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_MLX16__PATCH_ADDR2_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_MLX16__PATCH_INSTR2_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_MLX16__PATCH_ADDR3_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_MLX16__PATCH_INSTR3_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_VERSION__VERSION_L_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_VERSION__VERSION_H_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_FLASH_SHELL__COMMAND_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x104  __attribute__((nodp, addr(0x104)));
extern volatile uint8_t io_byte_0x105  __attribute__((nodp, addr(0x105)));
extern volatile uint8_t io_byte_0x106  __attribute__((nodp, addr(0x106)));
extern volatile uint8_t io_byte_0x107  __attribute__((nodp, addr(0x107)));
extern volatile uint8_t io_byte_0x108  __attribute__((nodp, addr(0x108)));
extern volatile uint8_t io_byte_0x109  __attribute__((nodp, addr(0x109)));
extern volatile uint8_t io_byte_0x10a  __attribute__((nodp, addr(0x10a)));
extern volatile uint8_t io_byte_0x10b  __attribute__((nodp, addr(0x10b)));
extern volatile uint8_t error_IO_FLASH_SHELL__SEC_COUNTER_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_FLASH_SHELL__BIT_ERRORS_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x10f  __attribute__((nodp, addr(0x10f)));
extern volatile uint8_t error_IO_FLASH_SHELL__PL_TRL_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_FLASH_SHELL__TRH_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_FLASH_SHELL__PL_TR_AD_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x116  __attribute__((nodp, addr(0x116)));
extern volatile uint8_t io_byte_0x117  __attribute__((nodp, addr(0x117)));
extern volatile uint8_t io_byte_0x11a  __attribute__((nodp, addr(0x11a)));
extern volatile uint8_t io_byte_0x11b  __attribute__((nodp, addr(0x11b)));
extern volatile uint8_t error_IO_FLASH_SHELL__SIG_L_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_FLASH_SHELL__SIG_H_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_RAM_BIST__KEY_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_RAM_BIST__PHASE_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_RAM_BIST__TRANSPARENT_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_RAM_BIST__REGULAR_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_RAM_BIST__RUNNING_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_RAM_BIST__VALID_CLOCK_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_RAM_BIST__COMPLETED_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_RAM_BIST__LFSR_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_RAM_BIST__LSFR_GOT_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_RAM_BIST__LFSR_EXPECTED_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_RAM_BIST__ADL_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_RAM_BIST__ADH_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_RAM_BIST__ADD_START_L_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x14a  __attribute__((nodp, addr(0x14a)));
extern volatile uint8_t error_IO_RAM_BIST__ADD_STOP_L_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x14e  __attribute__((nodp, addr(0x14e)));
extern volatile uint8_t io_byte_0x150  __attribute__((nodp, addr(0x150)));
extern volatile uint8_t io_byte_0x151  __attribute__((nodp, addr(0x151)));
extern volatile uint8_t io_byte_0x152  __attribute__((nodp, addr(0x152)));
extern volatile uint8_t io_byte_0x153  __attribute__((nodp, addr(0x153)));
extern volatile uint8_t error_IO_ADC_XH018__SBASE_0_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x156  __attribute__((nodp, addr(0x156)));
extern volatile uint8_t io_byte_0x157  __attribute__((nodp, addr(0x157)));
extern volatile uint8_t io_byte_0x158  __attribute__((nodp, addr(0x158)));
extern volatile uint8_t io_byte_0x15a  __attribute__((nodp, addr(0x15a)));
extern volatile uint8_t io_byte_0x15b  __attribute__((nodp, addr(0x15b)));
extern IO_CONST volatile uint8_t io_byte_0x15c  __attribute__((nodp, addr(0x15c)));
extern volatile uint8_t error_IO_ADC_XH018__CORR_15_0_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_ADC_XH018__CORR_31_16_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_ADC_XH018__CORR_46_32_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x163  __attribute__((nodp, addr(0x163)));
extern volatile uint8_t error_IO_CUSTOM_91804__FLASH_TR0_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_CUSTOM_91804__FLASH_TR1_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_CUSTOM_91804__FLASH_TR2_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_CUSTOM_91804__FLASH_TR3_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_CUSTOM_91804__FLASH_TR4_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_CUSTOM_91804__FLASH_TR5_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x170  __attribute__((nodp, addr(0x170)));
extern volatile uint8_t io_byte_0x171  __attribute__((nodp, addr(0x171)));
extern volatile uint8_t io_byte_0x172  __attribute__((nodp, addr(0x172)));
extern volatile uint8_t io_byte_0x173  __attribute__((nodp, addr(0x173)));
extern volatile uint8_t io_byte_0x174  __attribute__((nodp, addr(0x174)));
extern volatile uint8_t io_byte_0x175  __attribute__((nodp, addr(0x175)));
extern volatile uint8_t io_byte_0x176  __attribute__((nodp, addr(0x176)));
extern volatile uint8_t io_byte_0x177  __attribute__((nodp, addr(0x177)));
extern volatile uint8_t io_byte_0x178  __attribute__((nodp, addr(0x178)));
extern volatile uint8_t io_byte_0x179  __attribute__((nodp, addr(0x179)));
extern volatile uint8_t io_byte_0x17a  __attribute__((nodp, addr(0x17a)));
extern volatile uint8_t io_byte_0x17b  __attribute__((nodp, addr(0x17b)));
extern volatile uint8_t io_byte_0x17c  __attribute__((nodp, addr(0x17c)));
extern volatile uint8_t io_byte_0x17d  __attribute__((nodp, addr(0x17d)));
extern volatile uint8_t io_byte_0x17e  __attribute__((nodp, addr(0x17e)));
extern volatile uint8_t io_byte_0x17f  __attribute__((nodp, addr(0x17f)));
extern volatile uint8_t io_byte_0x180  __attribute__((nodp, addr(0x180)));
extern volatile uint8_t io_byte_0x181  __attribute__((nodp, addr(0x181)));
extern volatile uint8_t io_byte_0x182  __attribute__((nodp, addr(0x182)));
extern volatile uint8_t io_byte_0x183  __attribute__((nodp, addr(0x183)));
extern volatile uint8_t io_byte_0x184  __attribute__((nodp, addr(0x184)));
extern volatile uint8_t io_byte_0x185  __attribute__((nodp, addr(0x185)));
extern volatile uint8_t io_byte_0x186  __attribute__((nodp, addr(0x186)));
extern volatile uint8_t io_byte_0x187  __attribute__((nodp, addr(0x187)));
extern volatile uint8_t io_byte_0x188  __attribute__((nodp, addr(0x188)));
extern volatile uint8_t io_byte_0x189  __attribute__((nodp, addr(0x189)));
extern volatile uint8_t io_byte_0x18a  __attribute__((nodp, addr(0x18a)));
extern volatile uint8_t io_byte_0x18b  __attribute__((nodp, addr(0x18b)));
extern volatile uint8_t io_byte_0x18c  __attribute__((nodp, addr(0x18c)));
extern volatile uint8_t io_byte_0x18d  __attribute__((nodp, addr(0x18d)));
extern volatile uint8_t io_byte_0x18e  __attribute__((nodp, addr(0x18e)));
extern volatile uint8_t io_byte_0x18f  __attribute__((nodp, addr(0x18f)));
extern volatile uint8_t io_byte_0x190  __attribute__((nodp, addr(0x190)));
extern volatile uint8_t io_byte_0x191  __attribute__((nodp, addr(0x191)));
extern volatile uint8_t io_byte_0x192  __attribute__((nodp, addr(0x192)));
extern volatile uint8_t io_byte_0x193  __attribute__((nodp, addr(0x193)));
extern volatile uint8_t io_byte_0x194  __attribute__((nodp, addr(0x194)));
extern volatile uint8_t io_byte_0x195  __attribute__((nodp, addr(0x195)));
extern volatile uint8_t io_byte_0x196  __attribute__((nodp, addr(0x196)));
extern volatile uint8_t io_byte_0x197  __attribute__((nodp, addr(0x197)));
extern volatile uint8_t io_byte_0x198  __attribute__((nodp, addr(0x198)));
extern volatile uint8_t io_byte_0x199  __attribute__((nodp, addr(0x199)));
extern volatile uint8_t io_byte_0x19a  __attribute__((nodp, addr(0x19a)));
extern volatile uint8_t io_byte_0x19b  __attribute__((nodp, addr(0x19b)));
extern volatile uint8_t io_byte_0x19c  __attribute__((nodp, addr(0x19c)));
extern volatile uint8_t io_byte_0x19d  __attribute__((nodp, addr(0x19d)));
extern volatile uint8_t io_byte_0x19e  __attribute__((nodp, addr(0x19e)));
extern volatile uint8_t io_byte_0x19f  __attribute__((nodp, addr(0x19f)));
extern volatile uint8_t io_byte_0x1a0  __attribute__((nodp, addr(0x1a0)));
extern volatile uint8_t io_byte_0x1a1  __attribute__((nodp, addr(0x1a1)));
extern volatile uint8_t io_byte_0x1a2  __attribute__((nodp, addr(0x1a2)));
extern volatile uint8_t io_byte_0x1a3  __attribute__((nodp, addr(0x1a3)));
extern volatile uint8_t io_byte_0x1a4  __attribute__((nodp, addr(0x1a4)));
extern volatile uint8_t io_byte_0x1a5  __attribute__((nodp, addr(0x1a5)));
extern volatile uint8_t io_byte_0x1a6  __attribute__((nodp, addr(0x1a6)));
extern volatile uint8_t io_byte_0x1a7  __attribute__((nodp, addr(0x1a7)));
extern volatile uint8_t io_byte_0x1a8  __attribute__((nodp, addr(0x1a8)));
extern volatile uint8_t io_byte_0x1a9  __attribute__((nodp, addr(0x1a9)));
extern volatile uint8_t io_byte_0x1aa  __attribute__((nodp, addr(0x1aa)));
extern volatile uint8_t io_byte_0x1ab  __attribute__((nodp, addr(0x1ab)));
extern volatile uint8_t io_byte_0x1ac  __attribute__((nodp, addr(0x1ac)));
extern volatile uint8_t io_byte_0x1ad  __attribute__((nodp, addr(0x1ad)));
extern volatile uint8_t io_byte_0x1ae  __attribute__((nodp, addr(0x1ae)));
extern volatile uint8_t io_byte_0x1af  __attribute__((nodp, addr(0x1af)));
extern volatile uint8_t io_byte_0x1b0  __attribute__((nodp, addr(0x1b0)));
extern volatile uint8_t io_byte_0x1b1  __attribute__((nodp, addr(0x1b1)));
extern volatile uint8_t io_byte_0x1b2  __attribute__((nodp, addr(0x1b2)));
extern volatile uint8_t io_byte_0x1b3  __attribute__((nodp, addr(0x1b3)));
extern volatile uint8_t io_byte_0x1b4  __attribute__((nodp, addr(0x1b4)));
extern volatile uint8_t io_byte_0x1b5  __attribute__((nodp, addr(0x1b5)));
extern volatile uint8_t io_byte_0x1b6  __attribute__((nodp, addr(0x1b6)));
extern volatile uint8_t io_byte_0x1b7  __attribute__((nodp, addr(0x1b7)));
extern volatile uint8_t io_byte_0x1b8  __attribute__((nodp, addr(0x1b8)));
extern volatile uint8_t io_byte_0x1b9  __attribute__((nodp, addr(0x1b9)));
extern volatile uint8_t io_byte_0x1ba  __attribute__((nodp, addr(0x1ba)));
extern volatile uint8_t io_byte_0x1bb  __attribute__((nodp, addr(0x1bb)));
extern volatile uint8_t io_byte_0x1bc  __attribute__((nodp, addr(0x1bc)));
extern volatile uint8_t io_byte_0x1bd  __attribute__((nodp, addr(0x1bd)));
extern volatile uint8_t io_byte_0x1be  __attribute__((nodp, addr(0x1be)));
extern volatile uint8_t io_byte_0x1bf  __attribute__((nodp, addr(0x1bf)));
extern IO_CONST volatile uint8_t error_IO_CUSTOM_91804__FREQCNT_OUT_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t io_byte_0x1c2  __attribute__((nodp, addr(0x1c2)));
extern IO_CONST volatile uint8_t io_byte_0x1c3  __attribute__((nodp, addr(0x1c3)));
extern volatile uint8_t io_byte_0x1c4  __attribute__((nodp, addr(0x1c4)));
extern volatile uint8_t io_byte_0x1c5  __attribute__((nodp, addr(0x1c5)));
extern volatile uint8_t io_byte_0x1c6  __attribute__((nodp, addr(0x1c6)));
extern volatile uint8_t io_byte_0x1c7  __attribute__((nodp, addr(0x1c7)));
extern volatile uint8_t error_IO_CUSTOM_91804__RFTX_PATTERN_L_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_CUSTOM_91804__RFTX_PATTERN_H_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x1cc  __attribute__((nodp, addr(0x1cc)));
extern volatile uint8_t io_byte_0x1cd  __attribute__((nodp, addr(0x1cd)));
extern volatile uint8_t io_byte_0x1ce  __attribute__((nodp, addr(0x1ce)));
extern volatile uint8_t io_byte_0x1cf  __attribute__((nodp, addr(0x1cf)));
extern volatile uint8_t error_IO_CUSTOM_91804__RFTX_CRC_INIT_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x1d2  __attribute__((nodp, addr(0x1d2)));
extern volatile uint8_t io_byte_0x1d3  __attribute__((nodp, addr(0x1d3)));
extern volatile uint8_t io_byte_0x1d4  __attribute__((nodp, addr(0x1d4)));
extern volatile uint8_t io_byte_0x1d5  __attribute__((nodp, addr(0x1d5)));
extern volatile uint8_t io_byte_0x1d6  __attribute__((nodp, addr(0x1d6)));
extern volatile uint8_t io_byte_0x1d7  __attribute__((nodp, addr(0x1d7)));
extern volatile uint8_t error_IO_CUSTOM_91804__RFTX_CENTER_FREQ_L_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x1da  __attribute__((nodp, addr(0x1da)));
extern volatile uint8_t io_byte_0x1db  __attribute__((nodp, addr(0x1db)));
extern volatile uint8_t io_byte_0x1dc  __attribute__((nodp, addr(0x1dc)));
extern volatile uint8_t io_byte_0x1dd  __attribute__((nodp, addr(0x1dd)));
extern volatile uint8_t io_byte_0x1de  __attribute__((nodp, addr(0x1de)));
extern volatile uint8_t io_byte_0x1df  __attribute__((nodp, addr(0x1df)));
extern volatile uint8_t io_byte_0x1e0  __attribute__((nodp, addr(0x1e0)));
extern volatile uint8_t io_byte_0x1e1  __attribute__((nodp, addr(0x1e1)));
extern volatile uint8_t io_byte_0x1e2  __attribute__((nodp, addr(0x1e2)));
extern volatile uint8_t io_byte_0x1e3  __attribute__((nodp, addr(0x1e3)));
extern volatile uint8_t io_byte_0x1e4  __attribute__((nodp, addr(0x1e4)));
extern volatile uint8_t io_byte_0x1e5  __attribute__((nodp, addr(0x1e5)));
extern volatile uint8_t io_byte_0x1e6  __attribute__((nodp, addr(0x1e6)));
extern volatile uint8_t io_byte_0x1e7  __attribute__((nodp, addr(0x1e7)));
extern volatile uint8_t io_byte_0x1e8  __attribute__((nodp, addr(0x1e8)));
extern volatile uint8_t io_byte_0x1e9  __attribute__((nodp, addr(0x1e9)));
extern volatile uint8_t io_byte_0x1ea  __attribute__((nodp, addr(0x1ea)));
extern volatile uint8_t io_byte_0x1eb  __attribute__((nodp, addr(0x1eb)));
extern volatile uint8_t io_byte_0x1ec  __attribute__((nodp, addr(0x1ec)));
extern volatile uint8_t io_byte_0x1ed  __attribute__((nodp, addr(0x1ed)));
extern volatile uint8_t error_IO_CUSTOM_91804__LFRX_HDR_L_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_CUSTOM_91804__LFRX_HDR_H_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_CUSTOM_91804__LFRX_HDR_MASK_L_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_CUSTOM_91804__LFRX_HDR_MASK_H_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_CUSTOM_91804__LFRX_RXD_HDR_L_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_CUSTOM_91804__LFRX_RXD_HDR_H_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x1fa  __attribute__((nodp, addr(0x1fa)));
extern volatile uint8_t io_byte_0x1fb  __attribute__((nodp, addr(0x1fb)));
extern volatile uint8_t io_byte_0x1fc  __attribute__((nodp, addr(0x1fc)));
extern volatile uint8_t io_byte_0x1fd  __attribute__((nodp, addr(0x1fd)));
extern volatile uint8_t io_byte_0x1fe  __attribute__((nodp, addr(0x1fe)));
extern volatile uint8_t io_byte_0x1ff  __attribute__((nodp, addr(0x1ff)));
extern volatile uint8_t io_byte_0x200  __attribute__((nodp, addr(0x200)));
extern volatile uint8_t io_byte_0x201  __attribute__((nodp, addr(0x201)));
extern volatile uint8_t io_byte_0x202  __attribute__((nodp, addr(0x202)));
extern volatile uint8_t io_byte_0x203  __attribute__((nodp, addr(0x203)));
extern volatile uint8_t io_byte_0x204  __attribute__((nodp, addr(0x204)));
extern volatile uint8_t io_byte_0x205  __attribute__((nodp, addr(0x205)));
extern volatile uint8_t io_byte_0x206  __attribute__((nodp, addr(0x206)));
extern volatile uint8_t io_byte_0x207  __attribute__((nodp, addr(0x207)));
extern volatile uint8_t io_byte_0x208  __attribute__((nodp, addr(0x208)));
extern volatile uint8_t io_byte_0x209  __attribute__((nodp, addr(0x209)));
extern volatile uint8_t io_byte_0x20a  __attribute__((nodp, addr(0x20a)));
extern volatile uint8_t io_byte_0x20b  __attribute__((nodp, addr(0x20b)));
extern volatile uint8_t io_byte_0x20c  __attribute__((nodp, addr(0x20c)));
extern volatile uint8_t io_byte_0x20d  __attribute__((nodp, addr(0x20d)));
extern volatile uint8_t io_byte_0x20e  __attribute__((nodp, addr(0x20e)));
extern volatile uint8_t io_byte_0x20f  __attribute__((nodp, addr(0x20f)));
extern volatile uint8_t io_byte_0x210  __attribute__((nodp, addr(0x210)));
extern volatile uint8_t io_byte_0x211  __attribute__((nodp, addr(0x211)));
extern volatile uint8_t io_byte_0x212  __attribute__((nodp, addr(0x212)));
extern volatile uint8_t io_byte_0x213  __attribute__((nodp, addr(0x213)));
extern volatile uint8_t io_byte_0x214  __attribute__((nodp, addr(0x214)));
extern volatile uint8_t io_byte_0x215  __attribute__((nodp, addr(0x215)));
extern volatile uint8_t io_byte_0x216  __attribute__((nodp, addr(0x216)));
extern volatile uint8_t io_byte_0x217  __attribute__((nodp, addr(0x217)));
extern volatile uint8_t io_byte_0x218  __attribute__((nodp, addr(0x218)));
extern volatile uint8_t io_byte_0x219  __attribute__((nodp, addr(0x219)));
extern volatile uint8_t io_byte_0x21a  __attribute__((nodp, addr(0x21a)));
extern volatile uint8_t io_byte_0x21b  __attribute__((nodp, addr(0x21b)));
extern volatile uint8_t io_byte_0x21c  __attribute__((nodp, addr(0x21c)));
extern volatile uint8_t io_byte_0x21d  __attribute__((nodp, addr(0x21d)));
extern volatile uint8_t io_byte_0x21e  __attribute__((nodp, addr(0x21e)));
extern volatile uint8_t io_byte_0x21f  __attribute__((nodp, addr(0x21f)));
extern volatile uint8_t io_byte_0x220  __attribute__((nodp, addr(0x220)));
extern volatile uint8_t io_byte_0x221  __attribute__((nodp, addr(0x221)));
extern volatile uint8_t io_byte_0x222  __attribute__((nodp, addr(0x222)));
extern volatile uint8_t io_byte_0x223  __attribute__((nodp, addr(0x223)));
extern volatile uint8_t io_byte_0x224  __attribute__((nodp, addr(0x224)));
extern volatile uint8_t io_byte_0x225  __attribute__((nodp, addr(0x225)));
extern volatile uint8_t io_byte_0x226  __attribute__((nodp, addr(0x226)));
extern volatile uint8_t io_byte_0x227  __attribute__((nodp, addr(0x227)));
extern volatile uint8_t io_byte_0x228  __attribute__((nodp, addr(0x228)));
extern volatile uint8_t io_byte_0x229  __attribute__((nodp, addr(0x229)));
extern volatile uint8_t io_byte_0x22a  __attribute__((nodp, addr(0x22a)));
extern volatile uint8_t io_byte_0x22b  __attribute__((nodp, addr(0x22b)));
extern volatile uint8_t io_byte_0x22c  __attribute__((nodp, addr(0x22c)));
extern volatile uint8_t io_byte_0x22d  __attribute__((nodp, addr(0x22d)));
extern volatile uint8_t io_byte_0x22e  __attribute__((nodp, addr(0x22e)));
extern volatile uint8_t io_byte_0x22f  __attribute__((nodp, addr(0x22f)));
extern volatile uint8_t io_byte_0x230  __attribute__((nodp, addr(0x230)));
extern volatile uint8_t io_byte_0x231  __attribute__((nodp, addr(0x231)));
extern volatile uint8_t io_byte_0x232  __attribute__((nodp, addr(0x232)));
extern volatile uint8_t io_byte_0x233  __attribute__((nodp, addr(0x233)));
extern volatile uint8_t io_byte_0x234  __attribute__((nodp, addr(0x234)));
extern volatile uint8_t io_byte_0x235  __attribute__((nodp, addr(0x235)));
extern volatile uint8_t io_byte_0x236  __attribute__((nodp, addr(0x236)));
extern volatile uint8_t io_byte_0x237  __attribute__((nodp, addr(0x237)));
extern volatile uint8_t io_byte_0x238  __attribute__((nodp, addr(0x238)));
extern volatile uint8_t io_byte_0x239  __attribute__((nodp, addr(0x239)));
extern volatile uint8_t io_byte_0x23a  __attribute__((nodp, addr(0x23a)));
extern volatile uint8_t io_byte_0x23b  __attribute__((nodp, addr(0x23b)));
extern volatile uint8_t io_byte_0x23c  __attribute__((nodp, addr(0x23c)));
extern volatile uint8_t io_byte_0x23d  __attribute__((nodp, addr(0x23d)));
extern volatile uint8_t io_byte_0x23e  __attribute__((nodp, addr(0x23e)));
extern volatile uint8_t io_byte_0x23f  __attribute__((nodp, addr(0x23f)));
extern volatile uint8_t io_byte_0x240  __attribute__((nodp, addr(0x240)));
extern volatile uint8_t io_byte_0x241  __attribute__((nodp, addr(0x241)));
extern volatile uint8_t io_byte_0x242  __attribute__((nodp, addr(0x242)));


/*****************************************************************************/
/* 3.2 Get macros. */

#define IO_RST_CTRL__AWD_WBOOT_GET() \
	((io0x0 & 0x0001u) >> 0U)
#define IO_RST_CTRL__SOFT_WBOOT_GET() \
	((io0x0 & 0x0002u) >> 1U)
#define IO_RST_CTRL__HVDIG_WBOOT_GET() \
	((io0x0 & 0x0004u) >> 2U)
#define IO_RST_CTRL__DBG_WBOOT_GET() \
	((io0x0 & 0x0008u) >> 3U)
#define IO_RST_CTRL__SOFT_RESET_GET() \
	((io0x0 & 0x2000u) >> 13U)
#define IO_RST_CTRL__HVDIG_USED_GET() \
	((io0x0 & 0x4000u) >> 14U)
#define IO_RST_CTRL__HVDIG_OK_GET() \
	((io0x0 & 0x8000u) >> 15U)
#define IO_FUNC_TEST__DISABLE_RESET_GET() \
	((io0x4 & 0x0001u) >> 0U)
#define IO_FUNC_TEST__DISABLE_SUSPEND_GET() \
	((io0x4 & 0x0002u) >> 1U)
#define IO_FUNC_TEST__DISABLE_IRQ_GET() \
	((io0x4 & 0x0004u) >> 2U)
#define IO_FUNC_TEST__FTC_SEL_GET() \
	((io0x4 & 0x0300u) >> 8U)
#define IO_FUNC_TEST__FTC_DIV_GET() \
	((io0x4 & 0x3c00u) >> 10U)
#define IO_FUNC_TEST__TEST_UNPROT_GET() \
	(io0x6 >> 0U)
#define IO_MUPET__PTC_RECEIVE_GET() \
	((io0x8 & 0xffffu) >> 0U)
#define IO_MUPET__PTC_SEND_GET() \
	((io0x8 & 0xffffu) >> 0U)
#define IO_MUPET__PTCA_PEND_GET() \
	((io0xa & 0x0001u) >> 0U)
#define IO_MUPET__PTCS_PEND_GET() \
	((io0xa & 0x0002u) >> 1U)
#define IO_MUPET__PTCR_PEND_GET() \
	((io0xa & 0x0004u) >> 2U)
#define IO_MUPET__PTC_KEY_GET() \
	((io0xa & 0x0080u) >> 7U)
#define IO_MUPET__WARM_TRIGGER_GET() \
	((io0xa & 0x1f00u) >> 8U)
#define IO_MUPET__CONNECTION_GET() \
	((io0xa & 0x6000u) >> 13U)
#define IO_MUPET__IN_APPLICATION_GET() \
	((io0xa & 0x8000u) >> 15U)
static __attribute__((always_inline)) inline uint16_t IO_AWD__ACK_GET (void) {
	register uint16_t result = 0u;
	asm volatile (
		"mov c, io:0xc.6\n"
		"adc %[acc], #0" : [acc]"=r"(result) : "0"(result));
	return result;
}
static __attribute__((always_inline)) inline uint16_t IO_AWD__WIN_OPEN_GET (void) {
	register uint16_t result = 0u;
	asm volatile (
		"mov c, io:0xd.7\n"
		"adc %[acc], #0" : [acc]"=r"(result) : "0"(result));
	return result;
}
#define IO_ROM_BIST__ADD_START_L_GET() \
	(io0x10 >> 0U)
#define IO_ROM_BIST__ADD_START_H_GET() \
	(io0x12 >> 0U)
#define IO_ROM_BIST__ADD_STOP_L_GET() \
	(io0x14 >> 0U)
#define IO_ROM_BIST__ADD_STOP_H_GET() \
	(io0x16 >> 0U)
#define IO_ROM_BIST__SIG_EXPECTED_L_GET() \
	(io0x18 >> 0U)
#define IO_ROM_BIST__SIG_EXPECTED_H_GET() \
	(io0x1a >> 0U)
#define IO_ROM_BIST__ECC_POSITION_GET() \
	((io0x1c & 0x0003u) >> 0U)
#define IO_ROM_BIST__BIST_GET() \
	((io0x1c & 0x0010u) >> 4U)
#define IO_ROM_BIST__SINGLE_RAMP_GET() \
	((io0x1c & 0x0020u) >> 5U)
#define IO_ROM_BIST__MASK_SIG_ERR_GET() \
	((io0x1c & 0x0040u) >> 6U)
#define IO_ROM_BIST__BIST_REQUEST_GET() \
	((io0x1c & 0x0200u) >> 9U)
#define IO_ROM_BIST__VALID_CLOCK_GET() \
	((io0x1c & 0x0800u) >> 11U)
#define IO_ROM_BIST__COMPLETED_GET() \
	((io0x1c & 0x8000u) >> 15U)
#define IO_ROM_BIST__START_BIST_GET() \
	(io0x1e >> 0U)
#define IO_ROM_BIST__SIG_RECEIVED_L_GET() \
	(io0x20 >> 0U)
#define IO_ROM_BIST__SIG_RECEIVED_H_GET() \
	(io0x22 >> 0U)
#define IO_NVRAM_SHELL__SEC_GET() \
	((io0x26 & 0x0004u) >> 2U)
#define IO_NVRAM_SHELL__DED_GET() \
	((io0x26 & 0x0008u) >> 3U)
#define IO_NVRAM_SHELL__STORE_REQ_GET() \
	((io0x26 & 0x0100u) >> 8U)
#define IO_NVRAM_SHELL__RECALL_REQ_GET() \
	((io0x26 & 0x0200u) >> 9U)
#define IO_NVRAM_SHELL__LOCK_GET() \
	(io0x28 >> 15U)
#define IO_NVRAM_SHELL__DMA_DATA_GET() \
	((io0x2a & 0x0001u) >> 0U)
#define IO_NVRAM_SHELL__DMA_CB_GET() \
	((io0x2a & 0x0002u) >> 1U)
#define IO_NVRAM_SHELL__TM_BIT_VFY_GET() \
	((io0x2a & 0x0004u) >> 2U)
#define IO_NVRAM_SHELL__TM_FULL_GET() \
	((io0x2a & 0x0008u) >> 3U)
#define IO_NVRAM_SHELL__PEIN_GET() \
	((io0x2a & 0x0010u) >> 4U)
#define IO_NVRAM_SHELL__TM_NVCP_GET() \
	((io0x2a & 0x01e0u) >> 5U)
#define IO_NVRAM_SHELL__TM_STORE_REQ_GET() \
	((io0x2a & 0x0600u) >> 9U)
#define IO_NVRAM_SHELL__TM_RECALL_REQ_GET() \
	((io0x2a & 0x1800u) >> 11U)
#define IO_STIMER1__CURRENT_GET() \
	(io0x2c >> 0U)
#define IO_STIMER1__VALUE_GET() \
	((io0x2e & 0x3fffu) >> 0U)
#define IO_STIMER1__MODE_GET() \
	((io0x2e & 0xc000u) >> 14U)
#define IO_STIMER2__CURRENT_GET() \
	(io0x30 >> 0U)
#define IO_STIMER2__VALUE_GET() \
	((io0x32 & 0x3fffu) >> 0U)
#define IO_STIMER2__MODE_GET() \
	((io0x32 & 0xc000u) >> 14U)
#define IO_MLX16__XCG_STAT_GET() \
	(io0x38 >> 0U)
#define IO_MLX16__XCG_DR1_GET() \
	(io0x3a >> 0U)
#define IO_MLX16__XCG_DR3_GET() \
	(io0x3c >> 0U)
#define IO_MLX16__SHELL_VERSION_GET() \
	(io0x3e >> 0U)
#define IO_MLX16__ITC_PEND0_GET() \
	((io0x40 & 0xffffu) >> 0U)
#define IO_MLX16__MLX16_EXCHG_PEND_GET() \
	((io0x40 & 0x0001u) >> 0U)
#define IO_MLX16__MLX16_DMAERR_PEND_GET() \
	((io0x40 & 0x0002u) >> 1U)
#define IO_MLX16__AWD_ATT_PEND_GET() \
	((io0x40 & 0x0004u) >> 2U)
#define IO_MLX16__ROM_SHELL_MEM_ERR_PEND_GET() \
	((io0x40 & 0x0008u) >> 3U)
#define IO_MLX16__FLASH_SHELL_ERR_PEND_GET() \
	((io0x40 & 0x0010u) >> 4U)
#define IO_MLX16__RAM_SHELL_MEM_ERR_PEND_GET() \
	((io0x40 & 0x0020u) >> 5U)
#define IO_MLX16__NVRAM_SHELL_STORE_PEND_GET() \
	((io0x40 & 0x0040u) >> 6U)
#define IO_MLX16__STIMER1_PEND_GET() \
	((io0x40 & 0x0080u) >> 7U)
#define IO_MLX16__STIMER2_PEND_GET() \
	((io0x40 & 0x0100u) >> 8U)
#define IO_MLX16__ADC_XH018_PEND_GET() \
	((io0x40 & 0x0200u) >> 9U)
#define IO_MLX16__GPIO0_PEND_GET() \
	((io0x40 & 0x0400u) >> 10U)
#define IO_MLX16__GPIO1_PEND_GET() \
	((io0x40 & 0x0800u) >> 11U)
#define IO_MLX16__GPIO2_PEND_GET() \
	((io0x40 & 0x1000u) >> 12U)
#define IO_MLX16__GPIO3_PEND_GET() \
	((io0x40 & 0x2000u) >> 13U)
#define IO_MLX16__DIAG0_PEND_GET() \
	((io0x40 & 0x4000u) >> 14U)
#define IO_MLX16__DIAG1_PEND_GET() \
	((io0x40 & 0x8000u) >> 15U)
#define IO_MLX16__DIAG2_PEND_GET() \
	((io0x42 & 0x0001u) >> 0U)
#define IO_MLX16__ITC_PEND1_GET() \
	((io0x42 & 0xffffu) >> 0U)
#define IO_MLX16__DIAG3_PEND_GET() \
	((io0x42 & 0x0002u) >> 1U)
#define IO_MLX16__WUPTMR_PEND_GET() \
	((io0x42 & 0x0004u) >> 2U)
#define IO_MLX16__TEMPSENS_PEND_GET() \
	((io0x42 & 0x0008u) >> 3U)
#define IO_MLX16__RFTX_LOCKED_PEND_GET() \
	((io0x42 & 0x0010u) >> 4U)
#define IO_MLX16__RFTX_HALF_PEND_GET() \
	((io0x42 & 0x0020u) >> 5U)
#define IO_MLX16__RFTX_DONE_PEND_GET() \
	((io0x42 & 0x0040u) >> 6U)
#define IO_MLX16__LFTX_HALF_PEND_GET() \
	((io0x42 & 0x0080u) >> 7U)
#define IO_MLX16__LFTX_DONE_PEND_GET() \
	((io0x42 & 0x0100u) >> 8U)
#define IO_MLX16__LFRX_SNIFF_PEND_GET() \
	((io0x42 & 0x0200u) >> 9U)
#define IO_MLX16__LFRX_TIMEOUT_PEND_GET() \
	((io0x42 & 0x0400u) >> 10U)
#define IO_MLX16__LFRX_SYNC_PEND_GET() \
	((io0x42 & 0x0800u) >> 11U)
#define IO_MLX16__LFRX_DATA_PEND_GET() \
	((io0x42 & 0x1000u) >> 12U)
#define IO_MLX16__LFRX_DONE_PEND_GET() \
	((io0x42 & 0x2000u) >> 13U)
#define IO_MLX16__FREQCNT_DONE_PEND_GET() \
	((io0x42 & 0x4000u) >> 14U)
#define IO_MLX16__MLX16_SOFT_PEND_GET() \
	((io0x42 & 0x8000u) >> 15U)
#define IO_MLX16__SWI_GET() \
	(io0x5a >> 15U)
#define IO_MLX16__ITC_MASK0_GET() \
	((io0x5c & 0xffffu) >> 0U)
#define IO_MLX16__MLX16_EXCHG_ITC_GET() \
	((io0x5c & 0x0001u) >> 0U)
#define IO_MLX16__MLX16_DMAERR_ITC_GET() \
	((io0x5c & 0x0002u) >> 1U)
#define IO_MLX16__AWD_ATT_ITC_GET() \
	((io0x5c & 0x0004u) >> 2U)
#define IO_MLX16__ROM_SHELL_MEM_ERR_ITC_GET() \
	((io0x5c & 0x0008u) >> 3U)
#define IO_MLX16__FLASH_SHELL_ERR_ITC_GET() \
	((io0x5c & 0x0010u) >> 4U)
#define IO_MLX16__RAM_SHELL_MEM_ERR_ITC_GET() \
	((io0x5c & 0x0020u) >> 5U)
#define IO_MLX16__NVRAM_SHELL_STORE_ITC_GET() \
	((io0x5c & 0x0040u) >> 6U)
#define IO_MLX16__STIMER1_ITC_GET() \
	((io0x5c & 0x0080u) >> 7U)
#define IO_MLX16__STIMER2_ITC_GET() \
	((io0x5c & 0x0100u) >> 8U)
#define IO_MLX16__ADC_XH018_ITC_GET() \
	((io0x5c & 0x0200u) >> 9U)
#define IO_MLX16__GPIO0_ITC_GET() \
	((io0x5c & 0x0400u) >> 10U)
#define IO_MLX16__GPIO1_ITC_GET() \
	((io0x5c & 0x0800u) >> 11U)
#define IO_MLX16__GPIO2_ITC_GET() \
	((io0x5c & 0x1000u) >> 12U)
#define IO_MLX16__GPIO3_ITC_GET() \
	((io0x5c & 0x2000u) >> 13U)
#define IO_MLX16__DIAG0_ITC_GET() \
	((io0x5c & 0x4000u) >> 14U)
#define IO_MLX16__DIAG1_ITC_GET() \
	((io0x5c & 0x8000u) >> 15U)
#define IO_MLX16__DIAG2_ITC_GET() \
	((io0x5e & 0x0001u) >> 0U)
#define IO_MLX16__ITC_MASK1_GET() \
	((io0x5e & 0xffffu) >> 0U)
#define IO_MLX16__DIAG3_ITC_GET() \
	((io0x5e & 0x0002u) >> 1U)
#define IO_MLX16__WUPTMR_ITC_GET() \
	((io0x5e & 0x0004u) >> 2U)
#define IO_MLX16__TEMPSENS_ITC_GET() \
	((io0x5e & 0x0008u) >> 3U)
#define IO_MLX16__RFTX_LOCKED_ITC_GET() \
	((io0x5e & 0x0010u) >> 4U)
#define IO_MLX16__RFTX_HALF_ITC_GET() \
	((io0x5e & 0x0020u) >> 5U)
#define IO_MLX16__RFTX_DONE_ITC_GET() \
	((io0x5e & 0x0040u) >> 6U)
#define IO_MLX16__LFTX_HALF_ITC_GET() \
	((io0x5e & 0x0080u) >> 7U)
#define IO_MLX16__LFTX_DONE_ITC_GET() \
	((io0x5e & 0x0100u) >> 8U)
#define IO_MLX16__LFRX_SNIFF_ITC_GET() \
	((io0x5e & 0x0200u) >> 9U)
#define IO_MLX16__LFRX_TIMEOUT_ITC_GET() \
	((io0x5e & 0x0400u) >> 10U)
#define IO_MLX16__LFRX_SYNC_ITC_GET() \
	((io0x5e & 0x0800u) >> 11U)
#define IO_MLX16__LFRX_DATA_ITC_GET() \
	((io0x5e & 0x1000u) >> 12U)
#define IO_MLX16__LFRX_DONE_ITC_GET() \
	((io0x5e & 0x2000u) >> 13U)
#define IO_MLX16__FREQCNT_DONE_ITC_GET() \
	((io0x5e & 0x4000u) >> 14U)
#define IO_MLX16__MLX16_SOFT_ITC_GET() \
	((io0x5e & 0x8000u) >> 15U)
#define IO_MLX16__ITC_PRIO0_GET() \
	((io0x78 & 0xffffu) >> 0U)
#define IO_MLX16__ROM_SHELL_MEM_ERR_PRIO_GET() \
	((io0x78 & 0x0003u) >> 0U)
#define IO_MLX16__FLASH_SHELL_ERR_PRIO_GET() \
	((io0x78 & 0x000cu) >> 2U)
#define IO_MLX16__RAM_SHELL_MEM_ERR_PRIO_GET() \
	((io0x78 & 0x0030u) >> 4U)
#define IO_MLX16__NVRAM_SHELL_STORE_PRIO_GET() \
	((io0x78 & 0x00c0u) >> 6U)
#define IO_MLX16__STIMER1_PRIO_GET() \
	((io0x78 & 0x0300u) >> 8U)
#define IO_MLX16__STIMER2_PRIO_GET() \
	((io0x78 & 0x0c00u) >> 10U)
#define IO_MLX16__ADC_XH018_PRIO_GET() \
	((io0x78 & 0x3000u) >> 12U)
#define IO_MLX16__GPIO0_PRIO_GET() \
	((io0x78 & 0xc000u) >> 14U)
#define IO_MLX16__GPIO1_PRIO_GET() \
	((io0x7a & 0x0003u) >> 0U)
#define IO_MLX16__ITC_PRIO1_GET() \
	((io0x7a & 0xffffu) >> 0U)
#define IO_MLX16__GPIO2_PRIO_GET() \
	((io0x7a & 0x000cu) >> 2U)
#define IO_MLX16__GPIO3_PRIO_GET() \
	((io0x7a & 0x0030u) >> 4U)
#define IO_MLX16__DIAG0_PRIO_GET() \
	((io0x7a & 0x00c0u) >> 6U)
#define IO_MLX16__DIAG1_PRIO_GET() \
	((io0x7a & 0x0300u) >> 8U)
#define IO_MLX16__DIAG2_PRIO_GET() \
	((io0x7a & 0x0c00u) >> 10U)
#define IO_MLX16__DIAG3_PRIO_GET() \
	((io0x7a & 0x3000u) >> 12U)
#define IO_MLX16__WUPTMR_PRIO_GET() \
	((io0x7a & 0xc000u) >> 14U)
#define IO_MLX16__ITC_PRIO2_GET() \
	((io0x7c & 0xffffu) >> 0U)
#define IO_MLX16__TEMPSENS_PRIO_GET() \
	((io0x7c & 0x0003u) >> 0U)
#define IO_MLX16__RFTX_LOCKED_PRIO_GET() \
	((io0x7c & 0x000cu) >> 2U)
#define IO_MLX16__RFTX_HALF_PRIO_GET() \
	((io0x7c & 0x0030u) >> 4U)
#define IO_MLX16__RFTX_DONE_PRIO_GET() \
	((io0x7c & 0x00c0u) >> 6U)
#define IO_MLX16__LFTX_HALF_PRIO_GET() \
	((io0x7c & 0x0300u) >> 8U)
#define IO_MLX16__LFTX_DONE_PRIO_GET() \
	((io0x7c & 0x0c00u) >> 10U)
#define IO_MLX16__LFRX_SNIFF_PRIO_GET() \
	((io0x7c & 0x3000u) >> 12U)
#define IO_MLX16__LFRX_TIMEOUT_PRIO_GET() \
	((io0x7c & 0xc000u) >> 14U)
#define IO_MLX16__ITC_PRIO3_GET() \
	((io0x7e & 0x00ffu) >> 0U)
#define IO_MLX16__LFRX_SYNC_PRIO_GET() \
	((io0x7e & 0x0003u) >> 0U)
#define IO_MLX16__LFRX_DATA_PRIO_GET() \
	((io0x7e & 0x000cu) >> 2U)
#define IO_MLX16__LFRX_DONE_PRIO_GET() \
	((io0x7e & 0x0030u) >> 4U)
#define IO_MLX16__FREQCNT_DONE_PRIO_GET() \
	((io0x7e & 0x00c0u) >> 6U)
#define IO_MLX16__CPU_FP0ADR_GET() \
	(io0xae >> 0U)
#define IO_MLX16__PATCH_ADDR0_GET() \
	(io0xb0 >> 0U)
#define IO_MLX16__PATCH_INSTR0_GET() \
	(io0xb2 >> 0U)
#define IO_MLX16__PATCH_ADDR1_GET() \
	(io0xb4 >> 0U)
#define IO_MLX16__PATCH_INSTR1_GET() \
	(io0xb6 >> 0U)
#define IO_MLX16__PATCH_ADDR2_GET() \
	(io0xb8 >> 0U)
#define IO_MLX16__PATCH_INSTR2_GET() \
	(io0xba >> 0U)
#define IO_MLX16__PATCH_ADDR3_GET() \
	(io0xbc >> 0U)
#define IO_MLX16__PATCH_INSTR3_GET() \
	(io0xbe >> 0U)
#define IO_VERSION__VERSION_L_GET() \
	(io0x100 >> 0U)
#define IO_VERSION__VERSION_H_GET() \
	(io0x102 >> 0U)
#define IO_FLASH_SHELL__COMMAND_GET() \
	((io0x104 & 0xffffu) >> 0U)
#define IO_FLASH_SHELL__STATUS_GET() \
	((io0x104 & 0x001fu) >> 0U)
#define IO_FLASH_SHELL__VERSION_GET() \
	((io0x104 & 0xff00u) >> 8U)
#define IO_FLASH_SHELL__WR_TIME_GET() \
	((io0x106 & 0x007fu) >> 0U)
#define IO_FLASH_SHELL__ER_TIME_GET() \
	((io0x106 & 0x7f00u) >> 8U)
#define IO_FLASH_SHELL__LOCK_ER_WR_GET() \
	((io0x106 & 0x8000u) >> 15U)
#define IO_FLASH_SHELL__T_50NS_GET() \
	((io0x108 & 0x0007u) >> 0U)
#define IO_FLASH_SHELL__T_30US_GET() \
	((io0x108 & 0x3f00u) >> 8U)
#define IO_FLASH_SHELL__LOCK_T_GET() \
	((io0x108 & 0x8000u) >> 15U)
#define IO_FLASH_SHELL__READY_GET() \
	((io0x10a & 0x000fu) >> 0U)
#define IO_FLASH_SHELL__DED_RETRY_GET() \
	((io0x10a & 0x0070u) >> 4U)
#define IO_FLASH_SHELL__BYPASS_QUEUE_GET() \
	((io0x10a & 0x0100u) >> 8U)
#define IO_FLASH_SHELL__SYNCHRONOUS_GET() \
	((io0x10a & 0x0200u) >> 9U)
#define IO_FLASH_SHELL__HALT_BEHAVIOR_GET() \
	((io0x10a & 0x0c00u) >> 10U)
#define IO_FLASH_SHELL__LOCK_RDY_GET() \
	((io0x10a & 0x8000u) >> 15U)
#define IO_FLASH_SHELL__SEC_COUNTER_GET() \
	(io0x10c >> 0U)
#define IO_FLASH_SHELL__BIT_ERRORS_GET() \
	((io0x10e & 0x0fffu) >> 0U)
#define IO_FLASH_SHELL__PATTERN_ID_GET() \
	((io0x10e & 0x7000u) >> 12U)
#define IO_FLASH_SHELL__PL_TRL_GET() \
	(io0x110 >> 0U)
#define IO_FLASH_SHELL__TRH_GET() \
	(io0x112 >> 0U)
#define IO_FLASH_SHELL__PL_TR_AD_GET() \
	(io0x114 >> 0U)
#define IO_FLASH_SHELL__T_TEST_GET() \
	((io0x116 & 0x0001u) >> 0U)
#define IO_FLASH_SHELL__T_CLK_GET() \
	((io0x116 & 0x0002u) >> 1U)
#define IO_FLASH_SHELL__T_DESELECT_GET() \
	((io0x116 & 0x0f00u) >> 8U)
#define IO_FLASH_SHELL__FL1_D_ATD_GET() \
	((io0x11a & 0x00ffu) >> 0U)
#define IO_FLASH_SHELL__FL1_SW_A_RM_GET() \
	((io0x11a & 0x0f00u) >> 8U)
#define IO_FLASH_SHELL__SIG_L_GET() \
	(io0x13a >> 0U)
#define IO_FLASH_SHELL__SIG_H_GET() \
	(io0x13c >> 0U)
#define IO_RAM_BIST__KEY_GET() \
	((io0x13e & 0xffffu) >> 0U)
#define IO_RAM_BIST__PHASE_GET() \
	((io0x13e & 0x0007u) >> 0U)
#define IO_RAM_BIST__TRANSPARENT_GET() \
	((io0x13e & 0x0100u) >> 8U)
#define IO_RAM_BIST__REGULAR_GET() \
	((io0x13e & 0x0200u) >> 9U)
#define IO_RAM_BIST__RUNNING_GET() \
	((io0x13e & 0x0400u) >> 10U)
#define IO_RAM_BIST__VALID_CLOCK_GET() \
	((io0x13e & 0x0800u) >> 11U)
#define IO_RAM_BIST__COMPLETED_GET() \
	((io0x13e & 0x8000u) >> 15U)
#define IO_RAM_BIST__LFSR_GET() \
	((io0x140 & 0xffffu) >> 0U)
#define IO_RAM_BIST__LSFR_GOT_GET() \
	((io0x140 & 0xffffu) >> 0U)
#define IO_RAM_BIST__LFSR_EXPECTED_GET() \
	(io0x142 >> 0U)
#define IO_RAM_BIST__ADL_GET() \
	(io0x144 >> 0U)
#define IO_RAM_BIST__ADH_GET() \
	(io0x146 >> 0U)
#define IO_RAM_BIST__ADD_START_L_GET() \
	(io0x148 >> 0U)
#define IO_RAM_BIST__ADD_START_H_GET() \
	(io0x14a >> 0U)
#define IO_RAM_BIST__ADD_STOP_L_GET() \
	(io0x14c >> 0U)
#define IO_RAM_BIST__ADD_STOP_H_GET() \
	(io0x14e >> 0U)
#define IO_RAM_BIST__NB_ECC_BITS_GET() \
	((io0x150 & 0x001fu) >> 0U)
#define IO_RAM_BIST__ADD_SCRAMBLE_GET() \
	((io0x150 & 0x0700u) >> 8U)
#define IO_RAM_BIST__SIGNATURE_INIT_GET() \
	((io0x150 & 0x1000u) >> 12U)
#define IO_ADC_XH018__START_GET() \
	((io0x152 & 0x0001u) >> 0U)
#define IO_ADC_XH018__STOP_GET() \
	((io0x152 & 0x0002u) >> 1U)
#define IO_ADC_XH018__SOS_SOURCE_GET() \
	((io0x152 & 0x000cu) >> 2U)
#define IO_ADC_XH018__SOC_SOURCE_GET() \
	((io0x152 & 0x0030u) >> 4U)
#define IO_ADC_XH018__NO_INTERLEAVE_GET() \
	((io0x152 & 0x0040u) >> 6U)
#define IO_ADC_XH018__SATURATE_GET() \
	((io0x152 & 0x0080u) >> 7U)
#define IO_ADC_XH018__INT_SCHEME_GET() \
	((io0x152 & 0x0300u) >> 8U)
#define IO_ADC_XH018__ASB_GET() \
	((io0x152 & 0x0c00u) >> 10U)
#define IO_ADC_XH018__ADC_WIDTH_GET() \
	((io0x152 & 0x1000u) >> 12U)
#define IO_ADC_XH018__SBASE_0_GET() \
	(io0x154 >> 0U)
#define IO_ADC_XH018__PAUSE_GET() \
	((io0x156 & 0x0001u) >> 0U)
#define IO_ADC_XH018__RESUME_GET() \
	((io0x156 & 0x0002u) >> 1U)
#define IO_ADC_XH018__SW_TRIG_GET() \
	((io0x156 & 0x0004u) >> 2U)
#define IO_ADC_XH018__LAST_INT_SRC_GET() \
	((io0x156 & 0x0030u) >> 4U)
#define IO_ADC_XH018__STATE_GET() \
	((io0x156 & 0x00c0u) >> 6U)
#define IO_ADC_XH018__ADC_OVF_GET() \
	((io0x156 & 0x0100u) >> 8U)
#define IO_ADC_XH018__ADC_ERR_GET() \
	((io0x156 & 0x0200u) >> 9U)
#define IO_ADC_XH018__MEM_ERR_GET() \
	((io0x156 & 0x0400u) >> 10U)
#define IO_ADC_XH018__FRAME_ERR_GET() \
	((io0x156 & 0x0800u) >> 11U)
#define IO_ADC_XH018__ABORTED_GET() \
	((io0x156 & 0x1000u) >> 12U)
#define IO_ADC_XH018__ADC_CLK_DIV_GET() \
	(io0x158 >> 0U)
#define IO_ADC_XH018__COUNT_GET() \
	((io0x15a & 0x0007u) >> 0U)
#define IO_ADC_XH018__TYPE_GET() \
	((io0x15a & 0x0008u) >> 3U)
#define IO_ADC_XH018__MODE_GET() \
	((io0x15a & 0x0010u) >> 4U)
#define IO_ADC_XH018__FR_GET() \
	((io0x15a & 0x0060u) >> 5U)
#define IO_ADC_XH018__START_PHI_GET() \
	((io0x15a & 0x0080u) >> 7U)
#define IO_ADC_XH018__OUTMODE_GET() \
	((io0x15a & 0x0100u) >> 8U)
#define IO_ADC_XH018__FORCE_GET() \
	((io0x15a & 0x0600u) >> 9U)
#define IO_ADC_XH018__NOCHOP_GET() \
	((io0x15a & 0x0800u) >> 11U)
#define IO_ADC_XH018__SRC_GET() \
	(io0x15c >> 0U)
#define IO_ADC_XH018__CORR_15_0_GET() \
	(io0x15e >> 0U)
#define IO_ADC_XH018__CORR_31_16_GET() \
	(io0x160 >> 0U)
#define IO_ADC_XH018__CORR_46_32_GET() \
	((io0x162 & 0x7fffu) >> 0U)
#define IO_ADC_XH018__START_CALIB_GET() \
	((io0x162 & 0x8000u) >> 15U)
#define IO_CUSTOM_91804__FLASH_TR0_GET() \
	(io0x164 >> 0U)
#define IO_CUSTOM_91804__FLASH_TR1_GET() \
	(io0x166 >> 0U)
#define IO_CUSTOM_91804__FLASH_TR2_GET() \
	(io0x168 >> 0U)
#define IO_CUSTOM_91804__FLASH_TR3_GET() \
	(io0x16a >> 0U)
#define IO_CUSTOM_91804__FLASH_TR4_GET() \
	(io0x16c >> 0U)
#define IO_CUSTOM_91804__FLASH_TR5_GET() \
	(io0x16e >> 0U)
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE0_GET() \
	((io0x170 & 0x00ffu) >> 0U)
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE1_GET() \
	((io0x170 & 0xff00u) >> 8U)
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE2_GET() \
	((io0x172 & 0x00ffu) >> 0U)
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE3_GET() \
	((io0x172 & 0xff00u) >> 8U)
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE4_GET() \
	((io0x174 & 0x00ffu) >> 0U)
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE5_GET() \
	((io0x174 & 0xff00u) >> 8U)
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE6_GET() \
	((io0x176 & 0x00ffu) >> 0U)
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE7_GET() \
	((io0x176 & 0xff00u) >> 8U)
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE8_GET() \
	((io0x178 & 0x00ffu) >> 0U)
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE9_GET() \
	((io0x178 & 0xff00u) >> 8U)
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE10_GET() \
	((io0x17a & 0x00ffu) >> 0U)
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE11_GET() \
	((io0x17a & 0xff00u) >> 8U)
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE12_GET() \
	((io0x17c & 0x00ffu) >> 0U)
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE13_GET() \
	((io0x17c & 0xff00u) >> 8U)
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE14_GET() \
	((io0x17e & 0x00ffu) >> 0U)
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE15_GET() \
	((io0x17e & 0xff00u) >> 8U)
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE16_GET() \
	((io0x180 & 0x00ffu) >> 0U)
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE17_GET() \
	((io0x180 & 0xff00u) >> 8U)
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE18_GET() \
	((io0x182 & 0x00ffu) >> 0U)
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE19_GET() \
	((io0x182 & 0xff00u) >> 8U)
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE20_GET() \
	((io0x184 & 0x00ffu) >> 0U)
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE21_GET() \
	((io0x184 & 0xff00u) >> 8U)
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE22_GET() \
	((io0x186 & 0x00ffu) >> 0U)
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE23_GET() \
	((io0x186 & 0xff00u) >> 8U)
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE24_GET() \
	((io0x188 & 0x00ffu) >> 0U)
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE25_GET() \
	((io0x188 & 0xff00u) >> 8U)
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE26_GET() \
	((io0x18a & 0x00ffu) >> 0U)
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE27_GET() \
	((io0x18a & 0xff00u) >> 8U)
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE28_GET() \
	((io0x18c & 0x00ffu) >> 0U)
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE29_GET() \
	((io0x18c & 0xff00u) >> 8U)
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE30_GET() \
	((io0x18e & 0x00ffu) >> 0U)
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE31_GET() \
	((io0x18e & 0xff00u) >> 8U)
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE32_GET() \
	((io0x190 & 0x00ffu) >> 0U)
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE33_GET() \
	((io0x190 & 0xff00u) >> 8U)
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE34_GET() \
	((io0x192 & 0x00ffu) >> 0U)
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE35_GET() \
	((io0x192 & 0xff00u) >> 8U)
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE36_GET() \
	((io0x194 & 0x00ffu) >> 0U)
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE37_GET() \
	((io0x194 & 0xff00u) >> 8U)
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE38_GET() \
	((io0x196 & 0x00ffu) >> 0U)
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE39_GET() \
	((io0x196 & 0xff00u) >> 8U)
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE40_GET() \
	((io0x198 & 0x00ffu) >> 0U)
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE41_GET() \
	((io0x198 & 0xff00u) >> 8U)
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE42_GET() \
	((io0x19a & 0x00ffu) >> 0U)
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE43_GET() \
	((io0x19a & 0xff00u) >> 8U)
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE44_GET() \
	((io0x19c & 0x00ffu) >> 0U)
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE45_GET() \
	((io0x19c & 0xff00u) >> 8U)
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE46_GET() \
	((io0x19e & 0x00ffu) >> 0U)
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE47_GET() \
	((io0x19e & 0xff00u) >> 8U)
#define IO_CUSTOM_91804__WUPTMR_MANT_GET() \
	((io0x1a0 & 0x00ffu) >> 0U)
#define IO_CUSTOM_91804__WUPTMR_EXP_GET() \
	((io0x1a0 & 0x0f00u) >> 8U)
#define IO_CUSTOM_91804__WUPTMR_RESTART_GET() \
	((io0x1a0 & 0x1000u) >> 12U)
#define IO_CUSTOM_91804__WUPTMR_FLAG_GET() \
	((io0x1a0 & 0x2000u) >> 13U)
#define IO_CUSTOM_91804__WOKEN_UP_GET() \
	((io0x1a0 & 0x4000u) >> 14U)
#define IO_CUSTOM_91804__HAD_POR_GET() \
	((io0x1a0 & 0x8000u) >> 15U)
#define IO_CUSTOM_91804__HALT_ACTION_GET() \
	((io0x1a2 & 0x0003u) >> 0U)
#define IO_CUSTOM_91804__LF_MODE_GET() \
	((io0x1a2 & 0x000cu) >> 2U)
#define IO_CUSTOM_91804__RFTX_MODE_GET() \
	((io0x1a2 & 0x00f0u) >> 4U)
#define IO_CUSTOM_91804__WAKE_RFTX_HALF_GET() \
	((io0x1a2 & 0x0100u) >> 8U)
#define IO_CUSTOM_91804__WAKE_RFTX_DONE_GET() \
	((io0x1a2 & 0x0200u) >> 9U)
#define IO_CUSTOM_91804__WAKE_LFTX_HALF_GET() \
	((io0x1a2 & 0x0400u) >> 10U)
#define IO_CUSTOM_91804__WAKE_LFRX_SNIFF_GET() \
	((io0x1a2 & 0x0800u) >> 11U)
#define IO_CUSTOM_91804__WAKE_LFRX_TIMEOUT_GET() \
	((io0x1a2 & 0x1000u) >> 12U)
#define IO_CUSTOM_91804__WAKE_LFRX_SYNC_GET() \
	((io0x1a2 & 0x2000u) >> 13U)
#define IO_CUSTOM_91804__WAKE_LFRX_DATA_GET() \
	((io0x1a2 & 0x4000u) >> 14U)
#define IO_CUSTOM_91804__WAKE_LF_DONE_GET() \
	((io0x1a2 & 0x8000u) >> 15U)
#define IO_CUSTOM_91804__POL_DIAG0_GET() \
	((io0x1a4 & 0x0007u) >> 0U)
#define IO_CUSTOM_91804__POL_DIAG1_GET() \
	((io0x1a4 & 0x0070u) >> 4U)
#define IO_CUSTOM_91804__POL_DIAG2_GET() \
	((io0x1a4 & 0x0700u) >> 8U)
#define IO_CUSTOM_91804__POL_DIAG3_GET() \
	((io0x1a4 & 0x7000u) >> 12U)
#define IO_CUSTOM_91804__TEMPSENS_MODE_GET() \
	((io0x1a6 & 0x0007u) >> 0U)
#define IO_CUSTOM_91804__TEMPSENS_LT_FLAG_GET() \
	((io0x1a6 & 0x0040u) >> 6U)
#define IO_CUSTOM_91804__TEMPSENS_HT_FLAG_GET() \
	((io0x1a6 & 0x0080u) >> 7U)
#define IO_CUSTOM_91804__POL_GPIO0_GET() \
	((io0x1a6 & 0x0100u) >> 8U)
#define IO_CUSTOM_91804__POL_GPIO1_GET() \
	((io0x1a6 & 0x0200u) >> 9U)
#define IO_CUSTOM_91804__POL_GPIO2_GET() \
	((io0x1a6 & 0x0400u) >> 10U)
#define IO_CUSTOM_91804__POL_GPIO3_GET() \
	((io0x1a6 & 0x0800u) >> 11U)
#define IO_CUSTOM_91804__WAKE_GPIO0_GET() \
	((io0x1a6 & 0x1000u) >> 12U)
#define IO_CUSTOM_91804__WAKE_GPIO1_GET() \
	((io0x1a6 & 0x2000u) >> 13U)
#define IO_CUSTOM_91804__WAKE_GPIO2_GET() \
	((io0x1a6 & 0x4000u) >> 14U)
#define IO_CUSTOM_91804__WAKE_GPIO3_GET() \
	((io0x1a6 & 0x8000u) >> 15U)
#define IO_CUSTOM_91804__GPIO0_GET() \
	((io0x1a8 & 0x0001u) >> 0U)
#define IO_CUSTOM_91804__GPIO1_GET() \
	((io0x1a8 & 0x0002u) >> 1U)
#define IO_CUSTOM_91804__GPIO2_GET() \
	((io0x1a8 & 0x0004u) >> 2U)
#define IO_CUSTOM_91804__GPIO3_GET() \
	((io0x1a8 & 0x0008u) >> 3U)
#define IO_CUSTOM_91804__TDI_GET() \
	((io0x1a8 & 0x0010u) >> 4U)
#define IO_CUSTOM_91804__TCK_GET() \
	((io0x1a8 & 0x0020u) >> 5U)
#define IO_CUSTOM_91804__GPIO0_DRV_GET() \
	((io0x1a8 & 0x0300u) >> 8U)
#define IO_CUSTOM_91804__GPIO1_DRV_GET() \
	((io0x1a8 & 0x0c00u) >> 10U)
#define IO_CUSTOM_91804__GPIO2_DRV_GET() \
	((io0x1a8 & 0x3000u) >> 12U)
#define IO_CUSTOM_91804__GPIO3_DRV_GET() \
	((io0x1a8 & 0xc000u) >> 14U)
#define IO_CUSTOM_91804__GPIO0_AMUX_GET() \
	((io0x1aa & 0x0007u) >> 0U)
#define IO_CUSTOM_91804__EN_AIO0_PD_GET() \
	((io0x1aa & 0x0008u) >> 3U)
#define IO_CUSTOM_91804__GPIO1_AMUX_GET() \
	((io0x1aa & 0x0070u) >> 4U)
#define IO_CUSTOM_91804__EN_AIO1_PD_GET() \
	((io0x1aa & 0x0080u) >> 7U)
#define IO_CUSTOM_91804__GPIO2_AMUX_GET() \
	((io0x1aa & 0x0700u) >> 8U)
#define IO_CUSTOM_91804__EN_AIO2_PD_GET() \
	((io0x1aa & 0x0800u) >> 11U)
#define IO_CUSTOM_91804__GPIO3_AMUX_GET() \
	((io0x1aa & 0x7000u) >> 12U)
#define IO_CUSTOM_91804__EN_AIO3_PD_GET() \
	((io0x1aa & 0x8000u) >> 15U)
#define IO_CUSTOM_91804__GPIO0_CH_SEL_GET() \
	((io0x1ac & 0x00ffu) >> 0U)
#define IO_CUSTOM_91804__GPIO1_CH_SEL_GET() \
	((io0x1ac & 0xff00u) >> 8U)
#define IO_CUSTOM_91804__GPIO2_CH_SEL_GET() \
	((io0x1ae & 0x00ffu) >> 0U)
#define IO_CUSTOM_91804__GPIO3_CH_SEL_GET() \
	((io0x1ae & 0xff00u) >> 8U)
#define IO_CUSTOM_91804__DIAG0_LOW_GET() \
	((io0x1b0 & 0x0001u) >> 0U)
#define IO_CUSTOM_91804__DIAG0_HIGH_GET() \
	((io0x1b0 & 0x0002u) >> 1U)
#define IO_CUSTOM_91804__DIAG1_LOW_GET() \
	((io0x1b0 & 0x0004u) >> 2U)
#define IO_CUSTOM_91804__DIAG1_HIGH_GET() \
	((io0x1b0 & 0x0008u) >> 3U)
#define IO_CUSTOM_91804__DIAG2_LOW_GET() \
	((io0x1b0 & 0x0010u) >> 4U)
#define IO_CUSTOM_91804__DIAG2_HIGH_GET() \
	((io0x1b0 & 0x0020u) >> 5U)
#define IO_CUSTOM_91804__DIAG3_LOW_GET() \
	((io0x1b0 & 0x0040u) >> 6U)
#define IO_CUSTOM_91804__DIAG3_HIGH_GET() \
	((io0x1b0 & 0x0080u) >> 7U)
#define IO_CUSTOM_91804__DIAG_REF_SEL_GET() \
	((io0x1b0 & 0x0700u) >> 8U)
#define IO_CUSTOM_91804__DIAG0_CH_SEL_GET() \
	((io0x1b2 & 0x000fu) >> 0U)
#define IO_CUSTOM_91804__DIAG0_HYST_GET() \
	((io0x1b2 & 0x00c0u) >> 6U)
#define IO_CUSTOM_91804__DIAG1_CH_SEL_GET() \
	((io0x1b2 & 0x0f00u) >> 8U)
#define IO_CUSTOM_91804__DIAG1_HYST_GET() \
	((io0x1b2 & 0xc000u) >> 14U)
#define IO_CUSTOM_91804__DIAG2_CH_SEL_GET() \
	((io0x1b4 & 0x000fu) >> 0U)
#define IO_CUSTOM_91804__DIAG2_HYST_GET() \
	((io0x1b4 & 0x00c0u) >> 6U)
#define IO_CUSTOM_91804__DIAG3_CH_SEL_GET() \
	((io0x1b4 & 0x0f00u) >> 8U)
#define IO_CUSTOM_91804__DIAG3_HYST_GET() \
	((io0x1b4 & 0xc000u) >> 14U)
#define IO_CUSTOM_91804__DIAG0_VREFL_GET() \
	((io0x1b6 & 0x007fu) >> 0U)
#define IO_CUSTOM_91804__DIAG0_VREFH_GET() \
	((io0x1b6 & 0x7f00u) >> 8U)
#define IO_CUSTOM_91804__DIAG1_VREFL_GET() \
	((io0x1b8 & 0x007fu) >> 0U)
#define IO_CUSTOM_91804__DIAG1_VREFH_GET() \
	((io0x1b8 & 0x7f00u) >> 8U)
#define IO_CUSTOM_91804__DIAG2_VREFL_GET() \
	((io0x1ba & 0x007fu) >> 0U)
#define IO_CUSTOM_91804__DIAG2_VREFH_GET() \
	((io0x1ba & 0x7f00u) >> 8U)
#define IO_CUSTOM_91804__DIAG3_VREFL_GET() \
	((io0x1bc & 0x007fu) >> 0U)
#define IO_CUSTOM_91804__DIAG3_VREFH_GET() \
	((io0x1bc & 0x7f00u) >> 8U)
#define IO_CUSTOM_91804__FREQCNT_SRC_GET() \
	((io0x1be & 0x0007u) >> 0U)
#define IO_CUSTOM_91804__FREQCNT_TBASE_GET() \
	((io0x1be & 0x0030u) >> 4U)
#define IO_CUSTOM_91804__FREQCNT_STATE_GET() \
	((io0x1be & 0x00c0u) >> 6U)
#define IO_CUSTOM_91804__FREQCNT_MANT_GET() \
	((io0x1be & 0x0f00u) >> 8U)
#define IO_CUSTOM_91804__FREQCNT_EXP_GET() \
	((io0x1be & 0xf000u) >> 12U)
#define IO_CUSTOM_91804__FREQCNT_OUT_GET() \
	(io0x1c0 >> 0U)
#define IO_CUSTOM_91804__DTB_GET() \
	((io0x1c2 & 0x000fu) >> 0U)
#define IO_CUSTOM_91804__DCDC_VDDA3L_COUT_GET() \
	((io0x1c2 & 0x0010u) >> 4U)
#define IO_CUSTOM_91804__DCDC_VDDA3H_COUT_GET() \
	((io0x1c2 & 0x0020u) >> 5U)
#define IO_CUSTOM_91804__DCDC_VX_COUT_GET() \
	((io0x1c2 & 0x0040u) >> 6U)
#define IO_CUSTOM_91804__DCDC_VNVM_COUT_GET() \
	((io0x1c2 & 0x0080u) >> 7U)
#define IO_CUSTOM_91804__DCDC_STATUS_GET() \
	((io0x1c2 & 0x0300u) >> 8U)
#define IO_CUSTOM_91804__VDIG_LV_B_DB_GET() \
	((io0x1c2 & 0x0400u) >> 10U)
#define IO_CUSTOM_91804__VANA_LV_B_DB_GET() \
	((io0x1c2 & 0x0800u) >> 11U)
#define IO_CUSTOM_91804__VPA_LV_B_DB_GET() \
	((io0x1c2 & 0x1000u) >> 12U)
#define IO_CUSTOM_91804__LF_FD_B_GET() \
	((io0x1c2 & 0x2000u) >> 13U)
#define IO_CUSTOM_91804__LF_DATA_GET() \
	((io0x1c2 & 0x4000u) >> 14U)
#define IO_CUSTOM_91804__LF_CLK_GET() \
	((io0x1c2 & 0x8000u) >> 15U)
#define IO_CUSTOM_91804__RFTX_PATTERN_LEN_GET() \
	((io0x1c4 & 0x0003u) >> 0U)
#define IO_CUSTOM_91804__RFTX_MULTI_FRAME_GET() \
	((io0x1c4 & 0x0008u) >> 3U)
#define IO_CUSTOM_91804__RFTX_DIRECT_MOD_GET() \
	((io0x1c4 & 0x0070u) >> 4U)
#define IO_CUSTOM_91804__RF_EN_PD_GET() \
	((io0x1c4 & 0x0080u) >> 7U)
#define IO_CUSTOM_91804__RFTX_INFO_GET() \
	((io0x1c4 & 0x0700u) >> 8U)
#define IO_CUSTOM_91804__PLL_LOCKED_GET() \
	((io0x1c4 & 0x0800u) >> 11U)
#define IO_CUSTOM_91804__CYCLE_SLIP_GET() \
	((io0x1c4 & 0x1000u) >> 12U)
#define IO_CUSTOM_91804__PLL_XTAL_RUN_GET() \
	((io0x1c4 & 0x2000u) >> 13U)
#define IO_CUSTOM_91804__RFTX_WAIT_LOCK_GET() \
	((io0x1c4 & 0xc000u) >> 14U)
#define IO_CUSTOM_91804__RFTX_PREAMBLE_GET() \
	((io0x1c6 & 0x00ffu) >> 0U)
#define IO_CUSTOM_91804__RFTX_PREAMBLE_LEN_GET() \
	((io0x1c6 & 0xff00u) >> 8U)
#define IO_CUSTOM_91804__RFTX_PATTERN_L_GET() \
	(io0x1c8 >> 0U)
#define IO_CUSTOM_91804__RFTX_PATTERN_H_GET() \
	(io0x1ca >> 0U)
#define IO_CUSTOM_91804__RFTX_PACKET_LEN_GET() \
	((io0x1cc & 0x00ffu) >> 0U)
#define IO_CUSTOM_91804__RFTX_PACKET_LEN_CORR_GET() \
	((io0x1cc & 0x0f00u) >> 8U)
#define IO_CUSTOM_91804__RFTX_PACKET_LEN_POS_GET() \
	((io0x1cc & 0x3000u) >> 12U)
#define IO_CUSTOM_91804__RFTX_PACKET_LEN_FIX_GET() \
	((io0x1cc & 0x8000u) >> 15U)
#define IO_CUSTOM_91804__RFTX_ADDRESS_GET() \
	((io0x1ce & 0x00ffu) >> 0U)
#define IO_CUSTOM_91804__RFTX_EN_ADDRESS_GET() \
	((io0x1ce & 0x0100u) >> 8U)
#define IO_CUSTOM_91804__RFTX_CRC_INV_GET() \
	((io0x1ce & 0x0200u) >> 9U)
#define IO_CUSTOM_91804__RFTX_CRC_16_N8_GET() \
	((io0x1ce & 0x0400u) >> 10U)
#define IO_CUSTOM_91804__RFTX_EN_CRC_GET() \
	((io0x1ce & 0x0800u) >> 11U)
#define IO_CUSTOM_91804__RFTX_CRC_INIT_GET() \
	(io0x1d0 >> 0U)
#define IO_CUSTOM_91804__RFTX_DR_MANT_GET() \
	((io0x1d2 & 0x00ffu) >> 0U)
#define IO_CUSTOM_91804__RFTX_DR_EXP_GET() \
	((io0x1d2 & 0x0700u) >> 8U)
#define IO_CUSTOM_91804__RFTX_LSB_FIRST_GET() \
	((io0x1d2 & 0x0800u) >> 11U)
#define IO_CUSTOM_91804__RFTX_EN_MANCHESTER_GET() \
	((io0x1d2 & 0x1000u) >> 12U)
#define IO_CUSTOM_91804__RFTX_RAMP_GET() \
	((io0x1d2 & 0xe000u) >> 13U)
#define IO_CUSTOM_91804__RFTX_MANCHESTER_STOP_WORD_GET() \
	((io0x1d4 & 0x00ffu) >> 0U)
#define IO_CUSTOM_91804__RFTX_MANCHESTER_STOP_LEN_GET() \
	((io0x1d4 & 0x0300u) >> 8U)
#define IO_CUSTOM_91804__RFTX_MULT_MANT_GET() \
	((io0x1d6 & 0x000fu) >> 0U)
#define IO_CUSTOM_91804__RFTX_MULT_EXP_GET() \
	((io0x1d6 & 0x00f0u) >> 4U)
#define IO_CUSTOM_91804__RFTX_BIT_INVERT_GET() \
	((io0x1d6 & 0x0100u) >> 8U)
#define IO_CUSTOM_91804__RFTX_PN9_MODE_GET() \
	((io0x1d6 & 0x0200u) >> 9U)
#define IO_CUSTOM_91804__RFTX_PN9_REVERSE_GET() \
	((io0x1d6 & 0x0400u) >> 10U)
#define IO_CUSTOM_91804__RFTX_EN_DATAWHITE_GET() \
	((io0x1d6 & 0x0800u) >> 11U)
#define IO_CUSTOM_91804__RFTX_EN_INTERP_GET() \
	((io0x1d6 & 0x3000u) >> 12U)
#define IO_CUSTOM_91804__RFTX_EN_GAUSSIAN_GET() \
	((io0x1d6 & 0x4000u) >> 14U)
#define IO_CUSTOM_91804__RFTX_FSK_NOOK_GET() \
	((io0x1d6 & 0x8000u) >> 15U)
#define IO_CUSTOM_91804__RFTX_CENTER_FREQ_L_GET() \
	(io0x1d8 >> 0U)
#define IO_CUSTOM_91804__RFTX_CENTER_FREQ_H_GET() \
	((io0x1da & 0x00ffu) >> 0U)
#define IO_CUSTOM_91804__RFTX_POWER_GET() \
	((io0x1da & 0x7f00u) >> 8U)
#define IO_CUSTOM_91804__RFTX_BYPASS_VPA_GET() \
	((io0x1da & 0x8000u) >> 15U)
#define IO_CUSTOM_91804__RFTX_FIFO_DATA_GET() \
	((io0x1dc & 0x00ffu) >> 0U)
#define IO_CUSTOM_91804__RFTX_FIFO_COUNT_GET() \
	((io0x1dc & 0x3f00u) >> 8U)
#define IO_CUSTOM_91804__RFTX_FIFO_FLUSH_GET() \
	((io0x1dc & 0xff00u) >> 8U)
#define IO_CUSTOM_91804__RFTX_FIFO_FULL_GET() \
	((io0x1dc & 0x2000u) >> 13U)
#define IO_CUSTOM_91804__RFTX_FIFO_OVUND_GET() \
	((io0x1dc & 0x8000u) >> 15U)
#define IO_CUSTOM_91804__LFTX_FIFO_DATA_GET() \
	((io0x1de & 0x00ffu) >> 0U)
#define IO_CUSTOM_91804__LF_DATABUF_GET() \
	((io0x1de & 0x00ffu) >> 0U)
#define IO_CUSTOM_91804__LFTX_FIFO_COUNT_GET() \
	((io0x1de & 0x0f00u) >> 8U)
#define IO_CUSTOM_91804__LFTX_FIFO_FLUSH_GET() \
	((io0x1de & 0xff00u) >> 8U)
#define IO_CUSTOM_91804__LFTX_FIFO_FULL_GET() \
	((io0x1de & 0x0800u) >> 11U)
#define IO_CUSTOM_91804__LFTX_FIFO_OVUND_GET() \
	((io0x1de & 0x8000u) >> 15U)
#define IO_CUSTOM_91804__LFTX_POL_GET() \
	((io0x1e0 & 0x0001u) >> 0U)
#define IO_CUSTOM_91804__LFTX_MODE_GET() \
	((io0x1e0 & 0x000eu) >> 1U)
#define IO_CUSTOM_91804__LF_EN_FD_GET() \
	((io0x1e0 & 0x0080u) >> 7U)
#define IO_CUSTOM_91804__LFRX_STATE_GET() \
	((io0x1e0 & 0x0700u) >> 8U)
#define IO_CUSTOM_91804__LFRX_FIFO_DATA_GET() \
	((io0x1e2 & 0x00ffu) >> 0U)
#define IO_CUSTOM_91804__LFRX_FIFO_COUNT_GET() \
	((io0x1e2 & 0x0f00u) >> 8U)
#define IO_CUSTOM_91804__LFRX_FIFO_FLUSH_GET() \
	((io0x1e2 & 0xff00u) >> 8U)
#define IO_CUSTOM_91804__LFRX_FIFO_FULL_GET() \
	((io0x1e2 & 0x0800u) >> 11U)
#define IO_CUSTOM_91804__LFRX_FIFO_OVUND_GET() \
	((io0x1e2 & 0x8000u) >> 15U)
#define IO_CUSTOM_91804__LFRX_TIMEOUT_MANT_GET() \
	((io0x1e4 & 0x000fu) >> 0U)
#define IO_CUSTOM_91804__LFRX_TIMEOUT_EXP_GET() \
	((io0x1e4 & 0x00f0u) >> 4U)
#define IO_CUSTOM_91804__LFRX_STBY_MANT_GET() \
	((io0x1e4 & 0x0f00u) >> 8U)
#define IO_CUSTOM_91804__LFRX_STBY_EXP_GET() \
	((io0x1e4 & 0xf000u) >> 12U)
#define IO_CUSTOM_91804__LFRX_STBY_ON_GET() \
	((io0x1e6 & 0x0001u) >> 0U)
#define IO_CUSTOM_91804__LFRX_PREAMBLE_GET() \
	((io0x1e6 & 0x0004u) >> 2U)
#define IO_CUSTOM_91804__LF_GAIN_SEL_GET() \
	((io0x1e6 & 0x0030u) >> 4U)
#define IO_CUSTOM_91804__LFRX_ORDER_GET() \
	((io0x1e6 & 0x0080u) >> 7U)
#define IO_CUSTOM_91804__LFRX_HDRLEN_GET() \
	((io0x1e6 & 0x0300u) >> 8U)
#define IO_CUSTOM_91804__LFRX_POL_GET() \
	((io0x1e6 & 0x0400u) >> 10U)
#define IO_CUSTOM_91804__LFRX_SYNCPOL_GET() \
	((io0x1e6 & 0x0800u) >> 11U)
#define IO_CUSTOM_91804__LFRX_SYNCLEN_GET() \
	((io0x1e6 & 0xf000u) >> 12U)
#define IO_CUSTOM_91804__LFRX_SYNC_DIGIT1_GET() \
	((io0x1e8 & 0x000cu) >> 2U)
#define IO_CUSTOM_91804__LFRX_SYNC_DIGIT2_GET() \
	((io0x1e8 & 0x0030u) >> 4U)
#define IO_CUSTOM_91804__LFRX_SYNC_DIGIT3_GET() \
	((io0x1e8 & 0x00c0u) >> 6U)
#define IO_CUSTOM_91804__LFRX_SYNC_DIGIT4_GET() \
	((io0x1e8 & 0x0300u) >> 8U)
#define IO_CUSTOM_91804__LFRX_SYNC_DIGIT5_GET() \
	((io0x1e8 & 0x0c00u) >> 10U)
#define IO_CUSTOM_91804__LFRX_SYNC_DIGIT6_GET() \
	((io0x1e8 & 0x3000u) >> 12U)
#define IO_CUSTOM_91804__LFRX_SYNC_DIGIT7_GET() \
	((io0x1e8 & 0xc000u) >> 14U)
#define IO_CUSTOM_91804__LFRX_SYNC_DIGIT8_GET() \
	((io0x1ea & 0x0003u) >> 0U)
#define IO_CUSTOM_91804__LFRX_SYNC_DIGIT9_GET() \
	((io0x1ea & 0x000cu) >> 2U)
#define IO_CUSTOM_91804__LFRX_SYNC_DIGIT10_GET() \
	((io0x1ea & 0x0030u) >> 4U)
#define IO_CUSTOM_91804__LFRX_SYNC_DIGIT11_GET() \
	((io0x1ea & 0x00c0u) >> 6U)
#define IO_CUSTOM_91804__LFRX_SYNC_DIGIT12_GET() \
	((io0x1ea & 0x0300u) >> 8U)
#define IO_CUSTOM_91804__LFRX_SYNC_DIGIT13_GET() \
	((io0x1ea & 0x0c00u) >> 10U)
#define IO_CUSTOM_91804__LFRX_SYNC_DIGIT14_GET() \
	((io0x1ea & 0x3000u) >> 12U)
#define IO_CUSTOM_91804__LFRX_SYNC_DIGIT15_GET() \
	((io0x1ea & 0xc000u) >> 14U)
#define IO_CUSTOM_91804__LFRX_MINCNT_GET() \
	((io0x1ec & 0x003fu) >> 0U)
#define IO_CUSTOM_91804__LFRX_DELTACNT_GET() \
	((io0x1ec & 0x3f00u) >> 8U)
#define IO_CUSTOM_91804__LFRX_LONGCNT_GET() \
	((io0x1ec & 0x8000u) >> 15U)
#define IO_CUSTOM_91804__LFRX_HDR_L_GET() \
	(io0x1ee >> 0U)
#define IO_CUSTOM_91804__LFRX_HDR_H_GET() \
	(io0x1f0 >> 0U)
#define IO_CUSTOM_91804__LFRX_HDR_MASK_L_GET() \
	(io0x1f2 >> 0U)
#define IO_CUSTOM_91804__LFRX_HDR_MASK_H_GET() \
	(io0x1f4 >> 0U)
#define IO_CUSTOM_91804__LFRX_RXD_HDR_L_GET() \
	(io0x1f6 >> 0U)
#define IO_CUSTOM_91804__LFRX_RXD_HDR_H_GET() \
	(io0x1f8 >> 0U)
#define IO_CUSTOM_91804__ACCEL_SEL_GET() \
	((io0x1fa & 0x0003u) >> 0U)
#define IO_CUSTOM_91804__ACCEL_MEAS_GET() \
	((io0x1fa & 0x000cu) >> 2U)
#define IO_CUSTOM_91804__ACCEL_SPARE_GET() \
	((io0x1fa & 0x00f0u) >> 4U)
#define IO_CUSTOM_91804__ACCEL_POL_GET() \
	((io0x1fa & 0x0100u) >> 8U)
#define IO_CUSTOM_91804__ACCEL_PREBPOL_GET() \
	((io0x1fa & 0x0200u) >> 9U)
#define IO_CUSTOM_91804__ACCEL_PREB_GET() \
	((io0x1fa & 0x0400u) >> 10U)
#define IO_CUSTOM_91804__ACCEL_DIG_RSTB_GET() \
	((io0x1fa & 0x1000u) >> 12U)
#define IO_CUSTOM_91804__ACCEL_EN_GET() \
	((io0x1fa & 0x2000u) >> 13U)
#define IO_CUSTOM_91804__ACCEL_RDY_GET() \
	((io0x1fa & 0x8000u) >> 15U)
#define IO_CUSTOM_91804__SENS_CH_SEL_B_GET() \
	((io0x1fc & 0x000fu) >> 0U)
#define IO_CUSTOM_91804__SENS_CH_SEL_A_GET() \
	((io0x1fc & 0x0f00u) >> 8U)
#define IO_CUSTOM_91804__SENS_PGA_GAIN_GET() \
	((io0x1fe & 0x000fu) >> 0U)
#define IO_CUSTOM_91804__SENS_EN_PGA_GET() \
	((io0x1fe & 0x0040u) >> 6U)
#define IO_CUSTOM_91804__SENS_EN_BRIDGE_GET() \
	((io0x1fe & 0x0080u) >> 7U)
#define IO_CUSTOM_91804__SENS_REFL_SEL_GET() \
	((io0x1fe & 0x0700u) >> 8U)
#define IO_CUSTOM_91804__SENS_EN_EXTREF_GET() \
	((io0x1fe & 0x0800u) >> 11U)
#define IO_CUSTOM_91804__SENS_REFH_SEL_GET() \
	((io0x1fe & 0x7000u) >> 12U)
#define IO_CUSTOM_91804__SENS_CH_POL_GET() \
	((io0x1fe & 0x8000u) >> 15U)
#define IO_CUSTOM_91804__ADC_SPEED_GET() \
	((io0x200 & 0x0003u) >> 0U)
#define IO_CUSTOM_91804__ADC_INTREF_GET() \
	((io0x200 & 0x0100u) >> 8U)
#define IO_CUSTOM_91804__ADC_TESTINTP_GET() \
	((io0x202 & 0x0001u) >> 0U)
#define IO_CUSTOM_91804__ADC_TESTINTN_GET() \
	((io0x202 & 0x0002u) >> 1U)
#define IO_CUSTOM_91804__ADC_TESTCOMP_GET() \
	((io0x202 & 0x0004u) >> 2U)
#define IO_CUSTOM_91804__ADC_TESTANA_GET() \
	((io0x202 & 0x0008u) >> 3U)
#define IO_CUSTOM_91804__ADC_TEST_COMPNODELAY_GET() \
	((io0x202 & 0x0010u) >> 4U)
#define IO_CUSTOM_91804__ADC_EN_TESTOV_GET() \
	((io0x202 & 0x0020u) >> 5U)
#define IO_CUSTOM_91804__ADC_FRC_RDY_GET() \
	((io0x202 & 0x0040u) >> 6U)
#define IO_CUSTOM_91804__SENS_EN_PU_SN_GET() \
	((io0x202 & 0x0100u) >> 8U)
#define IO_CUSTOM_91804__SENS_EN_PD_SN_GET() \
	((io0x202 & 0x0200u) >> 9U)
#define IO_CUSTOM_91804__SENS_EN_PU_SP_GET() \
	((io0x202 & 0x0400u) >> 10U)
#define IO_CUSTOM_91804__SENS_EN_PD_SP_GET() \
	((io0x202 & 0x0800u) >> 11U)
#define IO_CUSTOM_91804__ACCEL_CCA_GET() \
	((io0x204 & 0x000fu) >> 0U)
#define IO_CUSTOM_91804__ACCEL_CCB_GET() \
	((io0x204 & 0x0f00u) >> 8U)
#define IO_CUSTOM_91804__DTB_SEL_GET() \
	((io0x206 & 0x0007u) >> 0U)
#define IO_CUSTOM_91804__DISCONNECT_TDO_GET() \
	((io0x206 & 0x0080u) >> 7U)
#define IO_CUSTOM_91804__ATB_SEL_GET() \
	((io0x206 & 0x1f00u) >> 8U)
#define IO_CUSTOM_91804__FRC_EN_IBG_1V7_GET() \
	((io0x208 & 0x00ffu) >> 0U)
#define IO_CUSTOM_91804__FRC_EN_IBG_3V_GET() \
	((io0x208 & 0x7f00u) >> 8U)
#define IO_CUSTOM_91804__FRC_EN_IBGH_1V7_GET() \
	((io0x20a & 0x000fu) >> 0U)
#define IO_CUSTOM_91804__FRC_EN_IPTAT_1V7_GET() \
	((io0x20a & 0x00f0u) >> 4U)
#define IO_CUSTOM_91804__FRC_EN_IPTAT_3V_GET() \
	((io0x20a & 0x1f00u) >> 8U)
#define IO_CUSTOM_91804__FRC_EN_PTAT_GET() \
	((io0x20a & 0x8000u) >> 15U)
#define IO_CUSTOM_91804__FRC_EN_ISPTAT_1V7_GET() \
	((io0x20c & 0x001fu) >> 0U)
#define IO_CUSTOM_91804__FRC_EN_SPTAT_GET() \
	((io0x20c & 0x0080u) >> 7U)
#define IO_CUSTOM_91804__FRC_EN_IBG_P_OUT_GET() \
	((io0x20c & 0x1f00u) >> 8U)
#define IO_CUSTOM_91804__FRC_EN_VBG_OUT_GET() \
	((io0x20e & 0x000fu) >> 0U)
#define IO_CUSTOM_91804__FRC_EN_BG_GET() \
	((io0x20e & 0x00c0u) >> 6U)
#define IO_CUSTOM_91804__VBG_TRIM_GET() \
	((io0x20e & 0x0f00u) >> 8U)
#define IO_CUSTOM_91804__IBG0_1V7_GET() \
	((io0x210 & 0x000fu) >> 0U)
#define IO_CUSTOM_91804__IBG1_1V7_GET() \
	((io0x210 & 0x00f0u) >> 4U)
#define IO_CUSTOM_91804__IBG2_1V7_GET() \
	((io0x210 & 0x0f00u) >> 8U)
#define IO_CUSTOM_91804__IBG3_1V7_GET() \
	((io0x210 & 0xf000u) >> 12U)
#define IO_CUSTOM_91804__IBG4_1V7_GET() \
	((io0x212 & 0x000fu) >> 0U)
#define IO_CUSTOM_91804__IBG5_1V7_GET() \
	((io0x212 & 0x00f0u) >> 4U)
#define IO_CUSTOM_91804__IBG6_1V7_GET() \
	((io0x212 & 0x0f00u) >> 8U)
#define IO_CUSTOM_91804__IBG7_1V7_GET() \
	((io0x212 & 0xf000u) >> 12U)
#define IO_CUSTOM_91804__IBG0_3V_GET() \
	((io0x214 & 0x000fu) >> 0U)
#define IO_CUSTOM_91804__IBG1_3V_GET() \
	((io0x214 & 0x00f0u) >> 4U)
#define IO_CUSTOM_91804__IBG2_3V_GET() \
	((io0x214 & 0x0f00u) >> 8U)
#define IO_CUSTOM_91804__IBG3_3V_GET() \
	((io0x214 & 0xf000u) >> 12U)
#define IO_CUSTOM_91804__IBG4_3V_GET() \
	((io0x216 & 0x000fu) >> 0U)
#define IO_CUSTOM_91804__IBG5_3V_GET() \
	((io0x216 & 0x00f0u) >> 4U)
#define IO_CUSTOM_91804__IBG6_3V_GET() \
	((io0x216 & 0x0f00u) >> 8U)
#define IO_CUSTOM_91804__IBGH0_1V7_GET() \
	((io0x218 & 0x000fu) >> 0U)
#define IO_CUSTOM_91804__IBGH1_1V7_GET() \
	((io0x218 & 0x00f0u) >> 4U)
#define IO_CUSTOM_91804__IBGH2_1V7_GET() \
	((io0x218 & 0x0f00u) >> 8U)
#define IO_CUSTOM_91804__IBGH3_1V7_GET() \
	((io0x218 & 0xf000u) >> 12U)
#define IO_CUSTOM_91804__IPTAT0_1V7_GET() \
	((io0x21a & 0x000fu) >> 0U)
#define IO_CUSTOM_91804__IPTAT1_1V7_GET() \
	((io0x21a & 0x00f0u) >> 4U)
#define IO_CUSTOM_91804__IPTAT2_1V7_GET() \
	((io0x21a & 0x0f00u) >> 8U)
#define IO_CUSTOM_91804__IPTAT3_1V7_GET() \
	((io0x21a & 0xf000u) >> 12U)
#define IO_CUSTOM_91804__IPTAT0_3V_GET() \
	((io0x21c & 0x000fu) >> 0U)
#define IO_CUSTOM_91804__IPTAT1_3V_GET() \
	((io0x21c & 0x00f0u) >> 4U)
#define IO_CUSTOM_91804__IPTAT2_3V_GET() \
	((io0x21c & 0x0f00u) >> 8U)
#define IO_CUSTOM_91804__IPTAT3_3V_GET() \
	((io0x21c & 0xf000u) >> 12U)
#define IO_CUSTOM_91804__IPTAT4_3V_GET() \
	((io0x21e & 0x000fu) >> 0U)
#define IO_CUSTOM_91804__IPTAT_TRIM_GET() \
	((io0x21e & 0x3000u) >> 12U)
#define IO_CUSTOM_91804__ISPTAT0_1V7_GET() \
	((io0x220 & 0x000fu) >> 0U)
#define IO_CUSTOM_91804__ISPTAT1_1V7_GET() \
	((io0x220 & 0x00f0u) >> 4U)
#define IO_CUSTOM_91804__ISPTAT2_1V7_GET() \
	((io0x220 & 0x0f00u) >> 8U)
#define IO_CUSTOM_91804__ISPTAT3_1V7_GET() \
	((io0x220 & 0xf000u) >> 12U)
#define IO_CUSTOM_91804__ISPTAT4_1V7_GET() \
	((io0x222 & 0x000fu) >> 0U)
#define IO_CUSTOM_91804__TEMPSENS_TRIM_GET() \
	((io0x222 & 0x7000u) >> 12U)
#define IO_CUSTOM_91804__VDIG_TRIM_GET() \
	((io0x224 & 0x0007u) >> 0U)
#define IO_CUSTOM_91804__VDIG_SHORT_GET() \
	((io0x224 & 0x0008u) >> 3U)
#define IO_CUSTOM_91804__VDIG_EN_IPK_LIM_GET() \
	((io0x224 & 0x0010u) >> 4U)
#define IO_CUSTOM_91804__FRC_VDIG_DIS_GET() \
	((io0x224 & 0x0020u) >> 5U)
#define IO_CUSTOM_91804__FRC_VDIG_DIS_LVD_GET() \
	((io0x224 & 0x0040u) >> 6U)
#define IO_CUSTOM_91804__FRC_VDIG_RDY_GET() \
	((io0x224 & 0x0080u) >> 7U)
#define IO_CUSTOM_91804__VANA_TRIM_GET() \
	((io0x224 & 0x0300u) >> 8U)
#define IO_CUSTOM_91804__LVD_ABORT_RFTX_GET() \
	((io0x224 & 0x0400u) >> 10U)
#define IO_CUSTOM_91804__VANA_SHORT_GET() \
	((io0x224 & 0x0800u) >> 11U)
#define IO_CUSTOM_91804__VANA_EN_IPK_LIM_GET() \
	((io0x224 & 0x1000u) >> 12U)
#define IO_CUSTOM_91804__FRC_VANA_EN_GET() \
	((io0x224 & 0x2000u) >> 13U)
#define IO_CUSTOM_91804__FRC_VANA_EN_LVD_GET() \
	((io0x224 & 0x4000u) >> 14U)
#define IO_CUSTOM_91804__FRC_VANA_RDY_GET() \
	((io0x224 & 0x8000u) >> 15U)
#define IO_CUSTOM_91804__VPA_TRIM_GET() \
	((io0x226 & 0x0003u) >> 0U)
#define IO_CUSTOM_91804__FRC_VPA_BYPASS_GET() \
	((io0x226 & 0x0004u) >> 2U)
#define IO_CUSTOM_91804__VPA_SHORT_GET() \
	((io0x226 & 0x0008u) >> 3U)
#define IO_CUSTOM_91804__VPA_EN_IPK_LIM_GET() \
	((io0x226 & 0x0010u) >> 4U)
#define IO_CUSTOM_91804__FRC_VPA_EN_GET() \
	((io0x226 & 0x0020u) >> 5U)
#define IO_CUSTOM_91804__FRC_VPA_EN_LVD_GET() \
	((io0x226 & 0x0040u) >> 6U)
#define IO_CUSTOM_91804__FRC_VPA_RDY_GET() \
	((io0x226 & 0x0080u) >> 7U)
#define IO_CUSTOM_91804__PLL_VCOREG_TRIM_GET() \
	((io0x226 & 0x3f00u) >> 8U)
#define IO_CUSTOM_91804__FRC_PLL_SHORT_NOISE_FILTER_GET() \
	((io0x226 & 0xc000u) >> 14U)
#define IO_CUSTOM_91804__PLL_N_DIV_XTAL_TEST_GET() \
	((io0x228 & 0x003fu) >> 0U)
#define IO_CUSTOM_91804__PLL_EN_CLK_XTAL_TEST_GET() \
	((io0x228 & 0x0040u) >> 6U)
#define IO_CUSTOM_91804__PLL_XTAL_BUFFER_ALWAYS_ON_GET() \
	((io0x228 & 0x0080u) >> 7U)
#define IO_CUSTOM_91804__PLL_XTAL_STARTUP_CURR_GET() \
	((io0x228 & 0x0300u) >> 8U)
#define IO_CUSTOM_91804__PLL_XTAL_AFTERSTARTUP_CURR_GET() \
	((io0x228 & 0x0c00u) >> 10U)
#define IO_CUSTOM_91804__PLL_EN_XTAL_INT_CAP_GET() \
	((io0x228 & 0x3000u) >> 12U)
#define IO_CUSTOM_91804__FRC_PLL_EN_CLK_XTAL_DIG_GET() \
	((io0x228 & 0x4000u) >> 14U)
#define IO_CUSTOM_91804__FRC_EN_XTAL_GET() \
	((io0x228 & 0x8000u) >> 15U)
#define IO_CUSTOM_91804__FRC_RF_OOK_MOD_GET() \
	((io0x22a & 0x00ffu) >> 0U)
#define IO_CUSTOM_91804__RF_FC_SEL_GET() \
	((io0x22a & 0x0f00u) >> 8U)
#define IO_CUSTOM_91804__FRC_RF_EN_MOD_CLK_GET() \
	((io0x22a & 0x4000u) >> 14U)
#define IO_CUSTOM_91804__FRC_RF_EN_OUT_GET() \
	((io0x22a & 0x8000u) >> 15U)
#define IO_CUSTOM_91804__RF_EN_PPA_BIAS_GET() \
	((io0x22c & 0x0001u) >> 0U)
#define IO_CUSTOM_91804__RF_EN_PA_OS_GET() \
	((io0x22c & 0x0002u) >> 1U)
#define IO_CUSTOM_91804__RF_EN_PA_BIAS_GET() \
	((io0x22c & 0x0004u) >> 2U)
#define IO_CUSTOM_91804__FRC_RF_EN_PA_GET() \
	((io0x22c & 0x0008u) >> 3U)
#define IO_CUSTOM_91804__FRC_PLL_EN_VCOBUF_PRESC_GET() \
	((io0x22c & 0x0010u) >> 4U)
#define IO_CUSTOM_91804__PLL_EN_VCOBUF_TEST_GET() \
	((io0x22c & 0x0020u) >> 5U)
#define IO_CUSTOM_91804__FRC_PLL_EN_VCOBUF_PA1_GET() \
	((io0x22c & 0x0040u) >> 6U)
#define IO_CUSTOM_91804__FRC_PLL_EN_VCOBUF_PA2_GET() \
	((io0x22c & 0x0080u) >> 7U)
#define IO_CUSTOM_91804__FRC_PLL_EN_VCO_GET() \
	((io0x22c & 0x0100u) >> 8U)
#define IO_CUSTOM_91804__FRC_PLL_EN_VCOREG_GET() \
	((io0x22c & 0x0200u) >> 9U)
#define IO_CUSTOM_91804__DIS_CSLIP_GET() \
	((io0x22c & 0x0400u) >> 10U)
#define IO_CUSTOM_91804__FRC_PLL_EN_DIV_PRESC_GET() \
	((io0x22c & 0x0800u) >> 11U)
#define IO_CUSTOM_91804__FRC_PLL_EN_PFD_CP_GET() \
	((io0x22c & 0x1000u) >> 12U)
#define IO_CUSTOM_91804__PLL_EN_LF_C2_GET() \
	((io0x22c & 0x2000u) >> 13U)
#define IO_CUSTOM_91804__PLL_EN_CLK_EXT_GET() \
	((io0x22c & 0x4000u) >> 14U)
#define IO_CUSTOM_91804__PLL_EN_DIV_TEST_GET() \
	((io0x22c & 0x8000u) >> 15U)
#define IO_CUSTOM_91804__PLL_N_DIV_TEST_GET() \
	((io0x22e & 0x00ffu) >> 0U)
#define IO_CUSTOM_91804__PLL_FORCE_VCTR_GET() \
	((io0x22e & 0x1f00u) >> 8U)
#define IO_CUSTOM_91804__PLL_EN_FRC_VCTR_GET() \
	((io0x22e & 0x2000u) >> 13U)
#define IO_CUSTOM_91804__PLL_EN_CURR_OFFS_GET() \
	((io0x22e & 0xc000u) >> 14U)
#define IO_CUSTOM_91804__MASH_ORDER_GET() \
	((io0x230 & 0x0003u) >> 0U)
#define IO_CUSTOM_91804__MASH_DITHER_GET() \
	((io0x230 & 0x0010u) >> 4U)
#define IO_CUSTOM_91804__FRC_MASH_EN_GET() \
	((io0x230 & 0x0020u) >> 5U)
#define IO_CUSTOM_91804__RFTX_CENTER_FREQ_U_GET() \
	((io0x230 & 0x0040u) >> 6U)
#define IO_CUSTOM_91804__PLL_CLK_PFD_POL_GET() \
	((io0x230 & 0x0080u) >> 7U)
#define IO_CUSTOM_91804__HFO_CLK_X_SEL_GET() \
	((io0x230 & 0x0300u) >> 8U)
#define IO_CUSTOM_91804__HFO_FREQ_TRIM_GET() \
	((io0x232 & 0x007fu) >> 0U)
#define IO_CUSTOM_91804__LFO_FREQ_TRIM_GET() \
	((io0x232 & 0x7f00u) >> 8U)
#define IO_CUSTOM_91804__FRC_LFO_EN_OUT_32K_GET() \
	((io0x234 & 0x0001u) >> 0U)
#define IO_CUSTOM_91804__LFO_FRC_ULP_MODE_GET() \
	((io0x234 & 0x0040u) >> 6U)
#define IO_CUSTOM_91804__LFO_EN_ULP_MODE_GET() \
	((io0x234 & 0x0080u) >> 7U)
#define IO_CUSTOM_91804__FRC_LF_MOD_GET() \
	((io0x234 & 0x0100u) >> 8U)
#define IO_CUSTOM_91804__LF_EN_QDAMP_B_GET() \
	((io0x234 & 0x0400u) >> 10U)
#define IO_CUSTOM_91804__FRC_LF_EN_CLK_GET() \
	((io0x234 & 0x1000u) >> 12U)
#define IO_CUSTOM_91804__FRC_LF_EN_LSTN_GET() \
	((io0x234 & 0x2000u) >> 13U)
#define IO_CUSTOM_91804__FRC_LF_EN_HDR_GET() \
	((io0x234 & 0x4000u) >> 14U)
#define IO_CUSTOM_91804__FRC_LF_EN_GET() \
	((io0x234 & 0x8000u) >> 15U)
#define IO_CUSTOM_91804__DCDC_FFINE_GET() \
	((io0x236 & 0x001fu) >> 0U)
#define IO_CUSTOM_91804__DCDC_1X5_FROUGH_GET() \
	((io0x236 & 0x0300u) >> 8U)
#define IO_CUSTOM_91804__DCDC_2X_FROUGH_GET() \
	((io0x236 & 0x0c00u) >> 10U)
#define IO_CUSTOM_91804__DCDC_REG_TRIM_GET() \
	((io0x236 & 0xf000u) >> 12U)
#define IO_CUSTOM_91804__DCDC_VX_VTH_GET() \
	((io0x238 & 0x000fu) >> 0U)
#define IO_CUSTOM_91804__DCDC_VX_HYST_GET() \
	((io0x238 & 0x00e0u) >> 5U)
#define IO_CUSTOM_91804__DCDC_VNVM_VTH_GET() \
	((io0x238 & 0x0f00u) >> 8U)
#define IO_CUSTOM_91804__DCDC_VNVM_HYST_GET() \
	((io0x238 & 0xe000u) >> 13U)
#define IO_CUSTOM_91804__DCDC_VDDA3L_VTH_GET() \
	((io0x23a & 0x000fu) >> 0U)
#define IO_CUSTOM_91804__DCDC_VDDA3L_HYST_GET() \
	((io0x23a & 0x00e0u) >> 5U)
#define IO_CUSTOM_91804__DCDC_VDDA3H_VTH_GET() \
	((io0x23a & 0x0f00u) >> 8U)
#define IO_CUSTOM_91804__DCDC_VDDA3H_HYST_GET() \
	((io0x23a & 0xe000u) >> 13U)
#define IO_CUSTOM_91804__DCDC_FORCE_1X_GET() \
	((io0x23c & 0x0001u) >> 0U)
#define IO_CUSTOM_91804__DCDC_FORCE_1_5X_GET() \
	((io0x23c & 0x0002u) >> 1U)
#define IO_CUSTOM_91804__DCDC_FORCE_2X_GET() \
	((io0x23c & 0x0004u) >> 2U)
#define IO_CUSTOM_91804__DCDC_DUTY_CYCLE_EN_GET() \
	((io0x23c & 0x0010u) >> 4U)
#define IO_CUSTOM_91804__DCDC_BYPASS_GET() \
	((io0x23c & 0x0020u) >> 5U)
#define IO_CUSTOM_91804__FRC_DCDC_ENDIS_GET() \
	((io0x23c & 0x00c0u) >> 6U)
#define IO_CUSTOM_91804__DCDC_FORCE_CLK_EXT_GET() \
	((io0x23c & 0x0100u) >> 8U)
#define IO_CUSTOM_91804__DCDC_PROBE_CLK_GET() \
	((io0x23c & 0x0200u) >> 9U)
#define IO_CUSTOM_91804__DCDC_PROBE_VREG_GET() \
	((io0x23c & 0x0400u) >> 10U)
#define IO_CUSTOM_91804__DCDC_PROBE_VX_GET() \
	((io0x23c & 0x0800u) >> 11U)
#define IO_CUSTOM_91804__SPARE_VDIG_GET() \
	((io0x23e & 0x00ffu) >> 0U)
#define IO_CUSTOM_91804__SPARE_VDD_GET() \
	((io0x23e & 0xff00u) >> 8U)
#define IO_CUSTOM_91804__SPARE_VANA_GET() \
	((io0x240 & 0x00ffu) >> 0U)
#define IO_CUSTOM_91804__SENS_RESERVE_GET() \
	((io0x240 & 0xf000u) >> 12U)
#define IO_CUSTOM_91804__LFO_FREQ_TRIM_ULP_GET() \
	(io0x242 >> 0U)

/*****************************************************************************/
/* 3.3 Ports. */

/*
 * Description of the records from left to right:
 * - Block name, double underscore, field name.
 * - Hosting word variable (or undefined 16-bit variable to catch word access if it not possible).
 * - Hosting byte variable (or undefined 8-bit variable to catch byte access if it not possible).
 * - Absolute word address of the field. Also location of the host variable.
 * - Absolute byte address of the field (or 0xFFFF if byte access is not possible).
 * - Bit mask within word, showing the meaningful bits of the port inside the host withing the word
 * - Bit mask within byte, showing the meaningful bits of the port inside the host withing the byte
 * - Offset within word, shows the location of the first significant bit of the mask within the word
 * - Offset within byte, shows the location of the first significant bit of the mask within the byte
 * - Boolean true (1U) or false (0U), showing whether the port is writable.
 * - Triboolean, showing whether the port is bit accessible i.e. the assembler
 *    operation setb, clrb and mov c can be used for it. The value 2U is
 *    reserved for MlxCCT's private dummy ports.
 * - Bit mask, showing which bits of the host variable are not occupied.
 * - Bit mask, showing how the host variable is occupied by rise-only fields.
 */

#define IO_RST_CTRL__AWD_WBOOT \
	io0x0, io_byte_0x0, 0x0000u, 0x0000, 0x0001u, 0x01u,  0U, 0, 1U, 12u, 0x9ff0u, 0x000fu
#define IO_RST_CTRL__SOFT_WBOOT \
	io0x0, io_byte_0x0, 0x0000u, 0x0000, 0x0002u, 0x02u,  1U, 1, 1U, 12u, 0x9ff0u, 0x000fu
#define IO_RST_CTRL__HVDIG_WBOOT \
	io0x0, io_byte_0x0, 0x0000u, 0x0000, 0x0004u, 0x04u,  2U, 2, 1U, 12u, 0x9ff0u, 0x000fu
#define IO_RST_CTRL__DBG_WBOOT \
	io0x0, io_byte_0x0, 0x0000u, 0x0000, 0x0008u, 0x08u,  3U, 3, 1U, 12u, 0x9ff0u, 0x000fu
#define IO_RST_CTRL__SOFT_RESET \
	io0x0, io_byte_0x1, 0x0000u, 0x0001, 0x2000u, 0x20u, 13U, 5, 1U, 12u, 0x9ff0u, 0x000fu
#define IO_RST_CTRL__HVDIG_USED \
	io0x0, io_byte_0x1, 0x0000u, 0x0001, 0x4000u, 0x40u, 14U, 6, 1U, 12u, 0x9ff0u, 0x000fu
#define IO_RST_CTRL__HVDIG_OK \
	io0x0, io_byte_0x1, 0x0000u, 0x0001, 0x8000u, 0x80u, 15U, 7, 0U, 12u, 0x9ff0u, 0x000fu
#define IO_FUNC_TEST__DISABLE_RESET \
	io0x4, io_byte_0x4, 0x0004u, 0x0004, 0x0001u, 0x01u,  0U, 0, 1U, 12u, 0xc0f8u, 0x0000u
#define IO_FUNC_TEST__DISABLE_SUSPEND \
	io0x4, io_byte_0x4, 0x0004u, 0x0004, 0x0002u, 0x02u,  1U, 1, 1U, 12u, 0xc0f8u, 0x0000u
#define IO_FUNC_TEST__DISABLE_IRQ \
	io0x4, io_byte_0x4, 0x0004u, 0x0004, 0x0004u, 0x04u,  2U, 2, 1U, 12u, 0xc0f8u, 0x0000u
#define IO_FUNC_TEST__FTC_SEL \
	io0x4, io_byte_0x5, 0x0004u, 0x0005, 0x0300u, 0x03u,  8U, 0, 1U, 12u, 0xc0f8u, 0x0000u
#define IO_FUNC_TEST__FTC_DIV \
	io0x4, io_byte_0x5, 0x0004u, 0x0005, 0x3c00u, 0x3cu, 10U, 2, 1U, 12u, 0xc0f8u, 0x0000u
#define IO_FUNC_TEST__TEST_UNPROT \
	io0x6, error_IO_FUNC_TEST__TEST_UNPROT_has_no_byte_access, 0x0006u, 0x0006, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0xffffu
#define IO_MUPET__PTC_RECEIVE \
	io0x8, error_IO_MUPET__PTC_RECEIVE_has_no_byte_access, 0x0008u, 0x0008, 0xffffu, 0xffu,  0U, 0, 0U, 8u, 0x0000u, 0x0000u
#define IO_MUPET__PTC_SEND \
	io0x8, error_IO_MUPET__PTC_SEND_has_no_byte_access, 0x0008u, 0x0008, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_MUPET__PTCA_PEND \
	io0xa, io_byte_0xa, 0x000au, 0x000a, 0x0001u, 0x01u,  0U, 0, 0U, 12u, 0x60ffu, 0x8000u
#define IO_MUPET__PTCS_PEND \
	io0xa, io_byte_0xa, 0x000au, 0x000a, 0x0002u, 0x02u,  1U, 1, 0U, 12u, 0x60ffu, 0x8000u
#define IO_MUPET__PTCR_PEND \
	io0xa, io_byte_0xa, 0x000au, 0x000a, 0x0004u, 0x04u,  2U, 2, 0U, 12u, 0x60ffu, 0x8000u
#define IO_MUPET__PTC_KEY \
	io0xa, io_byte_0xa, 0x000au, 0x000a, 0x0080u, 0x80u,  7U, 7, 0U, 12u, 0x60ffu, 0x8000u
#define IO_MUPET__WARM_TRIGGER \
	io0xa, io_byte_0xb, 0x000au, 0x000b, 0x1f00u, 0x1fu,  8U, 0, 1U, 12u, 0x60ffu, 0x8000u
#define IO_MUPET__CONNECTION \
	io0xa, io_byte_0xb, 0x000au, 0x000b, 0x6000u, 0x60u, 13U, 5, 0U, 12u, 0x60ffu, 0x8000u
#define IO_MUPET__IN_APPLICATION \
	io0xa, io_byte_0xb, 0x000au, 0x000b, 0x8000u, 0x80u, 15U, 7, 1U, 12u, 0x60ffu, 0x8000u
#define IO_AWD__ACK \
	error_IO_AWD__ACK_has_no_word_access, io_byte_0xc, 0x000cu, 0x000c, 0x0040u, 0x40u,  6U, 6, 1U, 7u, 0xffbfu, 0x0040u
#define IO_AWD__WIN_OPEN \
	error_IO_AWD__WIN_OPEN_has_no_word_access, io_byte_0xd, 0x000cu, 0x000d, 0x8000u, 0x80u, 15U, 7, 0U, 7u, 0xffbfu, 0x0040u
#define IO_ROM_BIST__ADD_START_L \
	io0x10, error_IO_ROM_BIST__ADD_START_L_has_no_byte_access, 0x0010u, 0x0010, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0xffffu
#define IO_ROM_BIST__ADD_START_H \
	io0x12, io_byte_0x12, 0x0012u, 0x0012, 0x000fu, 0x0fu,  0U, 0, 1U, 12u, 0xfff0u, 0x000fu
#define IO_ROM_BIST__ADD_STOP_L \
	io0x14, error_IO_ROM_BIST__ADD_STOP_L_has_no_byte_access, 0x0014u, 0x0014, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0xffffu
#define IO_ROM_BIST__ADD_STOP_H \
	io0x16, io_byte_0x16, 0x0016u, 0x0016, 0x000fu, 0x0fu,  0U, 0, 1U, 12u, 0xfff0u, 0x000fu
#define IO_ROM_BIST__SIG_EXPECTED_L \
	io0x18, error_IO_ROM_BIST__SIG_EXPECTED_L_has_no_byte_access, 0x0018u, 0x0018, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_ROM_BIST__SIG_EXPECTED_H \
	io0x1a, io_byte_0x1a, 0x001au, 0x001a, 0x00ffu, 0xffu,  0U, 0, 1U, 12u, 0xff00u, 0x0000u
#define IO_ROM_BIST__ECC_POSITION \
	io0x1c, io_byte_0x1c, 0x001cu, 0x001c, 0x0003u, 0x03u,  0U, 0, 1U, 12u, 0xff8cu, 0x0000u
#define IO_ROM_BIST__BIST \
	io0x1c, io_byte_0x1c, 0x001cu, 0x001c, 0x0010u, 0x10u,  4U, 4, 1U, 12u, 0xff8cu, 0x0000u
#define IO_ROM_BIST__SINGLE_RAMP \
	io0x1c, io_byte_0x1c, 0x001cu, 0x001c, 0x0020u, 0x20u,  5U, 5, 1U, 12u, 0xff8cu, 0x0000u
#define IO_ROM_BIST__MASK_SIG_ERR \
	io0x1c, io_byte_0x1c, 0x001cu, 0x001c, 0x0040u, 0x40u,  6U, 6, 1U, 12u, 0xff8cu, 0x0000u
#define IO_ROM_BIST__BIST_REQUEST \
	io0x1c, io_byte_0x1d, 0x001cu, 0x001d, 0x0200u, 0x02u,  9U, 1, 0U, 12u, 0xff8cu, 0x0000u
#define IO_ROM_BIST__VALID_CLOCK \
	io0x1c, io_byte_0x1d, 0x001cu, 0x001d, 0x0800u, 0x08u, 11U, 3, 0U, 12u, 0xff8cu, 0x0000u
#define IO_ROM_BIST__COMPLETED \
	io0x1c, io_byte_0x1d, 0x001cu, 0x001d, 0x8000u, 0x80u, 15U, 7, 0U, 12u, 0xff8cu, 0x0000u
#define IO_ROM_BIST__START_BIST \
	io0x1e, error_IO_ROM_BIST__START_BIST_has_no_byte_access, 0x001eu, 0x001e, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0xffffu
#define IO_ROM_BIST__SIG_RECEIVED_L \
	io0x20, error_IO_ROM_BIST__SIG_RECEIVED_L_has_no_byte_access, 0x0020u, 0x0020, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0xffffu
#define IO_ROM_BIST__SIG_RECEIVED_H \
	io0x22, io_byte_0x22, 0x0022u, 0x0022, 0x00ffu, 0xffu,  0U, 0, 1U, 12u, 0xff00u, 0x00ffu
#define IO_NVRAM_SHELL__SEC \
	io0x26, io_byte_0x26, 0x0026u, 0x0026, 0x0004u, 0x04u,  2U, 2, 1U, 12u, 0xfcf3u, 0x000cu
#define IO_NVRAM_SHELL__DED \
	io0x26, io_byte_0x26, 0x0026u, 0x0026, 0x0008u, 0x08u,  3U, 3, 1U, 12u, 0xfcf3u, 0x000cu
#define IO_NVRAM_SHELL__STORE_REQ \
	io0x26, io_byte_0x27, 0x0026u, 0x0027, 0x0100u, 0x01u,  8U, 0, 1U, 12u, 0xfcf3u, 0x000cu
#define IO_NVRAM_SHELL__RECALL_REQ \
	io0x26, io_byte_0x27, 0x0026u, 0x0027, 0x0200u, 0x02u,  9U, 1, 1U, 12u, 0xfcf3u, 0x000cu
#define IO_NVRAM_SHELL__LOCK \
	io0x28, io_byte_0x29, 0x0028u, 0x0029, 0x8000u, 0x80u, 15U, 7, 1U, 12u, 0x7fffu, 0x0000u
#define IO_NVRAM_SHELL__DMA_DATA \
	io0x2a, io_byte_0x2a, 0x002au, 0x002a, 0x0001u, 0x01u,  0U, 0, 1U, 12u, 0xe000u, 0x0000u
#define IO_NVRAM_SHELL__DMA_CB \
	io0x2a, io_byte_0x2a, 0x002au, 0x002a, 0x0002u, 0x02u,  1U, 1, 1U, 12u, 0xe000u, 0x0000u
#define IO_NVRAM_SHELL__TM_BIT_VFY \
	io0x2a, io_byte_0x2a, 0x002au, 0x002a, 0x0004u, 0x04u,  2U, 2, 1U, 12u, 0xe000u, 0x0000u
#define IO_NVRAM_SHELL__TM_FULL \
	io0x2a, io_byte_0x2a, 0x002au, 0x002a, 0x0008u, 0x08u,  3U, 3, 1U, 12u, 0xe000u, 0x0000u
#define IO_NVRAM_SHELL__PEIN \
	io0x2a, io_byte_0x2a, 0x002au, 0x002a, 0x0010u, 0x10u,  4U, 4, 1U, 12u, 0xe000u, 0x0000u
#define IO_NVRAM_SHELL__TM_NVCP \
	io0x2a, error_IO_NVRAM_SHELL__TM_NVCP_has_no_byte_access, 0x002au, 0x002a, 0x01e0u, 0xe0u,  5U, 5, 1U, 8u, 0xe000u, 0x0000u
#define IO_NVRAM_SHELL__TM_STORE_REQ \
	io0x2a, io_byte_0x2b, 0x002au, 0x002b, 0x0600u, 0x06u,  9U, 1, 1U, 12u, 0xe000u, 0x0000u
#define IO_NVRAM_SHELL__TM_RECALL_REQ \
	io0x2a, io_byte_0x2b, 0x002au, 0x002b, 0x1800u, 0x18u, 11U, 3, 1U, 12u, 0xe000u, 0x0000u
#define IO_STIMER1__CURRENT \
	io0x2c, error_IO_STIMER1__CURRENT_has_no_byte_access, 0x002cu, 0x002c, 0x3fffu, 0xffu,  0U, 0, 0U, 8u, 0xffffu, 0x0000u
#define IO_STIMER1__VALUE \
	io0x2e, error_IO_STIMER1__VALUE_has_no_byte_access, 0x002eu, 0x002e, 0x3fffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_STIMER1__MODE \
	io0x2e, error_IO_STIMER1__MODE_has_no_byte_access, 0x002eu, 0x002f, 0xc000u, 0xc0u, 14U, 6, 1U, 8u, 0x0000u, 0x0000u
#define IO_STIMER2__CURRENT \
	io0x30, error_IO_STIMER2__CURRENT_has_no_byte_access, 0x0030u, 0x0030, 0x3fffu, 0xffu,  0U, 0, 0U, 8u, 0xffffu, 0x0000u
#define IO_STIMER2__VALUE \
	io0x32, error_IO_STIMER2__VALUE_has_no_byte_access, 0x0032u, 0x0032, 0x3fffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_STIMER2__MODE \
	io0x32, error_IO_STIMER2__MODE_has_no_byte_access, 0x0032u, 0x0033, 0xc000u, 0xc0u, 14U, 6, 1U, 8u, 0x0000u, 0x0000u
#define IO_MLX16__XCG_STAT \
	io0x38, error_IO_MLX16__XCG_STAT_has_no_byte_access, 0x0038u, 0x0038, 0xffffu, 0xffu,  0U, 0, 0U, 8u, 0xffffu, 0x0000u
#define IO_MLX16__XCG_DR1 \
	io0x3a, error_IO_MLX16__XCG_DR1_has_no_byte_access, 0x003au, 0x003a, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_MLX16__XCG_DR3 \
	io0x3c, error_IO_MLX16__XCG_DR3_has_no_byte_access, 0x003cu, 0x003c, 0xffffu, 0xffu,  0U, 0, 0U, 8u, 0xffffu, 0x0000u
#define IO_MLX16__SHELL_VERSION \
	io0x3e, error_IO_MLX16__SHELL_VERSION_has_no_byte_access, 0x003eu, 0x003e, 0xffffu, 0xffu,  0U, 0, 0U, 8u, 0xffffu, 0x0000u
#define IO_MLX16__ITC_PEND0 \
	io0x40, error_IO_MLX16__ITC_PEND0_has_no_byte_access, 0x0040u, 0x0040, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0xffffu
#define IO_MLX16__MLX16_EXCHG_PEND \
	io0x40, io_byte_0x40, 0x0040u, 0x0040, 0x0001u, 0x01u,  0U, 0, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__MLX16_DMAERR_PEND \
	io0x40, io_byte_0x40, 0x0040u, 0x0040, 0x0002u, 0x02u,  1U, 1, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__AWD_ATT_PEND \
	io0x40, io_byte_0x40, 0x0040u, 0x0040, 0x0004u, 0x04u,  2U, 2, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__ROM_SHELL_MEM_ERR_PEND \
	io0x40, io_byte_0x40, 0x0040u, 0x0040, 0x0008u, 0x08u,  3U, 3, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__FLASH_SHELL_ERR_PEND \
	io0x40, io_byte_0x40, 0x0040u, 0x0040, 0x0010u, 0x10u,  4U, 4, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__RAM_SHELL_MEM_ERR_PEND \
	io0x40, io_byte_0x40, 0x0040u, 0x0040, 0x0020u, 0x20u,  5U, 5, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__NVRAM_SHELL_STORE_PEND \
	io0x40, io_byte_0x40, 0x0040u, 0x0040, 0x0040u, 0x40u,  6U, 6, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__STIMER1_PEND \
	io0x40, io_byte_0x40, 0x0040u, 0x0040, 0x0080u, 0x80u,  7U, 7, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__STIMER2_PEND \
	io0x40, io_byte_0x41, 0x0040u, 0x0041, 0x0100u, 0x01u,  8U, 0, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__ADC_XH018_PEND \
	io0x40, io_byte_0x41, 0x0040u, 0x0041, 0x0200u, 0x02u,  9U, 1, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__GPIO0_PEND \
	io0x40, io_byte_0x41, 0x0040u, 0x0041, 0x0400u, 0x04u, 10U, 2, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__GPIO1_PEND \
	io0x40, io_byte_0x41, 0x0040u, 0x0041, 0x0800u, 0x08u, 11U, 3, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__GPIO2_PEND \
	io0x40, io_byte_0x41, 0x0040u, 0x0041, 0x1000u, 0x10u, 12U, 4, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__GPIO3_PEND \
	io0x40, io_byte_0x41, 0x0040u, 0x0041, 0x2000u, 0x20u, 13U, 5, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__DIAG0_PEND \
	io0x40, io_byte_0x41, 0x0040u, 0x0041, 0x4000u, 0x40u, 14U, 6, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__DIAG1_PEND \
	io0x40, io_byte_0x41, 0x0040u, 0x0041, 0x8000u, 0x80u, 15U, 7, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__DIAG2_PEND \
	io0x42, io_byte_0x42, 0x0042u, 0x0042, 0x0001u, 0x01u,  0U, 0, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__ITC_PEND1 \
	io0x42, error_IO_MLX16__ITC_PEND1_has_no_byte_access, 0x0042u, 0x0042, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0xffffu
#define IO_MLX16__DIAG3_PEND \
	io0x42, io_byte_0x42, 0x0042u, 0x0042, 0x0002u, 0x02u,  1U, 1, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__WUPTMR_PEND \
	io0x42, io_byte_0x42, 0x0042u, 0x0042, 0x0004u, 0x04u,  2U, 2, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__TEMPSENS_PEND \
	io0x42, io_byte_0x42, 0x0042u, 0x0042, 0x0008u, 0x08u,  3U, 3, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__RFTX_LOCKED_PEND \
	io0x42, io_byte_0x42, 0x0042u, 0x0042, 0x0010u, 0x10u,  4U, 4, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__RFTX_HALF_PEND \
	io0x42, io_byte_0x42, 0x0042u, 0x0042, 0x0020u, 0x20u,  5U, 5, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__RFTX_DONE_PEND \
	io0x42, io_byte_0x42, 0x0042u, 0x0042, 0x0040u, 0x40u,  6U, 6, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__LFTX_HALF_PEND \
	io0x42, io_byte_0x42, 0x0042u, 0x0042, 0x0080u, 0x80u,  7U, 7, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__LFTX_DONE_PEND \
	io0x42, io_byte_0x43, 0x0042u, 0x0043, 0x0100u, 0x01u,  8U, 0, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__LFRX_SNIFF_PEND \
	io0x42, io_byte_0x43, 0x0042u, 0x0043, 0x0200u, 0x02u,  9U, 1, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__LFRX_TIMEOUT_PEND \
	io0x42, io_byte_0x43, 0x0042u, 0x0043, 0x0400u, 0x04u, 10U, 2, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__LFRX_SYNC_PEND \
	io0x42, io_byte_0x43, 0x0042u, 0x0043, 0x0800u, 0x08u, 11U, 3, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__LFRX_DATA_PEND \
	io0x42, io_byte_0x43, 0x0042u, 0x0043, 0x1000u, 0x10u, 12U, 4, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__LFRX_DONE_PEND \
	io0x42, io_byte_0x43, 0x0042u, 0x0043, 0x2000u, 0x20u, 13U, 5, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__FREQCNT_DONE_PEND \
	io0x42, io_byte_0x43, 0x0042u, 0x0043, 0x4000u, 0x40u, 14U, 6, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__MLX16_SOFT_PEND \
	io0x42, io_byte_0x43, 0x0042u, 0x0043, 0x8000u, 0x80u, 15U, 7, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__SWI \
	io0x5a, io_byte_0x5b, 0x005au, 0x005b, 0x8000u, 0x80u, 15U, 7, 1U, 12u, 0x7fffu, 0x0000u
#define IO_MLX16__ITC_MASK0 \
	io0x5c, error_IO_MLX16__ITC_MASK0_has_no_byte_access, 0x005cu, 0x005c, 0xffffu, 0xffu,  0U, 0, 1U, 10u, 0x0000u, 0x0000u
#define IO_MLX16__MLX16_EXCHG_ITC \
	io0x5c, io_byte_0x5c, 0x005cu, 0x005c, 0x0001u, 0x01u,  0U, 0, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__MLX16_DMAERR_ITC \
	io0x5c, io_byte_0x5c, 0x005cu, 0x005c, 0x0002u, 0x02u,  1U, 1, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__AWD_ATT_ITC \
	io0x5c, io_byte_0x5c, 0x005cu, 0x005c, 0x0004u, 0x04u,  2U, 2, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__ROM_SHELL_MEM_ERR_ITC \
	io0x5c, io_byte_0x5c, 0x005cu, 0x005c, 0x0008u, 0x08u,  3U, 3, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__FLASH_SHELL_ERR_ITC \
	io0x5c, io_byte_0x5c, 0x005cu, 0x005c, 0x0010u, 0x10u,  4U, 4, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__RAM_SHELL_MEM_ERR_ITC \
	io0x5c, io_byte_0x5c, 0x005cu, 0x005c, 0x0020u, 0x20u,  5U, 5, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__NVRAM_SHELL_STORE_ITC \
	io0x5c, io_byte_0x5c, 0x005cu, 0x005c, 0x0040u, 0x40u,  6U, 6, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__STIMER1_ITC \
	io0x5c, io_byte_0x5c, 0x005cu, 0x005c, 0x0080u, 0x80u,  7U, 7, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__STIMER2_ITC \
	io0x5c, io_byte_0x5d, 0x005cu, 0x005d, 0x0100u, 0x01u,  8U, 0, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__ADC_XH018_ITC \
	io0x5c, io_byte_0x5d, 0x005cu, 0x005d, 0x0200u, 0x02u,  9U, 1, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__GPIO0_ITC \
	io0x5c, io_byte_0x5d, 0x005cu, 0x005d, 0x0400u, 0x04u, 10U, 2, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__GPIO1_ITC \
	io0x5c, io_byte_0x5d, 0x005cu, 0x005d, 0x0800u, 0x08u, 11U, 3, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__GPIO2_ITC \
	io0x5c, io_byte_0x5d, 0x005cu, 0x005d, 0x1000u, 0x10u, 12U, 4, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__GPIO3_ITC \
	io0x5c, io_byte_0x5d, 0x005cu, 0x005d, 0x2000u, 0x20u, 13U, 5, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__DIAG0_ITC \
	io0x5c, io_byte_0x5d, 0x005cu, 0x005d, 0x4000u, 0x40u, 14U, 6, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__DIAG1_ITC \
	io0x5c, io_byte_0x5d, 0x005cu, 0x005d, 0x8000u, 0x80u, 15U, 7, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__DIAG2_ITC \
	io0x5e, io_byte_0x5e, 0x005eu, 0x005e, 0x0001u, 0x01u,  0U, 0, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__ITC_MASK1 \
	io0x5e, error_IO_MLX16__ITC_MASK1_has_no_byte_access, 0x005eu, 0x005e, 0xffffu, 0xffu,  0U, 0, 1U, 10u, 0x0000u, 0x0000u
#define IO_MLX16__DIAG3_ITC \
	io0x5e, io_byte_0x5e, 0x005eu, 0x005e, 0x0002u, 0x02u,  1U, 1, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__WUPTMR_ITC \
	io0x5e, io_byte_0x5e, 0x005eu, 0x005e, 0x0004u, 0x04u,  2U, 2, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__TEMPSENS_ITC \
	io0x5e, io_byte_0x5e, 0x005eu, 0x005e, 0x0008u, 0x08u,  3U, 3, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__RFTX_LOCKED_ITC \
	io0x5e, io_byte_0x5e, 0x005eu, 0x005e, 0x0010u, 0x10u,  4U, 4, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__RFTX_HALF_ITC \
	io0x5e, io_byte_0x5e, 0x005eu, 0x005e, 0x0020u, 0x20u,  5U, 5, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__RFTX_DONE_ITC \
	io0x5e, io_byte_0x5e, 0x005eu, 0x005e, 0x0040u, 0x40u,  6U, 6, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__LFTX_HALF_ITC \
	io0x5e, io_byte_0x5e, 0x005eu, 0x005e, 0x0080u, 0x80u,  7U, 7, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__LFTX_DONE_ITC \
	io0x5e, io_byte_0x5f, 0x005eu, 0x005f, 0x0100u, 0x01u,  8U, 0, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__LFRX_SNIFF_ITC \
	io0x5e, io_byte_0x5f, 0x005eu, 0x005f, 0x0200u, 0x02u,  9U, 1, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__LFRX_TIMEOUT_ITC \
	io0x5e, io_byte_0x5f, 0x005eu, 0x005f, 0x0400u, 0x04u, 10U, 2, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__LFRX_SYNC_ITC \
	io0x5e, io_byte_0x5f, 0x005eu, 0x005f, 0x0800u, 0x08u, 11U, 3, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__LFRX_DATA_ITC \
	io0x5e, io_byte_0x5f, 0x005eu, 0x005f, 0x1000u, 0x10u, 12U, 4, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__LFRX_DONE_ITC \
	io0x5e, io_byte_0x5f, 0x005eu, 0x005f, 0x2000u, 0x20u, 13U, 5, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__FREQCNT_DONE_ITC \
	io0x5e, io_byte_0x5f, 0x005eu, 0x005f, 0x4000u, 0x40u, 14U, 6, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__MLX16_SOFT_ITC \
	io0x5e, io_byte_0x5f, 0x005eu, 0x005f, 0x8000u, 0x80u, 15U, 7, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__ITC_PRIO0 \
	io0x78, error_IO_MLX16__ITC_PRIO0_has_no_byte_access, 0x0078u, 0x0078, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_MLX16__ROM_SHELL_MEM_ERR_PRIO \
	io0x78, io_byte_0x78, 0x0078u, 0x0078, 0x0003u, 0x03u,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__FLASH_SHELL_ERR_PRIO \
	io0x78, io_byte_0x78, 0x0078u, 0x0078, 0x000cu, 0x0cu,  2U, 2, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__RAM_SHELL_MEM_ERR_PRIO \
	io0x78, io_byte_0x78, 0x0078u, 0x0078, 0x0030u, 0x30u,  4U, 4, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__NVRAM_SHELL_STORE_PRIO \
	io0x78, io_byte_0x78, 0x0078u, 0x0078, 0x00c0u, 0xc0u,  6U, 6, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__STIMER1_PRIO \
	io0x78, io_byte_0x79, 0x0078u, 0x0079, 0x0300u, 0x03u,  8U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__STIMER2_PRIO \
	io0x78, io_byte_0x79, 0x0078u, 0x0079, 0x0c00u, 0x0cu, 10U, 2, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__ADC_XH018_PRIO \
	io0x78, io_byte_0x79, 0x0078u, 0x0079, 0x3000u, 0x30u, 12U, 4, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__GPIO0_PRIO \
	io0x78, io_byte_0x79, 0x0078u, 0x0079, 0xc000u, 0xc0u, 14U, 6, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__GPIO1_PRIO \
	io0x7a, io_byte_0x7a, 0x007au, 0x007a, 0x0003u, 0x03u,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__ITC_PRIO1 \
	io0x7a, error_IO_MLX16__ITC_PRIO1_has_no_byte_access, 0x007au, 0x007a, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_MLX16__GPIO2_PRIO \
	io0x7a, io_byte_0x7a, 0x007au, 0x007a, 0x000cu, 0x0cu,  2U, 2, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__GPIO3_PRIO \
	io0x7a, io_byte_0x7a, 0x007au, 0x007a, 0x0030u, 0x30u,  4U, 4, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__DIAG0_PRIO \
	io0x7a, io_byte_0x7a, 0x007au, 0x007a, 0x00c0u, 0xc0u,  6U, 6, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__DIAG1_PRIO \
	io0x7a, io_byte_0x7b, 0x007au, 0x007b, 0x0300u, 0x03u,  8U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__DIAG2_PRIO \
	io0x7a, io_byte_0x7b, 0x007au, 0x007b, 0x0c00u, 0x0cu, 10U, 2, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__DIAG3_PRIO \
	io0x7a, io_byte_0x7b, 0x007au, 0x007b, 0x3000u, 0x30u, 12U, 4, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__WUPTMR_PRIO \
	io0x7a, io_byte_0x7b, 0x007au, 0x007b, 0xc000u, 0xc0u, 14U, 6, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__ITC_PRIO2 \
	io0x7c, error_IO_MLX16__ITC_PRIO2_has_no_byte_access, 0x007cu, 0x007c, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_MLX16__TEMPSENS_PRIO \
	io0x7c, io_byte_0x7c, 0x007cu, 0x007c, 0x0003u, 0x03u,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__RFTX_LOCKED_PRIO \
	io0x7c, io_byte_0x7c, 0x007cu, 0x007c, 0x000cu, 0x0cu,  2U, 2, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__RFTX_HALF_PRIO \
	io0x7c, io_byte_0x7c, 0x007cu, 0x007c, 0x0030u, 0x30u,  4U, 4, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__RFTX_DONE_PRIO \
	io0x7c, io_byte_0x7c, 0x007cu, 0x007c, 0x00c0u, 0xc0u,  6U, 6, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__LFTX_HALF_PRIO \
	io0x7c, io_byte_0x7d, 0x007cu, 0x007d, 0x0300u, 0x03u,  8U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__LFTX_DONE_PRIO \
	io0x7c, io_byte_0x7d, 0x007cu, 0x007d, 0x0c00u, 0x0cu, 10U, 2, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__LFRX_SNIFF_PRIO \
	io0x7c, io_byte_0x7d, 0x007cu, 0x007d, 0x3000u, 0x30u, 12U, 4, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__LFRX_TIMEOUT_PRIO \
	io0x7c, io_byte_0x7d, 0x007cu, 0x007d, 0xc000u, 0xc0u, 14U, 6, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__ITC_PRIO3 \
	io0x7e, io_byte_0x7e, 0x007eu, 0x007e, 0x00ffu, 0xffu,  0U, 0, 1U, 12u, 0xff00u, 0x0000u
#define IO_MLX16__LFRX_SYNC_PRIO \
	io0x7e, io_byte_0x7e, 0x007eu, 0x007e, 0x0003u, 0x03u,  0U, 0, 1U, 12u, 0xff00u, 0x0000u
#define IO_MLX16__LFRX_DATA_PRIO \
	io0x7e, io_byte_0x7e, 0x007eu, 0x007e, 0x000cu, 0x0cu,  2U, 2, 1U, 12u, 0xff00u, 0x0000u
#define IO_MLX16__LFRX_DONE_PRIO \
	io0x7e, io_byte_0x7e, 0x007eu, 0x007e, 0x0030u, 0x30u,  4U, 4, 1U, 12u, 0xff00u, 0x0000u
#define IO_MLX16__FREQCNT_DONE_PRIO \
	io0x7e, io_byte_0x7e, 0x007eu, 0x007e, 0x00c0u, 0xc0u,  6U, 6, 1U, 12u, 0xff00u, 0x0000u
#define IO_MLX16__CPU_FP0ADR \
	io0xae, error_IO_MLX16__CPU_FP0ADR_has_no_byte_access, 0x00aeu, 0x00ae, 0x0fffu, 0xffu,  0U, 0, 1U, 8u, 0xf000u, 0x0000u
#define IO_MLX16__PATCH_ADDR0 \
	io0xb0, error_IO_MLX16__PATCH_ADDR0_has_no_byte_access, 0x00b0u, 0x00b0, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_MLX16__PATCH_INSTR0 \
	io0xb2, error_IO_MLX16__PATCH_INSTR0_has_no_byte_access, 0x00b2u, 0x00b2, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_MLX16__PATCH_ADDR1 \
	io0xb4, error_IO_MLX16__PATCH_ADDR1_has_no_byte_access, 0x00b4u, 0x00b4, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_MLX16__PATCH_INSTR1 \
	io0xb6, error_IO_MLX16__PATCH_INSTR1_has_no_byte_access, 0x00b6u, 0x00b6, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_MLX16__PATCH_ADDR2 \
	io0xb8, error_IO_MLX16__PATCH_ADDR2_has_no_byte_access, 0x00b8u, 0x00b8, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_MLX16__PATCH_INSTR2 \
	io0xba, error_IO_MLX16__PATCH_INSTR2_has_no_byte_access, 0x00bau, 0x00ba, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_MLX16__PATCH_ADDR3 \
	io0xbc, error_IO_MLX16__PATCH_ADDR3_has_no_byte_access, 0x00bcu, 0x00bc, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_MLX16__PATCH_INSTR3 \
	io0xbe, error_IO_MLX16__PATCH_INSTR3_has_no_byte_access, 0x00beu, 0x00be, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_VERSION__VERSION_L \
	io0x100, error_IO_VERSION__VERSION_L_has_no_byte_access, 0x0100u, 0x0100, 0xffffu, 0xffu,  0U, 0, 0U, 8u, 0xffffu, 0x0000u
#define IO_VERSION__VERSION_H \
	io0x102, error_IO_VERSION__VERSION_H_has_no_byte_access, 0x0102u, 0x0102, 0xffffu, 0xffu,  0U, 0, 0U, 8u, 0xffffu, 0x0000u
#define IO_FLASH_SHELL__COMMAND \
	io0x104, error_IO_FLASH_SHELL__COMMAND_has_no_byte_access, 0x0104u, 0x0104, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_FLASH_SHELL__STATUS \
	io0x104, io_byte_0x104, 0x0104u, 0x0104, 0x001fu, 0x1fu,  0U, 0, 0U, 12u, 0x0000u, 0x0000u
#define IO_FLASH_SHELL__VERSION \
	io0x104, io_byte_0x105, 0x0104u, 0x0105, 0xff00u, 0xffu,  8U, 0, 0U, 12u, 0x0000u, 0x0000u
#define IO_FLASH_SHELL__WR_TIME \
	io0x106, io_byte_0x106, 0x0106u, 0x0106, 0x007fu, 0x7fu,  0U, 0, 1U, 12u, 0x0080u, 0x0000u
#define IO_FLASH_SHELL__ER_TIME \
	io0x106, io_byte_0x107, 0x0106u, 0x0107, 0x7f00u, 0x7fu,  8U, 0, 1U, 12u, 0x0080u, 0x0000u
#define IO_FLASH_SHELL__LOCK_ER_WR \
	io0x106, io_byte_0x107, 0x0106u, 0x0107, 0x8000u, 0x80u, 15U, 7, 1U, 12u, 0x0080u, 0x0000u
#define IO_FLASH_SHELL__T_50NS \
	io0x108, io_byte_0x108, 0x0108u, 0x0108, 0x0007u, 0x07u,  0U, 0, 1U, 12u, 0x40f8u, 0x0000u
#define IO_FLASH_SHELL__T_30US \
	io0x108, io_byte_0x109, 0x0108u, 0x0109, 0x3f00u, 0x3fu,  8U, 0, 1U, 12u, 0x40f8u, 0x0000u
#define IO_FLASH_SHELL__LOCK_T \
	io0x108, io_byte_0x109, 0x0108u, 0x0109, 0x8000u, 0x80u, 15U, 7, 1U, 12u, 0x40f8u, 0x0000u
#define IO_FLASH_SHELL__READY \
	io0x10a, io_byte_0x10a, 0x010au, 0x010a, 0x000fu, 0x0fu,  0U, 0, 1U, 12u, 0x7080u, 0x0000u
#define IO_FLASH_SHELL__DED_RETRY \
	io0x10a, io_byte_0x10a, 0x010au, 0x010a, 0x0070u, 0x70u,  4U, 4, 1U, 12u, 0x7080u, 0x0000u
#define IO_FLASH_SHELL__BYPASS_QUEUE \
	io0x10a, io_byte_0x10b, 0x010au, 0x010b, 0x0100u, 0x01u,  8U, 0, 1U, 12u, 0x7080u, 0x0000u
#define IO_FLASH_SHELL__SYNCHRONOUS \
	io0x10a, io_byte_0x10b, 0x010au, 0x010b, 0x0200u, 0x02u,  9U, 1, 1U, 12u, 0x7080u, 0x0000u
#define IO_FLASH_SHELL__HALT_BEHAVIOR \
	io0x10a, io_byte_0x10b, 0x010au, 0x010b, 0x0c00u, 0x0cu, 10U, 2, 1U, 12u, 0x7080u, 0x0000u
#define IO_FLASH_SHELL__LOCK_RDY \
	io0x10a, io_byte_0x10b, 0x010au, 0x010b, 0x8000u, 0x80u, 15U, 7, 1U, 12u, 0x7080u, 0x0000u
#define IO_FLASH_SHELL__SEC_COUNTER \
	io0x10c, error_IO_FLASH_SHELL__SEC_COUNTER_has_no_byte_access, 0x010cu, 0x010c, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_FLASH_SHELL__BIT_ERRORS \
	io0x10e, error_IO_FLASH_SHELL__BIT_ERRORS_has_no_byte_access, 0x010eu, 0x010e, 0x0fffu, 0xffu,  0U, 0, 1U, 8u, 0x8000u, 0x0000u
#define IO_FLASH_SHELL__PATTERN_ID \
	io0x10e, io_byte_0x10f, 0x010eu, 0x010f, 0x7000u, 0x70u, 12U, 4, 1U, 12u, 0x8000u, 0x0000u
#define IO_FLASH_SHELL__PL_TRL \
	io0x110, error_IO_FLASH_SHELL__PL_TRL_has_no_byte_access, 0x0110u, 0x0110, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0xffffu
#define IO_FLASH_SHELL__TRH \
	io0x112, error_IO_FLASH_SHELL__TRH_has_no_byte_access, 0x0112u, 0x0112, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0xffffu
#define IO_FLASH_SHELL__PL_TR_AD \
	io0x114, error_IO_FLASH_SHELL__PL_TR_AD_has_no_byte_access, 0x0114u, 0x0114, 0x03ffu, 0xffu,  0U, 0, 1U, 8u, 0xfc00u, 0x0000u
#define IO_FLASH_SHELL__T_TEST \
	io0x116, io_byte_0x116, 0x0116u, 0x0116, 0x0001u, 0x01u,  0U, 0, 1U, 12u, 0xf0fcu, 0x0000u
#define IO_FLASH_SHELL__T_CLK \
	io0x116, io_byte_0x116, 0x0116u, 0x0116, 0x0002u, 0x02u,  1U, 1, 1U, 12u, 0xf0fcu, 0x0000u
#define IO_FLASH_SHELL__T_DESELECT \
	io0x116, io_byte_0x117, 0x0116u, 0x0117, 0x0f00u, 0x0fu,  8U, 0, 1U, 12u, 0xf0fcu, 0x0000u
#define IO_FLASH_SHELL__FL1_D_ATD \
	io0x11a, io_byte_0x11a, 0x011au, 0x011a, 0x00ffu, 0xffu,  0U, 0, 1U, 12u, 0xf000u, 0x0000u
#define IO_FLASH_SHELL__FL1_SW_A_RM \
	io0x11a, io_byte_0x11b, 0x011au, 0x011b, 0x0f00u, 0x0fu,  8U, 0, 1U, 12u, 0xf000u, 0x0000u
#define IO_FLASH_SHELL__SIG_L \
	io0x13a, error_IO_FLASH_SHELL__SIG_L_has_no_byte_access, 0x013au, 0x013a, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0xffffu
#define IO_FLASH_SHELL__SIG_H \
	io0x13c, error_IO_FLASH_SHELL__SIG_H_has_no_byte_access, 0x013cu, 0x013c, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0xffffu
#define IO_RAM_BIST__KEY \
	io0x13e, error_IO_RAM_BIST__KEY_has_no_byte_access, 0x013eu, 0x013e, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_RAM_BIST__PHASE \
	io0x13e, error_IO_RAM_BIST__PHASE_has_no_byte_access, 0x013eu, 0x013e, 0x0007u, 0x07u,  0U, 0, 0U, 8u, 0x0000u, 0x0000u
#define IO_RAM_BIST__TRANSPARENT \
	io0x13e, error_IO_RAM_BIST__TRANSPARENT_has_no_byte_access, 0x013eu, 0x013f, 0x0100u, 0x01u,  8U, 0, 0U, 8u, 0x0000u, 0x0000u
#define IO_RAM_BIST__REGULAR \
	io0x13e, error_IO_RAM_BIST__REGULAR_has_no_byte_access, 0x013eu, 0x013f, 0x0200u, 0x02u,  9U, 1, 0U, 8u, 0x0000u, 0x0000u
#define IO_RAM_BIST__RUNNING \
	io0x13e, error_IO_RAM_BIST__RUNNING_has_no_byte_access, 0x013eu, 0x013f, 0x0400u, 0x04u, 10U, 2, 0U, 8u, 0x0000u, 0x0000u
#define IO_RAM_BIST__VALID_CLOCK \
	io0x13e, error_IO_RAM_BIST__VALID_CLOCK_has_no_byte_access, 0x013eu, 0x013f, 0x0800u, 0x08u, 11U, 3, 0U, 8u, 0x0000u, 0x0000u
#define IO_RAM_BIST__COMPLETED \
	io0x13e, error_IO_RAM_BIST__COMPLETED_has_no_byte_access, 0x013eu, 0x013f, 0x8000u, 0x80u, 15U, 7, 0U, 8u, 0x0000u, 0x0000u
#define IO_RAM_BIST__LFSR \
	io0x140, error_IO_RAM_BIST__LFSR_has_no_byte_access, 0x0140u, 0x0140, 0xffffu, 0xffu,  0U, 0, 0U, 8u, 0xffffu, 0x0000u
#define IO_RAM_BIST__LSFR_GOT \
	io0x140, error_IO_RAM_BIST__LSFR_GOT_has_no_byte_access, 0x0140u, 0x0140, 0xffffu, 0xffu,  0U, 0, 0U, 8u, 0xffffu, 0x0000u
#define IO_RAM_BIST__LFSR_EXPECTED \
	io0x142, error_IO_RAM_BIST__LFSR_EXPECTED_has_no_byte_access, 0x0142u, 0x0142, 0xffffu, 0xffu,  0U, 0, 0U, 8u, 0xffffu, 0x0000u
#define IO_RAM_BIST__ADL \
	io0x144, error_IO_RAM_BIST__ADL_has_no_byte_access, 0x0144u, 0x0144, 0xffffu, 0xffu,  0U, 0, 0U, 8u, 0xffffu, 0x0000u
#define IO_RAM_BIST__ADH \
	io0x146, error_IO_RAM_BIST__ADH_has_no_byte_access, 0x0146u, 0x0146, 0x000fu, 0x0fu,  0U, 0, 0U, 8u, 0xffffu, 0x0000u
#define IO_RAM_BIST__ADD_START_L \
	io0x148, error_IO_RAM_BIST__ADD_START_L_has_no_byte_access, 0x0148u, 0x0148, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_RAM_BIST__ADD_START_H \
	io0x14a, io_byte_0x14a, 0x014au, 0x014a, 0x000fu, 0x0fu,  0U, 0, 1U, 12u, 0xfff0u, 0x0000u
#define IO_RAM_BIST__ADD_STOP_L \
	io0x14c, error_IO_RAM_BIST__ADD_STOP_L_has_no_byte_access, 0x014cu, 0x014c, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_RAM_BIST__ADD_STOP_H \
	io0x14e, io_byte_0x14e, 0x014eu, 0x014e, 0x000fu, 0x0fu,  0U, 0, 1U, 12u, 0xfff0u, 0x0000u
#define IO_RAM_BIST__NB_ECC_BITS \
	io0x150, io_byte_0x150, 0x0150u, 0x0150, 0x001fu, 0x1fu,  0U, 0, 1U, 12u, 0xe8e0u, 0x0000u
#define IO_RAM_BIST__ADD_SCRAMBLE \
	io0x150, io_byte_0x151, 0x0150u, 0x0151, 0x0700u, 0x07u,  8U, 0, 1U, 12u, 0xe8e0u, 0x0000u
#define IO_RAM_BIST__SIGNATURE_INIT \
	io0x150, io_byte_0x151, 0x0150u, 0x0151, 0x1000u, 0x10u, 12U, 4, 1U, 12u, 0xe8e0u, 0x0000u
#define IO_ADC_XH018__START \
	io0x152, io_byte_0x152, 0x0152u, 0x0152, 0x0001u, 0x01u,  0U, 0, 1U, 12u, 0xe000u, 0x0003u
#define IO_ADC_XH018__STOP \
	io0x152, io_byte_0x152, 0x0152u, 0x0152, 0x0002u, 0x02u,  1U, 1, 1U, 12u, 0xe000u, 0x0003u
#define IO_ADC_XH018__SOS_SOURCE \
	io0x152, io_byte_0x152, 0x0152u, 0x0152, 0x000cu, 0x0cu,  2U, 2, 1U, 12u, 0xe000u, 0x0003u
#define IO_ADC_XH018__SOC_SOURCE \
	io0x152, io_byte_0x152, 0x0152u, 0x0152, 0x0030u, 0x30u,  4U, 4, 1U, 12u, 0xe000u, 0x0003u
#define IO_ADC_XH018__NO_INTERLEAVE \
	io0x152, io_byte_0x152, 0x0152u, 0x0152, 0x0040u, 0x40u,  6U, 6, 1U, 12u, 0xe000u, 0x0003u
#define IO_ADC_XH018__SATURATE \
	io0x152, io_byte_0x152, 0x0152u, 0x0152, 0x0080u, 0x80u,  7U, 7, 1U, 12u, 0xe000u, 0x0003u
#define IO_ADC_XH018__INT_SCHEME \
	io0x152, io_byte_0x153, 0x0152u, 0x0153, 0x0300u, 0x03u,  8U, 0, 1U, 12u, 0xe000u, 0x0003u
#define IO_ADC_XH018__ASB \
	io0x152, io_byte_0x153, 0x0152u, 0x0153, 0x0c00u, 0x0cu, 10U, 2, 1U, 12u, 0xe000u, 0x0003u
#define IO_ADC_XH018__ADC_WIDTH \
	io0x152, io_byte_0x153, 0x0152u, 0x0153, 0x1000u, 0x10u, 12U, 4, 1U, 12u, 0xe000u, 0x0003u
#define IO_ADC_XH018__SBASE_0 \
	io0x154, error_IO_ADC_XH018__SBASE_0_has_no_byte_access, 0x0154u, 0x0154, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0xffffu
#define IO_ADC_XH018__PAUSE \
	io0x156, io_byte_0x156, 0x0156u, 0x0156, 0x0001u, 0x01u,  0U, 0, 1U, 12u, 0xe0f8u, 0x1f07u
#define IO_ADC_XH018__RESUME \
	io0x156, io_byte_0x156, 0x0156u, 0x0156, 0x0002u, 0x02u,  1U, 1, 1U, 12u, 0xe0f8u, 0x1f07u
#define IO_ADC_XH018__SW_TRIG \
	io0x156, io_byte_0x156, 0x0156u, 0x0156, 0x0004u, 0x04u,  2U, 2, 1U, 12u, 0xe0f8u, 0x1f07u
#define IO_ADC_XH018__LAST_INT_SRC \
	io0x156, io_byte_0x156, 0x0156u, 0x0156, 0x0030u, 0x30u,  4U, 4, 0U, 12u, 0xe0f8u, 0x1f07u
#define IO_ADC_XH018__STATE \
	io0x156, io_byte_0x156, 0x0156u, 0x0156, 0x00c0u, 0xc0u,  6U, 6, 0U, 12u, 0xe0f8u, 0x1f07u
#define IO_ADC_XH018__ADC_OVF \
	io0x156, io_byte_0x157, 0x0156u, 0x0157, 0x0100u, 0x01u,  8U, 0, 1U, 12u, 0xe0f8u, 0x1f07u
#define IO_ADC_XH018__ADC_ERR \
	io0x156, io_byte_0x157, 0x0156u, 0x0157, 0x0200u, 0x02u,  9U, 1, 1U, 12u, 0xe0f8u, 0x1f07u
#define IO_ADC_XH018__MEM_ERR \
	io0x156, io_byte_0x157, 0x0156u, 0x0157, 0x0400u, 0x04u, 10U, 2, 1U, 12u, 0xe0f8u, 0x1f07u
#define IO_ADC_XH018__FRAME_ERR \
	io0x156, io_byte_0x157, 0x0156u, 0x0157, 0x0800u, 0x08u, 11U, 3, 1U, 12u, 0xe0f8u, 0x1f07u
#define IO_ADC_XH018__ABORTED \
	io0x156, io_byte_0x157, 0x0156u, 0x0157, 0x1000u, 0x10u, 12U, 4, 1U, 12u, 0xe0f8u, 0x1f07u
#define IO_ADC_XH018__ADC_CLK_DIV \
	io0x158, io_byte_0x158, 0x0158u, 0x0158, 0x007fu, 0x7fu,  0U, 0, 1U, 12u, 0xff80u, 0x0000u
#define IO_ADC_XH018__COUNT \
	io0x15a, io_byte_0x15a, 0x015au, 0x015a, 0x0007u, 0x07u,  0U, 0, 1U, 12u, 0xf000u, 0x0000u
#define IO_ADC_XH018__TYPE \
	io0x15a, io_byte_0x15a, 0x015au, 0x015a, 0x0008u, 0x08u,  3U, 3, 1U, 12u, 0xf000u, 0x0000u
#define IO_ADC_XH018__MODE \
	io0x15a, io_byte_0x15a, 0x015au, 0x015a, 0x0010u, 0x10u,  4U, 4, 1U, 12u, 0xf000u, 0x0000u
#define IO_ADC_XH018__FR \
	io0x15a, io_byte_0x15a, 0x015au, 0x015a, 0x0060u, 0x60u,  5U, 5, 1U, 12u, 0xf000u, 0x0000u
#define IO_ADC_XH018__START_PHI \
	io0x15a, io_byte_0x15a, 0x015au, 0x015a, 0x0080u, 0x80u,  7U, 7, 1U, 12u, 0xf000u, 0x0000u
#define IO_ADC_XH018__OUTMODE \
	io0x15a, io_byte_0x15b, 0x015au, 0x015b, 0x0100u, 0x01u,  8U, 0, 1U, 12u, 0xf000u, 0x0000u
#define IO_ADC_XH018__FORCE \
	io0x15a, io_byte_0x15b, 0x015au, 0x015b, 0x0600u, 0x06u,  9U, 1, 1U, 12u, 0xf000u, 0x0000u
#define IO_ADC_XH018__NOCHOP \
	io0x15a, io_byte_0x15b, 0x015au, 0x015b, 0x0800u, 0x08u, 11U, 3, 1U, 12u, 0xf000u, 0x0000u
#define IO_ADC_XH018__SRC \
	io0x15c, io_byte_0x15c, 0x015cu, 0x015c, 0x003fu, 0x3fu,  0U, 0, 0U, 12u, 0xffffu, 0x0000u
#define IO_ADC_XH018__CORR_15_0 \
	io0x15e, error_IO_ADC_XH018__CORR_15_0_has_no_byte_access, 0x015eu, 0x015e, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_ADC_XH018__CORR_31_16 \
	io0x160, error_IO_ADC_XH018__CORR_31_16_has_no_byte_access, 0x0160u, 0x0160, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_ADC_XH018__CORR_46_32 \
	io0x162, error_IO_ADC_XH018__CORR_46_32_has_no_byte_access, 0x0162u, 0x0162, 0x7fffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x8000u
#define IO_ADC_XH018__START_CALIB \
	io0x162, io_byte_0x163, 0x0162u, 0x0163, 0x8000u, 0x80u, 15U, 7, 1U, 12u, 0x0000u, 0x8000u
#define IO_CUSTOM_91804__FLASH_TR0 \
	io0x164, error_IO_CUSTOM_91804__FLASH_TR0_has_no_byte_access, 0x0164u, 0x0164, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__FLASH_TR1 \
	io0x166, error_IO_CUSTOM_91804__FLASH_TR1_has_no_byte_access, 0x0166u, 0x0166, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__FLASH_TR2 \
	io0x168, error_IO_CUSTOM_91804__FLASH_TR2_has_no_byte_access, 0x0168u, 0x0168, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__FLASH_TR3 \
	io0x16a, error_IO_CUSTOM_91804__FLASH_TR3_has_no_byte_access, 0x016au, 0x016a, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__FLASH_TR4 \
	io0x16c, error_IO_CUSTOM_91804__FLASH_TR4_has_no_byte_access, 0x016cu, 0x016c, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__FLASH_TR5 \
	io0x16e, error_IO_CUSTOM_91804__FLASH_TR5_has_no_byte_access, 0x016eu, 0x016e, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE0 \
	io0x170, io_byte_0x170, 0x0170u, 0x0170, 0x00ffu, 0xffu,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE1 \
	io0x170, io_byte_0x171, 0x0170u, 0x0171, 0xff00u, 0xffu,  8U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE2 \
	io0x172, io_byte_0x172, 0x0172u, 0x0172, 0x00ffu, 0xffu,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE3 \
	io0x172, io_byte_0x173, 0x0172u, 0x0173, 0xff00u, 0xffu,  8U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE4 \
	io0x174, io_byte_0x174, 0x0174u, 0x0174, 0x00ffu, 0xffu,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE5 \
	io0x174, io_byte_0x175, 0x0174u, 0x0175, 0xff00u, 0xffu,  8U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE6 \
	io0x176, io_byte_0x176, 0x0176u, 0x0176, 0x00ffu, 0xffu,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE7 \
	io0x176, io_byte_0x177, 0x0176u, 0x0177, 0xff00u, 0xffu,  8U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE8 \
	io0x178, io_byte_0x178, 0x0178u, 0x0178, 0x00ffu, 0xffu,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE9 \
	io0x178, io_byte_0x179, 0x0178u, 0x0179, 0xff00u, 0xffu,  8U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE10 \
	io0x17a, io_byte_0x17a, 0x017au, 0x017a, 0x00ffu, 0xffu,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE11 \
	io0x17a, io_byte_0x17b, 0x017au, 0x017b, 0xff00u, 0xffu,  8U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE12 \
	io0x17c, io_byte_0x17c, 0x017cu, 0x017c, 0x00ffu, 0xffu,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE13 \
	io0x17c, io_byte_0x17d, 0x017cu, 0x017d, 0xff00u, 0xffu,  8U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE14 \
	io0x17e, io_byte_0x17e, 0x017eu, 0x017e, 0x00ffu, 0xffu,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE15 \
	io0x17e, io_byte_0x17f, 0x017eu, 0x017f, 0xff00u, 0xffu,  8U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE16 \
	io0x180, io_byte_0x180, 0x0180u, 0x0180, 0x00ffu, 0xffu,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE17 \
	io0x180, io_byte_0x181, 0x0180u, 0x0181, 0xff00u, 0xffu,  8U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE18 \
	io0x182, io_byte_0x182, 0x0182u, 0x0182, 0x00ffu, 0xffu,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE19 \
	io0x182, io_byte_0x183, 0x0182u, 0x0183, 0xff00u, 0xffu,  8U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE20 \
	io0x184, io_byte_0x184, 0x0184u, 0x0184, 0x00ffu, 0xffu,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE21 \
	io0x184, io_byte_0x185, 0x0184u, 0x0185, 0xff00u, 0xffu,  8U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE22 \
	io0x186, io_byte_0x186, 0x0186u, 0x0186, 0x00ffu, 0xffu,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE23 \
	io0x186, io_byte_0x187, 0x0186u, 0x0187, 0xff00u, 0xffu,  8U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE24 \
	io0x188, io_byte_0x188, 0x0188u, 0x0188, 0x00ffu, 0xffu,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE25 \
	io0x188, io_byte_0x189, 0x0188u, 0x0189, 0xff00u, 0xffu,  8U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE26 \
	io0x18a, io_byte_0x18a, 0x018au, 0x018a, 0x00ffu, 0xffu,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE27 \
	io0x18a, io_byte_0x18b, 0x018au, 0x018b, 0xff00u, 0xffu,  8U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE28 \
	io0x18c, io_byte_0x18c, 0x018cu, 0x018c, 0x00ffu, 0xffu,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE29 \
	io0x18c, io_byte_0x18d, 0x018cu, 0x018d, 0xff00u, 0xffu,  8U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE30 \
	io0x18e, io_byte_0x18e, 0x018eu, 0x018e, 0x00ffu, 0xffu,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE31 \
	io0x18e, io_byte_0x18f, 0x018eu, 0x018f, 0xff00u, 0xffu,  8U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE32 \
	io0x190, io_byte_0x190, 0x0190u, 0x0190, 0x00ffu, 0xffu,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE33 \
	io0x190, io_byte_0x191, 0x0190u, 0x0191, 0xff00u, 0xffu,  8U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE34 \
	io0x192, io_byte_0x192, 0x0192u, 0x0192, 0x00ffu, 0xffu,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE35 \
	io0x192, io_byte_0x193, 0x0192u, 0x0193, 0xff00u, 0xffu,  8U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE36 \
	io0x194, io_byte_0x194, 0x0194u, 0x0194, 0x00ffu, 0xffu,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE37 \
	io0x194, io_byte_0x195, 0x0194u, 0x0195, 0xff00u, 0xffu,  8U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE38 \
	io0x196, io_byte_0x196, 0x0196u, 0x0196, 0x00ffu, 0xffu,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE39 \
	io0x196, io_byte_0x197, 0x0196u, 0x0197, 0xff00u, 0xffu,  8U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE40 \
	io0x198, io_byte_0x198, 0x0198u, 0x0198, 0x00ffu, 0xffu,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE41 \
	io0x198, io_byte_0x199, 0x0198u, 0x0199, 0xff00u, 0xffu,  8U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE42 \
	io0x19a, io_byte_0x19a, 0x019au, 0x019a, 0x00ffu, 0xffu,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE43 \
	io0x19a, io_byte_0x19b, 0x019au, 0x019b, 0xff00u, 0xffu,  8U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE44 \
	io0x19c, io_byte_0x19c, 0x019cu, 0x019c, 0x00ffu, 0xffu,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE45 \
	io0x19c, io_byte_0x19d, 0x019cu, 0x019d, 0xff00u, 0xffu,  8U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE46 \
	io0x19e, io_byte_0x19e, 0x019eu, 0x019e, 0x00ffu, 0xffu,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__SCRATCHPAD_BYTE47 \
	io0x19e, io_byte_0x19f, 0x019eu, 0x019f, 0xff00u, 0xffu,  8U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__WUPTMR_MANT \
	io0x1a0, io_byte_0x1a0, 0x01a0u, 0x01a0, 0x00ffu, 0xffu,  0U, 0, 1U, 12u, 0x0000u, 0xf000u
#define IO_CUSTOM_91804__WUPTMR_EXP \
	io0x1a0, io_byte_0x1a1, 0x01a0u, 0x01a1, 0x0f00u, 0x0fu,  8U, 0, 1U, 12u, 0x0000u, 0xf000u
#define IO_CUSTOM_91804__WUPTMR_RESTART \
	io0x1a0, io_byte_0x1a1, 0x01a0u, 0x01a1, 0x1000u, 0x10u, 12U, 4, 1U, 12u, 0x0000u, 0xf000u
#define IO_CUSTOM_91804__WUPTMR_FLAG \
	io0x1a0, io_byte_0x1a1, 0x01a0u, 0x01a1, 0x2000u, 0x20u, 13U, 5, 1U, 12u, 0x0000u, 0xf000u
#define IO_CUSTOM_91804__WOKEN_UP \
	io0x1a0, io_byte_0x1a1, 0x01a0u, 0x01a1, 0x4000u, 0x40u, 14U, 6, 1U, 12u, 0x0000u, 0xf000u
#define IO_CUSTOM_91804__HAD_POR \
	io0x1a0, io_byte_0x1a1, 0x01a0u, 0x01a1, 0x8000u, 0x80u, 15U, 7, 1U, 12u, 0x0000u, 0xf000u
#define IO_CUSTOM_91804__HALT_ACTION \
	io0x1a2, io_byte_0x1a2, 0x01a2u, 0x01a2, 0x0003u, 0x03u,  0U, 0, 1U, 12u, 0x0000u, 0x00f0u
#define IO_CUSTOM_91804__LF_MODE \
	io0x1a2, io_byte_0x1a2, 0x01a2u, 0x01a2, 0x000cu, 0x0cu,  2U, 2, 1U, 12u, 0x0000u, 0x00f0u
#define IO_CUSTOM_91804__RFTX_MODE \
	io0x1a2, io_byte_0x1a2, 0x01a2u, 0x01a2, 0x00f0u, 0xf0u,  4U, 4, 1U, 12u, 0x0000u, 0x00f0u
#define IO_CUSTOM_91804__WAKE_RFTX_HALF \
	io0x1a2, io_byte_0x1a3, 0x01a2u, 0x01a3, 0x0100u, 0x01u,  8U, 0, 1U, 12u, 0x0000u, 0x00f0u
#define IO_CUSTOM_91804__WAKE_RFTX_DONE \
	io0x1a2, io_byte_0x1a3, 0x01a2u, 0x01a3, 0x0200u, 0x02u,  9U, 1, 1U, 12u, 0x0000u, 0x00f0u
#define IO_CUSTOM_91804__WAKE_LFTX_HALF \
	io0x1a2, io_byte_0x1a3, 0x01a2u, 0x01a3, 0x0400u, 0x04u, 10U, 2, 1U, 12u, 0x0000u, 0x00f0u
#define IO_CUSTOM_91804__WAKE_LFRX_SNIFF \
	io0x1a2, io_byte_0x1a3, 0x01a2u, 0x01a3, 0x0800u, 0x08u, 11U, 3, 1U, 12u, 0x0000u, 0x00f0u
#define IO_CUSTOM_91804__WAKE_LFRX_TIMEOUT \
	io0x1a2, io_byte_0x1a3, 0x01a2u, 0x01a3, 0x1000u, 0x10u, 12U, 4, 1U, 12u, 0x0000u, 0x00f0u
#define IO_CUSTOM_91804__WAKE_LFRX_SYNC \
	io0x1a2, io_byte_0x1a3, 0x01a2u, 0x01a3, 0x2000u, 0x20u, 13U, 5, 1U, 12u, 0x0000u, 0x00f0u
#define IO_CUSTOM_91804__WAKE_LFRX_DATA \
	io0x1a2, io_byte_0x1a3, 0x01a2u, 0x01a3, 0x4000u, 0x40u, 14U, 6, 1U, 12u, 0x0000u, 0x00f0u
#define IO_CUSTOM_91804__WAKE_LF_DONE \
	io0x1a2, io_byte_0x1a3, 0x01a2u, 0x01a3, 0x8000u, 0x80u, 15U, 7, 1U, 12u, 0x0000u, 0x00f0u
#define IO_CUSTOM_91804__POL_DIAG0 \
	io0x1a4, io_byte_0x1a4, 0x01a4u, 0x01a4, 0x0007u, 0x07u,  0U, 0, 1U, 12u, 0x8888u, 0x0000u
#define IO_CUSTOM_91804__POL_DIAG1 \
	io0x1a4, io_byte_0x1a4, 0x01a4u, 0x01a4, 0x0070u, 0x70u,  4U, 4, 1U, 12u, 0x8888u, 0x0000u
#define IO_CUSTOM_91804__POL_DIAG2 \
	io0x1a4, io_byte_0x1a5, 0x01a4u, 0x01a5, 0x0700u, 0x07u,  8U, 0, 1U, 12u, 0x8888u, 0x0000u
#define IO_CUSTOM_91804__POL_DIAG3 \
	io0x1a4, io_byte_0x1a5, 0x01a4u, 0x01a5, 0x7000u, 0x70u, 12U, 4, 1U, 12u, 0x8888u, 0x0000u
#define IO_CUSTOM_91804__TEMPSENS_MODE \
	io0x1a6, io_byte_0x1a6, 0x01a6u, 0x01a6, 0x0007u, 0x07u,  0U, 0, 1U, 12u, 0x00f8u, 0x0000u
#define IO_CUSTOM_91804__TEMPSENS_LT_FLAG \
	io0x1a6, io_byte_0x1a6, 0x01a6u, 0x01a6, 0x0040u, 0x40u,  6U, 6, 0U, 12u, 0x00f8u, 0x0000u
#define IO_CUSTOM_91804__TEMPSENS_HT_FLAG \
	io0x1a6, io_byte_0x1a6, 0x01a6u, 0x01a6, 0x0080u, 0x80u,  7U, 7, 0U, 12u, 0x00f8u, 0x0000u
#define IO_CUSTOM_91804__POL_GPIO0 \
	io0x1a6, io_byte_0x1a7, 0x01a6u, 0x01a7, 0x0100u, 0x01u,  8U, 0, 1U, 12u, 0x00f8u, 0x0000u
#define IO_CUSTOM_91804__POL_GPIO1 \
	io0x1a6, io_byte_0x1a7, 0x01a6u, 0x01a7, 0x0200u, 0x02u,  9U, 1, 1U, 12u, 0x00f8u, 0x0000u
#define IO_CUSTOM_91804__POL_GPIO2 \
	io0x1a6, io_byte_0x1a7, 0x01a6u, 0x01a7, 0x0400u, 0x04u, 10U, 2, 1U, 12u, 0x00f8u, 0x0000u
#define IO_CUSTOM_91804__POL_GPIO3 \
	io0x1a6, io_byte_0x1a7, 0x01a6u, 0x01a7, 0x0800u, 0x08u, 11U, 3, 1U, 12u, 0x00f8u, 0x0000u
#define IO_CUSTOM_91804__WAKE_GPIO0 \
	io0x1a6, io_byte_0x1a7, 0x01a6u, 0x01a7, 0x1000u, 0x10u, 12U, 4, 1U, 12u, 0x00f8u, 0x0000u
#define IO_CUSTOM_91804__WAKE_GPIO1 \
	io0x1a6, io_byte_0x1a7, 0x01a6u, 0x01a7, 0x2000u, 0x20u, 13U, 5, 1U, 12u, 0x00f8u, 0x0000u
#define IO_CUSTOM_91804__WAKE_GPIO2 \
	io0x1a6, io_byte_0x1a7, 0x01a6u, 0x01a7, 0x4000u, 0x40u, 14U, 6, 1U, 12u, 0x00f8u, 0x0000u
#define IO_CUSTOM_91804__WAKE_GPIO3 \
	io0x1a6, io_byte_0x1a7, 0x01a6u, 0x01a7, 0x8000u, 0x80u, 15U, 7, 1U, 12u, 0x00f8u, 0x0000u
#define IO_CUSTOM_91804__GPIO0 \
	io0x1a8, io_byte_0x1a8, 0x01a8u, 0x01a8, 0x0001u, 0x01u,  0U, 0, 0U, 12u, 0x00ffu, 0x0000u
#define IO_CUSTOM_91804__GPIO1 \
	io0x1a8, io_byte_0x1a8, 0x01a8u, 0x01a8, 0x0002u, 0x02u,  1U, 1, 0U, 12u, 0x00ffu, 0x0000u
#define IO_CUSTOM_91804__GPIO2 \
	io0x1a8, io_byte_0x1a8, 0x01a8u, 0x01a8, 0x0004u, 0x04u,  2U, 2, 0U, 12u, 0x00ffu, 0x0000u
#define IO_CUSTOM_91804__GPIO3 \
	io0x1a8, io_byte_0x1a8, 0x01a8u, 0x01a8, 0x0008u, 0x08u,  3U, 3, 0U, 12u, 0x00ffu, 0x0000u
#define IO_CUSTOM_91804__TDI \
	io0x1a8, io_byte_0x1a8, 0x01a8u, 0x01a8, 0x0010u, 0x10u,  4U, 4, 0U, 12u, 0x00ffu, 0x0000u
#define IO_CUSTOM_91804__TCK \
	io0x1a8, io_byte_0x1a8, 0x01a8u, 0x01a8, 0x0020u, 0x20u,  5U, 5, 0U, 12u, 0x00ffu, 0x0000u
#define IO_CUSTOM_91804__GPIO0_DRV \
	io0x1a8, io_byte_0x1a9, 0x01a8u, 0x01a9, 0x0300u, 0x03u,  8U, 0, 1U, 12u, 0x00ffu, 0x0000u
#define IO_CUSTOM_91804__GPIO1_DRV \
	io0x1a8, io_byte_0x1a9, 0x01a8u, 0x01a9, 0x0c00u, 0x0cu, 10U, 2, 1U, 12u, 0x00ffu, 0x0000u
#define IO_CUSTOM_91804__GPIO2_DRV \
	io0x1a8, io_byte_0x1a9, 0x01a8u, 0x01a9, 0x3000u, 0x30u, 12U, 4, 1U, 12u, 0x00ffu, 0x0000u
#define IO_CUSTOM_91804__GPIO3_DRV \
	io0x1a8, io_byte_0x1a9, 0x01a8u, 0x01a9, 0xc000u, 0xc0u, 14U, 6, 1U, 12u, 0x00ffu, 0x0000u
#define IO_CUSTOM_91804__GPIO0_AMUX \
	io0x1aa, io_byte_0x1aa, 0x01aau, 0x01aa, 0x0007u, 0x07u,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__EN_AIO0_PD \
	io0x1aa, io_byte_0x1aa, 0x01aau, 0x01aa, 0x0008u, 0x08u,  3U, 3, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__GPIO1_AMUX \
	io0x1aa, io_byte_0x1aa, 0x01aau, 0x01aa, 0x0070u, 0x70u,  4U, 4, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__EN_AIO1_PD \
	io0x1aa, io_byte_0x1aa, 0x01aau, 0x01aa, 0x0080u, 0x80u,  7U, 7, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__GPIO2_AMUX \
	io0x1aa, io_byte_0x1ab, 0x01aau, 0x01ab, 0x0700u, 0x07u,  8U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__EN_AIO2_PD \
	io0x1aa, io_byte_0x1ab, 0x01aau, 0x01ab, 0x0800u, 0x08u, 11U, 3, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__GPIO3_AMUX \
	io0x1aa, io_byte_0x1ab, 0x01aau, 0x01ab, 0x7000u, 0x70u, 12U, 4, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__EN_AIO3_PD \
	io0x1aa, io_byte_0x1ab, 0x01aau, 0x01ab, 0x8000u, 0x80u, 15U, 7, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__GPIO0_CH_SEL \
	io0x1ac, io_byte_0x1ac, 0x01acu, 0x01ac, 0x00ffu, 0xffu,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__GPIO1_CH_SEL \
	io0x1ac, io_byte_0x1ad, 0x01acu, 0x01ad, 0xff00u, 0xffu,  8U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__GPIO2_CH_SEL \
	io0x1ae, io_byte_0x1ae, 0x01aeu, 0x01ae, 0x00ffu, 0xffu,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__GPIO3_CH_SEL \
	io0x1ae, io_byte_0x1af, 0x01aeu, 0x01af, 0xff00u, 0xffu,  8U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__DIAG0_LOW \
	io0x1b0, io_byte_0x1b0, 0x01b0u, 0x01b0, 0x0001u, 0x01u,  0U, 0, 0U, 12u, 0xf8ffu, 0x0000u
#define IO_CUSTOM_91804__DIAG0_HIGH \
	io0x1b0, io_byte_0x1b0, 0x01b0u, 0x01b0, 0x0002u, 0x02u,  1U, 1, 0U, 12u, 0xf8ffu, 0x0000u
#define IO_CUSTOM_91804__DIAG1_LOW \
	io0x1b0, io_byte_0x1b0, 0x01b0u, 0x01b0, 0x0004u, 0x04u,  2U, 2, 0U, 12u, 0xf8ffu, 0x0000u
#define IO_CUSTOM_91804__DIAG1_HIGH \
	io0x1b0, io_byte_0x1b0, 0x01b0u, 0x01b0, 0x0008u, 0x08u,  3U, 3, 0U, 12u, 0xf8ffu, 0x0000u
#define IO_CUSTOM_91804__DIAG2_LOW \
	io0x1b0, io_byte_0x1b0, 0x01b0u, 0x01b0, 0x0010u, 0x10u,  4U, 4, 0U, 12u, 0xf8ffu, 0x0000u
#define IO_CUSTOM_91804__DIAG2_HIGH \
	io0x1b0, io_byte_0x1b0, 0x01b0u, 0x01b0, 0x0020u, 0x20u,  5U, 5, 0U, 12u, 0xf8ffu, 0x0000u
#define IO_CUSTOM_91804__DIAG3_LOW \
	io0x1b0, io_byte_0x1b0, 0x01b0u, 0x01b0, 0x0040u, 0x40u,  6U, 6, 0U, 12u, 0xf8ffu, 0x0000u
#define IO_CUSTOM_91804__DIAG3_HIGH \
	io0x1b0, io_byte_0x1b0, 0x01b0u, 0x01b0, 0x0080u, 0x80u,  7U, 7, 0U, 12u, 0xf8ffu, 0x0000u
#define IO_CUSTOM_91804__DIAG_REF_SEL \
	io0x1b0, io_byte_0x1b1, 0x01b0u, 0x01b1, 0x0700u, 0x07u,  8U, 0, 1U, 12u, 0xf8ffu, 0x0000u
#define IO_CUSTOM_91804__DIAG0_CH_SEL \
	io0x1b2, io_byte_0x1b2, 0x01b2u, 0x01b2, 0x000fu, 0x0fu,  0U, 0, 1U, 12u, 0x3030u, 0x0000u
#define IO_CUSTOM_91804__DIAG0_HYST \
	io0x1b2, io_byte_0x1b2, 0x01b2u, 0x01b2, 0x00c0u, 0xc0u,  6U, 6, 1U, 12u, 0x3030u, 0x0000u
#define IO_CUSTOM_91804__DIAG1_CH_SEL \
	io0x1b2, io_byte_0x1b3, 0x01b2u, 0x01b3, 0x0f00u, 0x0fu,  8U, 0, 1U, 12u, 0x3030u, 0x0000u
#define IO_CUSTOM_91804__DIAG1_HYST \
	io0x1b2, io_byte_0x1b3, 0x01b2u, 0x01b3, 0xc000u, 0xc0u, 14U, 6, 1U, 12u, 0x3030u, 0x0000u
#define IO_CUSTOM_91804__DIAG2_CH_SEL \
	io0x1b4, io_byte_0x1b4, 0x01b4u, 0x01b4, 0x000fu, 0x0fu,  0U, 0, 1U, 12u, 0x3030u, 0x0000u
#define IO_CUSTOM_91804__DIAG2_HYST \
	io0x1b4, io_byte_0x1b4, 0x01b4u, 0x01b4, 0x00c0u, 0xc0u,  6U, 6, 1U, 12u, 0x3030u, 0x0000u
#define IO_CUSTOM_91804__DIAG3_CH_SEL \
	io0x1b4, io_byte_0x1b5, 0x01b4u, 0x01b5, 0x0f00u, 0x0fu,  8U, 0, 1U, 12u, 0x3030u, 0x0000u
#define IO_CUSTOM_91804__DIAG3_HYST \
	io0x1b4, io_byte_0x1b5, 0x01b4u, 0x01b5, 0xc000u, 0xc0u, 14U, 6, 1U, 12u, 0x3030u, 0x0000u
#define IO_CUSTOM_91804__DIAG0_VREFL \
	io0x1b6, io_byte_0x1b6, 0x01b6u, 0x01b6, 0x007fu, 0x7fu,  0U, 0, 1U, 12u, 0x8080u, 0x0000u
#define IO_CUSTOM_91804__DIAG0_VREFH \
	io0x1b6, io_byte_0x1b7, 0x01b6u, 0x01b7, 0x7f00u, 0x7fu,  8U, 0, 1U, 12u, 0x8080u, 0x0000u
#define IO_CUSTOM_91804__DIAG1_VREFL \
	io0x1b8, io_byte_0x1b8, 0x01b8u, 0x01b8, 0x007fu, 0x7fu,  0U, 0, 1U, 12u, 0x8080u, 0x0000u
#define IO_CUSTOM_91804__DIAG1_VREFH \
	io0x1b8, io_byte_0x1b9, 0x01b8u, 0x01b9, 0x7f00u, 0x7fu,  8U, 0, 1U, 12u, 0x8080u, 0x0000u
#define IO_CUSTOM_91804__DIAG2_VREFL \
	io0x1ba, io_byte_0x1ba, 0x01bau, 0x01ba, 0x007fu, 0x7fu,  0U, 0, 1U, 12u, 0x8080u, 0x0000u
#define IO_CUSTOM_91804__DIAG2_VREFH \
	io0x1ba, io_byte_0x1bb, 0x01bau, 0x01bb, 0x7f00u, 0x7fu,  8U, 0, 1U, 12u, 0x8080u, 0x0000u
#define IO_CUSTOM_91804__DIAG3_VREFL \
	io0x1bc, io_byte_0x1bc, 0x01bcu, 0x01bc, 0x007fu, 0x7fu,  0U, 0, 1U, 12u, 0x8080u, 0x0000u
#define IO_CUSTOM_91804__DIAG3_VREFH \
	io0x1bc, io_byte_0x1bd, 0x01bcu, 0x01bd, 0x7f00u, 0x7fu,  8U, 0, 1U, 12u, 0x8080u, 0x0000u
#define IO_CUSTOM_91804__FREQCNT_SRC \
	io0x1be, io_byte_0x1be, 0x01beu, 0x01be, 0x0007u, 0x07u,  0U, 0, 1U, 12u, 0x0008u, 0x00c0u
#define IO_CUSTOM_91804__FREQCNT_TBASE \
	io0x1be, io_byte_0x1be, 0x01beu, 0x01be, 0x0030u, 0x30u,  4U, 4, 1U, 12u, 0x0008u, 0x00c0u
#define IO_CUSTOM_91804__FREQCNT_STATE \
	io0x1be, io_byte_0x1be, 0x01beu, 0x01be, 0x00c0u, 0xc0u,  6U, 6, 1U, 12u, 0x0008u, 0x00c0u
#define IO_CUSTOM_91804__FREQCNT_MANT \
	io0x1be, io_byte_0x1bf, 0x01beu, 0x01bf, 0x0f00u, 0x0fu,  8U, 0, 1U, 12u, 0x0008u, 0x00c0u
#define IO_CUSTOM_91804__FREQCNT_EXP \
	io0x1be, io_byte_0x1bf, 0x01beu, 0x01bf, 0xf000u, 0xf0u, 12U, 4, 1U, 12u, 0x0008u, 0x00c0u
#define IO_CUSTOM_91804__FREQCNT_OUT \
	io0x1c0, error_IO_CUSTOM_91804__FREQCNT_OUT_has_no_byte_access, 0x01c0u, 0x01c0, 0xffffu, 0xffu,  0U, 0, 0U, 8u, 0xffffu, 0x0000u
#define IO_CUSTOM_91804__DTB \
	io0x1c2, io_byte_0x1c2, 0x01c2u, 0x01c2, 0x000fu, 0x0fu,  0U, 0, 0U, 12u, 0xffffu, 0x0000u
#define IO_CUSTOM_91804__DCDC_VDDA3L_COUT \
	io0x1c2, io_byte_0x1c2, 0x01c2u, 0x01c2, 0x0010u, 0x10u,  4U, 4, 0U, 12u, 0xffffu, 0x0000u
#define IO_CUSTOM_91804__DCDC_VDDA3H_COUT \
	io0x1c2, io_byte_0x1c2, 0x01c2u, 0x01c2, 0x0020u, 0x20u,  5U, 5, 0U, 12u, 0xffffu, 0x0000u
#define IO_CUSTOM_91804__DCDC_VX_COUT \
	io0x1c2, io_byte_0x1c2, 0x01c2u, 0x01c2, 0x0040u, 0x40u,  6U, 6, 0U, 12u, 0xffffu, 0x0000u
#define IO_CUSTOM_91804__DCDC_VNVM_COUT \
	io0x1c2, io_byte_0x1c2, 0x01c2u, 0x01c2, 0x0080u, 0x80u,  7U, 7, 0U, 12u, 0xffffu, 0x0000u
#define IO_CUSTOM_91804__DCDC_STATUS \
	io0x1c2, io_byte_0x1c3, 0x01c2u, 0x01c3, 0x0300u, 0x03u,  8U, 0, 0U, 12u, 0xffffu, 0x0000u
#define IO_CUSTOM_91804__VDIG_LV_B_DB \
	io0x1c2, io_byte_0x1c3, 0x01c2u, 0x01c3, 0x0400u, 0x04u, 10U, 2, 0U, 12u, 0xffffu, 0x0000u
#define IO_CUSTOM_91804__VANA_LV_B_DB \
	io0x1c2, io_byte_0x1c3, 0x01c2u, 0x01c3, 0x0800u, 0x08u, 11U, 3, 0U, 12u, 0xffffu, 0x0000u
#define IO_CUSTOM_91804__VPA_LV_B_DB \
	io0x1c2, io_byte_0x1c3, 0x01c2u, 0x01c3, 0x1000u, 0x10u, 12U, 4, 0U, 12u, 0xffffu, 0x0000u
#define IO_CUSTOM_91804__LF_FD_B \
	io0x1c2, io_byte_0x1c3, 0x01c2u, 0x01c3, 0x2000u, 0x20u, 13U, 5, 0U, 12u, 0xffffu, 0x0000u
#define IO_CUSTOM_91804__LF_DATA \
	io0x1c2, io_byte_0x1c3, 0x01c2u, 0x01c3, 0x4000u, 0x40u, 14U, 6, 0U, 12u, 0xffffu, 0x0000u
#define IO_CUSTOM_91804__LF_CLK \
	io0x1c2, io_byte_0x1c3, 0x01c2u, 0x01c3, 0x8000u, 0x80u, 15U, 7, 0U, 12u, 0xffffu, 0x0000u
#define IO_CUSTOM_91804__RFTX_PATTERN_LEN \
	io0x1c4, io_byte_0x1c4, 0x01c4u, 0x01c4, 0x0003u, 0x03u,  0U, 0, 1U, 12u, 0x2f04u, 0x1000u
#define IO_CUSTOM_91804__RFTX_MULTI_FRAME \
	io0x1c4, io_byte_0x1c4, 0x01c4u, 0x01c4, 0x0008u, 0x08u,  3U, 3, 1U, 12u, 0x2f04u, 0x1000u
#define IO_CUSTOM_91804__RFTX_DIRECT_MOD \
	io0x1c4, io_byte_0x1c4, 0x01c4u, 0x01c4, 0x0070u, 0x70u,  4U, 4, 1U, 12u, 0x2f04u, 0x1000u
#define IO_CUSTOM_91804__RF_EN_PD \
	io0x1c4, io_byte_0x1c4, 0x01c4u, 0x01c4, 0x0080u, 0x80u,  7U, 7, 1U, 12u, 0x2f04u, 0x1000u
#define IO_CUSTOM_91804__RFTX_INFO \
	io0x1c4, io_byte_0x1c5, 0x01c4u, 0x01c5, 0x0700u, 0x07u,  8U, 0, 0U, 12u, 0x2f04u, 0x1000u
#define IO_CUSTOM_91804__PLL_LOCKED \
	io0x1c4, io_byte_0x1c5, 0x01c4u, 0x01c5, 0x0800u, 0x08u, 11U, 3, 0U, 12u, 0x2f04u, 0x1000u
#define IO_CUSTOM_91804__CYCLE_SLIP \
	io0x1c4, io_byte_0x1c5, 0x01c4u, 0x01c5, 0x1000u, 0x10u, 12U, 4, 1U, 12u, 0x2f04u, 0x1000u
#define IO_CUSTOM_91804__PLL_XTAL_RUN \
	io0x1c4, io_byte_0x1c5, 0x01c4u, 0x01c5, 0x2000u, 0x20u, 13U, 5, 0U, 12u, 0x2f04u, 0x1000u
#define IO_CUSTOM_91804__RFTX_WAIT_LOCK \
	io0x1c4, io_byte_0x1c5, 0x01c4u, 0x01c5, 0xc000u, 0xc0u, 14U, 6, 1U, 12u, 0x2f04u, 0x1000u
#define IO_CUSTOM_91804__RFTX_PREAMBLE \
	io0x1c6, io_byte_0x1c6, 0x01c6u, 0x01c6, 0x00ffu, 0xffu,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__RFTX_PREAMBLE_LEN \
	io0x1c6, io_byte_0x1c7, 0x01c6u, 0x01c7, 0xff00u, 0xffu,  8U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__RFTX_PATTERN_L \
	io0x1c8, error_IO_CUSTOM_91804__RFTX_PATTERN_L_has_no_byte_access, 0x01c8u, 0x01c8, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__RFTX_PATTERN_H \
	io0x1ca, error_IO_CUSTOM_91804__RFTX_PATTERN_H_has_no_byte_access, 0x01cau, 0x01ca, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__RFTX_PACKET_LEN \
	io0x1cc, io_byte_0x1cc, 0x01ccu, 0x01cc, 0x00ffu, 0xffu,  0U, 0, 1U, 12u, 0x4000u, 0x0000u
#define IO_CUSTOM_91804__RFTX_PACKET_LEN_CORR \
	io0x1cc, io_byte_0x1cd, 0x01ccu, 0x01cd, 0x0f00u, 0x0fu,  8U, 0, 1U, 12u, 0x4000u, 0x0000u
#define IO_CUSTOM_91804__RFTX_PACKET_LEN_POS \
	io0x1cc, io_byte_0x1cd, 0x01ccu, 0x01cd, 0x3000u, 0x30u, 12U, 4, 1U, 12u, 0x4000u, 0x0000u
#define IO_CUSTOM_91804__RFTX_PACKET_LEN_FIX \
	io0x1cc, io_byte_0x1cd, 0x01ccu, 0x01cd, 0x8000u, 0x80u, 15U, 7, 1U, 12u, 0x4000u, 0x0000u
#define IO_CUSTOM_91804__RFTX_ADDRESS \
	io0x1ce, io_byte_0x1ce, 0x01ceu, 0x01ce, 0x00ffu, 0xffu,  0U, 0, 1U, 12u, 0xf000u, 0x0000u
#define IO_CUSTOM_91804__RFTX_EN_ADDRESS \
	io0x1ce, io_byte_0x1cf, 0x01ceu, 0x01cf, 0x0100u, 0x01u,  8U, 0, 1U, 12u, 0xf000u, 0x0000u
#define IO_CUSTOM_91804__RFTX_CRC_INV \
	io0x1ce, io_byte_0x1cf, 0x01ceu, 0x01cf, 0x0200u, 0x02u,  9U, 1, 1U, 12u, 0xf000u, 0x0000u
#define IO_CUSTOM_91804__RFTX_CRC_16_N8 \
	io0x1ce, io_byte_0x1cf, 0x01ceu, 0x01cf, 0x0400u, 0x04u, 10U, 2, 1U, 12u, 0xf000u, 0x0000u
#define IO_CUSTOM_91804__RFTX_EN_CRC \
	io0x1ce, io_byte_0x1cf, 0x01ceu, 0x01cf, 0x0800u, 0x08u, 11U, 3, 1U, 12u, 0xf000u, 0x0000u
#define IO_CUSTOM_91804__RFTX_CRC_INIT \
	io0x1d0, error_IO_CUSTOM_91804__RFTX_CRC_INIT_has_no_byte_access, 0x01d0u, 0x01d0, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__RFTX_DR_MANT \
	io0x1d2, io_byte_0x1d2, 0x01d2u, 0x01d2, 0x00ffu, 0xffu,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__RFTX_DR_EXP \
	io0x1d2, io_byte_0x1d3, 0x01d2u, 0x01d3, 0x0700u, 0x07u,  8U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__RFTX_LSB_FIRST \
	io0x1d2, io_byte_0x1d3, 0x01d2u, 0x01d3, 0x0800u, 0x08u, 11U, 3, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__RFTX_EN_MANCHESTER \
	io0x1d2, io_byte_0x1d3, 0x01d2u, 0x01d3, 0x1000u, 0x10u, 12U, 4, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__RFTX_RAMP \
	io0x1d2, io_byte_0x1d3, 0x01d2u, 0x01d3, 0xe000u, 0xe0u, 13U, 5, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__RFTX_MANCHESTER_STOP_WORD \
	io0x1d4, io_byte_0x1d4, 0x01d4u, 0x01d4, 0x00ffu, 0xffu,  0U, 0, 1U, 12u, 0xfc00u, 0x0000u
#define IO_CUSTOM_91804__RFTX_MANCHESTER_STOP_LEN \
	io0x1d4, io_byte_0x1d5, 0x01d4u, 0x01d5, 0x0300u, 0x03u,  8U, 0, 1U, 12u, 0xfc00u, 0x0000u
#define IO_CUSTOM_91804__RFTX_MULT_MANT \
	io0x1d6, io_byte_0x1d6, 0x01d6u, 0x01d6, 0x000fu, 0x0fu,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__RFTX_MULT_EXP \
	io0x1d6, io_byte_0x1d6, 0x01d6u, 0x01d6, 0x00f0u, 0xf0u,  4U, 4, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__RFTX_BIT_INVERT \
	io0x1d6, io_byte_0x1d7, 0x01d6u, 0x01d7, 0x0100u, 0x01u,  8U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__RFTX_PN9_MODE \
	io0x1d6, io_byte_0x1d7, 0x01d6u, 0x01d7, 0x0200u, 0x02u,  9U, 1, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__RFTX_PN9_REVERSE \
	io0x1d6, io_byte_0x1d7, 0x01d6u, 0x01d7, 0x0400u, 0x04u, 10U, 2, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__RFTX_EN_DATAWHITE \
	io0x1d6, io_byte_0x1d7, 0x01d6u, 0x01d7, 0x0800u, 0x08u, 11U, 3, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__RFTX_EN_INTERP \
	io0x1d6, io_byte_0x1d7, 0x01d6u, 0x01d7, 0x3000u, 0x30u, 12U, 4, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__RFTX_EN_GAUSSIAN \
	io0x1d6, io_byte_0x1d7, 0x01d6u, 0x01d7, 0x4000u, 0x40u, 14U, 6, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__RFTX_FSK_NOOK \
	io0x1d6, io_byte_0x1d7, 0x01d6u, 0x01d7, 0x8000u, 0x80u, 15U, 7, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__RFTX_CENTER_FREQ_L \
	io0x1d8, error_IO_CUSTOM_91804__RFTX_CENTER_FREQ_L_has_no_byte_access, 0x01d8u, 0x01d8, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__RFTX_CENTER_FREQ_H \
	io0x1da, io_byte_0x1da, 0x01dau, 0x01da, 0x00ffu, 0xffu,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__RFTX_POWER \
	io0x1da, io_byte_0x1db, 0x01dau, 0x01db, 0x7f00u, 0x7fu,  8U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__RFTX_BYPASS_VPA \
	io0x1da, io_byte_0x1db, 0x01dau, 0x01db, 0x8000u, 0x80u, 15U, 7, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__RFTX_FIFO_DATA \
	io0x1dc, io_byte_0x1dc, 0x01dcu, 0x01dc, 0x00ffu, 0xffu,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__RFTX_FIFO_COUNT \
	io0x1dc, io_byte_0x1dd, 0x01dcu, 0x01dd, 0x3f00u, 0x3fu,  8U, 0, 0U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__RFTX_FIFO_FLUSH \
	io0x1dc, io_byte_0x1dd, 0x01dcu, 0x01dd, 0xff00u, 0xffu,  8U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__RFTX_FIFO_FULL \
	io0x1dc, io_byte_0x1dd, 0x01dcu, 0x01dd, 0x2000u, 0x20u, 13U, 5, 0U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__RFTX_FIFO_OVUND \
	io0x1dc, io_byte_0x1dd, 0x01dcu, 0x01dd, 0x8000u, 0x80u, 15U, 7, 0U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__LFTX_FIFO_DATA \
	io0x1de, io_byte_0x1de, 0x01deu, 0x01de, 0x00ffu, 0xffu,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__LF_DATABUF \
	io0x1de, io_byte_0x1de, 0x01deu, 0x01de, 0x00ffu, 0xffu,  0U, 0, 0U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__LFTX_FIFO_COUNT \
	io0x1de, io_byte_0x1df, 0x01deu, 0x01df, 0x0f00u, 0x0fu,  8U, 0, 0U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__LFTX_FIFO_FLUSH \
	io0x1de, io_byte_0x1df, 0x01deu, 0x01df, 0xff00u, 0xffu,  8U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__LFTX_FIFO_FULL \
	io0x1de, io_byte_0x1df, 0x01deu, 0x01df, 0x0800u, 0x08u, 11U, 3, 0U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__LFTX_FIFO_OVUND \
	io0x1de, io_byte_0x1df, 0x01deu, 0x01df, 0x8000u, 0x80u, 15U, 7, 0U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__LFTX_POL \
	io0x1e0, io_byte_0x1e0, 0x01e0u, 0x01e0, 0x0001u, 0x01u,  0U, 0, 1U, 12u, 0xff70u, 0x0000u
#define IO_CUSTOM_91804__LFTX_MODE \
	io0x1e0, io_byte_0x1e0, 0x01e0u, 0x01e0, 0x000eu, 0x0eu,  1U, 1, 1U, 12u, 0xff70u, 0x0000u
#define IO_CUSTOM_91804__LF_EN_FD \
	io0x1e0, io_byte_0x1e0, 0x01e0u, 0x01e0, 0x0080u, 0x80u,  7U, 7, 1U, 12u, 0xff70u, 0x0000u
#define IO_CUSTOM_91804__LFRX_STATE \
	io0x1e0, io_byte_0x1e1, 0x01e0u, 0x01e1, 0x0700u, 0x07u,  8U, 0, 0U, 12u, 0xff70u, 0x0000u
#define IO_CUSTOM_91804__LFRX_FIFO_DATA \
	io0x1e2, io_byte_0x1e2, 0x01e2u, 0x01e2, 0x00ffu, 0xffu,  0U, 0, 1U, 12u, 0x0000u, 0x00ffu
#define IO_CUSTOM_91804__LFRX_FIFO_COUNT \
	io0x1e2, io_byte_0x1e3, 0x01e2u, 0x01e3, 0x0f00u, 0x0fu,  8U, 0, 0U, 12u, 0x0000u, 0x00ffu
#define IO_CUSTOM_91804__LFRX_FIFO_FLUSH \
	io0x1e2, io_byte_0x1e3, 0x01e2u, 0x01e3, 0xff00u, 0xffu,  8U, 0, 1U, 12u, 0x0000u, 0x00ffu
#define IO_CUSTOM_91804__LFRX_FIFO_FULL \
	io0x1e2, io_byte_0x1e3, 0x01e2u, 0x01e3, 0x0800u, 0x08u, 11U, 3, 0U, 12u, 0x0000u, 0x00ffu
#define IO_CUSTOM_91804__LFRX_FIFO_OVUND \
	io0x1e2, io_byte_0x1e3, 0x01e2u, 0x01e3, 0x8000u, 0x80u, 15U, 7, 0U, 12u, 0x0000u, 0x00ffu
#define IO_CUSTOM_91804__LFRX_TIMEOUT_MANT \
	io0x1e4, io_byte_0x1e4, 0x01e4u, 0x01e4, 0x000fu, 0x0fu,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__LFRX_TIMEOUT_EXP \
	io0x1e4, io_byte_0x1e4, 0x01e4u, 0x01e4, 0x00f0u, 0xf0u,  4U, 4, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__LFRX_STBY_MANT \
	io0x1e4, io_byte_0x1e5, 0x01e4u, 0x01e5, 0x0f00u, 0x0fu,  8U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__LFRX_STBY_EXP \
	io0x1e4, io_byte_0x1e5, 0x01e4u, 0x01e5, 0xf000u, 0xf0u, 12U, 4, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__LFRX_STBY_ON \
	io0x1e6, io_byte_0x1e6, 0x01e6u, 0x01e6, 0x0001u, 0x01u,  0U, 0, 1U, 12u, 0x004au, 0x0000u
#define IO_CUSTOM_91804__LFRX_PREAMBLE \
	io0x1e6, io_byte_0x1e6, 0x01e6u, 0x01e6, 0x0004u, 0x04u,  2U, 2, 1U, 12u, 0x004au, 0x0000u
#define IO_CUSTOM_91804__LF_GAIN_SEL \
	io0x1e6, io_byte_0x1e6, 0x01e6u, 0x01e6, 0x0030u, 0x30u,  4U, 4, 1U, 12u, 0x004au, 0x0000u
#define IO_CUSTOM_91804__LFRX_ORDER \
	io0x1e6, io_byte_0x1e6, 0x01e6u, 0x01e6, 0x0080u, 0x80u,  7U, 7, 1U, 12u, 0x004au, 0x0000u
#define IO_CUSTOM_91804__LFRX_HDRLEN \
	io0x1e6, io_byte_0x1e7, 0x01e6u, 0x01e7, 0x0300u, 0x03u,  8U, 0, 1U, 12u, 0x004au, 0x0000u
#define IO_CUSTOM_91804__LFRX_POL \
	io0x1e6, io_byte_0x1e7, 0x01e6u, 0x01e7, 0x0400u, 0x04u, 10U, 2, 1U, 12u, 0x004au, 0x0000u
#define IO_CUSTOM_91804__LFRX_SYNCPOL \
	io0x1e6, io_byte_0x1e7, 0x01e6u, 0x01e7, 0x0800u, 0x08u, 11U, 3, 1U, 12u, 0x004au, 0x0000u
#define IO_CUSTOM_91804__LFRX_SYNCLEN \
	io0x1e6, io_byte_0x1e7, 0x01e6u, 0x01e7, 0xf000u, 0xf0u, 12U, 4, 1U, 12u, 0x004au, 0x0000u
#define IO_CUSTOM_91804__LFRX_SYNC_DIGIT1 \
	io0x1e8, io_byte_0x1e8, 0x01e8u, 0x01e8, 0x000cu, 0x0cu,  2U, 2, 1U, 12u, 0x0003u, 0x0000u
#define IO_CUSTOM_91804__LFRX_SYNC_DIGIT2 \
	io0x1e8, io_byte_0x1e8, 0x01e8u, 0x01e8, 0x0030u, 0x30u,  4U, 4, 1U, 12u, 0x0003u, 0x0000u
#define IO_CUSTOM_91804__LFRX_SYNC_DIGIT3 \
	io0x1e8, io_byte_0x1e8, 0x01e8u, 0x01e8, 0x00c0u, 0xc0u,  6U, 6, 1U, 12u, 0x0003u, 0x0000u
#define IO_CUSTOM_91804__LFRX_SYNC_DIGIT4 \
	io0x1e8, io_byte_0x1e9, 0x01e8u, 0x01e9, 0x0300u, 0x03u,  8U, 0, 1U, 12u, 0x0003u, 0x0000u
#define IO_CUSTOM_91804__LFRX_SYNC_DIGIT5 \
	io0x1e8, io_byte_0x1e9, 0x01e8u, 0x01e9, 0x0c00u, 0x0cu, 10U, 2, 1U, 12u, 0x0003u, 0x0000u
#define IO_CUSTOM_91804__LFRX_SYNC_DIGIT6 \
	io0x1e8, io_byte_0x1e9, 0x01e8u, 0x01e9, 0x3000u, 0x30u, 12U, 4, 1U, 12u, 0x0003u, 0x0000u
#define IO_CUSTOM_91804__LFRX_SYNC_DIGIT7 \
	io0x1e8, io_byte_0x1e9, 0x01e8u, 0x01e9, 0xc000u, 0xc0u, 14U, 6, 1U, 12u, 0x0003u, 0x0000u
#define IO_CUSTOM_91804__LFRX_SYNC_DIGIT8 \
	io0x1ea, io_byte_0x1ea, 0x01eau, 0x01ea, 0x0003u, 0x03u,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__LFRX_SYNC_DIGIT9 \
	io0x1ea, io_byte_0x1ea, 0x01eau, 0x01ea, 0x000cu, 0x0cu,  2U, 2, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__LFRX_SYNC_DIGIT10 \
	io0x1ea, io_byte_0x1ea, 0x01eau, 0x01ea, 0x0030u, 0x30u,  4U, 4, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__LFRX_SYNC_DIGIT11 \
	io0x1ea, io_byte_0x1ea, 0x01eau, 0x01ea, 0x00c0u, 0xc0u,  6U, 6, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__LFRX_SYNC_DIGIT12 \
	io0x1ea, io_byte_0x1eb, 0x01eau, 0x01eb, 0x0300u, 0x03u,  8U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__LFRX_SYNC_DIGIT13 \
	io0x1ea, io_byte_0x1eb, 0x01eau, 0x01eb, 0x0c00u, 0x0cu, 10U, 2, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__LFRX_SYNC_DIGIT14 \
	io0x1ea, io_byte_0x1eb, 0x01eau, 0x01eb, 0x3000u, 0x30u, 12U, 4, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__LFRX_SYNC_DIGIT15 \
	io0x1ea, io_byte_0x1eb, 0x01eau, 0x01eb, 0xc000u, 0xc0u, 14U, 6, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__LFRX_MINCNT \
	io0x1ec, io_byte_0x1ec, 0x01ecu, 0x01ec, 0x003fu, 0x3fu,  0U, 0, 1U, 12u, 0x40c0u, 0x0000u
#define IO_CUSTOM_91804__LFRX_DELTACNT \
	io0x1ec, io_byte_0x1ed, 0x01ecu, 0x01ed, 0x3f00u, 0x3fu,  8U, 0, 1U, 12u, 0x40c0u, 0x0000u
#define IO_CUSTOM_91804__LFRX_LONGCNT \
	io0x1ec, io_byte_0x1ed, 0x01ecu, 0x01ed, 0x8000u, 0x80u, 15U, 7, 1U, 12u, 0x40c0u, 0x0000u
#define IO_CUSTOM_91804__LFRX_HDR_L \
	io0x1ee, error_IO_CUSTOM_91804__LFRX_HDR_L_has_no_byte_access, 0x01eeu, 0x01ee, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__LFRX_HDR_H \
	io0x1f0, error_IO_CUSTOM_91804__LFRX_HDR_H_has_no_byte_access, 0x01f0u, 0x01f0, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__LFRX_HDR_MASK_L \
	io0x1f2, error_IO_CUSTOM_91804__LFRX_HDR_MASK_L_has_no_byte_access, 0x01f2u, 0x01f2, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__LFRX_HDR_MASK_H \
	io0x1f4, error_IO_CUSTOM_91804__LFRX_HDR_MASK_H_has_no_byte_access, 0x01f4u, 0x01f4, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__LFRX_RXD_HDR_L \
	io0x1f6, error_IO_CUSTOM_91804__LFRX_RXD_HDR_L_has_no_byte_access, 0x01f6u, 0x01f6, 0xffffu, 0xffu,  0U, 0, 0U, 8u, 0xffffu, 0x0000u
#define IO_CUSTOM_91804__LFRX_RXD_HDR_H \
	io0x1f8, error_IO_CUSTOM_91804__LFRX_RXD_HDR_H_has_no_byte_access, 0x01f8u, 0x01f8, 0xffffu, 0xffu,  0U, 0, 0U, 8u, 0xffffu, 0x0000u
#define IO_CUSTOM_91804__ACCEL_SEL \
	io0x1fa, io_byte_0x1fa, 0x01fau, 0x01fa, 0x0003u, 0x03u,  0U, 0, 1U, 12u, 0xc800u, 0x0000u
#define IO_CUSTOM_91804__ACCEL_MEAS \
	io0x1fa, io_byte_0x1fa, 0x01fau, 0x01fa, 0x000cu, 0x0cu,  2U, 2, 1U, 12u, 0xc800u, 0x0000u
#define IO_CUSTOM_91804__ACCEL_SPARE \
	io0x1fa, io_byte_0x1fa, 0x01fau, 0x01fa, 0x00f0u, 0xf0u,  4U, 4, 1U, 12u, 0xc800u, 0x0000u
#define IO_CUSTOM_91804__ACCEL_POL \
	io0x1fa, io_byte_0x1fb, 0x01fau, 0x01fb, 0x0100u, 0x01u,  8U, 0, 1U, 12u, 0xc800u, 0x0000u
#define IO_CUSTOM_91804__ACCEL_PREBPOL \
	io0x1fa, io_byte_0x1fb, 0x01fau, 0x01fb, 0x0200u, 0x02u,  9U, 1, 1U, 12u, 0xc800u, 0x0000u
#define IO_CUSTOM_91804__ACCEL_PREB \
	io0x1fa, io_byte_0x1fb, 0x01fau, 0x01fb, 0x0400u, 0x04u, 10U, 2, 1U, 12u, 0xc800u, 0x0000u
#define IO_CUSTOM_91804__ACCEL_DIG_RSTB \
	io0x1fa, io_byte_0x1fb, 0x01fau, 0x01fb, 0x1000u, 0x10u, 12U, 4, 1U, 12u, 0xc800u, 0x0000u
#define IO_CUSTOM_91804__ACCEL_EN \
	io0x1fa, io_byte_0x1fb, 0x01fau, 0x01fb, 0x2000u, 0x20u, 13U, 5, 1U, 12u, 0xc800u, 0x0000u
#define IO_CUSTOM_91804__ACCEL_RDY \
	io0x1fa, io_byte_0x1fb, 0x01fau, 0x01fb, 0x8000u, 0x80u, 15U, 7, 0U, 12u, 0xc800u, 0x0000u
#define IO_CUSTOM_91804__SENS_CH_SEL_B \
	io0x1fc, io_byte_0x1fc, 0x01fcu, 0x01fc, 0x000fu, 0x0fu,  0U, 0, 1U, 12u, 0xf0f0u, 0x0000u
#define IO_CUSTOM_91804__SENS_CH_SEL_A \
	io0x1fc, io_byte_0x1fd, 0x01fcu, 0x01fd, 0x0f00u, 0x0fu,  8U, 0, 1U, 12u, 0xf0f0u, 0x0000u
#define IO_CUSTOM_91804__SENS_PGA_GAIN \
	io0x1fe, io_byte_0x1fe, 0x01feu, 0x01fe, 0x000fu, 0x0fu,  0U, 0, 1U, 12u, 0x0030u, 0x0000u
#define IO_CUSTOM_91804__SENS_EN_PGA \
	io0x1fe, io_byte_0x1fe, 0x01feu, 0x01fe, 0x0040u, 0x40u,  6U, 6, 1U, 12u, 0x0030u, 0x0000u
#define IO_CUSTOM_91804__SENS_EN_BRIDGE \
	io0x1fe, io_byte_0x1fe, 0x01feu, 0x01fe, 0x0080u, 0x80u,  7U, 7, 1U, 12u, 0x0030u, 0x0000u
#define IO_CUSTOM_91804__SENS_REFL_SEL \
	io0x1fe, io_byte_0x1ff, 0x01feu, 0x01ff, 0x0700u, 0x07u,  8U, 0, 1U, 12u, 0x0030u, 0x0000u
#define IO_CUSTOM_91804__SENS_EN_EXTREF \
	io0x1fe, io_byte_0x1ff, 0x01feu, 0x01ff, 0x0800u, 0x08u, 11U, 3, 1U, 12u, 0x0030u, 0x0000u
#define IO_CUSTOM_91804__SENS_REFH_SEL \
	io0x1fe, io_byte_0x1ff, 0x01feu, 0x01ff, 0x7000u, 0x70u, 12U, 4, 1U, 12u, 0x0030u, 0x0000u
#define IO_CUSTOM_91804__SENS_CH_POL \
	io0x1fe, io_byte_0x1ff, 0x01feu, 0x01ff, 0x8000u, 0x80u, 15U, 7, 1U, 12u, 0x0030u, 0x0000u
#define IO_CUSTOM_91804__ADC_SPEED \
	io0x200, io_byte_0x200, 0x0200u, 0x0200, 0x0003u, 0x03u,  0U, 0, 1U, 12u, 0xfefcu, 0x0000u
#define IO_CUSTOM_91804__ADC_INTREF \
	io0x200, io_byte_0x201, 0x0200u, 0x0201, 0x0100u, 0x01u,  8U, 0, 1U, 12u, 0xfefcu, 0x0000u
#define IO_CUSTOM_91804__ADC_TESTINTP \
	io0x202, io_byte_0x202, 0x0202u, 0x0202, 0x0001u, 0x01u,  0U, 0, 1U, 12u, 0xf080u, 0x0000u
#define IO_CUSTOM_91804__ADC_TESTINTN \
	io0x202, io_byte_0x202, 0x0202u, 0x0202, 0x0002u, 0x02u,  1U, 1, 1U, 12u, 0xf080u, 0x0000u
#define IO_CUSTOM_91804__ADC_TESTCOMP \
	io0x202, io_byte_0x202, 0x0202u, 0x0202, 0x0004u, 0x04u,  2U, 2, 1U, 12u, 0xf080u, 0x0000u
#define IO_CUSTOM_91804__ADC_TESTANA \
	io0x202, io_byte_0x202, 0x0202u, 0x0202, 0x0008u, 0x08u,  3U, 3, 1U, 12u, 0xf080u, 0x0000u
#define IO_CUSTOM_91804__ADC_TEST_COMPNODELAY \
	io0x202, io_byte_0x202, 0x0202u, 0x0202, 0x0010u, 0x10u,  4U, 4, 1U, 12u, 0xf080u, 0x0000u
#define IO_CUSTOM_91804__ADC_EN_TESTOV \
	io0x202, io_byte_0x202, 0x0202u, 0x0202, 0x0020u, 0x20u,  5U, 5, 1U, 12u, 0xf080u, 0x0000u
#define IO_CUSTOM_91804__ADC_FRC_RDY \
	io0x202, io_byte_0x202, 0x0202u, 0x0202, 0x0040u, 0x40u,  6U, 6, 1U, 12u, 0xf080u, 0x0000u
#define IO_CUSTOM_91804__SENS_EN_PU_SN \
	io0x202, io_byte_0x203, 0x0202u, 0x0203, 0x0100u, 0x01u,  8U, 0, 1U, 12u, 0xf080u, 0x0000u
#define IO_CUSTOM_91804__SENS_EN_PD_SN \
	io0x202, io_byte_0x203, 0x0202u, 0x0203, 0x0200u, 0x02u,  9U, 1, 1U, 12u, 0xf080u, 0x0000u
#define IO_CUSTOM_91804__SENS_EN_PU_SP \
	io0x202, io_byte_0x203, 0x0202u, 0x0203, 0x0400u, 0x04u, 10U, 2, 1U, 12u, 0xf080u, 0x0000u
#define IO_CUSTOM_91804__SENS_EN_PD_SP \
	io0x202, io_byte_0x203, 0x0202u, 0x0203, 0x0800u, 0x08u, 11U, 3, 1U, 12u, 0xf080u, 0x0000u
#define IO_CUSTOM_91804__ACCEL_CCA \
	io0x204, io_byte_0x204, 0x0204u, 0x0204, 0x000fu, 0x0fu,  0U, 0, 1U, 12u, 0xf0f0u, 0x0000u
#define IO_CUSTOM_91804__ACCEL_CCB \
	io0x204, io_byte_0x205, 0x0204u, 0x0205, 0x0f00u, 0x0fu,  8U, 0, 1U, 12u, 0xf0f0u, 0x0000u
#define IO_CUSTOM_91804__DTB_SEL \
	io0x206, io_byte_0x206, 0x0206u, 0x0206, 0x0007u, 0x07u,  0U, 0, 1U, 12u, 0xe078u, 0x0000u
#define IO_CUSTOM_91804__DISCONNECT_TDO \
	io0x206, io_byte_0x206, 0x0206u, 0x0206, 0x0080u, 0x80u,  7U, 7, 1U, 12u, 0xe078u, 0x0000u
#define IO_CUSTOM_91804__ATB_SEL \
	io0x206, io_byte_0x207, 0x0206u, 0x0207, 0x1f00u, 0x1fu,  8U, 0, 1U, 12u, 0xe078u, 0x0000u
#define IO_CUSTOM_91804__FRC_EN_IBG_1V7 \
	io0x208, io_byte_0x208, 0x0208u, 0x0208, 0x00ffu, 0xffu,  0U, 0, 1U, 12u, 0x8000u, 0x0000u
#define IO_CUSTOM_91804__FRC_EN_IBG_3V \
	io0x208, io_byte_0x209, 0x0208u, 0x0209, 0x7f00u, 0x7fu,  8U, 0, 1U, 12u, 0x8000u, 0x0000u
#define IO_CUSTOM_91804__FRC_EN_IBGH_1V7 \
	io0x20a, io_byte_0x20a, 0x020au, 0x020a, 0x000fu, 0x0fu,  0U, 0, 1U, 12u, 0x6000u, 0x0000u
#define IO_CUSTOM_91804__FRC_EN_IPTAT_1V7 \
	io0x20a, io_byte_0x20a, 0x020au, 0x020a, 0x00f0u, 0xf0u,  4U, 4, 1U, 12u, 0x6000u, 0x0000u
#define IO_CUSTOM_91804__FRC_EN_IPTAT_3V \
	io0x20a, io_byte_0x20b, 0x020au, 0x020b, 0x1f00u, 0x1fu,  8U, 0, 1U, 12u, 0x6000u, 0x0000u
#define IO_CUSTOM_91804__FRC_EN_PTAT \
	io0x20a, io_byte_0x20b, 0x020au, 0x020b, 0x8000u, 0x80u, 15U, 7, 1U, 12u, 0x6000u, 0x0000u
#define IO_CUSTOM_91804__FRC_EN_ISPTAT_1V7 \
	io0x20c, io_byte_0x20c, 0x020cu, 0x020c, 0x001fu, 0x1fu,  0U, 0, 1U, 12u, 0xe060u, 0x0000u
#define IO_CUSTOM_91804__FRC_EN_SPTAT \
	io0x20c, io_byte_0x20c, 0x020cu, 0x020c, 0x0080u, 0x80u,  7U, 7, 1U, 12u, 0xe060u, 0x0000u
#define IO_CUSTOM_91804__FRC_EN_IBG_P_OUT \
	io0x20c, io_byte_0x20d, 0x020cu, 0x020d, 0x1f00u, 0x1fu,  8U, 0, 1U, 12u, 0xe060u, 0x0000u
#define IO_CUSTOM_91804__FRC_EN_VBG_OUT \
	io0x20e, io_byte_0x20e, 0x020eu, 0x020e, 0x000fu, 0x0fu,  0U, 0, 1U, 12u, 0xf030u, 0x0000u
#define IO_CUSTOM_91804__FRC_EN_BG \
	io0x20e, io_byte_0x20e, 0x020eu, 0x020e, 0x00c0u, 0xc0u,  6U, 6, 1U, 12u, 0xf030u, 0x0000u
#define IO_CUSTOM_91804__VBG_TRIM \
	io0x20e, io_byte_0x20f, 0x020eu, 0x020f, 0x0f00u, 0x0fu,  8U, 0, 1U, 12u, 0xf030u, 0x0000u
#define IO_CUSTOM_91804__IBG0_1V7 \
	io0x210, io_byte_0x210, 0x0210u, 0x0210, 0x000fu, 0x0fu,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__IBG1_1V7 \
	io0x210, io_byte_0x210, 0x0210u, 0x0210, 0x00f0u, 0xf0u,  4U, 4, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__IBG2_1V7 \
	io0x210, io_byte_0x211, 0x0210u, 0x0211, 0x0f00u, 0x0fu,  8U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__IBG3_1V7 \
	io0x210, io_byte_0x211, 0x0210u, 0x0211, 0xf000u, 0xf0u, 12U, 4, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__IBG4_1V7 \
	io0x212, io_byte_0x212, 0x0212u, 0x0212, 0x000fu, 0x0fu,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__IBG5_1V7 \
	io0x212, io_byte_0x212, 0x0212u, 0x0212, 0x00f0u, 0xf0u,  4U, 4, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__IBG6_1V7 \
	io0x212, io_byte_0x213, 0x0212u, 0x0213, 0x0f00u, 0x0fu,  8U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__IBG7_1V7 \
	io0x212, io_byte_0x213, 0x0212u, 0x0213, 0xf000u, 0xf0u, 12U, 4, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__IBG0_3V \
	io0x214, io_byte_0x214, 0x0214u, 0x0214, 0x000fu, 0x0fu,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__IBG1_3V \
	io0x214, io_byte_0x214, 0x0214u, 0x0214, 0x00f0u, 0xf0u,  4U, 4, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__IBG2_3V \
	io0x214, io_byte_0x215, 0x0214u, 0x0215, 0x0f00u, 0x0fu,  8U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__IBG3_3V \
	io0x214, io_byte_0x215, 0x0214u, 0x0215, 0xf000u, 0xf0u, 12U, 4, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__IBG4_3V \
	io0x216, io_byte_0x216, 0x0216u, 0x0216, 0x000fu, 0x0fu,  0U, 0, 1U, 12u, 0xf000u, 0x0000u
#define IO_CUSTOM_91804__IBG5_3V \
	io0x216, io_byte_0x216, 0x0216u, 0x0216, 0x00f0u, 0xf0u,  4U, 4, 1U, 12u, 0xf000u, 0x0000u
#define IO_CUSTOM_91804__IBG6_3V \
	io0x216, io_byte_0x217, 0x0216u, 0x0217, 0x0f00u, 0x0fu,  8U, 0, 1U, 12u, 0xf000u, 0x0000u
#define IO_CUSTOM_91804__IBGH0_1V7 \
	io0x218, io_byte_0x218, 0x0218u, 0x0218, 0x000fu, 0x0fu,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__IBGH1_1V7 \
	io0x218, io_byte_0x218, 0x0218u, 0x0218, 0x00f0u, 0xf0u,  4U, 4, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__IBGH2_1V7 \
	io0x218, io_byte_0x219, 0x0218u, 0x0219, 0x0f00u, 0x0fu,  8U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__IBGH3_1V7 \
	io0x218, io_byte_0x219, 0x0218u, 0x0219, 0xf000u, 0xf0u, 12U, 4, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__IPTAT0_1V7 \
	io0x21a, io_byte_0x21a, 0x021au, 0x021a, 0x000fu, 0x0fu,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__IPTAT1_1V7 \
	io0x21a, io_byte_0x21a, 0x021au, 0x021a, 0x00f0u, 0xf0u,  4U, 4, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__IPTAT2_1V7 \
	io0x21a, io_byte_0x21b, 0x021au, 0x021b, 0x0f00u, 0x0fu,  8U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__IPTAT3_1V7 \
	io0x21a, io_byte_0x21b, 0x021au, 0x021b, 0xf000u, 0xf0u, 12U, 4, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__IPTAT0_3V \
	io0x21c, io_byte_0x21c, 0x021cu, 0x021c, 0x000fu, 0x0fu,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__IPTAT1_3V \
	io0x21c, io_byte_0x21c, 0x021cu, 0x021c, 0x00f0u, 0xf0u,  4U, 4, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__IPTAT2_3V \
	io0x21c, io_byte_0x21d, 0x021cu, 0x021d, 0x0f00u, 0x0fu,  8U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__IPTAT3_3V \
	io0x21c, io_byte_0x21d, 0x021cu, 0x021d, 0xf000u, 0xf0u, 12U, 4, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__IPTAT4_3V \
	io0x21e, io_byte_0x21e, 0x021eu, 0x021e, 0x000fu, 0x0fu,  0U, 0, 1U, 12u, 0xcff0u, 0x0000u
#define IO_CUSTOM_91804__IPTAT_TRIM \
	io0x21e, io_byte_0x21f, 0x021eu, 0x021f, 0x3000u, 0x30u, 12U, 4, 1U, 12u, 0xcff0u, 0x0000u
#define IO_CUSTOM_91804__ISPTAT0_1V7 \
	io0x220, io_byte_0x220, 0x0220u, 0x0220, 0x000fu, 0x0fu,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__ISPTAT1_1V7 \
	io0x220, io_byte_0x220, 0x0220u, 0x0220, 0x00f0u, 0xf0u,  4U, 4, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__ISPTAT2_1V7 \
	io0x220, io_byte_0x221, 0x0220u, 0x0221, 0x0f00u, 0x0fu,  8U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__ISPTAT3_1V7 \
	io0x220, io_byte_0x221, 0x0220u, 0x0221, 0xf000u, 0xf0u, 12U, 4, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__ISPTAT4_1V7 \
	io0x222, io_byte_0x222, 0x0222u, 0x0222, 0x000fu, 0x0fu,  0U, 0, 1U, 12u, 0x8ff0u, 0x0000u
#define IO_CUSTOM_91804__TEMPSENS_TRIM \
	io0x222, io_byte_0x223, 0x0222u, 0x0223, 0x7000u, 0x70u, 12U, 4, 1U, 12u, 0x8ff0u, 0x0000u
#define IO_CUSTOM_91804__VDIG_TRIM \
	io0x224, io_byte_0x224, 0x0224u, 0x0224, 0x0007u, 0x07u,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__VDIG_SHORT \
	io0x224, io_byte_0x224, 0x0224u, 0x0224, 0x0008u, 0x08u,  3U, 3, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__VDIG_EN_IPK_LIM \
	io0x224, io_byte_0x224, 0x0224u, 0x0224, 0x0010u, 0x10u,  4U, 4, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__FRC_VDIG_DIS \
	io0x224, io_byte_0x224, 0x0224u, 0x0224, 0x0020u, 0x20u,  5U, 5, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__FRC_VDIG_DIS_LVD \
	io0x224, io_byte_0x224, 0x0224u, 0x0224, 0x0040u, 0x40u,  6U, 6, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__FRC_VDIG_RDY \
	io0x224, io_byte_0x224, 0x0224u, 0x0224, 0x0080u, 0x80u,  7U, 7, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__VANA_TRIM \
	io0x224, io_byte_0x225, 0x0224u, 0x0225, 0x0300u, 0x03u,  8U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__LVD_ABORT_RFTX \
	io0x224, io_byte_0x225, 0x0224u, 0x0225, 0x0400u, 0x04u, 10U, 2, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__VANA_SHORT \
	io0x224, io_byte_0x225, 0x0224u, 0x0225, 0x0800u, 0x08u, 11U, 3, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__VANA_EN_IPK_LIM \
	io0x224, io_byte_0x225, 0x0224u, 0x0225, 0x1000u, 0x10u, 12U, 4, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__FRC_VANA_EN \
	io0x224, io_byte_0x225, 0x0224u, 0x0225, 0x2000u, 0x20u, 13U, 5, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__FRC_VANA_EN_LVD \
	io0x224, io_byte_0x225, 0x0224u, 0x0225, 0x4000u, 0x40u, 14U, 6, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__FRC_VANA_RDY \
	io0x224, io_byte_0x225, 0x0224u, 0x0225, 0x8000u, 0x80u, 15U, 7, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__VPA_TRIM \
	io0x226, io_byte_0x226, 0x0226u, 0x0226, 0x0003u, 0x03u,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__FRC_VPA_BYPASS \
	io0x226, io_byte_0x226, 0x0226u, 0x0226, 0x0004u, 0x04u,  2U, 2, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__VPA_SHORT \
	io0x226, io_byte_0x226, 0x0226u, 0x0226, 0x0008u, 0x08u,  3U, 3, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__VPA_EN_IPK_LIM \
	io0x226, io_byte_0x226, 0x0226u, 0x0226, 0x0010u, 0x10u,  4U, 4, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__FRC_VPA_EN \
	io0x226, io_byte_0x226, 0x0226u, 0x0226, 0x0020u, 0x20u,  5U, 5, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__FRC_VPA_EN_LVD \
	io0x226, io_byte_0x226, 0x0226u, 0x0226, 0x0040u, 0x40u,  6U, 6, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__FRC_VPA_RDY \
	io0x226, io_byte_0x226, 0x0226u, 0x0226, 0x0080u, 0x80u,  7U, 7, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__PLL_VCOREG_TRIM \
	io0x226, io_byte_0x227, 0x0226u, 0x0227, 0x3f00u, 0x3fu,  8U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__FRC_PLL_SHORT_NOISE_FILTER \
	io0x226, io_byte_0x227, 0x0226u, 0x0227, 0xc000u, 0xc0u, 14U, 6, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__PLL_N_DIV_XTAL_TEST \
	io0x228, io_byte_0x228, 0x0228u, 0x0228, 0x003fu, 0x3fu,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__PLL_EN_CLK_XTAL_TEST \
	io0x228, io_byte_0x228, 0x0228u, 0x0228, 0x0040u, 0x40u,  6U, 6, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__PLL_XTAL_BUFFER_ALWAYS_ON \
	io0x228, io_byte_0x228, 0x0228u, 0x0228, 0x0080u, 0x80u,  7U, 7, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__PLL_XTAL_STARTUP_CURR \
	io0x228, io_byte_0x229, 0x0228u, 0x0229, 0x0300u, 0x03u,  8U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__PLL_XTAL_AFTERSTARTUP_CURR \
	io0x228, io_byte_0x229, 0x0228u, 0x0229, 0x0c00u, 0x0cu, 10U, 2, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__PLL_EN_XTAL_INT_CAP \
	io0x228, io_byte_0x229, 0x0228u, 0x0229, 0x3000u, 0x30u, 12U, 4, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__FRC_PLL_EN_CLK_XTAL_DIG \
	io0x228, io_byte_0x229, 0x0228u, 0x0229, 0x4000u, 0x40u, 14U, 6, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__FRC_EN_XTAL \
	io0x228, io_byte_0x229, 0x0228u, 0x0229, 0x8000u, 0x80u, 15U, 7, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__FRC_RF_OOK_MOD \
	io0x22a, io_byte_0x22a, 0x022au, 0x022a, 0x00ffu, 0xffu,  0U, 0, 1U, 12u, 0x3000u, 0x0000u
#define IO_CUSTOM_91804__RF_FC_SEL \
	io0x22a, io_byte_0x22b, 0x022au, 0x022b, 0x0f00u, 0x0fu,  8U, 0, 1U, 12u, 0x3000u, 0x0000u
#define IO_CUSTOM_91804__FRC_RF_EN_MOD_CLK \
	io0x22a, io_byte_0x22b, 0x022au, 0x022b, 0x4000u, 0x40u, 14U, 6, 1U, 12u, 0x3000u, 0x0000u
#define IO_CUSTOM_91804__FRC_RF_EN_OUT \
	io0x22a, io_byte_0x22b, 0x022au, 0x022b, 0x8000u, 0x80u, 15U, 7, 1U, 12u, 0x3000u, 0x0000u
#define IO_CUSTOM_91804__RF_EN_PPA_BIAS \
	io0x22c, io_byte_0x22c, 0x022cu, 0x022c, 0x0001u, 0x01u,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__RF_EN_PA_OS \
	io0x22c, io_byte_0x22c, 0x022cu, 0x022c, 0x0002u, 0x02u,  1U, 1, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__RF_EN_PA_BIAS \
	io0x22c, io_byte_0x22c, 0x022cu, 0x022c, 0x0004u, 0x04u,  2U, 2, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__FRC_RF_EN_PA \
	io0x22c, io_byte_0x22c, 0x022cu, 0x022c, 0x0008u, 0x08u,  3U, 3, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__FRC_PLL_EN_VCOBUF_PRESC \
	io0x22c, io_byte_0x22c, 0x022cu, 0x022c, 0x0010u, 0x10u,  4U, 4, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__PLL_EN_VCOBUF_TEST \
	io0x22c, io_byte_0x22c, 0x022cu, 0x022c, 0x0020u, 0x20u,  5U, 5, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__FRC_PLL_EN_VCOBUF_PA1 \
	io0x22c, io_byte_0x22c, 0x022cu, 0x022c, 0x0040u, 0x40u,  6U, 6, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__FRC_PLL_EN_VCOBUF_PA2 \
	io0x22c, io_byte_0x22c, 0x022cu, 0x022c, 0x0080u, 0x80u,  7U, 7, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__FRC_PLL_EN_VCO \
	io0x22c, io_byte_0x22d, 0x022cu, 0x022d, 0x0100u, 0x01u,  8U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__FRC_PLL_EN_VCOREG \
	io0x22c, io_byte_0x22d, 0x022cu, 0x022d, 0x0200u, 0x02u,  9U, 1, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__DIS_CSLIP \
	io0x22c, io_byte_0x22d, 0x022cu, 0x022d, 0x0400u, 0x04u, 10U, 2, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__FRC_PLL_EN_DIV_PRESC \
	io0x22c, io_byte_0x22d, 0x022cu, 0x022d, 0x0800u, 0x08u, 11U, 3, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__FRC_PLL_EN_PFD_CP \
	io0x22c, io_byte_0x22d, 0x022cu, 0x022d, 0x1000u, 0x10u, 12U, 4, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__PLL_EN_LF_C2 \
	io0x22c, io_byte_0x22d, 0x022cu, 0x022d, 0x2000u, 0x20u, 13U, 5, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__PLL_EN_CLK_EXT \
	io0x22c, io_byte_0x22d, 0x022cu, 0x022d, 0x4000u, 0x40u, 14U, 6, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__PLL_EN_DIV_TEST \
	io0x22c, io_byte_0x22d, 0x022cu, 0x022d, 0x8000u, 0x80u, 15U, 7, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__PLL_N_DIV_TEST \
	io0x22e, io_byte_0x22e, 0x022eu, 0x022e, 0x00ffu, 0xffu,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__PLL_FORCE_VCTR \
	io0x22e, io_byte_0x22f, 0x022eu, 0x022f, 0x1f00u, 0x1fu,  8U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__PLL_EN_FRC_VCTR \
	io0x22e, io_byte_0x22f, 0x022eu, 0x022f, 0x2000u, 0x20u, 13U, 5, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__PLL_EN_CURR_OFFS \
	io0x22e, io_byte_0x22f, 0x022eu, 0x022f, 0xc000u, 0xc0u, 14U, 6, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__MASH_ORDER \
	io0x230, io_byte_0x230, 0x0230u, 0x0230, 0x0003u, 0x03u,  0U, 0, 1U, 12u, 0xfc0cu, 0x0000u
#define IO_CUSTOM_91804__MASH_DITHER \
	io0x230, io_byte_0x230, 0x0230u, 0x0230, 0x0010u, 0x10u,  4U, 4, 1U, 12u, 0xfc0cu, 0x0000u
#define IO_CUSTOM_91804__FRC_MASH_EN \
	io0x230, io_byte_0x230, 0x0230u, 0x0230, 0x0020u, 0x20u,  5U, 5, 1U, 12u, 0xfc0cu, 0x0000u
#define IO_CUSTOM_91804__RFTX_CENTER_FREQ_U \
	io0x230, io_byte_0x230, 0x0230u, 0x0230, 0x0040u, 0x40u,  6U, 6, 1U, 12u, 0xfc0cu, 0x0000u
#define IO_CUSTOM_91804__PLL_CLK_PFD_POL \
	io0x230, io_byte_0x230, 0x0230u, 0x0230, 0x0080u, 0x80u,  7U, 7, 1U, 12u, 0xfc0cu, 0x0000u
#define IO_CUSTOM_91804__HFO_CLK_X_SEL \
	io0x230, io_byte_0x231, 0x0230u, 0x0231, 0x0300u, 0x03u,  8U, 0, 1U, 12u, 0xfc0cu, 0x0000u
#define IO_CUSTOM_91804__HFO_FREQ_TRIM \
	io0x232, io_byte_0x232, 0x0232u, 0x0232, 0x007fu, 0x7fu,  0U, 0, 1U, 12u, 0x8080u, 0x0000u
#define IO_CUSTOM_91804__LFO_FREQ_TRIM \
	io0x232, io_byte_0x233, 0x0232u, 0x0233, 0x7f00u, 0x7fu,  8U, 0, 1U, 12u, 0x8080u, 0x0000u
#define IO_CUSTOM_91804__FRC_LFO_EN_OUT_32K \
	io0x234, io_byte_0x234, 0x0234u, 0x0234, 0x0001u, 0x01u,  0U, 0, 1U, 12u, 0x0a3eu, 0x0000u
#define IO_CUSTOM_91804__LFO_FRC_ULP_MODE \
	io0x234, io_byte_0x234, 0x0234u, 0x0234, 0x0040u, 0x40u,  6U, 6, 1U, 12u, 0x0a3eu, 0x0000u
#define IO_CUSTOM_91804__LFO_EN_ULP_MODE \
	io0x234, io_byte_0x234, 0x0234u, 0x0234, 0x0080u, 0x80u,  7U, 7, 1U, 12u, 0x0a3eu, 0x0000u
#define IO_CUSTOM_91804__FRC_LF_MOD \
	io0x234, io_byte_0x235, 0x0234u, 0x0235, 0x0100u, 0x01u,  8U, 0, 1U, 12u, 0x0a3eu, 0x0000u
#define IO_CUSTOM_91804__LF_EN_QDAMP_B \
	io0x234, io_byte_0x235, 0x0234u, 0x0235, 0x0400u, 0x04u, 10U, 2, 1U, 12u, 0x0a3eu, 0x0000u
#define IO_CUSTOM_91804__FRC_LF_EN_CLK \
	io0x234, io_byte_0x235, 0x0234u, 0x0235, 0x1000u, 0x10u, 12U, 4, 1U, 12u, 0x0a3eu, 0x0000u
#define IO_CUSTOM_91804__FRC_LF_EN_LSTN \
	io0x234, io_byte_0x235, 0x0234u, 0x0235, 0x2000u, 0x20u, 13U, 5, 1U, 12u, 0x0a3eu, 0x0000u
#define IO_CUSTOM_91804__FRC_LF_EN_HDR \
	io0x234, io_byte_0x235, 0x0234u, 0x0235, 0x4000u, 0x40u, 14U, 6, 1U, 12u, 0x0a3eu, 0x0000u
#define IO_CUSTOM_91804__FRC_LF_EN \
	io0x234, io_byte_0x235, 0x0234u, 0x0235, 0x8000u, 0x80u, 15U, 7, 1U, 12u, 0x0a3eu, 0x0000u
#define IO_CUSTOM_91804__DCDC_FFINE \
	io0x236, io_byte_0x236, 0x0236u, 0x0236, 0x001fu, 0x1fu,  0U, 0, 1U, 12u, 0x00e0u, 0x0000u
#define IO_CUSTOM_91804__DCDC_1X5_FROUGH \
	io0x236, io_byte_0x237, 0x0236u, 0x0237, 0x0300u, 0x03u,  8U, 0, 1U, 12u, 0x00e0u, 0x0000u
#define IO_CUSTOM_91804__DCDC_2X_FROUGH \
	io0x236, io_byte_0x237, 0x0236u, 0x0237, 0x0c00u, 0x0cu, 10U, 2, 1U, 12u, 0x00e0u, 0x0000u
#define IO_CUSTOM_91804__DCDC_REG_TRIM \
	io0x236, io_byte_0x237, 0x0236u, 0x0237, 0xf000u, 0xf0u, 12U, 4, 1U, 12u, 0x00e0u, 0x0000u
#define IO_CUSTOM_91804__DCDC_VX_VTH \
	io0x238, io_byte_0x238, 0x0238u, 0x0238, 0x000fu, 0x0fu,  0U, 0, 1U, 12u, 0x1010u, 0x0000u
#define IO_CUSTOM_91804__DCDC_VX_HYST \
	io0x238, io_byte_0x238, 0x0238u, 0x0238, 0x00e0u, 0xe0u,  5U, 5, 1U, 12u, 0x1010u, 0x0000u
#define IO_CUSTOM_91804__DCDC_VNVM_VTH \
	io0x238, io_byte_0x239, 0x0238u, 0x0239, 0x0f00u, 0x0fu,  8U, 0, 1U, 12u, 0x1010u, 0x0000u
#define IO_CUSTOM_91804__DCDC_VNVM_HYST \
	io0x238, io_byte_0x239, 0x0238u, 0x0239, 0xe000u, 0xe0u, 13U, 5, 1U, 12u, 0x1010u, 0x0000u
#define IO_CUSTOM_91804__DCDC_VDDA3L_VTH \
	io0x23a, io_byte_0x23a, 0x023au, 0x023a, 0x000fu, 0x0fu,  0U, 0, 1U, 12u, 0x1010u, 0x0000u
#define IO_CUSTOM_91804__DCDC_VDDA3L_HYST \
	io0x23a, io_byte_0x23a, 0x023au, 0x023a, 0x00e0u, 0xe0u,  5U, 5, 1U, 12u, 0x1010u, 0x0000u
#define IO_CUSTOM_91804__DCDC_VDDA3H_VTH \
	io0x23a, io_byte_0x23b, 0x023au, 0x023b, 0x0f00u, 0x0fu,  8U, 0, 1U, 12u, 0x1010u, 0x0000u
#define IO_CUSTOM_91804__DCDC_VDDA3H_HYST \
	io0x23a, io_byte_0x23b, 0x023au, 0x023b, 0xe000u, 0xe0u, 13U, 5, 1U, 12u, 0x1010u, 0x0000u
#define IO_CUSTOM_91804__DCDC_FORCE_1X \
	io0x23c, io_byte_0x23c, 0x023cu, 0x023c, 0x0001u, 0x01u,  0U, 0, 1U, 12u, 0xf008u, 0x0000u
#define IO_CUSTOM_91804__DCDC_FORCE_1_5X \
	io0x23c, io_byte_0x23c, 0x023cu, 0x023c, 0x0002u, 0x02u,  1U, 1, 1U, 12u, 0xf008u, 0x0000u
#define IO_CUSTOM_91804__DCDC_FORCE_2X \
	io0x23c, io_byte_0x23c, 0x023cu, 0x023c, 0x0004u, 0x04u,  2U, 2, 1U, 12u, 0xf008u, 0x0000u
#define IO_CUSTOM_91804__DCDC_DUTY_CYCLE_EN \
	io0x23c, io_byte_0x23c, 0x023cu, 0x023c, 0x0010u, 0x10u,  4U, 4, 1U, 12u, 0xf008u, 0x0000u
#define IO_CUSTOM_91804__DCDC_BYPASS \
	io0x23c, io_byte_0x23c, 0x023cu, 0x023c, 0x0020u, 0x20u,  5U, 5, 1U, 12u, 0xf008u, 0x0000u
#define IO_CUSTOM_91804__FRC_DCDC_ENDIS \
	io0x23c, io_byte_0x23c, 0x023cu, 0x023c, 0x00c0u, 0xc0u,  6U, 6, 1U, 12u, 0xf008u, 0x0000u
#define IO_CUSTOM_91804__DCDC_FORCE_CLK_EXT \
	io0x23c, io_byte_0x23d, 0x023cu, 0x023d, 0x0100u, 0x01u,  8U, 0, 1U, 12u, 0xf008u, 0x0000u
#define IO_CUSTOM_91804__DCDC_PROBE_CLK \
	io0x23c, io_byte_0x23d, 0x023cu, 0x023d, 0x0200u, 0x02u,  9U, 1, 1U, 12u, 0xf008u, 0x0000u
#define IO_CUSTOM_91804__DCDC_PROBE_VREG \
	io0x23c, io_byte_0x23d, 0x023cu, 0x023d, 0x0400u, 0x04u, 10U, 2, 1U, 12u, 0xf008u, 0x0000u
#define IO_CUSTOM_91804__DCDC_PROBE_VX \
	io0x23c, io_byte_0x23d, 0x023cu, 0x023d, 0x0800u, 0x08u, 11U, 3, 1U, 12u, 0xf008u, 0x0000u
#define IO_CUSTOM_91804__SPARE_VDIG \
	io0x23e, io_byte_0x23e, 0x023eu, 0x023e, 0x00ffu, 0xffu,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__SPARE_VDD \
	io0x23e, io_byte_0x23f, 0x023eu, 0x023f, 0xff00u, 0xffu,  8U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_CUSTOM_91804__SPARE_VANA \
	io0x240, io_byte_0x240, 0x0240u, 0x0240, 0x00ffu, 0xffu,  0U, 0, 1U, 12u, 0x0f00u, 0x0000u
#define IO_CUSTOM_91804__SENS_RESERVE \
	io0x240, io_byte_0x241, 0x0240u, 0x0241, 0xf000u, 0xf0u, 12U, 4, 1U, 12u, 0x0f00u, 0x0000u
#define IO_CUSTOM_91804__LFO_FREQ_TRIM_ULP \
	io0x242, io_byte_0x242, 0x0242u, 0x0242, 0x007fu, 0x7fu,  0U, 0, 1U, 12u, 0xff80u, 0x0000u

/*****************************************************************************/
/* 3.4 Dummy ports. */

/* MlxCCT private. */

extern volatile uint16_t iodummy __attribute__((addr(0x0)));
extern volatile uint8_t  iodummy8 __attribute__((addr(0x0)));

#define IO_ADC_XH018__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ADC_XH018__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ADC_XH018__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ADC_XH018__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ADC_XH018__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ADC_XH018__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ADC_XH018__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ADC_XH018__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ADC_XH018__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ADC_XH018__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ADC_XH018__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ADC_XH018__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ADC_XH018__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ADC_XH018__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ADC_XH018__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ADC_XH018__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ADC_XH018__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_AWD__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_AWD__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_AWD__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_AWD__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_AWD__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_AWD__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_AWD__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_AWD__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_AWD__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_AWD__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_AWD__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_AWD__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_AWD__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_AWD__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_AWD__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_AWD__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_AWD__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_CUSTOM_91804__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_CUSTOM_91804__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_CUSTOM_91804__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_CUSTOM_91804__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_CUSTOM_91804__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_CUSTOM_91804__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_CUSTOM_91804__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_CUSTOM_91804__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_CUSTOM_91804__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_CUSTOM_91804__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_CUSTOM_91804__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_CUSTOM_91804__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_CUSTOM_91804__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_CUSTOM_91804__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_CUSTOM_91804__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_CUSTOM_91804__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_CUSTOM_91804__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_FLASH_SHELL__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_FLASH_SHELL__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_FLASH_SHELL__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_FLASH_SHELL__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_FLASH_SHELL__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_FLASH_SHELL__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_FLASH_SHELL__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_FLASH_SHELL__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_FLASH_SHELL__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_FLASH_SHELL__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_FLASH_SHELL__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_FLASH_SHELL__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_FLASH_SHELL__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_FLASH_SHELL__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_FLASH_SHELL__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_FLASH_SHELL__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_FLASH_SHELL__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_FUNC_TEST__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_FUNC_TEST__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_FUNC_TEST__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_FUNC_TEST__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_FUNC_TEST__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_FUNC_TEST__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_FUNC_TEST__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_FUNC_TEST__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_FUNC_TEST__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_FUNC_TEST__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_FUNC_TEST__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_FUNC_TEST__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_FUNC_TEST__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_FUNC_TEST__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_FUNC_TEST__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_FUNC_TEST__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_FUNC_TEST__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_MLX16__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_MLX16__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_MLX16__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_MLX16__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_MLX16__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_MLX16__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_MLX16__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_MLX16__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_MLX16__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_MLX16__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_MLX16__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_MLX16__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_MLX16__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_MLX16__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_MLX16__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_MLX16__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_MLX16__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_MUPET__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_MUPET__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_MUPET__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_MUPET__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_MUPET__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_MUPET__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_MUPET__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_MUPET__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_MUPET__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_MUPET__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_MUPET__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_MUPET__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_MUPET__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_MUPET__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_MUPET__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_MUPET__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_MUPET__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_NVRAM_SHELL__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_NVRAM_SHELL__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_NVRAM_SHELL__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_NVRAM_SHELL__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_NVRAM_SHELL__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_NVRAM_SHELL__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_NVRAM_SHELL__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_NVRAM_SHELL__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_NVRAM_SHELL__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_NVRAM_SHELL__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_NVRAM_SHELL__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_NVRAM_SHELL__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_NVRAM_SHELL__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_NVRAM_SHELL__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_NVRAM_SHELL__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_NVRAM_SHELL__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_NVRAM_SHELL__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_RAM_BIST__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_RAM_BIST__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_RAM_BIST__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_RAM_BIST__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_RAM_BIST__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_RAM_BIST__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_RAM_BIST__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_RAM_BIST__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_RAM_BIST__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_RAM_BIST__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_RAM_BIST__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_RAM_BIST__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_RAM_BIST__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_RAM_BIST__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_RAM_BIST__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_RAM_BIST__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_RAM_BIST__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_ROM_BIST__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ROM_BIST__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ROM_BIST__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ROM_BIST__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ROM_BIST__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ROM_BIST__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ROM_BIST__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ROM_BIST__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ROM_BIST__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ROM_BIST__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ROM_BIST__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ROM_BIST__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ROM_BIST__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ROM_BIST__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ROM_BIST__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ROM_BIST__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ROM_BIST__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_RST_CTRL__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_RST_CTRL__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_RST_CTRL__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_RST_CTRL__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_RST_CTRL__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_RST_CTRL__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_RST_CTRL__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_RST_CTRL__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_RST_CTRL__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_RST_CTRL__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_RST_CTRL__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_RST_CTRL__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_RST_CTRL__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_RST_CTRL__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_RST_CTRL__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_RST_CTRL__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_RST_CTRL__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_STIMER1__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_STIMER1__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_STIMER1__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_STIMER1__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_STIMER1__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_STIMER1__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_STIMER1__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_STIMER1__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_STIMER1__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_STIMER1__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_STIMER1__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_STIMER1__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_STIMER1__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_STIMER1__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_STIMER1__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_STIMER1__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_STIMER1__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_STIMER2__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_STIMER2__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_STIMER2__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_STIMER2__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_STIMER2__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_STIMER2__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_STIMER2__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_STIMER2__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_STIMER2__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_STIMER2__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_STIMER2__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_STIMER2__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_STIMER2__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_STIMER2__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_STIMER2__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_STIMER2__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_STIMER2__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_VERSION__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_VERSION__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_VERSION__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_VERSION__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_VERSION__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_VERSION__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_VERSION__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_VERSION__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_VERSION__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_VERSION__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_VERSION__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_VERSION__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_VERSION__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_VERSION__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_VERSION__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_VERSION__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_VERSION__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U


/*****************************************************************************/
/* 4. Statistics. */

/* Report of values which might be of interest. */

/*
 * Range: 0x0000u to 0x0242u
 * Variables: 191
 * Fields: 687
 * Occupation: 92.41% (353.0 bytes)
 */

#endif /* IO_H */

/* EOF */
