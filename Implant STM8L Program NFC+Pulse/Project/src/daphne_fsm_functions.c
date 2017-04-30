#include "daphne_fsm_functions.h"

/*******************************************************************************
*  PRIVATE FUNCTION:    startInspiration()
*******************************************************************************/
void start_Inspiration(void)
{ 
  // Start counting inspiratory time
  while(!RTC_WakeUpCmd(DISABLE)){;}                     // Disable WakeUp unit
  RTC_SetWakeUpCounter(time_in);                        // RTC counter to insp.
  while(!RTC_WakeUpCmd(ENABLE)){;}                      // Enable WakeUp unit;
  RTC_ITConfig(RTC_IT_WUT, ENABLE);
  
  // Enable pulse timing and polarity timers and their outputs
  TIM1_SetCounter(TI1Buffer[3]-100);                    // Starting point
  TIM2_CtrlPWMOutputs(ENABLE);                          // Enable TIM2 output
  TIM2_Cmd(ENABLE);                                     // Enable TIM2
  TIM1_CtrlPWMOutputs(ENABLE);                          // Enable TIM1 output
  TIM1_Cmd(ENABLE);                                     // Enable TIM1
  
  // Go into WAIT FOR INTERRUPT mode
  wfi();                                                // Wait for event mode
}
/*******************************************************************************
*  PRIVATE FUNCTION:    startExpiration()
*******************************************************************************/
void start_Expiration(void)
{
  // Starts counting expiratory time
  while(!RTC_WakeUpCmd(DISABLE)){;}                     // Disable WakeUp unit
  RTC_SetWakeUpCounter(time_ex);                        // RTC counter to insp.
  while(!RTC_WakeUpCmd(ENABLE)){;}                      // Enable WakeUp unit;
  RTC_ITConfig(RTC_IT_WUT, ENABLE);                     // Enable RTC interrupt
  
  // Disable pulse timing and polarity timers and their outputs
  TIM2_CtrlPWMOutputs(DISABLE);                         // Disable TIM2 output
  TIM2_Cmd(DISABLE);                                    // Disable TIM2
  TIM1_CtrlPWMOutputs(DISABLE);                         // Disable TIM1 output
  TIM1_Cmd(DISABLE);                                    // Disable TIM1
  
  // Read NFC memory, update variables (will take effect on next breath)
  get_Message();
  
  // Go into HALT mode
  halt();                                               // Stop all except RTC
}
