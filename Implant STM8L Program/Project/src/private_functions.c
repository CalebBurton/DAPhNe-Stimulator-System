/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%                                                                        %%%%
%%%%          BME 390: Phrenic Nerve Stimulation: Implant Program           %%%%
%%%%                          private_functions.c                           %%%%
%%%%                                                                        %%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%	
********************************************************************************
*	Author:		Alexey Revinski
*	Last Revised:	__/__/2016
*******************************************************************************/

#include "stm8l15x.h"
#include "private_functions.h"

/*******************************************************************************
*  PRIVATE FUNCTION:    initialize()
*******************************************************************************/
void    initialize(void)
{
  //Switch_To_HSI();
  //CLK_PeripheralClockConfig(CLK_Peripheral_TIM2,ENABLE);
  disableInterrupts();
  GPIO_Config();
  TIM2_Config();
  RTC_Config();
  enableInterrupts();
}

/*******************************************************************************
*  PRIVATE FUNCTION:    startInspiration()
*******************************************************************************/
void start_Inspiration(void)
{
  RTC_WakeUpCmd(DISABLE);
  Switch_To_HSI();
  RTC_ITConfig(RTC_IT_WUT,DISABLE);
}
/*******************************************************************************
*  PRIVATE FUNCTION:    startExpiration()
*******************************************************************************/
void start_Expiration(void)
{
  TIM2_Cmd(DISABLE);
  Switch_To_LSI();
  CLK_PeripheralClockConfig(CLK_Peripheral_RTC,ENABLE); // Enable RTC
  RTC_WakeUpCmd(ENABLE);
  RTC_ITConfig(RTC_IT_WUT,ENABLE);
}

/*******************************************************************************
*  PRIVATE FUNCTION:    GPO_Configuration
*       * Configures used and unused GPIO pins on the microcontroller
*******************************************************************************/
void    GPIO_Config(void)
{
  GPIO_Init(PE7_PORT,PE7_PIN,GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(PC7_PORT,PC7_PIN,GPIO_Mode_Out_PP_Low_Slow);
  GPIO_SetBits(PE7_PORT, PE7_PIN);
  GPIO_ResetBits(PC7_PORT, PC7_PIN);
}

/*******************************************************************************
*  PRIVATE FUNCTION:    Switch_To_HSI()
*       * Initializes the HSI (16 MHz) clock
*       * Divides the clock by 128 => 125000 Hz
*******************************************************************************/
void    Switch_To_HSI(void)
{
  CLK_SYSCLKSourceSwitchCmd(ENABLE);                    // Enable switching clock sources
  CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI);         // Switch to High Speed Internal Oscillator
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_128);               // Divide clock by 128 (16000000/128 = 125000 Hz)
  while(CLK_GetSYSCLKSource()!=CLK_SYSCLKSource_HSI){;} // Wait until conversion occurs
}

/*******************************************************************************
*  PRIVATE FUNCTION:    Switch_To_LSI()
*       * Initializes the LSI (38kHz) clock
*       * Divides the clock by 1 ==> 38000 Hz
*******************************************************************************/
void    Switch_To_LSI(void)
{
  CLK_SYSCLKSourceSwitchCmd(ENABLE);                    // Enable switching clock sources
  CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_LSI);         // Switch to Low Speed Internal Oscillator
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);                 // Divide clock by 1(38000/1 = 38000 Hz)
  while(CLK_GetSYSCLKSource()!=CLK_SYSCLKSource_LSI){;} // Wait until conversion occurs
}

/*******************************************************************************
*  PRIVATE FUNCTION:    TIM_Configuration
*       * Initializes 1 s, 16 bit timer (TIM2)
*       * Sets up timer update interrupt
*       * Calculations:
*               F_clk = 16000000/128 = 125000;
*               F_tim = 125000/1 = 125000;
*               Period = (0.05*125000Hz)-1 = 6249 cycles;
*               Actual time period = 1s.
*
*       CLK:    1:2:4:8:16:32:64:128
*       TIM:    1:2:4:8:16:32:64:128
*       BASE:   1:65535
******************************************************************************/
void    TIM2_Config(void)
{
  TIM2_TimeBaseInit(TIM2_Prescaler_1,
                    TIM2_CounterMode_Up,TIME_BASE);     // Initialize time base
  TIM2_ClearITPendingBit(TIM2_IT_Update);               // Clear interrupt flag
  TIM2_ITConfig(TIM2_IT_Update, ENABLE);                // Enable timer ITs
  TIM2_Cmd(ENABLE);                                     // Start timer 
}

/*******************************************************************************
*  PRIVATE FUNCTION:    RTC_Configuration
*******************************************************************************/
void    RTC_Config(void)
{  
  CLK_RTCClockConfig(CLK_RTCCLKSource_LSI, CLK_RTCCLKDiv_1);    // Set RTC to use LSI (38kHz)
  RTC_RatioCmd(ENABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);
  RTC_WakeUpCmd(DISABLE);
  RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16);
  RTC_SetWakeUpCounter(4600); //SHOULD BE 2375
}

/*******************************************************************************
**********************************   END   *************************************
*******************************************************************************/