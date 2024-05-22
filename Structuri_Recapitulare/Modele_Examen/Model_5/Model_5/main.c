#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct {
	unsigned int id;
	char* denumireHotel;
	unsigned char nrCamere;
	char* numeClient;
	float suma;
} Rezervare;

typedef struct {
	Rezervare inf;
	struct nodArbBin* stanga;
	struct nodArbBin* dreapta;
} nodArbBin;

nodArbBin* creareNod(Rezervare rez) {
	nodArbBin* nou = (nodArbBin*)malloc(sizeof(nodArbBin));
	nou->inf.id = rez.id;
	nou->inf.nrCamere = rez.nrCamere;
	nou->inf.suma = rez.suma;
	nou->inf.denumireHotel = (char*)malloc(strlen(rez.denumireHotel) + 1);
	strcpy(nou->inf.denumireHotel, rez.denumireHotel);
	nou->inf.numeClient = (char*)malloc(strlen(rez.numeClient) + 1);
	strcpy(nou->inf.numeClient, rez.numeClient);
	nou->stanga = NULL;
	nou->dreapta = NULL;

	return nou;
}

nodArbBin* inserareArbore(nodArbBin* rad, Rezervare rez) {
	if (rad != NULL) {
		if (rez.id < rad->inf.id) {
			rad->stanga = inserareArbore(rad->stanga, rez);
			return rad;
		}

		else if (rez.id > rad->inf.id) {
			rad->dreapta = inserareArbore(rad->dreapta, rez);
			return rad;
		}

		else {
			return rad;
		}
	}
	else {
		return creareNod(rez);
	}
}

void preordine(nodArbBin* rad) {
	if (rad != NULL) {
		printf("ID: %d, \t Client: %s\n",
			rad->inf.id, rad->inf.numeClient);
		preordine(rad->stanga);
		preordine(rad->dreapta);
	}
}

void inordine(nodArbBin* rad) {
	if (rad != NULL) {
		inordine(rad->stanga);
		printf("ID: %d, \t Client: %s\n",
			rad->inf.id, rad->inf.numeClient);
		inordine(rad->dreapta);
	}
}

void valoareRezervariClient(nodArbBin* rad, char* client, float* val) {
	if (rad != NULL) {
		if (strcmp(rad->inf.numeClient, client) == 0) {
			*val += rad->inf.suma;
		}

		valoareRezervariClient(rad->stanga, client, val);
		valoareRezervariClient(rad->dreapta, client, val);
	}
}

nodArbBin* findMin(nodArbBin* rad) {
	nodArbBin* temp = rad;
	while (temp->stanga != NULL)
	{
		temp = temp->stanga;
	}

	return temp;
}

nodArbBin* stergereNod(nodArbBin* rad, int id) {
	if (rad == NULL) {
		return rad;
	}

	if (id < rad->inf.id) {
		rad->stanga = stergereNod(rad->stanga, id);
	}

	else if (id > rad->inf.id) {
		rad->dreapta = stergereNod(rad->dreapta, id);
	}

	else {
		if (rad->dreapta == NULL) {
			nodArbBin* temp = rad->stanga;
			free(rad->inf.denumireHotel);
			free(rad->inf.numeClient);
			free(rad);
			return temp;
		}

		else if (rad->stanga == NULL) {
			nodArbBin* temp = rad->dreapta;
			free(rad->inf.denumireHotel);
			free(rad->inf.numeClient);
			free(rad);
			return temp;
		}

		nodArbBin* temp = findMin(rad->dreapta);
		rad->inf.id = temp->inf.id;
		rad->inf.suma = temp->inf.suma;
		rad->inf.nrCamere = temp->inf.nrCamere;
		free(rad->inf.numeClient);
		free(rad->inf.denumireHotel);
		rad->inf.numeClient = (char*)malloc(strlen(temp->inf.numeClient) + 1);
		strcpy(rad->inf.numeClient, temp->inf.numeClient);
		rad->inf.denumireHotel = (char*)malloc(strlen(temp->inf.denumireHotel) + 1);
		strcpy(rad->inf.denumireHotel, temp->inf.denumireHotel);
		rad->dreapta = stergereNod(rad->dreapta, temp->inf.id);
	}

	return rad;
}

int getCheieMinima(nodArbBin* rad) {
	nodArbBin* minNod = findMin(rad);
	return minNod->inf.id;
}

nodArbBin* stergeNodCheiaMinima(nodArbBin* rad) {
	int cheie = getCheieMinima(rad);
	rad = stergereNod(rad, cheie);
	return rad;
}

int max(int a, int b) {
	int max = a;
	if (b > a) {
		max = b;
	}
	return max;
}

int inaltime(nodArbBin* rad) {
	if (rad == NULL) {
		return 0;
	}

	int inaltS = inaltime(rad->stanga);
	int inaltD = inaltime(rad->dreapta);

	return 1 + max(inaltS, inaltD);
}

void numarNoduriNivel(nodArbBin* rad, int nivel, int* vector) {
	if (rad != NULL) {
		vector[nivel]++;
		numarNoduriNivel(rad->stanga, nivel + 1, vector);
		numarNoduriNivel(rad->dreapta, nivel + 1, vector);
	}
}

int* vectorCuNoduriPeNivel(nodArbBin* rad) {
	int inaltimeArbore = inaltime(rad);
	int* vector = (int*)malloc(inaltimeArbore * sizeof(int));
	for (int i = 0; i < inaltimeArbore; i++) {
		vector[i] = 0;
	}
	numarNoduriNivel(rad, 0, vector);
	return vector;
}

int main() {
	nodArbBin* rad = NULL;

	FILE* f = fopen("fisier.txt", "r");
	if (f == NULL) {
		printf("Fisierul nu a fost deschis\n");
		return -1;
	}

	int nrRezervari;
	Rezervare rez;
	char buffer[128];

	fscanf(f, "%d", &nrRezervari);
	for (int i = 0; i < nrRezervari; i++)
	{
		fscanf(f, "%d", &rez.id);

		fscanf(f, "%s", buffer);
		rez.denumireHotel = (char*)malloc(strlen(buffer) + 1);
		strcpy(rez.denumireHotel, buffer);

		fscanf(f, "%d", &rez.nrCamere);

		fscanf(f, "%s", buffer);
		rez.numeClient = (char*)malloc(strlen(buffer) + 1);
		strcpy(rez.numeClient, buffer);

		fscanf(f, "%f", &rez.suma);

		rad = inserareArbore(rad, rez);

		free(rez.denumireHotel);
		free(rez.numeClient);
	}
	fclose(f);

	printf("Arborele din fisier INORDINE:\n");
	inordine(rad);

	printf("\nArborele din fisier PREORDINE:\n");
	preordine(rad);

	float valoareRezervari = 0;
	valoareRezervariClient(rad, "Ionel", &valoareRezervari);
	printf("\nRezevarile clientului valoareaza: %f\n", valoareRezervari);

	valoareRezervari = 0;
	valoareRezervariClient(rad, "Maria", &valoareRezervari);
	printf("\nRezevarile clientului valoareaza: %f\n", valoareRezervari);

	valoareRezervari = 0;
	valoareRezervariClient(rad, "Andrei", &valoareRezervari);
	printf("\nRezevarile clientului valoareaza: %f\n", valoareRezervari);

	rad = stergeNodCheiaMinima(rad);
	printf("\nArborele dupa stergere:\n");
	inordine(rad);

	int* vector = vectorCuNoduriPeNivel(rad);
	printf("\nNumarul de noduri pe fiecare nivel:\n");
	for (int i = 0; i < inaltime(rad); i++) {
		printf("Nivel %d: %d noduri\n", i, vector[i]);
	}

	return 0;
}