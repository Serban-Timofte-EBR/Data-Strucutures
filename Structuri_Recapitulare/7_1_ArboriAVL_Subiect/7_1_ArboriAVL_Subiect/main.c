#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct {
	int id;
	char* client;
	float suma;
} Rezervare;

typedef struct {
	Rezervare inf;
	struct nodArbAVL* stanga;
	struct nodArbAVL* dreapta;
} nodArbAVL;

void inserareArbore(nodArbAVL** rad, Rezervare rez) {
	if (*rad != NULL) {
		if (rez.id < (*rad)->inf.id) {
			inserareArbore(&(*rad)->stanga, rez);
		}
		else if (rez.id > (*rad)->inf.id) {
			inserareArbore(&(*rad)->dreapta, rez);
		}
	}
	else {
		nodArbAVL* nou = (nodArbAVL*)malloc(sizeof(nodArbAVL));
		nou->inf.id = rez.id;
		nou->inf.suma = rez.suma;
		nou->inf.client = (char*)malloc(strlen(rez.client) + 1);
		strcpy(nou->inf.client, rez.client);

		nou->stanga = NULL;
		nou->dreapta = NULL;

		*rad = nou;
	}
}

void rotire_dreapta(nodArbAVL** rad) {
	nodArbAVL* aux = (*rad)->stanga;
	(*rad)->stanga = aux->dreapta;
	aux->dreapta = *rad;
	*rad = aux;
}

void rotire_stanga(nodArbAVL** rad) {
	nodArbAVL* aux = (*rad)->dreapta;
	(*rad)->dreapta = aux->stanga;
	aux->stanga = *rad;
	*rad = aux;
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

	int inaltS = inaltime(rad->stanga);
	int inaltD = inaltime(rad->dreapta);

	return 1 + maxim(inaltD, inaltS);
}

int getDezechilibru(nodArbAVL* rad) {
	if (rad == NULL) {
		return 0;
	}

	return inaltime(rad->stanga) - inaltime(rad->dreapta);
}

void inserareAVL(nodArbAVL** rad, Rezervare rez) {
	if (*rad != NULL) {
		if (rez.id < (*rad)->inf.id) {
			inserareArbore(&(*rad)->stanga, rez);
		}
		else if (rez.id > (*rad)->inf.id) {
			inserareArbore(&(*rad)->dreapta, rez);
		}
	}
	else {
		nodArbAVL* nou = (nodArbAVL*)malloc(sizeof(nodArbAVL));
		nou->inf.id = rez.id;
		nou->inf.suma = rez.suma;
		nou->inf.client = (char*)malloc(strlen(rez.client) + 1);
		strcpy(nou->inf.client, rez.client);

		nou->stanga = NULL;
		nou->dreapta = NULL;

		*rad = nou;
	}

	int dezechilibru = getDezechilibru(*rad);

	if (dezechilibru == -2) {
		if (getDezechilibru((*rad)->dreapta) == 1) {
			rotire_dreapta(&(*rad)->dreapta);
		}
		rotire_stanga(rad);
	}

	if (dezechilibru == 2) {
		if (getDezechilibru((*rad)->stanga) == -1) {
			rotire_stanga(&(*rad)->stanga);
		}
		rotire_dreapta(rad);
	}
}

void inordine(nodArbAVL* rad) {
	if (rad != NULL) {
		inordine(rad->stanga);
		printf("ID: %d \t Suma: %f \t Client: %s\n",
			rad->inf.id, rad->inf.suma, rad->inf.client);
		inordine(rad->dreapta);
	}
}

void preordine(nodArbAVL* rad) {
	if (rad != NULL) {
		printf("ID: %d \t Suma: %f \t Client: %s\n",
			rad->inf.id, rad->inf.suma, rad->inf.client);
		preordine(rad->stanga);
		preordine(rad->dreapta);
	}
}

float valaoreRezervariClient(nodArbAVL* rad, char* client) {
	float valoare = 0.0;

	if(rad != NULL) {
		if (strcmp(rad->inf.client, client) == 0) {
			valoare += rad->inf.suma;
		}

		valoare += valaoreRezervariClient(rad->stanga, client);
		valoare += valaoreRezervariClient(rad->dreapta, client);
	}

	return valoare;
}

nodArbAVL* findMin(nodArbAVL* rad) {
	nodArbAVL* temp = rad;
	while (temp->stanga != NULL)
	{
		temp = temp->stanga;
	}
	return temp;
}

nodArbAVL* stergereNodCheie(nodArbAVL* rad, int cheie) {
	if (rad == NULL) {
		return rad;
	}

	if (cheie < rad->inf.id) {
		rad->stanga = stergereNodCheie(rad->stanga, cheie);
	}

	else if (cheie > rad->inf.id) {
		rad->dreapta = stergereNodCheie(rad->dreapta, cheie);
	}

	else {
		if (rad->dreapta == NULL) {
			nodArbAVL* temp = rad->stanga;
			free(rad->inf.client);
			free(rad);
			return temp;
		}

		else if (rad->stanga == NULL) {
			nodArbAVL* temp = rad->dreapta;
			free(rad->inf.client);
			free(rad);
			return temp;
		}

		nodArbAVL* temp = findMin(rad->dreapta);
		rad->inf.id = temp->inf.id;
		rad->inf.suma = temp->inf.suma;
		free(rad->inf.client);
		rad->inf.client = (char*)malloc(strlen(temp->inf.client) + 1);
		strcpy(rad->inf.client, temp->inf.client);

		rad->dreapta = stergereNodCheie(rad->dreapta, temp->inf.id);
	}
	return rad;
}

int getCheieMin(nodArbAVL* rad) {
	nodArbAVL* temp = rad;
	while (temp->stanga != NULL)
	{
		temp = temp->stanga;
	}
	return temp->inf.id;
}

nodArbAVL* stergeCheieMin(nodArbAVL* rad) {
	int cheieMin = getCheieMin(rad);
	rad = stergereNodCheie(rad, cheieMin);
	return rad;
}

void calculValoriNivel(nodArbAVL* rad, int nivel, int* vector) {
	if (rad != NULL) {
		vector[nivel]++;
		calculValoriNivel(rad->stanga, nivel + 1, vector);
		calculValoriNivel(rad->dreapta, nivel + 1, vector);
	}
}

int* cerinta5(nodArbAVL* rad) {
	int nrElem = inaltime(rad);
	int* vector = (int*)malloc(nrElem * sizeof(int));
	for (int i = 0; i < nrElem; i++) {
		vector[i] = 0;
	}

	calculValoriNivel(rad, 0, vector);

	return vector;
}

int main() {
	nodArbAVL* rad = NULL;

	FILE* f = fopen("fisier.txt", "r");
	if (f == NULL) {
		printf("Fisierul nu a fost deschis\n");
		return -1;
	}

	int nrNoduri;
	fscanf(f, "%d", &nrNoduri);

	Rezervare rez;
	char buffer[128];

	for (int i = 0; i < nrNoduri; i++) {
		fscanf(f, "%d", &rez.id);

		fscanf(f, "%s", buffer);
		rez.client = (char*)malloc(strlen(buffer) + 1);
		strcpy(rez.client, buffer);

		fscanf(f, "%f", &rez.suma);

		inserareAVL(&rad, rez);

		free(rez.client);
	}
	fclose(f);

	printf("Arborele INORDINE:\n");
	inordine(rad);

	printf("\nArborele PREORDINE:\n");
	preordine(rad);

	float valoareRezervari = valaoreRezervariClient(rad, "Sebastian");
	printf("\nValoarea rezervarilor clientului este de %5.2f\n", valoareRezervari);

	rad = stergeCheieMin(rad);
	printf("\nArborele dupa stergere\n");
	inordine(rad);

	int* vector = cerinta5(rad);
	printf("\nVectorul este:\n");
	for (int i = 0; i < inaltime(rad); i++) {
		printf("%d ", vector[i]);
	}

	return 0;
}