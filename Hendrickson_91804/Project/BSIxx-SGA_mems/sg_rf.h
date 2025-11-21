#ifndef SG_RF_H
#define SG_RF_H

#define SUPPLIER_ID                 (15u)

void init_rf ( void );
void RftxSend_VDA ( bool ACalibNeeded, uint16_t Status_Bytes);

#endif
