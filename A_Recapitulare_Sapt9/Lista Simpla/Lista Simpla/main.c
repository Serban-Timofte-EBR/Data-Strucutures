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
	ContBancar cb;
	struct NodLS* next;
} NodLS;

typedef struct {
	char* iban;
	struct NodLS2* next;
} NodLS2;

void printare_lista_simplar(NodLS* prim) {
	NodLS* temp = prim;
	while (temp != NULL)
	{
		printf("IBAN: %s \t Titluar: %s \t Moneda: %s \t Sold: %f\n", temp->cb.iban, temp->cb.titular, temp->cb.moneda, temp->cb.sold);
		temp = temp->next;
	}
}

NodLS* inserare_sfarsit(NodLS* prim, ContBancar cont) {
	NodLS* nou = (NodLS*)malloc(sizeof(NodLS));
	strcpy(nou->cb.iban, cont.iban);

	nou->cb.titular = (char*)malloc(sizeof(char) * (strlen(cont.titular) + 1));
	strcpy(nou->cb.titular, cont.titular);

	strcpy(nou->cb.moneda, cont.moneda);
	nou->cb.sold = cont.sold;

	nou->next = NULL;

	if (prim == NULL) {
		prim = nou;
	}
	else
	{
		NodLS* temp = prim;
		while (temp->next != NULL)
		{
			temp = temp->next;
		}
		temp->next = nou;
	}

	return prim;
}

void inserare_inceput(NodLS** prim, ContBancar cont) {
	NodLS* nou = (NodLS*)malloc(sizeof(NodLS));
	strcpy(nou->cb.iban, cont.iban);

	nou->cb.titular = (char*)malloc(sizeof(char*) * (strlen(cont.titular) + 1));
	strcpy(nou->cb.titular, cont.titular);

	strcpy(nou->cb.moneda, cont.moneda);
	nou->cb.sold = cont.sold;

	if (*prim == NULL) {
		*prim = nou;
	}
	else
	{
		nou->next = *prim;
		*prim = nou;
	}
}

void salvare_conturi_euro_in_vector(ContBancar* vector, NodLS* prim, int* nrElem) {
	if (prim == NULL) {
		return;
	}
	else {
		NodLS* temp = prim;
		while (temp != NULL)
		{
			if (strcmp(temp->cb.moneda, "EUR") == 0) {
				/*strcpy(vector[*nrElem].iban, temp->cb.iban);
				vector[*nrElem].titular = (char*)malloc(strlen(temp->cb.titular) + 1);
				strcpy(vector[*nrElem].titular, temp->cb.titular);
				strcpy(vector[*nrElem].moneda, temp->cb.moneda);
				vector[*nrElem].sold = temp->cb.sold;*/
				vector[*nrElem] = temp->cb;
				(*nrElem)++;
			}
			temp = temp->next;
		}
	}
}

void salvare_conturi_persoan_in_ls(NodLS* capListaInitiala, NodLS2** capListaSecundara, char* titular) {
	if (capListaInitiala == NULL) {
		return;
	}
	else
	{
		NodLS* temp = capListaInitiala;
		while (temp != NULL) 
		{
			if (strcmp(temp->cb.titular, titular) == 0) {
				NodLS2* nou = (NodLS2*)malloc(sizeof(NodLS2));
				nou->iban = (char*)malloc(strlen(temp->cb.iban) + 1);
				strcpy(nou->iban, temp->cb.iban);
				nou->next = NULL;

				if (*capListaSecundara == NULL) {
					*capListaSecundara = nou;
				}
				else
				{
					NodLS2* temp2 = *capListaSecundara;
					while (temp2->next != NULL)
					{
						temp2 = temp2->next;
					}
					temp2->next = nou;
				}
			}
			temp = temp->next;
		}
	}
}

NodLS* stergere_cont(NodLS* prim, char* titular) {
	if (prim == NULL) { return prim; }
	else
	{
		if (strcmp(prim->cb.titular, titular) == 0) {
			NodLS* toBeDeleted = prim;
			prim = prim->next;
			free(toBeDeleted->cb.titular);
			free(toBeDeleted);
		}

		NodLS* temp = prim;
		//ma opresc cand ajung pe ultimul nod
		while (temp->next != NULL)
		{
			if (strcmp(temp->cb.titular, titular) == 0) {
				NodLS* prev = prim;
				while (prev->next != temp)
				{
					prev = prev->next;
				}
				prev->next = temp->next;
				free(temp->cb.titular);
				free(temp);
				temp = prev->next;
			}
			temp = temp->next;
		}
		//am iesit din while si vreau sa steg ultimul nod
		if (strcmp(temp->cb.titular, titular) == 0) {
			NodLS* prev = prim;
			while (prev->next != temp)
			{
				prev = prev->next;
			}
			prev->next = temp->next;
			free(temp->cb.titular);
			free(temp);
		}
	}
	return prim;
}

void dezalocareLS(NodLS* prim) {
	NodLS* temp = prim;
	while (temp != NULL)
	{
		NodLS2* aux = temp->next;
		free(temp->cb.titular);
		free(temp);
		temp = aux;
	}
}

void dezalocareLS2(NodLS2* prim) {
	NodLS2* temp = prim;
	while (temp != NULL)
	{
		NodLS2* aux = temp->next;
		free(temp->iban);
		free(temp);
		temp = aux;
	}
}

int main() {
	NodLS* prim = NULL;
	FILE* f = fopen("Conturi.txt", "r");
	if (f == NULL) {
		printf("Fisierul nu a fost deschis!");
		return -1;
	}

	char buffer[250];
	ContBancar contB;
	while (fscanf(f, "%s", contB.iban) == 1)
	{
		fscanf(f, " %[^\n]", buffer);
		contB.titular = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
		strcpy(contB.titular, buffer);

		fscanf(f, "%s", contB.moneda);
		fscanf(f, "%f", &contB.sold);

		prim = inserare_sfarsit(prim, contB);

		free(contB.titular);
	}

	printf("Lista simpla citita din fisier\n");
	printare_lista_simplar(prim);

	ContBancar contBancarInseratLaInceput;
	strcpy(contBancarInseratLaInceput.iban, "RO04BRDE1234567890556333");

	contBancarInseratLaInceput.titular = (char*)malloc(strlen("Serban Timofte") + 1);
	strcpy(contBancarInseratLaInceput.titular, "Serban Timofte");

	strcpy(contBancarInseratLaInceput.moneda, "EUR");
	contBancarInseratLaInceput.sold = 10000;

	inserare_inceput(&prim, contBancarInseratLaInceput);
	
	// inserare la inceput
	printf("\nLista simpla dupa inserarea la inceput\n");
	printare_lista_simplar(prim);

	//salvez conturile in EUR intr-un vector
	int nrElem = 0;
	ContBancar* conturiEURO = (ContBancar*)malloc(5 * sizeof(ContBancar));
	salvare_conturi_euro_in_vector(conturiEURO, prim, &nrElem);
	printf("\nVectorul cu conturile bancare in EURO este:\n");
	for (int i = 0; i < nrElem; i++)
	{
		printf("IBAN: %s \t Titular: %s \t MONEDA: %s \t Sold: %f\n", conturiEURO[i].iban, conturiEURO[i].titular, conturiEURO[i].moneda, conturiEURO[i].sold);
	}

	//salvam conturile unei persoane intr-o alta lista simpla
	NodLS2* primListaPopescuIulian = NULL;
	salvare_conturi_persoan_in_ls(prim, &primListaPopescuIulian, "Popescu Iulian");
	printf("Lista lui Iulian Popescu: \n");
	NodLS2* temp = primListaPopescuIulian;
	while (temp != NULL)
	{
		printf("\nIBAN: %s", temp->iban);
		temp = temp->next;
	}

	prim = stergere_cont(prim, "Ionescu Georgica");
	printf("\nLista simpla dupa prima stergere:\n");
	printare_lista_simplar(prim);

	prim = stergere_cont(prim, "Popescu Iulian");
	printf("\nLista simpla dupa a doua stergere:\n");
	printare_lista_simplar(prim);

	prim = stergere_cont(prim, "Serban Timofte");
	printf("\nLista simpla dupa a treia stergere:\n");
	printare_lista_simplar(prim);

	dezalocareLS(prim);
	dezalocareLS2(primListaPopescuIulian);

	free(conturiEURO);

	return 0;
}