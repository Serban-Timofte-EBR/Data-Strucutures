#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct {
	int id;
	char* titlu;
	int nrAutori;
	char** autori;
	float pret;
} Carte;

typedef struct {
	Carte inf;
	struct nodStiva* next;
} nodStiva;

typedef struct {
	Carte inf;
	struct nodLD* next;
	struct nodLD* prev;
} nodLD;

nodStiva* push(nodStiva* cap, Carte carte) {
	nodStiva* nou = (nodStiva*)malloc(sizeof(nodStiva));
	nou->inf.id = carte.id;
	nou->inf.pret = carte.pret;
	nou->inf.nrAutori = carte.nrAutori;
	nou->inf.titlu = (char*)malloc(strlen(carte.titlu) + 1);
	strcpy(nou->inf.titlu, carte.titlu);
	nou->inf.autori = (char**)malloc(nou->inf.nrAutori * sizeof(char*));
	for (int i = 0; i < nou->inf.nrAutori; i++) {
		nou->inf.autori[i] = (char*)malloc(strlen(carte.autori[i]) + 1);
		strcpy(nou->inf.autori[i], carte.autori[i]);
	}
	nou->next = NULL;

	if(cap == NULL) {
		cap = nou;
	}
	else {
		nou->next = cap;
		cap = nou;
	}

	return cap;
}

int pop(nodStiva** cap, Carte* output) {
	if (*cap == NULL) {
		return 0;
	}

	(*output).id = (*cap)->inf.id;
	(*output).pret = (*cap)->inf.pret;
	(*output).nrAutori = (*cap)->inf.nrAutori;
	(*output).titlu = (char*)malloc(strlen((*cap)->inf.titlu) + 1);
	strcpy((*output).titlu, (*cap)->inf.titlu);
	(*output).autori = (char**)malloc((*output).nrAutori * sizeof(char*));
	for (int i = 0; i < (*output).nrAutori; i++) {
		(*output).autori[i] = (char*)malloc(strlen((*cap)->inf.autori[i]) + 1);
		strcpy((*output).autori[i], (*cap)->inf.autori[i]);
	}

	nodStiva* temp = *cap;
	*cap = (*cap)->next;

	free(temp->inf.titlu);
	for (int i = 0; i < temp->inf.nrAutori; i++) {
		free(temp->inf.autori[i]);
	}
	free(temp->inf.autori);

	return 1;
}

void afisareStiva(nodStiva* cap) {
	nodStiva* temp = cap;
	while (temp != NULL)
	{
		printf("ID: %d, Pret: %5.2f, Nr. Autori: %d, Titlu: %s\n", temp->inf.id, temp->inf.pret, temp->inf.nrAutori, temp->inf.titlu);
		for (int i = 0; i < temp->inf.nrAutori; i++) {
			printf("\t%s\n", temp->inf.autori[i]);
		}
		temp = temp->next;
	}
}

void inserareLD(nodLD** prim, nodLD** ultim, Carte carte) {
	nodLD* nou = (nodLD*)malloc(sizeof(nodLD));
	nou->inf.id = carte.id;
	nou->inf.pret = carte.pret;
	nou->inf.nrAutori = carte.nrAutori;
	nou->inf.titlu = (char*)malloc(strlen(carte.titlu) + 1);
	strcpy(nou->inf.titlu, carte.titlu);
	nou->inf.autori = (char**)malloc(nou->inf.nrAutori * sizeof(char*));
	for (int i = 0; i < nou->inf.nrAutori; i++) {
		nou->inf.autori[i] = (char*)malloc(strlen(carte.autori[i]) + 1);
		strcpy(nou->inf.autori[i], carte.autori[i]);
	}

	nou->next = NULL;
	nou->prev = NULL;

	if (*prim == NULL) {
		*prim = nou;
		*ultim = nou;
	}
	else {
		nodLD* temp = *prim;
		while (temp != NULL && temp->inf.pret > carte.pret)
		{
			temp = temp->next;
		}

		if (temp == *prim) {
			nou->next = *prim;
			(*prim)->prev = nou;
			*prim = nou;
		} 

		else if (temp == NULL) {
			(*ultim)->next = nou;
			nou->prev = (*ultim);
			*ultim = nou;
		}

		else {
			nodLD* ant = temp->prev;
			ant->next = nou;
			nou->next = temp;
			nou->prev = ant;
			temp->prev = nou;
		}
	}
}

void generareListeDuble(nodLD** primPest50, nodLD** ultimPeste50, nodLD** primSub50, nodLD** ultimSub50, nodStiva** cap) {
	Carte aux;
	while (pop(cap, &aux) == 1)
	{
		if (aux.pret >= 50) {
			inserareLD(primPest50, ultimPeste50, aux);
		}
		else {
			inserareLD(primSub50, ultimSub50, aux);
		}
	}
}

void afisareCapCoadaLD(nodLD* prim) {
	nodLD* temp = prim;
	while (temp != NULL)
	{
		printf("ID: %d, Pret: %5.2f, Nr. Autori: %d, Titlu: %s\n", temp->inf.id, temp->inf.pret, temp->inf.nrAutori, temp->inf.titlu);
		for (int i = 0; i < temp->inf.nrAutori; i++) {
			printf("\t%s\n", temp->inf.autori[i]);
		}
		temp = temp->next;
	}
}

void afisareCoadaCapLD(nodLD* ultim) {
	nodLD* temp = ultim;
	while (temp != NULL)
	{
		printf("ID: %d, Pret: %5.2f, Nr. Autori: %d, Titlu: %s\n", temp->inf.id, temp->inf.pret, temp->inf.nrAutori, temp->inf.titlu);
		for (int i = 0; i < temp->inf.nrAutori; i++) {
			printf("\t%s\n", temp->inf.autori[i]);
		}
		temp = temp->prev;
	}
}

int counterNrCarti(nodLD* primPeste50, nodLD* primSub50, char* nume) {
	int counter = 0;
	nodLD* temp = primPeste50;
	while (temp != NULL)
	{
		for (int i = 0; i < temp->inf.nrAutori; i++) {
			if (strcmp(temp->inf.autori[i], nume) == 0) {
				counter++;
			}
		}
		temp = temp->next;
	}

	temp = primSub50;
	while (temp != NULL)
	{
		for (int i = 0; i < temp->inf.nrAutori; i++) {
			if (strcmp(temp->inf.autori[i], nume) == 0) {
				counter++;
			}
		}
		temp = temp->next;
	}

	return counter;
}

Carte* vectorulCuCartileUnuiAutor(nodLD* primPeste50, nodLD* primSub50, int* nrElem, char* autor) {
	*nrElem = counterNrCarti(primPeste50, primSub50, autor);
	if (*nrElem > 0) {
		Carte* vector = (Carte*)malloc(*nrElem * sizeof(Carte));
		int index = 0;

		nodLD* temp = primPeste50;
		while (temp != NULL)
		{
			for (int i = 0; i < temp->inf.nrAutori; i++) {
				if (strcmp(temp->inf.autori[i], autor) == 0) {
					vector[index] = temp->inf;
					index++;
				}
			}
			temp = temp->next;
		}

		temp = primSub50;
		while (temp != NULL)
		{
			for (int i = 0; i < temp->inf.nrAutori; i++) {
				if (strcmp(temp->inf.autori[i], autor) == 0) {
					vector[index] = temp->inf;
					index++;
				}
			}
			temp = temp->next;
		}

		return vector;
	}
	else {
		return NULL;
	}
}

int main() {
	nodStiva* cap = NULL;

	FILE* f = fopen("fisier.txt", "r");

	Carte carte;
	char buffer[128];

	while (fscanf(f, "%d", &carte.id) == 1)
	{
		fscanf(f, " %[^\n]", buffer);
		carte.titlu = (char*)malloc(strlen(buffer) + 1);
		strcpy(carte.titlu, buffer);

		fscanf(f, "%d", &carte.nrAutori);
		carte.autori = (char**)malloc(carte.nrAutori * sizeof(char*));
		for (int i = 0; i < carte.nrAutori; i++) {
			fscanf(f, " %[^\n]", buffer);
			carte.autori[i] = (char*)malloc(strlen(buffer) + 1);
			strcpy(carte.autori[i], buffer);
		}

		fscanf(f, "%f", &carte.pret);

		cap = push(cap, carte);

		free(carte.titlu);
		for (int i = 0; i < carte.nrAutori; i++) {
			free(carte.autori[i]);
		}
		free(carte.autori);
	}
	fclose(f);

	printf("Stiva din fisier:\n");
	afisareStiva(cap);

	/*Carte output;
	int res = pop(&cap, &output);
	if (res == 1) {
		printf("\nID: %d, Titlu: %s\n", output.id, output.titlu);
	}

	printf("\n\n");
	afisareStiva(cap);*/

	nodLD* primPeste50 = NULL;
	nodLD* ultimPeste50 = NULL;
	nodLD* primSub50 = NULL;
	nodLD* ultimSub50 = NULL;

	generareListeDuble(&primPeste50, &ultimPeste50, &primSub50, &ultimSub50, &cap);

	printf("\nStiva dupa extrageri:\n");
	afisareStiva(cap);

	printf("\nLista cu carti PESTE 50 lei:\n");
	afisareCapCoadaLD(primPeste50);

	printf("\nLista cu carti SUB 50 lei:\n");
	afisareCapCoadaLD(primSub50);

	printf("\nLista cu carti PESTE 50 lei COADA -> CAP:\n");
	afisareCoadaCapLD(ultimPeste50);

	printf("\nLista cu carti SUB 50 lei COADA -> CAP:\n");
	afisareCoadaCapLD(ultimSub50);

	int nrCarti = 0;
	Carte* vectorCartiAutor = vectorulCuCartileUnuiAutor(primPeste50, primSub50, &nrCarti, "Mihaela Lupu");
	printf("\nVectorul:\n");
	for (int i = 0; i < nrCarti; i++) {
		printf("ID: %d, Pret: %5.2f, Nr. Autori: %d, Titlu: %s\n",
			vectorCartiAutor[i].id, vectorCartiAutor[i].pret, vectorCartiAutor[i].nrAutori, vectorCartiAutor[i].titlu);
		for (int j = 0; j < vectorCartiAutor[i].nrAutori; j++) {
			printf("\t%s\n", vectorCartiAutor[i].autori[j]);
		}
	}

	return 0;
}