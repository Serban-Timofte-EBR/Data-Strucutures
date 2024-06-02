#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct {
	int cod;
	int zileScadenta;
	char* furnizor;
	char* clinet;
	float pret;
} Factura;

typedef struct {
	Factura* vector;
	int nrElem;
} heap;

typedef struct {
	Factura inf;
	struct nodArbBin* stanga;
	struct nodArbBin* dreapta;
} nodArbBin;

typedef struct {
	int cod;
	struct nodLS* next;
} nodLS;

void filtrare(heap h, int index) {
	int indexRad = index;
	int indexS = 2 * index + 1;
	int indexD = 2 * index + 2;

	if (indexS < h.nrElem && h.vector[indexS].zileScadenta < h.vector[indexRad].zileScadenta) {
		indexRad = indexS;
	}

	if (indexD < h.nrElem && h.vector[indexD].zileScadenta < h.vector[indexRad].zileScadenta) {
		indexRad = indexD;
	}

	if (index != indexRad) {
		Factura aux = h.vector[index];
		h.vector[index] = h.vector[indexRad];
		h.vector[indexRad] = aux;

		filtrare(h, indexRad);
	}
}

heap inserareHeap(heap h, Factura fac) {
	Factura* vectorNou = (Factura*)malloc((h.nrElem + 1) * sizeof(Factura));
	for (int i = 0; i < h.nrElem; i++) {
		vectorNou[i] = h.vector[i];
	}

	vectorNou[h.nrElem].cod = fac.cod;
	vectorNou[h.nrElem].pret = fac.pret;
	vectorNou[h.nrElem].zileScadenta = fac.zileScadenta;
	vectorNou[h.nrElem].clinet = (char*)malloc(strlen(fac.clinet) + 1);
	strcpy(vectorNou[h.nrElem].clinet, fac.clinet);
	vectorNou[h.nrElem].furnizor = (char*)malloc(strlen(fac.furnizor) + 1);
	strcpy(vectorNou[h.nrElem].furnizor, fac.furnizor);
	h.nrElem++;

	free(h.vector);
	h.vector = vectorNou;

	for (int i = (h.nrElem - 1) / 2; i >= 0; i--) {
		filtrare(h, i);
	}

	return h;
}

void afisareHeap(heap h) {
	for (int i = 0; i < h.nrElem; i++) {
		printf("Zile Scadenta: %d, Cod: %d, Pret: %5.2f, Client: %s, Furnizor: %s\n",
			h.vector[i].zileScadenta, h.vector[i].cod, h.vector[i].pret,
			h.vector[i].clinet, h.vector[i].furnizor);
	}
}

int countFacturiSub3ZileScadenta(heap h) {
	int counter = 0;
	for (int i = 0; i < h.nrElem; i++) {
		if (h.vector[i].zileScadenta < 3) {
			counter++;
		}
	}
	return counter;
}

heap stergereFactura(heap h) {
	Factura* vectorNou = (Factura*)malloc((h.nrElem - 1) * sizeof(Factura));

	Factura aux = h.vector[0];
	h.vector[0] = h.vector[h.nrElem - 1];
	h.nrElem--;

	for (int i = 0; i < h.nrElem; i++) {
		vectorNou[i] = h.vector[i];
	}

	free(h.vector);
	h.vector = vectorNou;

	free(aux.clinet);
	free(aux.furnizor);

	for (int i = (h.nrElem - 1) / 2; i >= 0; i--) {
		filtrare(h, i);
	}

	return h;
}

heap stergereFacturiSub3Zile(heap h) {
	int counter = countFacturiSub3ZileScadenta(h);
	for (int i = 0; i < counter; i++) {
		h = stergereFactura(h);
	}
	return h;
}

nodArbBin* inserareArbore(nodArbBin* rad, Factura inf) {
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
		nou->inf.pret = inf.pret;
		nou->inf.zileScadenta = inf.zileScadenta;
		nou->inf.clinet = (char*)malloc(strlen(inf.clinet) + 1);
		strcpy(nou->inf.clinet, inf.clinet);
		nou->inf.furnizor = (char*)malloc(strlen(inf.furnizor) + 1);
		strcpy(nou->inf.furnizor, inf.furnizor);
		nou->stanga = NULL;
		nou->dreapta = NULL;
		return nou;
	}
}

nodArbBin* heap_arbore(heap h) {
	nodArbBin* rad = NULL;
	for (int i = 0; i < h.nrElem; i++) {
		if (h.vector[i].pret > 100.0) {
			rad = inserareArbore(rad, h.vector[i]);
		}
	}
	return rad;
}

void inordine(nodArbBin* rad) {
	if (rad != NULL) {
		inordine(rad->stanga);
		printf("Cod: %d, Zile Scadenta: %d, Pret: %5.2f, Client: %s, Furnizor: %s\n",
			rad->inf.cod, rad->inf.zileScadenta, rad->inf.pret, rad->inf.clinet, rad->inf.furnizor);
		inordine(rad->dreapta);
	}
}

void inserareLS(nodLS** prim, int cod) {
	nodLS* nou = (nodLS*)malloc(sizeof(nodLS));
	nou->cod = cod;
	nou->next = NULL;

	if (*prim == NULL) {
		*prim = nou;
	}
	else {
		nodLS* temp = *prim;
		while (temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = nou;
	}
}

void dezalocareLS(nodLS* prim) {
	nodLS* temp = prim;
	while (temp != NULL) {
		nodLS* aux = temp->next;
		free(temp);
		temp = aux;
	}
}

void radToNod(nodArbBin* rad, nodLS** prim, int id) {
	if (rad == NULL) {
		dezalocareLS(*prim);
		*prim = NULL;
		return;
	}

	if (rad->inf.cod == id) {
		inserareLS(prim, id);
	}

	if (id < rad->inf.cod) {
		inserareLS(prim, rad->inf.cod);
		radToNod(rad->stanga, prim, id);
	}

	if (id > rad->inf.cod) {
		inserareLS(prim, rad->inf.cod);
		radToNod(rad->dreapta, prim, id);
	}
}

void printareLS(nodLS* prim) {
	nodLS* temp = prim;
	while (temp != NULL) {
		printf("%d ", temp->cod);
		temp = temp->next;
	}
}

void dezalocareHeap(heap h) {
	for (int i = 0; i < h.nrElem; i++) {
		free(h.vector[i].clinet);
		free(h.vector[i].furnizor);
	}
	free(h.vector);
}
 
void dezalocareArbore(nodArbBin* rad) {
	if (rad != NULL) {
		dezalocareArbore(rad->stanga);
		dezalocareArbore(rad->dreapta);
		free(rad->inf.clinet);
		free(rad->inf.furnizor);
		free(rad);
	}
}

heap stergeFacturaHeap(heap h, float prag) {
	for (int i = 0; i < h.nrElem; i++) {
		if (h.vector[i].pret < prag) {
			Factura* vectorNou = (Factura*)malloc((h.nrElem - 1) * sizeof(Factura));
			
			Factura aux = h.vector[i];
			h.vector[i] = h.vector[h.nrElem - 1];
			h.nrElem--;

			for (int i = 0; i < h.nrElem; i++) {
				vectorNou[i] = h.vector[i];
			}

			free(h.vector);
			h.vector = vectorNou;

			for (int i = (h.nrElem - 1) / 2; i >= 0; i--) {
				filtrare(h, i);
			}
		}
	}
	return h;
}

int main() {
	heap h;
	h.nrElem = 0;
	h.vector = (Factura*)malloc(sizeof(Factura));
	
	FILE* f = fopen("fisier.txt", "r");

	Factura fac;
	char buffer[128];

	while (fscanf(f, "%d", &fac.cod) == 1)
	{
		fscanf(f, "%d", &fac.zileScadenta);

		fscanf(f, " %[^\n]", buffer);
		fac.furnizor = (char*)malloc(strlen(buffer) + 1);
		strcpy(fac.furnizor, buffer);

		fscanf(f, " %[^\n]", buffer);
		fac.clinet = (char*)malloc(strlen(buffer) + 1);
		strcpy(fac.clinet, buffer);

		fscanf(f, "%f", &fac.pret);

		/*printf("Zile Scadenta: %d, Cod: %d, Pret: %5.2f, Client: %s, Furnizor: %s\n",
			fac.zileScadenta, fac.cod, fac.pret, fac.clinet, fac.furnizor);*/
		h = inserareHeap(h, fac);

		free(fac.clinet);
		free(fac.furnizor);
	}
	fclose(f);

	printf("HEAP:\n");
	afisareHeap(h);

	h = stergereFacturiSub3Zile(h);
	printf("\nHEAPUL dupa stergere:\n");
	afisareHeap(h);

	nodArbBin* rad = heap_arbore(h);

	printf("\nArborele:\n");
	inordine(rad);

	nodLS* prim = NULL;
	radToNod(rad, &prim, 78);
	printf("\nDrumul pana la nodul cerut:\n");
	printareLS(prim);

	//EXTRA: Sterge facturile sub 100 din HEAP
	h = stergeFacturaHeap(h, 100);
	printf("\n\nHEAPUL dupa stergere:\n");
	afisareHeap(h);

	dezalocareLS(prim);
	dezalocareHeap(h);
	dezalocareArbore(rad);

	return 0;
}