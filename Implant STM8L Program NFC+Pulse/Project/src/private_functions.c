/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%                                                                        %%%%
%%%%          BME 390: Phrenic Nerve Stimulation: Implant Program           %%%%
%%%%                          private_functions.c                           %%%%
%%%%                                                                        %%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%	
********************************************************************************
*	Author:		Alexey Revinski
*	Last Revised:	02/27/2017
*******************************************************************************/
#include "private_functions.h"
#include "daphne_hardware_config.h"

/*******************************************************************************
*  GLOBAL VARIABLES
*******************************************************************************/
extern bool     sleeping;                       // State variable
uint16_t        time_in         = RESET;        // Inspiration time (for RTC)
uint16_t        time_ex         = RESET;        // Expiration time (for RTC)
uint16_t        CCR1_Val        = RESET;        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
uint16_t        TIM1_period     = RESET;        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
uint16_t        DAC_Val         = RESET;        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
uint16_t        RTC_Div         = 2;            // Maybe delete? // !!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
uint16_t        dummy_time      = 10;           // Arbitraty start-up RTC value// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
uint16_t        mod_10k         = 10000;        // 10k multiplier// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
uint16_t        mod_100         = 100;          // 100 multiplier// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
uint16_t        res_8bit        = 256;          // 8 bit max decimal// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
uint16_t        res_12bit       = 4096;         // 12 bit max decimal// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
uint16_t        Vref_12bit      = 4095;         // Used in RAC calculation // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
uint16_t        minpersec       = 6000;         // seconds per minute x 100

extern uint16_t TI1Buffer[];                    // Pulse timing buffer  (TIM1)
extern uint16_t TI2Buffer[];                    // Polarity buffer      (TIM2)
extern uint16_t DACBuffer[];                    // Amplitude buffer     (DAC1)
extern uint16_t mult;                           // Neg:Pos ratio (amp & time)
extern uint16_t DAC_High;                       // High DAC value (stimulation)
extern uint16_t DAC_Low;                        // Low DAC value (recharging)

/*******************************************************************************
*  EXTERNAL VARIABLES
*******************************************************************************/
extern uint32_t pulse_freq;                     // Pulse frequency
extern uint32_t pulse_width;                    // Pulse width
extern uint32_t pulse_amp;                      // Pulse amplitude
extern uint32_t bpm;                            // Breaths per minute
extern uint32_t ie_ratio;                       // Inspiration:Expiration ratio

// NFC-related variables
uint8_t NDEFmessage[0x40];                      // NDEF message for NFC
//uint8_t sentMail[0x40];
uint8_t sentMail[] = {'n','o','t','h','i','n','g'};


//==============================================================================
//
//                       FINITE STATE MACHINE FUNCTIONS
//
//==============================================================================

/*******************************************************************************
*  PRIVATE FUNCTION:    startInspiration()
*******************************************************************************/
void start_Inspiration(void)
{ 
  get_Message();
  TIM1_SetCounter(TI1Buffer[3]-100);
  TIM2_CtrlPWMOutputs(ENABLE);
  TIM2_Cmd(ENABLE);
  TIM1_CtrlPWMOutputs(ENABLE);
  TIM1_Cmd(ENABLE);
  reset_RTC_counter(time_in);                           // Reset RTC
  RTC_ITConfig(RTC_IT_WUT, ENABLE);
  wfi();                                                // Wait for event mode
}
/*******************************************************************************
*  PRIVATE FUNCTION:    startExpiration()
*******************************************************************************/
void start_Expiration(void)
{
  TIM2_CtrlPWMOutputs(DISABLE);
  TIM2_Cmd(DISABLE);
  TIM1_CtrlPWMOutputs(DISABLE);
  TIM1_Cmd(DISABLE);
  reset_RTC_counter(time_ex);                           // Reset RTC
  RTC_ITConfig(RTC_IT_WUT, ENABLE);
  //PWR_UltraLowPowerCmd(ENABLE);                         // Ultra low power mode
  halt();                                               // Stop all except RTC
}

/*******************************************************************************
*  PRIVATE FUNCTION:    reconfigure()
*******************************************************************************/
void reconfigure(void)
{
  calculate();                                          // re-do calculations
}
/*******************************************************************************
*  PRIVATE FUNCTION:    reset_RTC_counter()
*******************************************************************************/
void reset_RTC_counter(uint16_t time)
{
  while(!RTC_WakeUpCmd(DISABLE)){;}                     // Disable WakeUp unit
  RTC_SetWakeUpCounter(time);                           // RTC counter to insp.
  while(!RTC_WakeUpCmd(ENABLE)){;}                      // Enable WakeUp unit;
}


//==============================================================================
//
//                         INITIALIZATION FUNCTIONS
//
//==============================================================================

/*******************************************************************************
*  PRIVATE FUNCTION:    initialize()
*******************************************************************************/
void    initialize(void)
{
  calculate();                                          // Calculate stim vals
  PWR_UltraLowPowerCmd(ENABLE);                         // Ultra low power mode
  configure();                                          // Configure peripherals
}
/*******************************************************************************
*  PRIVATE FUNCTION:    calculate()
*******************************************************************************/
void    calculate(void)
{
  uint32_t xy = ((uint32_t)LSE_FREQ/RTC_Div*minpersec)/ //Total breath
    (uint32_t)bpm;
  time_in = (uint16_t)((ie_ratio*xy)/mod_10k)-700;      // Inspiration
  time_ex = ((uint16_t)xy-time_in)-700;                 // Expiration
  
  /*
  CCR1_Val = pulse_width;                               // Pulse width
  DAC_Val = (uint16_t)(((uint32_t)pulse_amp*            // DAC value
                        (uint32_t)res_8bit)/
                        (uint32_t)Vref)/2;
  TIM1_period = ((1000000*mod_100)/                     // fclk/20Hz-1
    pulse_freq)-1;
  */
}
/*******************************************************************************
*  PRIVATE FUNCTION:    configure()
*******************************************************************************/
void    configure(void)
{
  CLK_Config();                                         // Configure clocks
  GPIO_Config();                                        // Configure pin in/out
  TIM1_Config();                                        // Configure TIM1
  TIM2_Config();                                        // Configure TIM2
  TIM4_Config();                                        // Configure TIM4
  DMA1_Config();                                        // Configure DMA
  DAC_Config();                                         // Configure DAC
  RTC_Config();                                         // Configure RTC
  PWR_Config();                                         // Configure PWR
}




//==============================================================================
//
//                    NEAR FIELD COMMUNICATION FUNCTIONS
//
//==============================================================================

/*******************************************************************************
*  PRIVATE FUNCTION:    get_Message()
*******************************************************************************/
void    get_Message(void)
{
  uint8_t PayloadLength;
  if (User_ReadNDEFMessage (&PayloadLength) == SUCCESS)
  {
    parse_Message();
    //if(write_Back()==SUCCESS);
  }
  else
  {
    ;
  }
}

/*******************************************************************************
*  PRIVATE FUNCTION:    write_Back();
*******************************************************************************/
uint8_t    write_Back(void)
{
  uint8_t data = 0x61;
  uint16_t WriteAddr = 0x000D;				
  M24LR04E_Init();     
  M24LR04E_WriteOneByte (M24LR16_EEPROM_ADDRESS_USER, WriteAddr++, data);			
  I2C_Cmd(M24LR04E_I2C, DISABLE);			      
  CLK_PeripheralClockConfig(CLK_Peripheral_I2C1, DISABLE);	
  GPIO_SetBits(M24LR04E_I2C_SCL_GPIO_PORT,M24LR04E_I2C_SCL_PIN);	
  GPIO_SetBits(M24LR04E_I2C_SCL_GPIO_PORT,M24LR04E_I2C_SDA_PIN);	     
  M24LR04E_DeInit();
  I2C_Cmd(M24LR04E_I2C, DISABLE);
  return SUCCESS;
}

/*******************************************************************************
*  PRIVATE FUNCTION:    parse_Message()
*******************************************************************************/
void    parse_Message(void)
{
  bool ERROR = FALSE;
  // Check if message changed (reader wrote new settings)
  if (NDEFmessage==sentMail){return;}
  // Format check (dddd,dddd,dddd,dddd) 
  for (int i=0;i<19;i++)
  {
    if (i==4||i==9||i==14){if(NDEFmessage[i]!=',')ERROR = TRUE;}
    else{if(!isdigit(NDEFmessage[i])){ERROR = TRUE;}}
  }
  // Scan the message (if format is wrong, skip scanning)
  if (ERROR)
    ;
  else
  {
    // Parody on sscanf(); NDEF is of type (dddd,dddd,dddd,dddd, where d = digit
    uint8_t breaks[5] = {0,4,9,14,19};
    uint32_t data[4]={0,0,0,0}, digit = 0;
    for (uint8_t i=0;i<4;i++)
    {
      for (uint8_t j=1;j<5;j++)
      {
        digit = (uint32_t)(NDEFmessage[breaks[i+1]-j]-48);
        for (uint8_t k=j-1;k>0;k--)
        {
          digit = (digit << 3) + (digit << 1);
        }
        data[i] = data[i]+digit;
      }
    }
    
    // Establish ceilings and floors for pulsing controls
    /*
    if      (data[0]>MAX_PW){pulse_width = MAX_PW;}
    else if (data[0]<MIN_PW){pulse_width = MIN_PW;}
    else                    {pulse_width = data[0];}
    
    if      (data[1]>MAX_PA){pulse_amp = MAX_PA;}
    else if (data[1]<MIN_PA){pulse_amp = MIN_PA;}
    else                    {pulse_amp = data[1];}
    
    if      (data[2]>MAX_BR){bpm = MAX_BR;}
    else if (data[2]<MIN_BR){bpm = MIN_BR;}
    else                    {bpm = data[2];}
    
    if      (data[3]>MAX_IE){ie_ratio = MAX_IE;}
    else if (data[3]<MIN_IE){ie_ratio = MIN_IE;}
    else                    {ie_ratio = data[3];}
    */
    
    pulse_width = data[0];
    pulse_amp = data[1];
    bpm = data[2];
    ie_ratio = data[3];
    
    
    uint16_t pw = pulse_width/2;
    TI1Buffer[0] = pw;                            //100           (200us) 3000us
    TI1Buffer[1] = pw*3;                          //500           (1000us) 12ms
    TI1Buffer[2] = pw*mult;                       //10000         (20ms)  30ms
    TI1Buffer[3] = TIM1_PERIOD-pw*(mult+5);       //14400         (28.8ms)
    
    
    DAC_High = (uint16_t)(((uint32_t)pulse_amp*            // DAC value
                        (uint32_t)res_12bit)*5/
                        (uint32_t)Vref_12bit)/2;
    
    DAC_Low = DAC_High/mult;             // about 0.3mA
    
    //TI2Buffer[0] = 50;
    //TI2Buffer[1] = 0xFFFF;
    //TI2Buffer[2] = 50;
    //TI2Buffer[3] = 0xFFFF;
    
    DACBuffer[0] = DAC_High;
    DACBuffer[1] = DAC_Low;
    DACBuffer[2] = DAC_Low;
    DACBuffer[3] = DAC_High;
    
    //reconfigure();
  }
}

/*******************************************************************************
*  PRIVATE FUNCTION:    User_ReadNDEFMessage()
*******************************************************************************/
static int8_t User_ReadNDEFMessage ( uint8_t *PayloadLength )			
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
static ErrorStatus User_CheckNDEFMessage(void)
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
static ErrorStatus User_GetPayloadLength(uint8_t *PayloadLength)
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
static ErrorStatus User_GetNDEFMessage(uint8_t  PayloadLength,uint8_t *NDEFmessage)
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
static void InitializeBuffer (uint8_t *Buffer ,uint8_t NbCar)
{
  do{
    Buffer[NbCar]= 0;
  } while (NbCar--);
}




/*******************************************************************************
**********************************   END   *************************************
*******************************************************************************/