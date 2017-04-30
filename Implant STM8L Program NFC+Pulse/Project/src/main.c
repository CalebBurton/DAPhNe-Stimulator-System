/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%                                                                        %%%%
%%%%             DAPhNe Stimulator System Firmware: IPNS_v0.1               %%%%
%%%%                                 main.c                                 %%%%
%%%%                                                                        %%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%	
********************************************************************************
*       Author:		Alexey Revinski
*	Last Revised:	04/30/2017
*******************************************************************************/
#include "private_functions.h"
#include "daphne_hardware_config.h"

/*******************************************************************************
*  GLOBAL VARIABLES
*******************************************************************************/

// Physiological values (default values assigned)
uint32_t        pulse_width     = 200;          //( 1000 = 1000 us  )
uint32_t        pulse_amp       = 500;          //( 1000 = 10.00 mA )
uint32_t        bpm             = 3000;         //( 1000 = 10.00 bpm)
uint32_t        ie_ratio        = 5000;         //( 1000 = 10.00 %  )
uint32_t        pulse_freq      = 2000;         //( 1000 = 10.00 Hz )

// Global state variable
state_t         daphne          = INHALE;       // Inspiration/Expiration

// DMA Buffers
uint16_t        TI1Buffer[4]    = {0,0,0,0};    // Pulse timing         (TIM1)
uint16_t        TI2Buffer[4]    = {0,0,0,0};    // Pulse polarity       (TIM2)
uint16_t        DACBuffer[4]    = {0,0,0,0};    // Pulse amplitude      (DAC1)

/*******************************************************************************
*  MAIN FUNCTION
*******************************************************************************/

void main(void)
{ 
  initialize();                                 // Startup functions
  while(1)                                      // Simple FSM
  {
    if (daphne==INHALE) {start_Inspiration();}  // Inspiratory state
    else                {start_Expiration(); }  // Expiratory state
  }
}

/*******************************************************************************
**********************************   END   *************************************
*******************************************************************************/