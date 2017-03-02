/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%                                                                        %%%%
%%%%        BME 390: Phrenic Nerve Stimulation: Biphasic Stimulator         %%%%
%%%%                           private_functions.c                          %%%%
%%%%                                                                        %%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%	
********************************************************************************
*	Author:		Alexey Revinski
*	Last Revised:	02/14/2017
*******************************************************************************/
#include "private_functions.h"

extern uint16_t TI1Buffer[];   //TIM1 time bases buffer
extern uint16_t TI2Buffer[];   //TIM2 polarity buffer
extern uint16_t DACBuffer[];   //DAC1 values buffer

/*******************************************************************************
*  PRIVATE FUNCTION:    initialize()
*******************************************************************************/
void    initialize(void)
{
  CLK_DeInit();                                         // Reset to HSI
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_16);                // 1MHz
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM1, ENABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM2, ENABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, ENABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_DMA1, ENABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_DAC, ENABLE);// Route clock to DAC
  GPIO_Init(PD2_PORT,PD2_PIN,GPIO_Mode_Out_PP_Low_Fast);// TIM1 Channel 1
  GPIO_Init(PB0_PORT,PB0_PIN,GPIO_Mode_Out_PP_Low_Fast);// TIM2 Channel 1
  GPIO_Init(PF0_PORT,PF0_PIN,GPIO_Mode_In_FL_No_IT);    // DAC Channel 1
  GPIO_Init(PC7_PORT,PC7_PIN,GPIO_Mode_Out_PP_Low_Fast);// LED BLUE
  GPIO_Init(PE7_PORT,PE7_PIN,GPIO_Mode_Out_PP_Low_Fast);// LED GREEN
  GPIO_SetBits(PE7_PORT,PE7_PIN);
  DMA_Config();
  TIM_Config();
  DAC_Config();
}

static void DMA_Config(void)
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

static void TIM_Config(void)
{
  // 1MHz/(1+1) = 500kHz, TIM1_Period can be:
  //    25000 for 50ms
  //    30000 for 60ms
  //    40000 for 80ms
  //    50000 for 100ms
  //    60000 for 120ms
  TIM1_TimeBaseInit(TIM1_PRESCALER,
                    TIM1_CounterMode_Up,
                    TIM1_PERIOD,
                    TIM1_REPTETION_COUNTER);
  TIM1_OC1Init(TIM1_OCMode_Toggle,
               TIM1_OutputState_Enable,
               TIM1_OutputNState_Disable,
               0,
               TIM1_OCPolarity_High,
               TIM1_OCNPolarity_Low,
               TIM1_OCIdleState_Set,
               TIM1_OCNIdleState_Set);
  TIM1_SelectOutputTrigger(TIM1_TRGOSource_Update);
  TIM1_ARRPreloadConfig(ENABLE);
  TIM1_DMACmd(TIM1_DMASource_Update, ENABLE);
  TIM1_CtrlPWMOutputs(ENABLE);
  TIM1_Cmd(ENABLE);
  
  TIM2_DeInit();
  TIM2_TimeBaseInit(TIM2_Prescaler_1,
                    TIM2_CounterMode_Up,
                    0xFFFF);
  TIM2_OC1Init(TIM2_OCMode_Toggle,
               TIM2_OutputState_Enable,
               0,
               TIM2_OCPolarity_High,
               TIM2_OCIdleState_Set);
  TIM2_SelectSlaveMode(TIM2_SlaveMode_Reset);
  TIM2_OC1PreloadConfig(ENABLE);
  TIM2_SelectInputTrigger(TIM2_TRGSelection_TIM1);
  TIM2_DMACmd(TIM2_DMASource_Update, ENABLE);
  TIM2_CtrlPWMOutputs(ENABLE);
  TIM2_Cmd(ENABLE);
  
  
  TIM4_DeInit();
  TIM4_TimeBaseInit(TIM4_Prescaler_1, 1);
  TIM4_SelectOnePulseMode(TIM4_OPMode_Single);
  TIM4_SelectSlaveMode(TIM4_SlaveMode_Reset);
  TIM4_SelectInputTrigger(TIM4_TRGSelection_TIM1);
  TIM4_SelectOutputTrigger(TIM4_TRGOSource_Update);
  TIM4_Cmd(ENABLE);
}

void    DAC_Config(void)
{
  DAC_DeInit();
  DAC_Init(DAC_Channel_1,DAC_Trigger_T4_TRGO,
           DAC_OutputBuffer_Enable);
  DAC_DMACmd(DAC_Channel_1, ENABLE);
  DAC_Cmd(DAC_Channel_1, ENABLE);
}
