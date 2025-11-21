/*
 * Copyright (C) 2014-2019 Melexis N.V.
 *
 * Software Platform
 *
 */

#ifndef LFTX_H
#define LFTX_H

/**
    @defgroup LfTx Low Frequency (125 kHz) Transmitter Interface, <lftx.h>

    The functions to operate the LF Transmitter.
 */

#include <stdint.h>
#include <stdbool.h>

#include <compiler_abstraction.h>
#include <io.h>

static INLINE void LfTx_FifoWriteByte(uint8_t data);
static INLINE void LfTx_FifoFlush(void);
static INLINE bool LfTx_IsFifoFull(void);
static INLINE bool LfTx_IsFifoError(void);
static INLINE bool LfTx_IsFifoOverrun(void);
static INLINE bool LfTx_IsFifoUnderrun(void);
static INLINE uint8_t LfTx_FifoCount(void);

extern volatile uint8_t port_lftx_fifo_count_   __attribute__((addr(IO_BYTE_ADDRESS(CUSTOM_91804, LFTX_FIFO_COUNT))));  /* MSByte */

/* -- LF TX FIFO operations -------------------------------------------------------------------- */
/** @ingroup LfTx
    Write byte into LFTX FIFO buffer.

    @param  data    Data byte to be written into FIFO buffer.

    @note
    -   LFTX FIFO size is 8 bytes.
 */
static INLINE
void LfTx_FifoWriteByte(uint8_t data)
{
    extern volatile uint8_t port_lftx_fifo_data_ __attribute__((addr(IO_BYTE_ADDRESS(CUSTOM_91804, LFTX_FIFO_DATA))));

    port_lftx_fifo_data_ = data;
}


/** @ingroup LfTx
    Flush LFTX FIFO buffer.

    @note
    -   LFTX FIFO size is 8 bytes.
    -   The flush operation clears the over-/underrun condition (LFTX_FIFO_OVUND = 0)
        as well as the LFTX FIFO counter (LFTX_FIFO_COUNT = 0).
 */
static INLINE
void LfTx_FifoFlush(void)
{
    extern volatile uint8_t port_lftx_fifo_flush_ __attribute__((addr(IO_BYTE_ADDRESS(CUSTOM_91804, LFTX_FIFO_FLUSH))));

    port_lftx_fifo_flush_ = 0;   /* Write any arbitrary dummy value to flush the FIFO */
}

/** @ingroup LfTx
    Get LFTX FIFO count.

    @returns 4-bit LFTX FIFO count, maximum = 8 if FIFO is full.
 */
static INLINE
uint8_t LfTx_FifoCount(void)
{
    return port_lftx_fifo_count_ & 0x0Fu; /* 4-bit only */
}

/** @ingroup LfTx
    Returns true if LFTX FIFO is full.

    @returns true if LFTX FIFO is full.
 */
static INLINE
bool LfTx_IsFifoFull(void)
{
    uint16_t mask = 1u << IO_OFFSET_WITHIN_BYTE(CUSTOM_91804, LFTX_FIFO_FULL);
    return (port_lftx_fifo_count_ & mask) != 0u;
}

/** @ingroup LfTx
    Returns true if FIFO errors (overrun or underrun) are detected.

    @returns true if FIFO errors are detected.

    @note
    -   Can be used to identify the reason of TX abortion: FIFO errors or LF carrier lost.
 */
static INLINE
bool LfTx_IsFifoError(void)
{
    uint16_t mask = 1u << IO_OFFSET_WITHIN_BYTE(CUSTOM_91804, LFTX_FIFO_OVUND);
    return (port_lftx_fifo_count_ & mask) != 0u;
}


/** @ingroup LfTx
    Returns true if LFTX FIFO is overrun.

    @returns true if LFTX FIFO is overrun.
 */
static INLINE
bool LfTx_IsFifoOverrun(void)
{
    uint16_t mask = (1u << IO_OFFSET_WITHIN_BYTE(CUSTOM_91804, LFTX_FIFO_FULL))
                |   (1u << IO_OFFSET_WITHIN_BYTE(CUSTOM_91804, LFTX_FIFO_OVUND));

    uint16_t expected = mask;

    return (port_lftx_fifo_count_ & mask) == expected;
}


/** @ingroup LfTx
    Returns true if LFTX FIFO is underrun.

    @returns true if LFTX FIFO is underrun.
 */
static INLINE
bool LfTx_IsFifoUnderrun(void)
{
    uint16_t mask = (1u << IO_OFFSET_WITHIN_BYTE(CUSTOM_91804, LFTX_FIFO_FULL))
                  | (1u << IO_OFFSET_WITHIN_BYTE(CUSTOM_91804, LFTX_FIFO_OVUND));

    uint16_t expected = 1u << IO_OFFSET_WITHIN_BYTE(CUSTOM_91804, LFTX_FIFO_OVUND);

    return (port_lftx_fifo_count_ & mask) == expected;
}

#endif  /* LFTX_H */
