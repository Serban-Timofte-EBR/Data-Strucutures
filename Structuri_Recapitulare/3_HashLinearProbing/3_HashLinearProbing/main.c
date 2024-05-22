#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <malloc.h>

typedef struct {
	int cod;
	char* denumire;
	float pret;
} produs;

typedef struct {
	produs inf;
	int ocupat; // 0 = liber		1 = ocupat		-1 = sters
} elementHash;

typedef struct {
	elementHash* vector;
	int nrElem;
} hashT;

int functieHash(int cod, hashT tabela) {
	return cod % tabela.nrElem;
}

void inserareHash(hashT* tabela, produs prod) {
	int poz = functieHash(prod.cod, *tabela);
	
	int i = 0;
	while (tabela->vector[(poz + i) % tabela->nrElem].ocupat == 1)
	{
		i++;
	}

	int index = (poz + i) % tabela->nrElem;
	tabela->vector[index].inf.cod = prod.cod;
	tabela->vector[index].inf.pret = prod.pret;
	tabela->vector[index].inf.denumire = (char*)malloc(strlen(prod.denumire) + 1);
	strcpy(tabela->vector[index].inf.denumire, prod.denumire);
	tabela->vector[index].ocupat = 1;
}

void printareHash(hashT tabela) {
	for (int i = 0; i < tabela.nrElem; i++) {
		if (tabela.vector[i].ocupat == 1) {
			printf("\nPozitie = %d", i);
			printf("\n\tCod = %d, Denumire = %s, Pret = %5.2f\n",
				tabela.vector[i].inf.cod, tabela.vector[i].inf.denumire, tabela.vector[i].inf.pret);
		}
	}
}

produs cautareProdus(hashT tabela, int cod) {
	int poz = functieHash(cod, tabela);
	int i = 0;

	while (tabela.vector[(poz + i) % tabela.nrElem].ocupat != 0)
	{
		if (tabela.vector[(poz + i) % tabela.nrElem].ocupat == 1 &&
			tabela.vector[(poz + i) % tabela.nrElem].inf.cod == cod) {
			return tabela.vector[(poz + i) % tabela.nrElem].inf;
		}
		i++;
	}

	produs prodNegasit = { -1, "N/A", 0.0 };
	return prodNegasit;
}

void stergereProdus(hashT* tabela, int cod) {
	int poz = functieHash(cod, *tabela);
	int i = 0;

	while (tabela->vector[(poz + i) % tabela->nrElem].ocupat != 0)
	{
		if (tabela->vector[(poz + i) % tabela->nrElem].inf.cod == cod &&
				tabela->vector[(poz + i) % tabela->nrElem].ocupat == 1) {
			tabela->vector[(poz + i) % tabela->nrElem].ocupat = -1;
			free(tabela->vector[(poz + i) % tabela->nrElem].inf.denumire);
		}
		i++;
	}
}

void stergereProdusDupaDenumire(hashT* tabela, char* denumire) {
	for (int i = 0; i < tabela->nrElem; i++) {
		if (tabela->vector[i].ocupat == 1
			&& strcmp(tabela->vector[i].inf.denumire, denumire) == 0)
		{
			tabela->vector[i].ocupat = -1;
			free(tabela->vector[i].inf.denumire);
		}
	}
}

int main() {
	hashT tabela;
	tabela.nrElem = 23;
	tabela.vector = (elementHash*)malloc(tabela.nrElem * sizeof(elementHash));
	for (int i = 0; i < tabela.nrElem; i++) {
		tabela.vector[i].ocupat = 0;
	}

	int nrProd;
	char buffer[128];
	produs p;

	FILE* f = fopen("fisier.txt", "r");
	fscanf(f, "%d", &nrProd);
	for (int i = 0; i < nrProd; i++) {
		fscanf(f, "%d", &p.cod);
		fscanf(f, "%s", buffer);
		p.denumire = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(p.denumire, buffer);
		fscanf(f, "%f", &p.pret);

		inserareHash(&tabela, p);

		free(p.denumire);
	}
	fclose(f);

	printf("Tabela prin LINEAR PROBING din fisier:\n");
	printareHash(tabela);

	//cautarea unui produs in tabela de dispersie dupa cod
	produs produsReturn = cautareProdus(tabela, 102);
	if (produsReturn.cod != -1) {
		printf("\nProdusul gasit este:\n");
		printf("\tCod: %d, Denumire: %s, Pret: %f\n",
			produsReturn.cod, produsReturn.denumire, produsReturn.pret);
	}
	else {
		printf("Produsul nu a fost gasit");
	}

	//stergem un produs din tabela dupa cod
	stergereProdus(&tabela, 102);
	printf("\nTabela dupa stergere:\n");
	printareHash(tabela);

	//stergem un produs din tabela dupa denumire
	stergereProdusDupaDenumire(&tabela, "Bere");
	printf("\nTabela dupa stergere:\n");
	printareHash(tabela);

	return 0;
}