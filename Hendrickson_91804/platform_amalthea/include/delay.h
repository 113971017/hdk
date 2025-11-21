/*
 * Copyright (C) 2014-2019 Melexis N.V.
 *
 * Software Platform
 *
 */

#ifndef DELAY_H
#define DELAY_H

/**
    Busy-wait (blocking) delay functions

 */

#include <stdint.h>
#include <compiler_abstraction.h>

/**
    Delay for specified number of 'nop' instructions

    @param nops [3..65535]  A number of 'nop' instructions for delay.

    @note
    The minimum value for the @p nops parameter is 3!
 */
static INLINE
void delay_nops(uint16_t nops)
{
    uint16_t dummy;     /* used to inform that input operand is clobbered  */
    __asm__ __volatile__ (
        "add %[cnt], #-2 \n\t"  /* '-2' to compensate time for X loading */
        "djnz %[cnt], ."
        : "=x" (dummy)
        : [cnt] "x" (nops)
    );
}

#endif /* DELAY_H */
