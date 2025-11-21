#include <sensorlib.h>
#include <platformlib.h>
#include <stdlib.h>
// Add by Ian @20230330 (5D_CRC-66806_2023-03-29.pdf)
#include <io.h>
#include <string.h>

#include "datatypes.h"
#include "WuTimerC.h"

#include "measurements.h"
#include "main.h"
#include "sg_lf.h"
#if defined(SG_ENABLE_SPI)
#include "spi_process.h"
#endif

#include "sg_dbg.h"
#include "sg_common.h"
#include "processing.h"



SP_MEM State_t  SP_state;

SP_MEM uint8_t SP_lf_count = 0;

SP_MEM uint16_t SP_state_keep_time;

// Dummy data for testing
#ifdef TEST_ENABLE_DUMMY_DATA
SP_MEM uint8_t  SP_dummy_enabled_flag;

#endif

static INLINE void update_measurements(void);

#ifdef LF_COMPONENT_ENABLE
static INLINE void Process_LF_Message(void);
#endif

DP_MEM detected_t measurements_valid;







uint16_t F_Xdir;		// flag of direction result

#if defined(SG_ENABLE_NVRAM)
#define RFTx_size                    (30u)       /* Payload size, max value 32 bytes */
/*
 * NVRAM memory parameters, used as RAM memory parameters
 */
uint16_t  NVRAM_MEM  RFTx_buffer[RFTx_size]; /* 00-29 */
uint16_t  NVRAM_MEM  reserved_nvram[2];      /* 30-31,  one more byte can be used here if needed */

#endif

const uint16_t fw_version[4] __attribute__((section(".myBufSection"))) = {0x2201, 0x04A8, 0x9042, APP_VERSION};

static INLINE void init_io_ports(void);
static INLINE void reset_io_ports(void);

// MAIN function
int main ( void )
{
	if (Sys_IsColdBoot()) {
		init_io_ports();

        #ifdef LF_COMPONENT_ENABLE
		init_lf();
        #endif

		// Add by Ian @20230330 (5D_CRC-66806_2023-03-29.pdf)
		IO_BYTE_HOST(CUSTOM_91804, VDIG_SHORT) = 0x0Cu;

		init_default_value();
	}
	else if (Itc_IsPending(ITC_WUPTMR)) { // Wake up By Wake-up-Timer
		WakeupTimer_IntClear();
		NvRam_Recall();  //added to ensure nvRAM is always recalled


		if( SP_state.as_field.action_state == AS_TRIGGER_MANCHESTER ){
			Itc_Disable(ITC_GPIO0);
		}
		else if( SP_lf_count > THRESHOLD_LF_COUNT_MAX ) {
			//SP_lf_count = 0;
			Itc_Disable(ITC_GPIO0);
            #ifdef LF_COMPONENT_ENABLE
			SP_state.as_field.action_state = AS_NONE;
			//SP_state.as_field.action_state = AS_TRIGGER;
            #else
			SP_state.as_field.action_state = AS_NONE;
            #endif



		}
		else
		{
			  SP_timer_wakeup_totalTime += SP_timer_wakeup_time;
		      SP_state_keep_time += SP_timer_wakeup_time;
			  SP_state.as_field.action_state = AS_NONE;

		}
		SP_lf_count = 0;



		update_measurements();
	}
	else if (Itc_IsPending(ITC_GPIO0)){ // Wake up By SPI MISO
		Itc_ClearPending(ITC_GPIO0);

		Itc_Disable(ITC_GPIO0);
		//dbg("WU.SPI");
		update_measurements();

	  #if defined(SG_ENABLE_SPI)
		if( SPI_READ() ){
			NvRam_Recall();
			Process_spi_buf_rx();
		}
	  #endif
	}
#ifdef SG_ENABLE_LFRX_MANCHESTER
	else if (Itc_IsPending(ITC_LFRX_DONE)) {	// Wake up By LF
		//Process_LF_Message();
        ;
	}
#endif

#ifdef LF_COMPONENT_ENABLE
	else if (Itc_IsPending(ITC_LFRX_TIMEOUT)) { // Wake up By LF timeout


		Itc_ClearPending(ITC_LFRX_TIMEOUT);

		Itc_Disable(ITC_LFRX_TIMEOUT);
		Itc_Disable(ITC_GPIO0);
		//Wakeup_Disable(WAKEUP_GPIO0_HIGH);

		SP_lf_count++;
		if(SP_lf_count > THRESHOLD_LF_COUNT_MAX ) {
			WakeupTimer_Restart(WT_PERIOD_50_MS);
		}

		LfRx_FifoFlush();
		Itc_Enable(ITC_LFRX_TIMEOUT);
		Sys_DeepSleepWith(LF_RECEIVE);

		#if defined(TEST_ENABLE_RESET_GPIO)
		reset_io_ports();
		#endif
	}
#endif
	else { // unexpected wrong wake-up
		if(SP_state.as_field.action_state != AS_REQUEST_INFO) {
			//dbg("WU.Others");
			//dbg_string("WU.Others");
			Sys_ResetCpu();
		}
	}


#if defined(SG_ENABLE_SPI)
  Itc_Disable(ITC_GPIO0);
#endif

  if(SP_state.as_field.action_state != AS_REQUEST_INFO) {
	  TPMS_state_Measurements();

	  //TPMS_state_Motion_Detection();

	  TPMS_state_processing_and_update();



  }
  //2024/01/04
  SP_lf_count = 0;
  TPMS_state_processing_end();
  return 0;
}



static INLINE void init_io_ports(void)
{
	//DBG("init_io_ports\n");
#if defined(SG_ENABLE_SPI)
    static const GpioConfig_Type cfg_io =
    {{
        .GPIO1_DRV = 1, /* Available levels:  0, 1, 2 and 3 */
        .GPIO2_DRV = 1,
        /* GPIOs initial states: */
        .GPIO0_CH_SEL = GPIO_DIG_INPUT_WITH_PULLDOWN,
        .GPIO1_CH_SEL = GPIO_DIG_INPUT_WITH_PULLDOWN,
        .GPIO2_CH_SEL = GPIO_DIG_INPUT_WITH_PULLDOWN,
#if 1 // org
        .GPIO3_CH_SEL = GPIO_DISABLED   /* Note: GPIO3 is not available */
#else
		.GPIO3_CH_SEL = GPIO_DIG_INPUT_WITH_PULLDOWN,
#endif
    }};
    Gpio_Init(&cfg_io);
#if defined(SG_ENABLE_SPI)
    Wakeup_Enable(WAKEUP_GPIO0_HIGH);
    Itc_Enable(ITC_GPIO0);
#endif
#else
    static const GpioConfig_Type cfg_io =
    {{
//        .GPIO0_DRV = 1, /* Output driving strength settings */
        .GPIO1_DRV = 1, /* Available levels:  0, 1, 2 and 3 */
        .GPIO2_DRV = 1,
//        .GPIO3_DRV = 2,
        /* GPIOs initial states: */
        .GPIO0_CH_SEL = GPIO_DIG_INPUT_WITH_PULLDOWN,
        .GPIO1_CH_SEL = GPIO_DIG_INPUT_WITH_PULLDOWN,
        .GPIO2_CH_SEL = GPIO_DIG_INPUT_WITH_PULLDOWN,
        .GPIO3_CH_SEL = GPIO_DISABLED   /* Note: GPIO3 is not available */
    }};
    Gpio_Init(&cfg_io);
    DEBUG_PIN_INIT(LOGIC_LOW);          /* Reinit Test Pin with enabled DEBUG */
#endif
} /* init_io_ports */


static INLINE void reset_io_ports(void)
{
	//DBG("reset_io_ports\n");
    static const GpioConfig_Type cfg_io =
    {{
        .GPIO1_DRV = 1, /* Available levels:  0, 1, 2 and 3 */
        .GPIO2_DRV = 1,
        /* GPIOs initial states: */
        .GPIO0_CH_SEL = GPIO_DIG_INPUT_WITH_PULLDOWN,
        .GPIO1_CH_SEL = GPIO_DIG_INPUT_WITH_PULLDOWN,
        .GPIO2_CH_SEL = GPIO_DIG_INPUT_WITH_PULLDOWN,
        .GPIO3_CH_SEL = GPIO_DISABLED   /* Note: GPIO3 is not available */
    }};
    Gpio_Init(&cfg_io);
#if 1 //defined(SG_ENABLE_SPI)
    Wakeup_Enable(WAKEUP_GPIO0_HIGH);
    Itc_Enable(ITC_GPIO0);
#endif

#if defined(SG_ENABLE_SPI)
    Gpio_SetPinMode(SPI_MISO, GPIO_DIG_INPUT_WITH_PULLDOWN);
    Gpio_SetPinMode(SPI_MOSI, GPIO_DIG_INPUT_WITH_PULLDOWN);
    Gpio_SetPinMode(SPI_CLK, GPIO_DIG_INPUT_WITH_PULLDOWN);
#else
    Gpio_SetPinMode(GPIO0, GPIO_DIG_INPUT_WITH_PULLDOWN);
    Gpio_SetPinMode(GPIO1, GPIO_DIG_INPUT_WITH_PULLDOWN);
    Gpio_SetPinMode(GPIO2, GPIO_DIG_INPUT_WITH_PULLDOWN);
#endif
} /* reset_io_ports */



extern void Hendrickson_Init(void);
 void init_default_value(void)
{
	TPMS_state_Init(DEFAULT_TPMS_STATE);
	Hendrickson_Init();

	SP_lfrx_rec.rx_times = 0;
	//SP_WAL_Result = 0;
	//SP_state.as_field.pressure_range = get_pressgre_range();

    //PAL_LR_flag =0;
	SP_lf_count = 0;
    SP_timer_wakeup_time = 0;
    SP_timer_wakeup_totalTime = 0;
    SP_state_keep_time = 0;
	get_pressgre_range();

    memset(&SP_measured_rec, 0x00, sizeof(T_TPMS_DATA_MEASURE_REC));

    // Measurement data from flash for testing
#ifdef TEST_ENABLE_DUMMY_DATA

    SP_dummy_enabled_flag = DBG_FLAG_NONE; //DBG_FLAG_NONE
    measured.pvt.pressure    = 1200;
    measured.pvt.voltage     = 330;     // 3.3v;
    // Temperature in [degC] equals to high byte of "Calc_Temperature_16b" - 52degC;
    measured.pvt.temperature = (124+PKG_TEMPERATURE_OFFSET); // 26 degC
    measured.acceleration_x  = 10;
    measured.acceleration_z  = 300; // 300 RPM

  #if defined (TEST_ENABLE_DEFAULT_STATE)
    if( DEFAULT_TPMS_STATE == STATE_DRIVE ||
    	DEFAULT_TPMS_STATE == STATE_LEARN 	) {
		measured.pvt.pressure    = 1200;
		measured.acceleration_x  = 10;
		measured.acceleration_z  = 203; // 300 RPM
    }
    else if( DEFAULT_TPMS_STATE == STATE_DRIVE_INTERIM ||
        	 DEFAULT_TPMS_STATE == STATE_LEARN_INTERIM 	) {
		measured.pvt.pressure    = 500;
		measured.acceleration_x  = 5;
		measured.acceleration_z  = 23; // 100 RPM
    }
    else if( DEFAULT_TPMS_STATE == STATE_STATIONARY ) {
		measured.pvt.pressure    = 500;
		measured.acceleration_x  = 0;
		measured.acceleration_z  = 0; // 0 RPM
    }
    else { // STATE_STORAGE
		measured.pvt.pressure    = 0;
		measured.acceleration_x  = 0;
		measured.acceleration_z  = 0; // 0 RPM
    }
  #endif

#ifdef SG_ENABLE_NEW_MEASURE_REC
    SP_measured_rec.pressure		 = measured.pvt.pressure;
    SP_measured_rec.acceleration_x	 = measured.acceleration_x;
    SP_measured_rec.acceleration_z	 = measured.acceleration_z;
#else
	memcpy(&SP_measured_rec, &measured, sizeof(T_TPMS_DATA_MEASURE));
#endif

#endif
}

static INLINE void update_measurements(void)
{
#ifdef TEST_ENABLE_DUMMY_DATA
	if( SP_dummy_enabled_flag & DBG_FLAG_DUMMY ) {
	#ifdef SG_ENABLE_NEW_MEASURE_REC
		SP_measured_rec.pressure		 = measured.pvt.pressure;
		SP_measured_rec.acceleration_x	 = measured.acceleration_x;
		SP_measured_rec.acceleration_z	 = measured.acceleration_z;
	#else
		memcpy(&measured, &SP_measured_rec, sizeof(T_TPMS_DATA_MEASURE));
	#endif
		measurements_valid = ( DET_PVT|DET_ACC|DET_G );
	}
	else
#endif
	{
		measurements_valid = DET_NONE;
	}
}

#ifdef LF_COMPONENT_ENABLE
static INLINE void Process_LF_Message(void) {
	uint8_t m_buf[8];
	uint8_t LfRx_size = 0;
	uint8_t i;

	LfRx_size = LfRx_FifoCount();

	if ((LfRx_size >= 4) && (!LfRx_IsFifoOverrun())) {
		for ( i = 0; i < LfRx_size; i++) {
			m_buf[i] = LfRx_FifoReadByte();
		}

		if( SP_lfrx_rec.rx_times == 0 ){
			if(LfRx_size <= 4){
				SP_lfrx_rec.data_len = LfRx_size;
				memset(SP_lfrx_rec.data, 0x00, sizeof(SP_lfrx_rec.data));
				memcpy(SP_lfrx_rec.data, m_buf, LfRx_size);

				SP_lfrx_rec.rx_times++;
			}
		}
		else{
			if( SP_lfrx_rec.data_len == LfRx_size &&
				memcmp(SP_lfrx_rec.data, m_buf, LfRx_size) == 0	){
				SP_lfrx_rec.rx_times++;

				if( SP_lfrx_rec.rx_times >= THRESHOLD_LFRX_COUNT_MAX ){
					NvRam_Recall();
					SP_lfrx_rec.rx_times = 0;
					SP_state.as_field.action_state = AS_TRIGGER_MANCHESTER;
#if 1
					#if 1
					WakeupTimer_Restart(WT_PERIOD_2_MS);	// example
					Sys_DeepSleepWith(LF_DISABLED);
					#else
					WakeupTimer_Restart(WT_PERIOD_50_MS);	// ian
					Lf_Disable();
					#endif
#else
					Lf_Disable();
					return;
#endif
				}
			}
		}

	}
  #if 0
	else { /* Too short payload size */
		gLfRx_buffer[0] = LfRx_size;
		for ( i = 0; i < LfRx_size; i++) {
			gLfRx_buffer[1+i] = LfRx_FifoReadByte();
		}
		SP_state.as_field.action_state = AS_TRIGGER_MANCHESTER;
	}
  #endif

	/* Invalid LF command */
	LfRx_FifoFlush(); /* Clean RFTx FIFO in case of an invalid payload */
	Sys_DeepSleepWith(LF_RECEIVE);
} /* Process_LF_Message */
#endif
/* ----------------------------------------------------------------------------
 * LFTx DONE Interrupt handler
 * Arguments:   none
 * Returns:     none
 * Notes:       LFTx DONE Interrupt handler
 */
__attribute__((interrupt))
void ISR_LFRX_DONE(void) {
//	Gpio_SetPinMode(SPI_CLK, GPIO_DIG_OUTPUT_1);
	Lf_Disable();
	Itc_Disable(ITC_LFRX_DONE);
	Itc_ClearPending(ITC_LFRX_DONE);
} /* ISR_LFRX_DONE */
/* EOF */
