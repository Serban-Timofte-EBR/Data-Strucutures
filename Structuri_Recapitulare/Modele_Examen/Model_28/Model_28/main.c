#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct {
	int cod;
	float salariul;
	char* nume;
	char* pozitie;
	float ultimulBonus;
} Angajat;

typedef struct {
	Angajat inf;
	struct nodLD* next;
	struct nodLD* prev;
} nodLD;

typedef struct {
	Angajat inf;
	struct nodArbBin* stanga;
	struct nodArbBin* dreapta;
} nodArbBin;

void inserareLD(nodLD** prim, nodLD** ultim, Angajat ang) {
	nodLD* nou = (nodLD*)malloc(sizeof(nodLD));
	nou->inf.cod = ang.cod;
	nou->inf.salariul = ang.salariul;
	nou->inf.ultimulBonus = ang.ultimulBonus;
	nou->inf.pozitie = (char*)malloc(strlen(ang.pozitie) + 1);
	strcpy(nou->inf.pozitie, ang.pozitie);
	nou->inf.nume = (char*)malloc(strlen(ang.nume) + 1);
	strcpy(nou->inf.nume, ang.nume);
	nou->next = NULL;
	nou->prev = NULL;

	if (*prim == NULL) {
		*prim = nou;
		*ultim = nou;
	}
	else {
		nou->prev = *ultim;
		(*ultim)->next = nou;
		*ultim = nou;
	}
}

void inserareLDOrdonat(nodLD** prim, nodLD** ultim, Angajat ang) {
	nodLD* nou = (nodLD*)malloc(sizeof(nodLD));
	nou->inf.cod = ang.cod;
	nou->inf.salariul = ang.salariul;
	nou->inf.ultimulBonus = ang.ultimulBonus;
	nou->inf.pozitie = (char*)malloc(strlen(ang.pozitie) + 1);
	strcpy(nou->inf.pozitie, ang.pozitie);
	nou->inf.nume = (char*)malloc(strlen(ang.nume) + 1);
	strcpy(nou->inf.nume, ang.nume);
	nou->next = NULL;
	nou->prev = NULL;

	if (*prim == NULL) {
		*prim = nou;
		*ultim = nou;
	}
	else {
		nodLD* temp = *prim;
		while (temp != NULL && temp->inf.salariul < ang.salariul) {
			temp = temp->next;
		}

		if (temp == *prim) {
			nou->next = *prim;
			(*prim)->prev = nou;
			*prim = nou;
		}
		else if (temp == NULL) {
			nou->prev = *ultim;
			(*ultim)->next = nou;
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

void afisareLD(nodLD* prim) {
	nodLD* temp = prim;
	while (temp != NULL) {
		printf("Cod: %d, Salariul: %5.2f, Bonus: %5.2f, Pozitie: %s, Nume: %s\n",
			temp->inf.cod, temp->inf.salariul, temp->inf.ultimulBonus,
			temp->inf.pozitie, temp->inf.nume);
		temp = temp->next;
	}
}

void afisareLDInvers(nodLD* ultim) {
	nodLD* temp = ultim;
	while (temp != NULL) {
		printf("Cod: %d, Salariul: %5.2f, Bonus: %5.2f, Pozitie: %s, Nume: %s\n",
			temp->inf.cod, temp->inf.salariul, temp->inf.ultimulBonus,
			temp->inf.pozitie, temp->inf.nume);
		temp = temp->prev;
	}
}

void stergereAngSalPrag(nodLD** prim, nodLD** ultim, float pragSal) {
	nodLD* temp = *prim;
	while (temp != NULL) {
		if (temp->inf.salariul > pragSal) {
			nodLD* deleMe = temp;
			if (temp == *prim) {
				*prim = (*prim)->next;
				(*prim)->prev = NULL;
			}

			else if (temp == *ultim) {
				*ultim = (*ultim)->prev;
				(*ultim)->next = NULL;
			}

			else {
				nodLD* ant = temp->prev;
				nodLD* urm = temp->next;
				ant->next = urm;
				urm->prev = ant;
			}
			temp = temp->next;
			free(deleMe->inf.nume);
			free(deleMe->inf.pozitie);
			free(deleMe);
		}
		else {
			temp = temp->next;
		}
	}
}

nodArbBin* inserareArbore(nodArbBin* rad, Angajat inf) {
	if (rad != NULL) {
		if (inf.cod < rad->inf.cod) {
			rad->stanga = inserareArbore(rad->stanga, inf);
			return rad;
		}
		else if (inf.cod > rad->inf.cod) {
			rad->dreapta = inserareArbore(rad->dreapta, inf);
			return rad;
		}
		else {
			return rad;
		}
	}
	else {
		nodArbBin* nou = (nodArbBin*)malloc(sizeof(nodArbBin));
		nou->inf.cod = inf.cod;
		nou->inf.salariul = inf.salariul;
		nou->inf.ultimulBonus = inf.ultimulBonus;
		nou->inf.nume = (char*)malloc(strlen(inf.nume) + 1);
		strcpy(nou->inf.nume, inf.nume);
		nou->inf.pozitie = (char*)malloc(strlen(inf.pozitie) + 1);
		strcpy(nou->inf.pozitie, inf.pozitie);
		nou->stanga = NULL;
		nou->dreapta = NULL;
		return nou;
	}
}

nodArbBin* lista_arbore(nodLD* prim) {
	nodArbBin* rad = NULL;

	nodLD* temp = prim;
	while (temp != NULL) {
		rad = inserareArbore(rad, temp->inf);
		temp = temp->next;
	}

	return rad;
}

void inordine(nodArbBin* rad) {
	if (rad != NULL) {
		inordine(rad->stanga);
		printf("Cod: %d, Salariul: %5.2f, Bonus: %5.2f, Pozitie: %s, Nume: %s\n",
			rad->inf.cod, rad->inf.salariul, rad->inf.ultimulBonus, rad->inf.pozitie, rad->inf.nume);
		inordine(rad->dreapta);
	}
}

void nrNoduriTraseu(nodArbBin* rad, int idNod, int* counter) {
	if (rad == NULL) {
		return;
	}

	(*counter)++;

	if (rad->inf.cod == idNod) {
		return;
	}

	if (idNod < rad->inf.cod) {
		nrNoduriTraseu(rad->stanga, idNod, counter);
	}

	else if (idNod > rad->inf.cod) {
		nrNoduriTraseu(rad->dreapta, idNod, counter);
	}
}

void salvareVector(nodArbBin* rad, int idNod, int* vector, int* index) {
	if (rad == NULL) {
		return;
	}

	vector[*index] = rad->inf.cod;
	(*index)++;

	if (idNod < rad->inf.cod) {
		salvareVector(rad->stanga, idNod, vector, index);
	}

	else if (idNod > rad->inf.cod) {
		salvareVector(rad->dreapta, idNod, vector, index);
	}

}

int* traseuNod(nodArbBin* rad, int idNod, int* nrElem) {
	nrNoduriTraseu(rad, idNod, nrElem);
	int* vector = (int*)malloc(*nrElem * sizeof(int));
	int index = 0;
	salvareVector(rad, idNod, vector, &index);
	return vector;
}

int main() {
	nodLD* prim = NULL;
	nodLD* ultim = NULL;

	nodLD* primOrd = NULL;
	nodLD* ultimOrd = NULL;

	FILE* f = fopen("fisier.txt", "r");

	Angajat ang;
	char buffer[128];

	while (fscanf(f, "%d", &ang.cod) == 1)
	{
		fscanf(f, "%f", &ang.salariul);

		fscanf(f, " %[^\n]", buffer);
		ang.nume = (char*)malloc(strlen(buffer) + 1);
		strcpy(ang.nume, buffer);

		fscanf(f, " %[^\n]", buffer);
		ang.pozitie = (char*)malloc(strlen(buffer) + 1);
		strcpy(ang.pozitie, buffer);

		fscanf(f, "%f", &ang.ultimulBonus);

		/*printf("Cod: %d, Salariul: %5.2f, Bonus: %5.2f, Pozitie: %s, Nume: %s\n",
			ang.cod, ang.salariul, ang.ultimulBonus, ang.pozitie, ang.nume);*/
		inserareLD(&prim, &ultim, ang);
		inserareLDOrdonat(&primOrd, &ultimOrd, ang);

		free(ang.nume);
		free(ang.pozitie);
	}
	fclose(f);

	printf("Lista dubla ordonata din fisier:\n");
	afisareLD(primOrd);

	printf("\nLista dubla ordonata din fisier invers:\n");
	afisareLDInvers(ultimOrd);

	printf("\nLista dubla din fisier:\n");
	afisareLD(prim);

	printf("\nLista dubla din fisier invers:\n");
	afisareLDInvers(ultim);

	stergereAngSalPrag(&prim, &ultim, 10000);
	printf("\nLista dupa stergere:\n");
	afisareLD(prim);
	printf("\nLista dupa stergere invers:\n");
	afisareLDInvers(ultim);

	nodArbBin* rad = lista_arbore(prim);
	printf("\nArborele:\n");
	inordine(rad);

	int nrElem = 0;
	//nrNoduriTraseu(rad, 67, &nrElem);
	//printf("\n%d\n", nrElem);

	int* vector = traseuNod(rad, 67, &nrElem);
	printf("\nTraseul pana la nodul cerut este: ");
	for (int i = 0; i < nrElem; i++) {
		printf("%d ", vector[i]);
	}
	printf("\n");

	return 0;
}