#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct {
	int cod;
	char* firma;
	int nrProd;
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
	nou->inf.nrProd = com.nrProd;
	nou->inf.suma = com.suma;
	nou->inf.firma = (char*)malloc(strlen(com.firma) + 1);
	strcpy(nou->inf.firma, com.firma);
	nou->next = NULL;

	if (prim == NULL) {
		prim = nou;
		*ultim = nou;
	}
	else {
		(*ultim)->next = nou;
		*ultim = nou;
	}

	return prim;
}

int get(nodCoada** prim, nodCoada** ultim, Comanda* out) {
	if (*prim == NULL) {
		return 0;
	}

	(*out).cod = (*prim)->inf.cod;
	(*out).nrProd = (*prim)->inf.nrProd;
	(*out).suma = (*prim)->inf.suma;
	(*out).firma = (char*)malloc(strlen((*prim)->inf.firma) + 1);
	strcpy((*out).firma, (*prim)->inf.firma);

	nodCoada* aux = (*prim)->next;
	free((*prim)->inf.firma);
	free(*prim);
	*prim = aux;

	return 1;
}

void afisareCoada(nodCoada* prim) {
	nodCoada* temp = prim;
	while (temp != NULL)
	{
		printf("Cod: %d, Nr. Prod: %d, Suma: %5.2f, Firma: %s\n",
			temp->inf.cod, temp->inf.nrProd, temp->inf.suma, temp->inf.firma);
		temp = temp->next;
	}
}

Comanda* vectorCuComenzileFirmei(nodCoada** prim, nodCoada** ultim, char* firma, int nrElem) {
	Comanda* vector = (Comanda*)malloc(nrElem * sizeof(Comanda));
	int index = 0;
	Comanda out;
	while (get(prim, ultim, &out) == 1)
	{
		if (strcmp(out.firma, firma) == 0) {
			vector[index].cod = out.cod;
			vector[index].nrProd = out.nrProd;
			vector[index].suma = out.suma;
			vector[index].firma = (char*)malloc(strlen(out.firma) + 1);
			strcpy(vector[index].firma, out.firma);
			index++;
		}
	}
	return vector;
}

void filtrare(heap h, int index) {
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

		filtrare(h, indexRad);
	}
}

void creareaHeap(heap h, nodCoada** prim, nodCoada** ultim) {
	Comanda out;
	int indexVector = 0;
	while (get(prim, ultim, &out) == 1)
	{
		h.vector[indexVector].cod = out.cod;
		h.vector[indexVector].nrProd = out.nrProd;
		h.vector[indexVector].suma = out.suma;
		h.vector[indexVector].firma = (char*)malloc(strlen(out.firma) + 1);
		strcpy(h.vector[indexVector].firma, out.firma);
		indexVector++;
	}

	for (int i = (h.nrElem - 1) / 2; i >= 0; i--) {
		filtrare(h, i);
	}
}

void afisareHeap(heap h) {
	for (int i = 0; i < h.nrElem; i++) {
		printf("Cod: %d, Nr. Prod: %d, Suma: %5.2f, Firma: %s\n",
			h.vector[i].cod, h.vector[i].nrProd, h.vector[i].suma, h.vector[i].firma);
	}
}

void modificareNrComenziHeap(heap h, int codComanda, int nrProdNou) {
	for (int i = 0; i < h.nrElem; i++) {
		if (h.vector[i].cod == codComanda) {
			h.vector[i].nrProd = nrProdNou;
		}
	}
}

int main() {
	nodCoada* prim = NULL;
	nodCoada* ultim = NULL;

	FILE* f = fopen("fisier.txt", "r");

	Comanda com;
	char buffer[128];

	while (fscanf(f, "%d", &com.cod) == 1)
	{
		fscanf(f, " %[^\n]", buffer);
		com.firma = (char*)malloc(strlen(buffer) + 1);
		strcpy(com.firma, buffer);

		fscanf(f, "%d", &com.nrProd);
		fscanf(f, "%f", &com.suma);

		/*printf("Cod: %d, Nr. Prod: %d, Suma: %5.2f, Firma: %s\n",
			com.cod, com.nrProd, com.suma, com.firma);*/
		prim = put(prim, &ultim, com);

		free(com.firma);
	}
	fclose(f);

	printf("Coada din fisier:\n");
	afisareCoada(prim);

	printf("\nCoada prin GET:\n");
	nodCoada* prim2 = NULL;
	nodCoada* ultim2 = NULL;
	Comanda out;

	// !!! Aici numar elementele pentru vectorul cu firme !!!
	int nrElemVector = 0;
	char* denumireFirmaCautata = "Fan Curier";

	int nrElemCoada = 0;

	nodCoada* prim3 = NULL;
	nodCoada* ultim3 = NULL;

	while (get(&prim, &ultim, &out) == 1)
	{
		printf("Cod: %d, Nr. Prod: %d, Suma: %5.2f, Firma: %s\n",
			out.cod, out.nrProd, out.suma, out.firma);
		if (strcmp(out.firma, denumireFirmaCautata) == 0) {
			nrElemVector++;
		}
		prim2 = put(prim2, &ultim2, out);
		nrElemCoada++;
		prim3 = put(prim3, &ultim3, out);
	}

	printf("\nCoada dupa traversare:\n");
	afisareCoada(prim);

	printf("\nCoada Reconstruita:\n");
	afisareCoada(prim2);

	Comanda* comenzileFirmei = vectorCuComenzileFirmei(&prim2, &ultim2, denumireFirmaCautata, nrElemVector);
	printf("\nVectorul:\n");
	for (int i = 0; i < nrElemVector; i++) {
		printf("Cod: %d, Nr. Prod: %d, Suma: %5.2f, Firma: %s\n",
			comenzileFirmei[i].cod, comenzileFirmei[i].nrProd, comenzileFirmei[i].suma, comenzileFirmei[i].firma);
	}

	printf("\nCoada refacuta:\n");
	afisareCoada(prim3);

	heap h;
	h.nrElem = nrElemCoada;
	h.vector = (Comanda*)malloc(h.nrElem * sizeof(Comanda));

	creareaHeap(h, &prim3, &ultim3);
	printf("\nHEAP:\n");
	afisareHeap(h);

	printf("\nCoada dupa crearea HEAP:\n");
	afisareCoada(prim3);

	modificareNrComenziHeap(h, 556, 21);
	printf("\nHeapul dupa modificare:\n");
	afisareHeap(h);

	return 0;
}