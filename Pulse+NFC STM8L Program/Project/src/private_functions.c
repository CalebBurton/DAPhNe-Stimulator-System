/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%                                                                        %%%%
%%%%          BME 390: Phrenic Nerve Stimulation: Implant Program           %%%%
%%%%                          private_functions.c                           %%%%
%%%%                                                                        %%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%	
********************************************************************************
*	Author:		Alexey Revinski
*	Last Revised:	11/20/2016
*******************************************************************************/
#include "private_functions.h"

/*******************************************************************************
*  GLOBAL VARIABLES
*******************************************************************************/
bool            sleeping        = TRUE;                 // State variable
uint16_t        time_in         = RESET;
uint16_t        time_ex         = RESET;
uint16_t        CCR1_Val        = RESET;
uint16_t        TIM1_period     = RESET;
uint16_t        DAC_Val         = RESET;
uint16_t        RTC_Div         = 2;
uint16_t        dummy_time      = 1000;
uint16_t        mod_10k         = 10000;
uint16_t        mod_100         = 100;
uint16_t        res_8bit        = 256;
uint16_t        Vref            = 270;
uint16_t        minpersec       = 6000;

/*******************************************************************************
*  EXTERNAL VARIABLES
*******************************************************************************/
extern uint32_t pulse_freq;
extern uint32_t pulse_width;
extern uint32_t pulse_amp;
extern uint32_t bpm;
extern uint32_t ie_ratio;
extern IN_RAM uint8_t NDEFmessage[0x40];

volatile char FLASH_CR2     @0x5051;    
volatile char FLASH_NCR2    @0x17ff;    
volatile char FLASH_IAPSR   @0x5054;  

/*******************************************************************************
*  PRIVATE FUNCTION:    initialize()
*******************************************************************************/
void    initialize(void)
{
  calculations();
  CLK_DeInit();                                         // Reset to HSI
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_8);                 // 1000000 Hz
  PWR_FastWakeUpCmd(DISABLE);
  GPIO_Config();                                        // Configure GPIO pins
  DAC_Config();                                         // Configure DAC output
  TIM1_Config();                                        // Configure TIM1
  RTC_Config();                                         // Configure RTC
  RTC_WakeUpCmd(ENABLE);                                // Enable RTC WakeUp
  enableInterrupts();                                   // Enable interrupts
}

/*******************************************************************************
*  PRIVATE FUNCTION:    startInspiration()
*******************************************************************************/
void start_Inspiration(void)
{
  sleeping = FALSE;                                     // Change state
  check_Message();
  reset_RTC_counter(time_in);                           // Reset RTC
  TIM1_SetCounter(0);                                   // Reset counter
  TIM1_Cmd(ENABLE);                                     // Start Timer 1
  TIM1_CtrlPWMOutputs(ENABLE);                          // Enable PWM output
  wfi();                                                // Wait for event mode
}

/*******************************************************************************
*  PRIVATE FUNCTION:    startExpiration()
*******************************************************************************/
void start_Expiration(void)
{
  sleeping = TRUE;                                      // Change state
  TIM1_CtrlPWMOutputs(DISABLE);                         // Disable PWM output
  reset_RTC_counter(time_ex);                           // Reset RTC
  PWR_UltraLowPowerCmd(ENABLE);                         // Ultra low power mode
  halt();                                               // Stop all except RTC
}

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
        //ToUpperCase (*PayloadLength,NDEFmessage);
        return SUCCESS;
      }
    }	
    M24LR04E_DeInit();
    I2C_Cmd(M24LR04E_I2C, DISABLE);
  }	
  return ERROR;
}


static ErrorStatus User_CheckNDEFMessage(void)
{
  uint8_t *OneByte = 0x00;
  uint16_t ReadAddr = 0x0000;
	
  // check the E1 at address 0
  M24LR04E_ReadOneByte (M24LR16_EEPROM_ADDRESS_USER, ReadAddr, OneByte);	
  if (*OneByte != 0xE1)
          return ERROR;
  ReadAddr = 0x0009;
  M24LR04E_ReadOneByte (M24LR16_EEPROM_ADDRESS_USER, ReadAddr, OneByte);	
  // text or URL message
  if (*OneByte != 0x54 /*&& *OneByte != 0x55*/)
          return ERROR;  
  return SUCCESS;	
}

static ErrorStatus User_GetPayloadLength(uint8_t *PayloadLength)
{
  uint16_t ReadAddr = 0x0008;
  *PayloadLength = 0x00;
  M24LR04E_ReadOneByte (M24LR16_EEPROM_ADDRESS_USER, ReadAddr, PayloadLength);	
  if (*PayloadLength == 0x00)
          return ERROR;     
  return SUCCESS;	
}

static ErrorStatus User_GetNDEFMessage(uint8_t  PayloadLength,uint8_t *NDEFmessage)
{
  uint16_t ReadAddr = 0x000D;
  if (PayloadLength == 0x00)
          return SUCCESS;		
          
  M24LR04E_ReadBuffer (M24LR16_EEPROM_ADDRESS_USER, ReadAddr,PayloadLength, NDEFmessage);
  return SUCCESS;	
}

static void InitializeBuffer (uint8_t *Buffer ,uint8_t NbCar)
{
  do{
    Buffer[NbCar]= 0;
  }	while (NbCar--);
}

void check_Message(void)
{
  uint8_t PayloadLength;
  FLASH_Unlock(FLASH_MemType_Data );
  if (User_ReadNDEFMessage (&PayloadLength) == SUCCESS)
  {
    if (NDEFmessage == "alex")
    {
      GPIO_SetBits(PC7_PORT,PC7_PIN);
    }
    else
    {
      GPIO_SetBits(PC7_PORT,PC7_PIN);
    }
  }
  else
  {
    ;//GPIO_SetBits(PC7_PORT,PC7_PIN);
  }
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

/*******************************************************************************
*  PRIVATE FUNCTION:    GPIO_Config()
*******************************************************************************/
void    GPIO_Config(void)
{
  GPIO_Init(PE7_PORT,PE7_PIN,GPIO_Mode_Out_PP_Low_Slow); // GREEN LED output
  GPIO_Init(PC7_PORT,PC7_PIN,GPIO_Mode_Out_PP_Low_Slow); // BLUE LED output
  GPIO_Init(PD2_PORT,PD2_PIN,GPIO_Mode_Out_PP_Low_Fast); // TIM1 Channel 1
  GPIO_Init(PF0_PORT,PF0_PIN,GPIO_Mode_In_FL_No_IT);     // DAC Channel 1
  GPIO_ResetBits(PE7_PORT, PE7_PIN);                     // Turn off GREEN LED
  GPIO_ResetBits(PC7_PORT, PC7_PIN);                     // Turn off BLUE LED
}

/*******************************************************************************
*  PRIVATE FUNCTION:    TIM1_Config()
******************************************************************************/
void    TIM1_Config(void)
{
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM1,ENABLE);// Enable TIM1 clocking
  TIM1_TimeBaseInit(TIM1_PSR,TIM1_CounterMode_Up,       // Initialize time base
                    TIM1_period,TIM1_REP);
  TIM1_OC1Init(TIM1_OCMode_PWM1,                        // Set up PWM
               TIM1_OutputState_Enable,
               TIM1_OutputNState_Disable,
               CCR1_Val, TIM1_OCPolarity_High,
               TIM1_OCNPolarity_High,
               TIM1_OCIdleState_Set,
               TIM1_OCNIdleState_Set);
  TIM1_OC1PreloadConfig(ENABLE);                        // Enable configuration
  TIM1_ARRPreloadConfig(ENABLE);                        // Enable configuration
}

/*******************************************************************************
*  PRIVATE FUNCTION:    RTC_Config()
*******************************************************************************/
void    RTC_Config(void)
{
  CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);// Enable RTC clocking
  CLK_RTCClockConfig(CLK_RTCCLKSource_LSE,              // Set LSE as RTC source
                     CLK_RTCCLKDiv_1);                  
  RTC_RatioCmd(ENABLE);                                 // No sync(fclk=frtc)
  RTC_WakeUpCmd(DISABLE);                               // Disable WakeUp unit
  RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div2);   // frtc/2 = 16384
  RTC_ITConfig(RTC_IT_WUT, ENABLE);                     // Enable interrupts
  RTC_SetWakeUpCounter(dummy_time);                     // Dummy counter value
}

/*******************************************************************************
*  PRIVATE FUNCTION:    DAC_Config()
*******************************************************************************/
void    DAC_Config(void)
{
  CLK_PeripheralClockConfig(CLK_Peripheral_DAC, ENABLE);// Route clock to DAC
  DAC_DeInit();                                         // Deinitialize DAC
  DAC_Init(DAC_Channel_1,DAC_Trigger_None,              // Initialize DAC
           DAC_OutputBuffer_Enable);
  DAC_SetChannel1Data(DAC_Align_8b_R,DAC_Val);          // DAC output value
  DAC_Cmd(DAC_Channel_1, ENABLE);                       // Enable DAC output
}

/*******************************************************************************
*  PRIVATE FUNCTION:    calculations()
*******************************************************************************/
void    calculations(void)
{
  uint32_t xy = ((uint32_t)LSE_FREQ/RTC_Div*minpersec)/ //Total breath
    (uint32_t)bpm;
  time_in = (uint16_t)((ie_ratio*xy)/mod_10k);          // Inspiration
  time_ex = ((uint16_t)xy-time_in);                     // Expiration
  CCR1_Val = pulse_width;                               // Pulse width
  TIM1_period = (CLK_GetClockFreq()*mod_100)/           // fclk/20Hz-1
    pulse_freq-1;
  DAC_Val = (uint16_t)(((uint32_t)pulse_amp*            // DAC value
                        (uint32_t)res_8bit)/
                        (uint32_t)Vref)/2;
}

/*******************************************************************************
**********************************   END   *************************************
*******************************************************************************/