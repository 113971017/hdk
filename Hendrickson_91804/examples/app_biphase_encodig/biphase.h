/*
 * Copyright (C) 2016-2022 Melexis N.V.
 *
 * Software Platform
 */

#ifndef BIPHASE_H
#define BIPHASE_H

#include <stdint.h>

/**
    @brief
    Encode data buffer `src` to Bi-Phase Space (BPS) buffer `dst`

    Data nibbles are encoded starting from the MSBit

    @param  dst         destination buffer for encoded data
    @param  src         source buffer with initial non-encoded data
    @param  size        number of nibbles to encode from input `src` buffer (equal
                        to the size of output `dst` buffer since each nibble of
                        input data is converted to one byte in the output buffer)
    @param  init_level  initial bus level (0 or 1)

    @warning
    Output buffer `dst` shall be as twice bigger than the input buffer `src`
*/
extern void encode_to_biphase_space(uint8_t *dst, uint8_t const *src, uint16_t size, uint16_t init_level);

/**
    @brief
    Encode data buffer `src` to Bi-Phase Mark (BPM) buffer `dst`

    Data nibbles are encoded starting from the MSBit

    @param  dst         destination buffer for encoded data
    @param  src         source buffer with initial non-encoded data
    @param  size        number of nibbles to encode from input `src` buffer (equal
                        to the size of output `dst` buffer since each nibble of
                        input data is converted to one byte in the output buffer)

    @param init_level  initial bus level (0 or 1)

    @warning
    Output buffer `dst` shall be as twice bigger than the input buffer `src`
*/
extern void encode_to_biphase_mark(uint8_t *dst, uint8_t const *src, uint16_t size, uint16_t init_level);

#endif /* BIPHASE_H */
