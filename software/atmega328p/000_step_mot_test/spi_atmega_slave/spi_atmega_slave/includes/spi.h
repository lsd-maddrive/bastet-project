/*
 * spi.h
 *
 * Created: 14.03.2022 19:02:37
 *  Author: Michael
 */ 


#ifndef SPI_H_
#define SPI_H_

#include "utils.h"

#define SS 2
#define MOSI 3
#define MISO 4
#define SCK 5

void spi_slave_init(void);
void spi_slave_transmit(uint8_t byte);
uint8_t spi_slave_receive(void);

#endif /* SPI_H_ */