#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

typedef struct {
	int nrMatricol;
	char* nume;
	float medie;
} student;

typedef struct {
	student inf;
	struct nodArbBin* stanga;
	struct nodArbBin* dreapta;
} nodArbBin;

nodArbBin* creareNod(student stud, nodArbBin* st, nodArbBin* dr) {
	nodArbBin* nou = (nodArbBin*)malloc(sizeof(nodArbBin));
	nou->inf.medie = stud.medie;
	nou->inf.nrMatricol = stud.nrMatricol;
	nou->inf.nume = (char*)malloc(strlen(stud.nume) + 1);
	strcpy(nou->inf.nume, stud.nume);

	nou->stanga = st;
	nou->dreapta = dr;

	return nou;
}

nodArbBin* inserareNod(nodArbBin* rad, student stud) {
	if (rad != NULL) {
		if (stud.nrMatricol < rad->inf.nrMatricol) {
			rad->stanga = inserareNod(rad->stanga, stud);
			return rad;
		}
		else if (stud.nrMatricol > rad->inf.nrMatricol) {
			rad->dreapta = inserareNod(rad->dreapta, stud);
			return rad;
		}
		else {
			return rad;
		}
	}
	else {
		return creareNod(stud, NULL, NULL);
	}
}

void inordine(nodArbBin* rad) {
	if (rad != NULL) {
		inordine(rad->stanga);
		printf("Nr. Matricol = %d \t Medie = %f \t Nume = %s \n",
			rad->inf.nrMatricol, rad->inf.medie, rad->inf.nume);
		inordine(rad->dreapta);
	}
}

void preordine(nodArbBin* rad) {
	if (rad != NULL) {
		printf("Nr. Matricol = %d \t Medie = %f \t Nume = %s \n",
			rad->inf.nrMatricol, rad->inf.medie, rad->inf.nume);
		preordine(rad->stanga);
		preordine(rad->dreapta);
	}
}

void postordine(nodArbBin* rad) {
	if (rad != NULL) {
		postordine(rad->stanga);
		postordine(rad->dreapta);
		printf("Nr. Matricol = %d \t Medie = %f \t Nume = %s \n",
			rad->inf.nrMatricol, rad->inf.medie, rad->inf.nume);
	}
}

void salvareInVectorFrunzele(nodArbBin* rad, student* vector, int* nrElem) {
	if (rad != NULL) {
		if (rad->stanga == NULL && rad->dreapta == NULL) {
			vector[*nrElem] = rad->inf;
			(*nrElem)++;
		}
		salvareInVectorFrunzele(rad->stanga, vector, nrElem);
		salvareInVectorFrunzele(rad->dreapta, vector, nrElem);
	}
}

void nrStudentiPrag(nodArbBin* rad, int* counter, float prag) {
	if (rad != NULL) {
		if (rad->inf.medie > prag) {
			(*counter)++;
		}
		nrStudentiPrag(rad->stanga, counter, prag);
		nrStudentiPrag(rad->dreapta, counter, prag);
	}
}

nodArbBin* findMin(nodArbBin* rad) {
	while (rad->stanga != NULL)
	{
		rad = rad->stanga;
	}
	return rad;
}

nodArbBin* stergereNod(nodArbBin* rad, int nrMatricol) {
	if (rad == NULL) {
		return rad;
	}

	if (nrMatricol < rad->inf.nrMatricol) {
		rad->stanga = stergereNod(rad->stanga, nrMatricol);
	}
	
	if (nrMatricol > rad->inf.nrMatricol) {
		rad->dreapta = stergereNod(rad->dreapta, nrMatricol);
	}

	else {
		// am identificat nodul de sters
		if (rad->stanga == NULL) {
			nodArbBin* temp = rad->dreapta;
			free(rad->inf.nume);
			free(rad);
			return temp;
		} 
		else if (rad->dreapta == NULL) {
			nodArbBin* temp = rad->stanga;
			free(rad->inf.nume);
			free(rad);
			return temp;
		}

		//nodul are doi copii
		nodArbBin* temp = findMin(rad->dreapta);
		rad->inf.nrMatricol = temp->inf.nrMatricol;
		rad->inf.medie = temp->inf.medie;
		free(rad->inf.nume);
		rad->inf.nume = (char*)malloc(strlen(temp->inf.nume) + 1);
		strcpy(rad->inf.nume, temp->inf.nume);

		rad->dreapta = stergereNod(rad->dreapta, temp->inf.nrMatricol);
	}

	return rad;
}

int main() {
	nodArbBin* rad = NULL;

	FILE* f = fopen("fisier.txt", "r");
	if (f == NULL) {
		printf("Fisierul nu a fost deschis!");
		return -1;
	}

	int nrStud;
	student stud;
	char buffer[128];

	fscanf(f, "%d", &nrStud);
	for (int i = 0; i < nrStud; i++) {
		fscanf(f, "%d", &stud.nrMatricol);

		fscanf(f, "%s", buffer);
		stud.nume = (char*)malloc(strlen(buffer) + 1);
		strcpy(stud.nume, buffer);

		fscanf(f, "%f", &stud.medie);

		rad = inserareNod(rad, stud);

		free(stud.nume);
	}
	fclose(f);

	printf("Arborele parcurs in INORDINE\n");
	inordine(rad);

	printf("\nArborele parcurs in PREORDINE\n");
	preordine(rad);

	printf("\nArborele parcurs in POSTORDINE\n");
	postordine(rad);

	int nrElem = 0;
	student* vector = (student*)malloc(nrStud * sizeof(student));
	salvareInVectorFrunzele(rad, vector, &nrElem);
	printf("\nStudentii din vector:\n");
	for (int i = 0; i < nrElem; i++) {
		printf("Nr. Matricol = %d \t Medie = %f \t Nume = %s \n",
			vector[i].nrMatricol, vector[i].medie, vector[i].nume);
	}

	int nrStudPrag = 0;
	float mediePrag = 9.0;
	nrStudentiPrag(rad, &nrStudPrag, mediePrag);
	printf("\nNumarul de stundeti cu medii mai mare de %f este: %d\n", mediePrag, nrStudPrag);

	int nrMatricolDeSters = 25; 
	rad = stergereNod(rad, nrMatricolDeSters);
	printf("\nArborele dupa stergerea studentului cu nr. matricol %d\n", nrMatricolDeSters);
	inordine(rad);

	return 0;
}