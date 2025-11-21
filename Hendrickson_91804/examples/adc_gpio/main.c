/*
 * Copyright (C) 2016-2022 Melexis N.V.
 *
 * Software Platform
 */

/*
    Implements a single-ended voltage measured on GPIO1 pin
    If DEBUG mode is enabled in the makefile
    GPIO0 is used to print out via SW UART the measured value
    Otherwise, a negative pulse on
    GPIO0 shows a duration of the Adc_ReadGpio function execution
    GPIO2 generates a positive pulse with duration proportional to the voltage on GPIO1

    Channels:
    SENS_CH_SEL_A = ADC_CH_GPIO1
    SENS_CH_SEL_B = ADC_CH_VSSA

    References:
    SENS_REFH_SEL = ADC_REFH_VBG (or ADC_REFH_VDD)
    SENS_REFL_SEL = ADC_REFL_VSSA
*/

#include <sensorlib.h>
#include <platformlib.h>
#include <debug.h>

#if 1
#define HAS_CUSTOM_GPIO_AVERAGING
#endif

#if !defined (DEBUG_ENABLED)
#define WRITE_GPIO0_0()     Gpio_WritePin(GPIO0, 0)
#define WRITE_GPIO0_1()     Gpio_WritePin(GPIO0, 1)
#else
/* empty definitions are used to disable the functions in case of DEBUG mode */
#define WRITE_GPIO0_0()
#define WRITE_GPIO0_1()
#endif /* DEBUG_ENABLED */

/*
 * main
 */
int main(void)
{
    Gpio_SetPinMode(GPIO0, GPIO_DIG_OUTPUT_1);
    Gpio_SetPinMode(GPIO2, GPIO_DIG_OUTPUT_0);

    while (true)
    {
        volatile uint16_t raw_v;
        WRITE_GPIO0_0();            /* negative edge on GPIO0 if debug mode is disabled */
        raw_v = Adc_ReadGpio(GPIO1);
        WRITE_GPIO0_1();            /* positive edge on GPIO0 if debug mode is disabled */
        DEBUG_PUT_WORD_HEX(raw_v);  /* print out the measured value (in debug mode only) */

        Gpio_WritePin(GPIO2, 1);
        delay_nops(raw_v);          /* It converts the measured voltage to delay time */
        Gpio_WritePin(GPIO2, 0);
        /* Acknowledge the watchdog if needed: */
        DEBUG_ACK_AWD();
    }

    return 0;
}

/*--------------------------------------------------------------------------*/

#if defined (HAS_CUSTOM_GPIO_AVERAGING)

extern uint16_t adc_ll_read_voltage_once(void);
extern uint16_t adc_ll_read_gpio(void);

#define  ADC_GPIO_VALUES    (4u)

/*
    Override the library version of the function adc_ll_read_gpio()
    by the custom one below that implements a multiple ADC measurement
    with the next averaging
*/

uint16_t adc_ll_read_gpio(void) {

    uint16_t vgpio;
    uint32_t vgpio_average = 0;

    for (uint16_t i = 0; i < ADC_GPIO_VALUES; i++) {
        vgpio_average += adc_ll_read_voltage_once();
    }
    vgpio = vgpio_average / ADC_GPIO_VALUES;

    return vgpio;
}
#endif /* HAS_CUSTOM_GPIO_AVERAGING */

/* EOF */
