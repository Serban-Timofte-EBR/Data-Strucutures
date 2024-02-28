#include <stdio.h>

int main() {
	char a; // 1 Byte format din 8 Biti
			// char is signed by default ( first bite - s^7 - is 0 for + and 1 for - )
			// pentru pozitivi avem Cod Direct, iar pentru negativ Cod Complementar 2 ( CD -> CI + 1 )
			// pentru unsigned char avem CD ( Nr naturale ) - doar nr pozitive

	a = 65; // a = 'A';

	printf("\'%c\' = %d in ASCII\n", a, a); // prin \ introduce " intr-o secventa de escape
								 //Vizualizam char a ca si cod ASCII 65 (A) si ca numar 65

	unsigned char b = 0x42; // reprezentare in baza 16
							// 1 byte impartit in doua jumatati egale 
							// fiecare jumatate este 2^0, 2^1, 2^2, 2^3
							// Exemplul usigned char b
							//	0	 1    0    0  |	  0    0     1     0
							// 2^3	2^2	 2^1  2^0 |  2^3   2^2  2^1   2^0 
							// adresa lui b este mai mica decat adresa lui a => marirea stivei se face prin scaderea varfului de segmentului de stiva
							// ESF indica varful stivei si daca primele 4 ( offset ) sunt egale cu primele 4 de la a si b, iar urmatoarele 4 sunt mai mari decat ultimele 4 din ESF => a si b in stack

	//short int - 2B
	//long int - 4B
	//long long - 8B
	
	//Deci, tipurile de baza int sunt char si int cu diferiti specificatori

	//Reali ( de la mic la mare )
	//float - 4B
	//double - 8B
	//long double - 10/16B

	return 0;
}