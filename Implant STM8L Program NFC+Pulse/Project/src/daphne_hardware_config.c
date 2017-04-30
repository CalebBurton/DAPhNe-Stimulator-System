#include "daphne_hardware_config.h"

extern uint16_t TI1Buffer[];                    // Pulse timing buffer  (TIM1)
extern uint16_t TI2Buffer[];                    // Polarity buffer      (TIM2)
extern uint16_t DACBuffer[];                    // Amplitude buffer     (DAC1)

/*******************************************************************************
*  PRIVATE FUNCTION:    CLK_Config()
*******************************************************************************/
void    CLK_Config(void)
{
  DeInitClock();
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
/*******************************************************************************
*  PRIVATE FUNCTION:    GPIO_Config()
*******************************************************************************/
void    GPIO_Config(void)
{
  DeInitGPIO();                                         // Deinitialize all pins
  GPIO_Init(PULSE_TIM_PORT,PULSE_TIM_PIN,               // TIM1 Channel 1
            GPIO_Mode_Out_PP_Low_Fast);
  GPIO_Init(PULSE_POL_PORT,PULSE_POL_PIN,               // TIM2 Channel 1
            GPIO_Mode_Out_PP_Low_Fast);                 
  GPIO_Init(PULSE_AMP_PORT,PULSE_AMP_PIN,               // DAC Channel 1
            GPIO_Mode_In_FL_No_IT);
  GPIO_ResetBits(PULSE_TIM_PORT,PULSE_TIM_PIN);
  GPIO_ResetBits(PULSE_POL_PORT,PULSE_POL_PIN);
}
/*******************************************************************************
*  PRIVATE FUNCTION:    TIM1_Config()
*******************************************************************************/
void    TIM1_Config(void)
{
  // 1MHz/(1+1) = 500kHz, TIM1_Period can be:
  //    25000 for 50ms
  //    30000 for 60ms
  //    40000 for 80ms
  //    50000 for 100ms
  //    60000 for 120ms
  TIM1_DeInit();
  TIM1_TimeBaseInit(TIM1_PRESCALER,
                    TIM1_CounterMode_Up,
                    TIM1_PERIOD,
                    TIM1_REPTETION_COUNTER);
  TIM1_OC1Init(TIM1_OCMode_Toggle,
               TIM1_OutputState_Enable,
               TIM1_OutputNState_Disable,
               TIM1_PULSE_INIT,
               TIM1_OCPolarity_Low,
               TIM1_OCNPolarity_Low,
               TIM1_OCIdleState_Set,
               TIM1_OCNIdleState_Set);
  TIM1_SelectOutputTrigger(TIM1_TRGOSource_Update);
  TIM1_ARRPreloadConfig(ENABLE);
  TIM1_DMACmd(TIM1_DMASource_Update, ENABLE);
}
/*******************************************************************************
*  PRIVATE FUNCTION:    TIM2_Config()
*******************************************************************************/
void    TIM2_Config(void)
{
  TIM2_DeInit();
  TIM2_TimeBaseInit(TIM2_Prescaler_1,
                    TIM2_CounterMode_Up,
                    TIM2_PERIOD);
  TIM2_OC1Init(TIM2_OCMode_Toggle,
               TIM2_OutputState_Enable,
               TIM2_PULSE_INIT,
               TIM2_OCPolarity_Low,
               TIM2_OCIdleState_Set);
  TIM2_SelectSlaveMode(TIM2_SlaveMode_Reset);
  TIM2_OC1PreloadConfig(ENABLE);
  TIM2_SelectInputTrigger(TIM2_TRGSelection_TIM1);
  TIM2_DMACmd(TIM2_DMASource_Update, ENABLE);
}
/*******************************************************************************
*  PRIVATE FUNCTION:    TIM4_Config()
*******************************************************************************/
void    TIM4_Config(void)
{
  TIM4_DeInit();
  TIM4_TimeBaseInit(TIM4_Prescaler_1,TIM4_PERIOD);
  TIM4_SelectOnePulseMode(TIM4_OPMode_Single);
  TIM4_SelectSlaveMode(TIM4_SlaveMode_Reset);
  TIM4_SelectInputTrigger(TIM4_TRGSelection_TIM1);
  TIM4_SelectOutputTrigger(TIM4_TRGOSource_Update);
  TIM4_Cmd(ENABLE);
}
/*******************************************************************************
*  PRIVATE FUNCTION:    DMA1_Config()
*******************************************************************************/
void    DMA1_Config(void)
{
  DMA_GlobalDeInit();
  DMA_Init(DMA1_Channel1, (uint16_t)TI2Buffer, TIM2_OC1_ADDRESS,
           BUFSIZE,DMA_DIR_MemoryToPeripheral,DMA_Mode_Circular,
           DMA_MemoryIncMode_Inc,DMA_Priority_High,DMA_MemoryDataSize_HalfWord);
  
  DMA_Init(DMA1_Channel2, (uint16_t)TI1Buffer, TIM1_ARR_ADDRESS,
           BUFSIZE,DMA_DIR_MemoryToPeripheral,DMA_Mode_Circular,
           DMA_MemoryIncMode_Inc,DMA_Priority_High,DMA_MemoryDataSize_HalfWord);
  
  DMA_Init(DMA1_Channel3, (uint32_t)DACBuffer, DAC_CH1RDHRH_ADDRESS,
           BUFSIZE,DMA_DIR_MemoryToPeripheral,DMA_Mode_Circular,
           DMA_MemoryIncMode_Inc,DMA_Priority_High,DMA_MemoryDataSize_HalfWord);
  DMA_Cmd(DMA1_Channel1, ENABLE);
  DMA_Cmd(DMA1_Channel2, ENABLE);
  DMA_Cmd(DMA1_Channel3, ENABLE);
  DMA_GlobalCmd(ENABLE);
}
/*******************************************************************************
*  PRIVATE FUNCTION:    DAC_Config()
*******************************************************************************/
void    DAC_Config(void)
{
  DAC_DeInit();
  DAC_Init(DAC_Channel_1,DAC_Trigger_T4_TRGO,
           DAC_OutputBuffer_Enable);
  DAC_DMACmd(DAC_Channel_1, ENABLE);
  DAC_Cmd(DAC_Channel_1, ENABLE);
}
/*******************************************************************************
*  PRIVATE FUNCTION:    RTC_Config()
*******************************************************************************/
void    RTC_Config(void)
{
  RTC_WakeUpCmd(DISABLE);                               // Disable WakeUp unit
  RTC_RatioCmd(ENABLE);                                 // No sync(fclk=frtc)
  RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div2);   // frtc/2 = 16384
  RTC_ITConfig(RTC_IT_WUT, ENABLE);                     // Enable interrupts
  RTC_SetWakeUpCounter(RTC_INIT_TIME);                  // Dummy counter value
  RTC_WakeUpCmd(ENABLE);                                // Enable RTC WakeUp
}
/*******************************************************************************
*  PRIVATE FUNCTION:    PWR_Config()
*******************************************************************************/
void    PWR_Config(void)
{
  PWR_FastWakeUpCmd(DISABLE);
}

/*******************************************************************************
*  PRIVATE FUNCTION:    DeInitGPIO()
*******************************************************************************/
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
/*******************************************************************************
*  PRIVATE FUNCTION:    DeInitClock()
*******************************************************************************/
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