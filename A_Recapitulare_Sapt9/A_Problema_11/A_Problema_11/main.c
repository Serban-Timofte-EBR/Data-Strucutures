#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

typedef struct {
	int cod;
	char* nume;
	int varsta;
	float medie;
} Student;

typedef struct {
	Student stud;
	struct NodLS* next;
} NodLS;

typedef struct {
	NodLS** vector;
	int nrElem;
} hashT;

typedef struct {
	int cod;
	char* nume;
} StudentiPromovati;

int functie_hash(char* nume, hashT tabela) {
	return nume[0] % tabela.nrElem;
}

void inserare_ls(NodLS** prim, Student stud) {
	NodLS* nou = (char*)malloc(sizeof(Student));
	nou->stud.cod = stud.cod;
	nou->stud.medie = stud.medie;
	nou->stud.varsta = stud.varsta;
	nou->stud.nume = (char*)malloc(strlen(stud.nume) + 1);
	strcpy(nou->stud.nume, stud.nume);
	nou->next = NULL;

	if (*prim == NULL) {
		*prim = nou;
	}
	else {
		NodLS* temp = *prim;
		while (temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = nou;
	}
}

void inserare_hash(hashT tabela, Student stud) {
	int poz = functie_hash(stud.nume, tabela);
	inserare_ls(&tabela.vector[poz], stud);
}

void print_ls(NodLS* prim) {
	NodLS* temp = prim;
	while (temp != NULL)
	{
		printf("\tCod: %d \t Varsta: %d \t Medie: %f \t Nume: %s\n", temp->stud.cod, temp->stud.varsta, temp->stud.medie, temp->stud.nume);
		temp = temp->next;
	}
}

void print_hash(hashT tabela) {
	for (unsigned char i = 0; i < tabela.nrElem; i++)
	{
		if (tabela.vector[i] != NULL) {
			printf("Pozitie = %d\n", i);
			print_ls(tabela.vector[i]);
		}
	}
}

float medie_max(hashT tabela) {
	float max = 0;
	for (int i = 0; i < tabela.nrElem; i++)
	{
		if (tabela.vector[i] != NULL) {
			NodLS* temp = tabela.vector[i];
			while (temp != NULL)
			{
				if (temp->stud.medie > max) {
					max = temp->stud.medie;
				}
				temp = temp->next;
			}
		}
	}
	return max;
}

void sterge_medie_max(hashT tabela) {
	float medie = medie_max(tabela);
	for (int i = 0; i < tabela.nrElem; i++) {
		if (tabela.vector[i] != NULL) {
			NodLS* temp = tabela.vector[i];
			NodLS* prev = NULL;
			while (temp != NULL)
			{
				if (temp->stud.medie == medie) {
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
					free(deleteMe->stud.nume);
				}
				else {
					prev = temp;
					temp = temp->next;
				}
			}
		}
	}
}

StudentiPromovati* salvare_promovati(hashT tabela, StudentiPromovati* vector, int* nrElem) {
	for (int i = 0; i < tabela.nrElem; i++)
	{
		if (tabela.vector[i] != NULL) {
			NodLS* temp = tabela.vector[i];
			while (temp != NULL)
			{
				if (temp->stud.medie >= 5) {
					vector[*nrElem].cod = temp->stud.cod;
					vector[*nrElem].nume = (char*)malloc(strlen(temp->stud.nume) + 1);
					strcpy(vector[*nrElem].nume, temp->stud.nume);
					(*nrElem)++;
				}
				temp = temp->next;
			}
		}
	}
	return vector;
}

void dezalocare_ls(NodLS* prim, FILE* f) {
	NodLS* temp = prim;
	while (temp != NULL)
	{
		fprintf(f, "\tCod: %d \t Varsta: %d \t Medie: %f \t Nume: %s\n", temp->stud.cod, temp->stud.varsta, temp->stud.medie, temp->stud.nume);
		NodLS* aux = temp->next;
		free(temp->stud.nume);
		temp = aux;
	}
}

void dezalocare_hash(hashT tabela, FILE* f) {
	for (int i = 0; i < tabela.nrElem; i++) {
		if (tabela.vector[i] != NULL) {
			fprintf(f, "Pozitie = %d\n", i);
			dezalocare_ls(tabela.vector[i], f);
		}
	}
}

int main() {
	hashT tabela;
	tabela.nrElem = 13;
	tabela.vector = (NodLS**)malloc(tabela.nrElem * sizeof(NodLS*));
	for (unsigned char i = 0; i < tabela.nrElem; i++)
	{
		tabela.vector[i] = NULL;
	}

	FILE* f = fopen("Studenti.txt", "r");
	if (f == NULL) {
		printf("Fisierul nu s-a deschis");
		return -1;
	}

	char buffer[128];
	Student stud;

	while (fscanf(f, "%d", &stud.cod) == 1)
	{
		fscanf(f, " %[^\n]", buffer);
		stud.nume = (char*)malloc(strlen(buffer) + 1);
		strcpy(stud.nume, buffer);

		fscanf(f, "%d", &stud.varsta);
		fscanf(f, "%f", &stud.medie);

		inserare_hash(tabela, stud);

		free(stud.nume);
	}
	fclose(f);

	printf("Tabela din fisier\n");
	print_hash(tabela);

	//printf("Medie max: %f", medie_max(tabela));
	sterge_medie_max(tabela);
	printf("\nTabela dupa stergere\n");
	print_hash(tabela);

	StudentiPromovati* vector = (StudentiPromovati*)malloc(10 * sizeof(StudentiPromovati));
	int nrStudenti = 0;
	vector = salvare_promovati(tabela, vector, &nrStudenti);
	printf("\nVectorul cu studenti promovati\n");
	for (int i = 0; i < nrStudenti; i++)
	{
		printf("Cod: %d \t Nume: %s\n", vector[i].cod, vector[i].nume);
	}

	FILE* f_hash = fopen("Tabela.txt", "w");
	dezalocare_hash(tabela, f_hash);
	fclose(f_hash);

	FILE* f_vector = fopen("Vector.txt", "w");
	for (int i = 0; i < nrStudenti; i++)
	{
		fprintf(f, "Cod: %d \t Nume: %s\n", vector[i].cod, vector[i].nume);
		free(vector[i].nume);
	}
	free(vector);
	fclose(f_vector);
	
	return 0;
}