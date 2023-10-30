/*
 * main.c
 *
 * Created: 25.01.2022 16:47:13
 * Author : Michael
 */ 

#include <avr/io.h>
/* важно не ошибиться при выставлении частоты тактирования */
#define F_CPU 16000000
#include <util/delay.h>
#include <math.h> //здесь есть возведение в степень

uint8_t buzz = 0;

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
  while(1);
  // заканчиваем, когда увидели признак \r\n
  return data;
}

uint16_t readADC(uint8_t channel)			
{
  /* переменные, которые будут хранить результат
  преобразования АЦП, lbv–младшая, hbv–старшая части */
  uint8_t lbv=0, hbv=0;
  /* Питание – AVCC; правое выравнивание, номер канала
  АЦП - channel (входная переменная) */
  ADMUX |= (1<<REFS0) | channel;
  _delay_us(25);
  //начать преобразование аналогового сигнала в цифру
  ADCSRA |= (1<<ADSC);
  /* Если бит ADIF = 0 – преобразование не окончено,
  если бит ADIF = 1 – преобразование окончено */
  while(!(ADCSRA & (1<<ADIF)));
  //сбросить ADIF – записать в него 1
  ADCSRA |= (1<<ADIF);
  //переприсваиваем значения регистров
  lbv = ADCL;
  hbv = ADCH;
  //«склеиваем» результат
  return ((hbv<<8)|lbv);
}

int main(void)
{
  uint8_t i;
  uint16_t result, av_val;
  uint8_t word[] = {"TEST"}, err[] = {"dist_error"};
  // задержка для завершения процесса программирования
  _delay_ms(50);
  // подключаем диод к ШИМ каналу А таймера №0
  DDRD |= (1<<6);
  DDRB |= (1<<1);
  // настраиваем ШИМ
  TCCR0A = (1<<COM0A1) | (1<<WGM01) | (1<<WGM00);
  TCCR0B = (1<<CS00);
  TCCR1A = (1<<COM1A0);
  TCCR1B = (1<<WGM12) | (1<<CS10);
  //включить АЦП и выставить предделитель на 4
  ADCSRA |= (1<<ADEN) | (1<<ADPS1);
  uart_init();
  // выводим в терминал слово TEST
  uart_transmit_str(word);
  // переходим в начало следующей строки
  uart_transmit('\r');
  while(1)
  {
    result=0;
    //совершаем пять измерений через каждые 50 мс
    for(i=0; i<5; i++)
    {
      result+=readADC(1); //дальномер подключен к ADC1
      _delay_ms(50);
    }
    //получаем среднее арифметическое из пяти измерений
    av_val=result/5;
    //5 В АЦП / 1024 уровней АЦП = 0.0048828125
    float sens_out = av_val*0.0048828125;
    //восстанавливаем график датчика (см. рис. 4.10.5)
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

