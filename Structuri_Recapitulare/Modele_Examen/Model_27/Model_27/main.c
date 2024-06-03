#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct {
	int cod;
	int durataZile;
	char* firmaExec;
	float pret;
} Proiect;

typedef struct {
	Proiect inf;
	struct nodArbAVL* stanga;
	struct nodArbAVL* dreapta;
} nodArbAVL;

typedef struct {
	Proiect inf;
	struct nodLC* next;
	struct nodLC* prev;
} nodLC;

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

int getDez(nodArbAVL* rad) {
	if (rad == NULL) {
		return 0;
	}

	return inaltime(rad->stanga) - inaltime(rad->dreapta);
}

void inserareAVL(nodArbAVL** rad, Proiect inf) {
	if (*rad != NULL) {
		if (inf.cod < (*rad)->inf.cod) {
			inserareAVL(&(*rad)->stanga, inf);
		}
		else if (inf.cod > (*rad)->inf.cod) {
			inserareAVL(&(*rad)->dreapta, inf);
		}
	}
	else {
		nodArbAVL* nou = (nodArbAVL*)malloc(sizeof(nodArbAVL));
		nou->inf.cod = inf.cod;
		nou->inf.durataZile = inf.durataZile;
		nou->inf.pret = inf.pret;
		nou->inf.firmaExec = (char*)malloc(strlen(inf.firmaExec) + 1);
		strcpy(nou->inf.firmaExec, inf.firmaExec);
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
		printf("Cod: %d, Durata: %d, Pret: %5.2f, Firma: %s\n",
			rad->inf.cod, rad->inf.durataZile, rad->inf.pret, rad->inf.firmaExec);
		inordine(rad->dreapta);
	}
}

int nrNoduriF(nodArbAVL* rad) {
	int nrNoduri = 0;
	
	if (rad == NULL) {
		return nrNoduri;
	}

	nrNoduri++;
	nrNoduri += nrNoduriF(rad->stanga);
	nrNoduri += nrNoduriF(rad->dreapta);

	return nrNoduri;
}

int getDurataTotala(nodArbAVL* rad) {
	int durata = 0;

	if (rad == NULL) {
		return durata;
	}

	durata += rad->inf.durataZile;
	durata += getDurataTotala(rad->stanga);
	durata += getDurataTotala(rad->dreapta);

	return durata;
}

float getDurataMedie(nodArbAVL* rad) {
	int nrProiecte = nrNoduriF(rad);
	int durataTotala = getDurataTotala(rad);
	return durataTotala / (float)nrProiecte;
}

int counterProiectePesteMedie(nodArbAVL* rad, float durataMedie) {
	int counter = 0;

	if (rad == NULL) {
		return counter;
	}

	if (rad->inf.durataZile > durataMedie) {
		counter++;
	}

	counter += counterProiectePesteMedie(rad->stanga, durataMedie);
	counter += counterProiectePesteMedie(rad->dreapta, durataMedie);

	return counter;
}

void inserareVector(nodArbAVL* rad, Proiect* vector, int* index, float medie) {
	if (rad != NULL) {
		if (rad->inf.durataZile > medie) {
			vector[*index] = rad->inf;
			(*index)++;
		}
		inserareVector(rad->stanga, vector, index, medie);
		inserareVector(rad->dreapta, vector, index, medie);
	}
}

Proiect* proiectePesteMedie(nodArbAVL* rad, int* nrElem) {
	float durataMedie = getDurataMedie(rad);
	*nrElem = counterProiectePesteMedie(rad, durataMedie);
	Proiect* vector = (Proiect*)malloc(*nrElem * sizeof(Proiect));
	int index = 0;

	inserareVector(rad, vector, &index, durataMedie);

	return vector;
}

nodArbAVL* findMin(nodArbAVL* rad) {
	nodArbAVL* temp = rad;
	while (temp->stanga != NULL) {
		temp = temp->stanga;
	}
	return temp;
}

nodArbAVL* stergereAVL(nodArbAVL* rad, int cod) {
	if (rad == NULL) {
		return rad;
	}

	if (cod < rad->inf.cod) {
		rad->stanga = stergereAVL(rad->stanga, cod);
	}
	else if (cod > rad->inf.cod) {
		rad->dreapta = stergereAVL(rad->dreapta, cod);
	}
	else {
		if (rad->stanga == NULL) {
			nodArbAVL* temp = rad->dreapta;
			free(rad->inf.firmaExec);
			free(rad);
			return temp;
		}

		if (rad->dreapta == NULL) {
			nodArbAVL* temp = rad->stanga;
			free(rad->inf.firmaExec);
			free(rad);
			return temp;
		}

		nodArbAVL* temp = findMin(rad->dreapta);
		rad->inf.cod = temp->inf.cod;
		rad->inf.durataZile = temp->inf.durataZile;
		rad->inf.pret = temp->inf.pret;
		free(rad->inf.firmaExec);
		rad->inf.firmaExec = (char*)malloc(strlen(temp->inf.firmaExec) + 1);
		strcpy(rad->inf.firmaExec, temp->inf.firmaExec);
		rad->dreapta = stergereAVL(rad->dreapta, temp->inf.cod);
	}
	return rad;
}

nodArbAVL* stergereAVLReechilibrare(nodArbAVL* rad, int cod) {
	rad = stergereAVL(rad, cod);

	int dez = getDez(rad);

	if (dez == -2) {
		if (getDez(rad->dreapta) == 1) {
			rotire_dreapta(&rad->dreapta);
		}
		rotire_stanga(&rad);
	}

	if (dez == 2) {
		if (getDez(rad->stanga) == -1) {
			rotire_stanga(&rad->stanga);
		}
		rotire_dreapta(&rad);
	}

	return rad;
}

int counterNoduri(nodArbAVL* rad) {
	int counter = 0;

	if (rad == NULL) {
		return counter;
	}

	counter++;

	counter += counterNoduri(rad->stanga);
	counter += counterNoduri(rad->dreapta);

	return counter;
}

void inserareVectorNoduri(nodArbAVL* rad, Proiect** vector, int* index) {
	if (rad != NULL) {
		vector[*index] = &rad->inf;
		(*index)++;

		inserareVectorNoduri(rad->stanga, vector, index);
		inserareVectorNoduri(rad->dreapta, vector, index);
	}
}

Proiect** vectorNoduriAVL(nodArbAVL* rad, int* nrNoduri) {
	*nrNoduri = counterNoduri(rad);
	Proiect** vector = (Proiect**)malloc(*nrNoduri * sizeof(Proiect));
	for (int i = 0; i < *nrNoduri; i++) {
		vector[i] = NULL;
	}
	int index = 0;

	inserareVectorNoduri(rad, vector, &index);

	return vector;
}

void inserareLC(nodLC** prim, Proiect inf) {
	nodLC* nou = (nodLC*)malloc(sizeof(nodLC));
	nou->inf.cod = inf.cod;
	nou->inf.durataZile = inf.durataZile;
	nou->inf.pret = inf.pret;
	nou->inf.firmaExec = (char*)malloc(strlen(inf.firmaExec) + 1);
	strcpy(nou->inf.firmaExec, inf.firmaExec);

	if (*prim == NULL) {
		nou->next = nou;
		nou->prev = nou;
		*prim = nou;
	}
	else {
		nodLC* temp = *prim;
		while (temp->next != *prim) {
			temp = temp->next;
		}
		temp->next = nou;
		nou->next = *prim;
		(*prim)->prev = nou;
		nou->prev = temp;
	}
}

void arbore_lc(nodArbAVL* rad, nodLC** prim) {
	if (rad == NULL) {
		return;
	}

	inserareLC(prim, rad->inf);

	arbore_lc(rad->stanga, prim);
	arbore_lc(rad->dreapta, prim);
}

void afisareLC(nodLC* prim) {
	nodLC* temp = prim;
	do {
		printf("Cod: %d, Durata: %d, Pret: %5.2f, Firma: %s\n",
			temp->inf.cod, temp->inf.durataZile, temp->inf.pret, temp->inf.firmaExec);
		temp = temp->next;
	} while (temp != prim);
}

void afisareLCInvers(nodLC* prim) {
	nodLC* temp = prim;
	do {
		printf("Cod: %d, Durata: %d, Pret: %5.2f, Firma: %s\n",
			temp->inf.cod, temp->inf.durataZile, temp->inf.pret, temp->inf.firmaExec);
		temp = temp->prev;
	} while (temp != prim);
}

void inserareLCOrdonat(nodLC** prim, Proiect inf) {
	nodLC* nou = (nodLC*)malloc(sizeof(nodLC));
	nou->inf.cod = inf.cod;
	nou->inf.durataZile = inf.durataZile;
	nou->inf.pret = inf.pret;
	nou->inf.firmaExec = (char*)malloc(strlen(inf.firmaExec) + 1);
	strcpy(nou->inf.firmaExec, inf.firmaExec);

	if (*prim == NULL) {
		nou->next = nou;
		nou->prev = nou;
		*prim = nou;
	}
	else {
		nodLC* temp = prim;
		nodLC* prev = NULL;
		do {
			if (temp->inf.durataZile > nou->inf.durataZile) {
				break;
			}
			prev = temp;
			temp = temp->next;
		} while (temp != prim);
		
		if (prev == NULL) {
			nodLC* ant = (*prim)->prev;
			nou->prev = ant;
			nou->next = *prim;
			ant->next = nou;
			(*prim)->prev = nou;
			*prim = nou;
		}

		else if (temp == *prim && temp->inf.durataZile >= nou->inf.durataZile) {
			nodLC* ant = temp->prev;
			ant->next = nou;
			nou->prev = ant;
			nou->next = *prim;
			(*prim)->prev = nou;
			//*prim = nou;
		}
		else {
			nodLC* ant = temp->prev;
			nodLC* urm = temp->next;
			ant->next = nou;
			nou->prev = ant;
			nou->next = urm;
			urm->prev = nou;
		}
	}
}

void dezalocareLC(nodLC* prim) {
	if (prim != NULL) {
		nodLC* temp = prim;
		do {
			nodLC* aux = temp->next;
			free(temp->inf.firmaExec);
			free(temp);
			temp = aux;
		} while (temp != prim);
	}
}

nodLC* ordonareLC(nodLC* prim) {
	nodLC* primOrd = NULL;
	nodLC* temp = prim;
	do {
		inserareLCOrdonat(&primOrd, temp->inf);
		temp = temp->next;
	} while (temp != prim);

	dezalocareLC(prim);

	return primOrd;
}

void dezalocareArbore(nodArbAVL* rad) {
	if (rad != NULL) {
		dezalocareArbore(rad->stanga);
		dezalocareArbore(rad->dreapta);
		free(rad->inf.firmaExec);
		free(rad);
	}
}

void dezalocareVector1(Proiect* vector) {
	free(vector);
}

void dezalocareVector2(Proiect** vector, int nrElem) {
	free(vector);
}

int main() {
	nodArbAVL* rad = NULL;

	FILE* f = fopen("fisier.txt", "r");

	Proiect pr;
	char buffer[128];

	while (fscanf(f, "%d", &pr.cod) == 1)
	{
		fscanf(f, "%d", &pr.durataZile);

		fscanf(f, " %[^\n]", buffer);
		pr.firmaExec = (char*)malloc(strlen(buffer) + 1);
		strcpy(pr.firmaExec, buffer);

		fscanf(f, "%f", &pr.pret);

		/*printf("Cod: %d, Durata: %d, Pret: %5.2f, Firma: %s\n", 
			pr.cod, pr.durataZile, pr.pret, pr.firmaExec);*/
		inserareAVL(&rad, pr);

		free(pr.firmaExec);
	}
	fclose(f);

	printf("Arborele din fisier:\n");
	inordine(rad);

	//float medieDurata = getDurataMedie(rad);
	//printf("\n%5.2f\n", medieDurata);

	//int nrElem = counterProiectePesteMedie(rad, medieDurata);
	//printf("\n%d\n", nrElem);

	int nrPr = 0;
	Proiect* vectorPrPesteMedie = proiectePesteMedie(rad, &nrPr);
	printf("\nProiecte peste medie:\n");
	for (int i = 0; i < nrPr; i++) {
		printf("Cod: %d, Durata: %d, Pret: %5.2f, Firma: %s\n",
			vectorPrPesteMedie[i].cod, vectorPrPesteMedie[i].durataZile, 
			vectorPrPesteMedie[i].pret, vectorPrPesteMedie[i].firmaExec);
	}

	rad = stergereAVLReechilibrare(rad, 45);
	printf("\nArborele dupa stergere:\n");
	inordine(rad);

	int nrNoduri = 0;
	Proiect** vector = vectorNoduriAVL(rad, &nrNoduri);
	printf("\nVectorul cu nodurile din arbore:\n");
	for (int i = 0; i < nrNoduri; i++) {
		printf("Cod: %d, Durata: %d, Pret: %5.2f, Firma: %s\n",
			vector[i]->cod, vector[i]->durataZile, vector[i]->pret, vector[i]->firmaExec);
	}

	nodLC* prim = NULL;
	arbore_lc(rad, &prim);
	printf("\nLista circulara:\n");
	afisareLC(prim);
	printf("\nLista circulara invers:\n");
	afisareLCInvers(prim);

	prim = ordonareLC(prim);
	printf("\nLista circulara ordonata:\n");
	afisareLC(prim);
	printf("\nLista circulara invers:\n");
	afisareLCInvers(prim);

	dezalocareArbore(rad);
	dezalocareLC(prim);
	dezalocareVector1(vectorPrPesteMedie);
	dezalocareVector2(vector, nrNoduri);

	return 0;
}