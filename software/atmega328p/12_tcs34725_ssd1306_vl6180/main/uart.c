#include "uart.h"

// ������� �������� �����
void uart_transmit_byte(uint8_t byte){
	/* ������� ����������� ���� UDRE0, � ����� ������ ������� � UDR0 */
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = byte;
}

// ������� ����� �����
uint8_t uart_receive_byte(void){
	/* ������� ����������� ���� RXC0, � ����� �������� ������� �� UDR0 */
	while(!(UCSR0A & (1<<RXC0)));
	return(UDR0);
}

// ������� ����������� 16-������� ����� � �� � ��������
void uart_send_dec(uint16_t val){
	uint8_t i, j;
	// ���� ���������� �� ����� �� ����
	if(val!=0){
		/* ��������� ���������� ������� �������� � ������������� �����, 
		�������� � ����� 64 ����� ��� ������� ���������� ������� � 00064 */
		j=1;
		while(!(Digit(val, j++)));
		// �� ������� ���������� ���� � ���������
		for(i=j-1; i<=5; i++) uart_transmit_byte(Digit(val, i)+'0');
	}
	/* ���� �������� ���� � ������� ����, ����� �������� 
	� ����� */
	else uart_transmit_byte('0');
}

// ������� �������� ������, ����� ��� �����
void uart_send_str(uint8_t* string){
	/* ������� ������ ��������� � ������� ��������� *
	��������� � ����� �������� �������� ������������ ������*/
	while(*string) uart_transmit_byte(*string++);
}

// ������� hex ����� � �� � ��������
void uart_send_hex(uint16_t val){
	uint8_t msb = (val & 0xF0) >> 4, lsb = val & 0x0F;
	msb += msb > 9 ? 'A' - 10 : '0';
	lsb += lsb > 9 ? 'A' - 10 : '0';	
	uart_transmit_byte(msb);
	uart_transmit_byte(lsb);
}

// ���� ����������� 16-������� ����� �� �� � ���������
uint16_t uart_receive_dec(void){
	uint16_t data = 0;
	uint8_t digit = 0;
	// ��������� ������������� �����
	do{
		digit = uart_receive_byte();
		// ���������, ���� �� ������ ������� ����� ������
		if(digit == '\r') continue;
		if(digit == '\n') break;
		if(!((digit >= '0') && (digit <= '9'))) break;
		// ������� ASCII ���� ����
		digit -= '0';
		data *= 10;
		data += digit;
	}
	while(1);
	// �����������, ����� ������� ������� \r\n
	return(data);
}

/* ������� �������������: ��������� �� �� ���� � �������� ������, 
����� ��������� � 8 ���, ���� ���� ���, ��� ��������, �������� ������ � 9600 ��� */
void uart_init(void){
	uint8_t word[] = {"OK"};
	UCSR0B |= (1<<RXEN0) | (1<<TXEN0);
	UCSR0C |= (1<<UCSZ01) | (1<<UCSZ00);
	//�������� �������� 9600 ���
	UBRR0H = 0;
	//��� �������� 9600 UBRR0L = 103 = 0x67
	UBRR0L = 0x67;
	// ������� � �������� ����� TEST
	uart_send_str(word);
	// ��������� � ������ ��������� ������
	uart_transmit_byte('\r');
}