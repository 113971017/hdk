/*
 * Copyright (C) 2013-2019 Melexis N.V.
 *
 * ML16 CPU configurations
 *
 */

#ifndef MLX16_CFG_H
#define MLX16_CFG_H

/* Check if MLX16 co-processor is available */
#if defined (__MLX16_X8__) || defined (__MLX16_FX__) \
  || defined (__MLX16_EX__)
#define HAS_MLX16_COPROCESSOR
#endif

/* Check if new extra instructions are available, e.g. FSB / SFB  */
#if defined (__MLX16_8__) || defined (__MLX16_X8__) || defined (__MLX16_FX__) \
    || defined (__MLX16_E8__) || defined (__MLX16_EX__)
#define HAS_MLX16_FSB_SFB_INSTRUCTIONS
#elif defined(__COVERITY__) || defined(__POLYSPACE__)
    /* Allow Coverity or Polyspace builds */
#else
#error Specified MLX16 architecture is NOT supported by mathlib
#endif

#endif /* MLX16_CFG_H */
