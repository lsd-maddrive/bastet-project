/*
 * main.c
 *
 * Created: 24.01.2022 18:38:44
 * Author : Michael
 */ 

#include <avr/io.h>
/* ����� �� ��������� ��� ����������� ������� ������������ */
#define F_CPU 16000000
#include <util/delay.h>
#include <avr/interrupt.h>

uint16_t dist;

/* ������� �������������: ��������� �� �� ���� � �������� ������, ����� ��������� � 8 ���, ���� ���� ���, ��� ��������, �������� ������ � 9600 ��� */
void uart_init(void){
  UCSR0B |= (1<<RXEN0) | (1<<TXEN0);
  UCSR0C |= (1<<UCSZ01) | (1<<UCSZ00);
  //�������� �������� 9600 ���
  UBRR0H = 0;
  //��� �������� 9600 UBRR0L = 103 = 0x67
  UBRR0L = 0x67;
}

// ������� �������� �����
void uart_transmit(uint8_t byte){
  /* ������� ����������� ���� UDRE0, � ����� ������
  ������� � UDR0 */
  while(!(UCSR0A & (1<<UDRE0)));
  UDR0 = byte;
}

// ������� ����� �����
uint8_t uart_receive(void){
  /* ������� ����������� ���� RXC0, � ����� ��������
  ������� �� UDR0 */
  while(!(UCSR0A & (1<<RXC0)));
  return UDR0;
}

// ������� �������� ������, ����� ��� �����
void uart_transmit_str(uint8_t* string){
/* ������� ������ ��������� � ������� ��������� *
��������� � ����� �������� �������� ������������ ������*/
  while(*string)
    uart_transmit(*string++);
}

uint8_t Digit(uint16_t d, uint8_t m){
  uint8_t i = 5, a;
  while(i){ //���� �� �������� �����
    a = d%10; //�������� ��������� ������
    //������� �������� ������ - ������
    if(i-- == m) break; 
    d /= 10; //��������� ����� � 10 ���
    }
  return(a);
}
// ������� ����������� 16-������� ����� � �� � ��������
void uart_transmit_dec(uint16_t val){
  uint8_t i, j;
// ���� ���������� �� ����� �� ����
  if(val!=0)
  {
    /* ��������� ���������� ������� �������� � 
    ������������� �����, �������� � ����� 64 ����� ��� 
    ������� ���������� ������� � 00064 */
    j=1;
    while(!(Digit(val, j++)));
    // �� ������� ���������� ���� � ���������
    for(i=j-1; i<=5; i++) uart_transmit(Digit(val, i)+'0');
  }
  /* ���� �������� ���� � ������� ����, ����� �������� 
  � ����� */
  else 
    uart_transmit('0');
}

// ���� ����������� 16-������� ����� �� �� � ���������
uint16_t uart_receive_dec(void){
  uint16_t data = 0;
  uint8_t digit = 0;
// ��������� ������������� �����
  do
  {
    digit = uart_receive();
    // ���������, ���� �� ������ ������� ����� ������
    if(digit == '\r') continue;
    if(digit == '\n') break;
    if(!((digit >= '0') && (digit <= '9'))) break;
    // ������� ASCII ���� ����
    digit -= '0';
    data *= 10;
    data += digit;
  }
  // �����������, ����� ������� ������� \r\n
  while(1);
  return data;
}

ISR (INT0_vect)
{
  //���� ����� ������� �� ECHO, �� ���������� ������
  if((PIND & (1<<2)) != 0)
  {
    TCNT1H = 0; 
    TCNT1L = 0;
  }
  //���� ���� ������� �� ECHO, �� �������� ��������
  else
  {
    /* dist � ���������� ����������, ������� ����� � 
    ������� ISR � main() */
    dist = TCNT1L; 
    dist |= (TCNT1H << 8);
  }
}


int main(void)
{
	// �������� ��� ���������� �������� ����������������
	_delay_ms(50);
	uint8_t word[] = {"TEST"};
	// ���������� ���� � ��� ������ � ������� �1
	DDRD |= (1<<6) | (1<<3);
	// ����������� ���
	TCCR0A = (1<<COM0A1) | (1<<WGM01) | (1<<WGM00);
	TCCR0B = (1<<CS00);
	TCCR1B = (1<<CS10) | (1<<CS11);
	// ����������� ����������
	EIMSK = (1<<INT0);
	EICRA = (1<<ISC00);
	sei();
	uart_init();
	// ������� � �������� ����� TEST
	uart_transmit_str(word);
	// ��������� � ������ ��������� ������
	uart_transmit('\r');
	while (1)
	{
		PORTD |= (1<<3);
		_delay_us(20);
		PORTD &= ~(1<<3);
		_delay_ms(200);
		// ������� ���������� � [��]
		uart_transmit_dec(dist/14);
		uart_transmit('\r');
	}
}


