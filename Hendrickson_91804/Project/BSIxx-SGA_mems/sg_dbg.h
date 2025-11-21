#ifndef SG_DBG_H
#define SG_DBG_H

#include "datatypes.h"
#include "spi_process.h"

#if defined(SG_ENABLE_DEBUG_UART)
#include <debug.h>

#define SG_UART_TX_PIN 	GPIO1	// uart output pin
//	#define DEBUG_ENABLED	// defined in Makefile

	#if defined(SG_UART_TX_PIN)
	#define DEFAULT_DBG_BUFFER_LEN_MAX		30
	#define dbg(...) dbg_string(__VA_ARGS__)
	#define DBG(...) dbg_string(__VA_ARGS__)
	#else
	#define dbg(...) DEBUG_PUT_STRING(__VA_ARGS__)
	#define DBG(...) DEBUG_PUT_STRING(__VA_ARGS__)
	#endif

#elif defined(SG_ENABLE_DEBUG_SPI)
/*
static char dbg_buf[0x11];
	#define dbg(...)                    \
		do {                            \
			sprintf(dbg_buf, __VA_ARGS__);  \
			dbg_spi(dbg_buf);               \
		} while(0)

	#define DBG(...) dbg(__VA_ARGS__)
*/

	#define dbg(...) dbg_str(__VA_ARGS__)
	#define DBG(...) dbg_str(__VA_ARGS__)
#else
	#define dbg(...)
	#define DBG(...)

#endif


#ifdef SG_ENABLE_DEBUG_IO

#define DBG_IO	SPI_CLK //GPIO3
#define DEFAULT_DEBUG_IO_TIME_US  10

#define DBG_IO_FLASH(n) do{ \
			Gpio_SetPinMode(DBG_IO, GPIO_DIG_OUTPUT_0); \
			DELAY_US(2); \
			for(uint8_t ii=0; ii<n; ii++) { \
				Gpio_WritePin(DBG_IO, 1); \
				DELAY_US(DEFAULT_DEBUG_IO_TIME_US); \
				Gpio_WritePin(DBG_IO, 0); \
				DELAY_US(DEFAULT_DEBUG_IO_TIME_US); \
			} \
		} while (0)
#else
#define DBG_IO_FLASH(n)
#endif

#define UINT16_TO_BYTE0(n)            ((uint8_t) (n)&0xFF)
#define UINT16_TO_BYTE1(n)            ((uint8_t) ((n) >> 8)&0xFF)

#define SG_TYPE_OLD_FORMAT  	(0x02)
#define SG_TYPE_NEW_FORMAT  	(0x20)

#if 0 //org:2024/01/14
#define SG_DBG_FORMAT_STRING	(0xFE)
#define SG_DBG_FORMAT_ARRAY		(0xFF)
#else
#define SG_DBG_FORMAT_STRING		(0xE0)
#define SG_DBG_FORMAT_ARRAY			(0xF0)
#define SG_DBG_FORMAT_ARRAY_PVT		(0xFF)	// PVT
#define SG_DBG_FORMAT_ARRAY_CNT		(0xFE)	// Counter
#endif



extern void dbg_byte(uint16_t b);
extern void dbg_word(uint16_t val);
extern void dbg_dec(int32_t val);
extern void dbg_hex(int32_t val);
extern void dbg_string(char *str);
extern void dbg_pvt(T_TPMS_DATA_MEASURE *pData);
#ifdef SG_ENABLE_DBG_TIMER
extern void dbg_sp_cnt(T_TPMS_SP_CNT *pData);
#endif

#endif // SG_DBG_H
