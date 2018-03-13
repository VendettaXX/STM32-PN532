/**************************************************************************/
/*! 
    @file     pn532.h
*/
/**************************************************************************/

#ifndef __PN532_H__
#define __PN532_H__

#include "bsp_config.h"

/* Error messages generated by the stack */
/* Not to be confused with app level errors from the PN532 */
/* These are the errors that are returned by the PN532 driver */
typedef enum pn532_error_e
{
  PN532_ERROR_NONE                    = 0x00,
  PN532_ERROR_UNABLETOINIT            = 0x01,   // Unable to initialise or wakeup the device
  PN532_ERROR_APPLEVELERROR           = 0x02,   // Application level error detected
  PN532_ERROR_BUSY                    = 0x03,   // Busy executing a previous command
  PN532_ERROR_NOACK                   = 0x04,   // No ack message received
  PN532_ERROR_INVALIDACK              = 0x05,   // Ack != 00 00 FF 00 FF 00
  PN532_ERROR_PREAMBLEMISMATCH        = 0x06,   // Frame preamble + start code mismatch
  PN532_ERROR_EXTENDEDFRAME           = 0x07,   // Extended frames currently unsupported
  PN532_ERROR_LENCHECKSUMMISMATCH     = 0x08,
  PN532_ERROR_RESPONSEBUFFEREMPTY     = 0x09,   // No response data received
  PN532_ERROR_READYSTATUSTIMEOUT      = 0x0A,   // Timeout waiting for 'ready' status (SPI/I2C only)
  PN532_ERROR_TIMEOUTWAITINGFORCARD   = 0x0B,   // No card detected in field with the specified timeout
  PN532_ERROR_BLOCKREADFAILED         = 0x0C,   // Unexpected response to block read request
  PN532_ERROR_WRONGCARDTYPE           = 0x0D,   // Card is not the expected format (based on SENS_RES/ATQA value)
  PN532_ERROR_ADDRESSOUTOFRANGE       = 0x0E,   // Specified block and page is out of range
  PN532_ERROR_I2C_NACK                = 0x0F    // I2C Bus - No ACK was received for master to slave data transfer
} pn532_error_t;

typedef enum pn532_modulation_e
{
  PN532_MODULATION_ISO14443A_106KBPS  = 0x00,
  PN532_MODULATION_FELICA_212KBPS     = 0x01,
  PN532_MODULATION_FELICA_424KBPS     = 0x02,
  PN532_MODULATION_ISO14443B_106KBPS  = 0x03,
  PN532_MODULATION_JEWEL_106KBPS      = 0x04
} pn532_modulation_t;

/* HW Commands for the PN532.  */
/* See UM0701-02 - PN532 User Manual */
enum
{
  PN532_COMMAND_DIAGNOSE              = 0x00,
  PN532_COMMAND_GETFIRMWAREVERSION    = 0x02,
  PN532_COMMAND_GETGENERALSTATUS      = 0x04,
  PN532_COMMAND_READREGISTER          = 0x06,
  PN532_COMMAND_WRITEREGISTER         = 0x08,
  PN532_COMMAND_READGPIO              = 0x0C,
  PN532_COMMAND_WRITEGPIO             = 0x0E,
  PN532_COMMAND_SETSERIALBAUDRATE     = 0x10,
  PN532_COMMAND_SETPARAMETERS         = 0x12,
  PN532_COMMAND_SAMCONFIGURATION      = 0x14,
  PN532_COMMAND_POWERDOWN             = 0x16,
  PN532_COMMAND_RFCONFIGURATION       = 0x32,
  PN532_COMMAND_RFREGULATIONTEST      = 0x58,
  PN532_COMMAND_INJUMPFORDEP          = 0x56,
  PN532_COMMAND_INJUMPFORPSL          = 0x46,
  PN532_COMMAND_INLISTPASSIVETARGET   = 0x4A,
  PN532_COMMAND_INATR                 = 0x50,
  PN532_COMMAND_INPSL                 = 0x4E,
  PN532_COMMAND_INDATAEXCHANGE        = 0x40,
  PN532_COMMAND_INCOMMUNICATETHRU     = 0x42,
  PN532_COMMAND_INDESELECT            = 0x44,
  PN532_COMMAND_INRELEASE             = 0x52,
  PN532_COMMAND_INSELECT              = 0x54,
  PN532_COMMAND_INAUTOPOLL            = 0x60,
  PN532_COMMAND_TGINITASTARGET        = 0x8C,
  PN532_COMMAND_TGSETGENERALBYTES     = 0x92,
  PN532_COMMAND_TGGETDATA             = 0x86,
  PN532_COMMAND_TGSETDATA             = 0x8E,
  PN532_COMMAND_TGSETMETADATA         = 0x94,
  PN532_COMMAND_TGGETINITIATORCOMMAND = 0x88,
  PN532_COMMAND_TGRESPONSETOINITIATOR = 0x90,
  PN532_COMMAND_TGGETTARGETSTATUS     = 0x8A
};

/* Application level errors generated by the PN532 chip */
/* See UM0701-02 - PN532 User Manual */
enum 
{
  PN532_APPERROR_NONE                 = 0x00,
  PN532_APPERROR_TIMEOUT              = 0x01,
  PN532_APPERROR_CRCERROR             = 0x02,
  PN532_APPERROR_PARITYERROR          = 0x04,
  PN532_APPERROR_FRAMINGERROR         = 0x05,
  PN532_APPERROR_BITCOLLISION         = 0x06,
  PN532_APPERROR_INSUFFICIENTBUFFER   = 0x07,
  PN532_APPERROR_RFBUFFEROVERFLOW     = 0x09,
  PN532_APPERROR_RFFIELDTIMEOUT       = 0x0A,
  PN532_APPERROR_RFPROTOCOLERROR      = 0x0B,
  PN532_APPERROR_TEMPERROR            = 0x0D,
  PN532_APPERROR_INTERNBUFFEROVERFLOW = 0x0E,
  PN532_APPERROR_INVALIDPARAMETER     = 0x10,
  PN532_APPERROR_DEP_UNSUPPORTEDCMD   = 0x12,
  PN532_APPERROR_DEP_INVALIDOFORMAT   = 0x13,
  PN532_APPERROR_AUTHENTERR           = 0x14,
  PN532_APPERROR_UIDCCHECKERROR       = 0x23,
  PN532_APPERROR_DEP_INVALIDDEVSTATE  = 0x25,
  PN532_APPERROR_OPERATIONNOTALLOWED  = 0x26,
  PN532_APPERROR_CMDNOTACCEPTABLE     = 0x27,
  PN532_APPERROR_TARGETRELEASED       = 0x29,
  PN532_APPERROR_IDMISMATCH           = 0x2A,
  PN532_APPERROR_CARDDISAPPEARED      = 0x2B,
  PN532_APPERROR_NFCID3MISMATCH       = 0x2C,
  PN532_APPERROR_OVERCURRENTEVENT     = 0x2D,
  PN532_APPERROR_NADMISSINGINDEP      = 0x2E
};

/* Possible states for the PN532 SW Stack */
typedef enum pn532_state_e
{
  PN532_STATE_SLEEP,
  PN532_STATE_READY,
  PN532_STATE_BUSY
}
pn532_state_t;

/* PN532 Protocol control block */
typedef struct
{
  bool                initialised;
  pn532_state_t       state;
  pn532_modulation_t  modulation;
  uint32_t            lastCommand;
  uint32_t            appError;
} pn532_pcb_t;

void          pn532PrintHex(const uint8_t * pbtData, const size_t szBytes);
void          pn532PrintHexChar(const uint8_t * pbtData, const size_t szBytes);
pn532_pcb_t * pn532GetPCB(void);
void          pn532Init(void);
pn532_error_t pn532Read(uint8_t *pbtResponse, size_t * pszLen);
pn532_error_t pn532Write(uint8_t *abtCommand, size_t szLen);

#endif
