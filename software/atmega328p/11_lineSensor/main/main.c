/*
 * main.c
 *
 * Created: 27.01.2022 17:11:09
 * Author : Michael
 */ 

#include <avr/io.h>
/* важно не ошибиться при выставлении частоты тактирования */
#define F_CPU 16000000
#include <util/delay.h>
#include <avr/interrupt.h>

#define NO_LINE 0
#define LINE 1
#define TURN_RIGHT 2
#define TURN_LEFT 3
#define GO_STRAIGHT 4

//переменная, замедляющая вызов функции прерывания
uint8_t t1_flag = 0;
//переменные, хранящие направления движения колёс
uint8_t sens_left = 0;
uint8_t sens_right = 0;

// функция передачи байта
void uart_transmit(uint8_t byte){
  /* ожидаем выставления бита UDRE0, а затем кладем
  посылку в UDR0 */
  while(!(UCSR0A & (1<<UDRE0)));
  UDR0 = byte;
}

// функция приёма байта
uint8_t uart_receive(void){
  /* ожидаем выставления бита RXC0, а затем забираем
  посылку из UDR0 */
  while(!(UCSR0A & (1<<RXC0)));
  return UDR0;
}

// функция передачи строки, слова или фразы
void uart_transmit_str(uint8_t* string){
/* передаём данные побайтово с помощью указателя *
указатель – адрес «первого элемента» передаваемых данных*/
  while(*string)
    uart_transmit(*string++);
}

uint8_t Digit(uint16_t d, uint8_t m){
  uint8_t i = 5, a;
  while(i){ //цикл по разрядам числа
    a = d%10; //выделяем очередной разряд
    //выделен заданный разряд - уходим
    if(i-- == m) break; 
    d /= 10; //уменьшаем число в 10 раз
    }
  return(a);
}
// посылка десятичного 16-битного числа с МК в терминал
void uart_transmit_dec(uint16_t val){
  uint8_t i, j;
// если посылаемое МК число не ноль
  if(val!=0)
  {
    /* проверяем количество нулевых разрядов в 
    пятиразрядном числе, например у числа 64 будет три 
    нулевых незначащих разряда – 00064 */
    j=1;
    while(!(Digit(val, j++)));
    // не выводим незначащие нули в терминале
    for(i=j-1; i<=5; i++) uart_transmit(Digit(val, i)+'0');
  }
  /* если значение ноль – выводим ноль, иначе зависнем 
  в цикле */
  else 
    uart_transmit('0');
}

// приём десятичного 16-битного числа на МК с терминала
uint16_t uart_receive_dec(void){
  uint16_t data = 0;
  uint8_t digit = 0;
// принимаем пятиразрядное число
  do
  {
    digit = uart_receive();
    // принимаем, пока не увидим признак конца строки
    if(digit == '\r') continue;
    if(digit == '\n') break;
    if(!((digit >= '0') && (digit <= '9'))) break;
    // выводим ASCII коды цифр
    digit -= '0';
    data *= 10;
    data += digit;
  }
  while(1);
  // заканчиваем, когда увидели признак \r\n
  return data;
}

/* функция инициализации: настройка МК на приём и передачу данных, длина сообщения – 8 бит, один СТОП бит, без паритета, скорость обмена – 9600 бод */
void uart_init(void){
	uint8_t word[] = {"TEST"};
	UCSR0B |= (1<<RXEN0) | (1<<TXEN0);
	UCSR0C |= (1<<UCSZ01) | (1<<UCSZ00);
	//скорость передачи 9600 бод
	UBRR0H = 0;
	//для значения 9600 UBRR0L = 103 = 0x67
	UBRR0L = 0x67;
	// выводим в терминал слово TEST
	uart_transmit_str(word);
	// переходим в начало следующей строки
	uart_transmit('\r');
}

int main(void){
	// задержка для завершения процесса программирования
	_delay_ms(50);
	// подключаем диод к ШИМ каналу А таймера №0
	DDRD |= (1<<5)|(1<<6);
	// настраиваем ШИМ
	TCCR0A = (1<<COM0A1) | (1<<COM0A0) | (1<<COM1B1) | (1<<COM1B0) | (1<<WGM01) | (1<<WGM00);
	TCCR0B = (1<<CS00);
	TCCR1B=(1<<CS11); //период таймера №1 10 мс
	TCNT1H=0xB1; //начальное значение таймера
	TCNT1L=0xE0;
	TIMSK1 = (1<<TOIE1); //прерывание по переполнению
	EIMSK = (1<<INT0) | (1<<INT1);
	EICRA = (1<<ISC00) | (1<<ISC10);
	uart_init();
	//разрешить прерывания глобально
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

/* функция обработчик прерывания (ФОП)
исполняется 1 раз в 10 мс */
ISR (TIMER1_OVF_vect)
{
	TCNT1H=0xB1; //начальное значение таймера
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

