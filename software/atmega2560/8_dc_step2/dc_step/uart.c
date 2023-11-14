#include "include/uart.h"

float data_received=0, angle_received=0;
uint8_t digit_received=0, negative_flag=0, complete_flag=0;

// ������� �������� �����
void UartTransmitByte(uint8_t byte){
	/* ������� ����������� ���� UDRE0, � ����� ������ ������� � UDR0 */
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0=byte;
}

// ������� ����� �����
uint8_t UartReceiveByte(void){
	/* ������� ����������� ���� RXC0, � ����� �������� ������� �� UDR0 */
	while(!(UCSR0A & (1<<RXC0)));
	return(UDR0);
}

// ������� ����������� 16-������� ����� � �� � ��������
void UartSendDec(int16_t val){
	uint8_t i, j;
	// ���� ���������� �� ����� �� ����
	if(val!=0){
		if(val<0){
			val=-val;
			UartTransmitByte('-');
		}
		/* ��������� ���������� ������� �������� � ������������� �����, 
		�������� � ����� 64 ����� ��� ������� ���������� ������� � 00064 */
		j=1;
		while(!(Digit(val, j++)));
		// �� ������� ���������� ���� � ���������
		for(i=j-1; i<=5; i++) UartTransmitByte(Digit(val, i)+'0');
	}
	/* ���� �������� ���� � ������� ����, ����� �������� 
	� ����� */
	else UartTransmitByte('0');
}

// ������� �������� ������, ����� ��� �����
void UartSendStr(uint8_t* string){
	/* ������� ������ ��������� � ������� ��������� *
	��������� � ����� �������� �������� ������������ ������*/
	while(*string) UartTransmitByte(*string++);
}

// ������� hex ����� � �� � ��������
void UartSendHex(uint16_t val){
	uint8_t msb=(val & 0xF0) >> 4, lsb=val & 0x0F;
	msb += msb > 9 ? 'A' - 10 : '0';
	lsb += lsb > 9 ? 'A' - 10 : '0';	
	UartTransmitByte(msb);
	UartTransmitByte(lsb);
}

// ���� ����������� 16-������� ����� �� �� � ���������
int16_t UartReceiveDec(void){
	int16_t data=0;
	uint8_t digit=0, negative_flag=0;
	// ��������� ������������� �����
	do{
		digit=UartReceiveByte();  
		// ���������, ���� �� ������ ������� ����� ������
		if(digit=='\r') continue;
		if(digit=='\n') break;
		//if(!((digit>='0') && (digit<='9'))) break;
		// ������� ASCII ���� ����
		if(digit=='-') negative_flag=1;
		else{
			digit-='0';
			data*=10;
			data+=digit;
		}
	}
	while(1);
	if(negative_flag==1) data=-data;
	// �����������, ����� ������� ������� \r\n
	return(data);
}

#define INTERRUPTS
/* ������� �������������: ��������� �� �� ���� � �������� ������, 
����� ��������� � 8 ���, ���� ���� ���, ��� ��������, �������� ������ � 19200 ��� */
void UartInit(void){
	uint8_t word[]={"UART_OK"};
	#ifdef INTERRUPTS
		UCSR0B|=(1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0);
	#endif
	#ifdef NO_INTERRUPTS
		UCSR0B|=(1<<RXEN0)|(1<<TXEN0);	
	#endif
	UCSR0C|=(1<<UCSZ01)|(1<<UCSZ00);
	//�������� �������� 19200 ���
	UBRR0H=0;
	//��� �������� UBRR0L = 51 = 0x33	UBRR0L=0x33;
	UBRR0L=0x33;
	// ������� � �������� ����� TEST_OK
	UartSendStr(word);
	// ��������� � ������ ��������� ������
	UartTransmitByte('\r');
}

//����� �������� ����������, ����� � UCSR0B � UartInit() ��������� ��� RXCIE0
float UartReceiveData(void){
	return(angle_received);
}

ISR(USART0_RX_vect){
	digit_received=UDR0;
	if(digit_received=='\r') complete_flag=0;
	if(digit_received=='\n') complete_flag=1;
	if(digit_received=='-') negative_flag=1;
	if((digit_received>='0') && (digit_received<='9')){
		digit_received-='0';
		data_received*=10;
		data_received+=digit_received;
	}
	if(complete_flag==1){
		if(negative_flag==1) data_received=-data_received;
		angle_received=data_received;
		complete_flag=0;
		negative_flag=0;
		data_received=0;
	}
}

