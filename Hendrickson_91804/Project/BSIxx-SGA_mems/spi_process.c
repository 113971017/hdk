#include <platformlib.h>
#include <sensorlib.h>
#include <string.h>
#include "spi_process.h"
#include "measurements.h"
#include "datatypes.h"

#include "main.h"
#include "sg_lf.h"
#include "sg_common.h"
#include "processing.h"
#include "sg_dbg.h"

#define SPI_TIMEOUT_MS_MOSI          150	// BLE wake up : 110 ms,	org:350
#define SPI_TIMEOUT_MS_MISO          80		// MLX wake up : 55 ms,		org:300
#define DEFAULT_MOSI_WAKEUP_TIME_US  1000
//#define DEFAULT_SPI_CLK_RISE_DELAY_TIME_US 10
//#define DEFAULT_SPI_CLK_FALL_DELAY_TIME_US 10



// GPIO execution time from datasheet
#if defined(SG_DEFAULT_CRYSTAL_8MHZ)

	#define SG_EXE_TIME_GPIO_init_CLK		63
	#define SG_EXE_TIME_GPIO_READPIN_CLK	60		// GPIO0/1	57 - 60
													// GPIO2	72 - 75
													// GPIO3	87 - 90
	#define SG_EXE_TIME_GPIO_READPORT_CLK	21
	#define SG_EXE_TIME_GPIO_W_CLK	120				// 97 - 124

	#define SG_SPI_CLK_DELAY_FRQ_CLK			40	// 95UL
	#define SG_SPI_CLK_DELAY_CHG_GPIO_FRQ_CLK	5	// 55UL
#else
#endif




#define WAKEUP_BLE() do{ \
			Gpio_SetPinMode(SPI_MOSI, GPIO_DIG_OUTPUT_0); \
			DELAY_US(2); \
			Gpio_WritePin(SPI_MOSI, 1); \
			DELAY_US(DEFAULT_MOSI_WAKEUP_TIME_US); \
			Gpio_WritePin(SPI_MOSI, 0); \
			DELAY_US(DEFAULT_MOSI_WAKEUP_TIME_US); \
		} while (0)


static uint8_t buffer_rx[SPI_BUFFER_MAX_SIZE_RX];
static uint8_t buffer_tx[SPI_BUFFER_MAX_SIZE_TX];







bool SPI_OUT_ENABLE(void)
{
	uint16_t waitack_count = 0;
	uint8_t	ackcalc_count = 0;

	WAKEUP_BLE();


    Gpio_WritePin(SPI_MOSI, 1);
	for(waitack_count=0; waitack_count<SPI_TIMEOUT_MS_MOSI; waitack_count++){

		if(Gpio_ReadPin(SPI_MISO)){
			ackcalc_count++;
			if(ackcalc_count > 3)
			{
				ackcalc_count = 0;
				Gpio_WritePin(SPI_MOSI, 0);
				while(Gpio_ReadPin(SPI_MISO)){
					if (AbsWatchdog_IsWindowOpen()){
						AbsWatchdog_Restart();
					}
					ackcalc_count++;
					DELAY_US(DEFAULT_MOSI_WAKEUP_TIME_US);

					if(ackcalc_count > 20)
					{
						return 0;
					}
				}

				delay_ms(1u);
				return 1;
			}
		}
		else {
			if (AbsWatchdog_IsWindowOpen()){
				AbsWatchdog_Restart();
			}

			ackcalc_count = 0;
		}

		DELAY_US(DEFAULT_MOSI_WAKEUP_TIME_US);


	}
    Gpio_WritePin(SPI_MOSI, 0);

	return 0;
}

void SPI_WRITE_BYTES(uint8_t b)
{
	int8_t i;
	for(i=7;i>=0;i--){
		Gpio_WritePin(SPI_CLK, 0);
		Gpio_WritePin(SPI_MOSI, b&(1<<i)?1:0);
		//delay_nops(SG_SPI_CLK_DELAY_CHG_GPIO_FRQ_CLK);
		Gpio_WritePin(SPI_CLK, 1);
		delay_nops(SG_SPI_CLK_DELAY_FRQ_CLK);
	}
}

void SPI_WRITE(uint8_t *buf, uint8_t len)
{
	uint8_t i;

	if(!buf){
		return;
	}

	if(SPI_OUT_ENABLE()){
		for(i=0;i<len;i++){
			SPI_WRITE_BYTES(buf[i]);
		}

		//DELAY_US(DEFAULT_MOSI_WAKEUP_TIME_US);
		delay_nops(SG_SPI_CLK_DELAY_FRQ_CLK);

		Gpio_WritePin(SPI_CLK, 0);
		Gpio_WritePin(SPI_MOSI, 0);
	}
	Gpio_SetPinMode(SPI_MOSI, GPIO_DIG_INPUT_WITH_PULLDOWN);
	Gpio_SetPinMode(SPI_CLK, GPIO_DIG_INPUT_WITH_PULLDOWN);
}

bool SPI_R_CHECK(void)
{
	uint16_t count = 0;
	uint8_t	fail_count = 0;

	Gpio_SetPinMode(SPI_MOSI, GPIO_DIG_OUTPUT_0);
	Gpio_SetPinMode(SPI_CLK, GPIO_DIG_OUTPUT_0);

	DELAY_US(DEFAULT_MOSI_WAKEUP_TIME_US);
	while(Gpio_ReadPin(SPI_MISO)){
		count++;
		DELAY_US(DEFAULT_MOSI_WAKEUP_TIME_US);

    	if (AbsWatchdog_IsWindowOpen()){
    		AbsWatchdog_Restart();
    	}
		if(count > SPI_TIMEOUT_MS_MISO){
			return 0;
		}else if(count > 50){
			Gpio_WritePin(SPI_MOSI, 1);
			while(Gpio_ReadPin(SPI_MISO)){
				fail_count++;

				DELAY_US(DEFAULT_MOSI_WAKEUP_TIME_US);


				if(fail_count > 50){
					Gpio_WritePin(SPI_MOSI, 0);
					return 0;
				}
			}
			Gpio_WritePin(SPI_MOSI, 0);
			//delay_ms(6u);
			delay_ms(1u);
			return 1;
		}
	}
	return 0;
}

#if defined(SG_ENABLE_SPI_RX_VERIFY)
uint8_t SPI_READ_BYTES_verify(uint8_t tx)
#else
uint8_t SPI_READ_BYTES(void)
#endif
{
	uint8_t b;
	int8_t i;
	b = 0;
	for(i=0;i<8;i++){
		Gpio_WritePin(SPI_CLK, 0);
#if defined(SG_ENABLE_SPI_RX_VERIFY)
		Gpio_WritePin(SPI_MOSI, (tx>>(7-i))&0x01 );
		//delay_nops(SG_SPI_CLK_DELAY_CHG_GPIO_FRQ_CLK);
#else
		delay_nops(SG_SPI_CLK_DELAY_FRQ_CLK);
#endif
		Gpio_WritePin(SPI_CLK, 1);
		b = (b<<1) | Gpio_ReadPin(SPI_MISO);
		delay_nops(SG_SPI_CLK_DELAY_CHG_GPIO_FRQ_CLK);
	}
	return (b);
}

//void SPI_READ(uint8_t *buf, uint8_t len)
bool SPI_READ(void)
{
	memset(buffer_rx, 0x00, SPI_BUFFER_MAX_SIZE_RX);
	if(!SPI_R_CHECK()){
		Gpio_SetPinMode(SPI_MOSI, GPIO_DIG_INPUT_WITH_PULLDOWN);
		Gpio_SetPinMode(SPI_CLK, GPIO_DIG_INPUT_WITH_PULLDOWN);

		return false;
	}

	for(uint8_t i=0; i<SPI_BUFFER_MAX_SIZE_RX; i++){
#if defined(SG_ENABLE_SPI_RX_VERIFY)
		buffer_rx[i] = SPI_READ_BYTES_verify( (i>0)? buffer_rx[i-1]:0 );
#else
		buffer_rx[i] = SPI_READ_BYTES();
#endif
	}

	delay_nops(SG_SPI_CLK_DELAY_FRQ_CLK);
	Gpio_WritePin(SPI_CLK, 0);


	Gpio_SetPinMode(SPI_MOSI, GPIO_DIG_INPUT_WITH_PULLDOWN);
	Gpio_SetPinMode(SPI_CLK, GPIO_DIG_INPUT_WITH_PULLDOWN);

	if((buffer_rx[5] == CalcCRC8(buffer_rx, SPI_BUFFER_MAX_SIZE_RX-2)) )
	  return true;
	else
	  return false;
}


static const uint8_t crc_table[] =
{
    0x00,0x07,0x0E,0x09,0x1C,0x1B,0x12,0x15,0x38,0x3F,0x36,0x31,0x24,0x23,0x2A,0x2D,
    0x70,0x77,0x7E,0x79,0x6C,0x6B,0x62,0x65,0x48,0x4F,0x46,0x41,0x54,0x53,0x5A,0x5D,
    0xE0,0xE7,0xEE,0xE9,0xFC,0xFB,0xF2,0xF5,0xD8,0xDF,0xD6,0xD1,0xC4,0xC3,0xCA,0xCD,
    0x90,0x97,0x9E,0x99,0x8C,0x8B,0x82,0x85,0xA8,0xAF,0xA6,0xA1,0xB4,0xB3,0xBA,0xBD,
    0xC7,0xC0,0xC9,0xCE,0xDB,0xDC,0xD5,0xD2,0xFF,0xF8,0xF1,0xF6,0xE3,0xE4,0xED,0xEA,
    0xB7,0xB0,0xB9,0xBE,0xAB,0xAC,0xA5,0xA2,0x8F,0x88,0x81,0x86,0x93,0x94,0x9D,0x9A,
    0x27,0x20,0x29,0x2E,0x3B,0x3C,0x35,0x32,0x1F,0x18,0x11,0x16,0x03,0x04,0x0D,0x0A,
    0x57,0x50,0x59,0x5E,0x4B,0x4C,0x45,0x42,0x6F,0x68,0x61,0x66,0x73,0x74,0x7D,0x7A,
    0x89,0x8E,0x87,0x80,0x95,0x92,0x9B,0x9C,0xB1,0xB6,0xBF,0xB8,0xAD,0xAA,0xA3,0xA4,
    0xF9,0xFE,0xF7,0xF0,0xE5,0xE2,0xEB,0xEC,0xC1,0xC6,0xCF,0xC8,0xDD,0xDA,0xD3,0xD4,
    0x69,0x6E,0x67,0x60,0x75,0x72,0x7B,0x7C,0x51,0x56,0x5F,0x58,0x4D,0x4A,0x43,0x44,
    0x19,0x1E,0x17,0x10,0x05,0x02,0x0B,0x0C,0x21,0x26,0x2F,0x28,0x3D,0x3A,0x33,0x34,
    0x4E,0x49,0x40,0x47,0x52,0x55,0x5C,0x5B,0x76,0x71,0x78,0x7F,0x6A,0x6D,0x64,0x63,
    0x3E,0x39,0x30,0x37,0x22,0x25,0x2C,0x2B,0x06,0x01,0x08,0x0F,0x1A,0x1D,0x14,0x13,
    0xAE,0xA9,0xA0,0xA7,0xB2,0xB5,0xBC,0xBB,0x96,0x91,0x98,0x9F,0x8A,0x8D,0x84,0x83,
    0xDE,0xD9,0xD0,0xD7,0xC2,0xC5,0xCC,0xCB,0xE6,0xE1,0xE8,0xEF,0xFA,0xFD,0xF4,0xF3
};


uint8_t CalcCRC8(uint8_t *ptr, uint8_t size){
    uint8_t crc = 0x00;

    while (size--) {
        crc = crc_table[crc ^ *ptr++];
    }
    return crc;
}


void dbg_spi(uint8_t *pBuf, uint8_t bufLen)
{
#ifdef SG_ENABLE_DEBUG_SPI
	uint8_t m_len = bufLen;//strlen(pBuf);
	//uint16_t deltaZ;
	uint8_t m_idx = 0;

	if( m_len > (SPI_BUFFER_MAX_SIZE_TX-4) )
		m_len = (SPI_BUFFER_MAX_SIZE_TX-4);

	buffer_tx[m_idx]   = 0x11;
	buffer_tx[++m_idx] = TPMS_TYPE_SENSOR_TO_BLE_DBG;

	memcpy( &buffer_tx[++m_idx], pBuf, m_len );
	for(uint8_t ii=m_len+m_idx; ii<SPI_BUFFER_MAX_SIZE_TX; ii++){
		buffer_tx[ii] = 0x00;
	}

	buffer_tx[m_idx+m_len]   = UINT16_TO_BYTE0(SP_timer_wakeup_totalTime); //SP_timer_wakeup_totalTime
	buffer_tx[m_idx+m_len+1] = UINT16_TO_BYTE1(SP_timer_wakeup_totalTime);


	buffer_tx[SPI_BUFFER_MAX_SIZE_TX] = CalcCRC8(buffer_tx, SPI_BUFFER_MAX_SIZE_TX-1);

	SPI_WRITE(buffer_tx, SPI_BUFFER_MAX_SIZE_TX);
#else
	(void)pBuf;
	(void)bufLen;
#endif
}

void dbg_str(uint8_t *pBuf)
{
	dbg_spi(pBuf, strlen(pBuf));
}


void Process_spi_buf_tx(action_state_t action)
{
	memset(buffer_tx, 0x00, SPI_BUFFER_MAX_SIZE_TX);

	if(action == AS_REQUEST_INFO) {
		T_TPMS_DATA_TX_INFO *pBuf = (T_TPMS_DATA_TX_INFO*) buffer_tx;

		pBuf->length = 0x11;
		pBuf->type  = TPMS_TYPE_SENSOR_TO_BLE_INFO;
		pBuf->state = SP_state.as_field.tpms_state;
		pBuf->ID[0] = Sys_GetChipId().as_byte[3];
		pBuf->ID[1] = Sys_GetChipId().as_byte[2];
		pBuf->ID[2] = Sys_GetChipId().as_byte[1];
		pBuf->ID[3] = Sys_GetChipId().as_byte[0];

		pBuf->version_sw[0] = APP_VERSION_MAJOR;
		pBuf->version_sw[1] = APP_VERSION_MINOR;
		//pBuf->pressure_range = SP_state.as_field.pressure_range;
		pBuf->pressure_range = get_pressgre_range();

#ifdef LF_COMPONENT_ENABLE
		pBuf->lfrx_hdr_len = SG_LFRX_PAYLOAD_LEN;
		pBuf->lfrx_hdr[0] = SG_LFRX_HDR_L;
		pBuf->lfrx_hdr[1] = SG_LFRX_HDR_H;
#else
		pBuf->lfrx_hdr_len = 0;
		pBuf->lfrx_hdr[0] = 0;
		pBuf->lfrx_hdr[1] = 0;
#endif

		pBuf->crc = CalcCRC8((uint8_t*)pBuf, pBuf->length);
	}
	else if(action == AS_TRIGGER ||	action == AS_TRIGGER_MANCHESTER) {
		T_TPMS_DATA_TX_TRIGGER *pBuf = (T_TPMS_DATA_TX_TRIGGER*) buffer_tx;

		pBuf->length = 0x11;
		pBuf->type  = TPMS_TYPE_SENSOR_TO_BLE_NEW;
		pBuf->action = SP_state.as_field.action_state;
		pBuf->state = SP_state.as_field.tpms_state;

		pBuf->pressure = measured.pvt.pressure;
		pBuf->voltage = measured.pvt.voltage;
		pBuf->temperature = measured.pvt.temperature;
		pBuf->accel_x = measured.acceleration_x;
		pBuf->accel_z = measured.acceleration_z;

		if( action == AS_TRIGGER_MANCHESTER ){
			memcpy(pBuf->manchester_cmd ,SP_lfrx_rec.data, 4);
		}
		else{
			memset(pBuf->manchester_cmd ,0x00, 4);
		}

		pBuf->crc = CalcCRC8((uint8_t*)pBuf, pBuf->length);

		return ;
#ifndef LF_COMPONENT_ENABLE
		return ;
#endif

#if(0)
		if( pBuf->pressure == 0 &&
#if 0 // org:2024/01/31
			(SP_state.as_field.tpms_state == STATE_STATIONARY ||
			 SP_state.as_field.tpms_state == STATE_STATIONARY_TFA) ){
#else
			SP_state.as_field.tpms_state == STATE_STATIONARY ){
#endif
			pBuf->state = STATE_STORAGE;
			SP_state.as_field.tpms_state = STATE_STORAGE;
		}
#endif

		//SP_timer_wakeup_totalTime = 0;
	}
	else {
		T_TPMS_DATA_TX *pBuf = (T_TPMS_DATA_TX*) buffer_tx;

		pBuf->length = 0x11;
		pBuf->type  = TPMS_TYPE_SENSOR_TO_BLE_NEW;
		if(action == AS_REQUEST_MEASURE) {
			pBuf->action = AS_REQUEST_MEASURE;
		}
		else {
			pBuf->action = SP_state.as_field.action_state;
		}
		pBuf->state = SP_state.as_field.tpms_state;
		//pBuf->pressure = (measured.pvt.pressure<PRESSURE_RANGE_LOW)? 0:(measured.pvt.pressure-PRESSURE_RANGE_LOW);
		//pBuf->pressure = get_cmd_pressure( measured.pvt.pressure );
		pBuf->pressure = measured.pvt.pressure;
		pBuf->voltage = measured.pvt.voltage;
		pBuf->temperature = measured.pvt.temperature;
		pBuf->accel_x = measured.acceleration_x;
		pBuf->accel_z = measured.acceleration_z;

#if 0	//org
		pBuf->speed = ((uint16_t)SP_WAL_Result<<8);
#else
		SPEED_t m_tmp;

		m_tmp.speed		= 0;	// clean

		m_tmp.rotation 	= SP_state.as_field.rotation;

		pBuf->speed 	= m_tmp.speed;
		//pBuf->speed = ((uint16_t)SP_state.as_field.dir <<13) | (SP_state.as_field.rotation<<4) | SP_state.as_field.leftright;
#endif
		pBuf->crc = CalcCRC8((uint8_t*)pBuf, pBuf->length);
	}
	delay_ms(5);

	SPI_WRITE(buffer_tx, SPI_BUFFER_MAX_SIZE_TX);
}


void Process_spi_buf_rx(void)
{
	T_TPMS_DATA_RX *pBuf = (T_TPMS_DATA_RX*) buffer_rx;

	if( pBuf->type == TPMS_TYPE_EXT ){ // BSI-15-BC
		switch( pBuf->mode&0x7F ) {
		case EXT_CMD_STATE:
			SP_state.as_field.tpms_state = pBuf->data[0];
			break;
		case EXT_CMD_MEASURE:
			//if(pBuf->mode&0x80) // 'W'
			//else				  // 'R'
			if( pBuf->data[0] == STATE_STORAGE ) {
				SP_state.as_field.action_state = AS_REQUEST_MEASURE;
			}
			else {
				SP_state.as_field.action_state = AS_TRIGGER;
			}

			if( pBuf->data[1] != 0 )
			{
//				SP_drive_mode_report_period_spi = (pBuf->data[1]*DEFAULT_DRIVE_MODE_REPORT_PERIOD_STEP_SEC);
#if defined(SG_DEFAULT_CUST) && (SG_DEFAULT_CUST == SG_CUST_ARVENTO)
				SP_drive_mode_report_period_spi = DEFAULT_DRIVE_MODE_REPORT_PERIOD_STEP_SEC/DEFAULT_REPORT_PERIOD_STEP_SEC;
#else
				SP_drive_mode_report_period_spi = ((uint16_t)pBuf->data[1]*DEFAULT_DRIVE_MODE_REPORT_PERIOD_STEP_SEC)/DEFAULT_REPORT_PERIOD_STEP_SEC;
#endif
			}

			break;
		case EXT_CMD_INFO:
			//if(pBuf->mode&0x80) // 'W'
			//else				  // 'R'
			SP_state.as_field.action_state = AS_REQUEST_INFO;
			//Sys_ResetCpu();
			break;
#ifdef TEST_ENABLE_DUMMY_DATA
		case EXT_CMD_DUMMY_PVT:
			measured.pvt.pressure = (uint16_t)(pBuf->data[0]<<8 | pBuf->data[1]);
			measured.pvt.voltage  = (uint16_t)(pBuf->data[2]<<8 | pBuf->data[3]);
			measured.pvt.temperature = pBuf->data[4];
			SP_state.as_field.action_state = AS_SET_DUMMY;
			break;
		case EXT_CMD_DUMMY_ACC_XZ:
			measured.acceleration_x = (uint16_t)(pBuf->data[0]<<8 | pBuf->data[1]);
			measured.acceleration_z = (uint16_t)(pBuf->data[2]<<8 | pBuf->data[3]);
			SP_state.as_field.action_state = AS_SET_DUMMY;
			break;
		case EXT_CMD_DUMMY_P:
			measured.pvt.pressure = (uint16_t)(pBuf->data[0]<<8 | pBuf->data[1]);
			SP_state.as_field.action_state = AS_SET_DUMMY;
			break;
		case EXT_CMD_DUMMY_V:
			measured.pvt.voltage = (uint16_t)(pBuf->data[0]<<8 | pBuf->data[1]);
			SP_state.as_field.action_state = AS_SET_DUMMY;
			break;
		case EXT_CMD_DUMMY_T:
			measured.pvt.temperature = pBuf->data[1];
			SP_state.as_field.action_state = AS_SET_DUMMY;
			break;
		case EXT_CMD_DUMMY_ACC_X:
			measured.acceleration_x = (uint16_t)(pBuf->data[0]<<8 | pBuf->data[1]);
			SP_state.as_field.action_state = AS_SET_DUMMY;
			break;
		case EXT_CMD_DUMMY_ACC_Z:
			measured.acceleration_z = (uint16_t)(pBuf->data[0]<<8 | pBuf->data[1]);
			SP_state.as_field.action_state = AS_SET_DUMMY;
			break;
		case EXT_CMD_DBG_FLAG:
			SP_dummy_enabled_flag = pBuf->data[0];
			break;
#endif
		case EXT_CMD_RESET:
			init_default_value();
			Sys_ResetCpu();
			break;
		case EXT_CMD_SELFTEST:
			break;
		default:
			break;
		}

	}



}

