#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

typedef struct {
	int nrMatricol;
	char* nume;
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
	int nrMatricol;
	char* nume;
} DateStud;

int functie_hash(char* nume, hashT tabela) {
	return nume[0] % tabela.nrElem;
}

void inserare_ls(NodLS** prim, Student stud) {
	NodLS* nou = (NodLS*)malloc(sizeof(NodLS));
	nou->stud.medie = stud.medie;
	nou->stud.nrMatricol = stud.nrMatricol;
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

void traversare_ls(NodLS* prim) {
	NodLS* temp = prim;
	while (temp != NULL) {
		printf("\tNr. Matricol: %d \t Nume: %s \t Medie: %f\n",
			temp->stud.nrMatricol, temp->stud.nume, temp->stud.medie);
		temp = temp->next;
	}
}

void traversare_tabela(hashT tabela) {
	for (unsigned char i = 0; i < tabela.nrElem; i++) {
		if (tabela.vector[i] != NULL) {
			printf("Pozitia = %d\n", i);
			traversare_ls(tabela.vector[i]);
		}
	}
}

DateStud* extragerea_studentilor_sub_9(hashT tabela, DateStud* vector, int* nrElem) {
	for (int i = 0; i < tabela.nrElem; i++)
	{
		if (tabela.vector[i] != NULL) {
			NodLS* temp = tabela.vector[i];
			NodLS* prev = NULL;
			while (temp != NULL) {
				NodLS* deleteMe = temp;
				if (temp->stud.medie < 9.0) {
					if (prev == NULL) {
						vector[*nrElem].nrMatricol = temp->stud.nrMatricol;
						vector[*nrElem].nume = temp->stud.nume;
						(*nrElem)++;

						tabela.vector[i] = temp->next;
					}
					else if (temp->next == NULL) {
						vector[*nrElem].nrMatricol = temp->stud.nrMatricol;
						vector[*nrElem].nume = temp->stud.nume;
						(*nrElem)++;

						prev->next = NULL;
					}
					else {
						vector[*nrElem].nrMatricol = temp->stud.nrMatricol;
						vector[*nrElem].nume = temp->stud.nume;
						(*nrElem)++;

						prev->next = temp->next;
					}
					temp = temp->next;
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

void dezalocare_ls(NodLS* prim) {
	NodLS* temp = prim;
	while (temp != NULL) {
		NodLS* aux = temp->next;
		free(temp->stud.nume);
		free(temp);
		temp = aux;
	}
}

void dezalocare_vector(DateStud* vector, int nrElem) {
	for (int i = 0; i < nrElem; i++) {
		free(vector[i].nume);
	}
	free(vector);
}

void dezalocare_hash(hashT tabela) {
	for (unsigned char i = 0; i < tabela.nrElem; i++) {
		if (tabela.vector[i] != NULL) {
			dezalocare_ls(tabela.vector[i]);
		}
	}
	free(tabela.vector);
}

int main() {
	hashT tabela;
	tabela.nrElem = 13;
	tabela.vector = (NodLS**)malloc(tabela.nrElem * sizeof(NodLS*));
	for (unsigned char i = 0; i < tabela.nrElem; i++) {
		tabela.vector[i] = NULL;
	}

	FILE* f = fopen("Lista.txt", "r");
	if (f == NULL) {
		printf("Fisierul mnu s-a deschis");
		return -1;
	}

	char buffer[128];
	Student stud;
	
	while (fscanf(f, "%d", &stud.nrMatricol) == 1) 
	{
		fscanf(f, " %[^\n]", buffer);
		stud.nume = (char*)malloc(strlen(buffer) + 1);
		strcpy(stud.nume, buffer);
		
		fscanf(f, "%f", &stud.medie);

		inserare_hash(tabela, stud);

		free(stud.nume);
	}
	fclose(f);

	traversare_tabela(tabela);

	//sterge studentii cu note mici din tabela si salveaza-i intr-un vector
	DateStud* studentiSub9 = (DateStud*)malloc(tabela.nrElem * sizeof(DateStud));
	int nrStud = 0;
	studentiSub9 = extragerea_studentilor_sub_9(tabela, studentiSub9, &nrStud);
	printf("\nLista din vector\n");
	for (int i = 0; i < nrStud; i++) {
		printf("Nr. Matricol: %d \t Nume: %s\n", studentiSub9[i].nrMatricol, studentiSub9[i].nume);
	}

	printf("\nTabela dupa traversare\n");
	traversare_tabela(tabela);

	dezalocare_vector(studentiSub9, nrStud);
	dezalocare_hash(tabela);
	return 0;
}