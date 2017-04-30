#ifndef DAPHNE_FSM_FUNCTIONS_H
#define DAPHNE_FSM_FUNCTIONS_H

#include "stm8l15x_conf.h"
#include "private_functions.h"

void    start_Inspiration(void);        // Runs at the beginning of inspiration
void    start_Expiration(void);         // Runs at the beginning of expiration

extern uint16_t TI1Buffer[];            // Pulse timing         (TIM1)
extern uint16_t time_in;                // Inspiratory time
extern uint16_t time_ex;                // Expiratory time

#endif