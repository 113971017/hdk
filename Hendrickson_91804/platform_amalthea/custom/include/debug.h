/*
 * Copyright (C) 2019-2020 Melexis N.V.
 *
 * Software Platform
 *
 */
#ifndef DEBUG_H
#define DEBUG_H

#if defined(DEBUG_ENABLED)

#include <stdint.h>
#include <stdbool.h>
#include <compiler_abstraction.h>
#include <io.h>
#include <debug_private.h>

#define LOGIC_HIGH  (true)
#define LOGIC_LOW   (false)

#define DEBUG_PUT_BYTE(data_byte)       debug_put_byte(data_byte)
#define DEBUG_PUT_WORD(data_word)       debug_put_word(data_word)
#define DEBUG_PUT_WORD_HEX(data_word)   debug_put_word_hex(data_word)
#define DEBUG_PUT_STRING(str)           debug_put_string(str)

#define DEBUG_PIN_INIT(init_state)      debug_pin_init(init_state)
#define DEBUG_PIN_TOGGLE(n)             debug_pin_toggle(n)
#define DEBUG_PIN_PULSE(n)              debug_pin_pulse(n)

#define DEBUG_ACK_AWD()                 debug_ack_awd()

static INLINE void debug_ack_awd(void);

#else

#if !defined(__DOXYGEN__)
/* empty definitions to exclude debug functions from code generation */
#define DEBUG_PUT_BYTE(data_byte)
#define DEBUG_PUT_WORD(data_word)
#define DEBUG_PUT_WORD_HEX(data_word)
#define DEBUG_PUT_STRING(str)

#define DEBUG_PIN_INIT(init_state)
#define DEBUG_PIN_TOGGLE(n)
#define DEBUG_PIN_PULSE(n)

#define DEBUG_ACK_AWD()
#endif  /* __DOXYGEN__ */

#endif

/* --- Implementation ------------------------------------------------------- */
#if defined(DEBUG_ENABLED)
/* Helper macro to textify the argument */
#ifndef TEXTIFY
#define TEXTIFY(a) TEXTIFY_2(a)
#define TEXTIFY_2(a) #a
#endif

#define IO_(PORT_NAME, BIT_NAME)  \
   "io:" \
   TEXTIFY(IO_BYTE_ADDRESS(PORT_NAME, BIT_NAME)) \
   "." \
   TEXTIFY(IO_OFFSET_WITHIN_BYTE(PORT_NAME, BIT_NAME))

static INLINE void debug_ack_awd(void) {
    asm volatile (
        "mov c, " IO_(AWD, WIN_OPEN) "\n\t"
        "mov " IO_(AWD, ACK) ", c \n\t"
    );
}

#endif /* DEBUG_ENABLED */


/* --- Documentation ------------------------------------------------------- */
#if defined(__DOXYGEN__)
/**
    @defgroup Debug Debug functions, <debug.h>

    Functions to output debug information:
    - via serial interface (SW UART, 115200 baud rate);
    - as logic level pulses on debug pin.

    By default, debug functions are disabled and all their invocations are
    ignored and not included into the final application. To enable debug
    functions, add `-DDEBUG_ENABLED` define to `CPPLAGS` variable in the
    Makefile.

    When enabled, debug functions use GPIO0 for an execution tracing
    or to deliver a debug data via the SW UART.

 */

/** @ingroup Debug

    Acknowledge Absolute Watchdog Timer (AWD) if window is open.
*/
void DEBUG_ACK_AWD(void);

/** @ingroup Debug

    Output data byte by using SW UART serial interface with 115200 baud rate.

    @param   data_byte   Data byte to be sent in LSB first 8-n-1 format.
*/
void DEBUG_PUT_BYTE(uint8_t  data_byte);

/** @ingroup Debug

    Output data word by using SW UART serial interface with 115200 baud rate.

    @param   data_word   Data word to be sent in LSB first 8-n-1 format.
*/
extern void DEBUG_PUT_WORD(uint16_t data_word);

/** @ingroup Debug

    Output data word as hex string by using SW UART serial interface with 115200 baud rate.

    @param   data_word   Data word to be sent.
*/
extern void DEBUG_PUT_WORD_HEX(uint16_t data_word);

/** @ingroup Debug

    Output string by using SW UART serial interface with 115200 baud rate.

    @param  str	 String to be sent.

    @note
        all non-printable symbols shall be inserted via "\\".

    @warning
    Sending a lng sting might cause a watchdog reset, as watchdog is **NOT
    acknowledged** in this function.
*/
extern void DEBUG_PUT_STRING(char *str);

/** @ingroup Debug

    Output specified logic level to debug pin.

    @param  init_state  Logic level LOGIC_HIGH or LOGIC_LOW.
*/
extern void DEBUG_PIN_INIT(bool init_state);

/** @ingroup Debug

    Toggles debug pin specified number of times.

    @param  n   Number of toggles.

    @warning
    Generating too many toggles might cause a watchdog reset, as watchdog
    is **NOT acknowledged** in this function.
*/
extern void DEBUG_PIN_TOGGLE(uint16_t n);

/** @ingroup Debug

    Outputs specified number of pulses on debug pin.

    @param  n   Number of pulses.

    @warning
    Generating too many toggles might cause a watchdog reset, as watchdog
    is **NOT acknowledged** in this function.
*/
extern void DEBUG_PIN_PULSE(uint16_t n);

#endif  /* __DOXYGEN__ */



#endif /* DEBUG_H */
