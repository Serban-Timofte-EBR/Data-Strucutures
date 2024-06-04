#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct {
	int id;
	char* numeUtilizator;
	char* parola;
	int nrPrieteni;
	float screenTime;
} Cont;

typedef struct {
	Cont inf;
	struct nodStiva* next;
} nodStiva;

typedef struct {
	Cont* vector;
	int nrElem;
} heap;

void push(nodStiva** prim, Cont cont) {
	nodStiva* nou = (nodStiva*)malloc(sizeof(nodStiva));
	nou->inf.id = cont.id;
	nou->inf.nrPrieteni = cont.nrPrieteni;
	nou->inf.screenTime = cont.screenTime;
	nou->inf.numeUtilizator = (char*)malloc(strlen(cont.numeUtilizator) + 1);
	strcpy(nou->inf.numeUtilizator, cont.numeUtilizator);
	nou->inf.parola = (char*)malloc(strlen(cont.parola) + 1);
	strcpy(nou->inf.parola, cont.parola);
	
	nou->next = *prim;
	*prim = nou;
}

void afisareStiva(nodStiva* prim) {
	nodStiva* temp = prim;
	while (temp != NULL) {
		printf("ID: %d, Prieteni: %d, Screen Time: %f, Utilizator: %s, Parola: %s\n",
			temp->inf.id, temp->inf.nrPrieteni, temp->inf.screenTime, temp->inf.numeUtilizator, temp->inf.parola);
		temp = temp->next;
	}
}

int get(nodStiva** cap, Cont* out) {
	if (*cap == NULL) {
		return 0;
	}

	*out = (*cap)->inf;
	*cap = (*cap)->next;

	return 1;
}

int counterElemente(nodStiva** cap, int pragPrieteni) {
	int counter = 0;

	nodStiva* cap2 = NULL;
	Cont cont;
	while (get(cap, &cont) == 1) {
		if (cont.nrPrieteni > pragPrieteni) {
			counter++;
		}
		push(&cap2, cont);
		free(cont.numeUtilizator);
		free(cont.parola);
	}

	*cap = cap2;

	return counter;
}

Cont* conturiPeste100DePrieteni(nodStiva** cap, int* nrElem, int pragPrieteni) {
	nodStiva* cap2 = NULL;

	*nrElem = counterElemente(cap, pragPrieteni);
	Cont* vector = (Cont*)malloc(*nrElem * sizeof(Cont));
	int index = 0;

	Cont out;

	while (get(cap, &out) == 1)
	{
		if (out.nrPrieteni > pragPrieteni) {
			vector[index] = out;
			index++;
		}
		push(&cap2, out);
	}
	*cap = cap2;

	return vector;
}

void filtrare(heap h, int index) {
	int indexRad = index;
	int indexS = 2 * index + 1;
	int indexD = 2 * index + 2;

	if (indexS < h.nrElem && h.vector[indexS].nrPrieteni > h.vector[indexRad].nrPrieteni) {
		indexRad = indexS;
	}

	if (indexD < h.nrElem && h.vector[indexD].nrPrieteni > h.vector[indexRad].nrPrieteni) {
		indexRad = indexD;
	}

	if (indexRad != index) {
		Cont aux = h.vector[index];
		h.vector[index] = h.vector[indexRad];
		h.vector[indexRad] = aux;

		filtrare(h, indexRad);
	}
}

heap inserareHeap(heap h, Cont c) {
	Cont* vectorNou = (Cont*)malloc((h.nrElem + 1) * sizeof(Cont));
	for (int i = 0; i < h.nrElem; i++) {
		vectorNou[i] = h.vector[i];
	}

	vectorNou[h.nrElem].id = c.id;
	vectorNou[h.nrElem].nrPrieteni = c.nrPrieteni;
	vectorNou[h.nrElem].screenTime = c.screenTime;
	vectorNou[h.nrElem].numeUtilizator = (char*)malloc(strlen(c.numeUtilizator) + 1);
	strcpy(vectorNou[h.nrElem].numeUtilizator, c.numeUtilizator);
	vectorNou[h.nrElem].parola = (char*)malloc(strlen(c.parola) + 1);
	strcpy(vectorNou[h.nrElem].parola, c.parola);
	h.nrElem++;

	free(h.vector);
	h.vector = vectorNou;

	for (int i = (h.nrElem - 1) / 2; i >= 0; i--) {
		filtrare(h, i);
	}

	return h;
}

heap stiva_heap(nodStiva** cap) {
	heap h;
	h.nrElem = 0;
	h.vector = (Cont*)malloc(sizeof(Cont));

	Cont out;
	while (get(cap, &out) == 1)
	{
		h = inserareHeap(h, out);
		free(out.numeUtilizator);
		free(out.parola);
	}

	return h;
}

void afisareHeap(heap h) {
	for (int i = 0; i < h.nrElem; i++) {
		printf("ID: %d, Prieteni: %d, Screen Time: %f, Utilizator: %s, Parola: %s\n",
			h.vector[i].id, h.vector[i].nrPrieteni, h.vector[i].screenTime,
			h.vector[i].numeUtilizator, h.vector[i].parola);
	}
}

heap stergerHeapConditie(heap h, int screenTimePrag) {
	for (int i = 0; i < h.nrElem; i++) {
		if (h.vector[i].screenTime < screenTimePrag) {
			Cont* vectorNou = (Cont*)malloc((h.nrElem - 1) * sizeof(Cont));

			Cont aux = h.vector[i];
			h.vector[i] = h.vector[h.nrElem - 1];
			free(aux.numeUtilizator);
			free(aux.parola);
			h.nrElem--;

			for (int j = 0; j < h.nrElem; j++) {
				vectorNou[j] = h.vector[j];
			}

			free(h.vector);
			h.vector = vectorNou;

			i--;
		}
	}

	for (int i = (h.nrElem - 1) / 2; i >= 0; i--) {
		filtrare(h, i);
	}

	return h;
}

int main() {
	nodStiva* cap = NULL;

	FILE* f = fopen("fisier.txt", "r");

	Cont c;
	char buffer[128];

	while (fscanf(f, "%d", &c.id) == 1)
	{
		fscanf(f, " %[^\n]", buffer);
		c.numeUtilizator = (char*)malloc(strlen(buffer) + 1);
		strcpy(c.numeUtilizator, buffer);

		fscanf(f, " %[^\n]", buffer);
		c.parola = (char*)malloc(strlen(buffer) + 1);
		strcpy(c.parola, buffer);

		fscanf(f, "%d", &c.nrPrieteni);
		fscanf(f, "%f", &c.screenTime);

		/*printf("ID: %d, Prieteni: %d, Screen Time: %f, Utilizator: %s, Parola: %s\n", 
			c.id, c.nrPrieteni, c.screenTime, c.numeUtilizator, c.parola);*/
		push(&cap, c);

		free(c.numeUtilizator);
		free(c.parola);
	}
	fclose(f);

	printf("Stiva din fisier:\n");
	afisareStiva(cap);

	// vectoru cu mai mult de 100 de prieteni
	//Cont cont;
	//get(&cap, &cont);
	//printf("\n%d\n", cont.id);
	//afisareStiva(cap);

	/*int nrElem = counterElemente(&cap, 100);
	printf("\n%d\n", nrElem);
	afisareStiva(cap);*/

	int nrElem = 0;
	Cont* vector = conturiPeste100DePrieteni(&cap, &nrElem, 100);
	printf("\nVectorul:\n");
	for (int i = 0; i < nrElem; i++) {
		printf("Prieteni: %d, ID: %d, Screen Time: %f, Utilizator: %s, Parola: %s\n",
			vector[i].nrPrieteni, vector[i].id, vector[i].screenTime, vector[i].numeUtilizator, vector[i].parola);
	}

	printf("\nStiva refacuta:\n");
	afisareStiva(cap);

	heap h = stiva_heap(&cap);
	printf("\nStructura HEAP:\n");
	afisareHeap(h);

	// stergere daca screentime < 100
	h = stergerHeapConditie(h, 100);
	printf("\nHEAP-ul dupa stergere:\n");
	afisareHeap(h);

	return 0;
}