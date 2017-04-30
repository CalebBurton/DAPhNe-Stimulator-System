#ifndef DAPHNE_HARDWARE_CONFIG_H
#define DAPHNE_HARDWARE_CONFIG_H

#include "stm8l15x_conf.h"

// GPIO pin/port nicknames
#define LED_YELLOW_PORT GPIOB
#define LED_YELLOW_PIN  GPIO_Pin_0
#define LED_GREEN_PORT  GPIOB
#define LED_GREEN_PIN   GPIO_Pin_1
#define PULSE_POL_PORT  GPIOB
#define PULSE_POL_PIN   GPIO_Pin_0
#define PULSE_TIM_PORT  GPIOD
#define PULSE_TIM_PIN   GPIO_Pin_2
#define PULSE_AMP_PORT  GPIOF
#define PULSE_AMP_PIN   GPIO_Pin_0

// Hardware configuration values
#define RTC_INIT_TIME 10
#define TIM1_ARR_ADDRESS                0x52C3
#define TIM2_OC1_ADDRESS                0x5261
#define DAC_CH1RDHRH_ADDRESS            0x5388
#define TIM1_PERIOD                     25000
#define TIM1_REPTETION_COUNTER          0
#define TIM1_PRESCALER                  1
#define TIM1_PULSE_INIT                 0
#define TIM2_PERIOD                     0xFFFF
#define TIM2_PULSE_INIT                 0
#define TIM4_PERIOD                     1
#define BUFSIZE                         4

// Function prototypes
void    CLK_Config(void);
void    GPIO_Config(void);
void    TIM1_Config(void);
void    TIM2_Config(void);
void    TIM4_Config(void);
void    DMA1_Config(void);
void    DAC_Config(void);
void    RTC_Config(void);
void    PWR_Config(void);
void    DeInitGPIO(void);
void    DeInitClock(void);

// External variables
extern uint16_t TI1Buffer[];                    // Pulse timing buffer  (TIM1)
extern uint16_t TI2Buffer[];                    // Polarity buffer      (TIM2)
extern uint16_t DACBuffer[];                    // Amplitude buffer     (DAC1)

#endif /* DAPHNE_HARDWARE_CONFIG_H */