/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%                                                                        %%%%
%%%%           BME 390: Phrenic Nerve Stimulation: Implant Program          %%%%
%%%%                                 main.c                                 %%%%
%%%%                                                                        %%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%	
********************************************************************************
*	Author:		Alexey Revinski
*	Last Revised:	__/__/2016
*******************************************************************************/

#ifndef __STM8L15x_CONF_H
#define __STM8L15x_CONF_H

/*******************************************************************************
*  INCLUDES
*******************************************************************************/

#include "stm8l15x.h"
#include "private_functions.h"

// BATTERY LEVEL CONSTANTS
#define ADC_INIT        ((uint16_t) 5000)       // Initial sensor ADC value
#define ADC12BIT        ((uint16_t) 4095)       // 2^12-1 = 4095
#define BATT_CORRECTION ((uint32_t) 215)        // Experimental constant (ADC_unit)
#define VREF            ((uint16_t) 2870)       // Voltage supplied by the board

// TIMING CONSTANTS
#define CYCLE_END       ((uint16_t) 1000)       // ms, end of second-long cycle
#define LED_START       ((uint16_t) 1)          // ms, when to light up LEDs
#define LED_STOP        ((uint16_t) 20)         // ms, when to turn off LEDs
#define S_MS            ((uint16_t) 1000)       // Seconds-to-milliseconds factor

/*******************************************************************************
*  MACROS
*******************************************************************************/
// CHANGING EQUATIONS BASED ON RESISTOR ADDED
#define P_OFF_NEW (RESISTOR_ADDED?\
                    ((uint32_t)(B0-B1*(N/(N/R_DEFAULT+N/RESISTOR_ADDED)))/M):\
                    P_OFF_DEFAULT)
/*
#define ERROR ((P_HIGH>P_LOW||P_OFF_DEFAULT>P_OFF_NEW||P_LOW>P_OFF_NEW||\
                P_OFF_DEFAULT!=21900||BATTERY_LEVEL>=VREF||RESISTOR_ADDED<0||\
                TIME_TRY<=0||TIME_FAIL<=0||TIME_LEAK<=0||P_LOW==0||\
                PRESSURE_UNITS<1||PRESSURE_UNITS>4||P_HIGH<500)\
                ?TRUE:FALSE)
*/


/*******************************************************************************
*  ST LIBRARY CODE
*******************************************************************************/

/* Uncomment the line below to expanse the "assert_param" macro in the
   Standard Peripheral Library drivers code */
/* #define USE_FULL_ASSERT    (1) */
#ifdef  USE_FULL_ASSERT

/**
  * @brief  The assert_param macro is used for function's parameters check.
  * @param expr: If expr is false, it calls assert_failed function
  *   which reports the name of the source file and the source
  *   line number of the call that failed.
  *   If expr is true, it returns no value.
  * @retval : None
  */
#define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))

void assert_failed(uint8_t* file, uint32_t line);

#else
#define assert_param(expr) ((void)0)
#endif /* USE_FULL_ASSERT */

#endif /* __STM8L15x_CONF_H */
