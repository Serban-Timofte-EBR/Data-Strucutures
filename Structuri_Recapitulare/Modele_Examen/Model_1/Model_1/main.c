#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

typedef struct {
	int cod;
	char* firma;
	int nrProduse;
	float suma;
} Comanda;

typedef struct {
	Comanda inf;
	struct nodCoada* next;
} nodCoada;

typedef struct {
	Comanda* vector;
	int nrElem;
} heap;

nodCoada* put(nodCoada* prim, nodCoada** ultim, Comanda com) {
	nodCoada* nou = (nodCoada*)malloc(sizeof(nodCoada));
	nou->inf.cod = com.cod;
	nou->inf.nrProduse = com.nrProduse;
	nou->inf.suma = com.suma;
	nou->inf.firma = (char*)malloc(strlen(com.firma) + 1);
	strcpy(nou->inf.firma, com.firma);
	nou->next = NULL;

	if (prim == NULL) {
		prim = nou;
	}
	else {
		nodCoada* temp = prim;
		while (temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = nou;
		*ultim = nou;
	}
	return prim;
}

int get(nodCoada** prim, nodCoada** ultim, Comanda* comExtrasa) {
	if (*prim == NULL) {
		return 0;
	}

	(*comExtrasa).cod = (*prim)->inf.cod;
	(*comExtrasa).nrProduse = (*prim)->inf.nrProduse;
	(*comExtrasa).suma = (*prim)->inf.suma;
	(*comExtrasa).firma = (char*)malloc(strlen((*prim)->inf.firma) + 1);
	strcpy((*comExtrasa).firma, (*prim)->inf.firma);

	nodCoada* temp = *prim;
	*prim = (*prim)->next;
	free(temp->inf.firma);
	free(temp);

	return 1;
}

void afisareCoada(nodCoada* prim) {
	nodCoada* temp = prim;
	while (temp != NULL) {
		printf("Cod = %d \t Nr Produse = %d \t Suma = %5.2f \t Firma = %s\n",
			temp->inf.cod, temp->inf.nrProduse, temp->inf.suma, temp->inf.firma);
		temp = temp->next;
	}
}

void cerinta2(nodCoada** prim, nodCoada** ultim) {
	nodCoada* primTemp = NULL;
	nodCoada* ultimTemp = NULL;

	Comanda comandaExtrasa;
	printf("\nElementele din coada PRIN GET sunt:\n");
	while (get(prim, ultim, &comandaExtrasa) == 1)
	{
		printf("Cod = %d \t Nr Produse = %d \t Suma = %5.2f \t Firma = %s\n",
			comandaExtrasa.cod, comandaExtrasa.nrProduse, comandaExtrasa.suma, comandaExtrasa.firma);
		primTemp = put(primTemp, &ultimTemp, comandaExtrasa);
	}

	printf("\nCoada DUPA AFISAREA PRIN GET:\n");
	afisareCoada(*prim);

	while (get(&primTemp, &ultimTemp, &comandaExtrasa) == 1)
	{
		*prim = put(*prim, ultim, comandaExtrasa);
	}
}

float cerinta3(nodCoada** prim, nodCoada** ultim, Comanda* vector, int* nrElem, char* denumire) {
	float suma = 0;
	Comanda comandaExtrasa;
	while (get(prim, ultim, &comandaExtrasa) == 1)
	{
		if (strcmp(comandaExtrasa.firma, denumire) == 0) {
			vector[*nrElem].cod = comandaExtrasa.cod;
			vector[*nrElem].nrProduse = comandaExtrasa.nrProduse;
			vector[*nrElem].suma = comandaExtrasa.suma;
			vector[*nrElem].firma = (char*)malloc(strlen(comandaExtrasa.firma) + 1);
			strcpy(vector[*nrElem].firma, comandaExtrasa.firma);
			(*nrElem)++;
		}
	}
	for (int i = 0; i < *nrElem; i++) {
		suma += vector[i].suma;
	}
	return suma;
}

void filtrareHeap(heap h, int index) {
	int indexRad = index;
	int indexS = 2 * index + 1;
	int indexD = 2 * index + 2;

	if (indexS < h.nrElem && h.vector[indexS].suma > h.vector[indexRad].suma) {
		indexRad = indexS;
	}

	if (indexD < h.nrElem && h.vector[indexD].suma > h.vector[indexRad].suma) {
		indexRad = indexD;
	}

	if (index != indexRad) {
		Comanda aux = h.vector[index];
		h.vector[index] = h.vector[indexRad];
		h.vector[indexRad] = aux;

		filtrareHeap(h, indexRad);
	}
}

heap vector_heap(Comanda* vectorComenzi, int nrElem) {
	heap h;
	h.nrElem = nrElem;
	h.vector = (Comanda*)malloc(h.nrElem * sizeof(Comanda));
	for (int i = 0; i < h.nrElem; i++) {
		h.vector[i] = vectorComenzi[i];
	}

	for (int i = (h.nrElem - 1) / 2; i >= 0; i--) {
		filtrareHeap(h, i);
	}

	return h;
}

void afisare_heap(heap h) {
	for (int i = 0; i < h.nrElem; i++) {
		printf("Cod = %d \t Nr Produse = %d \t Suma = %5.2f \t Firma = %s\n",
			h.vector[i].cod, h.vector[i].nrProduse, h.vector[i].suma, h.vector[i].firma);
	}
}

heap modificare_nr_produse(heap h, int codComanda, int nrProd) {
	for (int i = 0; i < h.nrElem; i++) {
		if (h.vector[i].cod == codComanda) {
			h.vector[i].nrProduse = nrProd;
		}
	}
	return h;
}

int main() {
	nodCoada* prim = NULL;
	nodCoada* ultim = NULL;

	FILE* f = fopen("fisier.txt", "r");
	if (f == NULL) {
		printf("Fisierul nu a fost deschis!");
		return -1;
	}

	int nrComenzi;
	Comanda com;
	char buffer[128];

	fscanf(f, "%d", &nrComenzi);
	for (int i = 0; i < nrComenzi; i++)
	{
		fscanf(f, "%d", &com.cod);

		fscanf(f, "%s", buffer);
		com.firma = (char*)malloc(strlen(buffer) + 1);
		strcpy(com.firma, buffer);

		fscanf(f, "%d", &com.nrProduse);
		fscanf(f, "%f", &com.suma);

		prim = put(prim, &ultim, com);

		free(com.firma);
	}
	fclose(f);

	printf("Coada citita din fisier prin FUNCTIA AFISARE:\n");
	afisareCoada(prim);


	cerinta2(&prim, &ultim);
	printf("\nCoada refacuta de la CERINTA 2:\n");
	afisareCoada(prim);

	//preluarea intr-un vector a comenzilor de la o anumita firma si calculam suma comenzilor
	Comanda* vectorComenzi = (Comanda*)malloc(nrComenzi * sizeof(Comanda));
	int nrElem = 0;
	float valoareComenzi = cerinta3(&prim, &ultim, vectorComenzi, &nrElem, "RoadTrans");
	printf("\nValoarea comenzilor din vector: %f\n", valoareComenzi);

	printf("\nComenzile din vector: \n");
	for (int i = 0; i < nrElem; i++) {
		printf("Cod = %d \t Nr Produse = %d \t Suma = %5.2f \t Firma = %s\n",
			vectorComenzi[i].cod, vectorComenzi[i].nrProduse, vectorComenzi[i].suma, vectorComenzi[i].firma);
	}

	printf("\nCoada dupa preluarea in vector:\n");
	afisareCoada(prim);

	heap h = vector_heap(vectorComenzi, nrElem);
	printf("\nStructura HEAP:\n");
	afisare_heap(h);

	printf("\nStructura HEAP dupa modificare:\n");
	h = modificare_nr_produse(h, 101, 23);
	afisare_heap(h);

	return 0;
}