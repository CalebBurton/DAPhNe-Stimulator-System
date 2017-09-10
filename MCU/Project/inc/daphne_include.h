/*
  ******************************************************************************
  * @file    daphne_include.h
  * @author  Alexey Revinski
  * @date    09/10/2017
  * @brief   Main include file containing defines, type definitions, and all
  *          header files used in the project. This file is common to all 
  *          source files.
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

#ifndef PLAN_INCLUDE_H
#define PLAN_INCLUDE_H

// INCLUDES  ===================================================================

#include <ctype.h>   
#include "stm8l15x_conf.h"
#include "M24LR04E-R_I2C.h"
#include "M24LR04E-R_User.h"
#include "daphne_fsm_functions.h"
#include "daphne_hardware_config.h"
#include "daphne_utilities.h"

// CONSTANTS  ==================================================================

// Hardware configuration values
#define TIM1_ARR_ADDRESS                0x52C3
#define TIM2_OC1_ADDRESS                0x5261
#define DAC_CH1RDHRH_ADDRESS            0x5388
#define TIM1_PERIOD                     25000
#define TIM1_REPETITION_COUNTER          0
#define TIM1_PRESCALER                  1
#define TIM1_PULSE_INIT                 0
#define TIM2_PERIOD                     0xFFFF
#define TIM2_PULSE_INIT                 0
#define TIM4_PERIOD                     1
#define BUFSIZE                         4
#define PULSE_RATIO                     (uint16_t) 10
#define LSE_FREQ                        ((uint32_t) 32768)

// Breathing Parameters Limiting Values
#define MAX_PW                          (uint32_t) 1000
#define MIN_PW                          (uint32_t) 10
#define MAX_PA                          (uint32_t) 7000
#define MIN_PA                          (uint32_t) 10
#define MAX_BR                          (uint32_t) 3500
#define MIN_BR                          (uint32_t) 1000
#define MAX_IT                          (uint32_t) 2000
#define MIN_IT                          (uint32_t) 800
#define MAX_II                          (uint32_t) 1200
#define MIN_II                          (uint32_t) 100

// NICKNAMES  ==================================================================

#define LED_YELLOW_PORT                 GPIOB
#define LED_YELLOW_PIN                  GPIO_Pin_0
#define LED_GREEN_PORT                  GPIOB
#define LED_GREEN_PIN                   GPIO_Pin_1
#define PULSE_POL_PORT                  GPIOB
#define PULSE_POL_PIN                   GPIO_Pin_0
#define PULSE_TIM_PORT                  GPIOD
#define PULSE_TIM_PIN                   GPIO_Pin_2
#define PULSE_AMP_PORT                  GPIOF
#define PULSE_AMP_PIN                   GPIO_Pin_0

// TYPE DEFINITIONS  ===========================================================

typedef enum {INHALE,EXHALE} state_t;

#endif // PLAN_INCLUDE_H