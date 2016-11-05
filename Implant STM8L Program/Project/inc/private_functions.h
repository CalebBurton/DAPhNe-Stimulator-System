/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%                                                                        %%%%
%%%%          BME 390: Phrenic Nerve Stimulation: Implant Program           %%%%
%%%%                          private_functions.h                           %%%%
%%%%                                                                        %%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%	
********************************************************************************
*	Author:		Alexey Revinski
*	Last Revised:	__/__/2016
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM8L15x_IT_H
#define __STM8L15x_IT_H

/* Includes ------------------------------------------------------------------*/
#include "stm8l15x.h"
#include "stm8l15x_adc.h"
#include "stm8l15x_clk.h"
#include "stm8l15x_exti.h"
#include "stm8l15x_gpio.h"
#include "stm8l15x_tim2.h"
#include "stm8l15x_it.h"
#include "stm8l15x_rtc.h"

/*******************************************************************************
*  FUNCTION PROTOTYPES
*******************************************************************************/

void    initialize(void);
void    start_Inspiration(void);
void    start_Expiration(void);
void    Switch_To_HSI(void);
void    Switch_To_LSI(void); 
void    GPIO_Config(void);
void    TIM2_Config(void);
void    PWM2_Config(void);
void    RTC_Config(void);


#endif /* __STM8L15x_IT_H */

/*******************************************************************************
**********************************   END   *************************************
*******************************************************************************/