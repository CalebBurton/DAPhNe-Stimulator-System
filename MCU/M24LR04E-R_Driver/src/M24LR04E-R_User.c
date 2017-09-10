/*
  ******************************************************************************
  * @file    M24LR04E-R_User.c
  * @author  Alexey Revinski
  * @date    09/10/2017
  * @brief   This file contains functions used to interface with the
  *          M24LR04E-R_I2C.c driver
  ******************************************************************************
  * @copy
  *
  * DAPHNE STIMULATOR SYSTEM FIRMWARE IS COPYRIGHTED AND IS OWNED BY
  * NORTHWESTERN UNIVERSITY, AN ILLINOIS NOT-FOR-PROFIT CORPORATION, HAVING A
  * PLACE OF BUSINESS AT 633 CLARK STREET, EVANSTON, ILLINOIS  60208. IT CAN BE 
  * FREELY USED FOR EDUCATIONAL AND RESEARCH PURPOSES BY NON-PROFIT INSTITUTIONS
  * AND US GOVERNMENT AGENCIES ONLY. OTHER ORGANIZATIONS ARE ALLOWED TO USE 
  * DAPHNE STIMULATOR SYSTEM FIRMWARE ONLY FOR EVALUATION PURPOSES, AND ANY 
  * FURTHER USES WILL REQUIRE PRIOR APPROVAL. THE SOFTWARE MAY NOT BE SOLD OR 
  * REDISTRIBUTED WITHOUT PRIOR APPROVAL. ONE MAY MAKE COPIES OF THE SOFTWARE 
  * FOR THEIR USE PROVIDED THAT THE COPIES, ARE NOT SOLD OR DISTRIBUTED, ARE 
  * USED UNDER THE SAME TERMS AND CONDITIONS.
  * 
  * AS UNESTABLISHED RESEARCH SOFTWARE, THIS CODE IS PROVIDED ON AN "AS IS" 
  * BASIS WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED. THE
  * DOWNLOADING, OR EXECUTING ANY PART OF THIS SOFTWARE CONSTITUTES AN IMPLICIT
  * AGREEMENT TO THESE TERMS. THESE TERMS AND CONDITIONS ARE SUBJECT TO CHANGE 
  * AT ANY TIME WITHOUT PRIOR NOTICE.
  *
  ******************************************************************************
  *                 COPYRIGHT 2017 NORTHWESTERN UNIVERSITY
  *****************************************************************************/

// INCLUDES  ===================================================================

#include "stm8l15x.h"
#include "stm8l15x_i2c.h"
#include "M24LR04E-R_I2C.h"
#include "M24LR04E-R_User.h"

// EXTERNAL VARIABLES  =========================================================

extern uint8_t NDEFmessage[0x40];                      // NDEF message for NFC

/*==============================================================================
  FUNCTION:    User_ReadNDEFMessage
--------------------------------------------------------------------------------
  - Reads an NDEF message from the NFC memory
==============================================================================*/
ErrorStatus User_ReadNDEFMessage ( uint8_t *PayloadLength )			
{
  uint8_t NthAttempt=0,NbAttempt = 2;
  *PayloadLength = 0;
  for (NthAttempt = 0; NthAttempt < NbAttempt ; NthAttempt++)
  {
    M24LR04E_Init();
    // check if a NDEF message is available in the M24LR04 EEPROM
    if (User_CheckNDEFMessage() == SUCCESS)
    {
      User_GetPayloadLength(PayloadLength);
      if (PayloadLength !=0x00)
      {
        (*PayloadLength) -=2;
        InitializeBuffer (NDEFmessage,(*PayloadLength)+10);
        User_GetNDEFMessage(*PayloadLength,NDEFmessage);
        I2C_Cmd(M24LR04E_I2C, DISABLE);	
        CLK_PeripheralClockConfig(CLK_Peripheral_I2C1, DISABLE);
        GPIO_SetBits(M24LR04E_I2C_SCL_GPIO_PORT,M24LR04E_I2C_SCL_PIN);
        GPIO_SetBits(M24LR04E_I2C_SCL_GPIO_PORT,M24LR04E_I2C_SDA_PIN);
	return SUCCESS;
      }
    }
    M24LR04E_DeInit();
    I2C_Cmd(M24LR04E_I2C, DISABLE);
  }
  return ERROR;
}

/*==============================================================================
  FUNCTION:    User_CheckNDEFMessage
--------------------------------------------------------------------------------
  - Checks the NFC memory for new NDEF message
==============================================================================*/
ErrorStatus User_CheckNDEFMessage(void)
{
  uint8_t data = 0x00;
  uint8_t *OneByte;
  OneByte = &data;
  uint16_t ReadAddr = 0x0000;
  // check the E1 at address 0
  M24LR04E_ReadOneByte (M24LR16_EEPROM_ADDRESS_USER, ReadAddr, OneByte);
  if (*OneByte != 0xE1)
    return ERROR;
  ReadAddr = 0x0009;
  M24LR04E_ReadOneByte (M24LR16_EEPROM_ADDRESS_USER, ReadAddr, OneByte);
  // check the 54 at address 9
  if (*OneByte != 0x54 /*&& *OneByte != 0x55*/)
    return ERROR;
  return SUCCESS;	
}

/*==============================================================================
  FUNCTION:    User_GetPayloadLength
--------------------------------------------------------------------------------
  - Checks NFC memory for length of NDEF message
==============================================================================*/
ErrorStatus User_GetPayloadLength(uint8_t *PayloadLength)
{
  uint16_t ReadAddr = 0x0008;
  *PayloadLength = 0x00;
  M24LR04E_ReadOneByte (M24LR16_EEPROM_ADDRESS_USER, ReadAddr, PayloadLength);
  if (*PayloadLength == 0x00)
    return ERROR;
  return SUCCESS;	
}

/*==============================================================================
  FUNCTION:    User_GetNDEFMessage
--------------------------------------------------------------------------------
  - Fills a buffer with the NDEF message on NFC memory
==============================================================================*/
ErrorStatus User_GetNDEFMessage(uint8_t  PayloadLength,uint8_t *NDEFmessage)
{
  uint16_t ReadAddr = 0x000D;
  if (PayloadLength == 0x00)
    return SUCCESS;
  M24LR04E_ReadBuffer(M24LR16_EEPROM_ADDRESS_USER, ReadAddr,PayloadLength, NDEFmessage);
  return SUCCESS;	
}

/*==============================================================================
  FUNCTION:    InitalizeBuffer
--------------------------------------------------------------------------------
  - Sets all values in a buffer to 0
==============================================================================*/
void InitializeBuffer (uint8_t *Buffer ,uint8_t NbCar)
{
  do{
    Buffer[NbCar]= 0;
  } while (NbCar--);
}
