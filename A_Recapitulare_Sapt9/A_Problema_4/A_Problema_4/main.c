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

NodLS* inserare_ls_sfarsit(NodLS* prim, Student stud) {
	NodLS* nou = (NodLS*)malloc(sizeof(NodLS));
	nou->stud.nrMatricol = stud.nrMatricol;
	nou->stud.medie = stud.medie;
	nou->stud.nume = (char*)malloc(strlen(stud.nume) + 1);
	strcpy(nou->stud.nume, stud.nume);
	nou->next = NULL;

	if (prim == NULL)
		prim = nou;
	else
	{
		NodLS* temp = prim;
		while (temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = nou;
	}
	return prim;
}

NodLS* inserare_ls_inceput(NodLS* prim, Student stud) {
	NodLS* nou = (NodLS*)malloc(sizeof(NodLS));
	nou->stud.nrMatricol = stud.nrMatricol;
	nou->stud.medie = stud.medie;
	nou->stud.nume = (char*)malloc(strlen(stud.nume) + 1);
	strcpy(nou->stud.nume, stud.nume);

	if (prim == NULL) {
		nou->next = NULL;
		prim = nou;
	}
	else
	{
		nou->next = prim;
		prim = nou;
	}

	return prim;
}

NodLS* inserare_ls_index(NodLS* prim, Student stud, int index) {
	NodLS* nou = (NodLS*)malloc(sizeof(NodLS));
	nou->stud.nrMatricol = stud.nrMatricol;
	nou->stud.medie = stud.medie;
	nou->stud.nume = (char*)malloc(strlen(stud.nume) + 1);
	strcpy(nou->stud.nume, stud.nume);

	if (index < 0) { return prim; }
	else {
		int i = 0;
		NodLS* temp = prim;
		NodLS* prev = NULL;
		while (temp != NULL)
		{
			if (i == index) {
				if (temp == prim) {
					nou->next = prim;
					prim = nou;
					break;
				}
				else if (temp->next == NULL) {
					nou->next = NULL;
					temp->next = nou;
					break;
				}
				else {
					prev->next = nou;
					nou->next = temp;
					break;
				}
			}
			else
			{
				prev = temp;
				temp = temp->next;
				i++;
			}
		}
		return prim;
	}
}

NodLS* sterge_nod(NodLS* prim, int nrMatricol) {
	NodLS* temp = prim;
	NodLS* prev = NULL;
	while (temp != NULL)
	{
		if (temp->stud.nrMatricol == nrMatricol) {
			NodLS* deleteMe = temp;
			if (temp == prim) {
				prim = prim->next;
			}
			else if (temp->next == NULL) {
				prev->next = NULL;
			}
			else {
				prev->next = temp->next;
			}
			temp = temp->next;
			free(deleteMe->stud.nume);
			free(deleteMe);
		}
		else {
			prev = temp;
			temp = temp->next;
		}
	}
	return prim;
}

void traversare_ls(NodLS* prim) {
	NodLS* temp = prim;
	while (temp != NULL)
	{
		printf("Nr. Matricol: %d \t Nume: %s \t\t Medie: %f\n", temp->stud.nrMatricol, temp->stud.nume, temp->stud.medie);
		temp = temp->next;
	}
}

void salvare_vector_studenti_nrMat_1(NodLS* prim, Student** vector, int* nrElem, int nrMat) {
	NodLS* temp = prim;
	while (temp != NULL) {
		if (temp->stud.nrMatricol == nrMat) {
			(*vector)[*nrElem].nrMatricol = temp->stud.nrMatricol;
			(*vector)[*nrElem].medie = temp->stud.medie;
			(*vector)[*nrElem].nume = (char*)malloc(strlen(temp->stud.nume) + 1);
			strcpy((*vector)[*nrElem].nume, temp->stud.nume);
			(*nrElem)++;
		}
		temp = temp->next;
	}
}

void salvare_vector_studenti_nrMat_2(NodLS* prim, Student* vector, int* nrElem, int nrMat) {
	NodLS* temp = prim;
	while (temp != NULL) {
		if (temp->stud.nrMatricol == nrMat) {
			vector[*nrElem].nrMatricol = temp->stud.nrMatricol;
			vector[*nrElem].medie = temp->stud.medie;
			vector[*nrElem].nume = (char*)malloc(strlen(temp->stud.nume) + 1);
			strcpy(vector[*nrElem].nume, temp->stud.nume);
			(*nrElem)++;
		}
		temp = temp->next;
	}
}


void dezalocare_ls(NodLS* prim) {
	NodLS* temp = prim;
	while (temp != NULL)
	{
		NodLS* aux = temp->next;
		free(temp->stud.nume);
		free(temp);
		temp = aux;
	}
}

void dezalocare_vector_1(Student** vector, int nrElem) {
	for (int i = 0; i < nrElem; i++)
	{
		free((*vector)[i].nume);
	}
	free((*vector));
}

void dezalocare_vector_2(Student* vector, int nrElem) {
	for (int i = 0; i < nrElem; i++)
	{
		free(vector[i].nume);
	}
	free(vector);
}

int main() {
	NodLS* prim = NULL;

	FILE* f = fopen("Lista.txt", "r");
	if (f == NULL) {
		printf("Lista nu a fost deschisa");
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

		prim = inserare_ls_sfarsit(prim, stud);

		free(stud.nume);
	}

	printf("Lista din fisier\n");
	traversare_ls(prim);

	Student studDeInserat;
	studDeInserat.nrMatricol = 210;
	studDeInserat.medie = 9.5;
	studDeInserat.nume = (char*)malloc(strlen("Serban Timofte") + 1);
	strcpy(studDeInserat.nume, "Serban Timofte");

	Student studDeInserat2;
	studDeInserat2.nrMatricol = 320;
	studDeInserat2.medie = 8.5;
	studDeInserat2.nume = (char*)malloc(strlen("Ionel Popescu") + 1);
	strcpy(studDeInserat2.nume, "Ionel Popescu");

	prim = inserare_ls_sfarsit(prim, studDeInserat);
	prim = inserare_ls_inceput(prim, studDeInserat);
	prim = inserare_ls_index(prim, studDeInserat, 2);

	printf("\nLista dupa inserari\n");
	traversare_ls(prim);

	prim = sterge_nod(prim, 210);
	//prim = sterge_nod(prim, 2000);
	printf("\nLista dupa stergere\n");
	traversare_ls(prim);

	prim = inserare_ls_sfarsit(prim, studDeInserat);
	prim = inserare_ls_inceput(prim, studDeInserat);
	prim = inserare_ls_index(prim, studDeInserat, 2);
	printf("\nLista refacuta\n");
	traversare_ls(prim);

	Student* studenti_nrMatricol_210 = (Student*)malloc(5 * sizeof(Student));
	int nrStud = 0;
	salvare_vector_studenti_nrMat_1(prim, &studenti_nrMatricol_210, &nrStud, 210);
	printf("\nVectorul de elemente\n");
	for (int i = 0; i < nrStud; i++) {
		printf("Nr. Matricol: %d \t Nume: %s \t\t Medie: %f\n", 
			studenti_nrMatricol_210[i].nrMatricol, studenti_nrMatricol_210[i].nume, studenti_nrMatricol_210[i].medie);
	}

	Student* studenti_nrMatricol_210_2 = (Student*)malloc(5 * sizeof(Student));
	int nrStud2 = 0;
	salvare_vector_studenti_nrMat_2(prim, studenti_nrMatricol_210_2, &nrStud2, 210);
	printf("\nVectorul 2 de elemente\n");
	for (int i = 0; i < nrStud; i++) {
		printf("Nr. Matricol: %d \t Nume: %s \t\t Medie: %f\n",
			studenti_nrMatricol_210_2[i].nrMatricol, studenti_nrMatricol_210_2[i].nume, studenti_nrMatricol_210_2[i].medie);
	}

	dezalocare_vector_1(&studenti_nrMatricol_210, nrStud);
	dezalocare_vector_2(studenti_nrMatricol_210_2, nrStud2);
	dezalocare_ls(prim);
	return 0;
}