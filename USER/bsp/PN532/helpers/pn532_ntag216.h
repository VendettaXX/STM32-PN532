#ifndef __PN532_NTAG216_H__
#define __PN532_NTAG216_H__

#include "bsp_config.h"
#include "pn532_mifare.h"
#include "pn532.h"

pn532_error_t pn532_mifareultralight_WaitForPassiveTarget (uint8_t * pbtCUID, size_t * szCUIDLen);
pn532_error_t pn532_mifareultralight_ReadPage (uint8_t page, uint8_t * pbtBuffer);
pn532_error_t PN532_mifareultralight_WritePage (uint8_t page, uint8_t *buffer);
pn532_error_t pn532_ntag216_fast_ReadPage (uint8_t * pbtBuffer);
#endif
