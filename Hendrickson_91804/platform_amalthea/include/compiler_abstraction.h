/*
 * Copyright (C) 2013-2019 Melexis N.V.
 *
 * Amalthea Software Platform
 *
 */


#ifndef COMPILER_ABSTRACTION_H
#define COMPILER_ABSTRACTION_H

/* Abstraction for inlining */
#ifndef INLINE
#   define INLINE       __attribute__((always_inline)) inline           /**< define function as inline */
#else
#   warning "An alternative definition of INLINE is detected"
#endif

/* Interrupt */
#ifndef INTERRUPT
#   define INTERRUPT    __attribute__((interrupt))                      /**< define function as interrupt handler */
#else
#   warning "An alternative definition of INTERRUPT is detected"
#endif

/* Static */
#ifndef STATIC
#   define  STATIC      static
#else
#   warning "An alternative definition of STATIC is detected"
#endif

/* Memory Attributes */
#define DP_MEM                  __attribute__((dp))                     /**< links an object to DP section of the RAM  */
#define NOINIT_MEM              __attribute__((section(".noinit")))     /**< links an object to NOINIT section of the RAM (not initialized at startup) */
#define EP_MEM                  __attribute__((ep))                     /**< links an object to EP section (normally mapped to nvRAM or EEPROM) */
#define NVRAM_MEM               EP_MEM                                  /**< links an object to nvRAM area */

#endif /* COMPILER_ABSTRACTION_H */
