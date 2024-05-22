#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

// Hash Table prin Chainlink
// Preluare in Lista Simpla

typedef struct {
	unsigned int cod;
	char* titlu;
	unsigned char numarExecutanti;
	float buget;
} Proiect;

typedef struct {
	Proiect inf;
	struct nodLS* next;
} nodLS;

typedef struct {
	nodLS** vector;
	int nrElem;
} hashT;

int functieHash(hashT tabela, char* beneficiar) {
	return beneficiar[0] % tabela.nrElem;
}

void inserareLS(nodLS** cap, Proiect pr) {
	nodLS* nou = (nodLS*)malloc(sizeof(nodLS));
	nou->inf.buget = pr.buget;
	nou->inf.cod = pr.cod;
	nou->inf.numarExecutanti = pr.numarExecutanti;
	nou->inf.titlu = (char*)malloc(strlen(pr.titlu) + 1);
	strcpy(nou->inf.titlu, pr.titlu);
	nou->next = NULL;

	if (*cap == NULL) {
		*cap = nou;
	}
	else {
		nodLS* temp = *cap;
		while (temp->next != NULL)
		{
			temp = temp->next;
		}
		temp->next = nou;
	}
}

void inserareHash(hashT tabela, Proiect pr) {
	int poz = functieHash(tabela, pr.titlu);
	inserareLS(&tabela.vector[poz], pr);
}

void afisareLS(nodLS* prim) {
	nodLS* temp = prim;
	while (temp != NULL)
	{
		printf("\tCod Proiect: %d, Beneficiar: %s\n",
			temp->inf.cod, temp->inf.titlu);
		temp = temp->next;
	}
}

void afisareHash(hashT tabela) {
	for (int i = 0; i < tabela.nrElem; i++)
	{
		if (tabela.vector[i] != NULL) {
			printf("Pozitia %d:\n", i);
			afisareLS(tabela.vector[i]);
		}
	}
}

float bugetTotalBenficiar(hashT tabela, char* beneficiar) {
	float bugetTotal = 0;
	int poz = functieHash(tabela, beneficiar);

	if (tabela.vector[poz] != NULL) {
		nodLS* temp = tabela.vector[poz];
		while (temp != NULL) {
			if (strcmp(temp->inf.titlu, beneficiar) == 0) {
				bugetTotal += temp->inf.buget;
			}
			temp = temp->next;
		}
	}

	return bugetTotal;
}

void modificareBeneficiar(hashT tabela, char* beneficiar, char* nouaDenumire) {
	int poz = functieHash(tabela, beneficiar);

	if (tabela.vector[poz] != NULL) {
		nodLS* temp = tabela.vector[poz];
		nodLS* prev = NULL;
		while (temp != NULL)
		{
			if (strcmp(temp->inf.titlu, beneficiar) == 0) {
				nodLS* nodBeneficiar = temp;
				if (prev == NULL) {
					tabela.vector[poz] = temp->next;
				}
				else if (temp->next == NULL) {
					prev->next = NULL;
				}
				else {
					prev->next = temp->next;
				}
				temp = temp->next;

				free(nodBeneficiar->inf.titlu);
				nodBeneficiar->inf.titlu = (char*)malloc(strlen(nouaDenumire) + 1);
				strcpy(nodBeneficiar->inf.titlu, nouaDenumire);
				inserareHash(tabela, nodBeneficiar->inf);
			}
			else {
				prev = temp;
				temp = temp->next;
			}
		}
	}
}

void proiecteInLS(hashT tabela, nodLS** prim, float prag) {
	for (int i = 0; i < tabela.nrElem; i++) {
		if (tabela.vector[i] != NULL) {
			nodLS* temp = tabela.vector[i];
			while (temp != NULL)
			{
				if (temp->inf.buget > prag) {
					inserareLS(prim, temp->inf);
				}
				temp = temp->next;
			}
		}
	}
}

int main() {
	hashT tabela;
	tabela.nrElem = 23;
	tabela.vector = (nodLS**)malloc(tabela.nrElem * sizeof(nodLS*));
	for (int i = 0; i < tabela.nrElem; i++) {
		tabela.vector[i] = NULL;
	}

	FILE* f = fopen("fisier.txt", "r");
	if (f == NULL) {
		printf("Fisierul nu a fost deschis\n");
		return -1;
	}

	int nrProiecte;
	fscanf(f, "%d", &nrProiecte);

	Proiect pr;
	char buffer[128];

	for (int i = 0; i < nrProiecte; i++) {
		fscanf(f, "%d", &pr.cod);

		fscanf(f, "%s", buffer);
		pr.titlu = (char*)malloc(strlen(buffer) + 1);
		strcpy(pr.titlu, buffer);

		fscanf(f, "%d", &pr.numarExecutanti);
		fscanf(f, "%f", &pr.buget);

		inserareHash(tabela, pr);

		free(pr.titlu);
	}
	fclose(f);

	printf("Tabela din fisier:\n");
	afisareHash(tabela);

	float buget = bugetTotalBenficiar(tabela, "CSIE");
	printf("\nBugetul total al beneficiarului este: %5.2f\n", buget);
	buget = bugetTotalBenficiar(tabela, "Microsoft");
	printf("\nBugetul total al beneficiarului este: %5.2f\n", buget);
	buget = bugetTotalBenficiar(tabela, "MK");
	printf("\nBugetul total al beneficiarului este: %5.2f\n", buget);

	modificareBeneficiar(tabela, "CSIE", "Facultatea de Informatica Economica");
	printf("\nTabela dupa modificare:\n");
	afisareHash(tabela);

	nodLS* primLista5 = NULL;

	proiecteInLS(tabela, &primLista5, 4000);
	printf("\nLista simpla cu proiecte:\n");
	afisareLS(primLista5);

	return 0;
}