#ifndef SPI_PROCESS_H
#define SPI_PROCESS_H
#include "datatypes.h"


// size of transmission and reception buffers
#define SPI_BUFFER_MAX_SIZE_RX       (0x05+2)
#define SPI_BUFFER_MAX_SIZE_TX       (0x11+1)

enum{
#if 0
  PKG_TYPE_RX          = 0x01,
  PKG_TYPE_TX          = 0x02,
  PKG_TYPE_TX_NEW      = 0x04,
  PKG_TYPE_TX_NEW_INFO = 0x08,
#else
  TPMS_TYPE_BLE_TO_SENSOR      = 0x01,
  TPMS_TYPE_SENSOR_TO_BLE      = 0x02,
  TPMS_TYPE_SENSOR_TO_BLE_NEW  = 0x03,
  TPMS_TYPE_SENSOR_TO_BLE_INFO = 0x04,
#endif
  TPMS_TYPE_EXT                = 0x10,
  TPMS_TYPE_SENSOR_TO_BLE_DBG  = 0x80,
};

enum{
  EXT_CMD_READ         = 0x00,
  EXT_CMD_WRITE        = 0x80
};

enum{
  DBG_FLAG_NONE        = 0x00,
  DBG_FLAG_DUMMY       = 0x01,	// Enable dummy value
  DBG_FLAG_SPI         = 0x02,	// Enable SPI debug output
  DBG_FLAG_RF          = 0x04,	// Enable RF debug output
};

typedef enum {
  EXT_CMD_NONE         = 0x00,
#if 0
  EXT_CMD_VERSION      ,
  EXT_CMD_ID           ,
#else
  EXT_CMD_INFO         ,
#endif
  EXT_CMD_MEASURE      ,	// PVT, ACC, G
  EXT_CMD_STATE        ,
  EXT_CMD_OOK_DATA0    ,
  EXT_CMD_OOK_DATA1    ,
  EXT_CMD_PE_DATA0     ,  // LF Manchester wake up code
  EXT_CMD_PE_DATA1     ,

  EXT_CMD_DUMMY_PVT    = 0x10,
  EXT_CMD_DUMMY_P      ,
  EXT_CMD_DUMMY_V      ,
  EXT_CMD_DUMMY_T      ,

  EXT_CMD_DUMMY_ACC    = 0x20,
  EXT_CMD_DUMMY_ACC_X  ,
  EXT_CMD_DUMMY_ACC_Z  ,
  EXT_CMD_DUMMY_ACC_XZ ,

  EXT_CMD_DBG_FLAG     = 0x30,
  EXT_CMD_RESET        ,
  EXT_CMD_SELFTEST
}T_EXT_CMD;




#define SPI_CLK  	GPIO2
#define SPI_MOSI 	GPIO1
#define SPI_MISO 	GPIO0


void SPI_WRITE(uint8_t *buf, uint8_t len);
bool SPI_READ(void);

void Process_spi_buf_rx(void);
void Process_spi_buf_tx(action_state_t action);


uint8_t CalcCRC8(uint8_t *ptr, uint8_t size);

extern void dbg_spi(uint8_t *pBuf, uint8_t bufLen);
void dbg_str(uint8_t *pBuf);

#endif //SPI_PROCESS_H
