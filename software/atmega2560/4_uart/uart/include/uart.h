#ifndef UART_H_
#define UART_H_

#include "utils.h"

// ������� �������� �����
void UartTransmitByte(uint8_t byte);
// ������� ����� �����
uint8_t UartReceiveByte(void);
// ������� ����������� 16-������� ����� � �� � ��������
void UartSendDec(int16_t val);
// ������� �������� ������, ����� ��� �����
void UartSendStr(uint8_t* string);
// ������� hex ����� � �� � ��������
void UartSendHex(uint16_t val);
// ���� ����������� 16-������� ����� �� �� � ���������
int16_t UartReceiveDec(void);
//���� �������� � �������� ������ �� ����������
float UartReceiveData(void);
/* ������� �������������: ��������� �� �� ���� � �������� ������, 
����� ��������� � 8 ���, ���� ���� ���, ��� ��������, �������� ������ � 9600 ��� */
void UartInit(void);

#endif /* UART_H_ */