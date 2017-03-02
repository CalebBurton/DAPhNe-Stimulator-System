/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%                                                                        %%%%
%%%%        BME 390: Phrenic Nerve Stimulation: Biphasic Stimulator         %%%%
%%%%                                 main.c                                 %%%%
%%%%                                                                        %%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%	
********************************************************************************
*	Author:		Alexey Revinski
*	Last Revised:	02/14/2017
*******************************************************************************/
#include "private_functions.h"

uint16_t TI1Buffer[4] = {0,0,0,0};      //TIM1 time bases buffer
uint16_t TI2Buffer[4] = {0,0,0,0};      //TIM2 polarity buffer
uint16_t DACBuffer[4] = {0,0,0,0};      //DAC1 values buffer

uint16_t mult = 10;
uint16_t pulse_width = 200;             //us
uint16_t DAC_High = 790;                // about 3mA
uint16_t DAC_Low = 0;

void main(void)
{
  uint16_t pw = pulse_width/2;
  TI1Buffer[0] = pw*3;                          //500           (1000us) 12ms
  TI1Buffer[1] = pw*mult;                       //10000         (20ms)  30ms
  TI1Buffer[2] = TIM1_PERIOD-pw*(mult+5);       //14400         (28.8ms)
  TI1Buffer[3] = pw;                            //100           (200us) 3000us
  
  uint16_t DAC_Low = DAC_High/mult;       // about 0.3mA
  
  TI2Buffer[0] = 50;
  TI2Buffer[1] = 0xFFFF;
  TI2Buffer[2] = 50;
  TI2Buffer[3] = 0xFFFF;
  
  DACBuffer[0] = DAC_Low;
  DACBuffer[1] = DAC_Low;
  DACBuffer[2] = DAC_High;
  DACBuffer[3] = DAC_High;
  
  
  initialize();
  
  while(1)
  {
    wfi();
  }
}
/*******************************************************************************
**********************************   END   *************************************
*******************************************************************************/