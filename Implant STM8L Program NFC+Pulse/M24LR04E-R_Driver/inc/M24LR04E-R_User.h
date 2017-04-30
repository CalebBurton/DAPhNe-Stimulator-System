#ifndef M24LR04E_R_USER_H
#define M24LR04E_R_USER_H

int8_t           User_ReadNDEFMessage ( uint8_t *PayloadLength );
ErrorStatus      User_CheckNDEFMessage(void);
ErrorStatus      User_GetPayloadLength(uint8_t *PayloadLength);
ErrorStatus      User_GetNDEFMessage(uint8_t  PayloadLength,uint8_t *NDEFmessage);
void             InitializeBuffer (uint8_t *Buffer ,uint8_t NbCar);

#endif