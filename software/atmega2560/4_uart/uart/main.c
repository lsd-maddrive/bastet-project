/*
* troyka_robot_chassis.c
* Working with 4 stepper motors, 4 potentiometers, 4 DC motors and 3-DOF servo arm.
*
* Created: 11.03.2022 12:13:32
* Author : Michael
*/

#define MAIN_ROUTINE 0
#define TEST1 1

#define PROGRAM_ROUTINE TEST1
#include "uart.h"
#include "dc_motor.h"

void InitAll(void);

//int main(void){
//
//InitAll();
//
//while (1){
//float info=UartReceiveData();
//UartSendDec(info);
//UartTransmitByte('\t');
//uint16_t* dc_mot_enc=GetSpeed();
//for(uint8_t i=0; i<4; i++){
//UartSendDec(dc_mot_enc[i]);
//UartTransmitByte('\t');
//}
//
////Вывод скорости в терминал
//UartSendDec(GetSpeed());
//UartTransmitByte('\r');
//_delay_ms(20);
//}

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

//uint8_t Digit(uint16_t d, uint8_t m){
	//uint8_t i = 5, a;
	//while(i){ //цикл по разрядам числа
		//a = d%10; //выделяем очередной разряд
		////выделен заданный разряд - уходим
		//if(i-- == m) break;
		//d /= 10; //уменьшаем число в 10 раз
	//}
	//return(a);
//}
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

int main(void){
	InitAll();
	// задержка для завершения процесса программирования
	_delay_ms(50);
	uint8_t data, word[] = {"TEST"};
	// подключаем диод к ШИМ каналу А таймера №1
	//DDRH |= (1<<6);
	// настраиваем ШИМ
	//TCCR2A = (1<<COM2B1) | (1<<WGM21) | (1<<WGM20);
	//TCCR2B = (1<<CS20);
	uart_init();
	// выводим в терминал слово TEST
	uart_transmit_str(word);
	// переходим в начало следующей строки
	uart_transmit('\r');
	while (1)
	{
		// получаем данные с терминала 0…255 – скважность ШИМ
		data = uart_receive_dec();
		// выставляем яркость свечения светодиода
		DcMotGo(data);
		// табуляция положения курсора в терминале
		uart_transmit('\t');
		// выводим только что переданное значение
		uart_transmit_dec(data);
		uart_transmit('\r');
	}
	
}

void InitAll(void){
	UartInit();
	//I2cInit();
	//ExpInit(EXP_DEF_ADDR);
	//AdcInit();
	DcMotInit();
	//StMotInit();
	//StMotGo(0);
	//_delay_ms(100);
	//sei();
}