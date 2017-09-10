#ifndef DAPHNE_UTILITIES_H
#define DAPHNE_UTILITIES_H

#include "stm8l15x_conf.h"
#include "M24LR04E-R_User.h"
#include "daphne_hardware_config.h"
#include <ctype.h>

#define MAX_PW  (uint32_t) 1000
#define MIN_PW  (uint32_t) 10
#define MAX_PA  (uint32_t) 7000
#define MIN_PA  (uint32_t) 10
#define MAX_BR  (uint32_t) 3500
#define MIN_BR  (uint32_t) 1000
#define MAX_IT  (uint32_t) 2000                 //!!!!!
#define MIN_IT  (uint32_t) 800                  //!!!!!
#define MAX_II  (uint32_t) 1200
#define MIN_II  (uint32_t) 100

#define PULSE_RATIO     (uint16_t) 10

#define LSE_FREQ        ((uint32_t) 32768)

typedef enum {INHALE,EXHALE} state_t;

void            get_new_settings(void);
ErrorStatus     fetch_data(void);
ErrorStatus     check_data(void);
void            parse_data(void);
void            limit_data(void);
void            update();

#endif