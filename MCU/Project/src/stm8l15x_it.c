/*
  ******************************************************************************
  * @file    stm8l15x_it.c
  * @author  Alexey Revinski
  * @date    09/10/2017
  * @brief   Interrupt handler file based on stm8l15x_it.c provided with the
  *          standard peripheral library by STMicroelectronics
  ******************************************************************************
  * @copy
  *
  * DAPHNE STIMULATOR SYSTEM FIRMWARE IS COPYRIGHTED AND IS OWNED BY
  * NORTHWESTERN UNIVERSITY, AN ILLINOIS NOT-FOR-PROFIT CORPORATION, HAVING A
  * PLACE OF BUSINESS AT 633 CLARK STREET, EVANSTON, ILLINOIS  60208. IT CAN BE 
  * FREELY USED FOR EDUCATIONAL AND RESEARCH PURPOSES BY NON-PROFIT INSTITUTIONS
  * AND US GOVERNMENT AGENCIES ONLY. OTHER ORGANIZATIONS ARE ALLOWED TO USE 
  * DAPHNE STIMULATOR SYSTEM FIRMWARE ONLY FOR EVALUATION PURPOSES, AND ANY 
  * FURTHER USES WILL REQUIRE PRIOR APPROVAL. THE SOFTWARE MAY NOT BE SOLD OR 
  * REDISTRIBUTED WITHOUT PRIOR APPROVAL. ONE MAY MAKE COPIES OF THE SOFTWARE 
  * FOR THEIR USE PROVIDED THAT THE COPIES, ARE NOT SOLD OR DISTRIBUTED, ARE 
  * USED UNDER THE SAME TERMS AND CONDITIONS.
  * 
  * AS UNESTABLISHED RESEARCH SOFTWARE, THIS CODE IS PROVIDED ON AN "AS IS" 
  * BASIS WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED. THE
  * DOWNLOADING, OR EXECUTING ANY PART OF THIS SOFTWARE CONSTITUTES AN IMPLICIT
  * AGREEMENT TO THESE TERMS. THESE TERMS AND CONDITIONS ARE SUBJECT TO CHANGE 
  * AT ANY TIME WITHOUT PRIOR NOTICE.
  *
  ******************************************************************************
  *                 COPYRIGHT 2017 NORTHWESTERN UNIVERSITY
  *****************************************************************************/

// INCLUDES  ===================================================================

#include "daphne_utilities.h"

// VARIABLES  ==================================================================

uint16_t end_timer = 0;         // Count of phases left to compete pulse cycle

// EXTERNAL VARIABLES  =========================================================

extern state_t daphne;          // External state variable
extern uint16_t time_in;        // External counter variable (inspiratory time)
extern uint16_t time_ex;        // External counter variable (expiratory time)
extern uint16_t TI1Buffer[];    // External buffer (TIM1 overflow values)

/*==============================================================================
INTERRUPT HANDLER:      RTC
--------------------------------------------------------------------------------
  - Triggers on RTC interrupts
  - Used to transition between IM/EM and EM/IM
==============================================================================*/
INTERRUPT_HANDLER(RTC_IRQHandler, 4)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
  disableInterrupts();                  // Mask all interrupts
  static uint16_t timer_val;            // TIM1 overflow value holder
  RTC_ITConfig(RTC_IT_WUT, DISABLE);    // Disable RTC wakeup unit
  RTC_ClearITPendingBit(RTC_IT_WUT);    // Clear RTC interrupt flag
  if (daphne==INHALE)                   // Transition to expiration
  {    
    timer_val = (TIM1->ARRL);           // Read TIM1 value     
    timer_val |= (TIM1->ARRH)<<8;
    
    // Check which phase we are in
    if      (timer_val==TI1Buffer[0]){end_timer = 4;} // Allow 4 more phases
    else if (timer_val==TI1Buffer[1]){end_timer = 3;} // Allow 3 more phases
    else if (timer_val==TI1Buffer[2]){end_timer = 2;} // Allow 2 more phases
    else if (timer_val==TI1Buffer[3]){end_timer = 1;} // Allow 1 more phases
    
    daphne = EXHALE;                    // Change state to expiratory
  }
  else
  {
    daphne = INHALE;                    // Change state to inspiratory
  }
  enableInterrupts();
}

/*==============================================================================
INTERRUPT HANDLER:      TIM2 Update
--------------------------------------------------------------------------------
  - Triggers on TIM2 update interrupts
  - Used to count down pulse phases to end IM safely
==============================================================================*/
INTERRUPT_HANDLER(TIM2_UPD_OVF_TRG_BRK_IRQHandler, 19)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
  TIM2_ClearITPendingBit(TIM2_IT_Update);       // Clear TIM2 interrupt flag
  if(end_timer>0)                               // If still coasting
  {
    end_timer--;                                // Reduce count of phases left
    wfi();                                      // Wait until phase ends
  }
  else                                          // If final phase done
  {
    TIM2_ITConfig(TIM2_IT_Update, DISABLE);     // Disable TIM2 interrupts
    TIM2_Cmd(DISABLE);                          // Disable TIM2
    TIM1_Cmd(DISABLE);                          // Disable TIM1
  }
}
