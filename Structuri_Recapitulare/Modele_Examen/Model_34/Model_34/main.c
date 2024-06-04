#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct {
	int numarAeronava;
	char* plecare;
	char* destinatie;
	float durata;
	int nrPersonal;
} CursaAeriana;

typedef struct {
	CursaAeriana inf;
	struct nodLS* next;
} nodLS;

typedef struct {
	nodLS** vector;
	int nrElem;
} hashT;

typedef struct {
	CursaAeriana* vector;
	int nrElem;
} heap;

int functieHash(hashT tabela, char* destinatie) {
	return destinatie[0] % tabela.nrElem;
}

void inserareLS(nodLS** prim, CursaAeriana inf) {
	nodLS* nou = (nodLS*)malloc(sizeof(nodLS));
	nou->inf.durata = inf.durata;
	nou->inf.nrPersonal = inf.nrPersonal;
	nou->inf.numarAeronava = inf.numarAeronava;
	nou->inf.destinatie = (char*)malloc(strlen(inf.destinatie) + 1);
	strcpy(nou->inf.destinatie, inf.destinatie);
	nou->inf.plecare = (char*)malloc(strlen(inf.plecare) + 1);
	strcpy(nou->inf.plecare, inf.plecare);
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

void inserareHash(hashT tabela, CursaAeriana cursa) {
	int poz = functieHash(tabela, cursa.destinatie);
	inserareLS(&tabela.vector[poz], cursa);
}

void afisareLS(nodLS* prim) {
	nodLS* temp = prim;
	while (temp != NULL) {
		printf("\tDestinatie: %s, Plecare: %s, ID Aeronava: %d, Nr. Personal: %d, Durata: %5.2f\n",
			temp->inf.destinatie, temp->inf.plecare, temp->inf.numarAeronava, temp->inf.nrPersonal, temp->inf.durata);
		temp = temp->next;
	}
}

void afisareHash(hashT tabela) {
	for (int i = 0; i < tabela.nrElem; i++) {
		if (tabela.vector[i] != NULL) {
			printf("Pozitia %d:\n", i);
			afisareLS(tabela.vector[i]);
		}
	}
}

void stergereLS(nodLS** prim, char* destinatie) {
	nodLS* prev = NULL;
	nodLS* temp = *prim;
	while (temp != NULL) {
		if (strcmp(temp->inf.destinatie, destinatie) == 0) {
			nodLS* deleteMe = temp;
			if (temp == *prim) {
				*prim = (*prim)->next;
			}
			else {
				prev->next = temp->next;
			}
			temp = temp->next;
			free(deleteMe->inf.destinatie);
			free(deleteMe->inf.plecare);
			free(deleteMe);
		}
		else {
			prev = temp;
			temp = temp->next;
		}
	}
}

void stergereHash(hashT tabela, char* destinatie) {
	int poz = functieHash(tabela, destinatie);
	if (tabela.vector[poz] != NULL) {
		stergereLS(&tabela.vector[poz], destinatie);
	}
}

void filtrare(heap h, int index) {
	int indexRad = index;
	int indexS = 2 * index + 1;
	int indexD = 2 * index + 2;

	if (indexS < h.nrElem && h.vector[indexS].durata > h.vector[indexRad].durata) {
		indexRad = indexS;
	}

	if (indexD < h.nrElem && h.vector[indexD].durata > h.vector[indexRad].durata) {
		indexRad = indexD;
	}

	if (index != indexRad) {
		CursaAeriana aux = h.vector[index];
		h.vector[index] = h.vector[indexRad];
		h.vector[indexRad] = aux;

		filtrare(h, indexRad);
	}
}

heap inserareHeap(heap h, CursaAeriana cursa) {
	CursaAeriana* vectorNou = (CursaAeriana*)malloc((h.nrElem + 1) * sizeof(CursaAeriana));
	for (int i = 0; i < h.nrElem; i++) {
		vectorNou[i] = h.vector[i];
	}

	vectorNou[h.nrElem].durata = cursa.durata;
	vectorNou[h.nrElem].nrPersonal = cursa.nrPersonal;
	vectorNou[h.nrElem].numarAeronava = cursa.numarAeronava;
	vectorNou[h.nrElem].destinatie = (char*)malloc(strlen(cursa.destinatie) + 1);
	strcpy(vectorNou[h.nrElem].destinatie, cursa.destinatie);
	vectorNou[h.nrElem].plecare = (char*)malloc(strlen(cursa.plecare) + 1);
	strcpy(vectorNou[h.nrElem].plecare, cursa.plecare);
	h.nrElem++;

	free(h.vector);
	h.vector = vectorNou;

	for (int i = (h.nrElem - 1) / 2; i >= 0; i--) {
		filtrare(h, i);
	}

	return h;
}

heap hash_heap(hashT tabela) {
	heap h;
	h.nrElem = 0;
	h.vector = (CursaAeriana*)malloc(sizeof(CursaAeriana));

	for (int i = 0; i < tabela.nrElem; i++) {
		if (tabela.vector[i] != NULL) {
			nodLS* temp = tabela.vector[i];
			while (temp != NULL) {
				h = inserareHeap(h, temp->inf);
				temp = temp->next;
			}
		}
	}

	return h;
}

void afisareHeap(heap h) {
	for (int i = 0; i < h.nrElem; i++) {
		printf("Durata: %5.2f, Destinatie: %s, Plecare: %s, ID Aeronava: %d, Nr. Personal: %d\n",
			h.vector[i].durata, h.vector[i].destinatie, h.vector[i].plecare,
			h.vector[i].numarAeronava, h.vector[i].nrPersonal);
	}
}

void modificareDurataHeap(heap h, int nrAeronava, char* destinatie, float nouaDurata) {
	for (int i = 0; i < h.nrElem; i++) {
		if (h.vector[i].numarAeronava == nrAeronava &&
			strcmp(h.vector[i].destinatie, destinatie) == 0) {
			h.vector[i].durata = nouaDurata;
		}
	}

	for (int i = (h.nrElem - 1) / 2; i >= 0; i--) {
		filtrare(h, i);
	}
}

int main() {
	hashT tabela;
	tabela.nrElem = 23;
	tabela.vector = (nodLS**)malloc(tabela.nrElem * sizeof(nodLS));
	for (int i = 0; i < tabela.nrElem; i++) {
		tabela.vector[i] = NULL;
	}

	FILE* f = fopen("fisier.txt", "r");

	CursaAeriana cursa;
	char buffer[128];

	while (fscanf(f, "%d", &cursa.numarAeronava) == 1) 
	{
		fscanf(f, " %[^\n]", buffer);
		cursa.plecare = (char*)malloc(strlen(buffer) + 1);
		strcpy(cursa.plecare, buffer);

		fscanf(f, " %[^\n]", buffer);
		cursa.destinatie = (char*)malloc(strlen(buffer) + 1);
		strcpy(cursa.destinatie, buffer);

		fscanf(f, "%f", &cursa.durata);
		fscanf(f, "%d", &cursa.nrPersonal);

		/*printf("Destinatie: %s, Plecare: %s, ID Aeronava: %d, Nr. Personal: %d, Durata: %5.2f\n", 
			cursa.destinatie, cursa.plecare, cursa.numarAeronava, cursa.nrPersonal, cursa.durata);*/
		inserareHash(tabela, cursa);

		free(cursa.destinatie);
		free(cursa.plecare);
	}
	fclose(f);

	printf("Tabela de dispersie din fisier:\n");
	afisareHash(tabela);

	stergereHash(tabela, "Barcelona");
	//stergereHash(tabela, "Buenos Aeres");
	printf("\nTabela de dispersie dupa modificare:\n");
	afisareHash(tabela);

	heap h = hash_heap(tabela);
	printf("\nStructura HEAP:\n");
	afisareHeap(h);

	modificareDurataHeap(h, 567, "Atalanta", 18.56);
	printf("\nStructura HEAP dupa modificare:\n");
	afisareHeap(h);

	return 0;
}