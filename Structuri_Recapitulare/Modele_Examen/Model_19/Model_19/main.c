#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct {
	int cod;
	char* nume; 
	char* descriere;
	float varsta;
	int prioritate;
} Pacient;

typedef struct {
	Pacient* vector;
	int nrElem;
} heap;

void filtrare(heap h, int index) {
	int indexRad = index;
	int indexS = 2 * index + 1;
	int indexD = 2 * index + 2;

	if (indexS < h.nrElem && h.vector[indexS].prioritate > h.vector[indexRad].prioritate) {
		indexRad = indexS;
	}

	if (indexD < h.nrElem && h.vector[indexD].prioritate > h.vector[indexRad].prioritate) {
		indexRad = indexD;
	}

	if (index != indexRad) {
		Pacient aux = h.vector[index];
		h.vector[index] = h.vector[indexRad];
		h.vector[indexRad] = aux;

		filtrare(h, indexRad);
	}
}

heap inserareHeap(heap h, Pacient pac) {
	Pacient* vectorNou = (Pacient*)malloc((h.nrElem + 1) * sizeof(Pacient));
	for (int i = 0; i < h.nrElem; i++) {
		vectorNou[i] = h.vector[i];
	}
	vectorNou[h.nrElem].cod = pac.cod;
	vectorNou[h.nrElem].prioritate = pac.prioritate;
	vectorNou[h.nrElem].varsta = pac.varsta;
	vectorNou[h.nrElem].descriere = (char*)malloc(strlen(pac.descriere) + 1);
	strcpy(vectorNou[h.nrElem].descriere, pac.descriere);
	vectorNou[h.nrElem].nume = (char*)malloc(strlen(pac.nume) + 1);
	strcpy(vectorNou[h.nrElem].nume, pac.nume);
	h.nrElem++;

	free(h.vector);
	h.vector = vectorNou;

	for (int i = (h.nrElem - 1) / 2; i >= 0; i--) {
		filtrare(h, i);
	}

	return h;
}

heap inserareHeapPacient(heap h, int cod, char* nume, char* descriere, float varsta, int categorie) {
	Pacient pac;
	pac.cod = cod;
	pac.varsta = varsta;
	pac.nume = (char*)malloc(strlen(nume) + 1);
	strcpy(pac.nume, nume);
	pac.descriere = (char*)malloc(strlen(descriere) + 1);
	strcpy(pac.descriere, descriere);
	pac.prioritate = 0;

	if (categorie == 1) {
		pac.prioritate = 5;
	}
	else if (categorie == 2) {
		pac.prioritate = 4;
	}
	else if (categorie == 3) {
		pac.prioritate = 1;
	}

	h = inserareHeap(h, pac);

	return h;
}

void afisareHeap(heap h) {
	for (int i = 0; i < h.nrElem; i++) {
		printf("Prioritate: %d, ID: %d, Varsta: %5.2f, Nume: %s, Descriere: %s\n",
			h.vector[i].prioritate, h.vector[i].cod, h.vector[i].varsta, h.vector[i].nume, h.vector[i].descriere);
	}
}

int counterPrioritate(heap h) {
	int counter = 0;
	for (int i = 0; i < h.nrElem; i++) {
		if (h.vector[i].prioritate == 5) {
			counter++;
		}
	}
	return counter;
}

heap cresterePrioritati(heap h) {
	for (int i = 0; i < h.nrElem; i++) {
		if (h.vector[i].prioritate > 1) {
			if (h.vector[i].prioritate != 5 && h.vector[i].prioritate != 4) {
				h.vector[i].prioritate++;
			}
		}
	}

	for (int i = (h.nrElem - 1) / 2; i >= 0; i--) {
		filtrare(h, i);
	}

	return h;
}

void generareRaport(heap h) {
	FILE* f = fopen("raport.txt", "w");

	int* procesat = (int*)malloc(h.nrElem * sizeof(int));
	for (int i = 0; i < h.nrElem; i++) {
		procesat[i] = 0;
	}

	for (int i = 0; i < h.nrElem; i++) {
		if (procesat[i] == 0) {
			int counterSimptom = 1;
			procesat[i] = 1;

			for (int j = i + 1; j < h.nrElem; j++) {
				if (strcmp(h.vector[i].descriere, h.vector[j].descriere) == 0) {
					counterSimptom++;
					procesat[j] = 1;
				}
			}

			fprintf(f, "%s: %d pacineti\n", h.vector[i].descriere, counterSimptom);
		}
	}
	fclose(f);
}

Pacient stergereDinHeap(heap* h) {
	Pacient aux = (*h).vector[0];
	(*h).vector[0] = (*h).vector[(*h).nrElem - 1];
	(*h).nrElem--;

	Pacient* vectorNou = (Pacient*)malloc((*h).nrElem * sizeof(Pacient));
	for (int i = 0; i < (*h).nrElem; i++) {
		vectorNou[i] = (*h).vector[i];
	}
	free((*h).vector);
	(*h).vector = vectorNou;

	for (int i = ((*h).nrElem - 1 / 2); i >= 0; i--) {
		filtrare(*h, i);
	}

	return aux;
}

int main() {
	heap h;
	h.nrElem = 0;
	h.vector = (Pacient*)malloc(sizeof(Pacient));

	FILE* f = fopen("fisier.txt", "r");

	Pacient pac;
	char buffer[128];

	while (fscanf(f, "%d", &pac.cod) == 1) 
	{
		fscanf(f, " %[^\n]", buffer);
		pac.nume = (char*)malloc(strlen(buffer) + 1);
		strcpy(pac.nume, buffer);

		fscanf(f, " %[^\n]", buffer);
		pac.descriere = (char*)malloc(strlen(buffer) + 1);
		strcpy(pac.descriere, buffer);

		fscanf(f, "%f", &pac.varsta);
		fscanf(f, "%d", &pac.prioritate);

		/*printf("Prioritate: %d, ID: %d, Varsta: %5.2f, Nume: %s, Descriere: %s\n",
			pac.prioritate, pac.cod, pac.varsta, pac.nume, pac.descriere);*/
		h = inserareHeap(h, pac);

		free(pac.nume);
		free(pac.descriere);
	}
	fclose(f);

	printf("HEAP-ul din fisier:\n");
	afisareHeap(h);

	h = inserareHeapPacient(h, 69, "Gabriel Popescu", "Simptome de COVID-19", 21, 3);
	h = inserareHeapPacient(h, 98, "Ionela Ionescu", "Stari de greata", 31, 1);
	printf("\nHEAP-ul dupa inserare:\n");
	afisareHeap(h);

	int nrPacPrior5 = counterPrioritate(h);
	printf("\nNumarul de gravide si de copii cu varsta de sub 1 an este: %d\n", nrPacPrior5);

	h = cresterePrioritati(h);
	printf("\nHEAP-ul dupa modificare:\n");
	afisareHeap(h);

	generareRaport(h);

	Pacient pac2 = stergereDinHeap(&h);
	printf("\nHEAP-ul dupa stergere:\n");
	afisareHeap(h);
	printf("\nPacientul sters este:\n");
	printf("\tPrioritate: %d, ID: %d, Varsta: %5.2f, Nume: %s, Descriere: %s\n",
		pac2.prioritate, pac2.cod, pac2.varsta, pac2.nume, pac2.descriere);

	return 0;
}