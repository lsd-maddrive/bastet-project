#include "st_mot.h"
#include "utils.h"
#include "uart.h"

void InitAll(void);

int main(void)
{
	InitAll();
	while (1)
	{
		
		//получаем данные с терминала 0Е100 Ц скважность Ў»ћ
		//data = UartReceiveDec();
		// выставл€ем скорость вращени€ двигател€
			// табул€ци€ положени€ курсора в терминале
		UartTransmitByte('\t');
		// выводим только что переданное значение
		UartSendDec(GetCount());
		//UartTransmitByte('\t');
		//error = data - GetSpeed();
		//UartSendDec(error);
		
		UartTransmitByte('\r');
		//PORTB^=(1<<7);
		//_delay_ms(500);
	}
}

void InitAll(void){
	UartInit();
	DDRB|=(1<<7);
	DDRF|=(1<<5);
	//AdcInit();
	StMotInit();
	//StMotGo(0);
	_delay_ms(100);
	sei();
}