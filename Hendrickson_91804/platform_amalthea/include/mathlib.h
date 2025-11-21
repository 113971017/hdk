/*
 * Copyright (C) 2007-2019 Melexis N.V.
 *
 * Math Library
 *
 *
 *
 */

#ifndef MATHLIB_H_
#define MATHLIB_H_

#include <mlx16_cfg.h>

/* SW component version */
#define MLX_MATHLIB_SW_MAJOR_VERSION    2
#define MLX_MATHLIB_SW_MINOR_VERSION    2
#define MLX_MATHLIB_SW_PATCH_VERSION    4

/* Validate MLX16-GCC version */
#if ((__MLX16_GCC_MAJOR__ == 1) && (__MLX16_GCC_MINOR__ >= 8)) || (__MLX16_GCC_MAJOR__ > 1)
    /* ok */
#else
#warning "Math library requires MLX16-GCC release 1.8 or later"
#endif


#if !defined (__ASSEMBLER__)

#include <stdint.h>
#include <builtin_mlx16.h>

/*
 *  Multiplication
 */

uint32_t mulU32_U16byU16(uint16_t a, uint16_t b);
int32_t  mulI32_I16byI16(int16_t a, int16_t b);
int32_t  mulI32_I16byU16(int16_t a, uint16_t b);

int16_t  mulI16_I16byI16(int16_t a, int16_t b);
int16_t  mulI16_I16byU16(int16_t a, uint16_t b);
uint16_t mulU16_U16byU16(uint16_t a, uint16_t b);

int16_t  mulQ15_Q15byQ15(int16_t a, int16_t b);

int32_t  mulI32_I32byI16(int32_t a, int16_t b);
int32_t  mulI32_I32byU16(int32_t a, uint16_t b);
uint32_t mulU32_U32byU16(uint32_t a, uint16_t b);

int32_t  mulI32hi_I32byI16(int32_t a, int16_t b);
int32_t  mulI32hi_I32byU16(int32_t a, uint16_t b);
uint32_t mulU32hi_U32byU16(uint32_t a, uint16_t b);

int32_t  mulI24_I16byI8(int16_t a, int8_t b);
int32_t  mulI24_I16byU8(int16_t a, uint8_t b);
uint32_t mulU24_U16byU8(uint16_t a, uint8_t b);

int16_t  mulI16hi_I16byI8(int16_t a, int8_t b);
int16_t  mulI16hi_I16byU8(int16_t a, uint8_t b);
uint16_t mulU16hi_U16byU8(uint16_t a, uint8_t b);

int16_t  mulI16_I8byI8(int8_t a, int8_t b);
int16_t  mulI16_I8byU8(int8_t a, uint8_t b);
uint16_t mulU16_U8byU8(uint8_t a, uint8_t b);

/*
 *  Division
 */

uint32_t divU32_U32byU16(uint32_t n, uint16_t d);
int32_t  divI32_I32byI16(int32_t n, int16_t d);
int32_t  divI32_I32byU16(int32_t n, uint16_t d);

uint16_t divU16_U32byU16(uint32_t n, uint16_t d);
int16_t  divI16_I32byI16(int32_t n, int16_t d);
int16_t  divI16_I32byU16(int32_t n, uint16_t d);

uint16_t divU16_U16byU16(uint16_t n, uint16_t d);
int16_t  divI16_I16byI16(int16_t n, int16_t d);
int16_t  divI16_I16byU16(int16_t n, uint16_t d);

uint8_t  divU8_U8byU8(uint8_t n, uint8_t d);
int8_t   divI8_I8byI8(int8_t n, int8_t d);
int8_t   divI8_I8byU8(int8_t n, uint8_t d);

uint8_t  divU8hi_U8byU8(uint8_t n, uint8_t d);

/*
 *  Power
 */

uint16_t isqrt16 (uint16_t x);
uint16_t isqrt32 (uint32_t x);

/*
 *  log/exp
 */

uint16_t ilog2_U16 (uint16_t v);
uint16_t ilog2_U32 (uint32_t v);

uint16_t iexp2_U16 (uint16_t v);
uint32_t iexp2_U32 (uint16_t v);

/*
 *  DSP
 */

uint32_t vecsumU32_U16(const uint16_t *a, uint16_t n);
uint32_t vecsumU32_U32(const uint32_t *a, uint16_t n);
uint32_t vecsumU48_U32(const uint32_t *a, uint16_t n, uint16_t *msw);
uint32_t norm2U32_U16byU16(uint16_t a, uint16_t b);
uint32_t norm2U48_U16byU16(uint16_t a, uint16_t b, uint16_t *msw);
uint32_t norm2vectorU32_U16byU16(const uint16_t *a, uint16_t n);
uint32_t norm2vectorU48_U16byU16(const uint16_t *a, uint16_t n, uint16_t *msw);
uint32_t dotproductU32_U16byU16(const uint16_t *a, const uint16_t *b, uint16_t n);
uint32_t dotproductU48_U16byU16(const uint16_t *a, const uint16_t *b, uint16_t n, uint16_t *msw);

/*
 *  Trigonometric
 */

int16_t sinU16 (uint16_t in);
int16_t sinI16 (int16_t x);

int16_t cosU16 (uint16_t x);
int16_t cosI16 (int16_t x);

int32_t tanU16 (uint16_t x);
int32_t tanI16 (int16_t x);

int16_t atan2U16 (uint16_t y, uint16_t x);
int16_t atan2I16 (int16_t y, int16_t x);

/*
 *  Other
 */

uint32_t rand32(uint32_t seed);
void   init_lfsr16(uint16_t seed);
void   init_lfsr32(uint32_t seed);
uint16_t lfsr16(void);
uint32_t lfsr32(void);

uint16_t parity4 (uint8_t in);
uint16_t parity8 (uint8_t in);
uint16_t parity16(uint16_t in);
uint16_t parity32(uint32_t v);

uint16_t crc16 (uint8_t c, uint16_t seed);
uint8_t  crc8 (uint8_t c, uint8_t seed);
uint16_t crc_ccitt(uint8_t c, uint16_t seed);

uint8_t   bitrev4 (uint8_t  x);
uint8_t   bitrev8 (uint8_t  x);
uint16_t  bitrev16(uint16_t in);

uint8_t   interleave4 (uint8_t  x, uint8_t  y);
uint16_t  interleave8 (uint8_t  x, uint8_t  y);
uint32_t  interleave16(uint16_t x, uint16_t y);


/** @ingroup mathlib
    Calculates (first order) low pass filter
      out[k] = 2^n-1/2^n * out[k-1] + in/2^n = out[k-1] - (out[k-1] - in)/2^n
      also updates out[k]

    @param      in      input value, in A
    @param      p_out   pointer to out[k-1], in [S-4]
    @param      n       filter depth, in [S-6]
    @returns    out[k]
    @note
        1. Optimized version of lpfn_I32 from Melexis mathlib library
        2. Requires x8 coprocessor
*/
#if defined(HAS_MLX16_COPROCESSOR)
extern int16_t lpfn_I32 (int16_t in, int32_t *p_out, int16_t n);
#endif

#endif /* ! __ASSEMBLER__ */

#endif /* MATHLIB_H_ */
