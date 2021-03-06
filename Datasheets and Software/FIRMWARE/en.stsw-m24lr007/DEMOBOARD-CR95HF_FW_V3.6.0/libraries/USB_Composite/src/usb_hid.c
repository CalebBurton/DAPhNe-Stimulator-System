/**
  ******************************************************************************
  * @file    usb_hid.c 
  * @author  MMY Application Team
  * @version V2.0
  * @date    12/06/2012
  * @brief   Handle messages received through USB HID
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "usb_hid.h"

#include "lib_ConfigManager.h"

/** @addtogroup USB
 * 	@{
 */
 
 /** 
 * @addtogroup Interfaces
 * @{
 */


/** @addtogroup HID
  * @{
  */
	
static void HID_GetMCUversion							( uint8_t *pResponse ); 
static void HID_GetHardwareVersion				( uint8_t *pResponse );
static void HID_GetInterfaceBus 					( uint8_t *pResponse );
static void HID_SendCustomCommand					( uc8 *pCommand, uint8_t *pResponse);
//static void HID_SendCustomCommand					( uc8 *pCommand, uint8_t *pResponse );

#if defined (CR95HF) || defined (ST95HF)
static int8_t HID_RunMultiTagHunting (uc8 *pCommand, uint8_t *pResponse );
static int8_t HID_MultiTagResPart2 (uc8 *pCommand, uint8_t *pResponse );
static int8_t HID_StartTagTraking 				( uc8 *pCommand, uint8_t *pResponse );
static int8_t HID_Run15693Anticollision 	( uc8 *pCommand, uint8_t *pResponse );
static int8_t HID_RunInventoty16slots 		( uc8 *pCommand, uint8_t *pResponse );
static int8_t HID_GotoTagDetectingState		( uc8 *pCommand, uint8_t *pResponse );
static int8_t HID_IsWakeUp								( uc8 *pCommand, uint8_t *pResponse );
static int8_t HID_CalibrateTagDetection		( uc8 *pCommand, uint8_t *pResponse );
static int8_t HID_IsWakeUpByTimeOut				( uint8_t *IdleResponse);
static int8_t HID_IsWakeUpByTagDetecting	( uint8_t *IdleResponse);
static int8_t HID_GetTagDetectionRefValue	( uint8_t *DacDataRef );
static int8_t HID_GetIRQOutState 					( void );

#endif /* (CR95HF) || (ST95HF) */


int8_t HID_ReadAllMemory 									( uc8 *pCommand, uint8_t *pResponse );


static void HID_Reset95HF (uc8 *pCommand, uint8_t *pResponse );
uint8_t																NbHIDTrame;
int8_t 									HIDTransaction;

#ifdef USE_MSD_DRIVE
static void HID_SetUSBDisconnectPin				( void );
#endif

extern drv95HF_ConfigStruct			drv95HFConfig;
extern uint8_t									u95HFBuffer [RFTRANS_95HF_MAX_BUFFER_SIZE+3];

extern bool 										USB_Control_Allowed;

/**  
 * @brief this function is blank function
 * @param  none
 * @retval None
 */
void HID_Data_In( void )
{
	/* None. */
}

/**  
 * @brief this function is the input point of HID user command. the user command is exeuted and a response is send to the user through USB bus.
 */
void HID_Data_Out( void )
{
uint8_t 	RcvBuffer 	[HID_MAX_BUFFER_SIZE], //0x3F bytes reserved for data + 1 for report ID
					SendBuffer	[RFTRANS_95HF_RESPONSEBUFFER_SIZE]; //0x3F bytes reserved for data + 1 for report ID
	
	if( USB_Control_Allowed == true)
	{
		/* Stop FW and let USB take the control */
		ConfigManager_Stop();
		
		/* in order to cancel the tag tracking	*/
		if (HIDTransaction == false )
		{
			if(drv95HF_GetSerialInterface() == RFTRANS_95HF_INTERFACE_SPI)
			{
				drv95HF_ResetSPI();		
			}
			HIDTransaction = true;
		}

		/* Read received data (64 bytes) ... */
		SetEPRxStatus(ENDP3, EP_RX_VALID);  
		USB_SIL_Read(EP3_OUT, RcvBuffer);

		/* Rcv buffer = command to CR95HF*/
		/* SendBuffer = CR95HF response	*/
		switch(RcvBuffer[HID_OFFSET])
		{
			/* Send CR95HF command to CR95HF*/
			case HID_SPI_SEND_CR95HFCMD : 
				if (RcvBuffer[HID_OFFSET_CMDCODE] != BAUD_RATE) 
					drv95HF_SendReceive(&RcvBuffer[HID_OFFSET_CMDCODE], &SendBuffer[HID_OFFSET_CMDCODE]);
			break;
			/* send customs command	*/
			case HID_SPI_SEND_DATA:				
				HID_SendCustomCommand ( &RcvBuffer[HID_OFFSET_CMDCODE], &SendBuffer[HID_OFFSET_CMDCODE]);					
			break;
		}
		
		/* SendBuffer format	*/
		/*	1st byte 	: ID_SEND_HID_RESPONSE	*/
		/*	2nd byte 	: repply or error code flag 	*/
		/*	3rd byte 	: nb byte	*/
		/*	others bytes: data	*/
	
		SendBuffer[HID_OFFSET] = HID_SEND_HID_RESPONSE;
		SendBuffer[HID_OFFSET_LENGTH] =MIN(HID_MAX_BUFFER_SIZE-3,SendBuffer[HID_OFFSET_LENGTH]);
		
		/* Allows the transmission */
		SetEPTxStatus(ENDP3, EP_TX_VALID);
		USB_SIL_Write(EP3_IN, SendBuffer, HID_MAX_BUFFER_SIZE);
				
	}
	else
	{
		/* Read received data (64 bytes) ... */
		SetEPRxStatus(ENDP3, EP_RX_VALID);  
		USB_SIL_Read(EP3_OUT, RcvBuffer);
		
		/* SendBuffer format	*/
		/*	1st byte 	: ID_SEND_HID_RESPONSE	*/
		/*	2nd byte 	: repply or error code flag 	*/
		/*	3rd byte 	: nb byte	*/
		/*	others bytes: data	*/
	
		SendBuffer[HID_OFFSET] = HID_SEND_HID_RESPONSE;
		SendBuffer[HID_OFFSET_CMDCODE] = HID_ERRORCODE_DEFAULT;
		SendBuffer[HID_OFFSET_LENGTH] = 1;

		/* Allows the transmission */
		SetEPTxStatus(ENDP3, EP_TX_VALID);
		USB_SIL_Write(EP3_IN, SendBuffer, HID_MAX_BUFFER_SIZE);
	}	
}


/**
 *	@brief  this function returns the harware version 
 *  @param  *pResponse : pointer on the Rf transceiver response ( Command | Length | Data)
 */
static void HID_GetHardwareVersion(uint8_t *pResponse)
{

#ifdef USE_MB1054_BOARD 
	int8_t HardwareVersion []= {'M','B','1','0','5','4','x'},
#elif defined USE_MB1145_BOARD
	int8_t HardwareVersion []= {'M','B','1','1','4','5','x'},
	#elif defined USE_MB1132_BOARD
	int8_t HardwareVersion []= {'M','B','1','1','3','2','x'},
#else
	int8_t HardwareVersion []= {'U','n','k','n','o','w','n'},
#endif
	
	NbBytes = 0x07;

	*pResponse = MCUVERSION_STATUS;
	*(pResponse+1) = NbBytes;
	memcpy((pResponse+2),HardwareVersion ,NbBytes);	
}

/**
 *	@brief  this function returns the MCU version 
  *  @param  *pResponse : pointer on the CR95HF response ( Command | Length | Data)
 *  @retval  none
 */
static void HID_GetMCUversion ( uint8_t *pResponse )
{
uint8_t Mcuversion [MCUVERSION_LENGTH] = {
																						MCUVERSION_FIRSTNUMBER,
																						MCUVERSION_SECONDNUMBER,
																						MCUVERSION_THIRDNUMBER
																					};


	*pResponse = MCUVERSION_STATUS;
	*(pResponse+1) = MCUVERSION_LENGTH;
	memcpy((pResponse+2),Mcuversion ,MCUVERSION_LENGTH);	
}

/**
 *	@brief  this function returns the inferface bus choosed 
 *  @param  *pResponse : pointer on the the PCD device response
 *  @param  : 0x00 : status ok
 *  @param  : 0x01 : length
 *  @param  : 0x0X : with X = 0 => UART Or X = 0 => SPI Or X = 2 => TWI
 *  @retval 
 */
static void HID_GetInterfaceBus (uint8_t *pResponse)
{
   	*pResponse 		= 	HID_SUCCESS_CODE_0X80;
		*(pResponse+1) 	= 0x01;
		*(pResponse+2) 	= drv95HF_GetInterfacePinState ();
}


/**
 *	@brief  this function sends a custom command to XX95HF device over SPI or UART bus and receive
 *	@brief  its response.
 *  @param  *pCommand  : pointer on the buffer to send to the XX95HF ( Command | Length | Data)
 *  @param  *pResponse : pointer on the XX95HF response ( Command | Length | Data)
 */
static void HID_SendCustomCommand(uc8 *pCommand, uint8_t *pResponse)
{
	
	*pResponse = HID_SUCCESS_CODE_0X80;
	*(pResponse+1) = 0x00;
		
	switch (pCommand[XX95HF_COMMAND_OFFSET])
	{
		case XX95HF_RESETSEQUENCE: 		
			if(drv95HF_GetSerialInterface() == RFTRANS_95HF_INTERFACE_SPI)
			{
				drv95HF_ResetSPI();		
			}
			break;
		case XX95HF_PULSE_IRQIN: 
			drv95HF_SendIRQINPulse();
			break;
		case XX95HF_PULSE_SPINSS: 
			SendSPINSSPulse();
			break;
		case XX95HF_PULSE_POOLINGREADING: 
			drv95HF_PoolingReading(pResponse);
			break;
		case XX95HF_GETMCUVERSION: 
		 	HID_GetMCUversion(pResponse);
			break;
		
		case XX95HF_GETHARDWAREVERSION: 
		 	HID_GetHardwareVersion(pResponse);
			break;
		
		case XX95HF_POR :
			HID_Reset95HF (pCommand, pResponse );
			break;
			
//#if defined (CR95HF)
		case XX95HF_GETINTERFACEPINSTATE: 
		 	HID_GetInterfaceBus (pResponse);
			break;
//#endif		

#if defined (CR95HF) || defined (ST95HF)
		
		case XX95HF_INVENTORY16SLOTS: 
			HID_RunInventoty16slots (pCommand,pResponse);	
			break;
		case XX95HF_15693ANTICOL: 
			HID_Run15693Anticollision (pCommand,pResponse);		
			break;
		
		case XX95HF_MULTITAG: 
			HID_RunMultiTagHunting(pCommand,pResponse);	
			break;
		case XX95HF_MULTITAG_EXTENDED: 
			HID_MultiTagResPart2(pCommand,pResponse);	
			break;
				
		case XX95HF_TAG_TRAKING:
			HID_StartTagTraking (pCommand, pResponse );
			break;
		
		case XX95HF_GOTOTAGDETECTINGSTATE :
			HID_GotoTagDetectingState (pCommand, pResponse );
			break;
		
		case XX95HF_ISWAKEUP :
			HID_IsWakeUp (pCommand, pResponse );
			break;
		
		case XX95HF_CALIBRATETHETAGDETECTION:
			HID_CalibrateTagDetection (pCommand, pResponse );
			break;
		
#ifdef USE_MSD_DRIVE
			
		case XX95HF_SETUSBDISCONNECTSTATE: 
				HID_SetUSBDisconnectPin ( );		
			break;			
		case XX95HF_COPYTAGTOFILE:
			PCD_CopyTagToFile ( pCommand, pResponse );
			break;
		case XX95HF_COPYFILETOTAG:
			PCD_CopyFileToTag (pCommand, pResponse);
			break;	
			
#endif
		
#endif /* (CR95HF) || (ST95HF) */
		
		default :
				*pResponse = HID_ERRORCODE_CUSTOMCOMMANDUNKNOWN;
		break;

	}
	
}


/**  
 * @brief  this function restet the RF transceiver
 * @brief  C3: Command instruction
 * @brief  00: Length Data
 * @param  *pCommand  : pointer on the buffer to send to the CR95HF ( Command | Length | Data)
 * @param  *pResponse : pointer on the CR95HF response ( Command | Length | Data)
 * @retval HID_SUCCESSCODE.
 */
static void HID_Reset95HF (uc8 *pCommand, uint8_t *pResponse )
{

	*(pResponse+1) = 0x00;

#if defined (CR95HF) || defined (ST95HF)	
	if(PCD_PORsequence() == HID_SUCCESSCODE)
#else		
	if(PICC_PORsequence() == HID_SUCCESSCODE)
#endif		
	{
		*(pResponse) = HID_SUCCESSCODE;
	}
	else
	{
		*(pResponse) = HID_ERRORCODE_PORERROR;
	}

}

#if defined (CR95HF) || defined (ST95HF)

/**  
 * @brief  this function launches the POR sequence?
 * @param  pCommand	: HID commend received from the PC
 * @param  pResponse : HID response to be send to the PC
 * @retval HID_SUCCESSCODE
 */
static int8_t HID_GetIRQOutState ( void )
{
	if ((GPIO_ReadInputData(EXTI_GPIO_PORT) & 0x0008) != 0x00)
		return 0x01;
	else 
		return 0x00;
}

/**  
 * @brief  this function re activate the tag tracking
 * @brief  C4: Command instruction
 * @brief  00: Length Data
 * @param  *pCommand  : pointer on the buffer to send to the CR95HF ( Command | Length | Data)
 * @param  *pResponse : pointer on the CR95HF response ( Command | Length | Data)
 * @retval HID_SUCCESSCODE.


 */
static int8_t HID_StartTagTraking (uc8 *pCommand, uint8_t *pResponse )
{

	HIDTransaction = false;
 	*(pResponse) = HID_SUCCESSCODE;
	*(pResponse+1) = 0x00;

	return HID_SUCCESSCODE;
}



/**  
 * @brief  this function launches the POR sequence?
 * @param  *pCommand  : pointer on the buffer to send to the CR95HF ( Command | Length | Data)
 * @param  *pResponse : pointer on the CR95HF response ( Command | Length | Data)
 * @retval HID_SUCCESSCODE
 */
static int8_t HID_GotoTagDetectingState	( uc8 *pCommand, uint8_t *pResponse )
{
uint8_t IdleData[]	= {0x0A,0x21,0x00,0x79,0x01,0x18,0x00,0x20,0x60,0x60,0x6C,0x74,0x3F,0x01};

 	*(pResponse) = HID_ERRORCODE_TAGDETECTING;
	*(pResponse+1) = 0x00;

	/*  the received command is composed as A3 02 XX YY 	*/
	/* A3 : operating command code	*/
	/* 02 : number of byte		*/
	/* XX : DACdataL	*/
	/* YY : DACdataH	*/

	/* reset the CR95HF */
	if(drv95HF_GetSerialInterface() == RFTRANS_95HF_INTERFACE_SPI)
	{
		drv95HF_ResetSPI();		
	}
	PCD_Echo(u95HFBuffer);
	if (u95HFBuffer[0] != ECHORESPONSE)
			return HID_ERRORCODE_TAGDETECTING;
	
	IdleData [0] = PCD_WAKEUPSOURCE_TAGDETECTING | PCD_WAKEUPSOURCE_IRRQINN;
	/* copies the DACdataL & DACdataH values */
	IdleData [10] = pCommand[PCD_DATA_OFFSET];
	IdleData [11] = pCommand[PCD_DATA_OFFSET+1];
	PCD_Idle(0x0E,IdleData);
	
	/* enable the interuption on the IRQout to catch the CR95HF response	*/
	/* -- An EXTI must be set to catch the IRQ (a edge on the UART RX pin) from the CR95HF	*/
	/* EXTI_Config( );	   */
	/* enable the interupt 	*/
	/*	IRQOUT_Interrupts_Config (); 	*/
	/*	PCD_DataReady == FALSE; */
	
	drv95HFConfig.uState = RFTRANS_95HF_STATE_TAGDETECTOR;
	*(pResponse) = HID_SUCCESSCODE;
	
	return HID_SUCCESSCODE;	
	
}



/**  
 * @brief  this function sends to the CR95HF a Idle command.
 * @brief  The wake up sources are IRQ_in and tag detecting. 
 * @param  *pCommand  : pointer on the buffer to send to the CR95HF ( Command | Length | Data)
 * @param  *pResponse : pointer on the CR95HF response ( Command | Length | Data)
 * @retval HID_SUCCESSCODE : the function is succesful
 * @retval PCD_ERRORCODE_TAGDETECTINGCALIBRATION : the function is not succesful
 */
static int8_t HID_CalibrateTagDetection	(uc8 *pCommand, uint8_t *pResponse )
{
uint8_t DacDataRef;
	
	/* initialize the response*/
	*(pResponse) = HID_ERRORCODE_TAGDETECTINGCALIBRATION;
	*(pResponse+1) = 00;	
	
		if ( HID_GetTagDetectionRefValue(&DacDataRef) == HID_SUCCESSCODE)
		{	
			/* creates the response*/
			*(pResponse) = SUCCESFUL_COMMAND_0x80;
			*(pResponse+1) = 0x02;
			*(pResponse+2) = DacDataRef - 0x08;
			*(pResponse+3) = DacDataRef + 0x08;
		}

		return HID_SUCCESSCODE;	
	
}

/**  
 * @brief  this function sends to the CR95HF a Idle command.
 * @brief  The wake up sources are IRQ_in and tag detecting. 
 * @param  *pCommand  : pointer on the buffer to send to the CR95HF ( Command | Length | Data)
 * @param  *pResponse : pointer on the CR95HF response ( Command | Length | Data)
 * @retval HID_SUCCESSCODE : the function is succesful
 * @retval PCD_ERRORCODE_TAGDETECTINGCALIBRATION : the function is not succesful
 */
static int8_t HID_GetTagDetectionRefValue	( uint8_t *DacDataRef )
{

uint8_t i=0,
				IdleData[]={0x03,0xA1,0x00,0xF8,0x01,0x18,0x00,0x20,0x60,0x60,0x00,0x00,0x3F,0x01};
								
					
	(*DacDataRef )	= 0xFC;

	/* reset the CR95HF */
	if(drv95HF_GetSerialInterface() == RFTRANS_95HF_INTERFACE_SPI)
	{
		drv95HF_ResetSPI();		
	}
	PCD_Echo(u95HFBuffer);
	if (u95HFBuffer[0] != ECHORESPONSE)
			return HID_ERRORCODE_TAGDETECTINGCALIBRATION;
	
	/* the received command is composed as A4 00 00 */
	/* A4 : operating command code	*/
	/* 00 : number of byte	*/
		
	/* 1st step : VERIFY detection with DacDataH=0x00 (min value)	*/
	/*  070E03A100F801180020606000003F01	*/
	PCD_Idle(0x0E,IdleData);
	drv95HF_PoolingReading (u95HFBuffer);
	if (HID_IsWakeUpByTagDetecting(u95HFBuffer) != true)
			return HID_ERRORCODE_TAGDETECTINGCALIBRATION; 
	
	/* 2nd step : VERIFY no detection with DacDataH=0xFC (max value)	*/
	/* 070E03A100F801180020606000FC3F01	*/
	IdleData [11]= (*DacDataRef);  
	PCD_Idle(0x0E,IdleData);
	drv95HF_PoolingReading (u95HFBuffer);
	if (HID_IsWakeUpByTimeOut(u95HFBuffer) != true)
			return HID_ERRORCODE_TAGDETECTINGCALIBRATION; 
	
	/* next steps : Calibration with DacDataH=0x1F	*/
	for (i=0;i<=5;i++)
	{
		
			if (HID_IsWakeUpByTimeOut(u95HFBuffer) == true)
					(*DacDataRef )-= (0x080 >> i);
			else if (HID_IsWakeUpByTagDetecting(u95HFBuffer) == true)
				(*DacDataRef ) += (0x080 >> i) ;
			else return HID_ERRORCODE_TAGDETECTINGCALIBRATION;
			
			IdleData [11]=(*DacDataRef); 
			PCD_Idle(0x0E,IdleData);
			drv95HF_PoolingReading (u95HFBuffer);
	}
	
	if (HID_IsWakeUpByTimeOut(u95HFBuffer) == true)
		*DacDataRef -= 4; 
	else if (HID_IsWakeUpByTagDetecting(u95HFBuffer) == true)
		(*DacDataRef) = (*DacDataRef); 
	
	return HID_SUCCESSCODE;	
}



/**  
 * @brief  this function returns TRUE if the wake up source is the internal timeout
 * @param  IdleResponse	: reponse of the CR95HF to the Idle command
 * @retval TRUE : the timeout of the CR95Hf is the source of the wake up
 * @retval FALSE : the timeout of the CR95Hf is not the source of the wake up
 */
static int8_t HID_IsWakeUpByTimeOut	(uint8_t *IdleResponse)
{
		if (IdleResponse[0] == HID_SUCCESSCODE &&
				IdleResponse[1] == PCD_IDLERES_LENGTH &&
				IdleResponse[2] == PCD_WAKEUPSOURCE_TIMEOUT )
		
				return true ;
		else
			return false ;
}


/**  
 * @brief  this function returns TRUE if the wake up source is the tag detecting block
 * @param  IdleResponse	: reponse of the CR95HF to the Idle command
 * @retval TRUE : the tag detecting of the CR95Hf is the source of the wake up
 * @retval FALSE : the tag detecting of the CR95Hf is not the source of the wake up
 */
static int8_t HID_IsWakeUpByTagDetecting	(uint8_t *IdleResponse)
{
		if (IdleResponse[0] == HID_SUCCESSCODE &&
				IdleResponse[1] == PCD_IDLERES_LENGTH &&
				IdleResponse[2] == PCD_WAKEUPSOURCE_TAGDETECTING )
		
				return true ;
		else
			return false ;
}


/**  
 * @brief  this function launches the POR sequence?
 * @param  pCommand	: HID commend received from the PC
 * @param  pResponse : HID response to be send to the PC
 * @retval HID_SUCCESSCODE
 */
static int8_t HID_IsWakeUp	(uc8 *pCommand, uint8_t *pResponse )
{
 	*(pResponse) = SUCCESFUL_COMMAND_0x80;
	*(pResponse+1) = 0x01;
	/* the received command is composed as A2 00 	*/
	/* A2 : operating command code	*/
	/* 00 : number of byte	*/
	
	if ( HID_GetIRQOutState () != 0x00)
		*(pResponse+2) = 0x01;
	else 
		*(pResponse+2) = 0x00;
				
	return HID_SUCCESSCODE;	
}

/**  
 * @brief  this function runs the 15693 anticollision process.
 * @param  *pCommand  : pointer on the buffer to send to the CR95HF ( Command | Length | Data)
 * @param  *pResponse : pointer on the CR95HF response ( Command | Length | Data)
 * @retval HID_SUCCESS_CODE : the function is successful
 * @retval HID_ERRORCODE_DEFAULT : anticollision doesn't succeed
 */

static int8_t HID_Run15693Anticollision (uc8 *pCommand, uint8_t *pResponse )
{
uint8_t 	Flags ,
			AFI = 0x00,
			NbTag = 0x00,
			pUIDout [20*9]; // to save up to 20 UID
int8_t		status;
	
	// initialize the USB response
	*pResponse = HID_ERRORCODE_DEFAULT;
	*(pResponse+1) = 0x00;


	/* the received command is composed as A0 XX YY with 	*/
	/* A0 : operating command code	*/
	/* XX : number of byte */
	/* YY : request flags	*/

	/* the response is composed as SC NB NT XX... with 	*/
	/* SC : status code (0x00 for a success exucution)	*/
	/* NB : number of byte*/
	/* NT : number of tags seen	*/
	/* XX : UID of the tags seen	*/
	
	/* get the request flags	*/
	Flags= pCommand[PCD_DATA_OFFSET]; 
	
	errchk(ISO15693_RunAntiCollision (Flags , AFI,&NbTag,pUIDout));

	/* create the response */
	*pResponse = SUCCESFUL_COMMAND_0x80;
	*(pResponse+1) = 1+(ISO15693_NBBYTE_UID+1)*NbTag;
	*(pResponse+2) = NbTag;
	if (NbTag > 0 && NbTag < 20) 
	{
		memcpy((pResponse+3),pUIDout, (ISO15693_NBBYTE_UID+1)*NbTag); 
	}
	return HID_SUCCESSCODE;
Error:
	return HID_ERRORCODE_DEFAULT;
}



/**  
 * @brief  this function runs the 15693 anticollision process.
 *  @param  *pCommand  : pointer on the buffer to send to the PCD ( Command | Length | Data)
 *  @param  *pResponse : pointer on the PCD response ( Command | Length | Data)
 * @retval HID_SUCCESS_CODE : the function is successful
 * @retval HID_ERRORCODE_DEFAULT : anticollision doesn't succeed
 */

static int8_t HID_RunInventoty16slots (uc8 *pCommand, uint8_t *pResponse )
{
uint8_t 	Flags ,
					AFI = 0x00,
					NbTag = 0x00,
					pUIDout [20*9]; // to save up to 20 UID
int8_t		status;
	
	/* initialize the USB response	*/
	*pResponse = HID_ERRORCODE_DEFAULT;
	*(pResponse+1) = 0x00;


	/* the received command is composed as A0 XX YY with */
	/* A1 : operating command code	*/
	/* XX : number of byte	*/
	/* YY : request flags	*/

	/* the response is composed as SC NB NT XX... with 	*/
	/* SC : status code (0x00 for a success exucution)	*/
	/* NB : number of byte	*/
	/* NT : number of tags seen */
	/* XX : UID of the tags seen */
	
	/* get the request flags	*/
	Flags= pCommand[PCD_DATA_OFFSET]; 
	
	errchk(ISO15693_RunInventory16slots (Flags , AFI,&NbTag,pUIDout));

	/* create the response */
	*pResponse = SUCCESFUL_COMMAND_0x80;
	*(pResponse+1) = 1+8*NbTag;
	*(pResponse+2) = NbTag;
	if (NbTag > 0 && NbTag < 16) 
		memcpy((pResponse+3),pUIDout, 8*NbTag); 


   	return HID_SUCCESSCODE;
Error:
	return HID_ERRORCODE_DEFAULT;
}


/**  
 * @brief  this function runs the 15693 anticollision process.
 *  @param  *pCommand  : pointer on the buffer to send to the PCD ( Command | Length | Data)
 *  @param  *pResponse : pointer on the PCD response ( Command | Length | Data)
 * @retval HID_SUCCESS_CODE : the function is successful
 * @retval HID_ERRORCODE_DEFAULT : anticollision doesn't succeed
 */

static int8_t HID_RunMultiTagHunting (uc8 *pCommand, uint8_t *pResponse )
{
uint8_t 	NbTag = 0x00,
					pUIDout [20*11]; // to save up to 20 UID
	
	/* initialize the USB response	*/
	*pResponse = HID_ERRORCODE_DEFAULT;
	*(pResponse+1) = 0x00;
	
	ISO14443A_MultiTagHunting(&NbTag,pUIDout);

	/* create the response */
	*pResponse = SUCCESFUL_COMMAND_0x80;
	
	/* we will return only the 5 first UID due to USB 64bytes limitation */
	if( NbTag>5)
	{
		*(pResponse+1) = 1+(10+1)*5; /* only data of 1st 5 UID will be transmit */
		*(pResponse+2) = NbTag;  /* but number of tag found transmit */
		memcpy((pResponse+3),pUIDout, 11*5);
	}
	else
	{
		*(pResponse+1) = 1+(10+1)*NbTag;
		*(pResponse+2) = NbTag;
		memcpy((pResponse+3),pUIDout, 11*NbTag);
	}
	
  return HID_SUCCESSCODE;
}

static int8_t HID_MultiTagResPart2 (uc8 *pCommand, uint8_t *pResponse )
{
	uint8_t 	NbTag = 0x00,
					pUIDout [10*11]; // to save up to 10 UID
	
	/* initialize the USB response	*/
	*pResponse = HID_ERRORCODE_DEFAULT;
	*(pResponse+1) = 0x00;
	
	ISO14443A_MultiTagPart2(&NbTag, pUIDout);
	
	/* create the response */
	*pResponse = SUCCESFUL_COMMAND_0x80;
	*(pResponse+1) = 1+(10+1)*NbTag;
	*(pResponse+2) = NbTag;
	memcpy((pResponse+3),pUIDout, 11*NbTag);

 	return HID_SUCCESSCODE;

}

#endif /* CR95HF || ST95HF */

// /**  
//  * @brief  this function sends 15 blocks of TAG memory by USB HID 
//  * @brief  the Tag memery need to be store into STM32 memory before use 
//  * @brief  this function. 
//  * @param  *pCommand  : pointer on the buffer to send to the CR95HF ( Command | Length | Data)
//  * @param  *pResponse : pointer on the CR95HF response ( Command | Length | Data)
//  * @retval none
//  */
// static void HID_ReadMCUBuffer (uc8 *pCommand, uint8_t *pResponse )
// {

// uint8_t	  NumTram = MIN((NbHIDTrame-1),pCommand[PCD_DATA_OFFSET]);
// 	
// 			/*Copy Data of Select Bloc for USB Transfert*/
// 	 		*(pResponse) = SUCCESFUL_COMMAND_0x80;
// 			*(pResponse+1) = 0x3D;
// 			*(pResponse+2) = NumTram;

// 			 memcpy(&pResponse[3],&TagMemoryBuffer[NumTram*0x3C],0x3C);

// }

// /**  
//  * @brief  this function reads the TAG memory and save it on STM32 memory
//  * @brief  with CRC control.
//  * @param  *pCommand  : pointer on the buffer to send to the PCD ( Command | Length | Data)
//  * @param  *pResponse : pointer on the PCD response ( Command | Length | Data)
//  * @retval ISO15693_SUCCESSCODE
//  */
// int8_t HID_ReadAllMemory (uc8 *pCommand, uint8_t *pResponse )
// {
// uint8_t IC_Ref_Tag,
// 				RequestFlags = 0x02,
// 				Tag_Density;
// int8_t 	status; 
// uint16_t NbBlockToRead = 0, 
// 				AddStart =0,
// 				NbMemoryBlock,
// 				TotalCRCError = 0,
// 				NbCRCError = 0,
// 				OffsetBuffer = 0,
// 				NthDataToRead = 0;


// 	/* the received command is composed as B0 03 XX XX YY with 	*/
// 	/* B0 : operating command code	*/
// 	/* XX XX : first block address	*/
// 	/* YY : number of block	*/

// 	*pResponse =ISO15693_ERRORCODE_DEFAULT;
// 	*(pResponse+1) = 0x00;

// 	/*Send Protocol Select ISO/IEC 15693 protocol and identify the ISO/IEC 15693 tag*/
// 	errchk(ISO15693_GetTagIdentification(&NbMemoryBlock,&Tag_Density, &IC_Ref_Tag));	

// 	/*Read the number of adress byte*/
// 	/*Low density memory : the number of byte of the memory address is one */
// 	if (pCommand [PCD_DATA_OFFSET+2] == 0x00)
// 			NbBlockToRead = NbMemoryBlock;
// 	else 
// 	{
// 			AddStart = ((pCommand[PCD_DATA_OFFSET]<<8) & 0xFF00) | pCommand[PCD_DATA_OFFSET+1] ;
// 			NbBlockToRead = pCommand [4]+1;
// 	}
// 	
// 	// for the high density tag, the read command shall be updated
// 	if (Tag_Density == ISO15693_HIGH_DENSITY)
// 	{
// 			RequestFlags = 0x0A;
// 	}
// 		
// 	for ( NthDataToRead=AddStart; NthDataToRead<(NbBlockToRead+AddStart); NthDataToRead++)
// 	{						

// 		/*Read the data from the conctacless tag*/ 
// 		do
// 		{
// 			//drv95HF_SendReceive(ReadBuffer, RepBuffer);
// 			status = ISO15693_ReadSingleBlock 	(RequestFlags, 0x00, NthDataToRead,u95HFBuffer );
// 			NbCRCError++;
// 		}while (status != ISO15693_SUCCESSCODE && NbCRCError < 3);

// 		/*CRC Error Compter*/
// 		if((u95HFBuffer[9] & 0x03) != 0x00)
// 			TotalCRCError++;

// 		memcpy(&(TagMemoryBuffer[(OffsetBuffer)*4]),&(u95HFBuffer[3]),4);

// 		NbCRCError=0;
// 		OffsetBuffer++;

// 	}

// 	/*Memory Read Parameters*/	
// 	NbHIDTrame = (NbBlockToRead/0x0F)+1;

// 	/*Error code if more than 25% error during reading*/
// 	if(TotalCRCError > (NbBlockToRead/4))
// 	{
// 		*(pResponse) = HID_ERRORCODE_READALLMEMORY;
// 		return HID_SUCCESSCODE;
// 	}
// 	else 
// 		*(pResponse) = SUCCESFUL_COMMAND_0x80;
// 	
// 		*(pResponse+1) = 0x02;
// 		*(pResponse+2) = NbHIDTrame;
// 		*(pResponse+3) = TotalCRCError;
// 		

// 	return ISO15693_SUCCESSCODE;
// Error : 
// 	*(pResponse) = HID_ERRORCODE_READALLMEMORY;
// 	return HID_SUCCESSCODE;		
// }



#ifdef USE_MSD_DRIVE

/**
 *	@brief  this function returns the inferface bus choosed 
 *  @param  *pCommand : command received 
 */
static void HID_SetUSBDisconnectPin ( void )
{
	
	USB_MSD_Reboot();

}

#endif 
/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */



/******************* (C) COPYRIGHT 2014 STMicroelectronics *****END OF FILE****/


