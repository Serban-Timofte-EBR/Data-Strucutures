#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#define decolat 0
#define aterizat 1
#define intarziat 2

typedef struct {
	char* cod;
	char* destinatie;
	char* data_plecare;
	char* ora_plecare;
	int status;
} CursaAeriana;

typedef struct {
	CursaAeriana cursa;
	struct NodLS* next;
} NodLS;

typedef struct {
	NodLS** vector;
	int nrElem;
} hashT;

int functie_hash(char* destinatie, hashT tabela) {
	return destinatie[0] % tabela.nrElem;
}

void inserare_ls(NodLS** prim, CursaAeriana cursa) {
	NodLS* nou = (NodLS*)malloc(sizeof(NodLS));
	nou->cursa.cod = (char*)malloc(strlen(cursa.cod) + 1);
	strcpy(nou->cursa.cod, cursa.cod);
	nou->cursa.destinatie = (char*)malloc(strlen(cursa.destinatie) + 1);
	strcpy(nou->cursa.destinatie, cursa.destinatie);
	nou->cursa.data_plecare = (char*)malloc(strlen(cursa.data_plecare) + 1);
	strcpy(nou->cursa.data_plecare, cursa.data_plecare);
	nou->cursa.ora_plecare = (char*)malloc(strlen(cursa.ora_plecare) + 1);
	strcpy(nou->cursa.ora_plecare, cursa.ora_plecare);
	nou->cursa.status = cursa.status;
	nou->next = NULL;

	if (*prim == NULL) {
		*prim = nou;
	}
	else {
		NodLS* temp = *prim;
		while (temp->next != NULL)
		{
			temp = temp->next;
		}
		temp->next = nou;
	}
}

void inserare_hash(hashT tabela, CursaAeriana cursa) {
	int poz = functie_hash(cursa.destinatie, tabela);
	inserare_ls(&tabela.vector[poz], cursa);
}

void traversare_ls(NodLS* prim) {
	NodLS* temp = prim;
	while (temp != NULL)
	{
		printf("Cod: %s \t Destinatie: %s \t Data: %s \t Ora: %s \t Status: %d\n",
			temp->cursa.cod, temp->cursa.destinatie, temp->cursa.data_plecare, temp->cursa.ora_plecare, temp->cursa.status);
		temp = temp->next;
	}
}

void traversare_hash(hashT tabela) {
	for (int i = 0; i < tabela.nrElem; i++)
	{
		if (tabela.vector[i] != NULL) {
			printf("Pozitie = %d\n", i);
			traversare_ls(tabela.vector[i]);
		}
	}
}

int counter_curse(hashT tabela, char* destinatia) {
	int counter = 0;
	int poz = functie_hash(destinatia, tabela);
	if (tabela.vector[poz] != NULL) {
		NodLS* temp = tabela.vector[poz];
		while (temp != NULL)
		{
			if (strcmp(temp->cursa.destinatie, destinatia) == 0) {
				counter++;
			}
			temp = temp->next;
		}
	}
	return counter;
}

void sterge_destinatia(hashT tabela, char* destinatia) {
	int poz = functie_hash(destinatia, tabela);
	if (tabela.vector[poz] != NULL) {
		NodLS* temp = tabela.vector[poz];
		NodLS* prev = NULL;
		while (temp != NULL)
		{
			if (strcmp(temp->cursa.destinatie, destinatia) == 0) {
				NodLS* deleteMe = temp;
				if (prev == NULL) {
					tabela.vector[poz] = temp->next;
				}
				else {
					prev->next = temp->next;
				}
				temp = temp->next;
				free(deleteMe->cursa.cod);
				free(deleteMe->cursa.destinatie);
				free(deleteMe->cursa.data_plecare);
				free(deleteMe->cursa.ora_plecare);
			}
			else {
				prev = temp;
				temp = temp->next;
			}
		}
	}
}

int curse_intarziate(hashT tabela) {
	int counter = 0;
	for (int i = 0; i < tabela.nrElem; i++) {
		if (tabela.vector[i] != NULL) {
			NodLS* temp = tabela.vector[i];
			while (temp != NULL)
			{
				if (temp->cursa.status == intarziat) {
					counter++;
				}
				temp = temp->next;
			}
		}
	}
	return counter;
}

void salveaza_vector_intarziat(hashT tabela, CursaAeriana** vector, int* nrElem) {
	for (int i = 0; i < tabela.nrElem; i++)
	{
		if (tabela.vector[i] != NULL) {
			NodLS* temp = tabela.vector[i];
			while (temp != NULL)
			{
				if (temp->cursa.status == intarziat) {		//aici avem hardcodat statusul
					(*vector)[*nrElem] = temp->cursa;
					(*nrElem)++;
				}
				temp = temp->next;
			}
		}
	}
}

int main() {
	hashT tabela;
	tabela.nrElem = 13;
	tabela.vector = (NodLS**)malloc(tabela.nrElem * sizeof(NodLS*));
	for (int i = 0; i < tabela.nrElem; i++) {
		tabela.vector[i] = NULL;
	}

	FILE* f = fopen("Curse.txt", "r");

	char buffer[128];
	CursaAeriana cursa;
	while (fscanf(f, "%s", buffer) == 1)
	{
		cursa.cod = (char*)malloc(strlen(buffer) + 1);
		strcpy(cursa.cod, buffer);

		fscanf(f, "%s", buffer);
		cursa.destinatie = (char*)malloc(strlen(buffer) + 1);
		strcpy(cursa.destinatie, buffer);

		fscanf(f, "%s", buffer);
		cursa.data_plecare = (char*)malloc(strlen(buffer) + 1);
		strcpy(cursa.data_plecare, buffer);

		fscanf(f, "%s", buffer);
		cursa.ora_plecare = (char*)malloc(strlen(buffer) + 1);
		strcpy(cursa.ora_plecare, buffer);

		fscanf(f, "%d", &cursa.status);

		inserare_hash(tabela, cursa);

		free(cursa.cod);
		free(cursa.data_plecare);
		free(cursa.destinatie);
		free(cursa.ora_plecare);
	}
	fclose(f);
	traversare_hash(tabela);

	int counterBarcelona = counter_curse(tabela, "Barcelona");
	printf("\nCurse la Barcelona: %d\n", counterBarcelona);

	sterge_destinatia(tabela, "Barcelona");
	printf("\nStergere apelata\n");
	int counterBarcelona2 = counter_curse(tabela, "Barcelona");
	printf("\nCurse la Barcelona 2: %d\n", counterBarcelona2);

	printf("\n\n");
	traversare_hash(tabela);

	int nrCurseIntarziate = curse_intarziate(tabela);
	CursaAeriana* vector = (CursaAeriana*)malloc(nrCurseIntarziate * sizeof(CursaAeriana));
	int nrElem = 0;
	salveaza_vector_intarziat(tabela, &vector, &nrElem);
	printf("\nVectorul:\n");
	for (int i = 0; i < nrElem; i++) {
		printf("Cod: % s \t Destinatie : % s \t Data : % s \t Ora : % s \t Status : %d\n",
			vector[i].cod, vector[i].destinatie, vector[i].data_plecare, vector[i].ora_plecare, vector[i].status);
	}
	return 0;
}