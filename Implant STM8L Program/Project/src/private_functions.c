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

#include "stm8l15x.h"
#include "private_functions.h"

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
extern uint32_t pulse_freq;
extern uint32_t pulse_width;
extern uint32_t pulse_amp;
extern uint32_t bpm;
extern uint32_t ie_ratio;

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
  sleeping = FALSE;
  RTC_WakeUpCmd(DISABLE);                               // Disable WakeUp unit
  RTC_SetWakeUpCounter(time_in);                        // RTC counter to insp.
  while(!RTC_WakeUpCmd(ENABLE)){;}                      // Enable WakeUp unit
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
  sleeping = TRUE;
  TIM1_CtrlPWMOutputs(DISABLE);                         // Disable PWM output
  RTC_WakeUpCmd(DISABLE);                               // Disable WakeUp unit
  RTC_SetWakeUpCounter(time_ex);                        // RTC counter to exp.
  RTC_WakeUpCmd(ENABLE);                                // Disable WakeUp unit
  PWR_UltraLowPowerCmd(ENABLE);                         // Ultra low power mode
  halt();                                               // Stop all except RTC
}

/*******************************************************************************
*  PRIVATE FUNCTION:    GPIO_Config
*       * Configures used and unused GPIO pins on the microcontroller
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
*  PRIVATE FUNCTION:    TIM1_Config
*       * Initializes TIM1
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
*  PRIVATE FUNCTION:    RTC_Config
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
*  PRIVATE FUNCTION:    DAC_Config
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
*  PRIVATE FUNCTION:    calculate_RTC
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