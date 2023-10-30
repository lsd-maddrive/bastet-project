#include <avr/io.h>
#define F_CPU 16000000
#include <util/delay.h>


//int main(void)
//{
    //DDRD = 0xF0;
	//while (1) 
    //{
		//for(uint8_t i=0; i<6; i++)
		//{
			//if(i<=3) PORTD=(1<<(4+i));
			//else PORTD = (1<<(6-(i-4)));
			//_delay_ms(50);
		//}
    //}
//}

int main(void)
{
	DDRE = (1<<4);
	while (1)
	{
		PORTE ^= (1<<4);
		_delay_ms(1000);
	}
}

