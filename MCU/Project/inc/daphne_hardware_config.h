/*
  ******************************************************************************
  * @file    daphne_hardware_config.h
  * @author  Alexey Revinski
  * @date    09/10/2017
  * @brief   STM8L152 hardware configuration header file
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

#ifndef DAPHNE_HARDWARE_CONFIG_H
#define DAPHNE_HARDWARE_CONFIG_H

// INCLUDES  ===================================================================

#include "daphne_include.h"

// FUNCTION PROTOTYPES  ========================================================

void            CLK_Config(void);
void            GPIO_Config(void);
void            TIM1_Config(void);
void            TIM2_Config(void);
void            TIM4_Config(void);
void            DMA1_Config(void);
void            DAC_Config(void);
void            RTC_Config(void);
void            PWR_Config(void);
void            DeInitGPIO(void);
void            DeInitClock(void);

#endif // DAPHNE_HARDWARE_CONFIG_H