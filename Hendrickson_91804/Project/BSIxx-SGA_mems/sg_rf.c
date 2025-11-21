#include <sensorlib.h>
#include <platformlib.h>
#include "sg_rf.h"

#include "main.h"

extern volatile uint16_t FSensP;
extern volatile uint16_t FSensT;
extern volatile uint16_t FSensV;
extern uint16_t Flow_chipID;
extern uint16_t Fhigh_chipID;

extern uint16_t CNT_ACQ;
extern int16_t FMeasThis_X90;
extern int16_t FMeasThis_X270;
extern int16_t FMeas_1;
extern int16_t FMeas_Acc_HWF_1;
extern int16_t FMeas_Acc_HWF_2;
extern uint16_t F_Speed_changed; //flag to indicate change of speed
extern uint16_t FT_Speed;

//VDA format define for packt_ID = 0xC2
static const RfTxConfig_Type CRfCfg =
    {{  /* Note: All unconfigured fields keeps their default zero values */
        .RFTX_PATTERN_LEN = 1,      /* 2 bits a length of the synchronization pattern : 0 = 8 bits, 1 = 16 bits, 2 = 24 bits, 3 = 32 bits */
        .RFTX_MULTI_FRAME = 0,      /* 1 bit, not used (Enable the transmission of packets consisting of multiple frames) */
/* 3-bit of modulation source selection:
0 = Fixed logic 0, 1 = All raw data from FIFO, no preamble, sync pattern or packet
2 = Preamble and sync pattern, no packet, 3 = Preamble, sync pattern and packet,
4 = GPIO0 input, 5 = GPIO1 input, 6 = GPIO2 input, 7 = GPIO3 input */
        .RFTX_DIRECT_MOD = 3,
        .RF_EN_PD = 0,              /* 1 bit to enable RF TX power detector (if it's 1) */

/* 2 bits coding the time to wait before declaring PLL in-lock: 0 = 40 us, 1 = 80 us, 2 = 160 us, 3 = 320 us */
        .RFTX_WAIT_LOCK = 3,

        .RFTX_PREAMBLE = 0x55,      /* 8-bit preamble pattern: 0x55 corresponds to 0xF in Manchester */
        .RFTX_PREAMBLE_LEN = 4,     /* 8-bit that defines a length of the preamble to be transmitted (number+1 of RFTX_PREAMBLE bytes in NRZ) */

        .RFTX_PATTERN_L = 0x55A6,   /* Low  word of 32-bit RF TX synchronization word (in NRZ): 0x55A6 corresponds to 0xF2 in Manchester */
        .RFTX_PATTERN_H = 0x0000,   /* High word of 32-bit RF TX synchronization word */
        .RFTX_PACKET_LEN = 14, 		// 8-bit packet length in fixed packet length mode for Packet ID = C2 version
									// 1 byte packet ID 4 bytes UID, 3 bytes PVT, 4 bytes acceleration 2 bytes status

        .RFTX_PACKET_LEN_CORR = 0,  /* 4-bit signed word that specifies how the packet length should be corrected */
/* 2-bit position of the packet length byte, starting from the synchronization word (only applicable when the packet length is not fixed) */
        .RFTX_PACKET_LEN_POS = 0,
        .RFTX_PACKET_LEN_FIX = 1,   /* Packet length: 0 = variable, 1 = fixed */

        .RFTX_ADDRESS = 0x00,       /* 8-bit address to be transmitted when address byte is enabled */
        .RFTX_EN_ADDRESS = 0,       /* Add address byte to transmitted packets */

        .RFTX_CRC_INV = 0,          /* Optional inversion of the CRC value */
        .RFTX_CRC_16_N8 = 0,        /* CRC length and type: 0 = 8 bit, Baicheva polynomial, 1 = 16 bit, CCITT */
        .RFTX_EN_CRC = 1,           /* Add CRC to transmitted packets */
        .RFTX_CRC_INIT = 0xAA00,    /* 16-bit initial value for CRC calculation. In case of 8-bit CRC, the init value is in RFTX_CRC_INIT[15:8] */

/* Baud rate = Crystal frequency / ((240 + RFTX_DR_MANT) * 2^(RFTX_DR_EXP-1)), half of this for Manchester encoding */
        .RFTX_DR_MANT = 0x63,       /* 8-bit mantissa to set the baud rate, set for 38400 baud, Fcrystal = 26 MHz */
        .RFTX_DR_EXP = 0x02,        /* 3-bit exponent to set the baud rate, set for 38400 baud, Fcrystal = 26 MHz */
        .RFTX_LSB_FIRST = 0,        /* Select the bit order: 0 = MSB first, 1 = LSB first */
        .RFTX_EN_MANCHESTER = 1,    /* Enable Manchester coding of the data bits: 0 = NRZ, 1 = Manchester */
/* TX ramp up/down duration: 0 = no ramp up/down, 1 = 40us, 2 = 80us, 3 = 160us, 4 = 320us, 5 = 640us, 6 = 1.3ms, 7 = 2.6ms */
        .RFTX_RAMP = 0,

        .RFTX_MANCHESTER_STOP_WORD = 0xFF,  /* 8-bit stop word for Manchester encoding */
/* 2-bit length of stop word in case of Manchester encoding: 0 = 0 bit, i.e. no stop word, 1 = 2 bits, 2 = 4 bits, 3 = 8 bits */
        .RFTX_MANCHESTER_STOP_LEN = 2,

/* 4-bit mantissa of the coefficient that multiplies the signal, equal to (16 + RFTX_MULT_MANT) / 16 */
        .RFTX_MULT_MANT = 8,
/* 4-bit exponential scaling to be applied to the pulse shaping coefficients by shifting them in the modulator, equivalent to 2^RFTX_MULT_EXP */
        .RFTX_MULT_EXP = 2,
/* Invert the polarity of the data bits if set. In case of Manchester encoding: 1 = direct, 0 = inverse */
        .RFTX_BIT_INVERT = 1,
        .RFTX_PN9_MODE = 0,         /* Select TI-compatible PN9 sequence */
        .RFTX_PN9_REVERSE = 0,      /* Generate PN9 sequence: 0 = LSB first, 1 = MSB first (TI-compatible) */
        .RFTX_EN_DATAWHITE = 0,     /* Enable data whitening (if it's = 1) */
/* Interpolator on TX modulator output: 0 = OFF; 1 = ON, at 13 MHz; 2 = ON, at 6.5 MHz; 3 = ON, at 3.25 MHz */
        .RFTX_EN_INTERP = 0,
        .RFTX_EN_GAUSSIAN = 0,      /* Enable Guassian pulse shaping */
		.RFTX_FSK_NOOK = 1          /* Select the modulation type: 0 = OOK, 1 = FSK */
    }};



void init_rf ( void )
{
 RfTx_Init(&CRfCfg);
 RfTx_Calibrate();
 Radio_Disable();
}


void RftxSend_VDA (bool ACalibNeeded,uint16_t Status_Bytes )
{
 uint8_t VDA_Pressure;
 uint8_t VDA_Temperature;
 uint8_t VDA_Voltage;
 // for more information about RF format look at RfConfig.c
 RfTx_FifoFlush(); //flush FIFO

 // 1x Packet_ID 4xUID 2xMeas0 1xDirection 7x2xintermediate values 2xSensT 2xSensP 2xSensV 1xChecksum
 //1. fill in the Fifo with packet ID = 0xC@
 RfTx_FifoWriteByte(0xC2);

 //2. fill in the Fifo with UID
 //RfTx_FifoWriteByte((uint8_t)(Fhigh_chipID >> 8));
 RfTx_FifoWriteByte((SUPPLIER_ID << 4) | ((uint8_t)(Fhigh_chipID >> 8) & 0x0F));
 RfTx_FifoWriteByte((uint8_t)Fhigh_chipID);
 RfTx_FifoWriteByte((uint8_t)(Flow_chipID >> 8));
 RfTx_FifoWriteByte((uint8_t)Flow_chipID);

 //3. send one Byte Pressure in VDA format
#if 0	// org
 if (FSensP < 100)
	 VDA_Pressure = 0x01;
 else if (FSensP > 730)
	 VDA_Pressure = 0xFF;
#else

#if defined( SG_ENABLE_MEASURE_CHK_LIMIT_P )
 if (FSensP < PRESSURE_RANGE_LOW)
	 VDA_Pressure = PKG_PRESSURE_UNDERFLOW;
 else if (FSensP > PRESSURE_RANGE_HIGH)
	 VDA_Pressure = (uint8_t)PKG_PRESSURE_OVERFLOW;
#endif	//defined( SG_ENABLE_MEASURE_CHK_LIMIT_P )

#endif
 else
 {
	 FSensP = ((FSensP - 100) * (10 * 2)) / 25;
     if (0 == (FSensP & 1u))
    	 VDA_Pressure = (uint8_t)(FSensP >> 1) + 2;
     else
    	 VDA_Pressure = (uint8_t)(FSensP >> 1) + 2 + 1;
 }
 RfTx_FifoWriteByte(VDA_Pressure);

 //4. send one Byte Temperature in VDA format
 if (FSensT<(-40+52))
	 VDA_Temperature = 0x01;
 else if (FSensT > (125+52))
	 VDA_Temperature= 0xF8;
 else
	 VDA_Temperature = (uint8_t)FSensT;

 RfTx_FifoWriteByte(VDA_Temperature);

 //5. send one Byte of Voltage
 VDA_Voltage = (uint8_t)FSensV;
 RfTx_FifoWriteByte(VDA_Voltage);

 //6. send 2 Byte of Accelerometer X
 RfTx_FifoWriteByte((uint8_t)((FMeasThis_X90)&0xFF));
 RfTx_FifoWriteByte((uint8_t)((FMeasThis_X90)>>8));

 //7. send 2 Byte of Accelerometer Z
 RfTx_FifoWriteByte((uint8_t)((FMeas_1)&0xFF));
 RfTx_FifoWriteByte((uint8_t)((FMeas_1)>>8));

 //8. send 2 Byte of Status
 RfTx_FifoWriteByte((uint8_t)((Status_Bytes)&0xFF));
 RfTx_FifoWriteByte((uint8_t)((Status_Bytes)>>8));

 while ( (RfTx_GetStatus().RFTX_INFO == RFTX_POWERUP)         // Powering up (LDOs, XTAL generator, PLL etc.)
      || (RfTx_GetStatus().RFTX_INFO == RFTX_WAIT_FOR_PLL)    // Waiting for PLL lock
      || (RfTx_GetStatus().RFTX_INFO == RFTX_WAIT_FOR_FIFO)   // TX waiting for FIFO data
      || (RfTx_GetStatus().RFTX_INFO == RFTX_TRANS_PAYLOAD) ) // TX transmitting payload
 {  }// Wait the end of previous RFTx if it's not finished


 if (ACalibNeeded)
  {
   RfTx_Calibrate(); // check if necessary ....
  }
 else {} // Nothing to do in case the RFTx calibration is not requested

 // send RF message in backgnd of stop mode
 RfTx_Run();
}

