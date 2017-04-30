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
#include "stm8l15x.h"
#include "stm8l15x_adc.h"
#include "stm8l15x_clk.h"
#include "stm8l15x_exti.h"
#include "stm8l15x_gpio.h"
#include "stm8l15x_tim1.h"
#include "stm8l15x_tim2.h"
#include "stm8l15x_tim4.h"
#include "stm8l15x_dac.h"
#include "stm8l15x_dma.h"
#include "stm8l15x_it.h"
#include "stm8l15x_rtc.h"
#include "stm8l15x_pwr.h"
#include "stm8l15x_i2c.h"
#include "stm8l15x_iwdg.h"
#include "I2C_M24LR04E-R.h"
#include "stm8l15x_flash.h"
#include <ctype.h>

/*******************************************************************************
*  CONSTANT DEFINITIONS
*******************************************************************************/


#define LSE_FREQ        ((uint32_t) 32768)

#define M24LR04E_I2C                         I2C1
#define M24LR04E_I2C_CLK                     CLK_Peripheral_I2C1
#define M24LR04E_I2C_SCL_PIN                 GPIO_Pin_1
#define M24LR04E_I2C_SCL_GPIO_PORT           GPIOC
#define M24LR04E_I2C_SDA_PIN                 GPIO_Pin_0
#define M24LR04E_I2C_SDA_GPIO_PORT           GPIOC


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
void                    reconfigure(void);
void                    configure(void);
void                    start_Inspiration(void);
void                    start_Expiration(void);


// NFC stuff to be dealt with later
void                    calculate(void);
void                    reset_RTC_counter(uint16_t time);
void                    get_Message(void);
void                    parse_Message(void);
uint8_t                 write_Back(void);
static int8_t           User_ReadNDEFMessage ( uint8_t *PayloadLength );
static int8_t           User_ReadNDEFMessage ( uint8_t *PayloadLength );
static ErrorStatus      User_CheckNDEFMessage(void);
static ErrorStatus      User_GetPayloadLength(uint8_t *PayloadLength);
static ErrorStatus      User_GetNDEFMessage(uint8_t  PayloadLength,uint8_t *NDEFmessage);
static void             InitializeBuffer (uint8_t *Buffer ,uint8_t NbCar);

void Fill_Buffers(uint32_t pulse_width,uint32_t pulse_amp);

typedef enum {INHALE,EXHALE} state_t;


#endif /* PRIVATE_FUNCTIONS_H */
/*******************************************************************************
**********************************   END   *************************************
*******************************************************************************/