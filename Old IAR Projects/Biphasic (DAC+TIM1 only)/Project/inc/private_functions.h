/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%                                                                        %%%%
%%%%        BME 390: Phrenic Nerve Stimulation: Biphasic Stimulator         %%%%
%%%%                           private_functions.h                          %%%%
%%%%                                                                        %%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%	
********************************************************************************
*	Author:		Alexey Revinski
*	Last Revised:	02/13/2017
*******************************************************************************/
#ifndef PRIVATE_FUNCTIONS_H
#define PRIVATE_FUNCTIONS_H

/*******************************************************************************
*  INCLUDES
*******************************************************************************/
#include "stm8l15x.h"
#include "stm8l15x_tim1.h"
#include "stm8l15x_clk.h"
#include "stm8l15x_gpio.h"
#include "stm8l15x_dma.h"
#include "stm8l15x_dac.h"
#include "stm8l15x_tim4.h"
#include "stm8l15x_tim2.h"
#include "stm8l15x_it.h"

/*******************************************************************************
*  CONSTANT DEFINITIONS
*******************************************************************************/
#define PE7_PORT        GPIOE
#define PE7_PIN         GPIO_Pin_7
#define PC7_PORT        GPIOC
#define PC7_PIN         GPIO_Pin_7
#define PD2_PORT        GPIOD
#define PD2_PIN         GPIO_Pin_2
#define PB0_PORT        GPIOB
#define PB0_PIN         GPIO_Pin_0
#define PD4_PORT        GPIOD
#define PD4_PIN         GPIO_Pin_4
#define PD5_PORT        GPIOD
#define PD5_PIN         GPIO_Pin_5
#define PF0_PORT        GPIOF
#define PF0_PIN         GPIO_Pin_0
#define TIM1_PSR        ((uint16_t) 1)
#define TIM1_REP        ((uint16_t) 0)
#define LSE_FREQ        ((uint32_t) 32768)


#define TIM1_ARR_ADDRESS        0x52C3
#define TIM2_OC1_ADDRESS        0x5261
#define DAC_CH1RDHRH_ADDRESS    0x5388
#define TIM1_PERIOD              25000
#define TIM4_PERIOD                250
#define TIM1_REPTETION_COUNTER       0
#define TIM1_PRESCALER               1
#define TIM_BUFSIZE                  4
#define DAC_BUFSIZE                  4

/*******************************************************************************
*  FUNCTION PROTOTYPES
*******************************************************************************/
void                    initialize(void);
static void             DMA_Config(void);
static void             TIM_Config(void);
void                    DAC_Config(void);

#endif /* PRIVATE_FUNCTIONS_H */
/*******************************************************************************
**********************************   END   *************************************
*******************************************************************************/