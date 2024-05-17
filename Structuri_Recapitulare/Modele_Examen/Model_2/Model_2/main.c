#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

typedef struct {
	int numarVagon;
	char* firmaTransport;
	int numarBileteValabile;
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

nodLD* inserareLD(nodLD* prim, nodLD** ultim, Vagon vagon) {
	nodLD* nou = (nodLD*)malloc(sizeof(nodLD));
	nou->inf.capacitateVagon = vagon.capacitateVagon;
	nou->inf.numarBileteValabile = vagon.numarBileteValabile;
	nou->inf.numarVagon = vagon.numarVagon;
	nou->inf.firmaTransport = (char*)malloc(strlen(vagon.firmaTransport) + 1);
	strcpy(nou->inf.firmaTransport, vagon.firmaTransport);
	nou->next = NULL;
	nou->prev = NULL;

	if (prim == NULL) {
		prim = nou;
		*ultim = nou;
	}
	else {
		nodLD* temp = prim;
		while (temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = nou;
		nou->prev = temp;
		*ultim = nou;
	}

	return prim;
}

void afisareCapCoada(nodLD* prim) {
	nodLD* temp = prim;
	while (temp != NULL) {
		printf("Nr. vagon = %d \t Nr. bilete = %d \t Capacitate = %d \t Firma = %s\n", 
			temp->inf.numarVagon, temp->inf.numarBileteValabile, temp->inf.capacitateVagon, temp->inf.firmaTransport);
		temp = temp->next;
	}
}

void afisareCoadaCap(nodLD* ultim) {
	nodLD* temp = ultim;
	while (temp != NULL) {
		printf("Nr. vagon = %d \t Nr. bilete = %d \t Capacitate = %d \t Firma = %s\n",
			temp->inf.numarVagon, temp->inf.numarBileteValabile, temp->inf.capacitateVagon, temp->inf.firmaTransport);
		temp = temp->prev;
	}
}

nodLD* stergeVagoane(nodLD* prim, nodLD** ultim, int nrBileteVandute) {
	nodLD* temp = prim;
	while (temp != NULL)
	{
		if (temp->inf.capacitateVagon - temp->inf.numarBileteValabile > nrBileteVandute) {
			//stergem nodul
			nodLD* deleteMe = temp;
			if (temp->prev == NULL) {
				//stergem primul element
				prim = prim->next;
				prim->prev = NULL;
			}
			else if (temp->next == NULL) {
				//stergem ultimul element
				nodLD* ant = temp->prev;
				ant->next = NULL;
				*ultim = ant;
			}
			else {
				nodLD* urm = temp->next;
				nodLD* ant = temp->prev;
				urm->prev = ant;
				ant->next = urm;
			}
			temp = temp->next;

			free(deleteMe->inf.firmaTransport);
			free(deleteMe);
		}
		else {
			temp = temp->next;
		}
	}
	return prim;
}

void filtrareHeap(heap h, int index) {
	int indexRad = index;
	int indexS = 2 * index + 1;
	int indexD = 2 * index + 2;

	if (indexS < h.nrElem &&
		((float)(h.vector[indexS].capacitateVagon - h.vector[indexS].numarBileteValabile) / h.vector[indexS].capacitateVagon) <
		((float)(h.vector[indexRad].capacitateVagon - h.vector[indexRad].numarBileteValabile) / h.vector[indexRad].capacitateVagon)) {
		indexRad = indexS;
	}

	if (indexD < h.nrElem &&
		((float)(h.vector[indexD].capacitateVagon - h.vector[indexD].numarBileteValabile) / h.vector[indexD].capacitateVagon) <
		((float)(h.vector[indexRad].capacitateVagon - h.vector[indexRad].numarBileteValabile) / h.vector[indexRad].capacitateVagon)) {
		indexRad = indexD;
	}

	if (index != indexRad) {
		Vagon aux = h.vector[index];
		h.vector[index] = h.vector[indexRad];
		h.vector[indexRad] = aux;

		filtrareHeap(h, indexRad);
	}
}

heap lista_heap(nodLD* prim) {
	heap h;

	int nrVagoane = 0;
	nodLD* temp = prim;
	while (temp != NULL) {
		nrVagoane++;
		temp = temp->next;
	}

	temp = prim;
	h.nrElem = nrVagoane;
	h.vector = (Vagon*)malloc(nrVagoane * sizeof(Vagon));
	for (int i = 0; i < nrVagoane; i++) {
		h.vector[i].capacitateVagon = temp->inf.capacitateVagon;
		h.vector[i].numarBileteValabile = temp->inf.numarBileteValabile;
		h.vector[i].numarVagon = temp->inf.numarVagon;
		h.vector[i].firmaTransport = (char*)malloc(strlen(temp->inf.firmaTransport) + 1);
		strcpy(h.vector[i].firmaTransport, temp->inf.firmaTransport);

		temp = temp->next;
	}

	for (int i = (h.nrElem - 1) / 2; i >= 0; i--) {
		filtrareHeap(h, i);
	}

	return h;
}

void afisareHeap(heap h) {
	for (int i = 0; i < h.nrElem; i++)
	{
		float gradOcupare = ((float)(h.vector[i].capacitateVagon - h.vector[i].numarBileteValabile) / h.vector[i].capacitateVagon) * 100;
		printf("Nr. vagon = %d \t Nr. bilete = %d \t Capacitate = %d \t Firma = %s \t\t Grad Ocupare = %5.2f\n",
			h.vector[i].numarVagon, h.vector[i].numarBileteValabile, h.vector[i].capacitateVagon, h.vector[i].firmaTransport, gradOcupare);
	}
}

int main() {
	nodLD* prim = NULL;
	nodLD* ultim = NULL;

	FILE* f = fopen("fisier.txt", "r");
	if (f == NULL) {
		printf("Fisierul nu s-a deschis!");
		return -1;
	}

	int nrVagoane;
	char buffer[128];
	Vagon vagon;

	fscanf(f, "%d", &nrVagoane);
	for (int i = 0; i < nrVagoane; i++) {
		fscanf(f, "%d", &vagon.numarVagon);

		fscanf(f, "%s", buffer);
		vagon.firmaTransport = (char*)malloc(strlen(buffer) + 1);
		strcpy(vagon.firmaTransport, buffer);

		fscanf(f, "%d", &vagon.numarBileteValabile);
		fscanf(f, "%d", &vagon.capacitateVagon);

		prim = inserareLD(prim, &ultim, vagon);

		free(vagon.firmaTransport);
	}

	printf("Lista Cap -> Coada citita din fisier\n");
	afisareCapCoada(prim);

	printf("\nLista Coada -> Cap citita din fisier\n");
	afisareCoadaCap(ultim);

	prim = stergeVagoane(prim, &ultim, 15);
	printf("\nLista Cap -> Coada dupa stergere\n");
	afisareCapCoada(prim);

	printf("\nLista Coada -> Cap dupa stergere\n");
	afisareCoadaCap(ultim);

	heap h = lista_heap(prim);
	printf("\nStructura HEAP:\n");
	afisareHeap(h);

	return 0;
}