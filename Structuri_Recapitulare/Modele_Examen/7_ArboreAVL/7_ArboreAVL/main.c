#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct {
	int id;
	char* nume;
} Cofetarie;

typedef struct {
	Cofetarie info;
	struct nodArbAVL* stanga;
	struct nodArbAVL* dreapta;
} nodArbAVL;

Cofetarie initCofetarie(int id, char* nume) {
	Cofetarie cof;
	cof.id = id;
	cof.nume = (char*)malloc(strlen(nume) + 1);
	strcpy(cof.nume, nume);
	return cof;
}

void afisareCofetarie(Cofetarie c)
{
	printf("\n Cofetaria %s are id-ul %d", c.nume, c.id);
}

void inserareArbore(nodArbAVL** rad, Cofetarie cof) {
	if (*rad != NULL) {
		if (cof.id < (*rad)->info.id) {
			inserareArbore(&(*rad)->stanga, cof);
		}
		else if (cof.id > (*rad)->info.id) {
			inserareArbore(&(*rad)->dreapta, cof);
		}
	}
	else {
		nodArbAVL* nou = (nodArbAVL*)malloc(sizeof(nodArbAVL));
		nou->info = cof;
		nou->dreapta = NULL;
		nou->stanga = NULL;
		*rad = nou;
	}
}

void inordine(nodArbAVL* rad) {
	if (rad != NULL) {
		inordine(rad->stanga);
		afisareCofetarie(rad->info);
		inordine(rad->dreapta);
	}
}

void preordine(nodArbAVL* rad) {
	if (rad != NULL) {
		afisareCofetarie(rad->info);
		preordine(rad->stanga);
		preordine(rad->dreapta);
	}
}

int maxim(int a, int b) {
	int max = a;
	if (b > a) {
		max = b;
	}
	return max;
}

int inaltime(nodArbAVL* rad) {
	if (rad == NULL) {
		return 0;
	}

	int inaltSt = inaltime(rad->stanga);
	int inaltDr = inaltime(rad->dreapta);

	return 1 + maxim(inaltDr, inaltSt);
}

void rotire_stanga(nodArbAVL** rad) {
	nodArbAVL* aux = (*rad)->dreapta;
	(*rad)->dreapta = aux->stanga;
	aux->stanga = *rad;
	*rad = aux;
}

void rotire_dreapta(nodArbAVL** rad) {
	nodArbAVL* aux = (*rad)->stanga;
	(*rad)->stanga = aux->dreapta;
	aux->dreapta = *rad;
	*rad = aux;
}

int calculDezechilibru(nodArbAVL* rad) {
	if (rad == NULL) {
		return 0;
	}

	return inaltime(rad->stanga) - inaltime(rad->dreapta);
}

void inserareAVL(Cofetarie c, nodArbAVL** rad) {
	if (*rad == NULL) {
		nodArbAVL* nou = (nodArbAVL*)malloc(sizeof(nodArbAVL));
		nou->info = c;
		nou->dreapta = NULL;
		nou->stanga = NULL;
		*rad = nou;
	}
	else {
		if (c.id < (*rad)->info.id) {
			inserareArbore(&(*rad)->stanga, c);
		}
		else {
			inserareArbore(&(*rad)->dreapta, c);
		}
	}

	int dezechilibru = calculDezechilibru(*rad);

	if (dezechilibru == -2) {
		if (calculDezechilibru((*rad)->dreapta) == 1) {
			rotire_dreapta(&(*rad)->dreapta);
		}
		rotire_stanga(rad);
	}

	if (dezechilibru == 2) {
		if (calculDezechilibru((*rad)->stanga) == 1) {
			rotire_stanga(&(*rad)->stanga);
		}
		rotire_dreapta(rad);
	}
}

int main() {
	nodArbAVL* radacina = NULL;
	inserareAVL(initCofetarie(5, "Ana"), &radacina);
	inserareAVL(initCofetarie(4, "Amalia"), &radacina);
	inserareAVL(initCofetarie(7, "Andreea"), &radacina);
	inserareAVL(initCofetarie(6, "Ina"), &radacina);
	inserareAVL(initCofetarie(9, "Raluca"), &radacina);
	inserareAVL(initCofetarie(8, "Anna"), &radacina);

	inordine(radacina);
	printf("\n------------------------------------------------\n");
	preordine(radacina);
	return 0;
}