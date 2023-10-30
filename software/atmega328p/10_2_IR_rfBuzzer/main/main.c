/*
 * main.c
 *
 * Created: 25.01.2022 16:47:13
 * Author : Michael
 */ 

#include <avr/io.h>
/* ����� �� ��������� ��� ����������� ������� ������������ */
#define F_CPU 16000000
#include <util/delay.h>
#include <math.h> //����� ���� ���������� � �������

uint8_t buzz = 0;

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
  while(1);
  // �����������, ����� ������� ������� \r\n
  return data;
}

uint16_t readADC(uint8_t channel)			
{
  /* ����������, ������� ����� ������� ���������
  �������������� ���, lbv��������, hbv�������� ����� */
  uint8_t lbv=0, hbv=0;
  /* ������� � AVCC; ������ ������������, ����� ������
  ��� - channel (������� ����������) */
  ADMUX |= (1<<REFS0) | channel;
  _delay_us(25);
  //������ �������������� ����������� ������� � �����
  ADCSRA |= (1<<ADSC);
  /* ���� ��� ADIF = 0 � �������������� �� ��������,
  ���� ��� ADIF = 1 � �������������� �������� */
  while(!(ADCSRA & (1<<ADIF)));
  //�������� ADIF � �������� � ���� 1
  ADCSRA |= (1<<ADIF);
  //��������������� �������� ���������
  lbv = ADCL;
  hbv = ADCH;
  //���������� ���������
  return ((hbv<<8)|lbv);
}

int main(void)
{
  uint8_t i;
  uint16_t result, av_val;
  uint8_t word[] = {"TEST"}, err[] = {"dist_error"};
  // �������� ��� ���������� �������� ����������������
  _delay_ms(50);
  // ���������� ���� � ��� ������ � ������� �0
  DDRD |= (1<<6);
  DDRB |= (1<<1);
  // ����������� ���
  TCCR0A = (1<<COM0A1) | (1<<WGM01) | (1<<WGM00);
  TCCR0B = (1<<CS00);
  TCCR1A = (1<<COM1A0);
  TCCR1B = (1<<WGM12) | (1<<CS10);
  //�������� ��� � ��������� ������������ �� 4
  ADCSRA |= (1<<ADEN) | (1<<ADPS1);
  uart_init();
  // ������� � �������� ����� TEST
  uart_transmit_str(word);
  // ��������� � ������ ��������� ������
  uart_transmit('\r');
  while(1)
  {
    result=0;
    //��������� ���� ��������� ����� ������ 50 ��
    for(i=0; i<5; i++)
    {
      result+=readADC(1); //��������� ��������� � ADC1
      _delay_ms(50);
    }
    //�������� ������� �������������� �� ���� ���������
    av_val=result/5;
    //5 � ��� / 1024 ������� ��� = 0.0048828125
    float sens_out = av_val*0.0048828125;
    //��������������� ������ ������� (��. ���. 4.10.5)
    float dist = 29.988*pow(sens_out,-1.173);
	if((dist <= 80) && (dist >= 10))
	{
		OCR0A = ((-4)*dist)+295;
		if(dist<=15) OCR1A = 8000;
		else OCR1A = 0;
		uart_transmit_dec((uint16_t)dist); 
	}
	else  uart_transmit_str(err);
	uart_transmit('\r');
  }
}

