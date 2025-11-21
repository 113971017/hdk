#include <stdlib.h>
#include <sensorlib.h>
#include <platformlib.h>
#include "string.h"
#include "sg_dbg.h"
#include "sg_common.h"

//#include "main.h"


#if defined(SG_UART_TX_PIN)
#define SW_UART_PIN 		GPIO1
#define BIT_TIME_IN_NOPS 	(100u * SG_DELAY_NOPS_1_US)
#endif

//inline void dbg_byte(const char *str, uint8_t val){


void dbg_byte(uint16_t b)
{
#ifdef SG_UART_TX_PIN
	b |= (1u << 8); /* add STOP bit */
	b = b << 1; /* add START bit */
	Gpio_WritePin(SW_UART_PIN, (b & 1) ? 1 : 0);
	b >>= 1;
	delay_nops(BIT_TIME_IN_NOPS);
	Gpio_WritePin(SW_UART_PIN, (b & 1) ? 1 : 0);
	b >>= 1;
	delay_nops(BIT_TIME_IN_NOPS);
	Gpio_WritePin(SW_UART_PIN, (b & 1) ? 1 : 0);
	b >>= 1;
	delay_nops(BIT_TIME_IN_NOPS);
	Gpio_WritePin(SW_UART_PIN, (b & 1) ? 1 : 0);
	b >>= 1;
	delay_nops(BIT_TIME_IN_NOPS);
	Gpio_WritePin(SW_UART_PIN, (b & 1) ? 1 : 0);
	b >>= 1;
	delay_nops(BIT_TIME_IN_NOPS);
	Gpio_WritePin(SW_UART_PIN, (b & 1) ? 1 : 0);
	b >>= 1;
	delay_nops(BIT_TIME_IN_NOPS);
	Gpio_WritePin(SW_UART_PIN, (b & 1) ? 1 : 0);
	b >>= 1;
	delay_nops(BIT_TIME_IN_NOPS);
	Gpio_WritePin(SW_UART_PIN, (b & 1) ? 1 : 0);
	b >>= 1;
	delay_nops(BIT_TIME_IN_NOPS);
	Gpio_WritePin(SW_UART_PIN, (b & 1) ? 1 : 0);
	b >>= 1;
	delay_nops(BIT_TIME_IN_NOPS);
	Gpio_WritePin(SW_UART_PIN, (b & 1) ? 1 : 0);
	b >>= 1;
	delay_nops(BIT_TIME_IN_NOPS);
#else
	(void)b;
#endif
} /* send_byte */



INLINE uint8_t IntegertoString(char *string, int number)
{
   if(number == 0) { string[0] = '0'; return 1; };
   int divide = 0;
   int modResult;
   int  length = 0;
   int isNegative = 0;
   int  copyOfNumber;
//   int offset = 0;
   copyOfNumber = number;
   if( number < 0 ) {
     isNegative = 1;
     number = 0 - number;
     length++;
   }
   while(copyOfNumber != 0)
   {
     length++;
     copyOfNumber /= 10;
   }

   for(divide = 0; divide < length; divide++) {
     modResult = number % 10;
     number    = number / 10;
     string[length - (divide + 1)] = modResult + '0';
   }
   if(isNegative) {
   string[0] = '-';
   }
   string[length] = '\0';

   return length;
}

void dbg_pvt(T_TPMS_DATA_MEASURE *pData)
{
#if defined(SG_ENABLE_DEBUG_SPI)
	char m_buf[SPI_BUFFER_MAX_SIZE_TX];
	uint8_t m_len = sizeof(T_TPMS_DATA_MEASURE);

	memset(m_buf, 0x00, SPI_BUFFER_MAX_SIZE_TX); //18-4=14
	m_buf[0] = SG_DBG_FORMAT_ARRAY_PVT;
	m_buf[1] = SP_state.as_field.action_state;
	m_buf[2] = SP_state.as_field.tpms_state;
	memcpy(&m_buf[3], pData, m_len);

	dbg_spi(m_buf, m_len+3);
#else
	(void)pData;
#endif
}

#ifdef SG_ENABLE_DBG_TIMER
void dbg_sp_cnt(T_TPMS_SP_CNT *pData)
{
#if defined(SG_ENABLE_DEBUG_SPI)
	char m_buf[SPI_BUFFER_MAX_SIZE_TX];
	uint8_t m_len = sizeof(T_TPMS_SP_CNT);

	memset(m_buf, 0x00, SPI_BUFFER_MAX_SIZE_TX); //18-4=14
	m_buf[0] = SG_DBG_FORMAT_ARRAY_CNT;
	m_buf[1] = SP_state.as_field.action_state;
	m_buf[2] = SP_state.as_field.tpms_state;
	memcpy(&m_buf[3], pData, m_len);

	dbg_spi(m_buf, m_len+3);
#else
	(void)pData;
#endif
}
#endif

void dbg_word(uint16_t val)
{
#if defined(SG_UART_TX_PIN)
	dbg_byte(val >> 8);
	dbg_byte(val & 0xFF);
#elif defined(SG_ENABLE_DEBUG_SPI)

	//char m_buf[SPI_BUFFER_MAX_SIZE_TX-4];
	char m_buf[12];
	uint8_t m_len;

	//sprintf(m_buf, "%d", val);	// doesnot work !
	m_len = IntegertoString(m_buf, val);

	dbg_spi(m_buf, m_len);

#else
	DEBUG_PUT_WORD(val);
#endif
}


void dbg_dec(int32_t val){
#if defined(SG_UART_TX_PIN)
	dbg_word(val >> 16);
	dbg_word(val & 0xFFFF);
#else
	DEBUG_PUT_WORD(val >> 16);
	DEBUG_PUT_WORD(val & 0xFFFF);
#endif
}

void dbg_hex(int32_t val){
#if defined(SG_UART_TX_PIN)
	char str[12];

	sprintf(str, "0x%x", val);
	dbg_string(str);
#else
	DEBUG_PUT_WORD_HEX(val);
#endif
}

void dbg_string(char *str){
	if(!str)
		return;

#if defined(SG_ENABLE_DEBUG_SPI)
	char m_buf[SPI_BUFFER_MAX_SIZE_TX];
	uint8_t m_len = strlen(str);

	memset(m_buf, 0x00, SPI_BUFFER_MAX_SIZE_TX); //18-4=14
	m_buf[0] = SG_DBG_FORMAT_STRING;
	strcpy(&m_buf[1], str);

	dbg_spi(m_buf, m_len+2);

#elif defined(SG_UART_TX_PIN)
	for(uint8_t ii=0; ii<strlen(str); ii++)
		dbg_byte(str[ii]);

#else
	DEBUG_PUT_STRING(str);
#endif
}
