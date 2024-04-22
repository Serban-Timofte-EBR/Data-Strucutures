#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

typedef struct {
	int cod;
	char* adresa;
	float suma;
} Banca;

typedef struct {
	Banca banca;
	struct NodLD* next;
	struct NodLD* prev;
} NodLD;

typedef struct {
	int cod;
	char* adresa;
} VectorBanci;

NodLD* inserare_ld(NodLD* prim, NodLD** ultim, Banca banca) {
	NodLD* nou = (NodLD*)malloc(sizeof(NodLD));
	nou->banca.cod = banca.cod;
	nou->banca.suma = banca.suma;
	nou->banca.adresa = (char*)malloc(strlen(banca.adresa) + 1);
	strcpy(nou->banca.adresa, banca.adresa);
	nou->next = NULL;
	nou->prev = NULL;

	if (prim == NULL) {
		prim = nou;
		*ultim = nou;
	}
	else {
		NodLD* temp = prim;
		while (temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = nou;
		nou->prev = temp;
		*ultim = nou;
	}
	return prim;
}

void traversare_ld(NodLD* prim) {
	NodLD* temp = prim;
	while (temp != NULL) {
		printf("Cod: %d \t Suma: %f \t Adresa: %s\n", temp->banca.cod, temp->banca.suma, temp->banca.adresa);
		temp = temp->next;
	}
}

void traversare_ld_invers(NodLD* ultim) {
	NodLD* temp = ultim;
	while (temp != NULL) {
		printf("Cod: %d \t Suma: %f \t Adresa: %s\n", temp->banca.cod, temp->banca.suma, temp->banca.adresa);
		temp = temp->prev;
	}
}

NodLD* stergere_cod_impare(NodLD* prim, NodLD** ultim, FILE* f) {
	NodLD* temp = prim;
	while (temp != NULL)
	{
		if (temp->banca.cod % 2 != 0) {
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
			free(deleteMe->banca.adresa);
			free(deleteMe);
		}
		else
		{
			fprintf(f, "Cod: %d \t Suma: %f \t Adresa: %s\n", temp->banca.cod, temp->banca.suma, temp->banca.adresa);
			temp = temp->next;
		}
	}
	return prim;
}

VectorBanci* salvare_vector(NodLD** prim, NodLD** ultim, VectorBanci* vector, int* nrElem) {
	NodLD* temp = *prim;
	while (temp != NULL)
	{
		if(temp->banca.suma >= 1000.0) {
			vector[*nrElem].cod = temp->banca.cod;
			vector[*nrElem].adresa = (char*)malloc(strlen(temp->banca.adresa) + 1);
			strcpy(vector[*nrElem].adresa, temp->banca.adresa);
			(*nrElem)++;
		}
		temp = temp->next;
	}
	return vector;
}

void dezalocare_ld(NodLD* prim, FILE* f) {
	NodLD* temp = prim;
	while (temp != NULL)
	{
		fprintf(f, "Cod: %d \t Suma: %f \t Adresa: %s\n", temp->banca.cod, temp->banca.suma, temp->banca.adresa);
		NodLD* aux = temp->next;
		free(temp->banca.adresa);
		free(temp);
		temp = aux;
	}
}

void dezalocare_vector(VectorBanci* vector, int nrElem, FILE* f) {
	for (unsigned char i = 0; i < nrElem; i++)
	{
		fprintf(f, "Cod: %d \t Adresa: %s\n", vector[i].cod, vector[i].adresa);
		free(vector[i].adresa);
	}
	free(vector);
}

int main() {
	NodLD* prim = NULL;
	NodLD* ultim = NULL;

	FILE* f = fopen("Banci.txt", "r");
	if (f == NULL) {
		printf("Fisierul nu a fost deschis");
		return -1;
	} 

	char buffer[128];
	Banca bancaDoc;

	while (fscanf(f, "%d", &bancaDoc.cod) == 1) 
	{
		fscanf(f, " %[^\n]", buffer);
		bancaDoc.adresa = (char*)malloc(strlen(buffer) + 1);
		strcpy(bancaDoc.adresa, buffer);

		fscanf(f, "%f", &bancaDoc.suma);

		prim = inserare_ld(prim, &ultim, bancaDoc);
		
		free(bancaDoc.adresa);
	}
	fclose(f);

	printf("Lista din fisier\n");
	traversare_ld(prim);

	printf("\nLista invers din fisier\n");
	traversare_ld_invers(ultim);

	FILE* f_scriere = fopen("Output.txt", "w");
	prim = stergere_cod_impare(prim, &ultim, f_scriere);
	printf("\nLista din stergeri\n");
	traversare_ld(prim);
	fclose(f_scriere);

	printf("\nLista invers dupa stergeri\n");
	traversare_ld_invers(ultim);

	VectorBanci* banci_peste_100 = (VectorBanci*)malloc(10 * sizeof(VectorBanci));
	int nrBanci = 0;
	banci_peste_100 = salvare_vector(&prim, &ultim, banci_peste_100, &nrBanci);
	printf("\nLista bancilor din vector\n");
	for (unsigned char i = 0; i < nrBanci; i++)
	{
		printf("Cod: %d \t Adresa: %s\n", banci_peste_100[i].cod, banci_peste_100[i].adresa);
	}

	FILE* f_listaDubla = fopen("ListaDubla.txt", "w");
	FILE* f_vector = fopen("Vector.txt", "w");
	dezalocare_ld(prim, f_listaDubla);
	dezalocare_vector(banci_peste_100, nrBanci, f_vector);
	fclose(f_listaDubla);
	fclose(f_vector);

	return 0;
}