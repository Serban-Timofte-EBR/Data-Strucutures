#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct {
	int id;
	char* nume;
	char* facultate;
	float medie;
} Student;

typedef struct {
	Student inf;
	struct nodArbAVL* stanga;
	struct nodArbAVL* dreapta;
} nodArbAVL;

typedef struct {
	Student inf;
	struct nodLS* next;
} nodLS;

typedef struct {
	nodLS* inf;
	struct nodLL* next;
} nodLL;

void inserareArbore(nodArbAVL** rad, Student stud) {
	if (*rad != NULL) {
		if (stud.id < (*rad)->inf.id) {
			inserareArbore(&(*rad)->stanga, stud);
		}
		else if (stud.id > (*rad)->inf.id) {
			inserareArbore(&(*rad)->dreapta, stud);
		}
	}
	else {
		nodArbAVL* nou = (nodArbAVL*)malloc(sizeof(nodArbAVL));
		nou->inf.id = stud.id;
		nou->inf.medie = stud.medie;
		nou->inf.facultate = (char*)malloc(strlen(stud.facultate) + 1);
		strcpy(nou->inf.facultate, stud.facultate);
		nou->inf.nume = (char*)malloc(strlen(stud.nume) + 1);
		strcpy(nou->inf.nume, stud.nume);
		nou->stanga = NULL;
		nou->dreapta = NULL;
		*rad = nou;
	}
}

void rotire_dreapta(nodArbAVL** rad) {
	nodArbAVL* aux = (*rad)->stanga;
	(*rad)->stanga = aux->dreapta;
	aux->dreapta = *rad;
	*rad = aux;
}

void rotire_stanga(nodArbAVL** rad) {
	nodArbAVL* aux = (*rad)->dreapta;
	(*rad)->dreapta = aux->stanga;
	aux->stanga = *rad;
	*rad = aux;
}

int maxim(int a, int b) {
	int max = a;
	if (b > a) {
		max = b;
	}
	return max;
}

int inaltime(nodArbAVL* rad) {
	if (rad == NULL) {
		return 0;
	}

	int inaltS = inaltime(rad->stanga);
	int inaltD = inaltime(rad->dreapta);

	return 1 + maxim(inaltS, inaltD);
}

int getDezechilibru(nodArbAVL* rad) {
	if (rad == NULL) {
		return 0;
	}

	return inaltime(rad->stanga) - inaltime(rad->dreapta);
}

void inserareAVL(nodArbAVL** rad, Student stud) {
	if (*rad != NULL) {
		if (stud.id < (*rad)->inf.id) {
			inserareArbore(&(*rad)->stanga, stud);
		}
		else if (stud.id > (*rad)->inf.id) {
			inserareArbore(&(*rad)->dreapta, stud);
		}
	}
	else {
		nodArbAVL* nou = (nodArbAVL*)malloc(sizeof(nodArbAVL));
		nou->inf.id = stud.id;
		nou->inf.medie = stud.medie;
		nou->inf.facultate = (char*)malloc(strlen(stud.facultate) + 1);
		strcpy(nou->inf.facultate, stud.facultate);
		nou->inf.nume = (char*)malloc(strlen(stud.nume) + 1);
		strcpy(nou->inf.nume, stud.nume);
		nou->stanga = NULL;
		nou->dreapta = NULL;
		*rad = nou;
	}

	int dez = getDezechilibru(*rad);

	if (dez == -2) {
		if (getDezechilibru((*rad)->dreapta) == 1) {
			rotire_dreapta(&(*rad)->dreapta);
		}
		rotire_stanga(rad);
	}

	if (dez == 2) {
		if (getDezechilibru((*rad)->stanga) == -1) {
			rotire_stanga(&(*rad)->stanga);
		}
		rotire_dreapta(rad);
	}
}

void inordine(nodArbAVL* rad) {
	if (rad != NULL) {
		inordine(rad->stanga);
		printf("ID: %d, Medie: %5.2f, Student: %s, Facultate: %s\n",
			rad->inf.id, rad->inf.medie, rad->inf.nume, rad->inf.facultate);
		inordine(rad->dreapta);
	}
}

void modificareMedieStud(nodArbAVL** rad, int id, float medieNoua) {
	if (*rad == NULL) {
		return;
	}

	if ((*rad)->inf.id == id) {
		(*rad)->inf.medie = medieNoua;
	}

	if (id < (*rad)->inf.id) {
		modificareMedieStud(&(*rad)->stanga, id, medieNoua);
	}

	else if (id > (*rad)->inf.id) {
		modificareMedieStud(&(*rad)->dreapta, id, medieNoua);
	}
}

int counterStudentiFac(nodArbAVL* rad, char* facultate) {
	int counter = 0;

	if (rad == NULL) {
		return 0;
	}

	if (strcmp(rad->inf.facultate, facultate) == 0) {
		counter++;
	}

	counter += counterStudentiFac(rad->stanga, facultate);
	counter += counterStudentiFac(rad->dreapta, facultate);

	return counter;
}

float sumaMediilorFac(nodArbAVL* rad, char* fac) {
	float val = 0.0;

	if (rad == NULL) {
		return 0.0;
	}

	if (strcmp(rad->inf.facultate, fac) == 0) {
		val += rad->inf.medie;
	}

	val += sumaMediilorFac(rad->stanga, fac);
	val += sumaMediilorFac(rad->dreapta, fac);

	return val;
}

float calculMedieFac(nodArbAVL* rad, char* fac) {
	int nrStud = counterStudentiFac(rad, fac);
	float sumaMedii = sumaMediilorFac(rad, fac);

	return sumaMedii / nrStud;
}

nodArbAVL* findMin(nodArbAVL* rad) {
	nodArbAVL* temp = rad;
	while (temp->stanga != NULL)
	{
		temp = temp->stanga;
	}
	return temp;
}

nodArbAVL* stergereNodCheie(nodArbAVL* rad, Student* out, int id) {
	if (rad == NULL) {
		return rad;
	}

	if (id < rad->inf.id) {
		rad->stanga = stergereNodCheie(rad->stanga, out, id);
	}

	else if (id > rad->inf.id) {
		rad->dreapta = stergereNodCheie(rad->dreapta, out, id);
	}

	else {
		if (rad->stanga == NULL) {
			nodArbAVL* temp = rad->dreapta;
			if ((*out).medie == -1.0) {
				(*out).medie = rad->inf.medie;
				(*out).id = rad->inf.id;
				(*out).facultate = (char*)malloc(strlen(rad->inf.facultate) + 1);
				strcpy((*out).facultate, rad->inf.facultate);
				(*out).nume = (char*)malloc(strlen(rad->inf.nume) + 1);
				strcpy((*out).nume, rad->inf.nume);
			}
			free(rad->inf.facultate);
			free(rad->inf.nume);
			return temp;
		}

		else if (rad->dreapta == NULL) {
			nodArbAVL* temp = rad->stanga;
			if ((*out).medie == -1.0) {
				(*out).medie = rad->inf.medie;
				(*out).id = rad->inf.id;
				(*out).facultate = (char*)malloc(strlen(rad->inf.facultate) + 1);
				strcpy((*out).facultate, rad->inf.facultate);
				(*out).nume = (char*)malloc(strlen(rad->inf.nume) + 1);
				strcpy((*out).nume, rad->inf.nume);
			}
			free(rad->inf.facultate);
			free(rad->inf.nume);
			return temp;
		}

		nodArbAVL* temp = findMin(rad->dreapta);
		(*out).medie = rad->inf.medie;
		(*out).id = rad->inf.id;
		(*out).facultate = (char*)malloc(strlen(rad->inf.facultate) + 1);
		strcpy((*out).facultate, rad->inf.facultate);
		(*out).nume = (char*)malloc(strlen(rad->inf.nume) + 1);
		strcpy((*out).nume, rad->inf.nume);

		rad->inf.id = temp->inf.id;
		rad->inf.medie = temp->inf.medie;
		free(rad->inf.facultate);
		rad->inf.facultate = (char*)malloc(strlen(temp->inf.facultate) + 1);
		strcpy(rad->inf.facultate, temp->inf.facultate);
		free(rad->inf.nume);
		rad->inf.nume = (char*)malloc(strlen(temp->inf.nume) + 1);
		strcpy(rad->inf.nume, temp->inf.nume);

		rad->dreapta = stergereNodCheie(rad->dreapta, out, temp->inf.id);
	}
	return rad;
}

nodArbAVL* modificareCheie(nodArbAVL* rad, int idCurent, int idNou) {
	Student stud;
	stud.medie = -1.0;
	rad = stergereNodCheie(rad, &stud, idCurent);
	stud.id = idNou;
	inserareAVL(&rad, stud);
	return rad;
}

void inserareLS(nodLS** prim, Student stud) {
	nodLS* nou = (nodLS*)malloc(sizeof(nodLS));
	nou->inf = stud;
	nou->next = NULL;

	if (*prim == NULL) {
		*prim = nou;
	}
	else {
		nodLS* temp = *prim;
		nodLS* prev = NULL;
		while (temp != NULL && temp->inf.medie < stud.medie)
		{
			prev = temp;
			temp = temp->next;
		}

		if (temp == NULL) {
			prev->next = nou;
		}
		else if (temp->inf.medie > stud.medie) {
			if (prev == NULL) {
				nou->next = *prim;
				*prim = nou;
			}
			else {
				prev->next = nou;
				nou->next = temp;
			}
		}
	}
}

void inserareLL(nodLL** prim, nodLS* inf) {
	nodLL* nou = (nodLL*)malloc(sizeof(nodLL));
	nou->inf = inf;
	nou->next = NULL;

	if (*prim == NULL) {
		*prim = nou;
	}
	else {
		nodLL* temp = *prim;
		while (temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = nou;
	}
}

nodLL* checkNodLL(nodLL* prim, char* facultate) {
	nodLL* temp = prim;
	while (temp != NULL)
	{
		if (strcmp(temp->inf->inf.facultate, facultate) == 0) {
			return temp;
		}
		temp = temp->next;
	}
	return NULL;
}

void arbore_lista(nodArbAVL* rad, nodLL** prim) {
	if (rad == NULL) {
		return;
	}

	nodLL* checker = checkNodLL(*prim, rad->inf.facultate);
	if (checker != NULL) {
		inserareLS(&(*prim)->inf, rad->inf);
	}
	else {
		nodLS* inf = NULL;
		inserareLS(&inf, rad->inf);
		inserareLL(prim, inf);
	}

	arbore_lista(rad->stanga, prim);
	arbore_lista(rad->dreapta, prim);
}

void afisareLS(nodLS* prim) {
	nodLS* temp = prim;
	while (temp != NULL)
	{
		printf("\tID: %d, Medie: %5.2f, Student: %s, Facultate: %s\n",
			temp->inf.id, temp->inf.medie, temp->inf.nume, temp->inf.facultate);
		temp = temp->next;
	}
}

void afisareLL(nodLL* prim) {
	nodLL* temp = prim;
	while (temp != NULL)
	{
		printf("Facultatea %s:\n", temp->inf->inf.facultate);
		afisareLS(temp->inf);
		temp = temp->next;
	}
}

nodLL* modificareMedie(nodLL* prim, int id, float medieNoua) {
	nodLL* temp = prim;
	while (temp != NULL)
	{
		nodLS* temp2 = temp->inf;
		while (temp2 != NULL)
		{
			if (temp2->inf.id == id) {
				temp2->inf.medie = medieNoua;
			}
			temp2 = temp2->next;
		}
		temp = temp->next;
	}
	return prim;
}

int main() {
	nodArbAVL* rad = NULL;

	FILE* f = fopen("fisier.txt", "r");

	Student stud;
	char buffer[128];

	while (fscanf(f, "%d", &stud.id) == 1) 
	{
		fscanf(f, " %[^\n]", buffer);
		stud.nume = (char*)malloc(strlen(buffer) + 1);
		strcpy(stud.nume, buffer);

		fscanf(f, " %[^\n]", buffer);
		stud.facultate = (char*)malloc(strlen(buffer) + 1);
		strcpy(stud.facultate, buffer);

		fscanf(f, "%f", &stud.medie);

		/*printf("ID: %d, Medie: %5.2f, Student: %s, Facultate: %s\n",
			stud.id, stud.medie, stud.nume, stud.facultate);*/
		inserareAVL(&rad, stud);

		free(stud.facultate);
		free(stud.nume);
	}
	fclose(f);

	printf("Arborele din fisier INORDINE:\n");
	inordine(rad);

	modificareMedieStud(&rad, 76, 9.9);
	modificareMedieStud(&rad, 100, 10.0);
	printf("\nArborele dupa modificare:\n");
	inordine(rad);

	float medieFac = calculMedieFac(rad, "Informatica Economica");
	printf("\nMedia mediilor la facultatea ceruta este: %5.2f\n", medieFac);

	rad = modificareCheie(rad, 76, 81);
	printf("\nArborele dupa modificare:\n");
	inordine(rad);

	nodLL* prim = NULL;
	arbore_lista(rad, &prim);
	printf("\nLista de liste:\n");
	afisareLL(prim);

	prim = modificareMedie(prim, 99, 9.78);
	printf("\nLista de liste:\n");
	afisareLL(prim);

	return 0;
}