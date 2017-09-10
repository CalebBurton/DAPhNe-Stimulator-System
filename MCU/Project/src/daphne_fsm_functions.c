/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%                                                                        %%%%
%%%%             DAPhNe Stimulator System Firmware: IPNS_v0.1               %%%%
%%%%                        daphne_fsm_functions.c                          %%%%
%%%%                                                                        %%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%	
********************************************************************************
*       Author:		Alexey Revinski
*	Last Revised:	05/16/2017
*******************************************************************************/

// INCLUDES
#include "daphne_fsm_functions.h"

// EXTERNAL VARIABLES
extern uint16_t TI1Buffer[];            // Pulse timing         (TIM1)
extern uint16_t time_in;                // Inspiratory time
extern uint16_t time_ex;                // Expiratory time
extern uint16_t end_timer;

/*==============================================================================
  PRIVATE FUNCTION:    startInspiration()
--------------------------------------------------------------------------------
  - Resets RTC timer to interrupt at end of inspiratory time
  - Enables pulse timing and polarity outputs
  - Puts device in wait-for-interrupt (RTC interrupt) mode
  - Upon wake up, enables TIM2 interrupts to poll pulse cycle completion
==============================================================================*/
void start_Inspiration(void)
{ 
  // Start counting inspiratory time
  while(!RTC_WakeUpCmd(DISABLE)){;}                     // Disable WakeUp unit
  RTC_SetWakeUpCounter(time_in);                        // RTC counter to insp.
  while(!RTC_WakeUpCmd(ENABLE)){;}                      // Enable WakeUp unit;
  RTC_ITConfig(RTC_IT_WUT, ENABLE);
  
  // Enable pulse timing and polarity timers and their outputs
  TIM2_CtrlPWMOutputs(ENABLE);                          // Enable TIM2 output
  TIM2_Cmd(ENABLE);                                     // Enable TIM2
  TIM1_CtrlPWMOutputs(ENABLE);                          // Enable TIM1 output
  TIM1_Cmd(ENABLE);                                     // Enable TIM1
  
  // Go into WAIT FOR INTERRUPT mode
  wfi();                                                // Wait for event mode
  
  // Make sure the last pulse cycle is complete (next wfi in interrupt)
  TIM2_ITConfig(TIM2_IT_Update,ENABLE); // Enable TIM2 update interrupt
}

/*==============================================================================
  PRIVATE FUNCTION:    startExpiration()
--------------------------------------------------------------------------------
  - Resets RTC timer to interrupt at end of expiratory time
  - Disables pulse timing and polarity outputs
  - Checks for new NFC data, implements changes in software
  - Puts device into halt mode (everythings stops except RTC and LSE)
==============================================================================*/
void start_Expiration(void)
{
  // Start counting expiratory time
  while(!RTC_WakeUpCmd(DISABLE)){;}                     // Disable WakeUp unit
  RTC_SetWakeUpCounter(time_ex);                        // RTC counter to insp.
  while(!RTC_WakeUpCmd(ENABLE)){;}                      // Enable WakeUp unit;
  RTC_ITConfig(RTC_IT_WUT, ENABLE);                     // Enable RTC interrupt
  
  // Disable pulse timing and polarity timers and their outputs
  TIM1_CtrlPWMOutputs(DISABLE);                         // Disable TIM1 output
  TIM2_CtrlPWMOutputs(DISABLE);                         // Disable TIM2 output
  
  // Read NFC memory, update variables (will take effect on next breath)
  get_new_settings();
  
  // Go into HALT mode
  halt();                                               // Stop all except RTC
}
