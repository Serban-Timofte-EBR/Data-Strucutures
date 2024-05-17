#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

typedef struct {
	int prioritate;
	char* denumire;
	float efortOre;
} task;

typedef struct {
	task* vect;
	int nrElem;
} heap;

void filtrare(heap h, int index) {
	int indexRad = index;
	int indexS = 2 * index + 1;
	int indexD = 2 * index + 2;

	if (indexS < h.nrElem && h.vect[indexS].prioritate > h.vect[indexRad].prioritate) {
		indexRad = indexS;
	}

	if (indexD < h.nrElem && h.vect[indexD].prioritate > h.vect[indexRad].prioritate) {
		indexRad = indexD;
	}

	if (index != indexRad) {
		task aux = h.vect[index];
		h.vect[index] = h.vect[indexRad];
		h.vect[indexRad] = aux;

		filtrare(h, indexRad);
	}
}

void afisareHeap(heap h) {
	for (int i = 0; i < h.nrElem; i++) {
		printf("Prioritate = %d \t Efort = %f \t Denumire = %s\n", h.vect[i].prioritate,
			h.vect[i].efortOre, h.vect[i].denumire);
	}
}

heap inserareHeap(heap h, task taskNou) {
	h.nrElem += 1;
	task* vectorNou = (task*)malloc(h.nrElem * sizeof(task));
	for (int i = 0; i < h.nrElem - 1; i++) {
		vectorNou[i] = h.vect[i];
	}
	vectorNou[h.nrElem - 1] = taskNou;
	free(h.vect);
	h.vect = vectorNou;
	for (int i = (h.nrElem - 1) / 2; i >= 0; i--) {
		filtrare(h, i);
	}
	return h;
}

heap extragere(heap h, task* extras) {
	task* vectorNou = (task*)malloc((h.nrElem - 1) * sizeof(task));

	task aux = h.vect[0];
	h.vect[0] = h.vect[h.nrElem - 1];
	h.vect[h.nrElem - 1] = aux;

	*extras = h.vect[h.nrElem - 1];
	h.nrElem--;

	for (int i = 0; i < h.nrElem; i++) {
		vectorNou[i] = h.vect[i];
	}
	free(h.vect);
	h.vect = vectorNou;

	for (int i = (h.nrElem - 1) / 2; i >= 0; i--) {
		filtrare(h, i);
	}

	return h;
}

void dezalocareheap(heap h) {
	for (int i = 0; i < h.nrElem; i++)
	{
		free(h.vect[i].denumire);
	}
	free(h.vect);
}

int main() {
	heap h;

	FILE* f = fopen("fisier.txt", "r");
	if (f == NULL) {
		printf("Fisierul nu s-a deschis!");
		return -1;
	}

	fscanf(f, "%d", &h.nrElem);
	h.vect = (task*)malloc(h.nrElem * sizeof(task));

	char buffer[128];
	for (int i = 0; i < h.nrElem; i++) {
		fscanf(f, "%d", &h.vect[i].prioritate);

		fscanf(f, "%s", buffer);
		h.vect[i].denumire = (char*)malloc(strlen(buffer) + 1);
		strcpy(h.vect[i].denumire, buffer);

		fscanf(f, "%f", &h.vect[i].efortOre);
	}
	fclose(f);

	//filtrarea vectorului pentru a deveni heap
	for (int i = (h.nrElem - 1) / 2; i >= 0; i--) {
		filtrare(h, i);
	}

	printf("Structura HEAP din fisier:\n");
	afisareHeap(h);

	task nou;
	printf("\nPrioritatea noului task = ");
	scanf("%d", &nou.prioritate);
	printf("\nDenumirea noului task = ");
	scanf("%s", buffer);
	nou.denumire = (char*)malloc(strlen(buffer) + 1);
	strcpy(nou.denumire, buffer);
	printf("\nEfortul noului taks = ");
	scanf("%f", &nou.efortOre);

	h = inserareHeap(h, nou);

	printf("\nStructura HEAP dupa inserare\n");
	afisareHeap(h);

	task extras;
	h = extragere(h, &extras);
	printf("\nTask extras: \nPrioritate = %d, Denumire = %s, Efort = %5.2f",
		extras.prioritate, extras.denumire, extras.efortOre);
	afisareHeap(h);

	task* vectorSortat = (task*)malloc(h.nrElem * sizeof(task));
	int nr = h.nrElem;
	for (int i = 0; i < nr; i++) {
		h = extragere(h, &extras);
		vectorSortat[i] = extras;
	}

	printf("\nVectorul sortat:");
	for (int i = 0; i < nr; i++)
		printf("\nPrioritate = %d, Denumire = %s, Efort = %5.2f",
			vectorSortat[i].prioritate,
			vectorSortat[i].denumire,
			vectorSortat[i].efortOre);

	for (int i = 0; i < h.nrElem; i++)
		free(vectorSortat[i].denumire);
	free(vectorSortat);

	dezalocareheap(h);

	return 0;
}