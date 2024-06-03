#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct {
	unsigned int cod;
	char* numeClient;
	char* adresaLivrare;
	float valoare;
} Comanda;

typedef struct {
	Comanda inf;
	struct nodCoada* next;
} nodCoada;

typedef struct {
	Comanda* vector;
	int nrElem;
} heap;

nodCoada* put(nodCoada* prim, Comanda com) {
	nodCoada* nou = (nodCoada*)malloc(sizeof(nodCoada));
	nou->inf.cod = com.cod;
	nou->inf.valoare = com.valoare;
	nou->inf.adresaLivrare = (char*)malloc(strlen(com.adresaLivrare) + 1);
	strcpy(nou->inf.adresaLivrare, com.adresaLivrare);
	nou->inf.numeClient = (char*)malloc(strlen(com.numeClient) + 1);
	strcpy(nou->inf.numeClient, com.numeClient);
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
	}

	return prim;
}

void afisareCoada(nodCoada* prim) {
	nodCoada* temp = prim;
	while (temp != NULL) {
		printf("Cod: %d, Valoare: %5.2f, Nume: %s, Adresa: %s\n",
			temp->inf.cod, temp->inf.valoare, temp->inf.numeClient, temp->inf.adresaLivrare);
		temp = temp->next;
	}
}

int get(nodCoada** prim, Comanda* out) {
	if (*prim == NULL) {
		return 0;
	}

	(*out).cod = (*prim)->inf.cod;
	(*out).valoare = (*prim)->inf.valoare;
	(*out).adresaLivrare = (char*)malloc(strlen((*prim)->inf.adresaLivrare) + 1);
	strcpy((*out).adresaLivrare, (*prim)->inf.adresaLivrare);
	(*out).numeClient = (char*)malloc(strlen((*prim)->inf.numeClient) + 1);
	strcpy((*out).numeClient, (*prim)->inf.numeClient);

	Comanda* aux = *prim;
	*prim = (*prim)->next;
	free(aux->adresaLivrare);
	free(aux->numeClient);
	free(aux);

	return 1;
}

nodCoada* stergereCodCoada(nodCoada* prim, int idCom) {
	nodCoada* primAux = NULL;

	Comanda com;
	while (get(&prim, &com) == 1) {
		if (com.cod != idCom) {
			primAux = put(primAux, com);
		}
		free(com.adresaLivrare);
		free(com.numeClient);
	}

	return primAux;
}

nodCoada* counterComenziPrag(nodCoada** prim, float prag, int* counter) {
	nodCoada* primAux = NULL;

	*counter = 0;
	Comanda com;
	while (get(prim, &com) == 1) {
		if (com.valoare > prag) {
			(*counter)++;
		}

		primAux = put(primAux, com);

		free(com.adresaLivrare);
		free(com.numeClient);
	}

	return primAux;
}

Comanda* vectorComenziPrag(nodCoada** prim, int* nrElem, float prag) {
	nodCoada* primAux;
	primAux = counterComenziPrag(prim, prag, nrElem);
	Comanda* vector = (Comanda*)malloc(*nrElem * sizeof(Comanda));
	int index = 0;
	
	Comanda com;

	while (get(&primAux, &com) == 1)
	{
		if (com.valoare > prag) {
			vector[index].cod = com.cod;
			vector[index].valoare = com.valoare;
			vector[index].numeClient = (char*)malloc(strlen(com.numeClient) + 1);
			strcpy(vector[index].numeClient, com.numeClient);
			vector[index].adresaLivrare = (char*)malloc(strlen(com.adresaLivrare) + 1);
			strcpy(vector[index].adresaLivrare, com.adresaLivrare);
			index++;
		}
		free(com.adresaLivrare);
		free(com.numeClient);
	}

	*prim = primAux;
	return vector;
}

void filtrareHeap(heap h, int index) {
	int indexRad = index;
	int indexS = 2 * index + 1;
	int indexD = 2 * index + 2;

	if (indexS < h.nrElem && h.vector[indexS].valoare < h.vector[indexRad].valoare) {
		indexRad = indexS;
	}

	if (indexD < h.nrElem && h.vector[indexD].valoare < h.vector[indexRad].valoare) {
		indexRad = indexD;
	}

	if (index != indexRad) {
		Comanda aux = h.vector[index];
		h.vector[index] = h.vector[indexRad];
		h.vector[indexRad] = aux;

		filtrareHeap(h, indexRad);
	}
}

heap inserareHeap(heap h, Comanda com) {
	Comanda* vectorNou = (Comanda*)malloc((h.nrElem + 1) * sizeof(Comanda));
	for (int i = 0; i < h.nrElem; i++) {
		vectorNou[i] = h.vector[i];
	}

	vectorNou[h.nrElem].cod = com.cod;
	vectorNou[h.nrElem].valoare = com.valoare;
	vectorNou[h.nrElem].adresaLivrare = (char*)malloc(strlen(com.adresaLivrare) + 1);
	strcpy(vectorNou[h.nrElem].adresaLivrare, com.adresaLivrare);
	vectorNou[h.nrElem].numeClient = (char*)malloc(strlen(com.numeClient) + 1);
	strcpy(vectorNou[h.nrElem].numeClient, com.numeClient);

	free(h.vector);
	h.vector = vectorNou;
	h.nrElem++;

	for (int i = (h.nrElem - 1) / 2; i >= 0; i--) {
		filtrareHeap(h, i);
	}

	return h;
}

heap vector_heap(Comanda* vector, int nrElem) {
	heap h;
	h.nrElem = 0;
	h.vector = (Comanda*)malloc(sizeof(Comanda));

	for (int i = 0; i < nrElem; i++) {
		h = inserareHeap(h, vector[i]);
	}

	return h;
}

afisareHeap(heap h) {
	for (int i = 0; i < h.nrElem; i++) {
		printf("Valoare: %5.2f, Cod: %d, Nume: %s, Adresa: %s\n",
			h.vector[i].valoare, h.vector[i].cod, h.vector[i].numeClient, h.vector[i].adresaLivrare);
	}
}

heap stergereHeap(heap h) {
	Comanda* vectorNou = (Comanda*)malloc((h.nrElem - 1) * sizeof(Comanda));
	
	Comanda aux = h.vector[0];
	h.vector[0] = h.vector[h.nrElem - 1];
	
	free(aux.adresaLivrare);
	free(aux.numeClient);

	h.nrElem--;
	for (int i = 0; i < h.nrElem; i++) {
		vectorNou[i] = h.vector[i];
	}

	free(h.vector);
	h.vector = vectorNou;

	for (int i = (h.nrElem - 1) / 2; i >= 0; i--) {
		filtrareHeap(h, i);
	}

	return h;
}

int counterComenziHeapPrag(heap h, float prag) {
	int counter = 0;
	for (int i = 0; i < h.nrElem; i++) {
		if (h.vector[i].valoare > prag) {
			counter++;
		}
	}
	return counter;
}

int main() {
	nodCoada* prim = NULL;

	FILE* f = fopen("fisier.txt", "r");

	Comanda com;
	char buffer[128];

	while (fscanf(f, "%d", &com.cod) == 1)
	{
		fscanf(f, " %[^\n]", buffer);
		com.numeClient = (char*)malloc(strlen(buffer) + 1);
		strcpy(com.numeClient, buffer);

		fscanf(f, " %[^\n]", buffer);
		com.adresaLivrare = (char*)malloc(strlen(buffer) + 1);
		strcpy(com.adresaLivrare, buffer);

		fscanf(f, "%f", &com.valoare);

		/*printf("Cod: %d, Valoare: %5.2f, Nume: %s, Adresa: %s\n", 
			com.cod, com.valoare, com.numeClient, com.adresaLivrare);*/
		prim = put(prim, com);

		free(com.numeClient);
		free(com.adresaLivrare);
	}
	fclose(f);

	printf("Coada din fisier:\n");
	afisareCoada(prim);

	prim = stergereCodCoada(prim, 549);
	printf("\nCoada dupa stergere:\n");
	afisareCoada(prim);

	int nrElem = 0;
	/*prim = counterComenziPrag(&prim, 100, &nrElem);
	printf("\n%d\n", nrElem);
	afisareCoada(prim);*/

	Comanda* vector = vectorComenziPrag(&prim, &nrElem, 100);
	printf("\nVectorul:\n");
	for (int i = 0; i < nrElem; i++) {
		printf("Cod: %d, Valoare: %5.2f, Nume: %s, Adresa: %s\n",
			vector[i].cod, vector[i].valoare, vector[i].numeClient, vector[i].adresaLivrare);
	}
	
	printf("\nCoada:\n");
	afisareCoada(prim);

	heap h = vector_heap(vector, nrElem);
	printf("\nHEAP:\n");
	afisareHeap(h);

	h = stergereHeap(h);
	printf("\nHeapul dupa stergere:\n");
	afisareHeap(h);

	int nrComenziPestePrag = counterComenziHeapPrag(h, 500);
	printf("\nAvem %d comenzi pestre pragul cerut\n", nrComenziPestePrag);

	return 0;
}