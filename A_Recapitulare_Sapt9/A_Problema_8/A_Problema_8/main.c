#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

typedef struct {
	char iban[25];
	char* titular;
	char moneda[4];
	float sold;
} ContBancar;

typedef struct {
	ContBancar cont;
	struct NodLS* next;
} NodLS;

typedef struct {
	NodLS** vector;
	int nrElem;
} hashT;

typedef struct {
	char iban[25];
	char* titular;
} DateClient;

int functie_hash(hashT tabela, ContBancar cont) {
	return cont.titular[0] % tabela.nrElem;
}

int functie_hash_2(hashT tabela, char* titular) {
	return titular[0] % tabela.nrElem;
}

void inserare_ls(NodLS** prim, ContBancar cont) {
	NodLS* nou = (NodLS*)malloc(sizeof(NodLS));
	strcpy(nou->cont.iban, cont.iban);
	strcpy(nou->cont.moneda, cont.moneda);
	nou->cont.sold = cont.sold;
	nou->cont.titular = (char*)malloc(strlen(cont.titular) + 1);
	strcpy(nou->cont.titular, cont.titular);
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

void inserare_hash(hashT tabela, ContBancar cont) {
	int poz = functie_hash(tabela, cont);
	inserare_ls(&tabela.vector[poz], cont);
}

void traversare_ls(NodLS* prim) {
	NodLS* temp = prim;
	while (temp != NULL) {
		printf("\tIBAN: %s \t Titular: %s \t Moneda: %s \t Sold: %f\n", temp->cont.iban, temp->cont.titular, temp->cont.moneda, temp->cont.sold);
		temp = temp->next;
	}
}

void traversare_hash(hashT tabela) {
	for (unsigned char i = 0; i < tabela.nrElem; i++)
	{
		if (tabela.vector[i] != NULL) {
			printf("Pozitia = %d\n", i);
			traversare_ls(tabela.vector[i]);
		}
	}
}

DateClient* extragere_conturi_euro(hashT tabela, DateClient* vector, int* nrElem) {
	for (unsigned char i = 0; i < tabela.nrElem; i++) {
		if (tabela.vector[i] != NULL) {
			NodLS* temp = tabela.vector[i];
			NodLS* prev = NULL;
			while (temp != NULL)
			{
				if (strcmp(temp->cont.moneda, "EUR") == 0) {
					strcpy(vector[*nrElem].iban, temp->cont.iban);
					vector[*nrElem].titular = (char*)malloc(strlen(temp->cont.titular) + 1);
					strcpy(vector[*nrElem].titular, temp->cont.titular);
					(*nrElem)++;

					NodLS* deleteMe = temp;
					if (prev == NULL) {
						tabela.vector[i] = temp->next;
					}
					else if (temp == NULL) {
						prev->next = NULL;
					}
					else {
						prev->next = temp->next;
					}
					temp = temp->next;
					free(deleteMe->cont.titular);
					free(deleteMe);
				}
				else {
					prev = temp;
					temp = temp->next;
				}
			}
		}
	}
	return vector;
}

void stergere_nod_hash(hashT tabela, char* titular) {
	int poz = functie_hash_2(tabela, titular);
	if (tabela.vector[poz] != NULL) {
		NodLS* temp = tabela.vector[poz];
		NodLS* prev = NULL;
		while (temp != NULL)
		{
			if (strcmp(temp->cont.titular, titular) == 0) {
				NodLS* deleteMe = temp;
				if (prev == NULL) {
					tabela.vector[poz] = temp->next;
				}
				else if (temp == NULL) {
					prev->next = NULL;
				}
				else {
					prev->next = temp->next;
				}
				temp = temp->next;
				free(deleteMe->cont.titular);
				free(deleteMe);
			}
			else {
				prev = temp;
				temp = temp->next;
			}
		}
	}
}

void dezalocare_vector(DateClient* vector, int nrElem) {
	for (unsigned char i = 0; i < nrElem; i++) {
		free(vector[i].titular);
	}
	free(vector);
}

void dezalocare_ls(NodLS* prim) {
	NodLS* temp = prim;
	while (temp != NULL)
	{
		NodLS* aux = temp->next;
		free(temp->cont.titular);
		free(temp);
		temp = aux;
	}
}

void dezalocare_hash(hashT tabela) {
	for (unsigned char i = 0; i < tabela.nrElem; i++)
	{
		if (tabela.vector[i] != NULL) {
			dezalocare_ls(tabela.vector[i]);
		}
	}
}

int main() {
	hashT tabela;
	tabela.nrElem = 13;
	tabela.vector = (NodLS**)malloc(tabela.nrElem * sizeof(NodLS*));
	for (unsigned char i = 0; i < tabela.nrElem; i++) {
		tabela.vector[i] = NULL;
	}

	FILE* f = fopen("Conturi.txt", "r");
	if (f == NULL) {
		printf("Fisierul nu s-a deschis");
		return -1;
	}

	char buffer[128];
	ContBancar cont;

	while (fscanf(f, "%s", cont.iban) == 1) 
	{
		fscanf(f, " %[^\n]", buffer);
		cont.titular = (char*)malloc(strlen(buffer) + 1);
		strcpy(cont.titular, buffer);

		fscanf(f, "%s", cont.moneda);
		fscanf(f, "%f", &cont.sold);

		inserare_hash(tabela, cont);

		free(cont.titular);
	}

	printf("\nTabela hash din fisier\n");
	traversare_hash(tabela);

	//extrag si sterg conturile in EUR
	DateClient* vector_clienti_eur = (DateClient*)malloc(5 * sizeof(DateClient));
	int nrClientiVector = 0;
	vector_clienti_eur = extragere_conturi_euro(tabela, vector_clienti_eur, &nrClientiVector);
	printf("\nVectorul\n");
	for (unsigned char i = 0; i < nrClientiVector; i++) {
		printf("IBAN: %s \t Titular: %s \n", vector_clienti_eur[i].iban, vector_clienti_eur[i].titular);
	}

	printf("\nTabela hash dupa extragere\n");
	traversare_hash(tabela);

	stergere_nod_hash(tabela, "Parvulescu Cosmina");
	printf("\nTabela hash dupa stergere\n");
	traversare_hash(tabela);

	dezalocare_vector(vector_clienti_eur, nrClientiVector);
	dezalocare_hash(tabela);

	return 0;
}