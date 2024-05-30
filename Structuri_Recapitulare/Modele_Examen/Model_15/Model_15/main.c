#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct {
	char* email;
	char* denumireAlbum;
	int nrFoto;
	float spatiuOcupat;
} Album;

typedef struct {
	Album inf;
	struct nodArbBin* stanga;
	struct nodArbBin* dreapta;
} nodArbBin;

typedef struct {
	Album* vector;
	int nrElem;
} heap;

nodArbBin* creareNod(nodArbBin* st, nodArbBin* dr, Album alb) {
	nodArbBin* nou = (nodArbBin*)malloc(sizeof(nodArbBin));
	nou->inf.nrFoto = alb.nrFoto;
	nou->inf.spatiuOcupat = alb.spatiuOcupat;
	nou->inf.denumireAlbum = (char*)malloc(strlen(alb.denumireAlbum) + 1);
	strcpy(nou->inf.denumireAlbum, alb.denumireAlbum);
	nou->inf.email = (char*)malloc(strlen(alb.email) + 1);
	strcpy(nou->inf.email, alb.email);

	nou->stanga = st;
	nou->dreapta = dr;

	return nou;
}

nodArbBin* inserareArbore(nodArbBin* rad, Album alb) {
	if (rad != NULL) {
		if (alb.nrFoto < rad->inf.nrFoto) {
			rad->stanga = inserareArbore(rad->stanga, alb);
			return rad;
		}

		else if (alb.nrFoto > rad->inf.nrFoto) {
			rad->dreapta = inserareArbore(rad->dreapta, alb);
			return rad;
		} 

		else {
			return rad;
		}
	}
	else {
		return creareNod(NULL, NULL, alb);
	}
}

void inordine(nodArbBin* rad) {
	if (rad != NULL) {
		inordine(rad->stanga);
		printf("Nr. Foto: %d, Spatiu: %5.2f, Email: %s, Denumire: %s\n",
			rad->inf.nrFoto, rad->inf.spatiuOcupat, rad->inf.email, rad->inf.denumireAlbum);
		inordine(rad->dreapta);
	}
}

void preordine(nodArbBin* rad) {
	if (rad != NULL) {
		printf("Nr. Foto: %d, Spatiu: %5.2f, Email: %s, Denumire: %s\n",
			rad->inf.nrFoto, rad->inf.spatiuOcupat, rad->inf.email, rad->inf.denumireAlbum);
		preordine(rad->stanga);
		preordine(rad->dreapta);
	}
}

int getNrFotoMin(nodArbBin* rad) {
	nodArbBin* temp = rad;
	while (temp->stanga != NULL)
	{
		temp = temp->stanga;
	}
	return temp->inf.nrFoto;
}

nodArbBin* findMin(nodArbBin* rad) {
	nodArbBin* temp = rad;
	while (temp->stanga != NULL) {
		temp = temp->stanga;
	}
	return temp;
}

nodArbBin* stergereNodCheie(nodArbBin* rad, int cheie) {
	if (rad == NULL) {
		return rad;
	}

	if (cheie < rad->inf.nrFoto) {
		rad->stanga = stergereNodCheie(rad->stanga, cheie);
	}

	else if (cheie > rad->inf.nrFoto) {
		rad->dreapta = stergereNodCheie(rad->dreapta, cheie);
	}

	else {
		if (rad->dreapta == NULL) {
			nodArbBin* temp = rad->stanga;
			free(rad->inf.denumireAlbum);
			free(rad->inf.email);
			free(rad);
			return temp;
		}

		if (rad->stanga == NULL) {
			nodArbBin* temp = rad->dreapta;
			free(rad->inf.denumireAlbum);
			free(rad->inf.email);
			free(rad);
			return temp;
		}

		nodArbBin* temp = findMin(rad->dreapta);
		rad->inf.nrFoto = temp->inf.nrFoto;
		rad->inf.spatiuOcupat = temp->inf.spatiuOcupat;
		free(rad->inf.denumireAlbum);
		rad->inf.denumireAlbum = (char*)malloc(strlen(temp->inf.denumireAlbum) + 1);
		strcpy(rad->inf.denumireAlbum, temp->inf.denumireAlbum);
		free(rad->inf.email);
		rad->inf.email = (char*)malloc(strlen(temp->inf.email) + 1);
		strcpy(rad->inf.email, temp->inf.email);

		rad->dreapta = stergereNodCheie(rad->dreapta, temp->inf.nrFoto);
	}
	return rad;
}

nodArbBin* stergereAlbumCuNrMinFoto(nodArbBin* rad) {
	int nrMinFoto = getNrFotoMin(rad);
	rad = stergereNodCheie(rad, nrMinFoto);
	return rad;
}

int counterNoduri(nodArbBin* rad, char* email) {
	int counter = 0;
	 
	if (rad != NULL) {
		if (strcmp(rad->inf.email, email) == 0) {
			counter++;
		}
		counter += counterNoduri(rad->stanga, email);
		counter += counterNoduri(rad->dreapta, email);
	}

	return counter;
}

void filtrare(heap h, int index) {
	int indexRad = index;
	int indexS = 2 * index + 1;
	int indexD = 2 * index + 2;

	if (indexS < h.nrElem && h.vector[indexS].spatiuOcupat > h.vector[indexRad].spatiuOcupat) {
		indexRad = indexS;
	}

	if (indexD < h.nrElem && h.vector[indexD].spatiuOcupat > h.vector[indexRad].spatiuOcupat) {
		indexRad = indexD;
	}

	if (index != indexRad) {
		Album aux = h.vector[index];
		h.vector[index] = h.vector[indexRad];
		h.vector[indexRad] = aux;

		filtrare(h, indexRad);
	}
}

void populareHeap(heap h, nodArbBin* rad, int* index, char* email) {
	if (rad != NULL) {
		if (strcmp(rad->inf.email, email) == 0) {
			h.vector[(*index)].nrFoto = rad->inf.nrFoto;
			h.vector[(*index)].spatiuOcupat = rad->inf.spatiuOcupat;
			h.vector[(*index)].denumireAlbum = (char*)malloc(strlen(rad->inf.denumireAlbum) + 1);
			strcpy(h.vector[(*index)].denumireAlbum, rad->inf.denumireAlbum);
			h.vector[(*index)].email = (char*)malloc(strlen(rad->inf.email) + 1);
			strcpy(h.vector[(*index)].email, rad->inf.email);
			(*index)++;
		}
		populareHeap(h, rad->stanga, index, email);
		populareHeap(h, rad->dreapta, index, email);
	}
}

heap creareHeap(nodArbBin* rad, char* email) {
	heap h;
	h.nrElem = counterNoduri(rad, email);
	h.vector = (Album*)malloc(h.nrElem * sizeof(Album));
	int index = 0;

	populareHeap(h, rad, &index, email);

	for (int i = (h.nrElem - 1) / 2; i >= 0; i--) {
		filtrare(h, i);
	}

	return h;
}

void afisareHeap(heap h) {
	for (int i = 0; i < h.nrElem; i++) {
		printf("Spatiu: %5.2f, Nr. Foto: %d, Email: %s, Denumire: %s\n",
			h.vector[i].spatiuOcupat, h.vector[i].nrFoto, h.vector[i].email, h.vector[i].denumireAlbum);
	}
}

heap modificareSpatiuAlbum(heap h, char* denumireAlbum, int noulSpatiu) {
	for (int i = 0; i < h.nrElem; i++) {
		if (strcmp(h.vector[i].denumireAlbum, denumireAlbum) == 0) {
			h.vector[i].spatiuOcupat = noulSpatiu;
		}
	}

	for (int i = (h.nrElem - 1) / 2; i >= 0; i--) {
		filtrare(h, i);
	}

	return h;
}

int main() {
	nodArbBin* rad = NULL;

	FILE* f = fopen("fisier.txt", "r");

	Album alb;
	char buffer[128];

	while (fscanf(f, "%s", buffer) == 1)
	{
		alb.email = (char*)malloc(strlen(buffer) + 1);
		strcpy(alb.email, buffer);

		fscanf(f, " %[^\n]", buffer);
		alb.denumireAlbum = (char*)malloc(strlen(buffer) + 1);
		strcpy(alb.denumireAlbum, buffer);

		fscanf(f, "%d", &alb.nrFoto);
		fscanf(f, "%f", &alb.spatiuOcupat);

		/*printf("Nr. Foto: %d, Spatiu: %5.2f, Email: %s, Denumire: %s\n",
			alb.nrFoto, alb.spatiuOcupat, alb.email, alb.denumireAlbum);*/
		rad = inserareArbore(rad, alb);

		free(alb.email);
		free(alb.denumireAlbum);
	}
	fclose(f);

	printf("Arborele in inordine:\n");
	inordine(rad);

	printf("\nArborele in preordine:\n");
	preordine(rad);

	//int nrMin = getNrFotoMin(rad);
	//printf("\n%d\n", nrMin);
	//rad = stergereNodCheie(rad, 366);

	rad = stergereAlbumCuNrMinFoto(rad);
	printf("\nArborele dupa stergere in INORDINE:\n");
	inordine(rad);

	/*int nrNoduri = counterNoduri(rad, "ionel@gmail.com");
	printf("\n%d\n", nrNoduri);*/

	heap h = creareHeap(rad, "ionel@gmail.com");
	printf("\nStructura HEAP:\n");
	afisareHeap(h);

	h = modificareSpatiuAlbum(h, "Ea", 999.89);
	printf("\nHEAP dupa modificare:\n");
	afisareHeap(h);

	return 0;
}