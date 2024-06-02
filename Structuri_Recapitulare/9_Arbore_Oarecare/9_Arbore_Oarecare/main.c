#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct {
	int info;
	struct nod* primulCopil;
	struct nod* urmatorulFrate;
} nod;

nod* creareNod(int info) {
	nod* nou = (nod*)malloc(sizeof(nod));
	nou->info = info;
	nou->primulCopil = NULL;
	nou->urmatorulFrate = NULL;
	return nou;
}

void adaugareCopil(nod** parinte, int info) {
	nod* nou = creareNod(info);
	if ((*parinte)->primulCopil == NULL) {
		(*parinte)->primulCopil = nou;
	}
	else {
		nod* copilCurent = (*parinte)->primulCopil;
		while (copilCurent->urmatorulFrate != NULL)
		{
			copilCurent = copilCurent->urmatorulFrate;
		}
		copilCurent->urmatorulFrate = nou;
	}
}

nod* stergereNod(nod* rad, int id) {
	if (rad == NULL) {
		return rad;
	}

	if (rad->info == id) {
		nod* temp = rad->primulCopil;
		free(rad);
		return temp;
	}
	else {
		rad->primulCopil = stergereNod(rad->primulCopil, id);
		rad->urmatorulFrate = stergereNod(rad->urmatorulFrate, id);
	}

	return rad;
}

void dezalocare(nod* radacina) {
	if (radacina == NULL) {
		return;
	}
	dezalocare(radacina->primulCopil);
	dezalocare(radacina->urmatorulFrate);
	free(radacina);
	radacina = NULL;
}

void afisareArbore(nod* radacina) {
	if (radacina == NULL) {
		return;
	}
	printf("%d\n", radacina->info);
	afisareArbore(radacina->primulCopil);
	afisareArbore(radacina->urmatorulFrate);
}

int main() {
	nod* radacina = creareNod(1);

	adaugareCopil(&radacina, 2);
	adaugareCopil(&radacina, 3);
	adaugareCopil(&radacina, 4);

	afisareArbore(radacina);

	radacina = stergereNod(radacina, 1);

	printf("\n");

	afisareArbore(radacina);

	dezalocare(radacina);

	return 0;
}