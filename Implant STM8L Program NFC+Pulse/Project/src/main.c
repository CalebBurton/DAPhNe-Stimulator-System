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
*  PHYSIOLOGICAL VALUES
*******************************************************************************/
uint32_t        pulse_width     = 200;          //( 1000 = 1000 us  )
uint32_t        pulse_amp       = 500;          //( 1000 = 10.00 mA )
uint32_t        bpm             = 3000;         //( 1000 = 10.00 bpm)
uint32_t        ie_ratio        = 5000;         //( 1000 = 10.00 %  )
uint32_t        pulse_freq      = 2000;         //( 1000 = 10.00 Hz )
/******************************************************************************/
bool            sleeping        = FALSE;        // State variable
/******************************************************************************/
uint16_t TI1Buffer[4] = {0,0,0,0};              // TIM1 time bases buffer
uint16_t TI2Buffer[4] = {0,0,0,0};              // TIM2 polarity buffer
uint16_t DACBuffer[4] = {0,0,0,0};              // DAC1 values buffer
/******************************************************************************/
uint16_t mult = 10;                             // Neg:Pos pratio (amp & time)
uint16_t DAC_High = 820;                        // about 3mA
uint16_t DAC_Low = 0;                           // Charge-balancing ampl. var.

/*******************************************************************************
*  MAIN FUNCTION
*******************************************************************************/
void main(void)
{
  // Derived variables
  uint16_t pw = pulse_width/2;                  // Pulse width value
  DAC_Low = DAC_High/mult;                      // Charge-balancing pulse ampl.
  
  // TIM1 buffer (pulse timing)
  TI1Buffer[0] = pw;                            // Stimulation pulse
  TI1Buffer[1] = pw*3;                          // Break between pulses
  TI1Buffer[2] = pw*mult;                       // Charge-balancing pulse
  TI1Buffer[3] = TIM1_PERIOD-pw*(mult+5);       // Rest of the interpulse period
  
  // TIM2 buffer (pulse polarity)
  TI2Buffer[1] = 50;                            // Slight delay after TIM1 fires
  TI2Buffer[2] = 0xFFFF;                        // Never overflowing
  TI2Buffer[3] = 50;                            // Slight delay after TIM1 fires
  TI2Buffer[0] = 0xFFFF;                        // Never overflowing
  
  // DAC1 buffer (pulse amplitude)
  DACBuffer[0] = DAC_High;                      // High during negative pulse
  DACBuffer[1] = DAC_Low;                       // Low during break
  DACBuffer[2] = DAC_Low;                       // Low during positive pulse
  DACBuffer[3] = DAC_High;                      // High during rest of the time
  
  initialize();                                 // Hardware configurations
  
  while(1)                                      // Infinite two-state FSM
  {
    if (sleeping)       {start_Expiration(); }  // Expiratory state
    else                {start_Inspiration();}  // Inspiratory state
  }
}
/*******************************************************************************
**********************************   END   *************************************
*******************************************************************************/