/*
 * Copyright (C) 2018-2022 Melexis N.V.
 *
 * Software Platform
 */

#include <sensorlib.h>
#include <atomic.h>
#include <builtin_mlx16.h>

#include <lftx.h>
#include <diag.h>
#include <debug.h>

volatile uint8_t    u8;
volatile int16_t    i16;
volatile int16_t    ai16;
volatile uint16_t   u16;
volatile uint32_t   u32;
volatile bool       b1;

#define TX_SIZE (50u)       /* Should be < 256 */
uint8_t Tx_buf[TX_SIZE];    /* Prepared for the RfTx_RunFromBuf() */

typedef struct {
    uint16_t address;
    uint16_t key1;
    uint16_t key2;
    uint16_t data_buffer[32];
} flash_cmd_t;

flash_cmd_t flash_cmd;

void scratchpad_example (void);
void sys_reset_cpu_example(void);
void sys_sleep_with_example (void);
void sys_deep_sleep_with_example (void);
void atomic_section_example (void);
void debug_output_example (void);

/*
 * Main
 */
int main (void)
{
    /* abs_watchdog.h */
    if (AbsWatchdog_IsWindowOpen()) { AbsWatchdog_Restart(); }

#if defined(HAS_ACCEL_X_SENSOR)
    /* phys.h */
    i16 = Phys_AccelerationX();
    /* accel.h */
    i16 = Accel_GToRawX(12u);
    ai16 = Accel_RawToGX(i16);
    /* accel_hw_filter.h */
    Accel_InitFilterForX(HWF_TYPE_0, 0u, 10u /* x0.1 ms */);
#endif /* HAS_ACCEL_X_SENSOR */

#if defined(HAS_ACCEL_Z_SENSOR)
    /* phys.h */
    i16 = Phys_AccelerationZ();
    /* accel.h */
    i16 = Accel_GToRawZ(13u);
    ai16 = Accel_RawToGZ(i16);
    /* accel_hw_filter.h */
    Accel_InitFilterForZ(HWF_TYPE_0, 0u, 20u /* x0.1 ms */);
#endif /* HAS_ACCEL_Z_SENSOR */

#if defined(HAS_ACCEL_X_SENSOR) || defined(HAS_ACCEL_Z_SENSOR)
    i16 = Accel_WaitAndReadNext();
    i16 = Accel_GetLatestMeasurement();
    i16 = Accel_ReadNext();
    i16 = Accel_PeekNext();
    u16 = Accel_GetSampleCnt();
    Accel_DeinitFilter();
#endif /* HAS_ACCEL_X_SENSOR or HAS_ACCEL_Z_SENSOR */

    /* adc.h, calc.h */
    uint16_t raw_pressure = Adc_ReadPressure();
    uint16_t raw_temp     = Adc_ReadTemperature();
    uint16_t raw_volt     = Adc_ReadVoltage();
    u16 = Adc_ReadGpio(GPIO0);

    if( !Adc_IsError() ) {          /* if no error reported .. */
        u32 = Calc_Pressure(raw_pressure, raw_temp, raw_volt);
        u16 = Calc_Temperature(raw_temp);
        u16 = Calc_Voltage(raw_volt);
    }
    else {
        /* error handling here .. */
        Adc_ClearAllErrors();
    }

    Adc_RawPvt_Type raw_pvt;
    Adc_ReadPVT(&raw_pvt);
    if( !Adc_IsError() ) {          /* if no error reported .. */
        u32 = Calc_Pressure(raw_pvt.pressure,
                            raw_pvt.temperature,
                            raw_pvt.voltage);
        u16 = Calc_Temperature(raw_pvt.temperature);
        u16 = Calc_Voltage(raw_pvt.voltage);
    }
    else {
        /* error handling here .. */
        Adc_ClearAllErrors();
    }

    u32 = Calc_RomBistFor(0x1000u, 0x103Eu);

    if (!Check_IsBadPressure()) {
        u16 = low_power_adc_read_pressure();
    }

    b1 = Check_IsBadAccelX(400u, 2500u);
    b1 = Check_IsBadAccelZ(400u, 2500u);
    volatile DataCheck_Type nvram_state = Check_IsBadMlxData();
    if (nvram_state != DATA_VALID) { NOP(); }

    /* diag.h */
    static const DiagConfig_Type cfg_diag =
    {{
            .DIAG_REF_SEL = DIAG_REF_VANA,
            .DIAG0_CH_SEL = DIAG_CH_GPIO0,
            .DIAG0_HYST   = DIAG_HYST_4_LSB,

            .DIAG0_VREFL  = 11u,
            .DIAG0_VREFH  = 15u
    }};
    Diag_Init(&cfg_diag);

    Diag_SetReferenceSource(DIAG_REF_VBG);

    Diag_ConfigChannel(DIAG0, DIAG_CH_GPIO0, 10u, 20u, DIAG_HYST_4_LSB);
    Diag_ConfigChannel(DIAG1, DIAG_CH_GPIO1, 30u, 40u, DIAG_HYST_2_LSB);
    Diag_ConfigChannel(DIAG2, DIAG_CH_GPIO2, 50u, 60u, DIAG_HYST_1_LSB);
    Diag_ConfigChannel(DIAG3, DIAG_CH_GPIO3, 70u, 80u, DIAG_HYST_OFF);

    Diag_SetMode(DIAG0, DIAG_EVENT_WHILE_IN_RANGE);
    Diag_SetMode(DIAG1, DIAG_EVENT_WHILE_OUT_OF_RANGE);
    Diag_SetMode(DIAG2, DIAG_EVENT_WHILE_BELOW_UPPER_THRESHOLD);
    Diag_SetMode(DIAG3, DIAG_ENABLED_WITH_NO_EVENTS);

    Diag_SetMode(DIAG0, DIAG_DISABLED);
    Diag_SetMode(DIAG1, DIAG_DISABLED);
    Diag_SetMode(DIAG2, DIAG_DISABLED);
    Diag_SetMode(DIAG3, DIAG_DISABLED);

    DiagStatusAll_Type s = Diag_GetStatusAll();

    if (   (s.DIAG0 == DIAG_STATUS_BELOW_RANGE)
        && (s.DIAG1 == DIAG_STATUS_IN_RANGE)
        && (s.DIAG2 == DIAG_STATUS_IN_RANGE)
        && (s.DIAG3 == DIAG_STATUS_IN_RANGE))
    { NOP(); }

    if (Diag_GetStatusAll().DIAG0 == DIAG_STATUS_IN_RANGE)      { NOP(); }
    if (Diag_GetStatusAll().DIAG1 == DIAG_STATUS_BELOW_RANGE)   { NOP(); }
    if (Diag_GetStatusAll().DIAG2 == DIAG_STATUS_ABOVE_RANGE)   { NOP(); }
    if (Diag_GetStatusAll().DIAG3 == DIAG_STATUS_BELOW_RANGE)   { NOP(); }

    if (!Diag_IsInRangeAll())   { NOP(); }
    if (!Diag_IsInRange(DIAG0)) { NOP(); }
    if (!Diag_IsInRange(DIAG1)) { NOP(); }
    if (!Diag_IsInRange(DIAG2)) { NOP(); }
    if (!Diag_IsInRange(DIAG3)) { NOP(); }

    /* flash5_drv.h */
    if (Flash_GetSingleErrorCounter() == 5u) {
        /* error handler placeholder .. */
        Flash_ClearSingleErrorCounter();
    }

    if (Flash_SectorErase(flash_cmd.address,
                          flash_cmd.key1,
                          flash_cmd.key2)) {
        /* page write error handler .. */
    }
    else {
        /* success */
    }

    if (Flash_PageWrite(flash_cmd.address,
                        flash_cmd.key1,
                        flash_cmd.key2,
                        flash_cmd.data_buffer)) {
        /* page write error handler .. */
    }
    else {
        /* success */
    }

    /* gpio.h */
    static const GpioConfig_Type cfg =
    {{
        .GPIO0_DRV = 2u,
        .GPIO1_DRV = 2u,
        .GPIO2_DRV = 2u,
        .GPIO3_DRV = 2u,

        .GPIO0_CH_SEL = GPIO_DIG_OUTPUT_0,
        .GPIO1_CH_SEL = GPIO_DIG_OUTPUT_1,
        .GPIO2_CH_SEL = GPIO_DIG_OUTPUT_0,
        .GPIO3_CH_SEL = GPIO_DIG_OUTPUT_1
    }};

    Gpio_Init(&cfg);

    Gpio_SetPinMode(GPIO0, GPIO_DIG_INPUT_WITH_PULLUP);
    Gpio_SetPinMode(GPIO1, GPIO_ANA_INPUT);
    Gpio_SetPinMode(GPIO2, GPIO_DIG_OUTPUT_0);
    Gpio_SetPinMode(GPIO2, GPIO_DIG_OUTPUT_1);
    Gpio_SetPinMode(GPIO3, GPIO_DIG_OUTPUT_0);
    Gpio_SetPinMode(GPIO3, GPIO_DIG_OUTPUT_HI_Z);

    if (!Gpio_ReadPin(GPIO0))  { NOP(); }
    if ( Gpio_ReadPin(GPIO1))  { NOP(); }
    if ( Gpio_ReadPin(GPIO2))  { NOP(); }
    if (!Gpio_ReadPin(GPIO3))  { NOP(); }

    if (Gpio_ReadPort() == 0u) { NOP(); }

    Gpio_WritePin(GPIO0, Gpio_ReadPin(GPIO0));

    Gpio_WritePin(GPIO0, 0u);
    Gpio_WritePin(GPIO1, 1u);
    Gpio_WritePin(GPIO2, 1u);
    Gpio_WritePin(GPIO3, 0u);

    if (Gpio_TogglePin(GPIO0)) { NOP(); }
    if (Gpio_TogglePin(GPIO1)) { NOP(); }
    if (Gpio_TogglePin(GPIO2)) { NOP(); }
    if (Gpio_TogglePin(GPIO3)) { NOP(); }

    /* itc.h */
    Itc_SetPrio(ITC_STIMER1, 3u);
    Itc_Enable(ITC_STIMER1);
    if (Itc_IsPending(ITC_STIMER1)) { Itc_ClearPending(ITC_STIMER1); }
    Itc_Disable(ITC_STIMER1);

    Itc_SetPrio(ITC_STIMER2, 3u);
    Itc_Enable(ITC_STIMER2);
    if (Itc_IsPending(ITC_STIMER2)) { Itc_ClearPending(ITC_STIMER2); }
    Itc_Disable(ITC_STIMER2);

    Itc_SetPrio(ITC_NVRAM_SHELL_STORE, 3u);
    Itc_Enable(ITC_NVRAM_SHELL_STORE);
    if (Itc_IsPending(ITC_NVRAM_SHELL_STORE)) { Itc_ClearPending(ITC_NVRAM_SHELL_STORE); }
    Itc_Disable(ITC_NVRAM_SHELL_STORE);

    Itc_SetPrio(ITC_WUPTMR, 3u);
    Itc_Enable(ITC_WUPTMR);
    if (Itc_IsPending(ITC_WUPTMR)) { WakeupTimer_IntClear(); }
    Itc_Disable(ITC_WUPTMR);

    /* ITC_RFTX_HALF: reserved */

    /* ITC_RFTX_DONE: reserved */

    Itc_SetPrio(ITC_RFTX_LOCKED, 5u);
    Itc_Enable (ITC_RFTX_LOCKED);
    if (Itc_IsPending(ITC_RFTX_LOCKED)) { Itc_ClearPending(ITC_RFTX_LOCKED); }
    Itc_Disable(ITC_RFTX_LOCKED);

    Itc_SetPrio(ITC_GPIO0, 3u);
    Itc_Enable(ITC_GPIO0);
    if (Itc_IsPending(ITC_GPIO0)) { Itc_ClearPending(ITC_GPIO0); }
    Itc_Disable(ITC_GPIO0);

    Itc_SetPrio(ITC_GPIO1, 4u);
    Itc_Enable(ITC_GPIO1);
    if (Itc_IsPending(ITC_GPIO1)) { Itc_ClearPending(ITC_GPIO1); }
    Itc_Disable(ITC_GPIO1);

    Itc_SetPrio(ITC_GPIO2, 5u);
    Itc_Enable(ITC_GPIO2);
    if (Itc_IsPending(ITC_GPIO2)) { Itc_ClearPending(ITC_GPIO2); }
    Itc_Disable(ITC_GPIO2);

    Itc_SetPrio(ITC_GPIO3, 6u);
    Itc_Enable(ITC_GPIO3);
    if (Itc_IsPending(ITC_GPIO3)) { Itc_ClearPending(ITC_GPIO3); }
    Itc_Disable(ITC_GPIO3);

    Itc_Enable (ITC_AWD_ATT);
    if (Itc_IsPending(ITC_AWD_ATT)) { Itc_ClearPending(ITC_AWD_ATT); }
    Itc_Disable(ITC_AWD_ATT);

    Itc_SetPrio(ITC_ADC_XH018, 3u);
    Itc_Enable (ITC_ADC_XH018);
    if (Itc_IsPending(ITC_ADC_XH018)) { Itc_ClearPending(ITC_ADC_XH018); }
    Itc_Disable(ITC_ADC_XH018);

    Itc_SetPrio(ITC_LFRX_SNIFF, 5u);
    Itc_Enable (ITC_LFRX_SNIFF);
    if (Itc_IsPending(ITC_LFRX_SNIFF)) { Itc_ClearPending(ITC_LFRX_SNIFF); }
    Itc_Disable(ITC_LFRX_SNIFF);

    Itc_SetPrio(ITC_LFRX_TIMEOUT, 6u);
    Itc_Enable (ITC_LFRX_TIMEOUT);
    if (Itc_IsPending(ITC_LFRX_TIMEOUT)) { Itc_ClearPending(ITC_LFRX_TIMEOUT); }
    Itc_Disable(ITC_LFRX_TIMEOUT);

    Itc_SetPrio(ITC_LFRX_SYNC, 3u);
    Itc_Enable (ITC_LFRX_SYNC);
    if (Itc_IsPending(ITC_LFRX_SYNC)) { Itc_ClearPending(ITC_LFRX_SYNC); }
    Itc_Disable(ITC_LFRX_SYNC);

    Itc_SetPrio(ITC_LFRX_DATA, 4u);
    Itc_Enable (ITC_LFRX_DATA);
    if (Itc_IsPending(ITC_LFRX_DATA)) { Itc_ClearPending(ITC_LFRX_DATA); }
    Itc_Disable(ITC_LFRX_DATA);

    Itc_SetPrio(ITC_LFRX_DONE, 5);
    Itc_Enable (ITC_LFRX_DONE);
    if (Itc_IsPending(ITC_LFRX_DONE)) { Itc_ClearPending(ITC_LFRX_DONE); }
    Itc_Disable(ITC_LFRX_DONE);

    Itc_SetPrio(ITC_LFTX_HALF, 3u);
    Itc_Enable (ITC_LFTX_HALF);
    if (Itc_IsPending(ITC_LFTX_HALF)) { Itc_ClearPending(ITC_LFTX_HALF); }
    Itc_Disable(ITC_LFTX_HALF);

    Itc_SetPrio(ITC_LFTX_DONE, 4u);
    Itc_Enable (ITC_LFTX_DONE);
    if (Itc_IsPending(ITC_LFTX_DONE)) { Itc_ClearPending(ITC_LFTX_DONE); }
    Itc_Disable(ITC_LFTX_DONE);

    Itc_SetPrio(ITC_TEMPSENS, 5u);
    Itc_Enable (ITC_TEMPSENS);
    if (Itc_IsPending(ITC_TEMPSENS)) { Itc_ClearPending(ITC_TEMPSENS); }
    Itc_Disable(ITC_TEMPSENS);

    Itc_SetPrio(ITC_DIAG0, 3u);
    Itc_Enable(ITC_DIAG0);
    if (Itc_IsPending(ITC_DIAG0)) { Itc_ClearPending(ITC_DIAG0); }
    Itc_Disable(ITC_DIAG0);

    Itc_SetPrio(ITC_DIAG1, 4u);
    Itc_Enable(ITC_DIAG1);
    if (Itc_IsPending(ITC_DIAG1)) { Itc_ClearPending(ITC_DIAG1); }
    Itc_Disable(ITC_DIAG1);

    Itc_SetPrio(ITC_DIAG2, 5u);
    Itc_Enable(ITC_DIAG2);
    if (Itc_IsPending(ITC_DIAG2)) { Itc_ClearPending(ITC_DIAG2); }
    Itc_Disable(ITC_DIAG2);

    Itc_SetPrio(ITC_DIAG3, 6u);
    Itc_Enable(ITC_DIAG3);
    if (Itc_IsPending(ITC_DIAG3)) { Itc_ClearPending(ITC_DIAG3); }
    Itc_Disable(ITC_DIAG3);

    Itc_Enable(ITC_MLX16_SOFT);
    if (Itc_IsPending(ITC_MLX16_SOFT)) { Itc_ClearPending(ITC_MLX16_SOFT); }
    Itc_Disable(ITC_MLX16_SOFT);

    /* lf.h */
    static const LfConfig_Type lf_cfg =
    {{
        /* TX config */
        .LFTX_MODE  = LFTX_MANCHESTER_LSB_FIRST,    /* see ::LfTxMode_Type */
        .LFTX_POL   = LFTX_POLARITY_1,              /* see ::LfTxPolarity_Type */

        /* RX config */
        /* Preamble */
        .LFRX_PREAMBLE  = 1,
        .LFRX_LONGCNT   = 1,
        .LFRX_MINCNT    = 34,
        .LFRX_DELTACNT  = 13,

        .LF_GAIN_SEL = LF_GAIN_MID,

        /* Sync word */
        .LFRX_SYNCLEN       = 15,
        .LFRX_SYNCPOL       = 1,
        .LFRX_SYNC_DIGIT1   = 3,
        .LFRX_SYNC_DIGIT2   = 3,
        .LFRX_SYNC_DIGIT3   = 3,
        .LFRX_SYNC_DIGIT4   = 3,
        .LFRX_SYNC_DIGIT5   = 3,
        .LFRX_SYNC_DIGIT6   = 3,
        .LFRX_SYNC_DIGIT7   = 3,
        .LFRX_SYNC_DIGIT8   = 3,
        .LFRX_SYNC_DIGIT9   = 3,
        .LFRX_SYNC_DIGIT10  = 3,
        .LFRX_SYNC_DIGIT11  = 3,
        .LFRX_SYNC_DIGIT12  = 3,
        .LFRX_SYNC_DIGIT13  = 3,
        .LFRX_SYNC_DIGIT14  = 3,
        .LFRX_SYNC_DIGIT15  = 3,

        .LFRX_TIMEOUT_MANT  = 0,
        .LFRX_TIMEOUT_EXP   = 0,

        /* Header */
        .LFRX_HDRLEN        = LFRX_HDR_32_BITS,
        .LFRX_HDR_L         = 0xFFFF,
        .LFRX_HDR_H         = 0xFFFF,
        .LFRX_HDR_MASK_L    = 0,
        .LFRX_HDR_MASK_H    = 0,
        .LFRX_ORDER         = LFRX_ORDER_MSB_FIRST,
        .LFRX_POL           = 1,

        /* Stand by */
        .LF_EN_FD       = 1,
        .LFRX_STBY_ON   = 1,
        .LFRX_STBY_MANT = 0,
        .LFRX_STBY_EXP  = 0
    }};

    Lf_Init(&lf_cfg);
    Lf_Start(LF_RECEIVE);
    Lf_Disable();

    if (Lf_GetState() == LFRX_STATE_WAIT_HEADER) { NOP(); }

    /* lftx.h */
    LfTx_FifoWriteByte(0xAAu);
    if (LfTx_IsFifoFull())      { NOP(); }
    if (LfTx_IsFifoError())     { NOP(); }
    if (LfTx_IsFifoOverrun())   { NOP(); }
    if (LfTx_IsFifoUnderrun())  { NOP(); }
    u8 = LfTx_FifoCount();
    LfTx_FifoFlush();

    u8 = LfRx_FifoPeekByte();
    u8 = LfRx_FifoReadByte();
    if (LfRx_IsFifoFull())      { NOP(); }
    if (LfRx_IsFifoOverrun())   { NOP(); }
    if (LfRx_IsFifoUnderrun())  { NOP(); }
    u8 = LfRx_FifoCount();
    LfRx_FifoFlush();
    if (LfRx_ReceivedHeader() == 0xFEEDBEEFu) { NOP(); }

    /* nvram.h */
    NvRam_Recall();
    NvRam_Store();

    if (NvRam_IsSingleErrorCorrected()) {
        /* error handler placeholder .. */
        NvRam_ClearSingleError();
    }

    if (NvRam_IsDoubleErrorDetected()) {
        /* error handler placeholder .. */
        NvRam_ClearDoubleError();
    }

    NvRam_RestoreOscTrimming();
    NvRam_RestoreHfoTrimming();
    NvRam_RestoreLfoTrimming();
    NvRam_RestoreUlpoTrimming();

    /* radio.h */
    Radio_Disable();

    /* rf.h */
    static const RfTxExtraConfig_Type rftx_freq_cfg =
        RFTX_EXTRA_CONFIG(433920,
                          RFTX_PRESET_434MHZ_5_DBM_BYPASS_OFF,
                          XTAL_CAP_INT_6PF);

    static const RfTxConfig_Type cfg_rf =
    {{
        .RFTX_PATTERN_LEN = 1,
        .extra = &rftx_freq_cfg,
        /* add config for other fields here !! */
    }};

    RfTx_Init(&cfg_rf);
    if (RfTx_Calibrate()) {         /* if calibration failed .. */
        /* error handler placeholder .. */
        NOP();
    }

    RfTx_FifoFlush();
    RfTx_FifoWriteByte(u8);
    RfTx_FifoWriteByte(0x55u);

    if (RfTx_Run()) {   /* if RFTx failed .. */
        /* error handler placeholder .. */
        NOP();
    }

    if (RfTx_RunFromBuf(Tx_buf, TX_SIZE)) { /* if RFTx failed .. */
        /* error handler placeholder .. */
        NOP();
    }

    u8 = RfTx_FifoCount();

    if (RfTx_IsFifoFull())     { NOP(); }
    if (RfTx_IsFifoOverrun())  { NOP(); }
    if (RfTx_IsFifoUnderrun()) { NOP(); }

    if(RfTx_GetStatus().PLL_XTAL_RUN != 0u) { NOP(); }
    if(RfTx_GetStatus().PLL_LOCKED   != 0u) { NOP(); }
    if(RfTx_GetStatus().RFTX_INFO == RFTX_ERROR_PLL_OR_SUPPLY) { NOP(); }

    /* simple_timer.h */
    SimpleTimer1_Start(SIMPLE_TIMER_CPU_CLOCK, 1000u);
    SimpleTimer1_Stop();
    u16 = SimpleTimer1_GetCounter();

    SimpleTimer2_Start(SIMPLE_TIMER_CPU_CLOCK, 1000u);
    SimpleTimer2_Stop();
    u16 = SimpleTimer2_GetCounter();

    /* syslib.h */
    if(Sys_CalibrateHfo()) {    /* if failed .. */
        /* error handler placeholder .. */
        NOP();
    }

    if(Sys_CalibrateLfo()) {    /* if failed .. */
        /* error handler placeholder .. */
        NOP();
    }

    if(Sys_CalibrateUlpo()) {   /* if failed .. */
        /* error handler placeholder .. */
        NOP();
    }

    if(Sys_CalibrateHfoByExtClk()) {    /* if failed .. */
        /* error handler placeholder .. */
        NOP();
    }

    if(Sys_CalibrateLfoByExtClk()) {    /* if failed .. */
        /* error handler placeholder .. */
        NOP();
    }

    if(Sys_CalibrateUlpoByExtClk()) {   /* if failed .. */
        /* error handler placeholder .. */
        NOP();
    }

    Sys_HaltWith(LF_DISABLED);
    Sys_StopWith(LF_DISABLED);

    ChipId_Type Id = Sys_GetChipId();
    if (Id.as_dword == 0ul) { NOP(); }
    u32 = Sys_GetChipId().as_dword;

    u16 = Sys_GetChipId().as_word[0];
    u16 = Sys_GetChipId().as_word[1];

    u8  = Sys_GetChipId().as_byte[0];
    u8  = Sys_GetChipId().as_byte[1];
    u8  = Sys_GetChipId().as_byte[2];
    u8  = Sys_GetChipId().as_byte[3];

    ChipIdHi_Type IdHi = Sys_GetChipIdHi();
    if (IdHi.as_word == 0u) { NOP(); }
    u16 = Sys_GetChipIdHi().as_word;

    u8  = Sys_GetChipIdHi().as_byte[0];
    u8  = Sys_GetChipIdHi().as_byte[1];

    if (Sys_IsColdBoot())   { NOP(); }

    switch (Sys_GetResetReason()) {
        case RESET_REASON_AWD:      u16 = 1; break;
        case RESET_REASON_SOFT:     u16 = 2; break;
        case RESET_REASON_HVDIG:    u16 = 3; break;
        case RESET_REASON_DBG:      u16 = 4; break;
        case RESET_REASON_POR:      u16 = 5; break;
        case RESET_REASON_WAKEUP:   u16 = 6; break;
        default:                    u16 = 7; break;
    }

    Sys_SetUlpLfo();
    Sys_SetNormalLfo();

    /* temp_sensor.h */
    TempSensor_SetMode(TEMPSENS_EVENT_WHILE_OUT_OF_RANGE);

    TempSensorMode_Type saved_mode = TempSensor_GetMode();
    TempSensor_SetMode(TEMPSENS_ENABLED_WITH_NO_EVENTS);
    /* .. do ADC of temp channel */
    TempSensor_SetMode(saved_mode);     /* restore mode */

    if (TempSensor_GetStatus()  == TEMPSENS_IN_RANGE) { NOP(); }
    if (!TempSensor_IsInRange()) { NOP(); }

    /* wakeup_timer.h  */
    WakeupTimer_Restart(WT_PERIOD_MAX);
    WakeupTimer_Init(255u, 12u);
    WakeupTimer_IntClear();

    /* RFLF wake up */
    /* WAKEUP_RFTX_HALF: reserved */
    /* WAKEUP_RFTX_DONE: reserved */
    Wakeup_Enable(WAKEUP_LFTX_HALF);
    Wakeup_Enable(WAKEUP_LFRX_SNIFF);
    Wakeup_Enable(WAKEUP_LFRX_TIMEOUT);
    Wakeup_Enable(WAKEUP_LFRX_SYNC);
    Wakeup_Enable(WAKEUP_LFRX_DATA);
    Wakeup_Enable(WAKEUP_LF_DONE);

    /* WAKEUP_RFTX_HALF: reserved */
    /* WAKEUP_RFTX_DONE: reserved */
    Wakeup_Disable(WAKEUP_LFTX_HALF);
    Wakeup_Disable(WAKEUP_LFRX_SNIFF);
    Wakeup_Disable(WAKEUP_LFRX_TIMEOUT);
    Wakeup_Disable(WAKEUP_LFRX_SYNC);
    Wakeup_Disable(WAKEUP_LFRX_DATA);
    Wakeup_Disable(WAKEUP_LF_DONE);

    /* GPIO wake up */
    Wakeup_Enable(WAKEUP_GPIO0_LOW);
    Wakeup_Enable(WAKEUP_GPIO0_HIGH);
    Wakeup_Enable(WAKEUP_GPIO1_LOW);
    Wakeup_Enable(WAKEUP_GPIO1_HIGH);
    Wakeup_Enable(WAKEUP_GPIO2_LOW);
    Wakeup_Enable(WAKEUP_GPIO2_HIGH);
    Wakeup_Enable(WAKEUP_GPIO3_LOW);
    Wakeup_Enable(WAKEUP_GPIO3_HIGH);

    Wakeup_Disable(WAKEUP_GPIO0_LOW);
    Wakeup_Disable(WAKEUP_GPIO0_HIGH);
    Wakeup_Disable(WAKEUP_GPIO1_LOW);
    Wakeup_Disable(WAKEUP_GPIO1_HIGH);
    Wakeup_Disable(WAKEUP_GPIO2_LOW);
    Wakeup_Disable(WAKEUP_GPIO2_HIGH);
    Wakeup_Disable(WAKEUP_GPIO3_LOW);
    Wakeup_Disable(WAKEUP_GPIO3_HIGH);

    /* Temperature sensor wake up */
    Wakeup_Enable(TEMP_BELOW_LOW_THRESHOLD);
    Wakeup_Enable(TEMP_ABOVE_LOW_THRESHOLD);
    Wakeup_Enable(TEMP_ABOVE_HIGH_THRESHOLD);
    Wakeup_Enable(TEMP_BELOW_HIGH_THRESHOLD);
    Wakeup_Enable(TEMP_OUT_OF_RANGE);
    Wakeup_Enable(TEMP_IN_RANGE);

    Wakeup_Disable(TEMP_BELOW_LOW_THRESHOLD);
    Wakeup_Disable(TEMP_ABOVE_LOW_THRESHOLD);
    Wakeup_Disable(TEMP_ABOVE_HIGH_THRESHOLD);
    Wakeup_Disable(TEMP_BELOW_HIGH_THRESHOLD);
    Wakeup_Disable(TEMP_OUT_OF_RANGE);
    Wakeup_Disable(TEMP_IN_RANGE);

    scratchpad_example();
    sys_reset_cpu_example();
    sys_sleep_with_example();
    sys_deep_sleep_with_example();
    atomic_section_example();
    debug_output_example();

    return 0;
    asm("; main end"); /* to check that no functions are eliminated due to noreturn calls */
}

/* scratchpad RAM (Battery Backup RAM) */
uint16_t arr16[8] SP_MEM;
uint8_t  arr8[8]  SP_MEM;

void scratchpad_example (void)
{
    arr16[0] = 0x600Du;
    arr16[1] = 0xCAFEu;

    arr8[0] = 0xFEu;
    arr8[1] = 0xEDu;
}

/* syslib.h */
void sys_reset_cpu_example(void) {
    Sys_ResetCpu();                 /* NB: noreturn call */
}

void sys_sleep_with_example (void) {
    Sys_SleepWith(LF_DISABLED);     /* NB: noreturn call */
}

void sys_deep_sleep_with_example (void) {
    Sys_DeepSleepWith(LF_DISABLED); /* NB: noreturn call */
}

/* atomic.h */
volatile uint16_t value_atomic;
void atomic_section_example (void)
{
        ENTER_SECTION(ATOMIC_KEEP_MODE);
        value_atomic++;
        EXIT_SECTION();

        ENTER_SECTION(ATOMIC_SYSTEM_MODE);
        value_atomic--;
        EXIT_SECTION();

        ENTER_SECTION(SYSTEM_MODE);
        value_atomic = 0xFFFFu;
        EXIT_SECTION();
}

void debug_output_example (void)
{
    DEBUG_PIN_PULSE(2u);
    DEBUG_PIN_TOGGLE(1u);
    DEBUG_PUT_WORD_HEX(0xCAFEu);
    DEBUG_PUT_WORD(0xCAFE);
    DEBUG_PUT_STRING("Cafe\n");
}

__attribute__((interrupt))
void ISR_AWD_ATT (void)
{
    AbsWatchdog_Restart();
}

/* aliasing */
void ISR_MLX16_SOFT (void) __attribute__ ((alias("SW_Int_Handler")));

__attribute__((interrupt))
void SW_Int_Handler (void)
{
    NOP();
}

/* EOF */
