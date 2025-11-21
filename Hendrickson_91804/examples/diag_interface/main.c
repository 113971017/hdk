/*
 * Copyright (C) 2016-2022 Melexis N.V.
 *
 * Software Platform
 */

/*
 * Diagnostic interface example
 *
 * - Checks VDD voltage via Diagnostic Interface (channel DIAG_CH_VDD_X_0_25)
 * - Generates 1 GPIO0 pulse  if voltage is below low  threshold (2.5V; configurable)
 * - Generates 2 GPIO0 pulses if voltage is between low and high thresholds
 * - Generates 3 GPIO0 pulses if voltage is above high threshold (2.9V; configurable)
 */

#include <sensorlib.h>
#include <platformlib.h>
#include <diag.h>

/*
    V_in_lsb equals to [ (VDD/4) / VBG ] * VBG_in_lsb and equals to
    (VDD * VBG_in_lsb) / (VBG*4)
 */
#define VBG_IN_MV       (1200ul)    /* bandgap voltage; fixed */
#define VBG_IN_LSB      (127ul)     /* (2**7 - 1); fixed */
#define VDD_DIVIDER     (4ul)       /* divider of VDD voltage; fixed */

#define DIV_ROUND_2(a, b)     ( ( (a) + ((b)/2u) ) / (b) )

#define REF_MV_TO_LSB_2(v)    DIV_ROUND_2((uint32_t)(v) * (uint32_t)VBG_IN_LSB, (uint32_t)VBG_IN_MV * (uint32_t)VDD_DIVIDER)

#define REF_2V5    (DiagThreshold_Type)(REF_MV_TO_LSB_2(2500ul))
#define REF_2V9    (DiagThreshold_Type)(REF_MV_TO_LSB_2(2900ul))

extern volatile uint8_t port_en_vbg_out_ __attribute__((addr(IO_BYTE_ADDRESS(CUSTOM_91804, FRC_EN_VBG_OUT))));
extern volatile uint8_t port_en_ibg_3V_  __attribute__((addr(IO_BYTE_ADDRESS(CUSTOM_91804, FRC_EN_IBG_3V))));

STATIC volatile DiagStatusAll_Type v;

STATIC void delay_ms(uint16_t ms);
STATIC void led_generate_pulse(uint16_t n);
STATIC void led_init(void);

/*
 * Main
 */
int main(void)
{
    Itc_Enable(ITC_AWD_ATT);

    /* Enable diagnostic interface currents/references */
    port_en_vbg_out_ |= 4u;
    port_en_ibg_3V_  |= 4u; /* Current for diag ref buffer and comparators */

    Diag_SetReferenceSource(DIAG_REF_VBG);
    Diag_ConfigChannel(DIAG0, DIAG_CH_VDD_X_0_25, REF_2V5, REF_2V9, DIAG_HYST_OFF);
    Diag_SetMode(DIAG0, DIAG_ENABLED_WITH_NO_EVENTS);

    led_init();

    while (true) {
        uint16_t npulses;
        DiagStatusAll_Type status = Diag_GetStatusAll();

        switch (status.DIAG0) {
        case DIAG_STATUS_BELOW_RANGE:   npulses = 1u;   break;
        case DIAG_STATUS_IN_RANGE:      npulses = 2u;   break;
        case DIAG_STATUS_ABOVE_RANGE:   npulses = 3u;   break;
        default:                        npulses = 10u;  break;
        }

        led_generate_pulse(npulses);

        delay_ms(500u);
    }

    return 0;
}

/*
*/
STATIC void led_init(void)
{
    Gpio_WritePin(GPIO0, 0u);
}

/*
*/
STATIC void led_generate_pulse(uint16_t n)
{
    while (n-- != 0u) {
        Gpio_WritePin(GPIO0, 1u);
        delay_ms(50u);
        Gpio_WritePin(GPIO0, 0u);
        delay_ms(50u);
    }
}

/*
*/
STATIC void delay_ms(uint16_t ms)
{
    while (ms-- != 0u) {
        delay_nops(2u * 1000u);     /* 1ms @ 8 MHz */
    }
}

/*
 * Watchdog Attention Interrupt handler
 *
 * @note
 * - Watchdog is acknowledged unconditionally for the purpose of the test example
 */
__attribute__((interrupt))
void ISR_AWD_ATT(void)
{
    AbsWatchdog_Restart();
}

/* EOF */
