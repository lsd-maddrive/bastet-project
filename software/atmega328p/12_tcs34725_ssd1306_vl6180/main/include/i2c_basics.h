/* 
for apparatus i2c
freq control
sending/receiving a byte
start/stop conditions
*/

#ifndef I2C_BASICS_H_
#define I2C_BASICS_H_

#include "utils.h"

//чтение/запись по I2C
#define W 0
#define R 1

void i2c_init(void);
void i2c_start(void);
void i2c_stop(void);
void i2c_transmit_byte(uint8_t data);
uint8_t i2c_receive_byte(uint8_t is_last_byte);

#endif /* I2C_BASICS_H_ */