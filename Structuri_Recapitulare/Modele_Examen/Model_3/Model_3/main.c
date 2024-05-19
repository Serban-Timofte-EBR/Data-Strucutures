#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

// Produse in hash table
// Hash table in Arbore Binar de cautare
// Hash Table in HEAP

typedef struct {
	int cod;
	char* nume;
	float pret;
} produs;

typedef struct {
	produs inf;
	struct nodLS* next;
} nodLS;

typedef struct {
	nodLS** vector;
	int nrElem;
} hashT;

typedef struct {
	produs* vector;
	int nrElem;
} heap;

typedef struct {
	produs inf;
	struct nodArbBin* stanga;
	struct nodArbBin* dreapta;
} nodArbBin;

void inserareLS(nodLS** prim, produs prod) {
	nodLS* nou = (nodLS*)malloc(sizeof(nodLS));
	nou->inf.cod = prod.cod;
	nou->inf.pret = prod.pret;
	nou->inf.nume = (char*)malloc(strlen(prod.nume) + 1);
	strcpy(nou->inf.nume, prod.nume);
	nou->next = NULL;

	if (*prim == NULL) {
		*prim = nou;
	}
	else {
		nodLS* temp = *prim;
		while (temp->next != NULL) { 
			temp = temp->next;
		}
		temp->next = nou;
	}
}

int functieHash(hashT tabela, produs prod) {
	return prod.nume[0] % tabela.nrElem;
}

void inserareHash(hashT tabela, produs prod) {
	int poz = functieHash(tabela, prod);
	inserareLS(&tabela.vector[poz], prod);
}

void afisareLS(nodLS* prim) {
	nodLS* temp = prim;
	while (temp != NULL)
	{
		printf("\tCod = %d \t Pret = %f \t Produs = %s\n",
			temp->inf.cod, temp->inf.pret, temp->inf.nume);
		temp = temp->next;
	}
}

void afiasreHash(hashT tabela) {
	for (int i = 0; i < tabela.nrElem; i++) {
		if (tabela.vector[i] != NULL) {
			printf("Pozitia %d:\n", i);
			afisareLS(tabela.vector[i]);
		}
	}
}

void produseIeftineF(hashT tabela, produs* vector, int* nrElem, float prag) {
	for (int i = 0; i < tabela.nrElem; i++) {
		if (tabela.vector[i] != NULL) {
			nodLS* temp = tabela.vector[i];
			nodLS* prev = NULL;
			while (temp != NULL) {
				if (temp->inf.pret < prag) {
					nodLS* deleteMe = temp;

					vector[*nrElem].cod = temp->inf.cod;
					vector[*nrElem].pret = temp->inf.pret;
					vector[*nrElem].nume = (char*)malloc(strlen(temp->inf.nume) + 1);
					strcpy(vector[*nrElem].nume, temp->inf.nume);
					(*nrElem)++;

					if (prev == NULL) {
						tabela.vector[i] = temp->next;
					}
					else {
						prev->next = temp->next;
					}
					temp = temp->next;

					free(deleteMe->inf.nume);
					free(deleteMe);
				}
				else {
					prev = temp;
					temp = temp->next;
				}
			}
		}
	}
}

// MAX - Heap dupa cod
void filtrareHEAP(heap h, int index) {
	int indexRad = index;
	int indexS = 2 * index + 1;
	int indexD = 2 * index + 2;

	if (indexS < h.nrElem && h.vector[indexS].cod > h.vector[indexRad].cod) {
		indexRad = indexS;
	}

	if (indexD < h.nrElem && h.vector[indexD].cod > h.vector[indexRad].cod) {
		indexRad = indexD;
	}

	if (index != indexRad) {
		produs aux = h.vector[index];
		h.vector[index] = h.vector[indexRad];
		h.vector[indexRad] = aux;

		filtrareHEAP(h, indexRad);
	}
}

void populareHeap(hashT tabela, heap* h) {
	int index = 0;
	h->vector = (produs*)malloc(tabela.nrElem * sizeof(produs));
	h->nrElem = 0;
	for (int i = 0; i < tabela.nrElem; i++) {
		nodLS* temp = tabela.vector[i];
		while (temp != NULL) {
			h->vector[index].cod = temp->inf.cod;
			h->vector[index].pret = temp->inf.pret;
			h->vector[index].nume = (char*)malloc(strlen(temp->inf.nume) + 1);
			strcpy(h->vector[index].nume, temp->inf.nume);
			index++;
			h->nrElem++;
			temp = temp->next;
		}
	}

	for (int i = (h->nrElem - 1) / 2; i >= 0; i--) {
		filtrareHEAP(*h, i);
	}
}

void afisareHeap(heap h) {
	for (int i = 0; i < h.nrElem; i++) {
		printf("Cod = %d \t Pret = %f \t Produs = %s\n",
			h.vector[i].cod, h.vector[i].pret, h.vector[i].nume);
	}
}

heap stergereProdus(heap h, int cod, produs* extras) {
	produs* vectorNou = (produs*)malloc((h.nrElem - 1) * sizeof(produs));

	produs aux = h.vector[0];
	h.vector[0] = h.vector[h.nrElem - 1];
	h.vector[h.nrElem - 1] = aux;

	*extras = h.vector[h.nrElem - 1];

	h.nrElem--;
	for (int i = 0; i < h.nrElem; i++) {
		vectorNou[i] = h.vector[i];
	}
	free(h.vector);
	h.vector = vectorNou;

	for (int i = (h.nrElem - 1) / 2; i >= 0; i--) {
		filtrareHEAP(h, i);
	}

	return h;
}

nodArbBin* creareaNod(produs prod, nodArbBin* st, nodArbBin* dr) {
	nodArbBin* nou = (nodArbBin*)malloc(sizeof(nodArbBin));
	nou->inf.pret = prod.pret;
	nou->inf.cod = prod.cod;
	nou->inf.nume = (char*)malloc(strlen(prod.nume) + 1);
	strcpy(nou->inf.nume, prod.nume);

	nou->stanga = st;
	nou->dreapta = dr;

	return nou;
}

nodArbBin* inserareNod(nodArbBin* rad, produs prod) {
	if (rad != NULL) {
		if (prod.cod < rad->inf.cod) {
			rad->stanga = inserareNod(rad->stanga, prod);
			return rad;
		} 

		if (prod.cod > rad->inf.cod) {
			rad->dreapta = inserareNod(rad->dreapta, prod);
			return rad;
		}

		else {
			return rad;
		}
	}
	else {
		return creareaNod(prod, NULL, NULL);
	}
}

nodArbBin* hash_arbore(nodArbBin* rad, hashT h) {
	for (int i = 0; i < h.nrElem; i++) {
		if (h.vector[i] != NULL) {
			nodLS* temp = h.vector[i];
			while (temp != NULL)
			{
				rad = inserareNod(rad, temp->inf);
				temp = temp->next;
			}
		}
	}
	return rad;
}

void inordine(nodArbBin* rad) {
	if (rad != NULL) {
		inordine(rad->stanga);
		printf("Cod = %d \t Pret = %f \t Produs = %s\n",
			rad->inf.cod, rad->inf.pret, rad->inf.nume);
		inordine(rad->dreapta);
	}
}

void getSumaAndCounter(nodArbBin* rad, int* counter, float* suma) {
	if (rad != NULL) {
		*suma += rad->inf.pret;
		(*counter)++;
		getSumaAndCounter(rad->stanga, counter, suma);
		getSumaAndCounter(rad->dreapta, counter, suma);
	}
}

float getMediaPreturilor(nodArbBin* rad) {
	int counter = 0;
	float suma = 0.0;
	getSumaAndCounter(rad, &counter, &suma);
	return suma / counter;
}

nodArbBin* findMin(nodArbBin* rad) {
	while (rad->stanga != NULL)
	{
		rad = rad->stanga;
	}
	return rad;
}

nodArbBin* stergereNod(nodArbBin* rad, int cod) {
	if (rad == NULL) {
		return rad;
	}

	if (cod < rad->inf.cod) {
		rad->stanga = stergereNod(rad->stanga, cod);
	}

	else if (cod > rad->inf.cod) {
		rad->dreapta = stergereNod(rad->dreapta, cod);
	}

	else {
		if (rad->stanga == NULL) {
			nodArbBin* temp = rad->dreapta;
			free(rad->inf.nume);
			free(rad);
			return temp;
		}

		else if (rad->dreapta == NULL) {
			nodArbBin* temp = rad->stanga;
			free(rad->inf.nume);
			free(rad);
			return temp;
		}

		else {
			nodArbBin* temp = findMin(rad->dreapta);
			rad->inf.cod = temp->inf.cod;
			rad->inf.pret = temp->inf.pret;
			free(rad->inf.nume);
			rad->inf.nume = (char*)malloc(strlen(temp->inf.nume) + 1);
			strcpy(rad->inf.nume, temp->inf.nume);
			rad->dreapta = stergereNod(rad->dreapta, temp->inf.cod);
		}
		return rad;
	}
}

int main() {
	FILE* f = fopen("fisier.txt", "r");
	if (f == NULL) {
		printf("Fisierul nu a fost deshis");
		return -1;
	}
	
	hashT tabela;
	fscanf(f, "%d", &tabela.nrElem);
	tabela.vector = (produs*)malloc(tabela.nrElem * sizeof(produs));
	for (int i = 0; i < tabela.nrElem; i++) {
		tabela.vector[i] = NULL;
	}

	produs prod;
	char buffer[128];
	for (int i = 0; i < tabela.nrElem; i++) {
		fscanf(f, "%d", &prod.cod);

		fscanf(f, "%s", buffer);
		prod.nume = (char*)malloc(strlen(buffer) + 1);
		strcpy(prod.nume, buffer);

		fscanf(f, "%f", &prod.pret);

		inserareHash(tabela, prod);

		free(prod.nume);
	}
	fclose(f);

	printf("Tabela de dispersie citita din fisier:\n");
	afiasreHash(tabela);

	// Preluam intr-un vector elementele mai ieftine de 10 lei si le stergem din tabela de dispersie
	int nrElemProdIeftine = 0;
	produs* produseIeftine = (produs*)malloc(tabela.nrElem * sizeof(produs));
	produseIeftineF(tabela, produseIeftine, &nrElemProdIeftine, 10.0);

	printf("\nVectorul cu produse ieftine:\n");
	for (int i = 0; i < nrElemProdIeftine; i++) {
		printf("Cod = % d \t Pret = % f \t Produs = % s\n",
			produseIeftine[i].cod, produseIeftine[i].pret, produseIeftine[i].nume);
	}

	printf("\nTabela de dispersie dupa crearea vectorului:\n");
	afiasreHash(tabela);

	// Tabela -> Heap
	heap h;
	populareHeap(tabela, &h);

	printf("\nStructura HEAP:\n");
	afisareHeap(h);

	// Stergerea din HEAP a produsului cu un cod trimis ca parametru
	produs produsExtrasDinHeap;
	h = stergereProdus(h, 35, &produsExtrasDinHeap);

	printf("\nProdusul extras are codul: %d\n", produsExtrasDinHeap.cod);

	printf("\nStructura HEAP dupa extragere:\n");
	afisareHeap(h);

	// Tabela -> Arbore Binar de Cautare
	nodArbBin* rad = NULL;
	rad = hash_arbore(rad, tabela);

	printf("\nArborele INORDINE:\n");
	inordine(rad);

	// Media preturilor din arbore
	float media = getMediaPreturilor(rad);
	printf("\nMedia preturilor din arbore este: %5.2f\n", media);

	int codProdusDeSters = 1;
	rad = stergereNod(rad, codProdusDeSters);
	printf("\nArborele dupa stergerea produsului cu codul %d:\n", codProdusDeSters);
	inordine(rad);

	return 0;
}