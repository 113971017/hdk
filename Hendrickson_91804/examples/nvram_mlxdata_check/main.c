/*
 * Copyright (C) 2020-2022 Melexis N.V.
 *
 * Software Platform
 */

/*
 * nvRAM data validation example is based on the 24-bit signature 
 * calculation according to the polynomial: x 24  + x 23  + x 22  + x 17
*/

#include <sensorlib.h>
#include <platformlib.h>
#include <debug.h>

/* Starting address is equal to the first accessed even address 
 * It has to be aligned on 32 bits words (2 LSB are ignored) */
#define ROMBIST_STARTING_ADDRESS  (0x1000u)
/* Stopping address is equal to the last address accessed */
#define ROMBIST_STOPPING_ADDRESS  (0x103Eu)

/* Internal functions needed just for this example */
STATIC uint16_t nibble_to_hex(uint8_t nibble);
STATIC void debug_put_24bit_hex(uint32_t data);

/* ----------------------------------------------------------------------------
 * Main
 */
int main(void)
{
    DEBUG_PIN_INIT(LOGIC_HIGH);
    Itc_Enable(ITC_AWD_ATT);

    /* Note: For the case when the input memory range of the function
     * Calc_RomBistFor corresponds to the nvRAM, the function NvRam_Recall()
     * has to be executed in advance to avoid the memory error.
     * It is not needed here (after the cold bootstrap), because
     * the NvRam_Recall() is always implemented during the cold bootstrap */
    uint32_t bist_signature = Calc_RomBistFor(ROMBIST_STARTING_ADDRESS,
                                              ROMBIST_STOPPING_ADDRESS);
    /* Print out the ROM BIST signature calculated for the assigned memory range */
    DEBUG_PUT_STRING("ROM BIST Signature = ");
    debug_put_24bit_hex(bist_signature);
    DEBUG_PUT_STRING("\n");

    /* Validate Melexis nvRAM data by calculation the BIST signature */
    DataCheck_Type test = Check_IsBadMlxData();
    if (test == DATA_CHECK_FAILED) {
        DEBUG_PUT_STRING("ROM BIST calculation FAILED");
    }
    else if (test == DATA_INVALID) {
        DEBUG_PUT_STRING("MLX nvRAM data is INVALID");
    }
    else {  /* test = DATA_VALID */
        DEBUG_PUT_STRING("MLX nvRAM data is VALID");
    }
    DEBUG_PUT_STRING("!\n");

    while (true) { /* Infinite loop */ }

    return 0;
} /* main */

/* --- Watchdog Attention Interrupt handler -------------------------------- */

__attribute__((interrupt))
void ISR_AWD_ATT(void)
{
    AbsWatchdog_Restart();
}

/* --- Internal functions needed just for this example --------------------- */

STATIC uint16_t nibble_to_hex(uint8_t nibble)
{
    uint16_t tmp;

    nibble &= 0x0Fu;    /* only the least signficant nibble */

    if (nibble > 9u) {
        tmp = (uint16_t)(nibble + (0x41u - 9u - 1u));   /* 0x41u corresponds to symbol 'A' */
    }
    else {
        tmp = (uint16_t)(nibble + 0x30u);               /* 0x30u corresponds to symbol '0' */
    }

    return tmp;
} /* nibble_to_hex */

STATIC void debug_put_24bit_hex(uint32_t data)
{
    uint16_t tmp16 = (uint16_t)(data >> 8);
    uint8_t  tmp8  = (uint8_t)data;
    DEBUG_PUT_WORD_HEX(tmp16);
    tmp16 = (nibble_to_hex(tmp8 >> 4) << 8) | nibble_to_hex(tmp8);
    DEBUG_PUT_WORD(tmp16);
} /* debug_put_24bit_hex */

/* EOF */
