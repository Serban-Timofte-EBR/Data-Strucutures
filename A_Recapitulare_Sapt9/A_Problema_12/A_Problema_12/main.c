#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

typedef struct {
	char* denumire;
	float suprafata;
	unsigned char nr_angajati;
	char* adresa;
} Farmacie;

typedef struct {
	Farmacie farm;
	struct NodLD* next;
	struct NodLD* prev;
} NodLD;

NodLD* inserare_ld(NodLD* prim, NodLD** ultim, Farmacie farm) {
	NodLD* nou = (NodLD*)malloc(sizeof(NodLD));
	nou->farm.nr_angajati = farm.nr_angajati;
	nou->farm.suprafata = farm.suprafata;
	nou->farm.adresa = (char*)malloc(strlen(farm.adresa) + 1);
	strcpy(nou->farm.adresa, farm.adresa);
	nou->farm.denumire = (char*)malloc(strlen(farm.denumire) + 1);
	strcpy(nou->farm.denumire, farm.denumire);
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

void traversare_cap(NodLD* prim) {
	NodLD* temp = prim;
	while (temp != NULL)
	{
		printf("Farmacia: %s \t Angajati: %d \t Suprafata: %.2f \t Adresa: %s\n",
			temp->farm.denumire, temp->farm.nr_angajati, temp->farm.suprafata, temp->farm.adresa);
		temp = temp->next;
	}
}

void traversare_coada(NodLD* ultim) {
	NodLD* temp = ultim;
	while (temp != NULL)
	{
		printf("Farmacia: %s \t Angajati: %d \t Suprafata: %.2f \t Adresa: %s\n",
			temp->farm.denumire, temp->farm.nr_angajati, temp->farm.suprafata, temp->farm.adresa);
		temp = temp->prev;
	}
}

int nr_farmacii_suprafata(NodLD* prim, float prag_suprafata) {
	NodLD* temp = prim;
	int counter = 0;
	while (temp != NULL)
	{
		if (temp->farm.suprafata < prag_suprafata) {
			counter++;
		}
		temp = temp->next;
	}
	return counter;
}

NodLD* stergere_farmacii_5_ang(NodLD* prim, NodLD** ultim, int prag) {
	NodLD* temp = prim;
	while (temp != NULL)
	{
		if(temp->farm.nr_angajati > 5) {
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
			free(deleteMe->farm.adresa);
			free(deleteMe->farm.denumire);
			free(deleteMe);
		}
		else {
			temp = temp->next;
		}
	}
	return prim;
}

Farmacie* salvare_vector(NodLD* prim, Farmacie* vector, int* nrElem, char* lant) {
	NodLD* temp = prim;
	while (temp!=NULL)
	{
		if (strcmp(temp->farm.denumire, lant) == 0) {
			vector[*nrElem].nr_angajati = temp->farm.nr_angajati;
			vector[*nrElem].suprafata = temp->farm.suprafata;
			vector[*nrElem].adresa = (char*)malloc(strlen(temp->farm.adresa) + 1);
			strcpy(vector[*nrElem].adresa, temp->farm.adresa);
			vector[*nrElem].denumire = (char*)malloc(strlen(temp->farm.denumire) + 1);
			strcpy(vector[*nrElem].denumire, temp->farm.denumire);
			(*nrElem)++;
		}
		temp = temp->next;
	}
	return vector;
}

int main() {
	NodLD* prim = NULL;
	NodLD* ultim = NULL;

	FILE* f = fopen("Farmacii.txt", "r");
	if (f == NULL) {
		printf("Nu s-a deschis fisierul");
		return -1;
	}

	char buffer[128];
	Farmacie farm;

	while (fscanf(f, " %[^\n]", buffer) == 1)
	{
		farm.denumire = (char*)malloc(strlen(buffer) + 1);
		strcpy(farm.denumire, buffer);

		fscanf(f, "%f", &farm.suprafata);
		fscanf(f, "%d", &farm.nr_angajati);
		
		fscanf(f, " %[^\n]", buffer);
		farm.adresa = (char*)malloc(strlen(buffer) + 1);
		strcpy(farm.adresa, buffer);

		prim = inserare_ld(prim, &ultim, farm);

		free(farm.denumire);
		free(farm.adresa);
	}
	fclose(f);

	printf("Lista din fisier\n");
	traversare_cap(prim);

	printf("\nLista din fisier citita invers\n");
	traversare_coada(ultim);

	int nr_farmacii_sub_20 = nr_farmacii_suprafata(prim, 20.0);
	printf("\nSunt %d farmacii cu suprafata sub 20 de m2\n", nr_farmacii_sub_20);

	prim = stergere_farmacii_5_ang(prim, &ultim, 5);
	printf("\nLista dupa stergeri\n");
	traversare_cap(prim);

	printf("\nLista dupa stergeri invers\n");
	traversare_coada(ultim);

	Farmacie* vector = (Farmacie*)malloc(10 * sizeof(Farmacie));
	int nrFarmacii = 0;
	vector = salvare_vector(prim, vector, &nrFarmacii, "Help Net");
	printf("\nFarmaciile din vector\n");
	for (int i = 0; i < nrFarmacii; i++)
	{
		printf("Farmacia: %s \t Angajati: %d \t Suprafata: %.2f \t Adresa: %s\n",
			vector[i].denumire, vector[i].nr_angajati, vector[i].suprafata, vector[i].adresa);
	}

	return 0;
}