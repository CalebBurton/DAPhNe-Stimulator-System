/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%                                                                        %%%%
%%%%          BME 390: Phrenic Nerve Stimulation: Implant Program           %%%%
%%%%                          private_functions.c                           %%%%
%%%%                                                                        %%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%	
********************************************************************************
*	Author:		Alexey Revinski
*	Last Revised:	04/30/2017
*******************************************************************************/
#include "private_functions.h"

/*******************************************************************************
*  GLOBAL VARIABLES
*******************************************************************************/
extern bool     sleeping;                       // State variable





/*******************************************************************************
*  EXTERNAL VARIABLES
*******************************************************************************/
extern uint32_t pulse_freq;                     // Pulse frequency
extern uint32_t pulse_width;                    // Pulse width
extern uint32_t pulse_amp;                      // Pulse amplitude
extern uint32_t bpm;                            // Breaths per minute
extern uint32_t ie_ratio;                       // Inspiration:Expiration ratio


//





// Global variables for NFC






/*******************************************************************************
*  PRIVATE FUNCTION:    calculate()
*******************************************************************************/
/*
void    calculate(void)
{
  uint32_t xy = ((uint32_t)LSE_FREQ/RTC_Div*minpersec)/ //Total breath
    (uint32_t)bpm;
  time_in = (uint16_t)((ie_ratio*xy)/mod_10k)-700;      // Inspiration
  time_ex = ((uint16_t)xy-time_in)-700;                 // Expiration
  
  CCR1_Val = pulse_width;                               // Pulse width
  DAC_Val = (uint16_t)(((uint32_t)pulse_amp*            // DAC value
                        (uint32_t)res_8bit)/
                        (uint32_t)Vref)/2;
  TIM1_period = ((1000000*mod_100)/                     // fclk/20Hz-1
    pulse_freq)-1;
  
}*/

/*
void update_Buffers (uint32_t pulse_width,uint32_t pulse_amp)
{
  // Derived variables
  uint16_t pw = pulse_width/2;                  // Pulse width value
  uint16_t DAC_High = (uint16_t)(((uint32_t)pulse_amp*            // DAC value
                        (uint32_t)res_12bit)*5/
                        (uint32_t)Vref_12bit)/2;
  uint16_t DAC_Low = DAC_High/PULSE_RATIO;      // Charge-balancing pulse ampl.
  
  // TIM1 buffer (pulse timing)
  TI1Buffer[0] = pw;                            // Stimulation pulse
  TI1Buffer[1] = pw*3;                          // Break between pulses
  TI1Buffer[2] = pw*PULSE_RATIO;                // Charge-balancing pulse
  TI1Buffer[3] = TIM1_PERIOD-pw*(PULSE_RATIO+5);// Rest of the interpulse period
  
  // TIM2 buffer (pulse polarity)
  TI2Buffer[0] = 0xFFFF;                        // Never overflowing
  TI2Buffer[1] = 50;                            // Slight delay after TIM1 fires
  TI2Buffer[2] = 0xFFFF;                        // Never overflowing
  TI2Buffer[3] = 50;                            // Slight delay after TIM1 fires
  
  // DAC1 buffer (pulse amplitude)
  DACBuffer[0] = DAC_High;                      // High during negative pulse
  DACBuffer[1] = DAC_Low;                       // Low during break
  DACBuffer[2] = DAC_Low;                       // Low during positive pulse
  DACBuffer[3] = DAC_High;                      // High during rest of the time;
}
*/
//==============================================================================
//
//                    NEAR FIELD COMMUNICATION FUNCTIONS
//
//==============================================================================

/*******************************************************************************
*  PRIVATE FUNCTION:    get_Message()
*******************************************************************************/
/*
void    get_Message(void)
{
  
  uint8_t PayloadLength;
  if (User_ReadNDEFMessage(&PayloadLength) == SUCCESS)
  {
    //parse_Message();
    //if(write_Back()==SUCCESS);
  }
  else
  {
    ;
  }
}
*/
/*******************************************************************************
*  PRIVATE FUNCTION:    write_Back();
*******************************************************************************/
/*
uint8_t    write_Back(void)
{
  uint8_t data = 0x61;
  uint16_t WriteAddr = 0x000D;				
  M24LR04E_Init();     
  M24LR04E_WriteOneByte (M24LR16_EEPROM_ADDRESS_USER, WriteAddr++, data);			
  I2C_Cmd(M24LR04E_I2C, DISABLE);			      
  CLK_PeripheralClockConfig(CLK_Peripheral_I2C1, DISABLE);	
  GPIO_SetBits(M24LR04E_I2C_SCL_GPIO_PORT,M24LR04E_I2C_SCL_PIN);	
  GPIO_SetBits(M24LR04E_I2C_SCL_GPIO_PORT,M24LR04E_I2C_SDA_PIN);	     
  M24LR04E_DeInit();
  I2C_Cmd(M24LR04E_I2C, DISABLE);
  return SUCCESS;
}
*/
/*******************************************************************************
*  PRIVATE FUNCTION:    parse_Message()
*******************************************************************************/
/*
void    parse_Message(void)
{
  bool ERROR = FALSE;
  // Check if message changed (reader wrote new settings)
  if (NDEFmessage==sentMail){return;}
  // Format check (dddd,dddd,dddd,dddd) 
  for (int i=0;i<19;i++)
  {
    if (i==4||i==9||i==14){if(NDEFmessage[i]!=',')ERROR = TRUE;}
    else{if(!isdigit(NDEFmessage[i])){ERROR = TRUE;}}
  }
  // Scan the message (if format is wrong, skip scanning)
  if (ERROR);
  else
  {
    // Parody on sscanf(); NDEF is of type (dddd,dddd,dddd,dddd where d = digit)
    uint8_t breaks[5] = {0,4,9,14,19};
    uint32_t data[4]={0,0,0,0}, digit = 0;
    for (uint8_t i=0;i<4;i++)
    {
      for (uint8_t j=1;j<5;j++)
      {
        digit = (uint32_t)(NDEFmessage[breaks[i+1]-j]-48);
        for (uint8_t k=j-1;k>0;k--)
        {
          digit = (digit<<3)+(digit<<1);
        }
        data[i] = data[i]+digit;
      }
    }
    
    // Max/min control
    if      (data[0]>MAX_PW){pulse_width = MAX_PW;}
    else if (data[0]<MIN_PW){pulse_width = MIN_PW;}
    else                    {pulse_width = data[0];}
    
    if      (data[1]>MAX_PA){pulse_amp = MAX_PA;}
    else if (data[1]<MIN_PA){pulse_amp = MIN_PA;}
    else                    {pulse_amp = data[1];}
    
    if      (data[2]>MAX_BR){bpm = MAX_BR;}
    else if (data[2]<MIN_BR){bpm = MIN_BR;}
    else                    {bpm = data[2];}
    
    if      (data[3]>MAX_IE){ie_ratio = MAX_IE;}
    else if (data[3]<MIN_IE){ie_ratio = MIN_IE;}
    else                    {ie_ratio = data[3];}
    
    update_Buffers(pulse_width,pulse_amp);
    
  }
}
*/
/*******************************************************************************
**********************************   END   *************************************
*******************************************************************************/