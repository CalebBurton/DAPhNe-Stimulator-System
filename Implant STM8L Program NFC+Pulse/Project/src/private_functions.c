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
uint16_t        res_12bit       = 4096;
uint16_t        Vref_12bit      = 3800;
uint16_t        minpersec       = 6000;

extern uint16_t TI1Buffer[];   //TIM1 time bases buffer
extern uint16_t TI2Buffer[];   //TIM2 polarity buffer
extern uint16_t DACBuffer[];   //DAC1 values buffer
extern uint16_t mult;
extern uint16_t DAC_High;                // about 3mA
extern uint16_t DAC_Low;

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
  GPIO_SetBits(LEDG_PORT,LEDG_PIN);
  peripherals(ENABLE);
  reset_RTC_counter(time_in);                           // Reset RTC
  RTC_ITConfig(RTC_IT_WUT, ENABLE);
  //get_Message();
  wfi();                                                // Wait for event mode
}
/*******************************************************************************
*  PRIVATE FUNCTION:    startExpiration()
*******************************************************************************/
void start_Expiration(void)
{
  GPIO_ResetBits(LEDG_PORT,LEDG_PIN);
  reset_RTC_counter(time_ex);                           // Reset RTC
  RTC_ITConfig(RTC_IT_WUT, ENABLE);
  peripherals(DISABLE);
  PWR_UltraLowPowerCmd(ENABLE);                         // Ultra low power mode
  halt();                                               // Stop all except RTC
}

/*******************************************************************************
*  PRIVATE FUNCTION:    peripherals()
*******************************************************************************/
void peripherals(FunctionalState NewState)
{

  CLK_PeripheralClockConfig(CLK_Peripheral_TIM1,NewState);// TIM1 clock
  if(NewState==ENABLE)
  {
    TIM1_Config();                                        // Configure TIM1
  }
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM2,NewState);// TIM2 clock
  if(NewState==ENABLE)
  {
    TIM2_Config();                                        // Configure TIM2
  }
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM4,NewState);// TIM4 clock
  if(NewState==ENABLE)
  {
    TIM4_Config();                                        // Configure TIM4
  }
  CLK_PeripheralClockConfig(CLK_Peripheral_DMA1,NewState);// DMA1 clock
  if(NewState==ENABLE)
  {
    DMA1_Config();                                        // Configure DMA
  }
  CLK_PeripheralClockConfig(CLK_Peripheral_DAC, NewState);// DAC clock
  if(NewState==ENABLE)
  {
    DAC_Config();                                         // Configure DAC;
  }
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
  configure();                                          // Configure peripherals
  GPIO_SetBits(LEDG_PORT,LEDG_PIN);
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
//                    PERIPHERALS CONFIGURATION FUNCTIONS
//
//==============================================================================

/*******************************************************************************
*  PRIVATE FUNCTION:    CLK_Config()
*******************************************************************************/
void    CLK_Config(void)
{
  DeInitClock();
  CLK_DeInit();                                         // Reset to HSI
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_16);                // 1MHz
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM1,ENABLE);// Enable TIM1 clock
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM2,ENABLE);// Enable TIM2 clock
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM4,ENABLE);// Enable TIM4 clock
  CLK_PeripheralClockConfig(CLK_Peripheral_DMA1,ENABLE);// Enable DMA1 clock
  CLK_PeripheralClockConfig(CLK_Peripheral_DAC, ENABLE);// Enable DAC clock
  CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);// Enable RTC clock
  CLK_RTCClockConfig(CLK_RTCCLKSource_LSE,              // Set LSE as RTC source
                     CLK_RTCCLKDiv_1);
}
/*******************************************************************************
*  PRIVATE FUNCTION:    GPIO_Config()
*******************************************************************************/
void    GPIO_Config(void)
{
  DeInitGPIO();                                         // Configure GPIO pins
  GPIO_Init(PD2_PORT,PD2_PIN,GPIO_Mode_Out_PP_Low_Fast);// TIM1 Channel 1
  GPIO_Init(PB0_PORT,PB0_PIN,GPIO_Mode_Out_PP_Low_Fast);// TIM2 Channel 1
  GPIO_Init(PF0_PORT,PF0_PIN,GPIO_Mode_In_FL_No_IT);    // DAC Channel 1
  GPIO_Init(LEDG_PORT,LEDG_PIN,GPIO_Mode_Out_PP_Low_Fast);// LED GREEN
  GPIO_Init(PD5_PORT,PD5_PIN,GPIO_Mode_Out_PP_Low_Fast);// SHDN Pin on 10V source
  GPIO_ResetBits(LEDG_PORT,LEDG_PIN);                     // TURN OFF LED GREEN
}
/*******************************************************************************
*  PRIVATE FUNCTION:    TIM1_Config()
*******************************************************************************/
void    TIM1_Config(void)
{
  // 1MHz/(1+1) = 500kHz, TIM1_Period can be:
  //    25000 for 50ms
  //    30000 for 60ms
  //    40000 for 80ms
  //    50000 for 100ms
  //    60000 for 120ms
  TIM1_DeInit();
  TIM1_TimeBaseInit(TIM1_PRESCALER,
                    TIM1_CounterMode_Up,
                    TIM1_PERIOD,
                    TIM1_REPTETION_COUNTER);
  TIM1_OC1Init(TIM1_OCMode_Toggle,
               TIM1_OutputState_Enable,
               TIM1_OutputNState_Disable,
               0,
               TIM1_OCPolarity_Low,
               TIM1_OCNPolarity_Low,
               TIM1_OCIdleState_Set,
               TIM1_OCNIdleState_Set);
  TIM1_SelectOutputTrigger(TIM1_TRGOSource_Update);
  TIM1_ARRPreloadConfig(ENABLE);
  TIM1_DMACmd(TIM1_DMASource_Update, ENABLE);
  TIM1_CtrlPWMOutputs(ENABLE);
  TIM1_Cmd(ENABLE);
}
/*******************************************************************************
*  PRIVATE FUNCTION:    TIM2_Config()
*******************************************************************************/
void    TIM2_Config(void)
{
  TIM2_DeInit();
  TIM2_TimeBaseInit(TIM2_Prescaler_1,
                    TIM2_CounterMode_Up,
                    0xFFFF);
  TIM2_OC1Init(TIM2_OCMode_Toggle,
               TIM2_OutputState_Enable,
               0,
               TIM2_OCPolarity_Low,// CHANGE TO HIGH
               TIM2_OCIdleState_Set);
  TIM2_SelectSlaveMode(TIM2_SlaveMode_Reset);
  TIM2_OC1PreloadConfig(ENABLE);
  TIM2_SelectInputTrigger(TIM2_TRGSelection_TIM1);
  TIM2_DMACmd(TIM2_DMASource_Update, ENABLE);
  TIM2_CtrlPWMOutputs(ENABLE);
  TIM2_Cmd(ENABLE);
}
/*******************************************************************************
*  PRIVATE FUNCTION:    TIM4_Config()
*******************************************************************************/
void    TIM4_Config(void)
{
  TIM4_DeInit();
  TIM4_TimeBaseInit(TIM4_Prescaler_1, 1);
  TIM4_SelectOnePulseMode(TIM4_OPMode_Single);
  TIM4_SelectSlaveMode(TIM4_SlaveMode_Reset);
  TIM4_SelectInputTrigger(TIM4_TRGSelection_TIM1);
  TIM4_SelectOutputTrigger(TIM4_TRGOSource_Update);
  TIM4_Cmd(ENABLE);
}
/*******************************************************************************
*  PRIVATE FUNCTION:    DMA1_Config()
*******************************************************************************/
void    DMA1_Config(void)
{
  DMA_GlobalDeInit();
  DMA_Init(DMA1_Channel1, (uint16_t)TI2Buffer, TIM2_OC1_ADDRESS,
           BUFSIZE,DMA_DIR_MemoryToPeripheral,DMA_Mode_Circular,
           DMA_MemoryIncMode_Inc,DMA_Priority_High,DMA_MemoryDataSize_HalfWord);
  
  DMA_Init(DMA1_Channel2, (uint16_t)TI1Buffer, TIM1_ARR_ADDRESS,
           BUFSIZE,DMA_DIR_MemoryToPeripheral,DMA_Mode_Circular,
           DMA_MemoryIncMode_Inc,DMA_Priority_High,DMA_MemoryDataSize_HalfWord);
  
  DMA_Init(DMA1_Channel3, (uint32_t)DACBuffer, DAC_CH1RDHRH_ADDRESS,
           BUFSIZE,DMA_DIR_MemoryToPeripheral,DMA_Mode_Circular,
           DMA_MemoryIncMode_Inc,DMA_Priority_High,DMA_MemoryDataSize_HalfWord);
  DMA_Cmd(DMA1_Channel1, ENABLE);
  DMA_Cmd(DMA1_Channel2, ENABLE);
  DMA_Cmd(DMA1_Channel3, ENABLE);
  DMA_GlobalCmd(ENABLE);
}
/*******************************************************************************
*  PRIVATE FUNCTION:    DAC_Config()
*******************************************************************************/
void    DAC_Config(void)
{
  DAC_DeInit();
  DAC_Init(DAC_Channel_1,DAC_Trigger_T4_TRGO,
           DAC_OutputBuffer_Enable);
  DAC_DMACmd(DAC_Channel_1, ENABLE);
  DAC_Cmd(DAC_Channel_1, ENABLE);
}
/*******************************************************************************
*  PRIVATE FUNCTION:    RTC_Config()
*******************************************************************************/
void    RTC_Config(void)
{
  RTC_WakeUpCmd(DISABLE);                               // Disable WakeUp unit
  RTC_RatioCmd(ENABLE);                                 // No sync(fclk=frtc)
  RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div2);   // frtc/2 = 16384
  RTC_ITConfig(RTC_IT_WUT, ENABLE);                     // Enable interrupts
  RTC_SetWakeUpCounter(dummy_time);                     // Dummy counter value
  RTC_WakeUpCmd(ENABLE);                                // Enable RTC WakeUp
}
/*******************************************************************************
*  PRIVATE FUNCTION:    PWR_Config()
*******************************************************************************/
void    PWR_Config(void)
{
  PWR_FastWakeUpCmd(DISABLE);
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
    
    uint16_t pw = pulse_width/2;
    TI1Buffer[0] = pw*3;                          //500           (1000us) 12ms
    TI1Buffer[1] = pw*mult;                       //10000         (20ms)  30ms
    TI1Buffer[2] = TIM1_PERIOD-pw*(mult+5);       //14400         (28.8ms)
    TI1Buffer[3] = pw;                            //100           (200us) 3000us
    
    DAC_High = (uint16_t)(((uint32_t)pulse_amp*            // DAC value
                        (uint32_t)res_12bit)*5/
                        (uint32_t)Vref_12bit)/2;
    
    
    DAC_Low = DAC_High/mult;             // about 0.3mA
    
    //TI2Buffer[0] = 50;
    //TI2Buffer[1] = 0xFFFF;
    //TI2Buffer[2] = 50;
    //TI2Buffer[3] = 0xFFFF;
    
    DACBuffer[0] = DAC_Low;
    DACBuffer[1] = DAC_Low;
    DACBuffer[2] = DAC_High;
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


//==============================================================================
//
//                         DEINITIALIZATION FUNCTIONS
//
//==============================================================================

/*******************************************************************************
*  PRIVATE FUNCTION:    DeInitGPIO()
*******************************************************************************/
static void DeInitGPIO ( void )
{
  GPIO_Init( GPIOA, GPIO_Pin_All, GPIO_Mode_Out_OD_Low_Fast);
  GPIO_Init( GPIOB, GPIO_Pin_All, GPIO_Mode_Out_OD_Low_Fast);
  GPIO_Init( GPIOC, GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6, GPIO_Mode_Out_OD_Low_Fast);
  GPIO_Init( GPIOD, GPIO_Pin_All, GPIO_Mode_Out_OD_Low_Fast);
  GPIO_Init( GPIOE, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6, GPIO_Mode_Out_OD_Low_Fast);
  GPIO_Init(LEDG_PORT,LEDG_PIN,GPIO_Mode_Out_PP_Low_Slow); // GREEN LED output
  GPIOA->ODR = 0xFF;
  GPIOB->ODR = 0xFF;
  GPIOC->ODR = 0xFF;
  GPIOD->ODR = 0xFF;
  GPIOE->ODR = 0xFF;
  GPIO_ResetBits(LEDG_PORT,LEDG_PIN);
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