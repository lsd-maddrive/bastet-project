#include "utils.h"

uint16_t tim1_count=0;

uint8_t Digit(uint16_t d, uint8_t m){
	uint8_t i = 5, a;
	while(i){ //цикл по разр€дам числа
		a = d%10; //выдел€ем очередной разр€д
		//выделен заданный разр€д - уходим
		if(i-- == m) break;
		d /= 10; //уменьшаем число в 10 раз
	}
	return(a);
}

float find_max_out_of_3(float a, float b, float c){
	if((a != b) && (a != c) && (b != c)){
		if(a > b){
			if(a > c) return(a);
			else return(c);
		}
		else{
			if(b > c) return(b);
			else return(c);
		}
	}
	else{
		if(a == b){
			if(a > c) return(a);
			else return(c);
		}
		else{
			if(a == c){
				if(a > b) return(a);
				else return(b);
			}
			else{
				if(b == c){
					if(b > a) return(b);
					else return(a);
				}
			}
		}
	}
	return(0);
}

float find_min_out_of_3(float a, float b, float c){
	if((a != b) && (a != c) && (b != c)){
		if(a < b){
			if(a < c) return(a);
			else return(c);
		}
		else{
			if(b < c) return(b);
			else return(c);
		}
	}
	else{
		if(a == b){
			if(a < c) return(a);
			else return(c);
		}
		else{
			if(a == c){
				if(a < b) return(a);
				else return(b);
			}
			else{
				if(b == c){
					if(b < a) return(b);
					else return(a);
				}
			}
		}
	}
	return(0);
}

void tim1_delay_ms(uint16_t millisec){ //period - 1ms
	TCNT1H=0xC1;//(65536-16000)>>8;
	TCNT1L=0x80;//65536-16000;
	TCCR1B|=(1<<CS10); //prescaler - 1
	while(tim1_count<millisec){
		while((TIFR1&(1<<TOV1))==0);
		TCNT1H=0xC1;//(65536-16000)>>8;
		TCNT1L=0x80;//65536-16000;
		tim1_count++;
		TIFR1|=(1<<TOV1);
	}
	tim1_count=0;
	TCCR1B&=~(1<<CS10);
}