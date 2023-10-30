/*
 * utils.h
 *
 * Created: 14.03.2022 19:02:27
 *  Author: Michael
 */ 


#ifndef UTILS_H_
#define UTILS_H_

#include <avr/io.h>
#define F_CPU 16000000
#include <util/delay.h>

uint8_t Digit(uint16_t d, uint8_t m);

#endif /* UTILS_H_ */