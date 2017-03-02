/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%                                                                        %%%%
%%%%          BME 390: Phrenic Nerve Stimulation: Implant Program           %%%%
%%%%                          private_functions.h                           %%%%
%%%%                                                                        %%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%	
********************************************************************************
*	Author:		Alexey Revinski
*	Last Revised:	12/15/2016
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
#define LEDY_PORT       GPIOB
#define LEDY_PIN        GPIO_Pin_0
#define PD5_PORT        GPIOD
#define PD5_PIN         GPIO_Pin_5
#define LEDG_PORT       GPIOB
#define LEDG_PIN        GPIO_Pin_1
#define PB0_PORT        GPIOB
#define PB0_PIN         GPIO_Pin_0
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

#define M24LR04E_I2C                         I2C1
#define M24LR04E_I2C_CLK                     CLK_Peripheral_I2C1
#define M24LR04E_I2C_SCL_PIN                 GPIO_Pin_1
#define M24LR04E_I2C_SCL_GPIO_PORT           GPIOC
#define M24LR04E_I2C_SDA_PIN                 GPIO_Pin_0
#define M24LR04E_I2C_SDA_GPIO_PORT           GPIOC

#define TIM1_ARR_ADDRESS                0x52C3
#define TIM2_OC1_ADDRESS                0x5261
#define DAC_CH1RDHRH_ADDRESS            0x5388
#define TIM1_PERIOD                     25000
#define TIM4_PERIOD                     250
#define TIM1_REPTETION_COUNTER          0
#define TIM1_PRESCALER                  1
#define BUFSIZE                         4

#define MAX_PW          (uint32_t) 1000
#define MIN_PW          (uint32_t) 100
#define MAX_PA          (uint32_t) 500
#define MIN_PA          (uint32_t) 100
#define MAX_BR          (uint32_t) 2500
#define MIN_BR          (uint32_t) 1200
#define MAX_IE          (uint32_t) 7000
#define MIN_IE          (uint32_t) 2000

/*******************************************************************************
*  FUNCTION PROTOTYPES
*******************************************************************************/
void                    initialize(void);
void                    reconfigure(void);
void                    start_Inspiration(void);
void                    start_Expiration(void);
void                    Switch_To_HSI(void);
void                    Switch_To_LSE(void); 
void                    GPIO_Config(void);
void                    TIM1_Config(void);
void                    PWM2_Config(void);
void                    RTC_Config(void);
void                    DAC_Config(void);
void                    calculate(void);
void                    reset_RTC_counter(uint16_t time);
void                    get_Message(void);
void                    parse_Message(void);
int8_t                    write_Back(void);
static int8_t           User_ReadNDEFMessage ( uint8_t *PayloadLength );
static int8_t           User_ReadNDEFMessage ( uint8_t *PayloadLength );
static ErrorStatus      User_CheckNDEFMessage(void);
static ErrorStatus      User_GetPayloadLength(uint8_t *PayloadLength);
static ErrorStatus      User_GetNDEFMessage(uint8_t  PayloadLength,uint8_t *NDEFmessage);
static void             InitializeBuffer (uint8_t *Buffer ,uint8_t NbCar);
static void             DeInitGPIO ( void );
static void             DeInitClock ( void );

void peripherals(FunctionalState NewState);


void configure(void);
void CLK_Config(void);
void    GPIO_Config(void);
void    PWR_Config(void);
void    DMA1_Config(void);      // Configure DMA
void    TIM2_Config(void);      // Configure TIM2
void    TIM4_Config(void);      // Configure TIM4


#endif /* PRIVATE_FUNCTIONS_H */
/*******************************************************************************
**********************************   END   *************************************
*******************************************************************************/