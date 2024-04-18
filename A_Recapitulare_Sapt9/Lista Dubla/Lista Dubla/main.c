#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

typedef struct {
	char iban[25];
	char* titluar;
	char moneda[4];
	float sold;
} ContBancar;

typedef struct {
	ContBancar* cb;
	struct NodLD* next;
	struct NodLD* prev;
} NodLD;

NodLD* inserare_sfarsit(NodLD* prim, NodLD** ultim, ContBancar cont) {
	NodLD* nou = (NodLD*)malloc(sizeof(ContBancar));

	nou->cb = (ContBancar*)malloc(sizeof(ContBancar));

	strcpy(nou->cb->iban, cont.iban);

	nou->cb->titluar = (char*)malloc(sizeof(char) * (strlen(cont.titluar) + 1));
	strcpy(nou->cb->titluar, cont.titluar);

	strcpy(nou->cb->moneda, cont.moneda);
	
	nou->cb->sold = cont.sold;

	if (prim == NULL) {
		nou->next = NULL;
		nou->prev = NULL;
		prim = nou;
		*ultim = nou;
	}
	else {
		NodLD* temp = prim;
		while (temp->next != NULL)
		{
			temp = temp->next;
		}
		temp->next = nou;
		nou->prev = temp;
		nou->next = NULL;
		*ultim = nou;
	}

	return prim;
}

NodLD* insereaza_inceput(NodLD* prim, NodLD** ultim, ContBancar cont) {
	NodLD* nou = (NodLD*)malloc(sizeof(NodLD));
	nou->cb = (ContBancar*)malloc(sizeof(ContBancar));
	strcpy(nou->cb->iban, cont.iban);
	nou->cb->titluar = (char*)malloc(sizeof(char) * (strlen(cont.titluar) + 1));
	strcpy(nou->cb->titluar, cont.titluar);
	strcpy(nou->cb->moneda, cont.moneda);
	nou->cb->sold = cont.sold;

	if (prim == NULL) {
		nou->next = NULL;
		nou->prev = NULL;
		prim = nou;
		*ultim = nou;
	}
	else
	{
		nou->next = prim;
		prim->prev = nou;
		nou->prev = NULL;
		prim = nou;
	}
	return prim;
}

void printeaza_lista(NodLD* prim) {
	NodLD* temp = prim;
	while (temp != NULL)
	{
		printf("IBAN: %s \t Titular: %s \t Moneda: %s \t Sold: %f \n", temp->cb->iban, temp->cb->titluar, temp->cb->moneda, temp->cb->sold);
		temp = temp->next;
	}
}

void printeaza_lista_invers(NodLD* ultim) {
	NodLD* temp = ultim;
	while (temp != NULL)
	{
		printf("IBAN: %s \t Titular: %s \t Moneda: %s \t Sold: %f \n", temp->cb->iban, temp->cb->titluar, temp->cb->moneda, temp->cb->sold);
		temp = temp->prev;
	}
}

int main() {
	NodLD* prim = NULL;
	NodLD* ultim = NULL;

	FILE* f = fopen("Conturi.txt", "r");
	if (f == NULL) {
		printf("Fisierul nu s-a deschis");
		return -1;
	}

	char buffer[128];
	ContBancar contB;
	while (fscanf(f, "%s", contB.iban) == 1)
	{
		fscanf(f, " %[^\n]", buffer);
		contB.titluar = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
		strcpy(contB.titluar, buffer);

		fscanf(f, "%s", contB.moneda);
		fscanf(f, "%f", &contB.sold);

		prim = inserare_sfarsit(prim, &ultim, contB);

		free(contB.titluar);
	}

	printf("Lista dubla citita din fiser\n");
	printeaza_lista(prim);

	printf("\nLista dubla citita invers\n");
	printeaza_lista_invers(ultim);

	ContBancar contInseratUlterior;
	strcpy(contInseratUlterior.iban, "RO04INGB1234567890556222");
	contInseratUlterior.titluar = (char*)malloc(strlen("Serban Timofte") + 1);
	strcpy(contInseratUlterior.titluar, "Serban Timofte");
	strcpy(contInseratUlterior.moneda, "EUR");
	contInseratUlterior.sold = 100000;

	prim = insereaza_inceput(prim, &ultim, contInseratUlterior);
	printf("\nLista dubla citita dupa inserare\n");
	printeaza_lista(prim);

	printf("\nLista dubla citita invers dupa inserare\n");
	printeaza_lista_invers(ultim);
	return 0;
}