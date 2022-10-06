/*
 * adc.h
 *
 * Created: 16.03.2022 16:31:52
 *  Author: Michael
 */ 


#ifndef ADC_H_
#define ADC_H_

#include "utils.h"
//#include "gpio_expander.h"

#define ST_MOT1_POT 1
#define ST_MOT2_POT 2
#define ST_MOT3_POT 3
#define ST_MOT4_POT 4

#define AVR_READ // ARM_READ or AVR_READ
#define LOW_PASS_FILTER //either MIDDLE value or MAX+MIN values/2 or -> LOW_PASS_FILTER <- rules

void AdcInit(void);
uint16_t* AdcGetPos(void);

#endif /* ADC_H_ */