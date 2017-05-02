#include "daphne_fsm_functions.h"

extern uint16_t TI1Buffer[];            // Pulse timing         (TIM1)
extern uint16_t time_in;                // Inspiratory time
extern uint16_t time_ex;                // Expiratory time
extern uint16_t end_timer;
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
  TIM2_CtrlPWMOutputs(ENABLE);                          // Enable TIM2 output
  TIM2_Cmd(ENABLE);                                     // Enable TIM2
  TIM1_CtrlPWMOutputs(ENABLE);                          // Enable TIM1 output
  TIM1_Cmd(ENABLE);                                     // Enable TIM1
  // Go into WAIT FOR INTERRUPT mode
  wfi();                                                // Wait for event mode
  TIM2_ITConfig(TIM2_IT_Update,ENABLE); // Enable TIM2 update interrupt
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
  TIM1_CtrlPWMOutputs(DISABLE);                         // Disable TIM1 output
  TIM2_CtrlPWMOutputs(DISABLE);                         // Disable TIM2 output
  
  // Read NFC memory, update variables (will take effect on next breath)
  get_new_settings();
  
  // Go into HALT mode
  halt();                                               // Stop all except RTC
}
