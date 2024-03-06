#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

int main() {
	printf("Hello, World!\n");

	signed char a;
	a = -65; // a = 'A';
	unsigned char b = 0x42;

	printf("\'%c\' = %d, \'%c\' = 0x%X\n", a, a, a, (unsigned char)a);
	printf("\'%c\' = %d, \'%c\' = 0x%X\n", b, b, b, b);
	printf("\'%c\' = %d\n", a, (unsigned char)a);

	a += b;

	printf("\'%c\' = %d\n", a, a);

	printf("String(a) = %s\n", &a);
	char* pa;
	pa = &a;
	printf("String(a) = %s\n", pa);
	*pa = b;

	pa = (char*)malloc(sizeof(a) + 1);
	*pa = a;
	*(pa + 1) = 0; // pa[1] = 0;
	printf("String(a) = %s\n", pa);

	//free(pointer) nu il face NULL, asadar facem 
	free(pa);
	pa = NULL;

	//Continuarea Seminarului 2 in Seminarul 3

	char v[] = { 0x0a, 0x0b, 0x0c, 0x0d, 0x0e };	//sizeof(char) * sizeof(v)
	printf("%d", sizeof(v));

	pa = v;		// pa primeste adresa de inceput a lui v (pa se afla la o adresa de compilare si contine prima adresa din v )
	//pa = &v;	notatie corecta
				// practic, v are prima adresa din vectorul declarat

	// v este reprezentant in memorie contiguu ( fizic si logic ) - sunt la adrese consecutive de memorie
	// *pa accesez primul element din v prin pa
	// *(pa + 1 ) accesez v[1]

	printf("Elemente vector: ");
	for (unsigned char i = 0; i < sizeof(v); i++) {		//pot folosi sizeof() pentru ca am folosit initializarea direct cu nr de elemente. daca aveam v[20] dar aveam 5 elemente din lista nu mai mergea ok
		printf(" %d", pa[i]);
	}
	printf("\n");

	//pa[-1] = 0;	// se acceseaza adresa *(pa-1)

	pa = (char*)malloc((sizeof(v) * 2) * sizeof(char));

	for (unsigned char i = 0; i < sizeof(v) * 2; i++) {
		unsigned char j = i % sizeof(v);
		pa[i] = v[j] * 10 + 1;
	}

	printf("Elementele vectorului din HEAP: ");
	for (unsigned char i = 0; i < sizeof(v) * 2; i++) {
		printf(" %d", pa[i]);
	}
	printf("\n");

	free(pa);
	pa = NULL;

	return 0;
}