#ifndef DATATYPES_H
#define DATATYPES_H

#include <stdint.h>
#include <stdbool.h>
#include <platformlib.h>
#include <debug.h>
#include <stdio.h>



//~~~~~~Flag definition~~~~~~
#define SG_ENABLE_SPI_PKG_NEW_FORMAT

//#define SG_ENABLE_DEBUG_SPI		// *debug out interface : SPI
//#define SG_ENABLE_DEBUG_UART	// debug out interface : UART
//#define SG_ENABLE_DEBUG_IO

#ifdef SG_ENABLE_DEBUG_SPI
//	#define SG_ENABLE_DBG_TIMER	// If disabled, will dump PVT info
#endif

#define SG_ENABLE_NEW_MEASURE_REC	// 2023/12/26 for save SP_MEM size
#define SG_ENABLE_MOTION_DET_XZ		// Detection ∆Z axis change to ∆(X/Z) 2 axis for BES-18T.
#if 1
#define SG_ENABLE_MEASURE_CHK_LIMIT_P
#else
#define SG_ENABLE_MEASURE_CHK_LIMIT_V
#define SG_ENABLE_MEASURE_CHK_LIMIT_T
#endif

// Customization
#define SG_CUST_SYSGRATION   0
#define SG_CUST_HENDRICKSON	 1		// BSE-18T
#define SG_CUST_BRIDGESTONE	 2
#define SG_CUST_ARVENTO		 3

#define SG_DEFAULT_CUST      SG_CUST_HENDRICKSON	// BSE-18T code_5
//#define SG_DEFAULT_CUST      SG_CUST_ARVENTO		// BSE-18T code_5 (driving report 1 sec)

// --- Test ---
//#define TEST_ENABLE_DEFAULT_STATE
//#define TEST_ENABLE_USER_REPORT_PERIOD_SEC		(68u)   // 30sec For testing receive rate
//#define TEST_ENABLE_REPORT_USERDATA
//#define TEST_ENABLE_DUMMY_DATA

#if defined(SG_DEFAULT_CUST) && (SG_DEFAULT_CUST == SG_CUST_ARVENTO)
	#define TEST_ENABLE_DRIVE_REPORT_PERIOD_1_SEC			// 1 sec report period for BSE-18T
	#define TEST_ENABLE_USER_REPORT_PERIOD_SEC		(5u)    // 1sec For testing receive rate
#endif	// SG_DEFAULT_CUST == SG_CUST_ARVENTO

#if defined (SG_DEFAULT_CUST) && SG_DEFAULT_CUST == SG_CUST_HENDRICKSON
	#define TEST_ENABLE_DEFAULT_STATE_AUTO_SWITCHx
	//#define TEST_DEFAULT_STATE_AUTO_SWITCH_TIMEOUT_SEC	(4*60u)	// 15 MIN
#endif


#ifdef TEST_ENABLE_DEFAULT_STATE
	#define TEST_ENABLE_DUMMY_DATA
	#define DEFAULT_TPMS_STATE    	STATE_DRIVE	//STATE_STATIONARY	//STATE_DRIVE
#else
	#define DEFAULT_TPMS_STATE    	STATE_STORAGE //STATE_STORAGE
#endif

#if defined(TEST_ENABLE_DRIVE_REPORT_PERIOD_1_SEC)
	#define DEFAULT_DRIVE_MODE_REPORT_PERIOD_STEP_SEC	(1u)	// steps 1 sec
	#define DEFAULT_REPORT_PERIOD_STEP_SEC				(1u)	// steps 1 sec
#else
	#define DEFAULT_DRIVE_MODE_REPORT_PERIOD_STEP_SEC	(5u)	// steps 5 sec
	#define DEFAULT_REPORT_PERIOD_STEP_SEC				(5u)	// steps 5 sec
#endif

#ifdef TEST_ENABLE_USER_REPORT_PERIOD_SEC
	#define DEFAULT_DRIVE_MODE_REPORT_PERIOD_SEC        TEST_ENABLE_USER_REPORT_PERIOD_SEC
	#define DEFAULT_DRIVE_MODE_REPORT_PERIOD_SEC_TRUCK  TEST_ENABLE_USER_REPORT_PERIOD_SEC
#else
	#define DEFAULT_DRIVE_MODE_REPORT_PERIOD_SEC        (60u)     // 60sec
	//#define DEFAULT_DRIVE_MODE_REPORT_PERIOD_SEC_TRUCK  (90u)     // 90sec for Bridgestone
	#define DEFAULT_DRIVE_MODE_REPORT_PERIOD_SEC_TRUCK  (180u)  // 180sec for Sysgration
#endif


// 2024/05/13 timer calibration
//#define SG_ENABLE_TIMER_CALIBRATION		// 5 second only

//#define SG_ENABLE_LF_RX

#define LF_COMPONENT_ENABLE

#ifdef LF_COMPONENT_ENABLE
#define SG_ENABLE_LFRX_MANCHESTER		// Manchester

#ifdef SG_ENABLE_LFRX_MANCHESTER
#define LFRX_HDR_SYSGRATION			1
#define LFRX_HDR_BMW_3_SERIES		2

#define SG_LFRX_HDR_CUSTOMER		LFRX_HDR_BMW_3_SERIES
#define THRESHOLD_LFRX_COUNT_MAX    (10u)	// 4bits <=15

#if (SG_LFRX_HDR_CUSTOMER == LFRX_HDR_BMW_3_SERIES)
	#define SG_LFRX_PAYLOAD_LEN       (2u)
	#define SG_LFRX_HDR_L			0x257F 		// Lowest  16-bit header to be recognized in LFRx with LFRX_HDRLEN
	#define SG_LFRX_HDR_H			0x0000
#else // LFRX_HDR_SYSGRATION
	#define SG_LFRX_PAYLOAD_LEN       (2u)
	#define SG_LFRX_HDR_L			0x257C
	#define SG_LFRX_HDR_H			0x0000
#endif


#endif	//SG_ENABLE_LFRX_MANCHESTER
#endif //LF_COMPONENT_ENABLE
//#define SG_ENABLE_NVRAM			// nvRAM

#if 0 // unused
#define SG_ENABLE_LEARN_MODE		// Learn Drive Mode
#define SG_ENABLE_CONNECTED_MODE	// Connected Mode
#endif


#if defined(SG_ENABLE_DEBUG_UART)
//	#define TEST_ENABLE_WAKEUP_TIMER_MS		WT_PERIOD_2_S
	#define SG_ENABLE_SPI
	#define TEST_ENABLE_RESET_GPIO

#else
	#define SG_ENABLE_SPI
	#define SG_ENABLE_SPI_RX_VERIFY		// Enable SPI RX verify
#endif




#define SG_DEFAULT_CRYSTAL_8MHZ

#define VERSION_SW   0x0B
/*-------------------------------------------
 Customer ID - Version	(APP_VERSION_MINOR)
 x x x       - x x x x x
 */
#if defined (SG_DEFAULT_CUST)
	#define VERSION_CUST   0x00 //SG_DEFAULT_CUST
#else
	#define VERSION_CUST   0x00
#endif

#define APP_VERSION_MAJOR   0x00
#define APP_VERSION_MINOR   ((VERSION_CUST<<5) | VERSION_SW)

#define APP_VERSION         (((APP_VERSION_MAJOR << 8) & 0xFF00) | (APP_VERSION_MINOR & 0x00FF))
#if 0//   (APP_VERSION_MAJOR > 15) || (APP_VERSION_MINOR > 15)
#error "Version numbers should be < 16"
#endif

#define SDK_LIB_VERSION     0x110b      /* SW Library version 1_10_11 */

/* ----------------------------------------------------------------------------
 * The section is used to specify constants in the program memory
 */
#define RODATA_MEM __attribute__ ((section(".rodata,\"a\",@progbits ;"))) volatile




//~~~~~~Main constants and parameters~~~~~~
#define PRESSURE_RANGE_LOW          (100u) // 100 kPa < pressure <1400 kPa
#define PRESSURE_RANGE_HIGH         (900u)
#define PRESSURE_RANGE_HIGH_TRUCK   (1400u)
#define VOLTAGE_RANGE_LOW           (180u) // 1.8 Volt < voltage < 3.6 Volt
#define VOLTAGE_RANGE_HIGH          (360u)
#define TEMPERATURE_RANGE_LOW       (-40)  //  temperature above -40 °C
#define TEMPERATURE_RANGE_HIGH      (125u) // temperature below 125 °C

#if 0	// @2024/06/13
#define PKG_PRESSURE_INVALID        (0x00u) /* Package data for invalid pressure */
#define PKG_PRESSURE_UNDERFLOW      (0x01u) /* PKG code for pressure below 100 kPa */
#else
#define PKG_PRESSURE_INVALID        (0xFFFDu) /* Package data for invalid pressure */
#define PKG_PRESSURE_UNDERFLOW      (0xFFFEu) /* PKG code for pressure below 100 kPa */
#endif
#define PKG_PRESSURE_OVERFLOW       (0xFFFFu) /* VDA code for pressure above 1400/730 kPa */

/* PKG standard uses the next offset to convert the negative temperature range to positive scale */
#define PKG_TEMPERATURE_OFFSET      (52u)   //(55u)
#if 0	// @2024/06/13
#define PKG_TEMPERATURE_INVALID     (0x00u) /* VDA code for invalid temperatute */
#define PKG_TEMPERATURE_UNDERFLOW   (0x01u) /* VDA code for temperature below -40  degC */
#define PKG_TEMPERATURE_OVERFLOW    (0xF8u) /* VDA code for temperature above +125 degC */
#else
#define PKG_TEMPERATURE_INVALID     (0xFDu) /* VDA code for invalid temperatute */
#define PKG_TEMPERATURE_UNDERFLOW   (0xFEu) /* VDA code for temperature below -40  degC */
#define PKG_TEMPERATURE_OVERFLOW    (0xFFu) /* VDA code for temperature above +125 degC */
#endif

#if 0	// @2024/06/13
#define PKG_VOLTAGE_INVALID         (0xFFu) /* Code to report via RF about invalid voltage */
#define PKG_VOLTAGE_UNDERFLOW       (0xF0u) /* Code to report via RF about voltage below 1.8 Volt */
#define PKG_VOLTAGE_OVERFLOW        (0xF8u) /* Code to report via RF about voltage above 3.6 Volt */
#else
#define PKG_VOLTAGE_INVALID         (0xFDu) /* Code to report via RF about invalid voltage */
#define PKG_VOLTAGE_UNDERFLOW       (0xFEu) /* Code to report via RF about voltage below 1.8 Volt */
#define PKG_VOLTAGE_OVERFLOW        (0xFFu) /* Code to report via RF about voltage above 3.6 Volt */
#endif

#define DQA_TESTx

#if defined (SG_DEFAULT_CUST) && SG_DEFAULT_CUST == SG_CUST_HENDRICKSON
#define DEFAULT_ENTRY_STATIONARY_MODE_TIMEOUT_SEC	(3*60u)
#if defined(DQA_TEST)

  #define THRESHOLD_P_STORAGE_LOW              (28u) // PSIG 4 equal 27kPa
  #define THRESHOLD_30_PSIG                   (150u) // 150kPa
  #define THRESHOLD_60_PSIG                   (180u)// 60 PISG equal to 413kPa
  #define THRESHOLD_80_PSIG                   (200u) // 200kPa
  #define RANGE_P_STATIONARY_TO_STORAGE        (21u) // 3PSIG
  #define STATIONARY_TO_STORAGE_10MIN_TIMEOUT  (10u)
  #define RANGE_DELTA_P_MONITORING             (34u)
  #define THRESHOLD_P_NORMAL_TO_MONITORING_15MIN_TIMEOUT  (18u)
  #define THRESHOLD_4_POINT_5_PSIG             (31u)
  #define THRESHOLD_P_MONITORING_TO_NORMAL_TIMEOUT  (36u)//((60*1)/5)

#else

  #define THRESHOLD_P_STORAGE_HIGH     (20u)
  #define RANGE_DELTA_P_MOTION        (208u)
  #define THRESHOLD_P_NORMAL_TO_STANDBY_TIMEOUT           (12u)//((60*1)/5)

  #define THRESHOLD_5_PSIG            (34u)
  //Hendrickson state machine
  #define THRESHOLD_60_PSIG                  (413u)// 60 PISG equal to 413kPa
  #define THRESHOLD_P_STORAGE_LOW             (28u) // PSIG 4 equal 27kPa
  #define THRESHOLD_30_PSIG                  (207u)
  #define THRESHOLD_80_PSIG                  (551u) // 80 PISG equal 551kPa
  #define RANGE_P_STATIONARY_TO_STORAGE       (21u)// 3PSIG
  #define STATIONARY_TO_STORAGE_10MIN_TIMEOUT (10u)//(120u)//(120u)//(120u)//(120u)
  #define RANGE_DELTA_P_MONITORING    (35) //5PSIG
  #define THRESHOLD_P_NORMAL_TO_MONITORING_15MIN_TIMEOUT  (14u)//(15u)//((60*15)/5)//((60*1)/5)
  #define THRESHOLD_P_MONITORING_TO_NORMAL_TIMEOUT  (14u)//delta P15min monitoring to normal((60*15)/5)//((60*1)/5)
  #define THRESHOLD_4_POINT_5_PSIG    (31)
#endif

	// Stationary-Mode auto switch to Starage-Mode when pressure < THRESHOLD_P_STORAGE_LOW in 10 Min
	#define SG_ENABLE_AUTO_ENTRY_STORAGE_MODEx
	#define DEFAULT_ENTRY_STORAGE_MODE_TIMEOUT_SEC	(10*60u)// 10 Min

#else
#define DEFAULT_ENTRY_STATIONARY_MODE_TIMEOUT_SEC	(3*60u)// 3 Min
	#define THRESHOLD_P_STORAGE_HIGH    (150u)	//  150kPa
	#define THRESHOLD_P_STORAGE_LOW     (10u)	//  10Pa
//	#define THRESHOLD_DELTA_P_STATIONARY (15u)	//  100kPa
//	#define THRESHOLD_DELTA_P_MOTION     (15u)	//  100kPa
	#define RANGE_DELTA_P_MOTION         (15u)	// 	50kPa/10s
	#define RANGE_DELTA_P_STATIONARY     (15u)	// 	50kPa/10s
#endif

	#define ACC_Z_MOTION_THRESHOLD_HIGH	 (50u)	// Almost equal 20 km/h
	//#define ACC_Z_SIGNAL_THRESHOLD_DELTA  (15)
	#define ACC_Z_MOTION_THRESHOLD_LOW   (30)//(ACC_Z_MOTION_THRESHOLD_HIGH-ACC_Z_SIGNAL_THRESHOLD_DELTA)
	#define ACC_Z_SIGNAL_UNDERFLOW   	 (5)


#if 0	//org:2024/01/26 for 91805 (BSI-17)
	#define THRESHOLD_LF_COUNT_MAX       	 (3u)	//	180ms*3	//(15u)
#else
//	#define THRESHOLD_LF_COUNT_MAX       	 (10u)	//	MLX91805手晃還是會誤發Trigger
	#define THRESHOLD_LF_COUNT_MAX       	 (15u)	//	180ms*3	//(15u)
#endif





#define ACCELERATION_INVALID        (0x8000u)








//----------------------------------------------------------------------------
//    Data structures of the TPMS states and their properties
//----------------------------------------------------------------------------
/*
typedef enum {
	MODE_NONE                 = 0x00,
	MODE_STATIONARY           = 0x01,  //MODE_STATIONARY
	MODE_CONNECTED            = 0x02,  //MODE_CONNECTED
	MODE_P_VARIATION          = 0x04,  //MODE_P_VARIATION
	MODE_OD_PRESSURE          = 0x08,  //MODE_OD_PRESSURE
	MODE_OD_LOCALIZATION      = 0x10,  //MODE_OD_LOCALIZATION
	MODE_ADVERTISING          = 0x20,  //MODE_ADVERTISING
	MODE_SPECIFIC_PERIOD      = 0x40,  //MODE_SPECIFIC_PERIOD
	MODE_TRIGGER              = 0x80,  //MODE_TRIGGER
#if 0//def SG_ENABLE_DEBUG_SPI
	MODE_DEBUG                = 0xFF,  //MODE_DEBUG
#endif
} tpms_mode_t;
*/

//Main state  machine
typedef enum {

	STATE_STORAGE              = 0x00,	//
	STATE_STATIONARY,			// 1
	STATE_STATIONARY_TFA,    	// 2 Tire fill alert in stationary
	STATE_DRIVE,            	// 3
	STATE_MONITORING,			// 4
	STATE_DRIVE_TFA,         	// 4 Tire fill alert in drive

	STATE_TOTAL_NUMBER,

} tpms_state_t;

typedef enum {
    AS_NONE                    = 0x00,
    AS_TRANSMIT,
    AS_THERMAL_SHUTDOWN,
    AS_RESET,
    AS_REQUEST_INFO,
    AS_REQUEST_PVT,				// 0x05
    AS_REQUEST_ACC,				// 0x06
    AS_REQUEST_G,				// 0x07
	AS_REQUEST_MEASURE,			// 0x08
    AS_ENTRY_TFA,				// 0x09
    AS_TRIGGER,					// 0x0A
	AS_TRIGGER_MANCHESTER,		// 0x0B
//    AS_LEARN,
    AS_SELF_TEST			   = 0x0F,
	AS_SET_DUMMY               = 0x40,
	AS_STATE_CHANGE            = 0x80,
} action_state_t;

#define AS_FLAG_MASK		( 0xFF^AS_STATE_CHANGE )


typedef enum {
    STOPPED,                    /* Wheel is not rotating */
    ROTATING                    /* Wheel is rotating */
} rotation_t;

typedef enum {
	DET_NONE                 = 0x00,
	DET_PVT                  = 0x01,
	DET_G                    = 0x02,
	DET_ACC                  = 0x04
} detected_t;


//----------------------------------------------------------------------------
//    Structure to describe all application states
//----------------------------------------------------------------------------

typedef union {
    struct __attribute__((packed))  StateAsField_s {
        tpms_state_t   tpms_state         : 8;    /* Byte 0 */

        action_state_t action_state       : 8;    /* Byte 1 */
        rotation_t     rotation           : 1;    /* Byte 2 */
//        PressureRange_t pressure_range    : 4;

        uint8_t     Reserved_spmem_bits   : 1;    /* Byte 2 */ //used for SG_ENABLE_AUTO_ENTRY_STORAGE_MODE
    } as_field;

} State_t;


typedef union {
  uint16_t speed;

  struct __attribute__((packed))  {
      uint16_t                  : 8;	/* Byte 1 */
      rotation_t     rotation   : 1;    /* Byte 2 */
      uint16_t                  : 1;
  } ;

} SPEED_t;


typedef struct __attribute__ ((packed)) {
    uint8_t     enabled_tx_rf               : 1;
    uint8_t     enabled_rx_lf               : 1;    // Single bit: "1" enables LFRx reception
    uint8_t     calibration_needed_LFO      : 1;    // Single bit: "1" enables LFO calibration
    uint8_t     calibration_needed_HFO      : 1;    // Single bit: "1" enables HFO calibration
    uint8_t                                 : 4;    // reserved bit field
    uint8_t     det_period_PVT;					    // 8-bit number of periods between PVT measurements (Sec)
    uint8_t     det_period_G;    					// 8-bit number of periods between Motion Detections (Sec)
    uint16_t    report_period_spi;					// 16-bit number of periods between SPI reports (Sec)
    uint8_t     state_keep_timeout_back_state;
    uint16_t    state_keep_timeout;  				// sec
} tpms_state_property_t;


#if defined(TEST_ENABLE_DEFAULT_STATE_AUTO_SWITCH)
typedef struct __attribute__ ((packed)) {
	uint8_t		state_now;
    uint8_t     state_switch_next;
    uint16_t    state_switch_timeout;  				// sec
} tpms_state_auto_switch_t;

#endif


typedef struct  __attribute__((packed)) {
	uint16_t pressure;
	uint16_t voltage;
	uint8_t  temperature;
} T_TPMS_DATA_PVT;

typedef struct  __attribute__((packed)) {
	T_TPMS_DATA_PVT pvt;
	int16_t  acceleration_x;
	int16_t  acceleration_z;
#if 0 //def SG_ENABLE_DBG_TIMER // 2024/01/03
	uint8_t  SP_timer_wakeup_time;
	uint8_t  SP_drive_mode_report_period_spi;
#endif
} T_TPMS_DATA_MEASURE;

#ifdef SG_ENABLE_DBG_TIMER
typedef struct  __attribute__((packed)) {
	uint16_t SP_state_keep_time;				// 狀態維持時間
	uint16_t const_report_period_spi;			// 預設SPI report週期
	uint16_t SP_drive_mode_report_period_spi;	// USER SPI report週期
	uint16_t SP_timer_wakeup_totalTime;			// 目前時間計數
	uint8_t  SP_timer_wakeup_time;				// 目前喚醒時間值
} T_TPMS_SP_CNT;
#endif

#ifdef SG_ENABLE_NEW_MEASURE_REC
typedef struct  __attribute__((packed)) {
	uint16_t pressure;
	int16_t  acceleration_x;
	int16_t  acceleration_z;
} T_TPMS_DATA_MEASURE_REC;
#endif

typedef struct  __attribute__((packed)) {
	uint8_t	 rx_times	: 4;
	uint8_t	 data_len	: 4;
	uint8_t  data[4];
} T_LFRX_MANCHESTER;



typedef struct __attribute__((packed)) {
  uint8_t  length;    // 0x11
  uint8_t  type;      // 0x80(BSI15-A2)
  uint8_t  action;    // len:2
  uint8_t  state;
  uint16_t pressure;
  uint16_t voltage;   // len:7
  uint8_t  temperature;
  int16_t  accel_x;
  int16_t  accel_z;   // len:12
  uint16_t speed;     // len:14
  uint8_t  reserved[2];
  uint8_t  crc;
} T_TPMS_DATA_TX;

typedef struct __attribute__((packed)) {
  uint8_t  length;    // 0x11
  uint8_t  type;      // 0x80(BSI15-A2)
  uint8_t  action;    // len:2
  uint8_t  state;
  uint16_t pressure;
  uint16_t voltage;   // len:7
  uint8_t  temperature;
  int16_t  accel_x;
  int16_t  accel_z;   // len:12
  uint8_t  manchester_cmd[4];// Manchester recived command
  uint8_t  crc;
} T_TPMS_DATA_TX_TRIGGER;

typedef struct __attribute__((packed)) {
  uint8_t  length;        // 0x11
  uint8_t  type;          // 0x80(BSI15-A2)
  uint8_t  state;
  uint8_t  ID[4];         // len:6
  uint8_t  version_sw[2]; // [0]:major, [1]:minor
  uint8_t  pressure_range;
  uint8_t  lfrx_hdr_len;  // len:10
  uint16_t lfrx_hdr[2];	  // [0]:hdr_l [1]:hdr_h
  uint8_t  reserved[2];
  uint8_t  crc;
} T_TPMS_DATA_TX_INFO;



typedef struct  __attribute__((packed)) {
  uint8_t  length;    // 0x05
  uint8_t  type;      // 0x01(BSI15/BSI03P)		0x10(ExtendCmd)
  uint8_t  mode;      //						cmd
  uint8_t  data[2];   //
  uint8_t  crc;
} T_TPMS_DATA_RX;



// Circular Queue Structure
typedef struct __attribute__((packed)){
	uint8_t events[5];    // 5 bytes - event storage array
    uint8_t head;         // 1 byte - head index (0-4)
    uint8_t tail;         // 1 byte - tail index (0-4)
    uint8_t count;        // 1 byte - current number of elements
} CircularEventQueue;


/* ----------------------------------------------------------------------------
 * Scratchpad data
 * The Scratchpad memory (47 Bytes) is a special part of the Registers. It’s built on flops keeping own
 *	content in the sleep modes.
 */

extern SP_MEM State_t SP_state;							// 3 Bytes
#ifdef SG_ENABLE_NEW_MEASURE_REC
	extern SP_MEM T_TPMS_DATA_MEASURE_REC SP_measured_rec;	// 6 Bytes
#else
	extern SP_MEM T_TPMS_DATA_MEASURE SP_measured_rec;		// 5+4 Bytes
#endif

extern SP_MEM uint8_t   SP_timer_wakeup_time;			// 1 Bytes
extern SP_MEM uint16_t  SP_timer_wakeup_totalTime;		// 2 Bytes
extern SP_MEM uint8_t   SP_lf_count;					// 1 Bytes
extern SP_MEM uint16_t  SP_state_keep_time;				// 2 Bytes

#if 0	// org
	extern SP_MEM uint8_t   SP_drive_mode_report_period_spi;// 1 Bytes
#else	// 2023/12/08
	extern SP_MEM uint16_t   SP_drive_mode_report_period_spi;// 2 Bytes
#endif


// Dummy data for testing
#ifdef TEST_ENABLE_DUMMY_DATA
	extern SP_MEM uint8_t  SP_dummy_enabled_flag;		// 1 Bytes

#endif
/* ----------------------------------------------------------------------------
 * Global variables in RAM
 */

extern DP_MEM T_TPMS_DATA_MEASURE measured;

extern DP_MEM detected_t measurements_valid;

// LFRX Manchester code
#if 0
	extern DP_MEM uint8_t gLfRx_buffer[8];
#else
	extern SP_MEM T_LFRX_MANCHESTER SP_lfrx_rec;			// 5 Bytes
#endif

#endif /* DATATYPES_H */
