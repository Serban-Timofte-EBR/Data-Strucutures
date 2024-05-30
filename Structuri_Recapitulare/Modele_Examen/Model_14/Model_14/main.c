#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct {
	int numarVagon;
	char* firma;
	int nrBileteVandute;
	int capacitateVagon;
} Vagon;

typedef struct {
	Vagon inf;
	struct nodLD* next;
	struct nodLD* prev;
} nodLD;

typedef struct {
	Vagon* vector;
	int nrElem;
} heap;

nodLD* inserareLD(nodLD* prim, nodLD** ultim, Vagon vag) {
	nodLD* nou = (nodLD*)malloc(sizeof(nodLD));
	nou->inf.numarVagon = vag.numarVagon;
	nou->inf.nrBileteVandute = vag.nrBileteVandute;
	nou->inf.capacitateVagon = vag.capacitateVagon;
	nou->inf.firma = (char*)malloc(strlen(vag.firma) + 1);
	strcpy(nou->inf.firma, vag.firma);
	nou->next = NULL;
	nou->prev = NULL;

	if (prim == NULL) {
		prim = nou;
		*ultim = nou;
	}
	else {
		(*ultim)->next = nou;
		nou->prev = *ultim;
		*ultim = nou;
	}

	return prim;
}

void afisareCapCoada(nodLD* prim) {
	nodLD* temp = prim;
	while (temp != NULL)
	{
		printf("Nr: %d, Vandute: %d, Capacitate: %d, Firma: %s\n",
			temp->inf.numarVagon, temp->inf.nrBileteVandute, temp->inf.capacitateVagon, temp->inf.firma);
		temp = temp->next;
	}
}

void afisareCoadaCap(nodLD* ultim) {
	nodLD* temp = ultim;
	while (temp != NULL)
	{
		printf("Nr: %d, Vandute: %d, Capacitate: %d, Firma: %s\n",
			temp->inf.numarVagon, temp->inf.nrBileteVandute, temp->inf.capacitateVagon, temp->inf.firma);
		temp = temp->prev;
	}
}

int getNrMinBilete(nodLD* prim) {
	int nrMin = prim->inf.nrBileteVandute;
	nodLD* temp = prim;
	while (temp != NULL)
	{
		if (temp->inf.nrBileteVandute < nrMin) {
			nrMin = temp->inf.nrBileteVandute;
		}
		temp = temp->next;
	}
	return nrMin;
}

nodLD* stergereVagonNrMinBilete(nodLD* prim, nodLD** ultim) {
	int nrMinBilete = getNrMinBilete(prim);

	nodLD* temp = prim;
	while (temp != NULL)
	{
		if (temp->inf.nrBileteVandute == nrMinBilete) {
			nodLD* deleteMe = temp;
			if (temp == prim) {
				prim = prim->next;
				prim->prev = NULL;
			}
			else if (temp == *ultim) {
				nodLD* ant = (*ultim)->prev;
				ant->next = NULL;
				*ultim = ant;
			}
			else {
				nodLD* ant = temp->prev;
				nodLD* urm = temp->next;
				ant->next = urm;
				urm->prev = ant;
			}
			temp = temp->next;
			free(deleteMe->inf.firma);
			free(deleteMe);
		}
		else {
			temp = temp->next;
		}
	}
	return prim;
}

int countVagoane(nodLD* prim) {
	int counter = 0;
	nodLD* temp = prim;
	while (temp != NULL)
	{
		counter++;
		temp = temp->next;
	}
	return counter;
}

void filtrare(heap h, int index) {
	int indexRad = index;
	int indexS = 2 * index + 1;
	int indexD = 2 * index + 2;

	if (indexS < h.nrElem && h.vector[indexS].nrBileteVandute / (float)h.vector[indexS].capacitateVagon <
		h.vector[indexRad].nrBileteVandute / (float)h.vector[indexRad].capacitateVagon) {
		indexRad = indexS;
	}

	if (indexD < h.nrElem && h.vector[indexD].nrBileteVandute / (float)h.vector[indexD].capacitateVagon <
		h.vector[indexRad].nrBileteVandute / (float)h.vector[indexRad].capacitateVagon) {
		indexRad = indexD;
	}

	if (index != indexRad) {
		Vagon aux = h.vector[index];
		h.vector[index] = h.vector[indexRad];
		h.vector[indexRad] = aux;

		filtrare(h, indexRad);
	}
}

heap creareHeap(nodLD* prim) {
	heap h;
	h.nrElem = countVagoane(prim);
	h.vector = (Vagon*)malloc(h.nrElem * sizeof(Vagon));

	int index = 0;
	nodLD* temp = prim;
	while (temp != NULL)
	{
		h.vector[index].numarVagon = temp->inf.numarVagon;
		h.vector[index].nrBileteVandute = temp->inf.numarVagon;
		h.vector[index].capacitateVagon = temp->inf.capacitateVagon;
		h.vector[index].firma = (char*)malloc(strlen(temp->inf.firma) + 1);
		strcpy(h.vector[index].firma, temp->inf.firma);
		index++;
		temp = temp->next;
	}

	for (int i = (h.nrElem - 1) / 2; i >= 0; i--) {
		filtrare(h, i);
	}

	return h;
}

heap modificaNrBileteVandute(heap h, int nrVagon, int nrBileteVanduteNou) {
	for (int i = 0; i < h.nrElem; i++) {
		if (h.vector[i].numarVagon == nrVagon) {
			h.vector[i].nrBileteVandute = nrBileteVanduteNou;
		}
	}

	for (int i = (h.nrElem - 1) / 2; i >= 0; i--) {
		filtrare(h, i);
	}

	return h;
}

void afisareHeap(heap h) {
	for (int i = 0; i < h.nrElem; i++) {
		printf("Grad Ocupare: %5.2f, Nr: %d, Vandute: %d, Capacitate: %d, Firma: %s\n",
			h.vector[i].nrBileteVandute / (float)h.vector[i].capacitateVagon,
			h.vector[i].numarVagon, h.vector[i].nrBileteVandute, h.vector[i].capacitateVagon,
			h.vector[i].firma);
	}
}

int main() {
	nodLD* prim = NULL;
	nodLD* ultim = NULL;

	FILE* f = fopen("fisier.txt", "r");

	Vagon vag;
	char buffer[128];

	while (fscanf(f, "%d", &vag.numarVagon) == 1)
	{
		fscanf(f, " %[^\n]", buffer);
		vag.firma = (char*)malloc(strlen(buffer) + 1);
		strcpy(vag.firma, buffer);

		fscanf(f, "%d", &vag.nrBileteVandute);
		fscanf(f, "%d", &vag.capacitateVagon);

		/*printf("Nr: %d, Vandute: %d, Capacitate: %d, Firma: %s\n",
			vag.numarVagon, vag.nrBileteVandute, vag.capacitateVagon, vag.firma);*/
		prim = inserareLD(prim, &ultim, vag);

		free(vag.firma);
	}
	fclose(f);

	printf("Lista din fisier CAP -> COADA:\n");
	afisareCapCoada(prim);

	printf("\nLista din fisier COADA -> CAP:\n");
	afisareCoadaCap(ultim);

	prim = stergereVagonNrMinBilete(prim, &ultim);
	printf("\nLista dupa stergere CAP -> COADA:\n");
	afisareCapCoada(prim);

	printf("\nLista dupa stergere COADA -> CAP:\n");
	afisareCoadaCap(ultim);

	/*int nrVagoane = countVagoane(prim);
	printf("\n%d\n", nrVagoane);*/

	heap h = creareHeap(prim);
	printf("\nStructura HEAP:\n");
	afisareHeap(h);

	h = modificaNrBileteVandute(h, 21, 1);
	printf("\nStructura HEAP dupa modficare:\n");
	afisareHeap(h);

	return 0;
}