#include "uart.h"

// функция передачи байта
void uart_transmit_byte(uint8_t byte){
	/* ожидаем выставления бита UDRE0, а затем кладем посылку в UDR0 */
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = byte;
}

// функция приёма байта
uint8_t uart_receive_byte(void){
	/* ожидаем выставления бита RXC0, а затем забираем посылку из UDR0 */
	while(!(UCSR0A & (1<<RXC0)));
	return(UDR0);
}

// посылка десятичного 16-битного числа с МК в терминал
void uart_send_dec(uint16_t val){
	uint8_t i, j;
	// если посылаемое МК число не ноль
	if(val!=0){
		/* проверяем количество нулевых разрядов в пятиразрядном числе, 
		например у числа 64 будет три нулевых незначащих разряда – 00064 */
		j=1;
		while(!(Digit(val, j++)));
		// не выводим незначащие нули в терминале
		for(i=j-1; i<=5; i++) uart_transmit_byte(Digit(val, i)+'0');
	}
	/* если значение ноль – выводим ноль, иначе зависнем 
	в цикле */
	else uart_transmit_byte('0');
}

// функция передачи строки, слова или фразы
void uart_send_str(uint8_t* string){
	/* передаём данные побайтово с помощью указателя *
	указатель – адрес «первого элемента» передаваемых данных*/
	while(*string) uart_transmit_byte(*string++);
}

// посылка hex числа с МК в терминал
void uart_send_hex(uint16_t val){
	uint8_t msb = (val & 0xF0) >> 4, lsb = val & 0x0F;
	msb += msb > 9 ? 'A' - 10 : '0';
	lsb += lsb > 9 ? 'A' - 10 : '0';	
	uart_transmit_byte(msb);
	uart_transmit_byte(lsb);
}

// приём десятичного 16-битного числа на МК с терминала
uint16_t uart_receive_dec(void){
	uint16_t data = 0;
	uint8_t digit = 0;
	// принимаем пятиразрядное число
	do{
		digit = uart_receive_byte();
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
	return(data);
}

/* функция инициализации: настройка МК на приём и передачу данных, 
длина сообщения – 8 бит, один СТОП бит, без паритета, скорость обмена – 9600 бод */
void uart_init(void){
	uint8_t word[] = {"OK"};
	UCSR0B |= (1<<RXEN0) | (1<<TXEN0);
	UCSR0C |= (1<<UCSZ01) | (1<<UCSZ00);
	//скорость передачи 9600 бод
	UBRR0H = 0;
	//для значения 9600 UBRR0L = 103 = 0x67
	UBRR0L = 0x67;
	// выводим в терминал слово TEST
	uart_send_str(word);
	// переходим в начало следующей строки
	uart_transmit_byte('\r');
}