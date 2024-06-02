#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct {
    int id;
    char* titlu;
    char* autor;
    int anPublicare;
} Carte;

typedef struct {
	Carte info;
	struct nodArbOarecare* copil;
	struct nodArbOarecare* frate;
} nodArbOarecare;

nodArbOarecare* creareNod(Carte c) {
	nodArbOarecare* nodNou = (nodArbOarecare*)malloc(sizeof(nodArbOarecare));
	nodNou->info.id = c.id;
	nodNou->info.titlu = (char*)malloc(strlen(c.titlu) + 1);
	strcpy(nodNou->info.titlu, c.titlu);
	nodNou->info.autor = (char*)malloc(strlen(c.autor) + 1);
	strcpy(nodNou->info.autor, c.autor);
	nodNou->info.anPublicare = c.anPublicare;
	nodNou->copil = NULL;
	nodNou->frate = NULL;
	return nodNou;
}

void afisarePreordine(nodArbOarecare* radacina) {
	if (radacina != NULL) {
		printf("ID: %d, Titlu: %s, Autor: %s, An: %d\n",
			radacina->info.id, radacina->info.titlu,
			radacina->info.autor, radacina->info.anPublicare);
		afisarePreordine(radacina->copil);
		afisarePreordine(radacina->frate);
	}
}

void inserareNod(nodArbOarecare** rad, Carte info) {
	nodArbOarecare* nou = creareNod(info);

	if (*rad == NULL) {
		*rad = nou;
	}
	else {
		nodArbOarecare* temp = *rad;
		while (temp->frate != NULL) {
			temp = temp->frate;
		}
		temp->frate = nou;
	}
}

void inserareSubID(nodArbOarecare** rad, Carte inf, int idPar) {
	nodArbOarecare* nou = creareNod(inf);
	if (*rad == NULL) {
		*rad = nou;
	}
	else {
		nodArbOarecare* temp = *rad;
		nodArbOarecare* parinte = NULL;

		while (temp != NULL)
		{
			if (temp->info.id == idPar) {
				parinte = temp;
				break;
			}

			if (temp->copil != NULL) {
				temp = temp->copil;
			}

			else {
				temp = temp->frate;
			}
		}

		if (parinte != NULL) {
			if (parinte->copil == NULL) {
				parinte->copil = nou;
			}
			else {
				nodArbOarecare* copil = parinte->copil;
				while (copil->frate != NULL)
				{
					copil = copil->frate;
				}
				copil->frate = nou;
			}
		}
	}
}

int main() {
	nodArbOarecare* rad = NULL;

	FILE* f = fopen("fisier.txt", "r");

	Carte car;
	char buffer[128];

	while (fscanf(f, "%d", &car.id) == 1)
	{
		fscanf(f, " %[^\n]", buffer);
		car.titlu = (char*)malloc(strlen(buffer) + 1);
		strcpy(car.titlu, buffer);

		fscanf(f, " %[^\n]", buffer);
		car.autor = (char*)malloc(strlen(buffer) + 1);
		strcpy(car.autor, buffer);

		fscanf(f, "%d", &car.anPublicare);

		/*printf("ID: %d, An: %d, Autor: %s, Titlu: %s\n",
			car.id, car.anPublicare, car.autor, car.titlu);*/
		inserareNod(&rad, car);

		free(car.autor);
		free(car.titlu);
	}
	fclose(f);

	printf("Arborele oarecare:\n");
	afisarePreordine(rad);

	return 0;
}