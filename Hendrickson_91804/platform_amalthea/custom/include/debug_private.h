/*
 * Copyright (C) 2019 Melexis N.V.
 *
 * Software Platform
 *
 */
#ifndef DEBUG_PRIVATE_H
#define DEBUG_PRIVATE_H

#include <stdint.h>
#include <stdbool.h>

extern void debug_put_byte(uint8_t  data_byte);
extern void debug_put_word(uint16_t data_word);
extern void debug_put_word_hex(uint16_t word);
extern void debug_put_string(char const *s);

extern void debug_pin_init(bool init_state);
extern void debug_pin_toggle(uint16_t n);
extern void debug_pin_pulse(uint16_t n);

#endif /* DEBUG_PRIVATE_H */
