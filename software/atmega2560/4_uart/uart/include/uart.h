#ifndef UART_H_
#define UART_H_

#include "utils.h"

// функция передачи байта
void UartTransmitByte(uint8_t byte);
// функция приёма байта
uint8_t UartReceiveByte(void);
// посылка десятичного 16-битного числа с МК в терминал
void UartSendDec(int16_t val);
// функция передачи строки, слова или фразы
void UartSendStr(uint8_t* string);
// посылка hex числа с МК в терминал
void UartSendHex(uint16_t val);
// приём десятичного 16-битного числа на МК с терминала
int16_t UartReceiveDec(void);
//приём значения с верхнего уровня по прерыванию
float UartReceiveData(void);
/* функция инициализации: настройка МК на приём и передачу данных, 
длина сообщения – 8 бит, один СТОП бит, без паритета, скорость обмена – 9600 бод */
void UartInit(void);

#endif /* UART_H_ */