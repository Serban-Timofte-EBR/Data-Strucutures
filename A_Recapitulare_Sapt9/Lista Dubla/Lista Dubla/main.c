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

typedef struct {
	char* iban;
	char* titular;
	struct NodLS* next;
} NodLS;

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

NodLS* inserare_lista_simpla_secundara(NodLS* prim, char* titular, char* iban) {
	NodLS* nou = (NodLS*)malloc(sizeof(NodLS));
	nou->iban = (char*)malloc(strlen(iban) + 1);
	strcpy(nou->iban, iban);
	nou->titular = (char*)malloc(strlen(titular) + 1);
	strcpy(nou->titular, titular);
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

NodLS* extrage_nod(NodLD* prim, NodLD** ultim, NodLS** prim_ls, char* titular) {
	if (prim == NULL) {
		return prim;
	}
	else
	{
		// extrag primul nod
		if (strcmp(prim->cb->titluar, titular) == 0) {
			NodLD* temp = prim;
			prim = prim->next;
			prim->prev = NULL;
			*prim_ls = inserare_lista_simpla_secundara(*prim_ls, titular, temp->cb->iban);
		}
		else
		{
			//extrag un nod din mijloc
			NodLD* temp = prim;
			while (temp->next != NULL)
			{
				if (strcmp(temp->cb->titluar, titular) == 0) {
					NodLD* next = temp->next;
					NodLD* prev = temp->prev;
					next->prev = prev;
					prev->next = next;
					*prim_ls = inserare_lista_simpla_secundara(*prim_ls, titular, temp->cb->iban);
				}
				temp = temp->next;
			}
			if (strcmp(temp->cb->titluar, titular) == 0) {
				NodLD* prev = temp->prev;
				prev->next = NULL;
				*ultim = prev;
				*prim_ls = inserare_lista_simpla_secundara(*prim_ls, titular, temp->cb->iban);
			}
		}
	}
	return prim;
}

// doar salvam conturile intr-un alt vector
ContBancar* vector_conturi_sold(NodLD* prim, NodLD* ultim, ContBancar* vector, int* nrElem, float soldPrag) {
	if (prim == NULL) {
		return vector;
	}
	else
	{
		NodLD* temp = prim;
		while (temp != NULL)
		{
			if (temp->cb->sold > soldPrag) {
				vector[*nrElem] = *(temp->cb);
				(*nrElem)++;
			}
			temp = temp->next;
		}
	}
	return vector;
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

void dezalocare_LD(NodLD* prim) {
	if (prim != NULL) {
		NodLD* temp = prim;
		while (temp != NULL)
		{
			NodLD* aux = temp->next;
			free(temp->cb->titluar);
			free(temp);
			temp = aux;
		}
	}
}

void dezalocare_LS(NodLS* prim) {
	if (prim != NULL) {
		NodLS* temp = prim;
		while (temp != NULL)
		{
			NodLS* aux = temp->next;
			free(temp->iban);
			free(temp->titular);
			free(temp);
			temp = aux;
		}
	}
}

int main() {
	NodLD* prim = NULL;
	NodLD* ultim = NULL;
	NodLS* prim_listaSecundara = NULL;

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

	int nrElem = 0;
	ContBancar* conturiPeste100 = (ContBancar*)malloc(5 * sizeof(ContBancar));
	conturiPeste100 = vector_conturi_sold(prim, ultim, conturiPeste100, &nrElem, 100.0);
	printf("\nVectorul cu solduri mai mari de 100\n");
	for (unsigned char i = 0; i < nrElem; i++) {
		printf("IBAN: %s \t Titular: %s \t Moneda: %s \t Sold: %f \n",
			conturiPeste100[i].iban, conturiPeste100[i].titluar, conturiPeste100[i].moneda, conturiPeste100[i].sold);
	}
	free(conturiPeste100);

	prim = extrage_nod(prim, &ultim, &prim_listaSecundara, "Popescu Iulian");
	printf("\nLista dubla cap -> coada dupa extragere\n");
	printeaza_lista(prim);

	printf("\nLista dubla coada -> cap dupa extragere\n");
	printeaza_lista_invers(ultim);

	printf("\nLista simpla dupa extrageri\n");
	NodLS* temp = prim_listaSecundara;
	while (temp != NULL)
	{
		printf("IBAN: %s \t IBAN: %s \n", temp->iban, temp->titular);
		temp = temp->next;
	}
	free(temp);

	dezalocare_LD(prim);
	dezalocare_LS(prim_listaSecundara);

	return 0;
}