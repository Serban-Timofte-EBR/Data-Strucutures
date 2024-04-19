#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#define HASH_TABLE_SIZE 200;

typedef struct {
	char iban[25];
	char* titular;
	char moneda[4];
	float sold;
} ContBancar;

typedef struct {
	ContBancar data;
	struct NodLS* next;
} NodLS;

typedef struct {
	NodLS** vector;
	int nrElem;
} hashT;

typedef struct {
	ContBancar* cont;
	struct NodLS_Secundara* next;
} NodLS_Secundara;

int functie_hash(char* cheia, hashT tabela) {
	return cheia[0] % tabela.nrElem;
}

void inserare_lista_simpla(NodLS** prim, ContBancar cont) {
	NodLS* nou = (NodLS*)malloc(sizeof(NodLS));
	strcpy(nou->data.iban, cont.iban);
	nou->data.titular = (char*)malloc(strlen(cont.titular) + 1);
	strcpy(nou->data.titular, cont.titular);
	strcpy(nou->data.moneda, cont.moneda);
	nou->data.sold = cont.sold;
	nou->next = NULL;

	if (*prim == NULL)
		*prim = nou;
	else
	{
		NodLS* temp = *prim;
		while (temp->next != NULL)
		{
			temp = temp->next;
		}
		temp->next = nou;
	}
}

void inserare_hash_table(hashT tabela, ContBancar cont) {
	int poz = functie_hash(cont.titular, tabela);
	inserare_lista_simpla(&tabela.vector[poz], cont);
}

NodLS_Secundara* inserara_lista_secundara_inceput(NodLS* prim, ContBancar* cont) {
	NodLS_Secundara* nou = (NodLS_Secundara*)malloc(sizeof(NodLS_Secundara));
	nou->cont = cont;
	if (prim == NULL) {
		nou->next = NULL;
		prim = nou;
	}
	else {
		nou->next = prim;
		prim = nou;
	}
	return prim;
}

//extragerea conturilor unui titular intr-o lista simpla separata
NodLS_Secundara* extragere_conturi_titular(NodLS_Secundara* prim, hashT tabela, char* titular) {
	int poz = functie_hash(titular, tabela);
	if (tabela.vector[poz] != NULL) {
		NodLS* temp = tabela.vector[poz];
		while (temp != NULL)
		{
			if (strcmp(temp->data.titular, titular) == 0) {
				prim = inserara_lista_secundara_inceput(prim, &temp->data);
			}
			temp = temp->next;
		}
	}
	return prim;
}

hashT stergere_cont_titulari(hashT tabela, char* titular) {
	int poz = functie_hash(titular, tabela);
	NodLS* temp = tabela.vector[poz];
	NodLS* prev = NULL;

	while (temp != NULL)
	{
		if (strcmp(temp->data.titular, titular) == 0) {
			// Capul listei
			if (prev == NULL) {
				tabela.vector[poz] = temp->next;
			}
			// Nod in mijlocul listei
			else
			{
				prev->next = temp->next;
			}
			NodLS* toBeDeleted = temp;
			prev = temp;
			temp = temp->next;

			free(toBeDeleted->data.titular);
			free(toBeDeleted);
		}
		else {
			prev = temp;
			temp = temp->next;
		}
	}
	return tabela;
}

void printare_lista_simpla(NodLS* prim) {
	NodLS* temp = prim;
	while (temp != NULL)
	{
		printf("IBAN: %s \t Titular: %s \t Moneda: %s \t Sold: %f\n\t", 
			temp->data.iban, temp->data.titular, temp->data.moneda, temp->data.sold);
		temp = temp->next;
	}
}

void printare_hash_table(hashT tabela) {
	for (unsigned char i = 0; i < tabela.nrElem; i++) {
		if (tabela.vector[i] != NULL) {
			printf("\nPozitie = %d\n\t", i);
			printare_lista_simpla(tabela.vector[i]);
		}
	}
}

int main() {
	hashT tabela;
	tabela.nrElem = 23;
	tabela.vector = (NodLS**)malloc(tabela.nrElem * sizeof(NodLS*));
	for (unsigned char i = 0; i < tabela.nrElem; i++) {
		tabela.vector[i] = NULL;
	}

	FILE* f = fopen("Conturi.txt", "r");
	if (f == NULL) {
		printf("Fisierul nu s-a deschis!");
		return -1;
	}

	char buffer[128];
	ContBancar contB;
	while (fscanf(f, "%s", contB.iban) == 1)
	{
		fscanf(f, " %[^\n]", buffer);
		contB.titular = (char*)malloc(strlen(buffer) + 1);
		strcpy(contB.titular, buffer);

		fscanf(f, "%s", contB.moneda);
		fscanf(f, "%f", &contB.sold);

		inserare_hash_table(tabela, contB);

		free(contB.titular);
	}
	fclose(f);

	printf("Tabela de dispersie citita din fisier:\n");
	printare_hash_table(tabela);

	ContBancar contSerban;
	strcpy(contSerban.iban, "RO04INGB1234567890111222");
	contSerban.titular = (char*)malloc(strlen("Serban Timofte") + 1);
	strcpy(contSerban.titular, "Serban Timofte");
	strcpy(contSerban.moneda, "EUR");
	contSerban.sold = 100000;

	inserare_hash_table(tabela, contSerban);
	printf("\n------------------------------------------------------------------------------------------------------------------------\n");
	printf("Tabela dupa inserarea unui nod\n");
	printare_hash_table(tabela);

	NodLS_Secundara* prim_conturi_titular = (NodLS_Secundara*)malloc(sizeof(NodLS_Secundara));
	prim_conturi_titular = NULL;
	prim_conturi_titular = extragere_conturi_titular(prim_conturi_titular, tabela, "Popescu Iulian");
	printf("\n------------------------------------------------------------------------------------------------------------------------\n");
	printf("Lista conturilor extrase\n");
	NodLS_Secundara* temp = prim_conturi_titular;
	while (temp != NULL)
	{
		printf("IBAN: %s \t Titular: %s \t Moneda: %s \t Sold: %f\n",
			temp->cont->iban, temp->cont->titular, temp->cont->moneda, temp->cont->sold);
		temp = temp->next;
	}

	tabela = stergere_cont_titulari(tabela, "Popescu Iulian");
	printf("\nTabela dupa stergere\n");
	printare_hash_table(tabela);

	return 0;
}