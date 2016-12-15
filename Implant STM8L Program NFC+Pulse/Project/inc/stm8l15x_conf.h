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
