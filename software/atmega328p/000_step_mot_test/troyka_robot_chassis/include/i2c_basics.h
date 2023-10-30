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

#define SCL_FREQ 100 //kHz

#define VACANT 0
#define OCCUPIED 1

void I2cInit(void);
void I2cStart(void);
void I2cStop(void);
void I2cTransmitByte(uint8_t data);
uint8_t I2cReceiveByte(uint8_t is_last_byte);

void I2cAcquire(void);
void I2cRelease(void);

#endif /* I2C_BASICS_H_ */