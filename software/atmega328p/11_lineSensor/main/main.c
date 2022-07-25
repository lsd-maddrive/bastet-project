/*
 * main.c
 *
 * Created: 27.01.2022 17:11:09
 * Author : Michael
 */ 

#include <avr/io.h>
/* ����� �� ��������� ��� ����������� ������� ������������ */
#define F_CPU 16000000
#include <util/delay.h>
#include <avr/interrupt.h>

#define NO_LINE 0
#define LINE 1
#define TURN_RIGHT 2
#define TURN_LEFT 3
#define GO_STRAIGHT 4

//����������, ����������� ����� ������� ����������
uint8_t t1_flag = 0;
//����������, �������� ����������� �������� ����
uint8_t sens_left = 0;
uint8_t sens_right = 0;

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
  while(1);
  // �����������, ����� ������� ������� \r\n
  return data;
}

/* ������� �������������: ��������� �� �� ���� � �������� ������, ����� ��������� � 8 ���, ���� ���� ���, ��� ��������, �������� ������ � 9600 ��� */
void uart_init(void){
	uint8_t word[] = {"TEST"};
	UCSR0B |= (1<<RXEN0) | (1<<TXEN0);
	UCSR0C |= (1<<UCSZ01) | (1<<UCSZ00);
	//�������� �������� 9600 ���
	UBRR0H = 0;
	//��� �������� 9600 UBRR0L = 103 = 0x67
	UBRR0L = 0x67;
	// ������� � �������� ����� TEST
	uart_transmit_str(word);
	// ��������� � ������ ��������� ������
	uart_transmit('\r');
}

int main(void){
	// �������� ��� ���������� �������� ����������������
	_delay_ms(50);
	// ���������� ���� � ��� ������ � ������� �0
	DDRD |= (1<<5)|(1<<6);
	// ����������� ���
	TCCR0A = (1<<COM0A1) | (1<<COM0A0) | (1<<COM1B1) | (1<<COM1B0) | (1<<WGM01) | (1<<WGM00);
	TCCR0B = (1<<CS00);
	TCCR1B=(1<<CS11); //������ ������� �1 10 ��
	TCNT1H=0xB1; //��������� �������� �������
	TCNT1L=0xE0;
	TIMSK1 = (1<<TOIE1); //���������� �� ������������
	EIMSK = (1<<INT0) | (1<<INT1);
	EICRA = (1<<ISC00) | (1<<ISC10);
	uart_init();
	//��������� ���������� ���������
	sei();
	while (1)
	{
	}
}

ISR(INT0_vect)
{
	if((PIND & (1<<2)) != 0) sens_left = LINE;
	else sens_left = NO_LINE;
}

ISR(INT1_vect)
{
	if((PIND & (1<<3)) != 0) sens_right = LINE;
	else sens_right = NO_LINE;
}

/* ������� ���������� ���������� (���)
����������� 1 ��� � 10 �� */
ISR (TIMER1_OVF_vect)
{
	TCNT1H=0xB1; //��������� �������� �������
	TCNT1L=0xE0;
	if(t1_flag<10) t1_flag++;
	else
	{
		t1_flag=0;
		if((sens_left == NO_LINE) && (sens_right == NO_LINE))
		{
			OCR0A = 255;
			OCR0B = 255;
			uart_transmit_dec(GO_STRAIGHT);
			uart_transmit('\r');
		}
		if((sens_left == NO_LINE) && (sens_right == LINE))
		{
			OCR0A = 255;
			OCR0B = 32;
			uart_transmit_dec(TURN_LEFT);
			uart_transmit('\r');
		}
		if((sens_left == LINE) && (sens_right == NO_LINE))
		{
			OCR0A = 32;
			OCR0B = 255;
			uart_transmit_dec(TURN_RIGHT);
			uart_transmit('\r');
		}
	}
}

