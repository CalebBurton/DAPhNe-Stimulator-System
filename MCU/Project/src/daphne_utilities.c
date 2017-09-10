/*
  ******************************************************************************
  * @file    daphne_utilities.c
  * @author  Alexey Revinski
  * @date    09/10/2017
  * @brief   Utility and data functions source code file
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

//INCLUDES  ====================================================================

#include "daphne_utilities.h"

// GLOBAL VARIABLES  ===========================================================

// Physiological values (changed at run-time if NFC memory is present)
uint32_t        pulse_width             = 150;  //1000 = 1000 us
uint32_t        pulse_amplitude         = 50;   //1000 = 10.00 mA
uint32_t        breathing_rate          = 3000; //1000 = 10.00 bpm
uint32_t        inspiratory_time        = 1200; //1000 = 10.00 %
uint32_t        interpulse_interval     = 500;  //1000 = 100.0 ms

// DMA Buffers
uint16_t TI1Buffer[] = {0,0,0,0};       // Pulse timing phase values (TIM1)
uint16_t TI2Buffer[] = {0,0,0,0};       // Pulse polarity phase values (TIM2)
uint16_t DACBuffer[] = {0,0,0,0};       // Pulse amplitude phase values (DAC1)

// Data structure
static uint32_t data[5] = {0,0,0,0,0};  // DAPhNe incoming data
//static uint32_t prev_data[5] = {0,0,0,0,0}; // Save buffer

// NFC variables
uint8_t NDEFmessage[0x40];              // NDEF message for NFC
uint8_t sentMail[] = {'n','o','t','h','i','n','g'}; //!!!!! TODO

// Calculation variables --- NEED TODO
uint16_t        time_in = RESET;        // Inspiratory time
uint16_t        time_ex = RESET;        // Expiratory time
uint16_t        breath_period = RESET;  // One breath period
uint32_t        one_period = RESET;     // Interpulse period

/*==============================================================================
  FUNCTION:    initialize
--------------------------------------------------------------------------------
  - Initializes software data structures
  - Configures hardware
==============================================================================*/
void    initialize(void)
{
  // Initial values
  data[0]= pulse_width;
  data[1]= pulse_amplitude;
  data[2]= breathing_rate;
  data[3]= inspiratory_time;
  data[4]= interpulse_interval;
  //prev_data[0] = data[0];
  //prev_data[1] = data[1];
  //prev_data[2] = data[2];
  //prev_data[3] = data[3];
  //prev_data[4] = data[4];
  
  // Implement software buffers for DMA updates
  update();
  
  // Hardware cofigurations
  CLK_Config();                         // Configure clocks
  GPIO_Config();                        // Configure pin in/out
  TIM1_Config();                        // Configure TIM1
  TIM2_Config();                        // Configure TIM2
  TIM4_Config();                        // Configure TIM4
  DMA1_Config();                        // Configure DMA
  DAC_Config();                         // Configure DAC
  RTC_Config();                         // Configure RTC
  PWR_Config();                         // Configure PWR
}

/*==============================================================================
  FUNCTION:    get_new_settings
--------------------------------------------------------------------------------
  - Fetches new data
  - If successful, checks data for errors
  - If no errors detected, parses data for individual values
  - Clamps values to physiological max and min
  - Updates software buffers used to control hardware output
==============================================================================*/
void get_new_settings(void)
{
  //TODO: Error state actions
  if(fetch_data()==SUCCESS)             // If successfully read data
  {
    if(check_data()==SUCCESS)           // If data is properly formatted
    {
      parse_data();                     // Translate data into ints
      limit_data();                     // Limit values to physiological limits
      update();                         // Update software buffers with new data
    }
  }
}

/*==============================================================================
  FUNCTION:    fetch_data
--------------------------------------------------------------------------------
  - Uses M24LR04 library function to read data from NFC memory
==============================================================================*/
ErrorStatus fetch_data(void)
{
  uint8_t PayloadLength;
  return User_ReadNDEFMessage(&PayloadLength);  // M24LR04 read function
}

/*==============================================================================
  FUNCTION:    check_data
--------------------------------------------------------------------------------
  - Checks if character array has commas in correct places
  - Checks if other characters are digits
  - Data format should be 'dddd,dddd,dddd,dddd,dddd', where d = digit
==============================================================================*/
ErrorStatus check_data(void)
{
  if (NDEFmessage!=sentMail)    // NOT IMPLEMENTED, PASS-THROUGH
  {
    for (int i=0;i<24;i++)      // Format check (dddd,dddd,dddd,dddd,dddd) 
    {
      // Check for proper comma placement
      if (i==4||i==9||i==14||i==19){if(NDEFmessage[i]!=',') return ERROR;}
      // Check if the rest of characters are digits
      else{if(!isdigit(NDEFmessage[i])){return ERROR;}}
    }
  }
  else{return ERROR;}
  return SUCCESS;
}


/*==============================================================================
  FUNCTION:    parse_data
--------------------------------------------------------------------------------
  - Delimits data by commas
  - Bit shifts ASCII characters to obtain actual integer values
==============================================================================*/

void parse_data(void)
{
  // Message is dddd,dddd,dddd,dddd,dddd where d is digit
  uint8_t breaks[6] = {0,4,9,14,19,24}; // data bounds
  uint32_t digit = 0;                   // value holder
  for (uint8_t i=0;i<5;i++)             // 5 values
  {
    //prev_data[i] = data[i];             // save data into prev_data
    data[i] = 0;                        // clear data value
    // convert to ints; each value is 4 digits long; comma skipped (j=1)
    for (uint8_t j=1;j<5;j++)   
    {
      digit = (uint32_t)(NDEFmessage[breaks[i+1]-j]-48); //48 for ASCII shift
      // decimal bit shifting (3 at place x000 becomes 3000)
      for (uint8_t k=j-1;k>0;k--){digit = (digit<<3)+(digit<<1);}
      // add bit-shifted value to data ('..,1234,..' becomes 1000+200+30+4)
      data[i] = data[i]+digit;
    }
  }
}


/*==============================================================================
  FUNCTION:    limit_data
--------------------------------------------------------------------------------
  - Clamps obtained pulse control values to physiologically feasible values
==============================================================================*/

void limit_data(void)
{
  // Pulse width clamp
  if      (data[0]>MAX_PW){pulse_width = MAX_PW;}
  else if (data[0]<MIN_PW){pulse_width = MIN_PW;}
  else                    {pulse_width = data[0];}

  // Pulse amplitude clamp
  if      (data[1]>MAX_PA){pulse_amplitude = MAX_PA;}
  else if (data[1]<MIN_PA){pulse_amplitude = MIN_PA;}
  else                    {pulse_amplitude = data[1];}

  // Breaths-per-minute clamp
  if      (data[2]>MAX_BR){breathing_rate = MAX_BR;}
  else if (data[2]<MIN_BR){breathing_rate = MIN_BR;}
  else                    {breathing_rate = data[2];}

  // Inspiration time clamp
  if      (data[3]>MAX_IT){inspiratory_time = MAX_IT;}
  else if (data[3]<MIN_IT){inspiratory_time = MIN_IT;}
  else                    {inspiratory_time = data[3];}
  
  // TODO Interpulse interval clamp
  if      (data[4]>MAX_II){interpulse_interval = MAX_II;}
  else if (data[4]<MIN_II){interpulse_interval = MIN_II;}
  else                    {interpulse_interval = data[4];}
}


/*==============================================================================
  FUNCTION:    update
--------------------------------------------------------------------------------
  - Translates decimal physiological values into values used by hardware
  - Updates DMA memory buffers
==============================================================================*/
void update(void)
{
  // Pulse width
  uint16_t pw = data[0]/2;                      // Pulse width value
  
  // Pulse amplitude
  uint16_t DAC_High = data[1]*4096/330/5;       // Stim. amplitude, SEE GUIDE
  uint16_t DAC_Low = DAC_High/PULSE_RATIO;      // Charge-balancing pulse ampl.
  
  // Breaths-per-minute
  breath_period = (LSE_FREQ/4*60*100/data[2]);  // Breath period, SEE GUIDE
  
  // Inspiration time
  time_in = (LSE_FREQ/4*data[3]/1000);          // Inspiration time, SEE GUIDE
  time_ex = breath_period-time_in;              // Expiration time, SEE GUIDE
  
  // Interpulse interval
  one_period = (data[4]/2)*100;                 // Interpul. interval, SEE GUIDE
  
  // TIM1 buffer (pulse timing)
  TI1Buffer[0] = pw;                            // Stimulation pulse
  TI1Buffer[1] = pw*3;                          // Break between pulses
  TI1Buffer[2] = pw*PULSE_RATIO;                // Charge-balancing pulse
  TI1Buffer[3] = one_period-pw*(PULSE_RATIO+4); // Rest of the interpulse period
  
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
