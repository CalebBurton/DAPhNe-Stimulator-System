#include "stm8l15x.h"
#include "stm8l15x_i2c.h"
#include "M24LR04E-R_I2C.h"
#include "M24LR04E-R_User.h"

extern uint8_t NDEFmessage[0x40];                      // NDEF message for NFC

/*******************************************************************************
*  PRIVATE FUNCTION:    User_ReadNDEFMessage()
*******************************************************************************/
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

/*******************************************************************************
*  PRIVATE FUNCTION:    User_CheckNDEFMessage()
*******************************************************************************/
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

/*******************************************************************************
*  PRIVATE FUNCTION:    User_GetPayloadLength()
*******************************************************************************/
ErrorStatus User_GetPayloadLength(uint8_t *PayloadLength)
{
  uint16_t ReadAddr = 0x0008;
  *PayloadLength = 0x00;
  M24LR04E_ReadOneByte (M24LR16_EEPROM_ADDRESS_USER, ReadAddr, PayloadLength);
  if (*PayloadLength == 0x00)
    return ERROR;
  return SUCCESS;	
}

/*******************************************************************************
*  PRIVATE FUNCTION:    User_GetNDEFMessage()
*******************************************************************************/
ErrorStatus User_GetNDEFMessage(uint8_t  PayloadLength,uint8_t *NDEFmessage)
{
  uint16_t ReadAddr = 0x000D;
  if (PayloadLength == 0x00)
    return SUCCESS;
  M24LR04E_ReadBuffer (M24LR16_EEPROM_ADDRESS_USER, ReadAddr,PayloadLength, NDEFmessage);
  return SUCCESS;	
}

/*******************************************************************************
*  PRIVATE FUNCTION:    InitializeBuffer()
*******************************************************************************/
void InitializeBuffer (uint8_t *Buffer ,uint8_t NbCar)
{
  do{
    Buffer[NbCar]= 0;
  } while (NbCar--);
}

