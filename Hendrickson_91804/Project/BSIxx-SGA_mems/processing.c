#include <platformlib.h>
#include <sensorlib.h>
#include <debug.h>
#include "string.h"

#include "datatypes.h"
#include "measurements.h"
//#include <communication.h>
#include "processing.h"
#include "spi_process.h"
#include "sg_common.h"
#include "sg_dbg.h"




SP_MEM uint8_t   SP_timer_wakeup_time;
SP_MEM uint16_t  SP_timer_wakeup_totalTime;
SP_MEM tpms_state_t TmpSt;


SP_MEM uint16_t SensorModeCounter;
SP_MEM CircularEventQueue EventQueue;


#if 0
SP_MEM unsigned short NormalMode_Counter;
SP_MEM unsigned short NormalToStandbyMode_Counter;
SP_MEM unsigned short NormalToMonitoringMode_Counter;
SP_MEM unsigned short MonitoringToNoamalMode_Counter;
#endif

SP_MEM unsigned int pivot_pressure;


void Hendrickson_Init(void){

  //TmpSt = STATE_STORAGE;


  pivot_pressure=100;  // Invalid marker value;
  SP_measured_rec.pressure=0;// 0kPa
  SensorModeCounter=0;//
  queue_init(&EventQueue);
#if 0
  NormalToStandbyMode_Counter=0;
  NormalToMonitoringMode_Counter=0;
  MonitoringToNoamalMode_Counter=0;
#endif
}//Hendrickson_Init

#if 0	// org
	SP_MEM uint8_t   SP_drive_mode_report_period_spi;
#else	// 2023/12/08
	SP_MEM uint16_t   SP_drive_mode_report_period_spi;
#endif

#if 0 //defined(SG_ENABLE_AUTO_ENTRY_STORAGE_MODE)
	SP_MEM uint8_t   SP_flag;
#endif


static INLINE void TPMS_state_STATIONARY_processing(void);
static INLINE void TPMS_state_STATIONARY_TFA_processing(void);
static INLINE void TPMS_state_NORMAL_DRIVE_processing(void);
static INLINE void TPMS_state_NORMAL_DRIVE_TFA_processing(void);
static INLINE void TPMS_state_LEARN_processing(void);
static INLINE void TPMS_state_LEARN_TFA_processing(void);
uint8_t get_wakeup_time(void);
void Check_Pressure_Absnormal(void);


static INLINE uint8_t get_tpms_state_index(void)
{
	return (SP_state.as_field.tpms_state>=STATE_TOTAL_NUMBER)? STATE_STORAGE:SP_state.as_field.tpms_state;
}


/* ----------------------------------------------------------------------------
 * TEST states parameters
 */
#if defined(TEST_ENABLE_DEFAULT_STATE_AUTO_SWITCH)

const tpms_state_auto_switch_t STATE_AUTO_SWITCH[] = {
	{STATE_STORAGE		, STATE_STATIONARY		, 2*60},
	{STATE_STATIONARY	, STATE_STATIONARY_TFA	, 4*60},
};
#endif


/* ----------------------------------------------------------------------------
 * TPMS states parameters
 */

#define RFTX_DEFAULT_FORMAT RFTX_PT_REPORT_W


const tpms_state_property_t TPMS_STATE_PARAMETERS[STATE_TOTAL_NUMBER]  = {
#if defined(DQA_TEST)

		{
	        .enabled_rx_lf              = 1, //Standby mode  / Storage
			.enabled_tx_rf              = 0,
	        .calibration_needed_LFO     = 0,
	        .calibration_needed_HFO     = 0,
	        .det_period_PVT             = 15u,
	        .det_period_G               = 0,
			.report_period_spi          = 0,
			.state_keep_timeout_back_state = 0,
			.state_keep_timeout         = 0
	    },
	    {
	        .enabled_rx_lf              = 1, //Normal mode  / Stationary
			.enabled_tx_rf              = 0,
	        .calibration_needed_LFO     = 0,
	        .calibration_needed_HFO     = 0,
	        .det_period_PVT             = 10u,
	        .det_period_G               = 0u,
			.report_period_spi          = 500,
			.state_keep_timeout_back_state = 0,
			.state_keep_timeout         = 0
	    },
	    {
	        .enabled_rx_lf              = 1,  //Normal Mode TFA
			.enabled_tx_rf              = 0,
	        .calibration_needed_LFO     = 0,
	        .calibration_needed_HFO     = 0,
	        .det_period_PVT             = 5u,
	        .det_period_G               = 0,
			.report_period_spi          = 5u,
			.state_keep_timeout_back_state = STATE_STATIONARY,
			.state_keep_timeout         = 25u
	    }
	,{	
		.enabled_rx_lf				= 1, // Monitoring mode
		.enabled_tx_rf				= 0,
		.calibration_needed_LFO 	= 0,
		.calibration_needed_HFO 	= 0,
		.det_period_PVT 			= 5u,
		.det_period_G				= 0u,
		.report_period_spi			= 300,
		.state_keep_timeout_back_state = 0,
		.state_keep_timeout 		= 0
	}
	,{
	    .enabled_rx_lf				= 1, // Monitoring mode TFA
		.enabled_tx_rf				= 0,
		.calibration_needed_LFO 	= 0,
		.calibration_needed_HFO 	= 0,
		.det_period_PVT 			= 5u,
		.det_period_G				= 0u,
	    .report_period_spi			= 5u,
	    .state_keep_timeout_back_state = STATE_MONITORING,
	    .state_keep_timeout 		= 25u
	}
#else

		{
			        .enabled_rx_lf              = 1, //Standby mode  / Storage
					.enabled_tx_rf              = 0,
			        .calibration_needed_LFO     = 0,
			        .calibration_needed_HFO     = 0,
			        .det_period_PVT             = 60u,
			        .det_period_G               = 0,
					.report_period_spi          = 0u,
					.state_keep_timeout_back_state = 0,
					.state_keep_timeout         = 0
			    },
			    {
			        .enabled_rx_lf              = 1, //Normal mode  / Stationary
					.enabled_tx_rf              = 0,
			        .calibration_needed_LFO     = 0,
			        .calibration_needed_HFO     = 0,
			        .det_period_PVT             = 60u,
			        .det_period_G               = 0u,
					.report_period_spi          = 300u,//5 min
					.state_keep_timeout_back_state = 0,
					.state_keep_timeout         = 0
			    },
			    {
			        .enabled_rx_lf              = 1,  //Normal Mode TFA
					.enabled_tx_rf              = 0,
			        .calibration_needed_LFO     = 0,
			        .calibration_needed_HFO     = 0,
			        .det_period_PVT             = 60u,
			        .det_period_G               = 0,
					.report_period_spi          = 60u,
					.state_keep_timeout_back_state = STATE_STATIONARY,
					.state_keep_timeout         = 5u
			    }
			,{
							.enabled_rx_lf				= 1, //Driving mode
							.enabled_tx_rf				= 0,
							.calibration_needed_LFO 	= 0,
							.calibration_needed_HFO 	= 0,
							.det_period_PVT 			= 0u,
							.det_period_G				= 0u,
							.report_period_spi			= 0u,
							.state_keep_timeout_back_state = 0,
							.state_keep_timeout 		= 0
			}
			,{
				.enabled_rx_lf				= 1, // Monitoring mode
				.enabled_tx_rf				= 0,
				.calibration_needed_LFO 	= 0,
				.calibration_needed_HFO 	= 0,
				.det_period_PVT 			= 60u,
				.det_period_G				= 0u,
				.report_period_spi			= 1500u,//25 min
				.state_keep_timeout_back_state = 0,
				.state_keep_timeout 		= 0
			}
			,{
			    .enabled_rx_lf				= 1, // Monitoring mode TFA  (Drive TFA)
				.enabled_tx_rf				= 0,
				.calibration_needed_LFO 	= 0,
				.calibration_needed_HFO 	= 0,
				.det_period_PVT 			= 60u,
				.det_period_G				= 0u,
			    .report_period_spi			= 60u,
			    .state_keep_timeout_back_state = STATE_MONITORING,
			    .state_keep_timeout 		= 5u
			}




#endif

};

void update_measured_rec(void)
{
	if( measurements_valid & DET_PVT ) {
	#ifdef SG_ENABLE_NEW_MEASURE_REC
		SP_measured_rec.pressure	   = measured.pvt.pressure;
	#else
		memcpy(&SP_measured_rec.pvt, &measured.pvt, sizeof(T_TPMS_DATA_PVT));
	#endif
	}


}

/* ----------------------------------------------------------------------------
 * PVT and a simple acceleration measurements at TPMS state
 * Arguments:   none
 * Returns:     none
 * Notes:
 */
void TPMS_state_Measurements(void)
{
	bool bRun_pvt = false;


#ifdef TEST_ENABLE_DUMMY_DATA
	if( SP_dummy_enabled_flag & DBG_FLAG_DUMMY ) {
		measurements_valid |= (DET_PVT | DET_ACC);

	    measured.pvt.voltage     = 330;     // 3.3v;
	    // Temperature in [degC] equals to high byte of "Calc_Temperature_16b" - 52degC;
	    measured.pvt.temperature = (80+PKG_TEMPERATURE_OFFSET); // 26 degC
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
#if 0
#if defined(TEST_ENABLE_DEFAULT_STATE_AUTO_SWITCH)
		for(uint8_t m_ii=0; m_ii<2/*(sizeof(STATE_AUTO_SWITCH)/sizeof(tpms_state_auto_switch_t))*/; m_ii++ ){
			if( SP_state.as_field.tpms_state == STATE_AUTO_SWITCH[m_ii].state_now ){
				if( SP_state_keep_time > STATE_AUTO_SWITCH[m_ii].state_switch_timeout ){
					if( STATE_STATIONARY == STATE_AUTO_SWITCH[m_ii].state_switch_next ){
						measured.pvt.pressure    = 500;
					}
					else if( STATE_STORAGE == STATE_AUTO_SWITCH[m_ii].state_switch_next ){
						measured.pvt.pressure    = 0;
					}
					else if( STATE_STATIONARY_TFA == STATE_AUTO_SWITCH[m_ii].state_switch_next ){
						measured.pvt.pressure    = 800;
					}
				}
			}
		}

#endif	// End of #if defined(TEST_ENABLE_DEFAULT_STATE_AUTO_SWITCH)
#endif


		return;
	}
#endif

	if( (SP_state.as_field.action_state&AS_FLAG_MASK) == AS_TRIGGER ||
		(SP_state.as_field.action_state&AS_FLAG_MASK) == AS_TRIGGER_MANCHESTER	) {
		bRun_pvt = true;
		//bRun_acc = true;
	}
	else if( (SP_state.as_field.action_state&AS_FLAG_MASK) == AS_REQUEST_PVT ||
			 (SP_state.as_field.action_state&AS_FLAG_MASK) == AS_REQUEST_MEASURE) {
		bRun_pvt = true;
	}
	else {
		if(TPMS_STATE_PARAMETERS[get_tpms_state_index()].det_period_PVT) {
			if( SP_timer_wakeup_totalTime > 0 &&
					SP_timer_wakeup_totalTime % TPMS_STATE_PARAMETERS[get_tpms_state_index()].det_period_PVT == 0 ) {
				bRun_pvt = true;
			}
		}

		}


	if(bRun_pvt) {
		Measure_PVT();
	}


} /* TPMS_state_Measurements */


/* ----------------------------------------------------------------------------
 * Motion Detection at TPMS state
 * Arguments:   none
 * Returns:     none
 * Notes:
 */
void TPMS_state_Motion_Detection(void)
{
	bool bRun = false;

#ifdef TEST_ENABLE_DUMMY_DATA
	if( SP_dummy_enabled_flag & DBG_FLAG_DUMMY ) {
		return;
	}
#endif

#if defined (SG_DEFAULT_CUST) && SG_DEFAULT_CUST == SG_CUST_HENDRICKSON
	return;
#endif

	if( (SP_state.as_field.action_state&AS_FLAG_MASK) == AS_TRIGGER_MANCHESTER	){
		return;
	}
	else if( (SP_state.as_field.action_state&AS_FLAG_MASK) == AS_TRIGGER ){
#if 1	// 2024/02/22 check motion or not
		int16_t m_acceleration;

		if( measured.acceleration_z < 0 ){
			m_acceleration = 0 - measured.acceleration_z;
		}
		else {
			m_acceleration = measured.acceleration_z;
		}

	#ifdef SG_ENABLE_MOTION_DET_XZ
		if( measured.acceleration_x != (int16_t)ACCELERATION_INVALID ){
			if( measured.acceleration_x < 0 ) {
				if( (m_acceleration + measured.acceleration_x) < 0 ){
					m_acceleration = 0 - measured.acceleration_x;
				}
			}
			else {
				if( (m_acceleration - measured.acceleration_x) < 0 ){
					m_acceleration = measured.acceleration_x;
				}
			}
		}
	#endif

		if ( m_acceleration < (int16_t)ACC_Z_MOTION_THRESHOLD_HIGH ) {
			SP_state.as_field.rotation = STOPPED;
		}
#endif
		return;
	}

	if( (SP_state.as_field.action_state&AS_FLAG_MASK) == AS_REQUEST_G ||
		(SP_state.as_field.action_state&AS_FLAG_MASK) == AS_SET_DUMMY ) {
		bRun = true;
	}
	else {
		if(TPMS_STATE_PARAMETERS[get_tpms_state_index()].det_period_G) {
			if (SP_timer_wakeup_totalTime > 0 &&
				(SP_timer_wakeup_totalTime % TPMS_STATE_PARAMETERS[get_tpms_state_index()].det_period_G) == 0  ) {
				bRun = true;
			}

		}
    }

	if(bRun) {
		Direct_Motion_Detection();
	}
} /* TPMS_state_Motion_Detection */

/* ----------------------------------------------------------------------------
 * Final stage of the STT wake-up with or without RFTx report and LFRx listening
 * Arguments:   none
 * Returns:     none
 * Notes:
 */
void TPMS_state_processing_end(void)
{
	bool bSendMsg = false;
	bool bSendFinish = false;
	action_state_t action = (SP_state.as_field.action_state & AS_FLAG_MASK);

	uint8_t event;


    if(SP_state.as_field.action_state == AS_NONE)
    {
	//dequeue
        if(!queue_is_empty(&EventQueue))//queue not empty, dequeue
        {

    	    if(queue_dequeue(&EventQueue, &event))
    	    {
    	    	measurements_valid=0;
    	    	Measure_PVT();
    	    	SP_state.as_field.tpms_state=event;
    	    	SP_state.as_field.action_state |= AS_STATE_CHANGE;
    	    	action = (SP_state.as_field.action_state & AS_FLAG_MASK);
    	    }
        }
    }
    // Action
    if( SP_state.as_field.action_state & AS_STATE_CHANGE ) {
    	//bSendMsg = true;
#ifndef LF_COMPONENT_ENABLE
    	if((action != AS_TRIGGER) || (action != AS_TRIGGER_MANCHESTER) )
    	{

#endif
		Process_spi_buf_tx( action );
		bSendFinish = true;

#ifndef LF_COMPONENT_ENABLE
    	}
#endif


		SP_state.as_field.action_state = AS_NONE;
		SP_timer_wakeup_totalTime = 0;
		SP_state_keep_time = 0;
#if 0	//2024/01/02
		// Reset wakeup timer
		SP_timer_wakeup_time = get_wakeup_time();
		wtimer_restart_sec( SP_timer_wakeup_time );
#endif
    }
    else if(action != AS_NONE ){

		//switch( SP_state.as_field.action_state ){
    	switch( action ){

#ifdef LF_COMPONENT_ENABLE
		case AS_TRIGGER:
		case AS_TRIGGER_MANCHESTER:
			bSendMsg = true;
			break;
#endif
#if 0
		case AS_REQUEST_PVT:
		case AS_REQUEST_ACC:
		case AS_REQUEST_G:
#endif
		case AS_REQUEST_MEASURE:

		case AS_REQUEST_INFO:

		case AS_ENTRY_TFA:
			bSendMsg = true;
			measurements_valid=0;
			Measure_PVT();

			break;
		default:
			break;
		}

    }
    else {
    	uint8_t m_state_index = get_tpms_state_index();

    	if( TPMS_STATE_PARAMETERS[m_state_index].report_period_spi > 0) {



#if 1
        			// default define report_period
        			if( SP_timer_wakeup_totalTime >= TPMS_STATE_PARAMETERS[m_state_index].report_period_spi ) {
						bSendMsg = true;
						SP_timer_wakeup_totalTime = 0;
						measurements_valid=0;
						Measure_PVT();
        			}
#endif

    		}

#if 0
    		else
    		{
    			if( SP_timer_wakeup_totalTime >= TPMS_STATE_PARAMETERS[m_state_index].report_period_spi ) {
    				bSendMsg = true;
    				SP_timer_wakeup_totalTime = 0;
    			}

    		}
#endif
    }


	// Reset wakeup timer
	SP_timer_wakeup_time = 10;//get_wakeup_time();
	wtimer_restart_sec( SP_timer_wakeup_time );


    if(!bSendFinish){
		if( bSendMsg ){
			Process_spi_buf_tx( action );
		}
#ifdef SG_ENABLE_DEBUG_SPI
		else {
			#ifdef SG_ENABLE_DBG_TIMER // org:2024/01/14
			T_TPMS_SP_CNT m_cnt;

			m_cnt.SP_state_keep_time = SP_state_keep_time;
			m_cnt.const_report_period_spi = TPMS_STATE_PARAMETERS[get_tpms_state_index()].report_period_spi;
			m_cnt.SP_drive_mode_report_period_spi = SP_drive_mode_report_period_spi;
			m_cnt.SP_timer_wakeup_totalTime = SP_timer_wakeup_totalTime;
			m_cnt.SP_timer_wakeup_time = SP_timer_wakeup_time;
			delay_ms(5);
			dbg_sp_cnt( &m_cnt );
			#else
			//measured.SP_drive_mode_report_period_spi = SP_drive_mode_report_period_spi;
			//measured.SP_timer_wakeup_time = SP_timer_wakeup_time;
			measured.acceleration_x = SP_timer_wakeup_totalTime;
			measured.acceleration_z = SP_state_keep_time;
			delay_ms(5);
			dbg_pvt( &measured );
			#endif
		}

#endif
    }

#if 0
    // reset SP_timer_wakeup_totalTime
#if defined (SG_DEFAULT_CUST) && SG_DEFAULT_CUST == SG_CUST_HENDRICKSON
	if((STATE_STORAGE == SP_state.as_field.tpms_state ) &&
#else
	if((STATE_STATIONARY == SP_state.as_field.tpms_state) &&
#endif
		SP_timer_wakeup_totalTime > 0 ){
		if( SP_timer_wakeup_totalTime % TPMS_STATE_PARAMETERS[get_tpms_state_index()].det_period_PVT == 0 ){
			SP_timer_wakeup_totalTime = 0;
			SP_state_keep_time = 0;
			//SP_drive_mode_report_period_spi = 0;
		}
	}
#endif
    // Clear action flag
    if( SP_state.as_field.action_state != AS_NONE ){
    	SP_state.as_field.action_state = AS_NONE;
    }

   // update_measured_rec();

    // Wakeup enable : SPI
#if defined(SG_ENABLE_SPI)
	Wakeup_Enable(WAKEUP_GPIO0_HIGH);
	Itc_Enable(ITC_GPIO0);
#endif


    // Wakeup enable : LF
    if (TPMS_STATE_PARAMETERS[get_tpms_state_index()].enabled_rx_lf) {
        //dbg("DSleepLF ");
        Itc_Enable(ITC_LFRX_TIMEOUT);
        Wakeup_Enable(WAKEUP_LFRX_TIMEOUT);
        Itc_Enable(ITC_LFRX_DONE);
        Wakeup_Enable(WAKEUP_LF_DONE);

        LfRx_FifoFlush();
        Sys_DeepSleepWith(LF_RECEIVE);
    }
    else {
        //dbg("DSleep ");
        Itc_Disable(ITC_LFRX_TIMEOUT);
        Wakeup_Disable(WAKEUP_LFRX_TIMEOUT);

        Sys_DeepSleepWith(LF_DISABLED);
    }


    // Actually this cycle is not needed because a sleep mode is set before
    Sys_ResetCpu();

} /* TPMS_state_processing_end */

/* ----------------------------------------------------------------------------
 * Update TPMS state
 * Arguments:   none
 * Returns:     none
 * Notes:
 */
void TPMS_state_processing_and_update(void)
{
#ifdef TEST_ENABLE_DUMMY_DATA
	if( SP_dummy_enabled_flag & DBG_FLAG_DUMMY ) {
#if !defined(TEST_ENABLE_DEFAULT_STATE_AUTO_SWITCH)
		return;
#endif
	}
#endif

  if( (SP_state.as_field.action_state&AS_FLAG_MASK) != AS_REQUEST_MEASURE ) {
    if(measurements_valid & DET_PVT) {
      Check_Pressure_Absnormal();
    }
  }

#if 0
	if( (SP_state.as_field.action_state&AS_FLAG_MASK) == AS_TRIGGER ||
		(SP_state.as_field.action_state&AS_FLAG_MASK) == AS_TRIGGER_MANCHESTER	){
#if 1 //2024/01/31(disable) 2024/02/17(enable)
		if( (measurements_valid & DET_PVT) &&
//				measured.pvt.pressure == 0 ){
				measured.pvt.pressure < THRESHOLD_P_STORAGE_LOW ){
			SP_state.as_field.tpms_state = STATE_STORAGE;
			SP_state.as_field.action_state |= AS_STATE_CHANGE;
			SP_state.as_field.rotation = STOPPED;
			SP_state_keep_time = 0;
		}
#endif
		return;
	}
#endif
	if(SP_state.as_field.action_state == AS_NONE) {
		// check state keep time
		if( (TPMS_STATE_PARAMETERS[get_tpms_state_index()].state_keep_timeout > 0) &&
			( SP_state_keep_time >= TPMS_STATE_PARAMETERS[get_tpms_state_index()].state_keep_timeout) ) {


			if(TPMS_STATE_PARAMETERS[get_tpms_state_index()].state_keep_timeout_back_state == 0)
			{

				SP_state.as_field.action_state |= AS_STATE_CHANGE;

			}

			SP_state.as_field.tpms_state = TPMS_STATE_PARAMETERS[get_tpms_state_index()].state_keep_timeout_back_state;
			SP_state_keep_time = 0;

		}

#if defined (SG_DEFAULT_CUST) && SG_DEFAULT_CUST == SG_CUST_HENDRICKSON
	return;
#endif

	}

	//dequeue here


} /* TPMS_state_processing_and_update */


#if 0
/* ----------------------------------------------------------------------------
 * Processing of the TPMS state OFF
 * Arguments:   none
 * Returns:     none
 * Notes:
 */
static INLINE void TPMS_state_OFF_processing(void)
{
#if 0
   if (measurements_valid & DET_PVT)
    {
	    /* Please put here an application code to process TPMS states transitions */
    }
    else {}
    if (measurements_valid & DET_G)
    {
	    /* Please put here an application code to process TPMS states transitions */
    }
    else {}
#endif
} /* TPMS_state_OFF_processing */
#endif

/* ----------------------------------------------------------------------------
 * Processing of the TPMS state STATIONARY and FACTORY_STATIONARY
 * Arguments:   none
 * Returns:     none
 * Notes:
 */
static INLINE void TPMS_state_STATIONARY_processing(void)
{
#if 0
   if (measurements_valid & DET_PVT)
    {
	    /* Please put here an application code to process TPMS states transitions */
    }
    else {}
    if (measurements_valid & DET_ACC)
    {
	    /* Please put here an application code to process TPMS states transitions */
    }
    else {}
#endif
} /* TPMS_state_STATIONARY_processing */

/* ----------------------------------------------------------------------------
 * Processing of the TPMS state NORMAL_DRIVE and FACTORY_NORMAL_DRIVE
 * Arguments:   none
 * Returns:     none
 * Notes:
 */
static INLINE void TPMS_state_NORMAL_DRIVE_processing(void)
{
#if 0
if (measurements_valid & DET_PVT)
    {
	    /* Please put here an application code to process TPMS states transitions */
    }
    else {}
	if (measurements_valid & DET_ACC)
    {
	    /* Please put here an application code to process TPMS states transitions */
    }
    else {}
#endif
} /* TPMS_state_NORMAL_DRIVE_processing */

/* ----------------------------------------------------------------------------
 * Processing of the TPMS state ALERT_AT_STATIONARY and FACTORY_ALERT_AT_STATIONARY
 * Arguments:   none
 * Returns:     none
 * Notes:
 */
static INLINE void TPMS_state_STATIONARY_TFA_processing(void)
{
#if 0
    if (measurements_valid & DET_PVT)
    {
	    /* Please put here an application code to process TPMS states transitions */
    }
    else {}
    if (measurements_valid & DET_ACC)
    {
	    /* Please put here an application code to process TPMS states transitions */
    }
    else {}
#endif
} /* TPMS_state_STATIONARY_TFA_processing */

/* ----------------------------------------------------------------------------
 * Processing of the TPMS state ALERT_AT_NORMAL_DRIVE and FACTORY_ALERT_AT_DRIVE
 * Arguments:   none
 * Returns:     none
 * Notes:
 */
static INLINE void TPMS_state_NORMAL_DRIVE_TFA_processing(void)
{
#if 0
	if (measurements_valid & DET_PVT)
    {
	    /* Please put here an application code to process TPMS states transitions */
    }
    else {}
    if (measurements_valid & DET_ACC)
    {
	    /* Please put here an application code to process TPMS states transitions */
    }
    else {}
#endif
} /* TPMS_state_NORMAL_DRIVE_TFA_processing */

/* ----------------------------------------------------------------------------
 * Processing of the TPMS state LEARN and FACTORY_LEARN
 * Arguments:   none
 * Returns:     none
 * Notes:
 */
static INLINE void TPMS_state_LEARN_processing(void)
{


} /* TPMS_state_LEARN_processing */

static INLINE void TPMS_state_LEARN_TFA_processing(void)
{
#if 0
    if (measurements_valid & DET_PVT)
    {
	    /* Please put here an application code to process TPMS states transitions */
    }
    else {}
    if (measurements_valid & DET_ACC)
    {
	    /* Please put here an application code to process TPMS states transitions */
    }
    else {}
#endif
} /* TPMS_state_LEARN_TFA_processing */

/* ----------------------------------------------------------------------------
 * TPMS state configuration
 * Arguments:   TPMS state to be set
 * Returns:     none
 * Notes:
 */
void TPMS_state_Init(tpms_state_t state)
{
	memset(&SP_state, 0x00, sizeof(SP_state));

    SP_state.as_field.tpms_state = state;
    SP_state.as_field.action_state = AS_NONE;
    SP_state.as_field.rotation = STOPPED;


    // default timer
	//WakeupTimer_Init(55, 5); // 5 sec
	WakeupTimer_Init(MANT_10s, EXP_10s);

    SP_timer_wakeup_time = 10;//get_wakeup_time();
    SP_timer_wakeup_totalTime = 0;

} /* TPMS_state_Init */

uint8_t get_wakeup_time(void)
{
	uint8_t m_time[2];
	uint16_t m_report_period;
	uint8_t m_index = 0;
	uint8_t m_remainder;
	const tpms_state_property_t *pParm = &TPMS_STATE_PARAMETERS[get_tpms_state_index()];

	if( !pParm ){
		return TPMS_STATE_PARAMETERS[STATE_STORAGE].det_period_PVT;
	}

	//clear
	m_time[0] = m_time[1] = 0;

	//check PVT
	if( pParm->det_period_PVT > 0 ) {
		m_time[m_index] = pParm->det_period_PVT;
		m_index++;
	}
	//check G
	if( pParm->det_period_G > 0) {
		//Check det_period_G time is multiple of det_period_PVT
		if( m_index > 0  ) {
			if( (pParm->det_period_G % m_time[m_index-1]) != 0 ) {
				m_time[m_index] = pParm->det_period_G;
				m_index++;
			}
		}
		else {
			m_time[m_index] = pParm->det_period_G;
			m_index++;
		}
	}


	m_report_period = pParm->report_period_spi;



		// ?�設m_report_period?�被?�除
	if( m_index == 1 ){
		return m_time[0];
	}
	else {
		// m_time[0] : MIN
		// m_time[1] : MAX
		if( m_time[0] > m_time[1] ){
			uint8_t m_max = m_time[0];
			m_time[0] = m_time[1];
			m_time[1] = m_max;
		}

		m_remainder = SP_timer_wakeup_totalTime % m_time[1];

		if( m_remainder == 0 ){

			if( m_report_period == 0 &&
					SP_timer_wakeup_totalTime % m_time[0] == 0 ) {
				SP_timer_wakeup_totalTime = 0;
				return m_time[0];
			}

			return (m_time[0] - SP_timer_wakeup_totalTime%m_time[0]);
		}
		else if( (m_remainder + m_time[0]) <= m_time[1] ) {
			return m_time[0];
		}
		else{
			return (m_time[1] - m_remainder);
		}
	}
}


void Check_Pressure_Absnormal(void)
{
  uint16_t delta_p;
  bool bChangeMode = false;

  bool NormalToStandbyFlag =false;
  bool NormalToSMonitoring_DeltaPFlag =false;
  bool NormalToSMonitoring_80PSIPFlag =false;

  // Calculate delta safely
   uint16_t pivot_delta=0;

   #if defined (SG_DEFAULT_CUST) && SG_DEFAULT_CUST != SG_CUST_HENDRICKSON
  return;
#endif

  if(!(measurements_valid & DET_PVT)){  return;  }

#ifdef SG_ENABLE_NEW_MEASURE_REC
  if(measured.pvt.pressure > SP_measured_rec.pressure) {
    delta_p = measured.pvt.pressure - SP_measured_rec.pressure;
  }
  else {
    delta_p = SP_measured_rec.pressure - measured.pvt.pressure;
  }
#else
  if(measured.pvt.pressure > SP_measured_rec.pvt.pressure) {
    delta_p = measured.pvt.pressure - SP_measured_rec.pvt.pressure;
  }
  else {
    delta_p = SP_measured_rec.pvt.pressure - measured.pvt.pressure;
  }
#endif




  switch(SP_state.as_field.tpms_state) {

  /****************************************
   * Standby mode
   *
   *
   * out: delta P > 4 PSIG
   *****************************************/

  case STATE_STORAGE:
    if(SP_state.as_field.action_state == AS_TRIGGER){
      //TmpSt = SP_state.as_field.tpms_state;
     // SP_state.as_field.tpms_state = STATE_TRIGGER;
#ifdef LF_COMPONENT_ENABLE
      SP_state.as_field.action_state =AS_TRIGGER;
#else
      SP_state.as_field.action_state =AS_NONE;
#endif

      //bChangeMode = true;
    }else if(delta_p >= THRESHOLD_P_STORAGE_LOW){
      SP_state.as_field.tpms_state = STATE_STATIONARY;
      bChangeMode = true;


    }

  break;






  case STATE_STATIONARY:


	  /*************************************************
	   *
	   * Normal mode
	   *
	   * out:
	   * 1. Pressure < 3?�PSI for 10 mins (back to Standby)
	   * 2. Pressure >=80 PSIG (First priority)
	   * 3. delta P < 5PSIG for 15mins
	   *
	   *************************************************/
    #if defined(DQA_TEST)
	    delta_p=32;
    #endif

#if 0
	if(SP_state.as_field.action_state == AS_TRIGGER){
	        //TmpSt = SP_state.as_field.tpms_state;
	       // SP_state.as_field.tpms_state = STATE_TRIGGER;
	      SP_state.as_field.action_state =AS_TRIGGER;
	     // bChangeMode = true;
	}
#endif
	if(delta_p>= THRESHOLD_30_PSIG)
	{
		// TFA  delta P > 30 PSIG
      SP_state.as_field.tpms_state = STATE_STATIONARY_TFA;
      SP_state.as_field.action_state = AS_ENTRY_TFA;
     // bChangeMode = true;
   		//NormalToStandbyMode_Counter=0;
    }

	if(measured.pvt.pressure>=THRESHOLD_80_PSIG)
	{
		 // Pressure > 80PSIG
		  NormalToSMonitoring_80PSIPFlag=true;

       // NormalToStandbyMode_Counter=0;
	  //  NormalToMonitoringMode_Counter=0;
		SET_NORMAL_TO_STANDBY(SensorModeCounter,0);
		SET_NORMAL_TO_MONITORING(SensorModeCounter,0);
	}

	if(measured.pvt.pressure < RANGE_P_STATIONARY_TO_STORAGE)
    {
		IncrementNormalToStandby();

        if(GET_NORMAL_TO_STANDBY(SensorModeCounter)>=STATIONARY_TO_STORAGE_10MIN_TIMEOUT)
        {
    		//1. Pressure < 3?�PSIG for 10 mins (back to Standby)
        	NormalToStandbyFlag=true;
         	SET_NORMAL_TO_STANDBY(SensorModeCounter,0);
            SET_NORMAL_TO_MONITORING(SensorModeCounter,0);
          // bChangeMode = true;
           // NormalToStandbyMode_Counter=0;
	      //  NormalToMonitoringMode_Counter=0;
       }

    }
    else
    {
    	// Reset standby counter when P >= 3 PSI
    	SET_NORMAL_TO_STANDBY(SensorModeCounter, 0);
    }
#if 1

	//delta P
    //3. Normal mdoe to Monitoring mode.  Delta P <5 PSI for 15 mins.


	if((SP_state.as_field.tpms_state == STATE_STATIONARY)&&(GET_NORMAL_TO_MONITORING(SensorModeCounter)==0))
	{
		pivot_pressure=SP_measured_rec.pressure;
	}


	if(pivot_pressure > measured.pvt.pressure)
	{
	    pivot_delta = pivot_pressure - measured.pvt.pressure;
	}else
	{
	    pivot_delta = measured.pvt.pressure - pivot_pressure;
	}


	if(pivot_delta < RANGE_DELTA_P_MONITORING)
    {

		IncrementNormalToMonitoring();
    	if(GET_NORMAL_TO_MONITORING(SensorModeCounter)>THRESHOLD_P_NORMAL_TO_MONITORING_15MIN_TIMEOUT)
        //if(NormalToMonitoringMode_Counter>= 20)//5*20= 100 second
    	{
    	    //1.Delta P < 5  for 15 mins (go to Monitoring)
       //  SP_state.as_field.tpms_state = STATE_MONITORING;
         //bChangeMode = true;
    		//SP_NormalToMonitoring_flag=true;
    		  NormalToSMonitoring_DeltaPFlag=true;

        	  SET_NORMAL_TO_STANDBY(SensorModeCounter,0);
        	  SET_NORMAL_TO_MONITORING(SensorModeCounter,0);

        }

    }else
    {
    	// Reset counter
        SET_NORMAL_TO_MONITORING(SensorModeCounter, 0);
    }
#endif


	 if((NormalToStandbyFlag==true) && ( NormalToSMonitoring_DeltaPFlag ==true))
	     {
	    	 //pressure <3 PSI for 10 min
		     SP_state.as_field.tpms_state = STATE_STORAGE;


	     }else if(NormalToStandbyFlag==true)
	     {
	    	 SP_state.as_field.tpms_state = STATE_STORAGE;


	     }else if(NormalToSMonitoring_DeltaPFlag==true || NormalToSMonitoring_80PSIPFlag==true)
	     {
	    	 SP_state.as_field.tpms_state = STATE_MONITORING;
	      	 bChangeMode=true;
	     }else
	     {
	    	 SP_state.as_field.tpms_state = STATE_STATIONARY;

	     }

        //enqueue here

	    if( bChangeMode && (SP_state.as_field.action_state == AS_ENTRY_TFA))
	    {
	    	//queue STATE_STATIONARY , STATE_MONITORING event. AS_ENTRY_TFA has the highest priority.
	    	queue_enqueue(&EventQueue, SP_state.as_field.tpms_state);

	    }



    break;


  case STATE_MONITORING:

	  /******************************************
	   *
	   * Monitoring mode
	   * 1. Pressure < 60 PSIG (Priority 1)
	   * 2. Delta P > 4.5PSIG for 15 mins
	   *
	   ******************************************/
    #if defined(DQA_TEST)
	    delta_p=33;
    #endif

#if 0
	if(SP_state.as_field.action_state == AS_TRIGGER){
	    //TmpSt = SP_state.as_field.tpms_state;
	    //SP_state.as_field.tpms_state = STATE_TRIGGER;
	    SP_state.as_field.action_state =AS_TRIGGER;
     // bChangeMode = true;
	}
#endif
	if(delta_p>= THRESHOLD_30_PSIG)
    {
		// TFA  delta P > 30 PSIG
		SP_state.as_field.tpms_state = STATE_DRIVE_TFA;
        SP_state.as_field.action_state = AS_ENTRY_TFA;
    //  bChangeMode = true;
        // bChangeMode = true;


    }

	if(measured.pvt.pressure<THRESHOLD_60_PSIG)
    {
    	// Pressure < 60PSIG back to Normal  mode.
    	SP_state.as_field.tpms_state = STATE_STATIONARY;
    	bChangeMode=true;
      //bChangeMode = true;

    	SET_MONITORING_TO_NORMAL(SensorModeCounter,0);

    }
#if 1

	if((SP_state.as_field.tpms_state == STATE_MONITORING)&&(GET_MONITORING_TO_NORMAL(SensorModeCounter)==0))
	{
		pivot_pressure=SP_measured_rec.pressure;
	}


	if(pivot_pressure > measured.pvt.pressure)
	{
	    pivot_delta = pivot_pressure - measured.pvt.pressure;
	}else
	{
	    pivot_delta = measured.pvt.pressure - pivot_pressure;
	}


	if(pivot_delta>THRESHOLD_4_POINT_5_PSIG)
    {
		IncrementMonitoringToNormal();

    	//1.Delta P > 4.5?�PSI for 15 mins (back to Normal mode )
    	if(GET_MONITORING_TO_NORMAL(SensorModeCounter)>THRESHOLD_P_MONITORING_TO_NORMAL_TIMEOUT)
    	{

    	    		SP_state.as_field.tpms_state = STATE_STATIONARY;
    	    		bChangeMode=true;
                    // bChangeMode = true;
    	    		SET_MONITORING_TO_NORMAL(SensorModeCounter,0);
    	    		SET_NORMAL_TO_MONITORING(SensorModeCounter,0);

        }
    }else
    {
    	SET_MONITORING_TO_NORMAL(SensorModeCounter,0);
    	SET_NORMAL_TO_MONITORING(SensorModeCounter,0);
    }




	 //enqueue here
	    if( bChangeMode && (SP_state.as_field.action_state == AS_ENTRY_TFA))
	    {
	    	//queue STATE_STATIONARY , STATE_MONITORING event. AS_ENTRY_TFA has the highest priority.
	    	queue_enqueue(&EventQueue, SP_state.as_field.tpms_state);

	    }


#endif
    break;


#if 0
  case STATE_STATIONARY_TFA:
  case STATE_DRIVE_TFA:
      SP_state.as_field.action_state = AS_ENTRY_TFA;
  SP_measured_rec.pressure = measured.pvt.pressure;
      bChangeMode = true;
     break;
#endif

  default :
    break;
  }


  //update pressure  previous pressure = current pressure;
  SP_measured_rec.pressure	   = measured.pvt.pressure;
#if 1
  if( bChangeMode ){
    SP_state.as_field.action_state |= AS_STATE_CHANGE;

#if defined(SG_ENABLE_AUTO_ENTRY_STORAGE_MODE)
    SP_state.as_field.Reserved_spmem_bits = 0;
#endif
  }
#endif
}




#if 0
static INLINE void Process_PVT(void)
{
	Check_Pressure_Absnormal();

}   // Process_PVT
#endif




// Increment NormalToStandbyMode_Counter
void IncrementNormalToStandby(void)
{
    uint16_t current = GET_NORMAL_TO_STANDBY(SensorModeCounter);

    if (current < NORMAL_TO_STANDBY_MAX)
    {
        current++;
        SET_NORMAL_TO_STANDBY(SensorModeCounter, current);
    }

}

// Increment NormalToMonitoringMode_Counter
void IncrementNormalToMonitoring(void)
{
    uint16_t current = GET_NORMAL_TO_MONITORING(SensorModeCounter);

    if (current < NORMAL_TO_MONITORING_MAX)
    {
        current++;
        SET_NORMAL_TO_MONITORING(SensorModeCounter, current);
    }


}

// Increment MonitoringToNormalMode_Counter
void IncrementMonitoringToNormal(void)
{
    uint16_t current = GET_MONITORING_TO_NORMAL(SensorModeCounter);

    if (current < MONITORING_TO_NORMAL_MAX)
    {
        current++;
        SET_MONITORING_TO_NORMAL(SensorModeCounter, current);
    }


}




// Core Queue Functions
void queue_init(CircularEventQueue *queue) {
    queue->head = 0;
    queue->tail = 0;
    queue->count = 0;
    // Clear the events array for clean state
    memset(queue->events, 0, sizeof(queue->events));
}

bool queue_is_full(const CircularEventQueue *queue) {
    return queue->count == 5;
}

bool queue_is_empty(const CircularEventQueue *queue) {
    return queue->count == 0;
}

bool queue_enqueue(CircularEventQueue *queue, uint8_t event) {
    if (queue_is_full(queue)) {
        return false;  // Queue full
    }

    queue->events[queue->tail] = event;
    queue->tail = (queue->tail + 1) % 5;
    queue->count++;
    return true;
}

bool queue_dequeue(CircularEventQueue *queue, uint8_t *event) {
    if (queue_is_empty(queue)) {
        return false;  // Queue empty
    }

    *event = queue->events[queue->head];
    queue->head = (queue->head + 1) % 5;
    queue->count--;
    return true;
}

bool queue_peek(const CircularEventQueue *queue, uint8_t *event) {
    if (queue_is_empty(queue)) {
        return false;
    }

    *event = queue->events[queue->head];
    return true;
}

/* EOF */
