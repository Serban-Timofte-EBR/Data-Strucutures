#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

/*
	Arbore binar de cautare:
		- Orice nod are o cheie/id
		- Copilul stanga are o cheie mai mica fata de parinte 
		- Copilul dreapta are o cheie mai mare fata de parinte


*/

struct ContBancar
{
	char iban[25]; // 24 bytes pt iban + 1 byte terminator string
	char moneda[4]; // 3 bytes pt cod moneda + 1 byte terminator string
	char* titular;
	float sold;
};

struct NodABC {
	unsigned short int cheie;
	struct ContBancar data;
	struct NodABC* stanga;
	struct NodABC* dreapta;
};

typedef struct ContBancar ContBancar;
typedef struct NodABC NodABC;

NodABC* inserare_nod_arbore(NodABC* r, ContBancar cont, unsigned short int key, unsigned char* flag) {
	if (r != NULL) {
		if
	}
	else {
		//locul unde nodul nou se insereaza 
			// inserarea se face doar in frunza in stanga sau in dreapta
		NodABC* nou = (NodABC*)malloc(sizeof(NodABC));
		nou->cheie = nou;
		nou->data = cont;
		nou->stanga = NULL;
		nou->dreapta = NULL;

		*flag = 1;	//flag setat cu SUCCES
		r = nou;
	}

	return r;
}

int main() {
	FILE* f = fopen("Conturi.txt", "r");
	NodABC* root = NULL;
	unsigned short int key;

	char nume_buff[128];
	struct ContBancar contB;
	fscanf(f, "%hu", &key);	//unsigned
	while (key != 0)
	{
		fscanf(f, "%[^\n]", nume_buff);
		contB.titular = (char*)malloc(strlen(nume_buff) + 1);
		strcpy(contB.titular, nume_buff);

		fscanf(f, "%s", contB.moneda);
		fscanf(f, "%f", &contB.sold);
		
		//inserare in arbore binar de cautare
		unsigned char inserat = 0;
		root = inserare_nod_arbore(root, contB, key, &inserat);
		if (inserat != 0) {
			//inserarea a reusit
			contB.titular = NULL; 
			contB.iban[0] = 0;
		}
		else {
			free(contB.titular); //dezalocare HEAP pentru titular dupa inserare esuata
		}

		//reinitializare cheie pentru a verifica existenta urmatorului contBancar din fisier
		key = 0;

		fscanf(f, "%s\n", &key);
	}

	return 0;
}