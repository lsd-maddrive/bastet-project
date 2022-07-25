/*
 * spi.h
 *
 * Created: 14.03.2022 18:43:44
 *  Author: Michael
 */ 


#ifndef SPI_H_
#define SPI_H_

#include "utils.h"

#define SS 2
#define MOSI 3
#define MISO 4
#define SCK 5

void SpiMasterInit(void);
void SpiMasterTransmit(uint8_t byte);
uint8_t SpiMasterReceive(void); 

#endif /* SPI_H_ */