/*
 * Copyright (C) 2008-2012 Melexis N.V.
 *
 * DSP Library
 *
 */

#ifndef MATHLIB_DSP_H
#define MATHLIB_DSP_H

extern  uint8_t vecsumU8_U8  (const uint8_t  *a, uint16_t n);
extern   int8_t vecsumI8_I8  (const  int8_t  *a, uint16_t n);
extern uint16_t vecsumU16_U8 (const uint8_t  *a, uint16_t n);
extern  int16_t vecsumI16_I8 (const  int8_t  *a, uint16_t n);
extern uint16_t vecsumU16_U16(const uint16_t *a, uint16_t n);
extern  int16_t vecsumI16_I16(const  int16_t *a, uint16_t n);

extern int32_t vecsumI32_I16(const int16_t *a, uint16_t n);
extern int32_t vecsumI32_I32(const int32_t *a, uint16_t n);
extern int32_t vecsumI48_I32(const int32_t *a, uint16_t n, int16_t *msw);
extern uint32_t norm2U32_I16byI16(int16_t a, int16_t b);
extern uint32_t norm2U48_I16byI16(int16_t a, int16_t b, uint16_t *msw);
extern uint32_t norm2vectorU32_I16byI16(const int16_t *a, uint16_t n);
extern uint32_t norm2vectorU48_I16byI16(const int16_t *a, uint16_t n, uint16_t *msw);
extern int32_t dotproductI32_I16byI16(const int16_t *a, const int16_t *b, uint16_t n);
extern /*u*/int32_t dotproductI48_I16byI16(const int16_t *a, const int16_t *b, uint16_t n, int16_t *msw);

extern uint32_t norm1vectorU32_I16(const int16_t *a, uint16_t n);
extern uint32_t norm1vectorU32_I32(const int32_t *a, uint16_t n);
extern uint32_t norm1vectorU48_I32(const int32_t *a, uint16_t n, uint16_t *msw);

#endif /* MATHLIB_DSP_H */
