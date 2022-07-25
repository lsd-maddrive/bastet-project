/*
 * main.c
 *
 * Created: 30.12.2021 16:13:19
 * Author : Michael
 */ 

#include <avr/io.h>
#define F_CPU 16000000
#include <util/delay.h>
#include <avr/interrupt.h>

void SPI_init(void);
unsigned char Digit(uint16_t d, uint8_t m);
void showMe(int16_t spiData);

int16_t angle;
uint8_t flag = 0;

int main(void)
{
	EIMSK = (1<<INT0);
	//настройка формы сигнала
	EICRA = (1<<ISC00);
	
	int16_t res; //угол поворота сервомашинки
    DDRB |= (1<<1); //ножка Ў»ћ Ц на выход
    /* быстра€ Ў»ћ с переменным верхним пределом ICR и
    предделитель на 8 */
    TCCR1A = (1<<COM1A1) | (1<<WGM11);
    TCCR1B = (1<<WGM12) | (1<<WGM13) | (1<<CS11);
    //установка верхнего предела счЄта таймера
    ICR1H = (uint8_t)(40000 >> 8);
    ICR1L = (uint8_t)40000;
	
	SPI_init();
	_delay_ms(200);

	//глобальное разрешение прерываний
	sei();
	
	while (1)
	{
		res=angle;
		cli();
		showMe(res);   
		OCR1AH = ((2000+(res*20))>>8);
		OCR1AL = (2000+(res*20));
		sei();
	}
}

void SPI_init(void) //инициализаци€ аппаратной части SPI
{
	DDRB |= (1<<0)|(1<<2)|(1<<3)|(1<<5); //выводы CS, SS, MOSI, SCK Ц на выход
	//включить аппаратный SPI, режим 0, предделитель на 128
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR1)|(1<<SPR0);
}

/*функци€ дл€ выделени€ из трЄхразр€дного числа d разр€да m
например, Digit(14, 3) = 4; или Digit(591, 1) = 5 */
unsigned char Digit(uint16_t d, uint8_t m)
{
	uint8_t i = 3, a;
	while(i){ //цикл по разр€дам числа
		a = d%10; //выдел€ем очередной разр€д
		if(i-- == m) break; //выделен заданный разр€д - уходим
		d /= 10; //уменьшаем число в 10 раз
	}
	return(a);
}

void showMe(int16_t spiData)
{
	int8_t i;
	uint8_t dig[] = {0,0,0}, j; //разр€ды числа идут в dig[]
	uint8_t mas[] = {0x81,0xF3,0x49,0x61,0x33,0x25,0x05,0xF1,0x01,0x21,0xFF,0x7F}; //С0Т,..., С9Т, сегмент выключен, С-Т
	uint16_t res; //будем переприсваивать число в res
	if(spiData < 0) res = -spiData; //убираем отрицательность
	else res = spiData;
	for(i=1; i<=3; i++) //вычлен€ем цифры из числа
	dig[i-1] = Digit(res,i);
	if(res != 0) //если передан не ноль Ц выводим число
	{
		j=0;
		while(dig[j] == 0) //убираем нули (не 0428, а _428)
		j++;
		/* сначала посылаетс€ знак, потом - сотни, потом Ц дес€тки, а после всего Ц единицы */
		if(spiData < 0)
		{
			SPDR=mas[11]; //отрицательное число - знак минус
			while(!(SPSR&(1<<SPIF)));
		}
		else
		{
			SPDR=mas[10]; //положительное число - ничего
			while(!(SPSR&(1<<SPIF)));
		}
		for(i=0; i<=2; i++)
		{
			if(i<j) SPDR=mas[10]; //вместо ненужных нулей - пусто
			else SPDR=mas[dig[i]];
			while(!(SPSR&(1<<SPIF))); //высылаем значение SPDR
		}
	}
	else
	{
		for(i=0; i<=2; i++)
		{
			SPDR=mas[10];
			while(!(SPSR&(1<<SPIF)));
		}
		SPDR=mas[0]; //если res == 0, выводим на экран ___0
		while(!(SPSR&(1<<SPIF)));
	}
	PORTB &= ~(1<<0); //защелкиваем выводимое число
	_delay_us(20);
	PORTB |= (1<<0);
	_delay_us(20);
	PORTB &= ~(1<<0);
}

ISR (INT0_vect)
{
	if(flag == 1)
	{
		flag = 0;
		//провер€ем фронт/срез на канале A
		if((PIND & (1 << 2)) != 0)
		{
			//провер€ем фронт/срез на канале B
			if((PIND & (1 << 3)) != 0) angle--;
			else angle++;
		}
		else
		{
			if((PIND & (1 << 3)) != 0) angle++;
			else angle--;
		}
	}
	else
	{
		flag = 1;
	}
	if(angle>=100) angle=100;
	if(angle<=0) angle = 0;
}
