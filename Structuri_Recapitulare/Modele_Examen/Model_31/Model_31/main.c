#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct {
	int cod;
	char* nume;
	char* adresa;
	int nrCopii;
	int nrClase;
} Gradinita;

typedef struct {
	Gradinita inf;
	struct nodLS* next;
} nodLS;

typedef struct {
	nodLS** vector;
	int nrElem;
} hashT;

typedef struct {
	Gradinita inf;
	struct nodLD* next;
	struct nodLD* prev;
} nodLD;

int functieHash(hashT tabela, char* nume) {
	return nume[0] % tabela.nrElem;
}

void inserareLS(nodLS** prim, Gradinita grad) {
	nodLS* nou = (nodLS*)malloc(sizeof(nodLS));
	nou->inf.cod = grad.cod;
	nou->inf.nrClase = grad.nrClase;
	nou->inf.nrCopii = grad.nrCopii;
	nou->inf.adresa = (char*)malloc(strlen(grad.adresa) + 1);
	strcpy(nou->inf.adresa, grad.adresa);
	nou->inf.nume = (char*)malloc(strlen(grad.nume) + 1);
	strcpy(nou->inf.nume, grad.nume);
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

void inserareHash(hashT tabela, Gradinita grad) {
	int poz = functieHash(tabela, grad.nume);
	inserareLS(&tabela.vector[poz], grad);
}

void afisareLS(nodLS* prim) {
	nodLS* temp = prim;
	while (temp != NULL) {
		printf("\tCod: %d, Copii: %d, Clase: %d, Adresa: %s, Nume: %s\n",
			temp->inf.cod, temp->inf.nrCopii, temp->inf.nrClase, temp->inf.adresa, temp->inf.nume);
		temp = temp->next;
	}
}

void afisareHash(hashT tabela) {
	for (int i = 0; i < tabela.nrElem; i++) {
		if (tabela.vector[i] != NULL) {
			printf("Pozitia %d:\n", i);
			afisareLS(tabela.vector[i]);
		}
	}
}

void stergereLS(nodLS** prim, char* nume, Gradinita* out) {
	nodLS* prev = NULL;
	nodLS* temp = *prim;
	while (temp != NULL) {
		if (strcmp(temp->inf.nume, nume) == 0) {
			*out = temp->inf;
			if (prev == NULL) {
				*prim = (*prim)->next;
			}
			else {
				prev->next = temp->next;
			}
			temp = temp->next;
		}
		else {
			prev = temp;
			temp = temp->next;
		}
	}
}

void modificareNumeGrad(hashT tabela, char* nume, char* numeNou) {
	Gradinita grad;
	grad.cod = -1;
	
	int poz = functieHash(tabela, nume);
	stergereLS(&tabela.vector[poz], nume, &grad);

	if (grad.cod != -1) {
		free(grad.nume);
		grad.nume = (char*)malloc(strlen(numeNou) + 1);
		strcpy(grad.nume, numeNou);

		inserareHash(tabela, grad);
	}
}

void inserareLD(nodLD** prim, nodLD** ultim, Gradinita grad) {
	nodLD* nou = (nodLD*)malloc(sizeof(nodLD));
	nou->inf.cod = grad.cod;
	nou->inf.nrClase = grad.nrClase;
	nou->inf.nrCopii = grad.nrCopii;
	nou->inf.adresa = (char*)malloc(strlen(grad.adresa) + 1);
	strcpy(nou->inf.adresa, grad.adresa);
	nou->inf.nume = (char*)malloc(strlen(grad.nume) + 1);
	strcpy(nou->inf.nume, grad.nume);
	nou->next = NULL;
	nou->prev = NULL;

	if (*prim == NULL) {
		*prim = nou;
		*ultim = nou;
	}
	else {
		(*ultim)->next = nou;
		nou->prev = *ultim;
		*ultim = nou;
	}
}

void hashT_ld(hashT tabela, nodLD** prim, nodLD** ultim) {
	for (int i = 0; i < tabela.nrElem; i++) {
		if (tabela.vector[i] != NULL) {
			nodLS* temp = tabela.vector[i];
			while (temp != NULL) {
				if (temp->inf.nrCopii > 10) {
					inserareLD(prim, ultim, temp->inf);
				}
				temp = temp->next;
			}
		}
	}
}

void afisareLD(nodLD* prim) {
	nodLD* temp = prim;
	while (temp != NULL) {
		printf("Cod: %d, Copii: %d, Clase: %d, Adresa: %s, Nume: %s\n",
			temp->inf.cod, temp->inf.nrCopii, temp->inf.nrClase, temp->inf.adresa, temp->inf.nume);
		temp = temp->next;
	}
}

void afisareLDInvers(nodLD* ultim) {
	nodLD* temp = ultim;
	while (temp != NULL) {
		printf("Cod: %d, Copii: %d, Clase: %d, Adresa: %s, Nume: %s\n",
			temp->inf.cod, temp->inf.nrCopii, temp->inf.nrClase, temp->inf.adresa, temp->inf.nume);
		temp = temp->prev;
	}
}

void rupereLD(nodLD** prim, nodLD** ultim, nodLD** prim2, nodLD** ultim2, int cod) {
	nodLD* temp = *prim;
	while (temp != NULL && temp->inf.cod != cod) {
		temp = temp->next;
	}

	if (temp->inf.cod == cod) {
		*ultim = temp->prev;
		(*ultim)->next = NULL;

		nodLD* temp2 = temp;
		while (temp2 != NULL) {
			nodLD* aux = temp2->next;
			inserareLD(prim2, ultim2, temp2->inf);
			free(temp2->inf.adresa);
			free(temp2->inf.nume);
			free(temp2);
			temp2 = aux;
		}
	}
}

void dezalocareLS(nodLS* prim) {
	nodLS* temp = prim;
	while (temp != NULL) {
		nodLS* aux = temp->next;
		free(temp->inf.adresa);
		free(temp->inf.nume);
		free(temp);
		temp = aux;
	}
}

void dezalocareHash(hashT tabela) {
	for (int i = 0; i < tabela.nrElem; i++) {
		if (tabela.vector[i] != NULL) {
			dezalocareLS(tabela.vector[i]);
		}
	}
}

void dezalocareLD(nodLD* prim) {
	nodLD* temp = prim;
	while (temp != NULL) {
		nodLD* aux = temp->next;
		free(temp->inf.adresa);
		free(temp->inf.nume);
		free(temp);
		temp = aux;
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

	Gradinita grad;
	char buffer[128];

	while (fscanf(f, "%d", &grad.cod) == 1) 
	{
		fscanf(f, " %[^\n]", buffer);
		grad.nume = (char*)malloc(strlen(buffer) + 1);
		strcpy(grad.nume, buffer);

		fscanf(f, " %[^\n]", buffer);
		grad.adresa = (char*)malloc(strlen(buffer) + 1);
		strcpy(grad.adresa, buffer);

		fscanf(f, "%d", &grad.nrCopii);
		fscanf(f, "%d", &grad.nrClase);

		/*printf("Cod: %d, Copii: %d, Clase: %d, Adresa: %s, Nume: %s\n",
			grad.cod, grad.nrCopii, grad.nrClase, grad.adresa, grad.nume);*/
		inserareHash(tabela, grad);

		free(grad.nume);
		free(grad.adresa);
	}
	fclose(f);

	printf("Tabela de dispersie din fisier:\n");
	afisareHash(tabela);

	//modificareNumeGrad(tabela, "Ileana Cosanzeana", "Cambridge Kindergarden");
	//modificareNumeGrad(tabela, "I. L. Caragiale", "Cambridge Kindergarden");
	modificareNumeGrad(tabela, "Preston Partener Kindergarden", "Cambridge Kindergarden");
	//modificareNumeGrad(tabela, "N/A", "Cambridge Kindergarden");
	printf("\nTabela dupa modificare:\n");
	afisareHash(tabela);

	nodLD* prim = NULL;
	nodLD* ultim = NULL;
	hashT_ld(tabela, &prim, &ultim);
	printf("\nLista dubla:\n");
	afisareLD(prim);

	printf("\nLista dubla invers:\n");
	afisareLDInvers(ultim);

	nodLD* prim2 = NULL;
	nodLD* ultim2 = NULL;

	rupereLD(&prim, &ultim, &prim2, &ultim2, 799);
	printf("\nLista 1:\n");
	afisareLD(prim);

	printf("\nLista 2:\n");
	afisareLD(prim2);

	printf("\nLista 1 invers:\n");
	afisareLDInvers(ultim);

	printf("\nLista 2 invers:\n");
	afisareLDInvers(ultim2);

	dezalocareHash(tabela);
	dezalocareLD(prim);
	dezalocareLD(prim2);

	return 0;
}