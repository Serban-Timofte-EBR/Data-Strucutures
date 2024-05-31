#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct {
	int nrFactura;
	char* dataEmiterii;
	char* denumireClient;
	float valoare;
	int numarProduse;
} Factura;

typedef struct {
	Factura inf;
	struct nodArbAVL* stanga;
	struct nodArbAVL* dreapta;
} nodArbAVL;

typedef struct {
	Factura inf;
	int nivel;
	struct nodCoada* next;
} nodCoada;

void put(nodCoada** prim, nodCoada** ultim, nodArbAVL* nod, int nivel) {
	nodCoada* nou = (nodCoada*)malloc(sizeof(nodCoada));
	nou->inf = nod->inf;
	nou->nivel = nivel;
	nou->next = NULL;

	if (*prim == NULL) {
		*prim = nou;
		*ultim = nou;
	}
	else {
		(*ultim)->next = nou;
		*ultim = nou;
	}
}

nodCoada* get(nodCoada** prim, nodCoada** ultim) {
	if (*prim == NULL) {
		return NULL;
	}
	nodCoada* temp = *prim;
	*prim = (*prim)->next;
	if (*prim == NULL) {
		*ultim = NULL;
	}
	return temp;
}

void inserareArbore(nodArbAVL** rad, Factura fac) {
	if (*rad != NULL) {
		if (fac.nrFactura < (*rad)->inf.nrFactura) {
			inserareArbore(&(*rad)->stanga, fac);
		}

		else if (fac.nrFactura > (*rad)->inf.nrFactura) {
			inserareArbore(&(*rad)->dreapta, fac);
		}
	}
	else {
		nodArbAVL* nou = (nodArbAVL*)malloc(sizeof(nodArbAVL));
		nou->inf.nrFactura = fac.nrFactura;
		nou->inf.valoare = fac.valoare;
		nou->inf.numarProduse = fac.numarProduse;
		nou->inf.dataEmiterii = (char*)malloc(strlen(fac.dataEmiterii) + 1);
		strcpy(nou->inf.dataEmiterii, fac.dataEmiterii);
		nou->inf.denumireClient = (char*)malloc(strlen(fac.denumireClient) + 1);
		strcpy(nou->inf.denumireClient, fac.denumireClient);
		nou->stanga = NULL;
		nou->dreapta = NULL;
		*rad = nou;
	}
}

void rotire_stanga(nodArbAVL** rad) {
	nodArbAVL* aux = (*rad)->dreapta;
	(*rad)->dreapta = aux->stanga;
	aux->stanga = *rad;
	*rad = aux;
}

void rotire_dreapta(nodArbAVL** rad) {
	nodArbAVL* aux = (*rad)->stanga;
	(*rad)->stanga = aux->dreapta;
	aux->dreapta = *rad;
	*rad = aux;
}

int maxim(int a, int b) {
	int max = a;
	if (b > a) {
		max = b;
	}
	return b;
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

void inserareAVL(nodArbAVL** rad, Factura fac) {
	if (*rad != NULL) {
		if (fac.nrFactura < (*rad)->inf.nrFactura) {
			inserareArbore(&(*rad)->stanga, fac);
		}
		else if (fac.nrFactura > (*rad)->inf.nrFactura) {
			inserareArbore(&(*rad)->dreapta, fac);
		}
	}
	else {
		nodArbAVL* nou = (nodArbAVL*)malloc(sizeof(nodArbAVL));
		nou->inf.nrFactura = fac.nrFactura;
		nou->inf.valoare = fac.valoare;
		nou->inf.numarProduse = fac.numarProduse;
		nou->inf.dataEmiterii = (char*)malloc(strlen(fac.dataEmiterii) + 1);
		strcpy(nou->inf.dataEmiterii, fac.dataEmiterii);
		nou->inf.denumireClient = (char*)malloc(strlen(fac.denumireClient) + 1);
		strcpy(nou->inf.denumireClient, fac.denumireClient);
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
		printf("Nr. Factura: %d, Nr. Produse: %d, Suma: %5.2f, Data: %s, Client: %s\n",
			rad->inf.nrFactura, rad->inf.numarProduse, rad->inf.valoare, rad->inf.dataEmiterii, rad->inf.denumireClient);
		inordine(rad->dreapta);
	}
}

float valoareFactura(nodArbAVL* rad, char* data) {
	float val = 0.0;

	if (rad == NULL) {
		return 0.0;
	}

	if (strcmp(rad->inf.dataEmiterii, data) == 0) {
		val += rad->inf.valoare;
	}

	val += valoareFactura(rad->stanga, data);
	val += valoareFactura(rad->dreapta, data);

	return val;
}

int counterNrFacturi(nodArbAVL* rad, char* client) {
	int counter = 0;
	if (rad != NULL) {
		if (strcmp(rad->inf.denumireClient, client) == 0) {
			counter++;
		}
		counter += counterNrFacturi(rad->stanga, client);
		counter += counterNrFacturi(rad->dreapta, client);
	}
	return counter;
}

void modificareValoare(nodArbAVL** rad) {
	if (*rad != NULL) {
		(*rad)->inf.valoare += 2.5;

		modificareValoare(&(*rad)->stanga);
		modificareValoare(&(*rad)->dreapta);
	}
}

nodArbAVL* findMin(nodArbAVL* rad) {
	nodArbAVL* temp = rad;
	while (temp->stanga != NULL)
	{
		temp = rad->stanga;
	}
	return temp;
}

nodArbAVL* stergereNod(nodArbAVL* rad, int nrFac) {
	if (rad == NULL) {
		return rad;
	}

	if (nrFac < rad->inf.nrFactura) {
		rad->stanga = stergereNod(rad->stanga, nrFac);
	}
	else if (nrFac > rad->inf.nrFactura) {
		rad->dreapta = stergereNod(rad->dreapta, nrFac);
	}
	else {
		if (rad->dreapta == NULL) {
			nodArbAVL* temp = rad->stanga;
			free(rad->inf.denumireClient);
			free(rad->inf.dataEmiterii);
			free(rad);
			return temp;
		}

		else if (rad->stanga == NULL) {
			nodArbAVL* temp = rad->dreapta;
			free(rad->inf.denumireClient);
			free(rad->inf.dataEmiterii);
			free(rad);
			return temp;
		}

		nodArbAVL* temp = findMin(rad->dreapta);
		rad->inf.nrFactura = temp->inf.nrFactura;
		rad->inf.valoare = temp->inf.valoare;
		rad->inf.numarProduse = temp->inf.numarProduse;
		free(rad->inf.dataEmiterii);
		rad->inf.dataEmiterii = (char*)malloc(strlen(temp->inf.dataEmiterii) + 1);
		strcpy(rad->inf.dataEmiterii, temp->inf.dataEmiterii);
		free(rad->inf.denumireClient);
		rad->inf.denumireClient = (char*)malloc(strlen(rad->inf.denumireClient) + 1);
		strcpy(rad->inf.denumireClient, temp->inf.denumireClient);

		rad->dreapta = stergereNod(rad->dreapta, temp->inf.nrFactura);
	}
	return rad;
}

void calculNoduriNivel(nodArbAVL* rad, int nivel, int* vector) {
	if (rad != NULL) {
		vector[nivel]++;
		calculNoduriNivel(rad->stanga, nivel + 1, vector);
		calculNoduriNivel(rad->dreapta, nivel + 1, vector);
	}
}

int* vectorCuNrNoduriNivel(nodArbAVL* rad) {
	int nrElem = inaltime(rad);
	int* vector = (int*)malloc(nrElem * sizeof(int));

	for (int i = 0; i < nrElem; i++) {
		vector[i] = 0;
	}

	calculNoduriNivel(rad, 0, vector);
	return vector;
}

int main() {
	nodArbAVL* rad = NULL;

	FILE* f = fopen("fisier.txt", "r");

	Factura fac;
	char buffer[128];

	while (fscanf(f, "%d", &fac.nrFactura) == 1)
	{
		fscanf(f, "%s", buffer);
		fac.dataEmiterii = (char*)malloc(strlen(buffer) + 1);
		strcpy(fac.dataEmiterii, buffer);

		fscanf(f, " %[^\n]", buffer);
		fac.denumireClient = (char*)malloc(strlen(buffer) + 1);
		strcpy(fac.denumireClient, buffer);

		fscanf(f, "%f", &fac.valoare);
		fscanf(f, "%d", &fac.numarProduse);

		/*printf("Nr. Factura: %d, Nr. Produse: %d, Suma: %5.2f, Data: %s, Client: %s\n",
			fac.nrFactura, fac.numarProduse, fac.valoare, fac.dataEmiterii, fac.denumireClient);*/
		inserareAVL(&rad, fac);

		free(fac.denumireClient);
		free(fac.dataEmiterii);
	}
	fclose(f);

	printf("Arborele AVL din fisier in INORDINE:\n");
	inordine(rad);

	printf("\nArborele AVL pe niveluri:\n");
	int niv = 0;

	char* dataCautata = "21.10.2023";
	float valFacturiData = valoareFactura(rad, dataCautata);
	printf("\nValoarea facturilor din data ceruta este: %5.2f\n", valFacturiData);

	char* clientCautat = "Ionel Popescu";
	int nrFacturi = counterNrFacturi(rad, clientCautat);
	printf("\nClientul cautata are %d facturi\n", nrFacturi);

	modificareValoare(&rad);
	printf("\nArborele AVL dupa modificare:\n");
	inordine(rad);

	rad = stergereNod(rad, 54);
	rad = stergereNod(rad, 100);
	rad = stergereNod(rad, 10);
	printf("\nArborele AVL dupa stergere:\n");
	inordine(rad);

	/*int* vector = vectorCuNrNoduriNivel(rad);
	printf("\nVectorul cu nr. de noduri per nivel:");
	for (int i = 0; i < inaltime(rad); i++) {
		printf(" %d ", vector[i]);
	}*/

	

	return 0;
}