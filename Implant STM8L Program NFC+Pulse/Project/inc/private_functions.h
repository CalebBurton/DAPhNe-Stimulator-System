/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%                                                                        %%%%
%%%%          BME 390: Phrenic Nerve Stimulation: Implant Program           %%%%
%%%%                          private_functions.h                           %%%%
%%%%                                                                        %%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%	
********************************************************************************
*	Author:		Alexey Revinski
*	Last Revised:	04/30/2016
*******************************************************************************/
#ifndef PRIVATE_FUNCTIONS_H
#define PRIVATE_FUNCTIONS_H

/*******************************************************************************
*  INCLUDES
*******************************************************************************/
#include "daphne_hardware_config.h"
#include "M24LR04E-R_I2C.h"
#include "M24LR04E-R_User.h"
#include <ctype.h>

/*******************************************************************************
*  CONSTANT DEFINITIONS
*******************************************************************************/


#define LSE_FREQ        ((uint32_t) 32768)

#define PULSE_RATIO     (uint16_t) 10

#define MAX_PW          (uint32_t) 1000
#define MIN_PW          (uint32_t) 100
#define MAX_PA          (uint32_t) 500
#define MIN_PA          (uint32_t) 5
#define MAX_BR          (uint32_t) 2500
#define MIN_BR          (uint32_t) 1200
#define MAX_IE          (uint32_t) 7000
#define MIN_IE          (uint32_t) 2000

/*******************************************************************************
*  FUNCTION PROTOTYPES
*******************************************************************************/
void                    initialize(void);
void                    calculate(void);
void                    get_Message(void);
void                    parse_Message(void);
uint8_t                 write_Back(void);

void                    update_Buffers(uint32_t pulse_width,uint32_t pulse_amp);

typedef enum {INHALE,EXHALE} state_t;


#endif /* PRIVATE_FUNCTIONS_H */
/*******************************************************************************
**********************************   END   *************************************
*******************************************************************************/