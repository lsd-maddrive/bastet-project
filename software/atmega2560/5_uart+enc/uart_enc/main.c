#define MAIN_ROUTINE 0
#define TEST1 1
#define PROGRAM_ROUTINE TEST1

#include "dc_motor.h"
#include "utils.h"
#include "uart.h"

void InitAll(void);

int main(void){
	
	InitAll();
	_delay_ms(50);
	float data;
	float error;
	data = 0;
	while (1)
	{
		//получаем данные с терминала 0Е100 Ц скважность Ў»ћ
		//data = UartReceiveDec();
		// выставл€ем скорость вращени€ двигател€
		SetSpeed(data);
		// табул€ци€ положени€ курсора в терминале
		UartTransmitByte('\t');
		// выводим только что переданное значение
		UartSendDec(data);
		for (int i = 0; i < 4; i++){
			UartTransmitByte('\t');
			UartSendDec(GetSpeed()[i]);
			//UartSendDec(data - GetSpeed()[i]);
			
		}
		//UartTransmitByte('\t');
		//error = data - GetSpeed();
		//UartSendDec(error);
	
		UartTransmitByte('\r');
		
	}
}

void InitAll(void){
	UartInit();
	//ExpInit(EXP_DEF_ADDR);
	//AdcInit();
	DcMotInit();
	DDRB|=(1<<7);
	//StMotInit();
	//StMotGo(0);
	//_delay_ms(100);
	sei();
}
