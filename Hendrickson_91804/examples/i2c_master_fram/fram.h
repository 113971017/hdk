/*
 * Copyright (C) 2018-2022 Melexis N.V.
 *
 * Software Platform
 *
 */

#ifndef FRAM_H
#define FRAM_H

#include <platformlib.h>
#include <sensorlib.h>
#include <stdlib.h>
#include "i2c_sw_master.h"

#define FRAM_ADDR               (0x50u)
#define FRAM_CMD_DATA_WR        (FRAM_ADDR << 1u)
#define FRAM_CMD_DATA_RD        ((FRAM_ADDR << 1u) + 1u)

#define FRAM_RESERVED_ADDR      (0x7Cu)
#define FRAM_RESERVED_CMD_ID_WR (FRAM_RESERVED_ADDR << 1u)
#define FRAM_RESERVED_CMD_ID_RD ((FRAM_RESERVED_ADDR << 1u) + 1u)

#define FRAM_SLEEP_ADDR         (0x43u)
#define FRAM_CMD_SLEEP_WR       (FRAM_SLEEP_ADDR << 1u)

#define FRAM_WAKEUP_NTIMEOUT    (10u)

/*
    @brief FRAM device ID type
*/
typedef union __attribute__((packed)) {
    struct __attribute__((packed)) Fram_DeviceId_s
    {
        uint16_t    Manufacturer_Id : 12;
        uint8_t     Density         : 4;    
        uint8_t     Variation       : 5;
        uint8_t     Die_Revision    : 3;
    } as_fields;
    uint8_t as_bytes[3];
} Fram_DeviceId_t;

/*
    @brief FRAM memory address type
*/
typedef union {
    uint16_t Address;
    struct __attribute__((packed, aligned(2))) Fram_MemAddress_bytes_s
    {
        uint8_t AddressLSB;
        uint8_t AddressMSB;
    } as_bytes;
} Fram_MemAddress_t;

/*
    @brief Wake-up the FRAM device

    @return
        The routine checks that the device has woken up by sending write commands
        until they are acknowledged by the FRAM.
        It is tried a certain number of times (as defined by @c FRAM_WAKEUP_NTIMEOUT).
        If no acknowledge is received by the end of the trials, the function returns "true".
        Otherwise, in case of no problem, the function returns "false".
*/
extern bool Fram_WakeUp(void);

/*
@brief Read FRAM device ID

    @param device_id Pointer to device ID structure which will be updated with the result.
    @return
        If "true" the ID is successfully read, "false" if the operation failed.
*/
extern bool Fram_ReadId(Fram_DeviceId_t *device_id);

/*
    @brief Put FRAM in SLEEP mode
    @return
        If "true" the FRAM memory is put in its sleep mode, "false" if the operation failed.
*/
extern bool Fram_Sleep(void);

/*
    @brief Write data into FRAM

    @param address Memory address at which the data will start to be written
    @param tx_data_buffer Data (bytes array) to be written
    @param nbytes Number of bytes to be written

    @return
        If "true" finished successfully, "false" if the operation failed.
*/
extern bool Fram_WriteBytes(const Fram_MemAddress_t address,
                            const uint8_t *const tx_data_buffer,
                            const uint8_t nbytes);

/*
    @brief Read data from FRAM

    @param address Memory address at which the data will start to be read
    @param rx_data_buffer Buffer that will receive the read data bytes
    @param nbytes Number of bytes to be read

    @return
        If "true" if finished successfully, "false" if the operation failed.
*/
extern bool Fram_ReadBytes(const Fram_MemAddress_t address,
                           uint8_t *const rx_data_buffer,
                           const uint8_t nbytes);

#endif /* FRAM_H */
