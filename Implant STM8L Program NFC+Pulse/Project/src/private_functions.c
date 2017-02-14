/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%                                                                        %%%%
%%%%          BME 390: Phrenic Nerve Stimulation: Implant Program           %%%%
%%%%                          private_functions.c                           %%%%
%%%%                                                                        %%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%	
********************************************************************************
*	Author:		Alexey Revinski
*	Last Revised:	12/15/2016
*******************************************************************************/
#include "private_functions.h"

/*******************************************************************************
*  GLOBAL VARIABLES
*******************************************************************************/
extern bool     sleeping;                 // State variable
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
uint16_t        Vref            = 290;
uint16_t        minpersec       = 6000;

/*******************************************************************************
*  EXTERNAL VARIABLES
*******************************************************************************/
extern uint32_t pulse_freq;
extern uint32_t pulse_width;
extern uint32_t pulse_amp;
extern uint32_t bpm;
extern uint32_t ie_ratio;
uint8_t NDEFmessage[0x40];
//uint8_t sentMail[0x40];
uint8_t sentMail[] = {'n','o','t','h','i','n','g'};

/*******************************************************************************
*  PRIVATE FUNCTION:    initialize()
*******************************************************************************/
void    initialize(void)
{
  DeInitClock();
  CLK_DeInit();                                         // Reset to HSI
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_8);                 // 1000000 Hz
  DeInitGPIO();                                         // Configure GPIO pins
  PWR_FastWakeUpCmd(DISABLE);
  calculations();
  DAC_Config();                                         // Configure DAC output
  TIM1_Config();                                        // Configure TIM1
  RTC_Config();                                         // Configure RTC
  RTC_WakeUpCmd(ENABLE);                                // Enable RTC WakeUp
  enableInterrupts();                                   // Enable interrupts
}

/*******************************************************************************
*  PRIVATE FUNCTION:    reconfigure()
*******************************************************************************/
void reconfigure(void)
{
  calculations();                                       // re-do calculations
  DAC_Config();                                         // Configure DAC output
  TIM1_Config();                                        // Configure TIM1;
}

/*******************************************************************************
*  PRIVATE FUNCTION:    startInspiration()
*******************************************************************************/
void start_Inspiration(void)
{ 
  get_Message();
  TIM1_SetCounter(0);                                   // Reset counter
  TIM1_Cmd(ENABLE);                                     // Start Timer 1
  TIM1_CtrlPWMOutputs(ENABLE);                          // Enable PWM output
  reset_RTC_counter(time_in);                           // Reset RTC
  RTC_ITConfig(RTC_IT_WUT, ENABLE);
  wfi();                                                // Wait for event mode
}

/*******************************************************************************
*  PRIVATE FUNCTION:    startExpiration()
*******************************************************************************/
void start_Expiration(void)
{
  TIM1_CtrlPWMOutputs(DISABLE);                         // Disable PWM output
  TIM1_Cmd(DISABLE);                                    // Start Timer 1
  PWR_UltraLowPowerCmd(ENABLE);                         // Ultra low power mode
  reset_RTC_counter(time_ex);                           // Reset RTC
  RTC_ITConfig(RTC_IT_WUT, ENABLE);
  halt();                                               // Stop all except RTC
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
*  PRIVATE FUNCTION:    TIM1_Config()
******************************************************************************/
void    TIM1_Config(void)
{
  GPIO_Init(PD2_PORT,PD2_PIN,GPIO_Mode_Out_PP_Low_Fast);// TIM1 Channel 1
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
  GPIO_Init(PF0_PORT,PF0_PIN,GPIO_Mode_In_FL_No_IT);    // DAC Channel 1
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
  time_in = (uint16_t)((ie_ratio*xy)/mod_10k)-700;      // Inspiration
  time_ex = ((uint16_t)xy-time_in)-700;                 // Expiration
  CCR1_Val = pulse_width;                               // Pulse width
  DAC_Val = (uint16_t)(((uint32_t)pulse_amp*            // DAC value
                        (uint32_t)res_8bit)/
                        (uint32_t)Vref)/2;
  TIM1_period = ((1000000*mod_100)/                     // fclk/20Hz-1
    pulse_freq)-1;

}

/*******************************************************************************
*  PRIVATE FUNCTION:    get_Message()
*******************************************************************************/
void    get_Message(void)
{
  uint8_t PayloadLength;
  if (User_ReadNDEFMessage (&PayloadLength) == SUCCESS)
  {
    GPIO_ResetBits(PC7_PORT,PC7_PIN);
    parse_Message();
    if(write_Back()==SUCCESS) GPIO_SetBits(PE7_PORT,PE7_PIN);
  }
  else
  {
    GPIO_SetBits(PC7_PORT,PC7_PIN);
  }
}

/*******************************************************************************
*  PRIVATE FUNCTION:    write_Back();
*******************************************************************************/
int8_t    write_Back(void)
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
    GPIO_SetBits(PC7_PORT,PC7_PIN);
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
    reconfigure();
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
*  PRIVATE FUNCTION:    DeInitGPIO()
*******************************************************************************/
static void DeInitGPIO ( void )
{
  GPIO_Init( GPIOA, GPIO_Pin_All, GPIO_Mode_Out_OD_Low_Fast);
  GPIO_Init( GPIOB, GPIO_Pin_All, GPIO_Mode_Out_OD_Low_Fast);
  GPIO_Init( GPIOC, GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6, GPIO_Mode_Out_OD_Low_Fast);
  GPIO_Init(PC7_PORT,PC7_PIN,GPIO_Mode_Out_PP_Low_Slow); // BLUE LED output
  GPIO_Init( GPIOD, GPIO_Pin_All, GPIO_Mode_Out_OD_Low_Fast);
  GPIO_Init( GPIOE, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6, GPIO_Mode_Out_OD_Low_Fast);
  GPIO_Init(PE7_PORT,PE7_PIN,GPIO_Mode_Out_PP_Low_Slow); // GREEN LED output
  GPIOA->ODR = 0xFF;
  GPIOB->ODR = 0xFF;
  GPIOC->ODR = 0xFF;
  GPIOD->ODR = 0xFF;
  GPIOE->ODR = 0xFF;
  GPIO_ResetBits(PC7_PORT,PC7_PIN);
  GPIO_ResetBits(PE7_PORT,PE7_PIN);
}

/*******************************************************************************
*  PRIVATE FUNCTION:    DeInitClock()
*******************************************************************************/
static void DeInitClock ( void )
{
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM2, DISABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM3, DISABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, DISABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_I2C1, DISABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_SPI1, DISABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_USART1, DISABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_BEEP, DISABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_DAC, DISABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, DISABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM1, DISABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_RTC, DISABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_LCD, DISABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, DISABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_DMA1, DISABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, DISABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_BOOTROM, DISABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_AES, DISABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, DISABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM5, DISABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_SPI2, DISABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_USART2, DISABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_USART3, DISABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_CSSLSE, DISABLE);
}

/*******************************************************************************
**********************************   END   *************************************
*******************************************************************************/