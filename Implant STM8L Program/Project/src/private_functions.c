/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%                                                                        %%%%
%%%%          BME 390: Phrenic Nerve Stimulation: Implant Program           %%%%
%%%%                          private_functions.c                           %%%%
%%%%                                                                        %%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%	
********************************************************************************
*	Author:		Alexey Revinski
*	Last Revised:	11/05/2016
*******************************************************************************/

#include "stm8l15x.h"
#include "private_functions.h"

/*******************************************************************************
*  PRIVATE FUNCTION:    initialize()
*******************************************************************************/
void    initialize(void)
{
  CLK_DeInit();                         // Reset to default clock values (HSI)
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_128);// Divide clock to get 125000 Hz
  GPIO_Config();                        // Configure GPIO pins
  TIM2_Config();                        // Configure Timer 2
  RTC_Config();                         // Configure Real Time Clock
  RTC_WakeUpCmd(ENABLE);                // Enable RTC WakeUp
  enableInterrupts();                   // Enable interrupts
}

/*******************************************************************************
*  PRIVATE FUNCTION:    startInspiration()
*******************************************************************************/
void start_Inspiration(void)
{
  TIM2_Cmd(ENABLE);                     // Start Timer 2
}

/*******************************************************************************
*  PRIVATE FUNCTION:    startExpiration()
*******************************************************************************/
void start_Expiration(void)
{
  TIM2_Cmd(DISABLE);                    // Turn off Timer 2
  PWR_UltraLowPowerCmd(ENABLE);         // Put MCU into ultra low power
  halt();                               // Kill everything except RTC
}

/*******************************************************************************
*  PRIVATE FUNCTION:    GPO_Configuration
*       * Configures used and unused GPIO pins on the microcontroller
*******************************************************************************/
void    GPIO_Config(void)
{
  GPIO_Init(PE7_PORT,PE7_PIN,           // GREEN LED output
            GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(PC7_PORT,PC7_PIN,           // BLUE LED output
            GPIO_Mode_Out_PP_Low_Slow);
  GPIO_SetBits(PE7_PORT, PE7_PIN);      // Turn on GREEN LED
  GPIO_ResetBits(PC7_PORT, PC7_PIN);    // Turn off BLUE LED
}

/*******************************************************************************
*  PRIVATE FUNCTION:    TIM_Configuration
*       * Initializes 1 s, 16 bit timer (TIM2)
*       * Sets up timer update interrupt
******************************************************************************/
void    TIM2_Config(void)
{
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM2,ENABLE);// Enable TIM2 clocking
  TIM2_TimeBaseInit(TIM2_Prescaler_1,
                    TIM2_CounterMode_Up,TIME_BASE);     // Initialize time base
  TIM2_ClearITPendingBit(TIM2_IT_Update);               // Clear TIM2 IT flag
  TIM2_ITConfig(TIM2_IT_Update, ENABLE);                // Enable timer ITs
}

/*******************************************************************************
*  PRIVATE FUNCTION:    RTC_Configuration
*******************************************************************************/
void    RTC_Config(void)
{
  CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);// Enable RTC clocking
  CLK_RTCClockConfig(CLK_RTCCLKSource_LSE,
                     CLK_RTCCLKDiv_1);                  // Set LSE as RTC source
  RTC_RatioCmd(ENABLE);                                 // No sync(fclk=frtc)
  RTC_WakeUpCmd(DISABLE);                               // Disable WakeUp unit
  RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16);  // frtc/16 = 2048
  RTC_ITConfig(RTC_IT_WUT, ENABLE);                     // Enable interrupts
  RTC_SetWakeUpCounter(2048);                           // WakeUp counter
}

/*******************************************************************************
**********************************   END   *************************************
*******************************************************************************/