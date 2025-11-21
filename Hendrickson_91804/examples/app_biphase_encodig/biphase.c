/*
 * Copyright (C) 2016 Melexis N.V.
 *
 * Software Platform
 */

#include <stdint.h>
#include <stdbool.h>

#include "biphase.h"

static uint8_t encode_nibble_to_bps(uint8_t nibble, uint16_t init_level);

static const uint8_t to_bps[16] = {

    /*  BSP: Bi-Phase Space (BPS) encoding table
        - change output level on clock posedge (start of the input bit)
        - if input bit is 0, change output level on clock negedge (middle of the input bit)
        - MSBit first data flow
        - initial level 0 is assumed, otherwise result shall be inverted
    */
    [ 0 /*0b0000*/] = 0xAAu /*0b10101010*/,
    [ 1 /*0b0001*/] = 0xABu /*0b10101011*/,
    [ 2 /*0b0010*/] = 0xADu /*0b10101101*/,
    [ 3 /*0b0011*/] = 0xACu /*0b10101100*/,
    [ 4 /*0b0100*/] = 0xB5u /*0b10110101*/,
    [ 5 /*0b0101*/] = 0xB4u /*0b10110100*/,
    [ 6 /*0b0110*/] = 0xB2u /*0b10110010*/,
    [ 7 /*0b0111*/] = 0xB3u /*0b10110011*/,

    [ 8 /*0b1000*/] = 0xD5u /*0b11010101*/,
    [ 9 /*0b1001*/] = 0xD4u /*0b11010100*/,
    [10 /*0b1010*/] = 0xD2u /*0b11010010*/,
    [11 /*0b1011*/] = 0xD3u /*0b11010011*/,
    [12 /*0b1100*/] = 0xCAu /*0b11001010*/,
    [13 /*0b1101*/] = 0xCBu /*0b11001011*/,
    [14 /*0b1110*/] = 0xCDu /*0b11001101*/,
    [15 /*0b1111*/] = 0xCCu /*0b11001100*/
};


/*
*/
void encode_to_biphase_space(uint8_t *dst, uint8_t const *src, uint16_t size, uint16_t init_level)
{
    bool is_low_nibble = false; /* start from the high nibble */

    uint8_t  databyte = 0;
    uint16_t encoded = init_level;

    do {
        if (is_low_nibble) {
            encoded = encode_nibble_to_bps(databyte & 0x0Fu, /* low nibble */
                                            encoded & 1u);   /* last bit of previous value as init for the next */
            is_low_nibble = false;
        }
        else {
            databyte = *src++;
            encoded = encode_nibble_to_bps((databyte >> 4) & 0x0Fu,  /* high nibble */
                                            encoded & 1u);
            is_low_nibble = true;
        }

        *dst++ = (uint8_t)encoded;   /* store */

    } while (--size != 0u);
}


/*
    Bi-Phase Mark (BPM) encoding can be obtained from BPS by:
    - inverting input data stream (to get signal transition in the middle of
      the 1-bit, while in BPS it's in the middle of 0-bit) 
*/
void encode_to_biphase_mark(uint8_t *dst, uint8_t const *src, uint16_t size, uint16_t init_level)
{
    bool is_low_nibble = false; /* start from the high nibble */

    uint8_t  databyte = 0;
    uint16_t encoded = init_level;

    do {
        if (is_low_nibble) {
            encoded = encode_nibble_to_bps(databyte & 0x0Fu,     /* low nibble */
                                            encoded & 1u);       /* last bit of previous value as init for the next */
            is_low_nibble = false;
        }
        else {
            databyte = *src++;
            databyte = ~databyte;                                       /* invert input stream to get BPM based on BPS */
            encoded = encode_nibble_to_bps(((databyte) >> 4) & 0x0Fu,   /* high nibble */
                                            encoded & 1u);
            is_low_nibble = true;
        }

        *dst++ = (uint8_t)encoded;

    } while (--size != 0u);
}

/*
    @brief
    Encode data nibble to Bi-Phase Space (BPS) byte

    Data nibble is encoded starting from the MSBit with `init_level` as
    an initial line level

    @param  nibble      input data nibble
    @param  init_level  initial line level

    @return data byte encoded to Bi-Phase Space (BPS)

    @internal
    Bi-Phase Space (BPS) encoding algorithm:
    - change output level on clock posedge (start of the input bit)
    - if input bit is 0, change output level on clock negedge (middle of the input bit)

*/
static uint8_t encode_nibble_to_bps(uint8_t nibble, uint16_t init_level)
{
    uint16_t encoded = to_bps[nibble & 0x0Fu];

    if (init_level != 0u) {
        encoded = ~encoded;    /* invert output */
    }
    /* else: no inversion needed */

    return (uint8_t)encoded;
}

/* EOF */
