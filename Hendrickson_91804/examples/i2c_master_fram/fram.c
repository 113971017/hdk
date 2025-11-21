/*
 * Copyright (C) 2018-2021 Melexis N.V.
 *
 * Software Platform
 *
 */

#include "fram.h"

/* ----------------------------------------------------------------------------
 * Fram_WakeUp() is to wake-up the FRAM device.
 * Arguments:   none
 * Returns:     "true" if wake-up is completed, "false" if no acknowledge is received.
 * Notes: The routine checks that the device has woken up by sending write commands
          until they are acknowledged by the FRAM. It is tried a certain number of times
          defined by @c FRAM_WAKEUP_NTIMEOUT.
          If no acknowledge is received by the end of the trials, the function returns "true".
          Otherwise, in case of no problem, the function returns "false".
 */
bool Fram_WakeUp(void)
{
    uint8_t tx_status;
    uint16_t ntimeout = FRAM_WAKEUP_NTIMEOUT;

    I2C_sw_Start();
    do {
        tx_status = I2C_sw_TransmitByte(FRAM_CMD_DATA_WR);
        if (tx_status == 0u) {
            break;
        } /* else continue */
        else {
            I2C_sw_Restart();
        }
        ntimeout -= 1u;
    } while (ntimeout != 0u);
    I2C_sw_Stop();

    return (ntimeout == 0u) ? false : true;
} /* Fram_WakeUp */

/* ----------------------------------------------------------------------------
 * Fram_ReadId() is to read the FRAM device ID.
 * Arguments: "device_id" is a pointer to the device ID structure
              to be updated with a result.
 * Returns:   "true" if the ID is successfully read, "false" if the operation failed.
 */
bool Fram_ReadId(Fram_DeviceId_t *device_id)
{
    uint16_t tx_status;
    uint8_t tx_buffer[2] = {
        FRAM_RESERVED_CMD_ID_WR,
        FRAM_CMD_DATA_RD
    };

    I2C_sw_Start();
    tx_status = I2C_sw_TransmitBytes(tx_buffer, 2u);

    /* If some bytes have not been acknowledged, generate I2C STOP condition */
    if (tx_status > 0u) {
        I2C_sw_Stop();
        return false;
    } /* else continue */

    I2C_sw_Restart();
    tx_status = I2C_sw_TransmitByte(FRAM_RESERVED_CMD_ID_RD);

    /* If some bytes have not been acknowledged, generate I2C STOP condition */
    if (tx_status > 0u) {
        I2C_sw_Stop();
        return false;
    } /* else continue */

    I2C_sw_ReceiveBytes(device_id->as_bytes, 3u, true);
    I2C_sw_Stop();

    return true;   
} /* Fram_ReadId */

/* ----------------------------------------------------------------------------
 * Fram_Sleep() puts the FRAM device in its sleep mode.
 * Arguments: None
 * Returns:   "true" if the FRAM memory is put in its sleep mode, "false" if the operation failed.
 */
bool Fram_Sleep(void)
{
    uint16_t tx_status;
    uint8_t tx_buffer[2] = {
        FRAM_RESERVED_CMD_ID_WR,
        FRAM_CMD_DATA_WR
    };

    I2C_sw_Start();
    tx_status = I2C_sw_TransmitBytes(tx_buffer, 2u);

    if (tx_status > 0u) {   /* If some bytes have not been acknowledged, */
        I2C_sw_Stop();         /* generate I2C STOP condition */
    }
    else {                  /* Otherwise, transmit FRAM_CMD_SLEEP_WR command */
        I2C_sw_Restart();
        tx_status = I2C_sw_TransmitByte(FRAM_CMD_SLEEP_WR);
        I2C_sw_Stop();
    }

    return (tx_status > 0u) ? false : true;

} /* Fram_Sleep */

/* ----------------------------------------------------------------------------
 * Fram_WriteBytes() writes data into FRAM
 * Arguments:
            - "address" is a FRAM memory address at which the data will start to be written.
            - "tx_data_buffer" points to the data array with the bytes to be written.
            - "nbytes" specifies a number of bytes to be written.
 * Returns:   "true" if finished successfully, "false" if the operation failed.
 */
bool Fram_WriteBytes(const Fram_MemAddress_t address,
    const uint8_t *const tx_data_buffer,
    const uint8_t nbytes)
{
    uint16_t tx_status;
    uint8_t tx_buffer[3] = {
        FRAM_CMD_DATA_WR,
        address.as_bytes.AddressMSB,
        address.as_bytes.AddressLSB
    };

    I2C_sw_Start();
    tx_status = I2C_sw_TransmitBytes(tx_buffer, 3u);

    if (tx_status > 0u) {   /* If some bytes have not been acknowledged, */
        I2C_sw_Stop();         /* generate I2C STOP condition */
        return false;
    } /* else continue */

    tx_status = I2C_sw_TransmitBytes(tx_data_buffer, nbytes);
    I2C_sw_Stop();

    return (tx_status > 0u) ? false : true;
} /* Fram_WriteBytes */

/* ----------------------------------------------------------------------------
 * Fram_ReadBytes() is read data from FRAM.
 * Arguments:
            - "address" is a memory address at which the data will start to be read.
            - "rx_data_buffer" points to the buffer assigned to collect the read data bytes.
            - "nbytes" specifies a number of bytes to be read.
 * Returns:   "true" if finished successfully, "false" if the operation failed.
 */
bool Fram_ReadBytes(const Fram_MemAddress_t address,
    uint8_t *const rx_data_buffer,
    const uint8_t nbytes)
{
    uint16_t tx_status;
    uint8_t tx_buffer[3] = {
        FRAM_CMD_DATA_WR,
        address.as_bytes.AddressMSB,
        address.as_bytes.AddressLSB
    };

    I2C_sw_Start();
    tx_status = I2C_sw_TransmitBytes(tx_buffer, 3u);

    if (tx_status > 0u) {   /* If some bytes have not been acknowledged, */
        I2C_sw_Stop();         /* generate I2C STOP condition */
        return false;
    } /* else continue */

    I2C_sw_Restart();
    tx_status = I2C_sw_TransmitByte(FRAM_CMD_DATA_RD);

    if (tx_status > 0u) {   /* If some bytes have not been acknowledged, */
        I2C_sw_Stop();         /* generate I2C STOP condition */
        return false;
    } /* else continue */

    I2C_sw_ReceiveBytes(rx_data_buffer, nbytes, true);
    I2C_sw_Stop();

    return true;
} /* Fram_ReadBytes */

/* EOF */

