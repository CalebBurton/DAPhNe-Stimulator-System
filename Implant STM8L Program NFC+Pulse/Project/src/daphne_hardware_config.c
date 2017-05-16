/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%                                                                        %%%%
%%%%             DAPhNe Stimulator System Firmware: IPNS_v0.1               %%%%
%%%%                        daphne_hardware_config.c                        %%%%
%%%%                                                                        %%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%	
********************************************************************************
*       Author:		Alexey Revinski
*	Last Revised:	05/16/2017
*******************************************************************************/

// INCLUDES
#include "daphne_hardware_config.h"

// EXTERNAL VARIABLES
extern uint16_t TI1Buffer[];                    // Pulse timing buffer  (TIM1)
extern uint16_t TI2Buffer[];                    // Polarity buffer      (TIM2)
extern uint16_t DACBuffer[];                    // Amplitude buffer     (DAC1)
extern uint16_t time_in;                        // Inspiration time

/*==============================================================================
  PRIVATE FUNCTION:    CLK_Config()
--------------------------------------------------------------------------------
  - Prescales HSE clock (16MHz) by 16 ==>1MHz
  - Clocks TIM1, TIM2, TIM4, DMA, DAC, RTC
  - Sets LSE as RTC clock source
==============================================================================*/
void    CLK_Config(void)
{
  DeInitClock();                                        // Gate all clocks
  CLK_DeInit();                                         // Reset to HSI
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_16);                // 1MHz
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM1,ENABLE);// Enable TIM1 clock
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM2,ENABLE);// Enable TIM2 clock
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM4,ENABLE);// Enable TIM4 clock
  CLK_PeripheralClockConfig(CLK_Peripheral_DMA1,ENABLE);// Enable DMA1 clock
  CLK_PeripheralClockConfig(CLK_Peripheral_DAC, ENABLE);// Enable DAC clock
  CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);// Enable RTC clock
  CLK_RTCClockConfig(CLK_RTCCLKSource_LSE,              // Set LSE as RTC source
                     CLK_RTCCLKDiv_1);
}

/*==============================================================================
  PRIVATE FUNCTION:    GPIO_Config()
--------------------------------------------------------------------------------
  - Configures pin inputs and outputs
==============================================================================*/
void    GPIO_Config(void) // SEE GUIDE
{
  DeInitGPIO();                                         // Deinitialize all pins
  GPIO_Init(PULSE_TIM_PORT,PULSE_TIM_PIN,               // TIM1 Channel 1
            GPIO_Mode_Out_PP_Low_Fast);
  GPIO_Init(PULSE_POL_PORT,PULSE_POL_PIN,               // TIM2 Channel 1
            GPIO_Mode_Out_PP_Low_Fast);                 
  GPIO_Init(PULSE_AMP_PORT,PULSE_AMP_PIN,               // DAC Channel 1
            GPIO_Mode_In_FL_No_IT);
  GPIO_ResetBits(PULSE_TIM_PORT,PULSE_TIM_PIN);         // Low on TIM1 out
  GPIO_ResetBits(PULSE_POL_PORT,PULSE_POL_PIN);         // Low on TIM2 out
}

/*==============================================================================
  PRIVATE FUNCTION:    TIM1_Config()
--------------------------------------------------------------------------------
  - Prescales 1MHz TIM1 source clock by 1; TIM1 counter updates at 500kHz
  - On overflow, TIM1
      - toggles its OC1 output
      - triggers TIM2 and TIM4
  - Default polarity - low
  - Autoreload register is able to be written to at any time
  - DMA updates the autoreload register at the beginning of each pulse phase
==============================================================================*/
void    TIM1_Config(void) // SEE GUIDE
{
  TIM1_DeInit();                                        // Deinitialize TIM1
  TIM1_TimeBaseInit(TIM1_PRESCALER,
                    TIM1_CounterMode_Up,
                    TIM1_PERIOD,
                    TIM1_REPTETION_COUNTER);
  TIM1_OC1Init(TIM1_OCMode_Toggle,                      // Toggles on overflow
               TIM1_OutputState_Enable,
               TIM1_OutputNState_Disable,
               TIM1_PULSE_INIT,
               TIM1_OCPolarity_Low,                     // Default polarity low
               TIM1_OCNPolarity_Low,
               TIM1_OCIdleState_Set,
               TIM1_OCNIdleState_Set);
  TIM1_SelectOutputTrigger(TIM1_TRGOSource_Update);     // Trigger TIM2 and TIM4
  TIM1_ARRPreloadConfig(ENABLE);                        // Enable overflow reg. writes
  TIM1_DMACmd(TIM1_DMASource_Update, ENABLE);           // Enable DMA updates
}

/*==============================================================================
  PRIVATE FUNCTION:    TIM2_Config()
--------------------------------------------------------------------------------
  - Prescales 1MHz TIM2 source clock by 1; TIM2 counter updates at 500kHz
  - When counter reaches value in capture/compare, TIM2 toggles its OC1 output
  - Synchronized with TIM1
  - capture/compare register can be written to at any time
  - TIM1 triggers TIM2 pulse
  - DMA channel updates capture/compare register
==============================================================================*/
void    TIM2_Config(void) // SEE GUIDE
{
  TIM2_DeInit();                                        // Deinitialize TIM2
  TIM2_TimeBaseInit(TIM2_Prescaler_1,                   // Initizlie time base
                    TIM2_CounterMode_Up,
                    TIM2_PERIOD);
  TIM2_OC1Init(TIM2_OCMode_Toggle,                      // Toggles on overflow
               TIM2_OutputState_Enable,
               TIM2_PULSE_INIT,
               TIM2_OCPolarity_Low,                     // Default polarity low
               TIM2_OCIdleState_Set);
  TIM2_SelectSlaveMode(TIM2_SlaveMode_Reset);           // Slave mode - Reset
  TIM2_OC1PreloadConfig(ENABLE);                        // Enable writes to OC1
  TIM2_SelectInputTrigger(TIM2_TRGSelection_TIM1);      // TIM1 triggers TIM2
  TIM2_DMACmd(TIM2_DMASource_Update, ENABLE);           // Enable DMA updates
  
}

/*==============================================================================
  PRIVATE FUNCTION:    TIM4_Config()
--------------------------------------------------------------------------------
  - Prescales 1MHz TIM4 source clock by 1; TIM4 counter updates at 500kHz
  - TIM4 operates by single pulses
  - TIM4 is synchronized with TIM1
  - TIM1 triggers TIM4 count
  - TIM4 overflows right away and triggers DAC conversion
==============================================================================*/
void    TIM4_Config(void) // SEE GUIDE
{
  TIM4_DeInit();        
  TIM4_TimeBaseInit(TIM4_Prescaler_1,TIM4_PERIOD);      // Deinitialize TIM4
  TIM4_SelectOnePulseMode(TIM4_OPMode_Single);          // One pulse
  TIM4_SelectSlaveMode(TIM4_SlaveMode_Reset);           // Slave mode - Reset
  TIM4_SelectInputTrigger(TIM4_TRGSelection_TIM1);      // TIM1 triggers TIM4
  TIM4_SelectOutputTrigger(TIM4_TRGOSource_Update);     // TIM4 triggers DAC
  TIM4_Cmd(ENABLE);                                     // Enable TIM4
}

/*==============================================================================
  PRIVATE FUNCTION:    DMA1_Config()
--------------------------------------------------------------------------------
  - Channel 1 is used for updating TIM2_CCR1 registers - update on TIM2 overflow
  - Channel 2 is used for updating TIM1_ARR registers - update on TIM1 overflow
  - Channel 3 is used for updating DAC_RDHR registers - update on DAC conversion
  - Enables DMA channels
==============================================================================*/
void    DMA1_Config(void) // SEE GUIDE
{
  // Deinitialize DMA
  DMA_GlobalDeInit();
  
  // Channel 1 - TIM2 output compare
  DMA_Init(DMA1_Channel1, (uint16_t)TI2Buffer, TIM2_OC1_ADDRESS,
           BUFSIZE,DMA_DIR_MemoryToPeripheral,DMA_Mode_Circular,
           DMA_MemoryIncMode_Inc,DMA_Priority_High,DMA_MemoryDataSize_HalfWord);
  
  // Channel 2 - TIM1 overflow register value update, circular
  DMA_Init(DMA1_Channel2, (uint16_t)TI1Buffer, TIM1_ARR_ADDRESS,
           BUFSIZE,DMA_DIR_MemoryToPeripheral,DMA_Mode_Circular,
           DMA_MemoryIncMode_Inc,DMA_Priority_High,DMA_MemoryDataSize_HalfWord);
  
  // Channel 3 - DAC output value register update, circular
  DMA_Init(DMA1_Channel3, (uint32_t)DACBuffer, DAC_CH1RDHRH_ADDRESS,
           BUFSIZE,DMA_DIR_MemoryToPeripheral,DMA_Mode_Circular,
           DMA_MemoryIncMode_Inc,DMA_Priority_High,DMA_MemoryDataSize_HalfWord);
  
  // Enable DMA channels
  DMA_Cmd(DMA1_Channel1, ENABLE);
  DMA_Cmd(DMA1_Channel2, ENABLE);
  DMA_Cmd(DMA1_Channel3, ENABLE);
  
  // Enable DMA unit
  DMA_GlobalCmd(ENABLE);
}

/*==============================================================================
  PRIVATE FUNCTION:    DAC_Config()
--------------------------------------------------------------------------------
  - DAC converts on TIM4 trigger
  - On conversion, DMA update is requested
  - Enables DAC conversion
==============================================================================*/
void    DAC_Config(void) // SEE GUIDE
{
  DAC_DeInit();                                 // Deinitialize DAC
  // Initialize DAC to convert on TIM4 overflow
  DAC_Init(DAC_Channel_1,DAC_Trigger_T4_TRGO,           
           DAC_OutputBuffer_Enable);
  DAC_DMACmd(DAC_Channel_1, ENABLE);            // Enable DAC DMA channel 1
  DAC_Cmd(DAC_Channel_1, ENABLE);               // Enable DAC channel 1
}

/*==============================================================================
  PRIVATE FUNCTION:    RTC_Config()
--------------------------------------------------------------------------------
  - RTC uses LSE (32.768kHz), prescaled by 4 (8192 Hz)
  - RTC interrupts are used to transition between insp. and exp. states
==============================================================================*/
void    RTC_Config(void) // SEE GUIDE
{
  RTC_WakeUpCmd(DISABLE);                               // Disable WakeUp unit
  RTC_RatioCmd(ENABLE);                                 // No sync(fclk=frtc)
  RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div4);   // frtc/4 = 8192 Hz
  RTC_ITConfig(RTC_IT_WUT, ENABLE);                     // Enable interrupts
  RTC_SetWakeUpCounter(time_in);                        // Dummy counter value
  RTC_WakeUpCmd(ENABLE);                                // Enable RTC WakeUp
}

/*==============================================================================
  PRIVATE FUNCTION:    PWR_Config()
--------------------------------------------------------------------------------
  - Fast wake ups are disabled (fast - unreliable results)
  - Ultra Low Power mode is enabled
==============================================================================*/
void    PWR_Config(void) // SEE GUIDE
{
  PWR_FastWakeUpCmd(DISABLE);                           // FastWakeUp disable
  PWR_UltraLowPowerCmd(ENABLE);                         // Ultra low power mode
}

/*==============================================================================
  PRIVATE FUNCTION:    DeInitGPIO()
--------------------------------------------------------------------------------
  - All unused general pin inputs/outputs are set to Output Low
==============================================================================*/
static void DeInitGPIO ( void )
{
  GPIO_Init( GPIOA, GPIO_Pin_All, GPIO_Mode_Out_OD_Low_Fast);
  GPIO_Init( GPIOB, GPIO_Pin_All, GPIO_Mode_Out_OD_Low_Fast);
  GPIO_Init( GPIOC, GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6, GPIO_Mode_Out_OD_Low_Fast);
  GPIO_Init( GPIOD, GPIO_Pin_All, GPIO_Mode_Out_OD_Low_Fast);
  GPIO_Init( GPIOE, GPIO_Pin_All, GPIO_Mode_Out_OD_Low_Fast);
  GPIOA->ODR = 0xFF;
  GPIOB->ODR = 0xFF;
  GPIOC->ODR = 0xFF;
  GPIOD->ODR = 0xFF;
  GPIOE->ODR = 0xFF;
}

/*==============================================================================
  PRIVATE FUNCTION:    DeInitClock()
--------------------------------------------------------------------------------
  - Gates all peripheral clocks to reduce power consumption
==============================================================================*/
static void DeInitClock ( void )
{
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM2, DISABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM3, DISABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, DISABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_I2C1, DISABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_SPI1, DISABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_USART1, DISABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_BEEP, DISABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_DAC, DISABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, DISABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM1, DISABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_RTC, DISABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_LCD, DISABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, DISABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_DMA1, DISABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, DISABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_BOOTROM, DISABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_AES, DISABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, DISABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM5, DISABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_SPI2, DISABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_USART2, DISABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_USART3, DISABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_CSSLSE, DISABLE);
}