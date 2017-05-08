#include "daphne_utilities.h"


// Physiological values (changed at run-time if NFC memory is present)
uint32_t        pulse_width             = 200;          //( 1000 = 1000 us  )
uint32_t        pulse_amplitude         = 500;          //( 1000 = 10.00 mA )
uint32_t        breathing_rate          = 3000;         //( 1000 = 10.00 bpm)
uint32_t        inspiratory_time        = 1300;         //( 1000 = 10.00 %  )
uint32_t        interpulse_interval     = 500;         //( 1000 = 100.0 ms )

// DMA Buffers
uint16_t TI1Buffer[] = {0,0,0,0};       // Pulse timing         (TIM1)
uint16_t TI2Buffer[] = {0,0,0,0};       // Pulse polarity       (TIM2)
uint16_t DACBuffer[] = {0,0,0,0};       // Pulse amplitude      (DAC1)

// Data structure
static uint32_t data[5] = {0,0,0,0,0};  // DAPhNe incoming data structure
static uint32_t prev_data[5] = {0,0,0,0,0};

// NFC variables
uint8_t NDEFmessage[0x40];              // NDEF message for NFC
uint8_t sentMail[] = {'n','o','t','h','i','n','g'};


// CALCULATION THINGS --- NEED TODO
uint16_t        time_in = RESET;        // Inspiratory time
uint16_t        time_ex = RESET;        // Expiratory time
uint16_t        breath_period = RESET;
uint32_t        one_period = RESET;

/*******************************************************************************
*  PRIVATE FUNCTION:    initialize()
*******************************************************************************/
void    initialize(void)
{
  data[0]= pulse_width;
  data[1]= pulse_amplitude;
  data[2]= breathing_rate;
  data[3]= inspiratory_time;
  data[4]= interpulse_interval;
  prev_data[0] = data[0];
  prev_data[1] = data[1];
  prev_data[2] = data[2];
  prev_data[3] = data[3];
  prev_data[4] = data[4];
  update();
  CLK_Config();                                         // Configure clocks
  GPIO_Config();                                        // Configure pin in/out
  TIM1_Config();                                        // Configure TIM1
  TIM2_Config();                                        // Configure TIM2
  TIM4_Config();                                        // Configure TIM4
  DMA1_Config();                                        // Configure DMA
  DAC_Config();                                         // Configure DAC
  RTC_Config();                                         // Configure RTC
  PWR_Config();                                         // Configure PWR
  PWR_UltraLowPowerCmd(ENABLE);                         // Ultra low power mode
}


/*******************************************************************************
*  PRIVATE FUNCTION:    get_new_settings()
*******************************************************************************/
void get_new_settings(void)
{
  //TODO: Error state actions
  if(fetch_data()==SUCCESS)
  {
    if(check_data()==SUCCESS)
    {
      parse_data();
      limit_data();
      update();
    }
  }
}

/*******************************************************************************
*  PRIVATE FUNCTION:    fetch_data()
*******************************************************************************/
ErrorStatus fetch_data(void)
{
  uint8_t PayloadLength;
  return User_ReadNDEFMessage(&PayloadLength);
}

/*******************************************************************************
*  PRIVATE FUNCTION:    check_data()
*******************************************************************************/

ErrorStatus check_data(void)
{
  if (NDEFmessage!=sentMail) // Check if message changed
  {
    for (int i=0;i<24;i++) // Format check (dddd,dddd,dddd,dddd) 
    {
      if (i==4||i==9||i==14||i==19){if(NDEFmessage[i]!=',') return ERROR;}
      else{if(!isdigit(NDEFmessage[i])){return ERROR;}}
    }
  }
  else{return ERROR;}
  return SUCCESS;
}


/*******************************************************************************
*  PRIVATE FUNCTION:    parse_data()
*******************************************************************************/

void parse_data(void)
{
  // Message is dddd,dddd,dddd,dddd,dddd where d is digit
  uint8_t breaks[6] = {0,4,9,14,19,24};
  uint32_t digit = 0;
  for (uint8_t i=0;i<5;i++)     // 5 values
  {
    prev_data[i] = data[i];
    data[i] = 0;
    for (uint8_t j=1;j<5;j++)   // each value is 4 digits long; comma skipped
    {
      digit = (uint32_t)(NDEFmessage[breaks[i+1]-j]-48); //48 for ASCII shift
      for (uint8_t k=j-1;k>0;k--)
      {
        digit = (digit<<3)+(digit<<1); // Bit shifting (3 becomes 30, etc.)
      }
      data[i] = data[i]+digit;
    }
  }
}


/*******************************************************************************
*  PRIVATE FUNCTION:    limit_data()
*******************************************************************************/

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


/*******************************************************************************
*  PRIVATE FUNCTION:    update()
*******************************************************************************/
void update(void)
{
  // Pulse width
  uint16_t pw = data[0]/2;                  // Pulse width value
  
  // Pulse amplitude
  uint16_t DAC_High = data[1]*4096/330/5;
  uint16_t DAC_Low = DAC_High/PULSE_RATIO;      // Charge-balancing pulse ampl.
  
  // Breaths-per-minute
  breath_period = (LSE_FREQ/4*60*100/data[2]); //this must be done in 32bit - RTC
  // Inspiration time
  time_in = (LSE_FREQ/4*data[3]/1000);//also must be done in 32bit
  time_ex = breath_period-time_in;
  // Interpulse interval
  one_period = (data[4]/2)*100;
  
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