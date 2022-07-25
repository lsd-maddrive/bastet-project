#ifndef _UART_H
#define _UART_H

#include "utils.h"

void uart_transmit_byte(uint8_t byte);
uint8_t uart_receive_byte(void);
void uart_send_dec(uint16_t val);
void uart_send_hex(uint16_t val);
void uart_send_str(uint8_t* string);
uint16_t uart_get_dec(void);
void uart_init(void);

#endif