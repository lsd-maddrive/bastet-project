#include "utils.h"

uint8_t Digit(uint16_t d, uint8_t m){
	uint8_t i=5, a;
	while(i){ //���� �� �������� �����
		a=d%10; //�������� ��������� ������
		//������� �������� ������ - ������
		if(i-- == m) break;
		d /= 10; //��������� ����� � 10 ���
	}
	return(a);
}

uint16_t FindMiddleValue(uint16_t a, uint16_t b, uint16_t c){
	if(a>b){
		if(b>c){
			return(b);
		}
		else if(a>c){
			return(c);
		}
		else{
			return(a);
		}
	}
	else{
		if(a>c){
			return(a);
		}
		else if(b>c){
			return(c);
		}
		else{
			return(b);
		}
	}
}

uint16_t FindMaxValue(uint16_t a, uint16_t b, uint16_t c){
	if(a>b){
		if(b>c){
			return(a);
		}
		else if(c>a){
			return(c);
		}
		else{
			return(a);
		}
	}
	else{
		if(b>c){
			return(b);
		}
		else{
			return(c);
		}
	}
}

uint16_t FindMinValue(uint16_t a, uint16_t b, uint16_t c){
	if(a<b){
		if(b<c){
			return(a);
		}
		else if(c<a){
			return(c);
		}
		else{
			return(a);
		}
	}
	else{
		if(b<c){
			return(b);
		}
		else{
			return(c);
		}
	}
}