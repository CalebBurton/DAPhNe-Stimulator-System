/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%                                                                        %%%%
%%%%             DAPhNe Stimulator System Firmware: IPNS_v0.1               %%%%
%%%%                                 main.c                                 %%%%
%%%%                                                                        %%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%	
********************************************************************************
*       Author:		Alexey Revinski
*	Last Revised:	05/16/2017
*******************************************************************************/
#include "daphne_utilities.h"
#include "daphne_fsm_functions.h"
#include "daphne_hardware_config.h"

// GLOBAL VARIABLES
state_t daphne  = INHALE;                       // Inspiration/Expiration

// MAIN
void main(void)
{ 
  initialize();                                 // Startup functions
  while(1)                                      // Simple FSM
  {
    if (daphne==INHALE) {start_Inspiration();}  // Inspiratory state
    else                {start_Expiration(); }  // Expiratory state
  }
}