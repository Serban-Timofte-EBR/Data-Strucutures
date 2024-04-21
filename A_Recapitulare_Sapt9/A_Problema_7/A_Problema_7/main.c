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
	struct NodLD* next;
	struct NodLD* prev;
} NodLD;

typedef struct {
	char iban[25];
} IBAN;

typedef struct {
	char iban[25];
	struct NodLS* next;
} NodLS;

NodLD* inserare_titulari_sold(NodLD* prim, NodLD** ultim, ContBancar cont) {
	NodLD* nou = (NodLD*)malloc(sizeof(NodLD));
	strcpy(nou->cont.iban, cont.iban);
	nou->cont.titular = (char*)malloc(strlen(cont.titular) + 1);
	strcpy(nou->cont.titular, cont.titular);
	strcpy(nou->cont.moneda, cont.moneda);
	nou->cont.sold = cont.sold;
	nou->next = NULL;
	nou->prev = NULL;

	if (prim == NULL) {
		prim = nou;
		*ultim = nou;
	}
	else {
		NodLD* temp = prim;
		while (temp != NULL && temp->cont.sold <= cont.sold) {
			temp = temp->next;
		}
		if (temp == prim) {
			nou->next = prim;
			prim->prev = nou;
			prim = nou;
		}
		else if (temp == NULL) {
			(*ultim)->next = nou;
			nou->prev = *ultim;
			*ultim = nou;
		}
		else {
			NodLD* ant = temp->prev;
			nou->prev = ant;
			nou->next = temp;
			ant->next = nou;
			temp->prev = nou;
		}
	}
	return prim;
}

NodLD* stergere_cont(NodLD* prim, NodLD** ultim, char* titular) {
	NodLD* temp = prim;
	while (temp != NULL)
	{
		if (strcmp(temp->cont.titular, titular) == 0) {
			NodLD* deleteMe = temp;
			if (temp == prim) {
				prim = prim->next;
				prim->prev = NULL;
			}
			else if (temp == *ultim) {
				(*ultim) = (*ultim)->prev;
				(*ultim)->next = NULL;
			}
			else {
				NodLD* ant = temp->prev;
				NodLD* urm = temp->next;
				ant->next = urm;
				urm->prev = ant;
			}
			temp = temp->next;
			free(deleteMe->cont.titular);
			free(deleteMe);
		}
		else {
			temp = temp->next;
		}
	}
	return prim;
}

IBAN* preluare_vector(NodLD* prim, NodLD* ultim, IBAN* vector, int* nrElem) {
	NodLD* temp = prim;
	while (temp != NULL) {
		if (temp->cont.sold >= 500) {
			strcpy(vector[*nrElem].iban, temp->cont.iban);
			(*nrElem)++;
		}
		temp = temp->next;
	}
	return vector;
}

NodLS* inserare_ls(NodLS* primLS, char* iban) {
	NodLS* nou = (NodLS*)malloc(sizeof(NodLS));
	strcpy(nou->iban, iban);
	nou->next = NULL;

	if (primLS == NULL) {
		primLS = nou;
	}
	else {
		NodLS* temp = primLS;
		while (temp->next != NULL)
		{
			temp = temp->next;
		}
		temp->next = nou;
	}
	return primLS;
}

NodLS* preluare_nod(NodLD** prim, NodLD** ultim, NodLS* primLS) {
	NodLD* temp = *prim;
	while (temp != NULL)
	{
		if (temp->cont.sold >= 500) {
			primLS = inserare_ls(primLS, temp->cont.iban);
			NodLD* deleteMe = temp;
			if (temp == *prim) {
				*prim = (*prim)->next;
				(*prim)->prev = NULL;
			}
			else if (temp == *ultim) {
				(*ultim) = (*ultim)->prev;
				(*ultim)->next = NULL;
			}
			else {
				NodLD* ant = temp->prev;
				NodLD* urm = temp->next;
				ant->next = temp->next;
				urm->prev = ant;
			}
			temp = temp->next;
		}
		else {
			temp = temp->next;
		}
	}
	return primLS;
}

void traversare_ld(NodLD* prim) {
	NodLD* temp = prim;
	while (temp != NULL)
	{
		printf("IBAN: %s \t Numele: %s \t Moneda: %s \t Sold: %f\n", temp->cont.iban, temp->cont.titular, temp->cont.moneda, temp->cont.sold);
		temp = temp->next;
	}
}

void traversare_ld_inversa(NodLD* ultim) {
	NodLD* temp = ultim;
	while (temp != NULL)
	{
		printf("IBAN: %s \t Numele: %s \t Moneda: %s \t Sold: %f\n", temp->cont.iban, temp->cont.titular, temp->cont.moneda, temp->cont.sold);
		temp = temp->prev;
	}
}

void traversare_ls(NodLS* prim) {
	NodLS* temp = prim;
	while (temp!=NULL)
	{
		printf("IBAN: %s\n", temp->iban);
		temp = temp->next;
	}
}

void dezalocare_vector(IBAN* vector) {
	free(vector);
}

void dezalocare_ls(NodLS* prim) {
	NodLS* temp = prim;
	while (temp != NULL) {
		NodLS* aux = temp->next;
		free(temp);
		temp = aux;
	}
}

void dezalocare_ld(NodLD* prim) {
	NodLD* temp = prim;
	while (temp!=NULL)
	{
		NodLD* aux = temp->next;
		free(temp->cont.titular);
		free(temp);
		temp = aux;
	}
}

int main() {
	NodLD* prim = NULL;
	NodLD* ultim = NULL;

	FILE* f = fopen("Conturi.txt", "r");
	if (f == NULL) {
		printf("Fisierul nu e deschis");
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

		prim = inserare_titulari_sold(prim, &ultim, cont);

		free(cont.titular);
	}

	printf("Lista citita din fisier\n");
	traversare_ld(prim);

	printf("\nLista citita din fisier invers\n");
	traversare_ld_inversa(ultim);

	prim = stergere_cont(prim, &ultim, "Parvulescu Cosmina");

	printf("\nLista dupa stergere cap -> coada\n");
	traversare_ld(prim);

	printf("\nLista dupa stergere coada -> cap\n");
	traversare_ld_inversa(ultim);

	IBAN* vector_solduri_500 = (IBAN*)malloc(5 * sizeof(IBAN));
	int nrSolduriVect = 0;
	vector_solduri_500 = preluare_vector(prim, ultim, vector_solduri_500, &nrSolduriVect);

	printf("\nVectirul cu IBANURI\n");
	for (int i = 0; i < nrSolduriVect; i++) {
		printf("IBAN: %s\n", vector_solduri_500[i].iban);
	}

	NodLS* primLS = NULL;
	primLS = preluare_nod(&prim, &ultim, primLS);
	printf("\nLista simpla\n");
	traversare_ls(primLS);

	printf("\nLista dupa preluare cap -> coada\n");
	traversare_ld(prim);

	printf("\nLista dupa preluare coada -> cap\n");
	traversare_ld_inversa(ultim);

	dezalocare_vector(vector_solduri_500);
	dezalocare_ls(primLS);
	dezalocare_ld(prim);

	return 0;
}