/*
 * utils.c
 *
 * Created: 14.03.2022 19:03:31
 *  Author: Michael
 */ 

#include "utils.h"

uint8_t Digit(uint16_t d, uint8_t m){
	uint8_t i = 5, a;
	while(i){ //���� �� �������� �����
		a = d%10; //�������� ��������� ������
		//������� �������� ������ - ������
		if(i-- == m) break;
		d /= 10; //��������� ����� � 10 ���
	}
	return(a);
}