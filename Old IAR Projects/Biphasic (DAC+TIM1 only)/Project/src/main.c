/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%                                                                        %%%%
%%%%        BME 390: Phrenic Nerve Stimulation: Biphasic Stimulator         %%%%
%%%%                                 main.c                                 %%%%
%%%%                                                                        %%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%	
********************************************************************************
*	Author:		Alexey Revinski
*	Last Revised:	02/13/2017
*******************************************************************************/
#include "private_functions.h"


uint16_t SrcBuffer[4] = {0,0,0,0};      //TIM1 time bases buffer
uint16_t DAC_Vals[4] = {0,0,0,0};       //DAC1 values buffer
uint16_t pulse_width = 200;             //us
uint16_t DAC_High = 1450;               // about 1V
uint16_t DAC_Low = 145;                 // about 0.1V

void main(void)
{
  uint16_t pw = pulse_width/2;
  uint16_t mult = 100;
  SrcBuffer[3] = pw;                            //100           (200us) 3000us
  SrcBuffer[0] = pw*5;                          //500           (1000us) 12ms
  SrcBuffer[1] = pw*mult;                       //10000         (20ms)  30ms
  SrcBuffer[2] = TIM1_PERIOD-pw*(mult+5);       //14400         (28.8ms)
  DAC_Vals[2] = DAC_High;
  DAC_Vals[3] = DAC_High;
  DAC_Vals[0] = DAC_Low;
  DAC_Vals[1] = DAC_Low;
  
  initialize();
  
  while(1){wfi();}
}
/*******************************************************************************
**********************************   END   *************************************
*******************************************************************************/