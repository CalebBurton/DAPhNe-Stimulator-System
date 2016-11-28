/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%                                                                        %%%%
%%%%          BME 390: Phrenic Nerve Stimulation: Implant Program           %%%%
%%%%                          private_functions.h                           %%%%
%%%%                                                                        %%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%	
********************************************************************************
*	Author:		Alexey Revinski
*	Last Revised:	11/20/2016
*******************************************************************************/
#ifndef PRIVATE_FUNCTIONS_H
#define PRIVATE_FUNCTIONS_H

/*******************************************************************************
*  INCLUDES
*******************************************************************************/
#include "stm8l15x.h"
#include "stm8l15x_adc.h"
#include "stm8l15x_clk.h"
#include "stm8l15x_exti.h"
#include "stm8l15x_gpio.h"
#include "stm8l15x_tim1.h"
#include "stm8l15x_tim2.h"
#include "stm8l15x_dac.h"
#include "stm8l15x_it.h"
#include "stm8l15x_rtc.h"
#include "stm8l15x_pwr.h"
#include "stm8l15x_i2c.h"
#include "I2C_M24LR04E-R.h"
#include "stm8l15x_flash.h"

/*******************************************************************************
*  CONSTANT DEFINITIONS
*******************************************************************************/
#define PE7_PORT        GPIOE
#define PE7_PIN         GPIO_Pin_7
#define PC7_PORT        GPIOC
#define PC7_PIN         GPIO_Pin_7
#define PD2_PORT        GPIOD
#define PD2_PIN         GPIO_Pin_2
#define PD4_PORT        GPIOD
#define PD4_PIN         GPIO_Pin_4
#define PD5_PORT        GPIOD
#define PD5_PIN         GPIO_Pin_5
#define PF0_PORT        GPIOF
#define PF0_PIN         GPIO_Pin_0
#define TIM1_PSR        ((uint16_t) 1)
#define TIM1_REP        ((uint16_t) 0)
#define LSE_FREQ        ((uint32_t) 32768)

/*******************************************************************************
*  FUNCTION PROTOTYPES
*******************************************************************************/
void    initialize(void);
void    start_Inspiration(void);
void    start_Expiration(void);
void    Switch_To_HSI(void);
void    Switch_To_LSE(void); 
void    GPIO_Config(void);
void    TIM1_Config(void);
void    PWM2_Config(void);
void    RTC_Config(void);
void    DAC_Config(void);
void    calculations(void);
void    reset_RTC_counter(uint16_t time);


#endif /* PRIVATE_FUNCTIONS_H */
/*******************************************************************************
**********************************   END   *************************************
*******************************************************************************/