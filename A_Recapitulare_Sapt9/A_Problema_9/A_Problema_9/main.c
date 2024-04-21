#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

typedef struct {
	int id;
	char* denumire;
	float pret;
} Produs;

typedef struct {
	Produs prod;
	struct NodLD* next;
	struct NodLD* prev;
} NodLD;

typedef struct {
	char* denumire;
} DateProdus;

NodLD* inserare_ld_sfarsit(NodLD* prim, NodLD** ultim, Produs prod) {
	NodLD* nou = (NodLD*)malloc(sizeof(NodLD));
	nou->prod.id = prod.id;
	nou->prod.pret = prod.pret;
	nou->prod.denumire = (char*)malloc(strlen(prod.denumire) + 1);
	strcpy(nou->prod.denumire, prod.denumire);
	nou->next = NULL;
	nou->prev = NULL;

	if (prim == NULL) {
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
		*ultim = nou;
	}

	return prim;
}

NodLD* inserare_sortata(NodLD* prim, NodLD** ultim, Produs prod) {
	NodLD* nou = (NodLD*)malloc(sizeof(NodLD));
	nou->prod.id = prod.id;
	nou->prod.pret = prod.pret;
	nou->prod.denumire = (char*)malloc(strlen(prod.denumire) + 1);
	strcpy(nou->prod.denumire, prod.denumire);
	nou->next = NULL;
	nou->prev = NULL;

	if (prim == NULL) {
		prim = nou;
		*ultim = nou;
	}
	else {
		NodLD* temp = prim;
		while (temp != NULL && temp->prod.pret < prod.pret)
		{
			temp = temp->next;
		}
		if (temp == prim) {
			//inserez inainte de primul nod
			nou->next = prim;
			prim->prev = nou;
			prim = nou;
		}
		else if (temp == NULL) {
			//inserez la sfarsit
			(*ultim)->next = nou;
			nou->prev = *ultim;
			*ultim = nou;
		}
		else {
			//inserez inainte de temp
			NodLD* ant = temp->prev;
			nou->prev = ant;
			nou->next = temp;
			ant->next = nou;
			temp->prev = nou;
		}
	}
	return prim;
}

NodLD* inserare_index(NodLD* prim, NodLD** ultim, Produs prod, int index) {
	NodLD* nou = (NodLD*)malloc(sizeof(NodLD));
	nou->prod.id = prod.id;
	nou->prod.pret = prod.pret;
	nou->prod.denumire = (char*)malloc(strlen(prod.denumire) + 1);
	strcpy(nou->prod.denumire, prod.denumire);
	nou->next = NULL;
	nou->prev = NULL;

	NodLD* temp = prim;
	int i = 0;
	while (temp != NULL && i < index)
	{
		temp = temp->next;
		i++;
	}
	if (temp == prim) {
		nou->next = prim;
		prim->prev = nou;
		prim = nou;
	}
	else if (temp == NULL) {
		(*ultim)->next = nou;
		nou->prev = (*ultim);
		*ultim = nou;
	}
	else {
		NodLD* ant = temp->prev;
		nou->next = temp;
		nou->prev = ant;
		ant->next = nou;
		temp->prev = nou;
	}
	return prim;
}

DateProdus* insereaza_produs(DateProdus* vector, int* nrElem, Produs prod) {
	vector[*nrElem].denumire = (char*)malloc(strlen(prod.denumire) + 1);
	strcpy(vector[*nrElem].denumire, prod.denumire);
	(*nrElem)++;
	return vector;
}

DateProdus* salvare_vector_produse_scumpe(NodLD** prim, NodLD** ultim, DateProdus* vector, int* nrElem) {
	NodLD* temp = *prim;
	while (temp != NULL)
	{
		if (temp->prod.pret >= 1000.0) {
			vector = insereaza_produs(vector, nrElem, temp->prod);
			NodLD* deleteMe = temp;
			if (temp == *prim) {
				//sterg primul nod din lista dubla
				*prim = (*prim)->next;
				(*prim)->prev = NULL;
			}
			else if (temp == *ultim) {
				//sterg ultimul nod din lista dubla
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
			free(deleteMe->prod.denumire);
			free(deleteMe);
		}
		else {
			temp = temp->next;
		}
	}
	return vector;
}

void traversare_ls(NodLD* prim) {
	NodLD* temp = prim;
	while (temp != NULL)
	{
		printf("ID: %d \t Pret: %f \t Denumire: %s\n", temp->prod.id, temp->prod.pret, temp->prod.denumire);
		temp = temp->next;
	}
}

void traversare_ls_invers(NodLD* ultim) {
	NodLD* temp = ultim;
	while (temp != NULL)
	{
		printf("ID: %d \t Pret: %f \t Denumire: %s\n", temp->prod.id, temp->prod.pret, temp->prod.denumire);
		temp = temp->prev;
	}
}

void dezalocare_vector(DateProdus* vector, int nrElem) {
	for (unsigned char i = 0; i < nrElem; i++)
	{
		free(vector[i].denumire);
	}
	free(vector);
}

void dezalocare_ld(NodLD* prim) {
	NodLD* temp = prim;
	while (temp != NULL)
	{
		NodLD* aux = temp->next;
		free(temp->prod.denumire);
		free(temp);
		temp = aux;
	}
}

int main() {
	NodLD* prim = NULL;
	NodLD* ultim = NULL;
	NodLD* primSortat = NULL;
	NodLD* ultimSortat = NULL;

	FILE* f = fopen("Produse.txt", "r");
	if (f == NULL) {
		printf("Fisierul nu s-a deschis");
		return -1;
	}

	char buffer[128];
	Produs prod;

	while (fscanf(f, "%d", &prod.id) == 1) 
	{
		fscanf(f, " %[^\n]", buffer);
		prod.denumire = (char*)malloc(strlen(buffer) + 1);
		strcpy(prod.denumire, buffer);
		fscanf(f, "%f", &prod.pret);

		prim = inserare_ld_sfarsit(prim, &ultim, prod);
		primSortat = inserare_sortata(primSortat, &ultimSortat, prod);

		free(prod.denumire);
	}
	fclose(f);

	printf("Lista cap -> coada din fisier\n");
	traversare_ls(prim);

	printf("\nLista coada -> cap din fisier\n");
	traversare_ls_invers(ultim);

	printf("\nLista cap -> coada din fisier sortata\n");
	traversare_ls(primSortat);

	printf("\nLista coada -> cap din fisier sortata\n");
	traversare_ls_invers(ultimSortat);

	Produs prodDeInserat;
	prodDeInserat.id = 999;
	prodDeInserat.denumire = (char*)malloc(strlen("Produs Inserat") + 1);
	strcpy(prodDeInserat.denumire, "Produs Inserat");
	prodDeInserat.pret = 1999.9;

	prim = inserare_index(prim, &ultim, prodDeInserat, 0);
	printf("\nLista cap -> coada dupa inserare\n");
	traversare_ls(prim);

	printf("\nLista coada -> cap dupa inserare\n");
	traversare_ls_invers(ultim);

	//sterg din lista produsele mai scumpe de 1000 RON si le salvez in vector
	DateProdus* produseScumpe = (DateProdus*)malloc(5 * sizeof(produseScumpe));
	int nrProduse = 0;
	produseScumpe = salvare_vector_produse_scumpe(&primSortat, &ultimSortat, produseScumpe, &nrProduse);
	printf("\nElementele din vector\n");
	for (unsigned char i = 0; i < nrProduse; i++) {
		printf("Denumire: %s\n", produseScumpe[i].denumire);
	}

	printf("\nLista cap -> coada dupa extrageri\n");
	traversare_ls(primSortat);

	printf("\nLista coada -> cap dupa extrageri\n");
	traversare_ls_invers(ultimSortat);

	dezalocare_vector(produseScumpe, nrProduse);
	dezalocare_ld(prim);
	dezalocare_ld(primSortat);

	return 0;
}