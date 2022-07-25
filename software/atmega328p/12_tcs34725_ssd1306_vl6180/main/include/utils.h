#ifndef _UTILS_H
#define _UTILS_H

#include <avr/io.h>
#define F_CPU 16000000
//#include <util/delay.h>
#include <avr/interrupt.h>
#include <math.h>
#include <avr/pgmspace.h>
#include <stdlib.h>

uint8_t Digit(uint16_t d, uint8_t m);
float find_max_out_of_3(float a, float b, float c);
float find_min_out_of_3(float a, float b, float c);
void tim1_delay_ms(uint16_t millisec);

#endif