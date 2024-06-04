#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct {
	int cod;
	char* proiect;
	char* titlu;
	int nrOreAlocate;
	int prioritate;
} Task;

typedef struct {
	Task inf;
	struct nodArbAVL* stanga;
	struct nodArbAVL* dreapta;
} nodArbAVL;

typedef struct {
	Task inf;
	struct nodLD* next;
	struct nodLD* prev;
} nodLD;

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

	int inalS = inaltime(rad->stanga);
	int inalD = inaltime(rad->dreapta);

	return 1 + maxim(inalS, inalD);
}

int getDez(nodArbAVL* rad) {
	if (rad == NULL) {
		return 0;
	}

	return inaltime(rad->stanga) - inaltime(rad->dreapta);
}

void inserareAVL(nodArbAVL** rad, Task task) {
	if (*rad != NULL) {
		if (task.cod < (*rad)->inf.cod) {
			inserareAVL(&(*rad)->stanga, task);
		}
		else if (task.cod > (*rad)->inf.cod) {
			inserareAVL(&(*rad)->dreapta, task);
		}
	}
	else {
		nodArbAVL* nou = (nodArbAVL*)malloc(sizeof(nodArbAVL));
		nou->inf.cod = task.cod;
		nou->inf.nrOreAlocate = task.nrOreAlocate;
		nou->inf.prioritate = task.prioritate;
		nou->inf.proiect = (char*)malloc(strlen(task.proiect) + 1);
		strcpy(nou->inf.proiect, task.proiect);
		nou->inf.titlu = (char*)malloc(strlen(task.titlu) + 1);
		strcpy(nou->inf.titlu, task.titlu);
		nou->stanga = NULL;
		nou->dreapta = NULL;
		*rad = nou;
	}

	int dez = getDez(*rad);

	if (dez == -2) {
		if (getDez((*rad)->dreapta) == 1) {
			rotire_dreapta(&(*rad)->dreapta);
		}
		rotire_stanga(rad);
	} 

	if (dez == 2) {
		if (getDez((*rad)->stanga) == -1) {
			rotire_stanga(&(*rad)->stanga);
		}
		rotire_dreapta(rad);
	}
}

void inordine(nodArbAVL* rad) {
	if (rad != NULL) {
		inordine(rad->stanga);
		printf("Cod: %d, Nr. Ore: %d, Prioritate: %d, Proiect: %s, Titlu: %s\n",
			rad->inf.cod, rad->inf.nrOreAlocate, rad->inf.prioritate,
			rad->inf.proiect, rad->inf.titlu);
		inordine(rad->dreapta);
	}
}

nodArbAVL* findMin(nodArbAVL* rad) {
	nodArbAVL* temp = rad;
	while (temp->stanga != NULL) {
		temp = temp->stanga;
	}
	return temp;
}

nodArbAVL* stergereAVL(nodArbAVL* rad, int cod, Task* out) {
	if (rad == NULL) {
		return  rad;
	}

	if (cod < rad->inf.cod) {
		rad->stanga = stergereAVL(rad->stanga, cod, out);
	}

	else if (cod > rad->inf.cod) {
		rad->dreapta = stergereAVL(rad->dreapta, cod, out);
	} 

	else {
		if (rad->stanga == NULL) {
			nodArbAVL* temp = rad->dreapta;
			if ((*out).cod == -1) {
				(*out).cod = rad->inf.cod;
				(*out).nrOreAlocate = rad->inf.nrOreAlocate;
				(*out).prioritate = rad->inf.prioritate;
				(*out).proiect = (char*)malloc(strlen(rad->inf.proiect) + 1);
				strcpy((*out).proiect, rad->inf.proiect);
				(*out).titlu = (char*)malloc(strlen(rad->inf.titlu) + 1);
				strcpy((*out).titlu, rad->inf.titlu);
			}
			free(rad->inf.proiect);
			free(rad->inf.titlu);
			return temp;
		}

		else if (rad->dreapta == NULL) {
			nodArbAVL* temp = rad->stanga;
			if ((*out).cod == -1) {
				(*out).cod = rad->inf.cod;
				(*out).nrOreAlocate = rad->inf.nrOreAlocate;
				(*out).prioritate = rad->inf.prioritate;
				(*out).proiect = (char*)malloc(strlen(rad->inf.proiect) + 1);
				strcpy((*out).proiect, rad->inf.proiect);
				(*out).titlu = (char*)malloc(strlen(rad->inf.titlu) + 1);
				strcpy((*out).titlu, rad->inf.titlu);
			}
			free(rad->inf.proiect);
			free(rad->inf.titlu);
			return temp;
		}

		nodArbAVL* temp = findMin(rad->dreapta);

		(*out).cod = rad->inf.cod;
		(*out).nrOreAlocate = rad->inf.nrOreAlocate;
		(*out).prioritate = rad->inf.prioritate;
		(*out).proiect = (char*)malloc(strlen(rad->inf.proiect) + 1);
		strcpy((*out).proiect, rad->inf.proiect);
		(*out).titlu = (char*)malloc(strlen(rad->inf.titlu) + 1);
		strcpy((*out).titlu, rad->inf.titlu);

		rad->inf.cod = temp->inf.cod;
		rad->inf.nrOreAlocate = temp->inf.nrOreAlocate;
		rad->inf.prioritate = temp->inf.prioritate;
		free(rad->inf.titlu);
		rad->inf.titlu = (char*)malloc(strlen(temp->inf.titlu) + 1);
		strcpy(rad->inf.titlu, temp->inf.titlu);
		free(rad->inf.titlu);
		rad->inf.titlu = (char*)malloc(strlen(temp->inf.titlu) + 1);
		strcpy(rad->inf.titlu, temp->inf.titlu);

		rad->dreapta = stergereAVL(rad->dreapta, temp->inf.cod, out);
	}
	return rad;
}

int counterPrioritatiSubPrag(nodArbAVL* rad, int prag) {
	int counter = 0;

	if (rad == NULL) {
		return 0;
	}

	if (rad->inf.prioritate < prag) {
		counter++;
	}

	counter += counterPrioritatiSubPrag(rad->stanga, prag);
	counter += counterPrioritatiSubPrag(rad->dreapta, prag);

	return counter;
}

void salvareVector(nodArbAVL* rad, int* vector, int* index, int prag) {
	if (rad == NULL) {
		return;
	}

	if (rad->inf.prioritate < prag) {
		vector[*index] = rad->inf.cod;
		(*index)++;
	}

	salvareVector(rad->stanga, vector, index, prag);
	salvareVector(rad->dreapta, vector, index, prag);
}

int* idNoduriPrioritateSubPrag(nodArbAVL* rad, int* nrElem, int prag) {
	*nrElem = counterPrioritatiSubPrag(rad, prag);
	int* vector = (int*)malloc(*nrElem * sizeof(int));
	int index = 0;

	salvareVector(rad, vector, &index, prag);

	return vector;
}

void spargereArbore(nodArbAVL** rad1, nodArbAVL** rad2, int prag) {
	int nrElem = 0;
	int* vectorIDuriPrag = idNoduriPrioritateSubPrag(*rad1, &nrElem, prag);

	Task out;
	for (int i = 0; i < nrElem; i++) {
		out.cod = -1;
		*rad1 = stergereAVL(*rad1, vectorIDuriPrag[i], &out);
		inserareAVL(rad2, out);
		free(out.proiect);
		free(out.titlu);
	}

	free(vectorIDuriPrag);
}

void inserareLDSortat(nodLD** prim, nodLD** ultim, Task task) {
	nodLD* nou = (nodLD*)malloc(sizeof(nodLD));
	nou->inf.cod = task.cod;
	nou->inf.nrOreAlocate = task.nrOreAlocate;
	nou->inf.prioritate = task.prioritate;
	nou->inf.proiect = (char*)malloc(strlen(task.proiect) + 1);
	strcpy(nou->inf.proiect, task.proiect);
	nou->inf.titlu = (char*)malloc(strlen(task.titlu) + 1);
	strcpy(nou->inf.titlu, task.titlu);
	nou->next = NULL;
	nou->prev = NULL;

	if (*prim == NULL) {
		*prim = nou;
		*ultim = nou;
	}
	else {
		nodLD* temp = *prim;
		while (temp != NULL && temp->inf.prioritate < task.prioritate) {
			temp = temp->next;
		}

		if (temp == *prim) {
			nou->next = *prim;
			(*prim)->prev = nou;
			*prim = nou;
		}
		else if (temp == NULL) {
			(*ultim)->next = nou;
			nou->prev = *ultim;
			*ultim = nou;
		}
		else {
			nodLD* ant = temp->prev;
			ant->next = nou;
			nou->prev = ant;
			nou->next = temp;
			temp->prev = nou;
		}
	}
}

void arbore_ld(nodArbAVL* rad, nodLD** prim, nodLD** ultim) {
	if (rad == NULL) {
		return;
	}

	inserareLDSortat(prim, ultim, rad->inf);

	arbore_ld(rad->stanga, prim, ultim);
	arbore_ld(rad->dreapta, prim, ultim);
}

void afisareLD(nodLD* prim) {
	nodLD* temp = prim;
	while (temp != NULL) {
		printf("Cod: %d, Nr. Ore: %d, Prioritate: %d, Proiect: %s, Titlu: %s\n",
			temp->inf.cod, temp->inf.nrOreAlocate, temp->inf.prioritate, temp->inf.proiect, temp->inf.titlu);
		temp = temp->next;
	}
}

void afisareLDInvers(nodLD* ultim) {
	nodLD* temp = ultim;
	while (temp != NULL) {
		printf("Cod: %d, Nr. Ore: %d, Prioritate: %d, Proiect: %s, Titlu: %s\n",
			temp->inf.cod, temp->inf.nrOreAlocate, temp->inf.prioritate, temp->inf.proiect, temp->inf.titlu);
		temp = temp->prev;
	}
}

int counterNoduriLD(nodLD* prim, int prioritatePrag, int nrOrePrag) {
	int counter = 0;

	nodLD* temp = prim;
	while (temp != NULL) {
		if (temp->inf.nrOreAlocate > nrOrePrag && temp->inf.prioritate == prioritatePrag) {
			counter++;
		}
		temp = temp->next;
	}

	return counter;
}

Task* vectorLD(nodLD* prim, int prioritatePrag, int nrOrePrag, int* nrElem) {
	*nrElem = counterNoduriLD(prim, prioritatePrag, nrOrePrag);
	Task* vector = (int*)malloc(*nrElem * sizeof(Task));
	int index = 0;

	nodLD* temp = prim;
	while (temp != NULL) {
		if (temp->inf.nrOreAlocate > nrOrePrag && temp->inf.prioritate == prioritatePrag) {
			vector[index] = temp->inf;
			index++;
		}
		temp = temp->next;
	}

	return vector;
}

void dezalocareArbore(nodArbAVL* rad) {
	if (rad != NULL) {
		dezalocareArbore(rad->stanga);
		dezalocareArbore(rad->dreapta);
		free(rad->inf.proiect);
		free(rad->inf.titlu);
		free(rad);
	}
}

void dezalocareLD(nodLD* prim) {
	while (prim != NULL) {
		nodLD* aux = prim->next;
		free(prim->inf.proiect);
		free(prim->inf.titlu);
		free(prim);
		prim = aux;
	}
}

int main() {
	nodArbAVL* rad = NULL;

	FILE* f = fopen("fisier.txt", "r");

	Task task;
	char buffer[128];

	while (fscanf(f, "%d", &task.cod) == 1) 
	{
		fscanf(f, " %[^\n]", buffer);
		task.proiect = (char*)malloc(strlen(buffer) + 1);
		strcpy(task.proiect, buffer);

		fscanf(f, " %[^\n]", buffer);
		task.titlu = (char*)malloc(strlen(buffer) + 1);
		strcpy(task.titlu, buffer);

		fscanf(f, "%d", &task.nrOreAlocate);
		fscanf(f, "%d", &task.prioritate);

		/*printf("Cod: %d, Nr. Ore: %d, Prioritate: %d, Proiect: %s, Titlu: %s\n",
			task.cod, task.nrOreAlocate, task.prioritate, task.proiect, task.titlu);*/
		inserareAVL(&rad, task);

		free(task.titlu);
		free(task.proiect);
	}
	fclose(f);

	printf("Arborele din fisier:\n");
	inordine(rad);

	/*Task out;
	out.cod = -1;
	rad = stergereAVL(rad, 67, &out);
	printf("\nArborele 1 dupa modificare:\n");
	inordine(rad);*/

	/*int counter = counterPrioritatiSubPrag(rad, 3);
	printf("\n%d\n", counter);

	int* vector = idNoduriPrioritateSubPrag(rad, &counter, 3);
	for (int i = 0; i < counter; i++) {
		printf("\n%d\n", vector[i]);
	}*/

	nodArbAVL* rad2 = NULL;
	spargereArbore(&rad, &rad2, 3);
	
	printf("\nArborele 1:\n");
	inordine(rad);

	printf("\nArborele 2:\n");
	inordine(rad2);

	nodLD* prim = NULL;
	nodLD* ultim = NULL;

	arbore_ld(rad, &prim, &ultim);
	printf("\nLista dubla:\n");
	afisareLD(prim);

	printf("\nLista dubla invers:\n");
	afisareLDInvers(ultim);

	//vector cu prioritate 5 si numar ore mai mare mai mare de 8
	int nrElem = 0;
	Task* vector = vectorLD(prim, 5, 8, &nrElem);
	printf("\nVectorul:\n");
	for (int i = 0; i < nrElem; i++) {
		printf("Cod: %d, Nr. Ore: %d, Prioritate: %d, Proiect: %s, Titlu: %s\n",
			vector[i].cod, vector[i].nrOreAlocate, vector[i].prioritate,
			vector[i].proiect, vector[i].titlu);
	}

	dezalocareArbore(rad);
	dezalocareLD(prim);
	free(vector);

	return 0;
}