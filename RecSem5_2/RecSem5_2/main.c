#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

typedef struct
{
	int isbn;
	char* titlu;
	float pret;
} carte;

typedef struct {
	carte inf;
	struct nodLS* next;
} nodLS;

typedef struct {
	nodLS** vect;
	int nrElem;
} hashT;

void inserareLS(nodLS** cap, carte c) {
	nodLS* nou = (nodLS*)malloc(sizeof(nodLS));
	nou->inf.isbn = c.isbn;
	nou->inf.pret = c.pret;
	nou->inf.titlu = (char*)malloc((strlen(c.titlu) + 1) * sizeof(char));
	strcpy(nou->inf.titlu, c.titlu);
	nou->next = NULL;
	if (*cap == NULL) {
		*cap = nou;
	}
	else {
		nodLS* temp = cap;
		while (temp->next) {
			temp = temp->next;
		}
		temp->next = nou;
	}
}

int functieHash2(char* titlu, hashT tabel) {
	return titlu[0] % tabel.nrElem;
}

void inserareHash(hashT tabel, carte c) {
	int poz = functieHash2(c.titlu, tabel);
	inserareLS(&tabel.vect[poz], c);
}

void traversareLS(nodLS* capLS)
{
	nodLS* temp = capLS;
	while (temp)
	{
		printf("\nISBN = %d, Titlu = %s, Pret = %5.2f",
			temp->inf.isbn, temp->inf.titlu, temp->inf.pret);
		temp = temp->next;
	}
}

void traversareHash(hashT tabel) {
	for (int i = 0; i < tabel.nrElem; i++) {
		if (tabel.vect[i] != NULL) {
			printf("\nPozitia: %d\t\t", i);
			traversareLS(tabel.vect[i]);
		}
	}
}

void dezalocareLS(nodLS* cap) {
	nodLS* temp = cap;
	while (temp) {
		nodLS* temp2 = temp->next;
		free(temp->inf.titlu);
		free(temp);
		temp = temp2;
	}
}

void dezalocareHash(hashT tabel) {
	for (int i = 0; i < tabel.nrElem; i++) {
		if (tabel.vect[i] != NULL) {
			dezalocareLS(tabel.vect[i]);
		}
	}
	free(tabel.vect);
}

int main() {
	//Hash Table este un vector de pointeri catre capetele de lista simple. Functia hash doar indica pozitia
	hashT tabela;
	tabela.nrElem = 23;
	tabela.vect = (nodLS**)malloc(tabela.nrElem * sizeof(nodLS*));
	for (int i = 0; i < tabela.nrElem; i++) {
		tabela.vect[i] = NULL;
	}

	int nrCarti;
	char buffer[30];
	carte c;

	FILE* f = fopen("input.txt", "r");
	if (!f) {
		printf("FILE NOT OPENED!");
		return -1;
	}
	else {
		fscanf(f, "%d", &nrCarti);
		for (int i = 0; i < nrCarti; i++) {
			fscanf(f, "%d", &c.isbn);
			fscanf(f, "%s", buffer);
			c.titlu = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
			strcpy(c.titlu, buffer);
			fscanf(f, "%f", &c.pret);

			inserareHash(tabela, c);
			free(c.titlu);
		}
	}
	fclose(f);

	traversareHash(tabela);
	dezalocareHash(tabela);

	return 0;
}