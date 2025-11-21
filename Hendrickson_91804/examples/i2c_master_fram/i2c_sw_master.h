/*
 * Copyright (C) 2019-2024 Melexis N.V.
 *
 * Software Platform
 *
 */

#ifndef I2C_MASTER_H
#define I2C_MASTER_H

/*
    The functions of this SW module provides both sending and receiving a data
    over the I2C master interface with the clock stretching support. 
    The implementation is done fully on SW level that uses two GPIO pins:
    - SCL - GPIO0
    - SDA - GPIO1

    The functions can provide the SCL rate about **206** kHz at the HFO frequency 8 MHz
    (without stretching and artificial slowing)

    Warning
    - Melexis N.V. is supplying the I2C software module for use with the MLX16 processor embedded
      into the MLX91803/4/5 device only.
    - The I2C software module is provided "as is". Final validation to be performed by the customer
      with a final hardware configuration (i.e. on a customer's PCBA with dedicated I2C slave device).
    - I2C multi-master mode is not supported.
    - The application code has to keep the interruptions disabled during the I2C communication.
    - The GPIOs pins are configured with internal pull-up resistors to prevent electrical conflict
      with external I2C devices.
    - If GPIO0 and GPIO1 are used for other purposes except I2C, it is responsibility of application
      to care about their proper configuration.
    - In case of too high capacitive load, the SCL and SDA lines should be pulled-up
      by external resistors or by resistors inside of the external I2C device.
*/

/******************************************************************************
GPIO pins configuration
*******************************************************************************/
/* The following is mandatory and defines the configuration of the GPIO
pins implementing the SDA and SCL connection:

I2C_PIN_CFG_PU:     Pull-up configuration. With this configuration, both SDA and SCL
                    GPIO pins are configured using internal pull-up configuration
                    mark symbols. It does not work well and is not recommended
                    for a case with a big capacitive load.
                    Nevertheless, this configuration is set by default because
                    it can be robust and cost-efficient with resistors inside
                    of the external I2C device.

I2C_PIN_CFG_HZ:     Mark symbols are corresponding to Hi-Z configuration for both
                    SDA and SCL pins. External pull-up resistors are required on both.
*/
#define I2C_PINS_CONFIG I2C_PIN_CFG_PU

#define I2C_PIN_CFG_PU  (1)
#define I2C_PIN_CFG_HZ  (2)

#define I2C_NOPS_DELAY  (0)

#if I2C_NOPS_DELAY == 0
#define I2C_FASTEST_MODE
#endif

/******************************************************************************
Options to enable (partial) clock stretching
*******************************************************************************/
#define I2C_CLK_STRETCH_TX_ACK
#define I2C_CLK_STRETCH_RX_BYTE

/******************************************************************************
Option to ignore NACK
*******************************************************************************/
/* If defined, the I2C_IGNORE_NACK allows to ignore NACK and continue to
send data on the I2C bus even if no slave acknowledges the reception.
Use this for debug when no slave device is connected.
*/
//#define I2C_IGNORE_NACK

/*
 * GPIO configuration definition
 */
#define CUSTOM_91804_GPIO0_CH_SEL_ADDRESS (0x001AC)
#define CUSTOM_91804_GPIO0_ADDRESS        (0x001A8)
#define CUSTOM_91804_GPIO0_BITMASK        (0x01)
#define CUSTOM_91804_GPIO0_BIT            (0x00)

#define CUSTOM_91804_GPIO1_CH_SEL_ADDRESS (0x001AD)
#define CUSTOM_91804_GPIO1_BITMASK        (0x02)
#define CUSTOM_91804_GPIO1_BIT            (0x01)

#define TIMEOUT         (5000)

#define GPIO_DIG_IN_PU  (0x02)
#define GPIO_DIG_IN     (0x04)
#define GPIO_DIG_OUT_0  (0x05)
#define GPIO_DIG_OUT_1  (0x06)

#define GPIO_INPUT_ADDR CUSTOM_91804_GPIO0_ADDRESS
#define GPIO_INPUT      [X]
#define SCL_BITMASK     CUSTOM_91804_GPIO0_BITMASK
#define SDA_BITMASK     CUSTOM_91804_GPIO1_BITMASK
#define SCL_BIT         CUSTOM_91804_GPIO0_BIT
#define SDA_BIT         CUSTOM_91804_GPIO1_BIT

#define SCL_CFG_ADDR    CUSTOM_91804_GPIO0_CH_SEL_ADDRESS
#define SCL_CFG         [X+4]

#define SDA_CFG_ADDR    CUSTOM_91804_GPIO1_CH_SEL_ADDRESS
#define SDA_CFG         [X+5]

#define SCL_SDA_CFG     SCL_CFG /* to address word */

#if I2C_PINS_CONFIG == I2C_PIN_CFG_PU
/* Configuration with input and integrated pull-ups */
    #define GPIO_RELEASE GPIO_DIG_IN_PU
#else /* I2C_PINS_CONFIG == I2C_PIN_CFG_HZ */
/* Configuration with both SDA and SCL Hi-Z, requires external pull-ups */
    #define GPIO_RELEASE GPIO_DIG_IN
#endif


#if !defined (__ASSEMBLER__)
#include <platformlib.h>
#include <sensorlib.h>

/* I2C_Start() function initiates an I2C communication

1. Configures GPIO0 as SCL and GPIO1 as SDA.
2. Configures the bus in pull-up mode and waits till it becomes free (with timeout).
3. As soon as the bus is free, it sends a START sequence on the bus.
4. Both SCL and SDA are pulled-down at the end.

Return Status value:
- 0 = Bus was free, no error
- 1 = Bus was not free. START symbol not sent.
*/
uint16_t I2C_sw_Start(void);

/* I2C_Restart() function re-sends the START sequence

Note: Instead of the STOP condition, by executing this function, the master can generate
      a repeated START condition. Like a START condition, to generate a repeated START condition,
      the master changes the SDA line from one to zero while the SCL line is HIGH.
      In this case, the I2C bus remains busy. To prepare for the repeated START condition,
      the master sets the SDA line to one during the LOW phase of the SCL line.
*/
void I2C_sw_Restart(void);

/* I2C_Stop() function closes an I2C communication by sending a STOP sequence on the bus.

Note: The function generates a STOP condition by changing the SDA line from zero to one
      while the SCL line is HIGH. The I2C bus is considered free after the STOP condition.
      So, both SCL and SDA lines are kept free (pulled-up) at the end.
      To prepare for the STOP condition, the master sets the SDA line to zero in advance.
*/
void I2C_sw_Stop(void);

/* I2C_TransmitBytes() function transmits several consecutive bytes on the I2C bus

Param:
- I2C_tx_bytes_table is a pointer to table of bytes to be sent
- I2C_nbytes is a number of bytes to send

Return Status:
- 0 = Transmission was complete without error
(meaning that all bytes were acknowledged by slave)
- 1 or more = Error occurred and indicates how many bytes have not been
transmitted 

NOTE: By defining I2C_IGNORE_NACK it is posible to avoid that a transmission
      is aborted when an ACK is missing. To be used depending on application.
*/
uint16_t I2C_sw_TransmitBytes(const uint8_t *I2C_tx_bytes_table, uint8_t I2C_nbytes);

/* I2C_TransmitByte() function transmits one single byte on the I2C bus

Param: I2C_byte is a value of byte to be sent

Return Status value:
- 0 = Transmission was complete without error (meaning that transmitted
byte was acknowledged by slave)
- 1 = Error occurred (not acknowledged)
*/
uint8_t I2C_sw_TransmitByte(const uint8_t I2C_byte);

/* I2C_TransmitWord() function transmits one word (2 bytes) on the I2C bus

Param: I2C_word is a value of word to be sent

Return Status value:
- 0 = Transmission was complete without error (meaning that transmitted
bytes were both acknowledged by slave)
- 1 = Error occurred (1st or 2nd byte not acknowledged)
*/
uint16_t I2C_sw_TransmitWord(const uint16_t I2C_word);

/* I2C_ReceiveBytes() function receives several consecutive bytes on the I2C bus.

Param:
- I2C_rx_bytes_table is a pointer to RX buffer.
- I2C_nbytes is a number of bytes to send.
- I2C_nack_last_byte is a flag showing that the last received byte is not acknowledged, if set.
*/
void I2C_sw_ReceiveBytes(uint8_t *I2C_rx_bytes_table, uint8_t I2C_nbytes, bool I2C_nack_last_byte);

#endif /*__ASSEMBLER__*/
#endif /*I2C_MASTER_H*/
/* EOF */
