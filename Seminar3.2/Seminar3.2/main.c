#define _CRT_SECURED_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>

int suma1(char x, char y) {
	int z = 0;
	x += 1;
	y += x;

	z = x + y;
	return z;
}

int suma2(char x, char* y) {
	int z = 0;
	x += 1;
	*y += x;	//prelucrez continut de date

	z = x + *y;
	return z;
}


//Trimitere un pointer ca valoare 
char allocHeap1(char* vreauSaIlAloc, char size) {		//size - numar de octeti
	vreauSaIlAloc = (char*)malloc(size * sizeof(char));

	if (vreauSaIlAloc != NULL) {
		return 1;
	}
	return 0;
}

//vreauSaIlAloc este in main char*, dar in functie este char** ca sa salvez modificarile
char allocHeap2(char** vreauSaIlAloc, char size) {		//size - numar de octeti
	*vreauSaIlAloc = (char*)malloc(size * sizeof(char));

	if (*vreauSaIlAloc != NULL) {
		return 1;
	}
	return 0;
}

char* allocStack(unsigned char* size) {
	char v[] = { 1,2,3,4,5,6 };
	*size = sizeof(v);
	return v;	//nu vom mai avea la adresa primului element din v continutul folosit in functie
}

int main() {
	int a = 3, b = 8, c = 0;

	printf("a = %d, b = %d, c = %d\n", a, b, c);
	c = suma1(a, b);
	printf("a = %d, b = %d, c = %d\n", a, b, c);
	c = suma2(a, &b);
	printf("a = %d, b = %d, c = %d\n", a, b, c);

	char* p = NULL;
	printf("p = 0x%p", p);
	// Pointerul este la output null, dar functia a reusit sa aloce => memory leak
	// Ca sa transferi prin adresa, trebuie sa avem o steluta in plus in argumentele functiei
	char rez = allocHeap1(p, (unsigned char)a);	//aloc 3B
	printf("p = 0x%p, rezultat = %d\n", p, rez);

	char rez2 = allocHeap2(&p, (unsigned char)a);	//aloc 3B
	printf("p = 0x%p, rezultat = %d\n", p, rez2);

	free(p);
	p = NULL;

	unsigned char dim = 0;
	p = allocStack(&dim);
	printf("Vector alocat in stack functie: ");
	for (unsigned char i = 0; i < dim; i++) {
		printf(" %d ", p[i]);		//uneori s-ar putea sa mearga, dar este gresit 
	}

	return 0;
}