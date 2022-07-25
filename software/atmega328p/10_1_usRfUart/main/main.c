/*
 * main.c
 *
 * Created: 24.01.2022 18:38:44
 * Author : Michael
 */ 

#include <avr/io.h>
/* важно не ошибиться при выставлении частоты тактирования */
#define F_CPU 16000000
#include <util/delay.h>
#include <avr/interrupt.h>

uint16_t dist;

/* функция инициализации: настройка МК на приём и передачу данных, длина сообщения – 8 бит, один СТОП бит, без паритета, скорость обмена – 9600 бод */
void uart_init(void){
  UCSR0B |= (1<<RXEN0) | (1<<TXEN0);
  UCSR0C |= (1<<UCSZ01) | (1<<UCSZ00);
  //скорость передачи 9600 бод
  UBRR0H = 0;
  //для значения 9600 UBRR0L = 103 = 0x67
  UBRR0L = 0x67;
}

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
  // заканчиваем, когда увидели признак \r\n
  while(1);
  return data;
}

ISR (INT0_vect)
{
  //если фронт сигнала на ECHO, то сбрасываем таймер
  if((PIND & (1<<2)) != 0)
  {
    TCNT1H = 0; 
    TCNT1L = 0;
  }
  //если срез сигнала на ECHO, то забираем значение
  else
  {
    /* dist – глобальная переменная, которая видна и 
    функции ISR и main() */
    dist = TCNT1L; 
    dist |= (TCNT1H << 8);
  }
}


int main(void)
{
	// задержка для завершения процесса программирования
	_delay_ms(50);
	uint8_t word[] = {"TEST"};
	// подключаем диод к ШИМ каналу А таймера №1
	DDRD |= (1<<6) | (1<<3);
	// настраиваем ШИМ
	TCCR0A = (1<<COM0A1) | (1<<WGM01) | (1<<WGM00);
	TCCR0B = (1<<CS00);
	TCCR1B = (1<<CS10) | (1<<CS11);
	// настраиваем прерывания
	EIMSK = (1<<INT0);
	EICRA = (1<<ISC00);
	sei();
	uart_init();
	// выводим в терминал слово TEST
	uart_transmit_str(word);
	// переходим в начало следующей строки
	uart_transmit('\r');
	while (1)
	{
		PORTD |= (1<<3);
		_delay_us(20);
		PORTD &= ~(1<<3);
		_delay_ms(200);
		// выводим расстояние в [см]
		uart_transmit_dec(dist/14);
		uart_transmit('\r');
	}
}


