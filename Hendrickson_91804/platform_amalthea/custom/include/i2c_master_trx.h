/*
 * Copyright (C) 2019-2021 Melexis N.V.
 *
 * Software Platform
 *
 */

#ifndef I2C_MASTER_TRX_H
#define I2C_MASTER_TRX_H

#include <i2c_master.h>

/**
    @defgroup i2c_master_trx Transferring I2C master functions

    Group of the I2C functions responsible for the data transmitting and receiving.
    The functions provide the SCL rate about **206** kHz (without stretching)
    at the HFO frequency 8 MHz.
    Note that this software module is deprecated and hence is not recommended for
    the new designs. Please, use a code extracted from the i2c_master_fram example
    instead.
 */

#if !defined (__ASSEMBLER__)
#include <platformlib.h>

/** @ingroup i2c_master_trx
@brief Transmits several consecutive bytes on the I2C bus

@param I2C_tx_bytes_table Pointer to table of bytes to be sent
@param I2C_nbytes Number of bytes to send

@return Status:
- 0 = Transmission was complete without error
(meaning that all bytes were acknowledged by slave)
- 1 or more = Error occurred and indicates how many bytes have not been
transmitted 

@note Defining the macro @c I2C_IGNORE_NACK will avoid that a transmission
is aborted when an ACK is missing. To be used depending on the wanted
behaviour.

@deprecated Since the library version 1.10.5.
*/
uint16_t I2C_TransmitBytes(const uint8_t *I2C_tx_bytes_table, uint8_t I2C_nbytes) __attribute__((deprecated));

/** @ingroup i2c_master_trx
@brief Transmit one single byte on the I2C bus

@param I2C_byte Value of byte to be sent

@return Status value:
- 0 = Transmission was complete without error (meaning that transmitted
byte was acknowledged by slave)
- 1 = Error occurred (not acknowledged)

@deprecated Since the library version 1.10.5.
*/
uint8_t I2C_TransmitByte(const uint8_t I2C_byte) __attribute__((deprecated));

/** @ingroup i2c_master_trx
@brief Transmit one word (2 bytes) on the I2C bus

@param I2C_word Value of word to be sent

@return Status value:
- 0 = Transmission was complete without error (meaning that transmitted
bytes were both acknowledged by slave)
- 1 = Error occurred (1st or 2nd byte not acknowledged)

@deprecated Since the library version 1.10.5.
*/
uint16_t I2C_TransmitWord(const uint16_t I2C_word) __attribute__((deprecated));

/** @ingroup i2c_master_trx
@brief Receive several consecutive bytes on the I2C bus.

@param I2C_rx_bytes_table Pointer to RX buffer.
@param I2C_nbytes Number of bytes to send.
@param I2C_nack_last_byte If true, last received byte is not acknowledged.

@deprecated Since the library version 1.10.5.
*/
void I2C_ReceiveBytes(uint8_t *I2C_rx_bytes_table, uint8_t I2C_nbytes, bool I2C_nack_last_byte) __attribute__((deprecated));

#endif /*__ASSEMBLER__*/
#endif /*I2C_MASTER_TRX_H*/
/* EOF */
