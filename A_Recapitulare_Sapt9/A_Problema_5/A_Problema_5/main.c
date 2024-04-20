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
	struct NodLD* next;
	struct NodLD* prev;
} NodLD;

typedef struct {
	char* nume;
} NumeStud;

NodLD* inserare_ld_sfarsit(NodLD* prim, NodLD** ultim, Student stud) {
	NodLD* nou = (NodLD*)malloc(sizeof(NodLD));
	nou->stud.medie = stud.medie;
	nou->stud.nrMatricol = stud.nrMatricol;
	nou->stud.nume = (char*)malloc(strlen(stud.nume) + 1);
	strcpy(nou->stud.nume, stud.nume);
	nou->next = NULL;

	if (prim == NULL) {
		nou->prev = NULL;
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

NodLD* inserare_ld_inceput(NodLD* prim, NodLD** ultim, Student stud) {
	NodLD* nou = (NodLD*)malloc(sizeof(NodLD));
	nou->stud.medie = stud.medie;
	nou->stud.nrMatricol = stud.nrMatricol;
	nou->stud.nume = (char*)malloc(strlen(stud.nume) + 1);
	strcpy(nou->stud.nume, stud.nume);
	nou->next = NULL;

	if (prim == NULL) {
		nou->prev = NULL;
		prim = nou;
		*ultim = nou;
	}
	else {
		nou->next = prim;
		nou->prev = NULL;
		prim->prev = nou;
		prim = nou;
	}
	return prim;
}

NodLD* inserare_ld_index(NodLD* prim, NodLD** ultim, Student stud, int index) {
	NodLD* nou = (NodLD*)malloc(sizeof(NodLD));
	nou->stud.medie = stud.medie;
	nou->stud.nrMatricol = stud.nrMatricol;
	nou->stud.nume = (char*)malloc(strlen(stud.nume) + 1);
	strcpy(nou->stud.nume, stud.nume);

	NodLD* temp = prim;
	int i = 0;
	while (temp != NULL) {
		if (i == index) {
			if (temp == prim) {
				nou->next = prim;
				nou->prev = NULL;
				prim->prev = nou;
				prim = nou;
			}
			else if (temp == *ultim) {
				temp->next = nou;
				nou->next = NULL;
				nou->prev = temp;
				*ultim = nou;
			}
			else {
				NodLD* ant = temp->prev;
				nou->next = temp;
				nou->prev = ant;
				ant->next = nou;
				temp->prev = nou;
			}
			temp = temp->next;
			i++;
		}
		else {
			temp = temp->next;
			i++;
		}
	}
	return prim;
}

Student* salveaza_studenti_in_vector(NodLD** prim, NodLD** ultim, Student* vector, int* nrElem) {
	NodLD* temp = *prim;
	while (temp != NULL) {
		if (temp->stud.medie >= 9.0) {
			NodLD* deleteMe = temp;
			if (temp == *prim) {
				vector[*nrElem].medie = temp->stud.medie;
				vector[*nrElem].nrMatricol = temp->stud.nrMatricol;
				vector[*nrElem].nume = (char*)malloc(strlen(temp->stud.nume) + 1);
				strcpy(vector[*nrElem].nume, temp->stud.nume);
				(*nrElem)++;

				*prim = (*prim)->next;
				(*prim)->prev = NULL;
			}
			else if (temp == *ultim) {
				vector[*nrElem].medie = temp->stud.medie;
				vector[*nrElem].nrMatricol = temp->stud.nrMatricol;
				vector[*nrElem].nume = (char*)malloc(strlen(temp->stud.nume) + 1);
				strcpy(vector[*nrElem].nume, temp->stud.nume);
				(*nrElem)++;

				NodLD* ant = temp->prev;
				ant->next = NULL;
				*ultim = ant;
			}
			else {
				vector[*nrElem].medie = temp->stud.medie;
				vector[*nrElem].nrMatricol = temp->stud.nrMatricol;
				vector[*nrElem].nume = (char*)malloc(strlen(temp->stud.nume) + 1);
				strcpy(vector[*nrElem].nume, temp->stud.nume);
				(*nrElem)++;

				NodLD* ant = temp->prev;
				NodLD* urm = temp->next;
				ant->next = urm;
				urm->prev = ant;
			}
			temp = temp->next;
			free(deleteMe->stud.nume);
			free(deleteMe);
		}
		else {
			temp = temp->next;
		}
	}
	return vector;
}

NumeStud* salveaza_numele_in_vector(NodLD** prim, NodLD** ultim, NumeStud* vector, int* nrElem) {
	NodLD* temp = *prim;
	while (temp != NULL) {
		if (temp->stud.medie >= 9.0) {
			NodLD* deleteMe = temp;
			if (temp == *prim) {
				vector[*nrElem].nume = (char*)malloc(strlen(temp->stud.nume) + 1);
				strcpy(vector[*nrElem].nume, temp->stud.nume);
				(*nrElem)++;

				*prim = (*prim)->next;
				(*prim)->prev = NULL;
			}
			else if (temp == *ultim) {
				vector[*nrElem].nume = (char*)malloc(strlen(temp->stud.nume) + 1);
				strcpy(vector[*nrElem].nume, temp->stud.nume);
				(*nrElem)++;

				NodLD* ant = temp->prev;
				ant->next = NULL;
				*ultim = ant;
			}
			else {
				vector[*nrElem].nume = (char*)malloc(strlen(temp->stud.nume) + 1);
				strcpy(vector[*nrElem].nume, temp->stud.nume);
				(*nrElem)++;

				NodLD* ant = temp->prev;
				NodLD* urm = temp->next;
				ant->next = urm;
				urm->prev = ant;
			}
			temp = temp->next;
			free(deleteMe->stud.nume);
			free(deleteMe);
		}
		else {
			temp = temp->next;
		}
	}
	return vector;
}

void afisare_ld_prim(NodLD* prim) {
	NodLD* temp = prim;
	while (temp != NULL) {
		printf("Nr. Matricol: %d \t Nume: %s \t\t Medie: %f\n", temp->stud.nrMatricol, temp->stud.nume, temp->stud.medie);
		temp = temp->next;
	}
}

void afisare_ld_ultim(NodLD* ultim) {
	NodLD* temp = ultim;
	while (temp != NULL) {
		printf("Nr. Matricol: %d \t Nume: %s \t\t Medie: %f\n", temp->stud.nrMatricol, temp->stud.nume, temp->stud.medie);
		temp = temp->prev;
	}
}

void dezalocare_vector_studenti(Student* vector, int nrElem) {
	for (int i = 0; i < nrElem; i++) {
		free(vector[i].nume);
	}
	free(vector);
}

void dezalocare_vector_nume(NumeStud* vector, int nrElem) {
	for (int i = 0; i < nrElem; i++) {
		free(vector[i].nume);
	}
	free(vector);
}

void dezalocare_ld(NodLD* prim) {
	NodLD* temp = prim;
	while (temp != NULL) {
		NodLD* aux = temp->next;
		free(temp->stud.nume);
		free(temp);
		temp = aux;
	}
}

int main() { 
	NodLD* prim = NULL;
	NodLD* ultim = NULL;

	FILE* f = fopen("Lista.txt", "r");
	if (f == NULL) {
		printf("Lista nu a fost deschisa");
		return -1;
	}

	char buffer[128];
	Student stud;

	while (fscanf(f, "%d", &stud.nrMatricol) == 1) {
		fscanf(f, " %[^\n]", buffer);
		stud.nume = (char*)malloc(strlen(buffer) + 1);
		strcpy(stud.nume, buffer);
		fscanf(f, "%f", &stud.medie);

		prim = inserare_ld_inceput(prim, &ultim, stud);

		free(stud.nume);
	}

	printf("Lista din fisier cap -> coada\n");
	afisare_ld_prim(prim);

	printf("\nLista din fisier coada -> cap\n");
	afisare_ld_ultim(ultim);

	Student studDeInserat;
	studDeInserat.nrMatricol = 210;
	studDeInserat.medie = 9.5;
	studDeInserat.nume = (char*)malloc(strlen("Serban Timofte") + 1);
	strcpy(studDeInserat.nume, "Serban Timofte");

	prim = inserare_ld_sfarsit(prim, &ultim, studDeInserat);
	prim = inserare_ld_inceput(prim, &ultim, studDeInserat);
	prim = inserare_ld_index(prim, &ultim, studDeInserat, 0);
	prim = inserare_ld_index(prim, &ultim, studDeInserat, 5);
	prim = inserare_ld_index(prim, &ultim, studDeInserat, 3);

	printf("\nLista dupa inserare cap -> coada\n");
	afisare_ld_prim(prim);
	printf("\nLista dupa inserare coada -> cap\n");
	afisare_ld_ultim(ultim);

	//stundetii cu medii mai mari de 9 sunt salvati in vector si stersi din lista dubla
	Student* studentiCuMediiMari = (Student*)malloc(10 * sizeof(Student));
	int nrStud1 = 0;
	studentiCuMediiMari = salveaza_studenti_in_vector(&prim, &ultim, studentiCuMediiMari, &nrStud1);
	printf("\nVectorul cu studentii cu medii mari\n");
	for (int i = 0; i < nrStud1; i++) {
		printf("Nr. Matricol: %d \t Nume: %s \t\t Medie: %f\n",
			studentiCuMediiMari[i].nrMatricol, studentiCuMediiMari[i].nume, studentiCuMediiMari[i].medie);
	}

	printf("\nLista dupa crearea vectorului cap -> coada\n");
	afisare_ld_prim(prim);
	printf("\nLista dupa crearea vectorului coada -> cap\n");
	afisare_ld_ultim(ultim);

	//completam lista pentru a putea extrage in continuare 
	prim = inserare_ld_sfarsit(prim, &ultim, studDeInserat);
	prim = inserare_ld_inceput(prim, &ultim, studDeInserat);
	prim = inserare_ld_index(prim, &ultim, studDeInserat, 0);
	prim = inserare_ld_index(prim, &ultim, studDeInserat, 5);
	prim = inserare_ld_index(prim, &ultim, studDeInserat, 3);
	printf("\nLista repopulata cap -> coada\n");
	afisare_ld_prim(prim);

	//numele studentilor cu medii mai mari de 9 sunt salvati in vector si stersi din lista dubla
	NumeStud* numeleStudentilorCuMediiMari = (NumeStud*)malloc(5 * sizeof(NumeStud));
	int nrStud2 = 0;
	numeleStudentilorCuMediiMari = salveaza_numele_in_vector(&prim, &ultim, numeleStudentilorCuMediiMari, &nrStud2);
	printf("\nNumele studentilor din vectorul de nume\n");
	for (int i = 0; i < nrStud2; i++) {
		printf("Nume: %s\n", numeleStudentilorCuMediiMari[i].nume);
	}

	dezalocare_vector_studenti(studentiCuMediiMari, nrStud1);
	dezalocare_vector_nume(numeleStudentilorCuMediiMari, nrStud2);
	dezalocare_ld(prim);

	return 0;
}