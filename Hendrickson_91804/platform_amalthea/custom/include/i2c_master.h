/*
 * Copyright (C) 2019-2021 Melexis N.V.
 *
 * Software Platform
 *
 */

#ifndef I2C_MASTER_H
#define I2C_MASTER_H

/**
    @defgroup i2c_master I2C Master Interface, <i2c_master.h>, <i2c_master_trx.h>

    This group of functions provides both sending and receiving a data over the I2C master interface
    with the clock stretching support. 
    The implementation is done fully on SW level that uses two GPIO pins:
    - SCL - GPIO0
    - SDA - GPIO1
    
    The I2C software module requires the next header files:
    - @ref i2c_master_init "i2c_master.h" responsible for the I2C communication start, stop and restart.
    - @ref i2c_master_trx "i2c_master_trx.h" supporting the I2C master functions responsible for the data exchange.

    @warning
    - This software module is deprecated and hence is not recommended for the new designs.
      Please, use a code extracted from the i2c_master_fram example instead.
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
    - In case of too high capacitive load, the SCL and SDA lines should be pulled-up by external resistors
      or by resistors inside of the external I2C device.
 */

/*  The hidden part of the documentation describing the RAM-based functions:
    - @ref i2c_master_trx_cached "i2c_master_trx_cached.h" supporting the RAM-based I2C master functions
      responsible for the data exchange.
 */
 
/**
    @defgroup i2c_master_init I2C start and stop functions
    The functions responsible for the I2C communication start, stop and restart.
    Note that this software module is deprecated and hence is not recommended for
    the new designs. Please, use a code extracted from the i2c_master_fram example instead.
 */

#define I2C_PIN_CFG_PP 0
#define I2C_PIN_CFG_PU 1
#define I2C_PIN_CFG_HZ 2
#define I2C_PIN_CFG_HZ_SDA 3

/******************************************************************************
Define options
*******************************************************************************/
/******************************************************************************
Option to debug without external pull-ups required
*******************************************************************************/
/* If defined, the I2C_DEBUG_NO_EXTERNAL_COMPONENTS makes sure that the functionality
   of the I2C master can be debugged/tested without external components */
//#define I2C_DEBUG_NO_EXTERNAL_COMPONENTS

/******************************************************************************
Options to enable (partial) clock stretching
*******************************************************************************/
#define I2C_CLK_STRETCH_TX_ACK
#define I2C_CLK_STRETCH_RX_BYTE

/******************************************************************************
GPIO pins configuration
*******************************************************************************/
/* The following is mandatory and defines the configuration of the GPIO
pins implementing the SDA and SCL connection:

I2C_PIN_CFG_PP:     Push-pull configuration (also the default if not defined)
                    Use this configuration for debugging when no external bus with
                    pull-up resistors connected is available.

I2C_PIN_CFG_PU:     Pull-up configuration. With this configuration, both SDA and SCL
                    GPIO pins are configured using internal pull-up configuration
                    mark symbols. It does not work well and is not recommended
                    for a case with a big capacitive load.
                    Nevertheless, this configuration is set by default because
                    it can be robust and cost-efficient with resistors inside
                    of the external I2C device.

I2C_PIN_CFG_HZ:     Mark symbols are corresponding to Hi-Z configuration for both
                    SDA and SCL pins. External pull-up resistors are required on both.

I2C_PIN_CFG_HZ_SDA: SCL is configured in push-pull while the SDA line is conf-
                    igured to be Hi-Z for mark symbols. External pull-up resistor
                    is required on SDA only.
*/
#ifdef I2C_DEBUG_NO_EXTERNAL_COMPONENTS
    #define I2C_PINS_CONFIG I2C_PIN_CFG_PP
#else
    #define I2C_PINS_CONFIG I2C_PIN_CFG_PU
#endif

/******************************************************************************
Option to ignore NACK
*******************************************************************************/
/* If defined, the I2C_IGNORE_NACK allows to ignore NACK and continue to
send data on the I2C bus even if no slave acknowledges the reception.
Use this for debug when no slave device is connected.
*/
//#define I2C_IGNORE_NACK

#if !defined (__ASSEMBLER__)
#include <platformlib.h>

/** @ingroup i2c_master_init
@brief Initiates an I2C communication

1. Configures GPIO0 as SCL and GPIO1 as SDA.
2. Configures the bus in pull-up mode and waits till it becomes free (with timeout).
3. As soon as the bus is free, it sends a START sequence on the bus.
4. Both SCL and SDA are pulled-down at the end.

@return Status value:
- 0 = Bus was free, no error
- 1 = Bus was not free. START symbol not sent.

@deprecated Since the library version 1.10.5.
*/
uint16_t I2C_Start(void) __attribute__((deprecated));

/** @ingroup i2c_master_init
@brief Re-sends the START sequence
@note Instead of the STOP condition, by executing this function, the master can generate
      a repeated START condition. Like a START condition, to generate a repeated START condition,
      the master changes the SDA line from one to zero while the SCL line is HIGH.
      In this case, the I2C bus remains busy. To prepare for the repeated START condition,
      the master sets the SDA line to one during the LOW phase of the SCL line.

@deprecated Since the library version 1.10.5.
*/
void I2C_Restart(void) __attribute__((deprecated));

/** @ingroup i2c_master_init
@brief Closes an I2C communication  by sending a STOP sequence on the bus.

@note The function generates a STOP condition by changing the SDA line from zero to one
      while the SCL line is HIGH. The I2C bus is considered free after the STOP condition.
      So, both SCL and SDA lines are kept free (pulled-up) at the end.
      To prepare for the STOP condition, the master sets the SDA line to zero in advance.

@deprecated Since the library version 1.10.5.
*/
void I2C_Stop(void) __attribute__((deprecated));

#endif /*__ASSEMBLER__*/
#endif /*I2C_MASTER_H*/
/* EOF */
